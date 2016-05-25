/*
	GIF フォーマット
	https://www.w3.org/Graphics/GIF/spec-gif89a.txt
*/
#include "Internal.h"
#include <giflib-5.1.4\gif_lib.h>
#include "../../../external/jo_gif/jo_gif.cpp"
#include <Lumino/Threading/Task.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/FrameCapturer.h>
#include "GraphicsManager.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{
class CapturerContext
{
public:
	virtual ~CapturerContext() = default;
	virtual void Open(const PathName& filePath, const Size& size) = 0;
	virtual void Close() = 0;
	virtual void AddFrame(Bitmap* bitmap, int delayMS) = 0;
};

/*
	giflib と jo_gif のハイブリッド実装。
	giflib はイイ感じのリアルタイム減色機能を持っていないので jo_gif の機能を使う。
	jo_gif は lzw のデコードが giflib と比べて遅いので、デコード&ファイル保存は giflib を使う。
	※速度は 50ms～150ms vs 20ms くらい。写真のように色数が増えると、jo_gif は遅くなっていく。
*/
class DynamicPaletteGifContext
	: public CapturerContext
{
public:

	DynamicPaletteGifContext()
		: m_colorCount(128)
		, m_loopCount(0)
		, m_keyFrameSpan(300)
		, m_frameCount(0)
	{
	}

	virtual ~DynamicPaletteGifContext()
	{
		Close();
	}

	virtual void Open(const PathName& filePath, const Size& size) override
	{
		m_imageSize = size;
		m_framePixels.Resize(size.width * size.height * 4);	// 4 component. RGBX format, where X is unused
		m_frameCount = 0;

		StringA fn = StringA::FromNativeCharString(filePath);
		int error;
		m_gif = EGifOpenFileName(fn.c_str(), false, &error);

		m_joGif = jo_gif_start(size.width, size.height, 0, m_colorCount);

	}

	virtual void Close() override
	{
		if (m_task != nullptr)
		{
			m_task->Wait();
			m_task.SafeRelease();
		}
		jo_gif_end(&m_joGif);

		int error;
		EGifCloseFile(m_gif, &error);
	}

	virtual void AddFrame(Bitmap* bitmap, int delayMS) override
	{
		if (delayMS <= 0) return;

		struct RGBX
		{
			byte_t r, g, b, x;
		};

		if (m_task == nullptr || m_task->IsCompleted())
		{
			// Task の中で jo_gif で減色を行うため、ピクセルフォーマットを変換しつつ作業領域にビットマップデータを取り出す
			const Size& bmpSize = bitmap->GetSize();
			RGBX* framePixels = (RGBX*)m_framePixels.GetData();
			for (int y = 0; y < m_imageSize.height; ++y)
			{
				for (int x = 0; x < m_imageSize.width; ++x)
				{
					RGBX* p = &framePixels[y * m_imageSize.width + x];
					if (x >= bmpSize.width || y >= bmpSize.height)
					{
						p->r = 0x00;
						p->g = 0x00;
						p->b = 0x00;
						p->x = 0xFF;
					}
					else
					{
						Color c = bitmap->GetPixel(x, y);
						p->r = c.r;
						p->g = c.g;
						p->b = c.b;
						p->x = 0xFF;
					}
				}
			}

			// この中は別スレッドで実行される
			auto func = [this, delayMS]()
			{
				bool updatePalette = (m_frameCount % m_keyFrameSpan == 0);

				// ビットマップデータからインデックスデータと、必要に応じてパレットを作る
				jo_gif_frame(&m_joGif, &m_gifFrame, (unsigned char*)m_framePixels.GetConstData(), m_frameCount, updatePalette);

				// 使うパレットを選択する
				ColorMapObject* palette = nullptr;
				if (updatePalette)
				{
					// jo_gif のパレットを giflib のパレットに変換
					m_lastPalette.reset(GifMakeMapObject(m_colorCount, (GifColorType*)m_gifFrame.palette.GetConstData()));
					palette = m_lastPalette.get();
				}

				// 初回フレームではいろいろ情報を書き込む必要がある
				if (m_frameCount == 0)
				{
					// Logical Screen Descriptor (全体のサイズとグローバルパレット)
					EGifPutScreenDesc(m_gif, m_imageSize.width, m_imageSize.height, m_colorCount, 0, palette);

					// Netscape Extension (ループ回数)
					EGifPutExtensionLeader(m_gif, APPLICATION_EXT_FUNC_CODE);
					EGifPutExtensionBlock(m_gif, 11, "NETSCAPE2.0");	// 固定
					char subblock[3];
					subblock[0] = 1;
					subblock[2] = m_loopCount % 256;
					subblock[1] = m_loopCount / 256;
					EGifPutExtensionRaw(m_gif, 3, subblock);
					EGifPutExtensionTrailer(m_gif);
				}

				// Graphic Control Extension
				byte_t ext[4] = { 0x04, 0x00, 0x00, 0xff };
				ext[1] = (delayMS / 10) % 256;
				ext[2] = (delayMS / 10) / 256;
				EGifPutExtension(m_gif, GRAPHICS_EXT_FUNC_CODE, 4, ext);

				// Image Descriptor
				EGifPutImageDesc(m_gif, 0, 0, m_imageSize.width, m_imageSize.height, false, palette);
				
				// ピクセルインデックス出力
				const byte_t* indices = m_gifFrame.indexed_pixels.GetConstData();
				for (int y = 0; y < m_imageSize.height; ++y)
				{
					EGifPutLine(m_gif, (GifPixelType*)&indices[m_imageSize.width * y], m_imageSize.width);
				}

				++m_frameCount;
			};
			m_task = tr::Task::Run(Delegate<void()>(func));
		}
	}

private:
	int								m_colorCount;
	int								m_loopCount;		// 0 で無限ループ
	int								m_keyFrameSpan;		// パレットを更新するフレーム数間隔

	GifFileType*					m_gif;				// gifファイルを作るための giflib コンテキスト
	std::shared_ptr<ColorMapObject>	m_lastPalette;

	jo_gif_t						m_joGif;
	Size							m_imageSize;
	ByteBuffer						m_framePixels;
	RefPtr<BinaryWriter>			m_writer;
	int								m_frameCount;
	jo_gif_frame_t					m_gifFrame;			// 1フレームの作業領域。前のフレームを保存するなら配列化する
	tr::TaskPtr						m_task;
};

/*
	調査用に作ったもの。定義済みのパレットへ減色する。
	非情に高速に動作するが、画質はお察し。
*/
class GifContext
	: public CapturerContext
{
public:

	GifContext()
		: m_gif(nullptr)
		, m_globalPalette(nullptr)
	{
	}

	virtual ~GifContext()
	{
		Close();
	}

	virtual void Open(const PathName& filePath, const Size& size) override
	{
		m_imageSize = size;
		m_line.Alloc(sizeof(GifPixelType) * m_imageSize.width);

		StringA f = StringA::FromNativeCharString(filePath);
		int error;
		m_gif = EGifOpenFileName(f.c_str(), false, &error);

		m_globalPalette = GifMakeMapObject(256, PaletteGPriority);

		EGifPutScreenDesc(m_gif, size.width, size.height, 256, 0, m_globalPalette);

		EGifPutExtensionLeader(m_gif, APPLICATION_EXT_FUNC_CODE);
		EGifPutExtensionBlock(m_gif, 11, "NETSCAPE2.0");	// 固定

		// Data Sub-blocks
		int loop_count = 0;
		char subblock[3];
		subblock[0] = 1;
		subblock[2] = loop_count % 256;
		subblock[1] = loop_count / 256;
		EGifPutExtensionRaw(m_gif, 3, subblock);

		EGifPutExtensionTrailer(m_gif);
	}

	virtual void Close() override
	{
		int error;
		EGifCloseFile(m_gif, &error);

		GifFreeMapObject(m_globalPalette);
	}

	virtual void AddFrame(Bitmap* bitmap, int delayMS) override
	{
		if (delayMS <= 0) return;

		// Graphic Control Extension
		byte_t ext[4] = { 0x04, 0x00, 0x00, 0xff };
		ext[1] = (delayMS / 10) % 256;
		ext[2] = (delayMS / 10) / 256;
		EGifPutExtension(m_gif, GRAPHICS_EXT_FUNC_CODE, 4, ext);

		// Image Descriptor
		EGifPutImageDesc(m_gif, 0, 0, m_imageSize.width, m_imageSize.height, false, nullptr);

		const Size& bmpSize = bitmap->GetSize();
		GifPixelType* line = (GifPixelType*)m_line.GetData();
		for (int y = 0; y < m_imageSize.height; ++y)
		{
			for (int x = 0; x < m_imageSize.width; ++x)
			{
				GifPixelType* p = &line[x];
				if (x >= bmpSize.width || y >= bmpSize.height)
				{
					*p = 0x00;
				}
				else
				{
					*p = PeekR4G2B4Index(bitmap->GetPixel(x, y));
				}
			}

			EGifPutLine(m_gif, line, m_imageSize.width);
		}
	}

	uint8_t PeekR4G2B4Index(const Color& c)
	{
		return
			((c.r / 32) & 0x0F) |
			(((c.g / 64) & 0x03) << 3) |
			(((c.b / 32) & 0x0F) << 5);
	}

	static const GifColorType PaletteGPriority[256];

private:
	GifFileType*	m_gif;
	ColorMapObject*	m_globalPalette;
	Size			m_imageSize;
	ByteBuffer		m_line;
};

const GifColorType GifContext::PaletteGPriority[256] =
{
	{ 0x00, 0x00, 0x00 },{ 0x1E, 0x00, 0x00 },{ 0x3C, 0x00, 0x00 },{ 0x5A, 0x00, 0x00 },{ 0x78, 0x00, 0x00 },{ 0x96, 0x00, 0x00 },{ 0xB4, 0x00, 0x00 },{ 0xFF, 0x00, 0x00 },
	{ 0x00, 0x3E, 0x00 },{ 0x1E, 0x3E, 0x00 },{ 0x3C, 0x3E, 0x00 },{ 0x5A, 0x3E, 0x00 },{ 0x78, 0x3E, 0x00 },{ 0x96, 0x3E, 0x00 },{ 0xB4, 0x3E, 0x00 },{ 0xFF, 0x3E, 0x00 },
	{ 0x00, 0x7C, 0x00 },{ 0x1E, 0x7C, 0x00 },{ 0x3C, 0x7C, 0x00 },{ 0x5A, 0x7C, 0x00 },{ 0x78, 0x7C, 0x00 },{ 0x96, 0x7C, 0x00 },{ 0xB4, 0x7C, 0x00 },{ 0xFF, 0x7C, 0x00 },
	{ 0x00, 0xFF, 0x00 },{ 0x1E, 0xFF, 0x00 },{ 0x3C, 0xFF, 0x00 },{ 0x5A, 0xFF, 0x00 },{ 0x78, 0xFF, 0x00 },{ 0x96, 0xFF, 0x00 },{ 0xB4, 0xFF, 0x00 },{ 0xFF, 0xFF, 0x00 },
	{ 0x00, 0x00, 0x1E },{ 0x1E, 0x00, 0x1E },{ 0x3C, 0x00, 0x1E },{ 0x5A, 0x00, 0x1E },{ 0x78, 0x00, 0x1E },{ 0x96, 0x00, 0x1E },{ 0xB4, 0x00, 0x1E },{ 0xFF, 0x00, 0x1E },
	{ 0x00, 0x3E, 0x1E },{ 0x1E, 0x3E, 0x1E },{ 0x3C, 0x3E, 0x1E },{ 0x5A, 0x3E, 0x1E },{ 0x78, 0x3E, 0x1E },{ 0x96, 0x3E, 0x1E },{ 0xB4, 0x3E, 0x1E },{ 0xFF, 0x3E, 0x1E },
	{ 0x00, 0x7C, 0x1E },{ 0x1E, 0x7C, 0x1E },{ 0x3C, 0x7C, 0x1E },{ 0x5A, 0x7C, 0x1E },{ 0x78, 0x7C, 0x1E },{ 0x96, 0x7C, 0x1E },{ 0xB4, 0x7C, 0x1E },{ 0xFF, 0x7C, 0x1E },
	{ 0x00, 0xFF, 0x1E },{ 0x1E, 0xFF, 0x1E },{ 0x3C, 0xFF, 0x1E },{ 0x5A, 0xFF, 0x1E },{ 0x78, 0xFF, 0x1E },{ 0x96, 0xFF, 0x1E },{ 0xB4, 0xFF, 0x1E },{ 0xFF, 0xFF, 0x1E },
	{ 0x00, 0x00, 0x3C },{ 0x1E, 0x00, 0x3C },{ 0x3C, 0x00, 0x3C },{ 0x5A, 0x00, 0x3C },{ 0x78, 0x00, 0x3C },{ 0x96, 0x00, 0x3C },{ 0xB4, 0x00, 0x3C },{ 0xFF, 0x00, 0x3C },
	{ 0x00, 0x3E, 0x3C },{ 0x1E, 0x3E, 0x3C },{ 0x3C, 0x3E, 0x3C },{ 0x5A, 0x3E, 0x3C },{ 0x78, 0x3E, 0x3C },{ 0x96, 0x3E, 0x3C },{ 0xB4, 0x3E, 0x3C },{ 0xFF, 0x3E, 0x3C },
	{ 0x00, 0x7C, 0x3C },{ 0x1E, 0x7C, 0x3C },{ 0x3C, 0x7C, 0x3C },{ 0x5A, 0x7C, 0x3C },{ 0x78, 0x7C, 0x3C },{ 0x96, 0x7C, 0x3C },{ 0xB4, 0x7C, 0x3C },{ 0xFF, 0x7C, 0x3C },
	{ 0x00, 0xFF, 0x3C },{ 0x1E, 0xFF, 0x3C },{ 0x3C, 0xFF, 0x3C },{ 0x5A, 0xFF, 0x3C },{ 0x78, 0xFF, 0x3C },{ 0x96, 0xFF, 0x3C },{ 0xB4, 0xFF, 0x3C },{ 0xFF, 0xFF, 0x3C },
	{ 0x00, 0x00, 0x5A },{ 0x1E, 0x00, 0x5A },{ 0x3C, 0x00, 0x5A },{ 0x5A, 0x00, 0x5A },{ 0x78, 0x00, 0x5A },{ 0x96, 0x00, 0x5A },{ 0xB4, 0x00, 0x5A },{ 0xFF, 0x00, 0x5A },
	{ 0x00, 0x3E, 0x5A },{ 0x1E, 0x3E, 0x5A },{ 0x3C, 0x3E, 0x5A },{ 0x5A, 0x3E, 0x5A },{ 0x78, 0x3E, 0x5A },{ 0x96, 0x3E, 0x5A },{ 0xB4, 0x3E, 0x5A },{ 0xFF, 0x3E, 0x5A },
	{ 0x00, 0x7C, 0x5A },{ 0x1E, 0x7C, 0x5A },{ 0x3C, 0x7C, 0x5A },{ 0x5A, 0x7C, 0x5A },{ 0x78, 0x7C, 0x5A },{ 0x96, 0x7C, 0x5A },{ 0xB4, 0x7C, 0x5A },{ 0xFF, 0x7C, 0x5A },
	{ 0x00, 0xFF, 0x5A },{ 0x1E, 0xFF, 0x5A },{ 0x3C, 0xFF, 0x5A },{ 0x5A, 0xFF, 0x5A },{ 0x78, 0xFF, 0x5A },{ 0x96, 0xFF, 0x5A },{ 0xB4, 0xFF, 0x5A },{ 0xFF, 0xFF, 0x5A },
	{ 0x00, 0x00, 0x78 },{ 0x1E, 0x00, 0x78 },{ 0x3C, 0x00, 0x78 },{ 0x5A, 0x00, 0x78 },{ 0x78, 0x00, 0x78 },{ 0x96, 0x00, 0x78 },{ 0xB4, 0x00, 0x78 },{ 0xFF, 0x00, 0x78 },
	{ 0x00, 0x3E, 0x78 },{ 0x1E, 0x3E, 0x78 },{ 0x3C, 0x3E, 0x78 },{ 0x5A, 0x3E, 0x78 },{ 0x78, 0x3E, 0x78 },{ 0x96, 0x3E, 0x78 },{ 0xB4, 0x3E, 0x78 },{ 0xFF, 0x3E, 0x78 },
	{ 0x00, 0x7C, 0x78 },{ 0x1E, 0x7C, 0x78 },{ 0x3C, 0x7C, 0x78 },{ 0x5A, 0x7C, 0x78 },{ 0x78, 0x7C, 0x78 },{ 0x96, 0x7C, 0x78 },{ 0xB4, 0x7C, 0x78 },{ 0xFF, 0x7C, 0x78 },
	{ 0x00, 0xFF, 0x78 },{ 0x1E, 0xFF, 0x78 },{ 0x3C, 0xFF, 0x78 },{ 0x5A, 0xFF, 0x78 },{ 0x78, 0xFF, 0x78 },{ 0x96, 0xFF, 0x78 },{ 0xB4, 0xFF, 0x78 },{ 0xFF, 0xFF, 0x78 },
	{ 0x00, 0x00, 0x96 },{ 0x1E, 0x00, 0x96 },{ 0x3C, 0x00, 0x96 },{ 0x5A, 0x00, 0x96 },{ 0x78, 0x00, 0x96 },{ 0x96, 0x00, 0x96 },{ 0xB4, 0x00, 0x96 },{ 0xFF, 0x00, 0x96 },
	{ 0x00, 0x3E, 0x96 },{ 0x1E, 0x3E, 0x96 },{ 0x3C, 0x3E, 0x96 },{ 0x5A, 0x3E, 0x96 },{ 0x78, 0x3E, 0x96 },{ 0x96, 0x3E, 0x96 },{ 0xB4, 0x3E, 0x96 },{ 0xFF, 0x3E, 0x96 },
	{ 0x00, 0x7C, 0x96 },{ 0x1E, 0x7C, 0x96 },{ 0x3C, 0x7C, 0x96 },{ 0x5A, 0x7C, 0x96 },{ 0x78, 0x7C, 0x96 },{ 0x96, 0x7C, 0x96 },{ 0xB4, 0x7C, 0x96 },{ 0xFF, 0x7C, 0x96 },
	{ 0x00, 0xFF, 0x96 },{ 0x1E, 0xFF, 0x96 },{ 0x3C, 0xFF, 0x96 },{ 0x5A, 0xFF, 0x96 },{ 0x78, 0xFF, 0x96 },{ 0x96, 0xFF, 0x96 },{ 0xB4, 0xFF, 0x96 },{ 0xFF, 0xFF, 0x96 },
	{ 0x00, 0x00, 0xB4 },{ 0x1E, 0x00, 0xB4 },{ 0x3C, 0x00, 0xB4 },{ 0x5A, 0x00, 0xB4 },{ 0x78, 0x00, 0xB4 },{ 0x96, 0x00, 0xB4 },{ 0xB4, 0x00, 0xB4 },{ 0xFF, 0x00, 0xB4 },
	{ 0x00, 0x3E, 0xB4 },{ 0x1E, 0x3E, 0xB4 },{ 0x3C, 0x3E, 0xB4 },{ 0x5A, 0x3E, 0xB4 },{ 0x78, 0x3E, 0xB4 },{ 0x96, 0x3E, 0xB4 },{ 0xB4, 0x3E, 0xB4 },{ 0xFF, 0x3E, 0xB4 },
	{ 0x00, 0x7C, 0xB4 },{ 0x1E, 0x7C, 0xB4 },{ 0x3C, 0x7C, 0xB4 },{ 0x5A, 0x7C, 0xB4 },{ 0x78, 0x7C, 0xB4 },{ 0x96, 0x7C, 0xB4 },{ 0xB4, 0x7C, 0xB4 },{ 0xFF, 0x7C, 0xB4 },
	{ 0x00, 0xFF, 0xB4 },{ 0x1E, 0xFF, 0xB4 },{ 0x3C, 0xFF, 0xB4 },{ 0x5A, 0xFF, 0xB4 },{ 0x78, 0xFF, 0xB4 },{ 0x96, 0xFF, 0xB4 },{ 0xB4, 0xFF, 0xB4 },{ 0xFF, 0xFF, 0xB4 },
	{ 0x00, 0x00, 0xFF },{ 0x1E, 0x00, 0xFF },{ 0x3C, 0x00, 0xFF },{ 0x5A, 0x00, 0xFF },{ 0x78, 0x00, 0xFF },{ 0x96, 0x00, 0xFF },{ 0xB4, 0x00, 0xFF },{ 0xFF, 0x00, 0xFF },
	{ 0x00, 0x3E, 0xFF },{ 0x1E, 0x3E, 0xFF },{ 0x3C, 0x3E, 0xFF },{ 0x5A, 0x3E, 0xFF },{ 0x78, 0x3E, 0xFF },{ 0x96, 0x3E, 0xFF },{ 0xB4, 0x3E, 0xFF },{ 0xFF, 0x3E, 0xFF },
	{ 0x00, 0x7C, 0xFF },{ 0x1E, 0x7C, 0xFF },{ 0x3C, 0x7C, 0xFF },{ 0x5A, 0x7C, 0xFF },{ 0x78, 0x7C, 0xFF },{ 0x96, 0x7C, 0xFF },{ 0xB4, 0x7C, 0xFF },{ 0xFF, 0x7C, 0xFF },
	{ 0x00, 0xFF, 0xFF },{ 0x1E, 0xFF, 0xFF },{ 0x3C, 0xFF, 0xFF },{ 0x5A, 0xFF, 0xFF },{ 0x78, 0xFF, 0xFF },{ 0x96, 0xFF, 0xFF },{ 0xB4, 0xFF, 0xFF },{ 0xFF, 0xFF, 0xFF },
};

} // namespace detail

//==============================================================================
// FrameCapturer
//==============================================================================

//------------------------------------------------------------------------------
FrameCapturerPtr FrameCapturer::Create()
{
	auto ptr = FrameCapturerPtr::MakeRef();
	ptr->Initialize(GraphicsManager::GetInstance());
	return ptr;
}

//------------------------------------------------------------------------------
FrameCapturer::FrameCapturer()
	: m_manager(nullptr)
	, m_capturerTarget(nullptr)
	, m_requestedState(State::Stoped)
	, m_gifContext(nullptr)
	, m_currentState(State::Stoped)
	, m_lastTick(0)
{
}

//------------------------------------------------------------------------------
FrameCapturer::~FrameCapturer()
{
	LN_SAFE_RELEASE(m_capturerTarget);
	LN_SAFE_DELETE(m_gifContext);
}

//------------------------------------------------------------------------------
void FrameCapturer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_gifContext = LN_NEW detail::DynamicPaletteGifContext();//;GifContext
}

//------------------------------------------------------------------------------
void FrameCapturer::SetCapturerTarget(RenderTarget* renderTarget)
{
	LN_REFOBJ_SET(m_capturerTarget, renderTarget);
}

//------------------------------------------------------------------------------
void FrameCapturer::StartRecording()
{
	m_requestedState = State::Recording;
}

//------------------------------------------------------------------------------
void FrameCapturer::StopRecording()
{
	m_requestedState = State::Stoped;
}

//------------------------------------------------------------------------------
void FrameCapturer::Record()
{
	LN_CHECK_STATE(m_capturerTarget != nullptr);


	RefPtr<FrameCapturer> fc = this;
	Driver::ITexture* target = m_capturerTarget->GetDeviceObject();
	State newState = m_requestedState;

	LN_ENQUEUE_RENDER_COMMAND_3(
		FrameCapturer_Record, m_manager,
		RefPtr<FrameCapturer>, fc,
		RefPtr<Driver::ITexture>, target,
		State, newState,
		{
			fc->RecordCommand(target, newState);
		});
}

//------------------------------------------------------------------------------
void FrameCapturer::RecordCommand(Driver::ITexture* target, State newState)
{
	// 録画状態の変化確認
	if (newState != m_currentState)
	{
		if (newState == State::Stoped)
		{
			m_gifContext->Close();
		}
		else if (newState == State::Recording)
		{
			PathName filePath(_T("FrameCapturer.gif"));
			m_gifContext->Open(filePath, target->GetSize());
			m_lastTick = 0;
		}
		m_currentState = newState;
	}

	// 録画
	if (m_currentState == State::Recording)
	{
		// 差分時間計算
		uint64_t deltaTick = 0;
		uint64_t curTick = Environment::GetTickCount();
		if (m_lastTick != 0) deltaTick = curTick - m_lastTick;

		if (m_lastTick == 0 || deltaTick > 64)	// FPS15 くらいでプロットする場合はコレ (TODO: fps指定)
		{
			// RenderTarget の内容を読み取る
			Bitmap* bmp = target->Lock();	//TODO: Scoped
			m_gifContext->AddFrame(bmp, deltaTick);
			target->Unlock();

			m_lastTick = curTick;
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
