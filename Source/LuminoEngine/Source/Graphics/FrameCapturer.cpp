/*
	GIF フォーマット
	https://www.w3.org/Graphics/GIF/spec-gif89a.txt
*/
#include "Internal.h"
#include <gif_lib.h>
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
	virtual void open(const PathName& filePath, const SizeI& size) = 0;
	virtual void close() = 0;
	virtual void addFrame(Bitmap* bitmap, int delayMS, double timestamp) = 0;
};


class GifTaskManager
{
public:
	GifTaskManager()
		: m_maxTasks(8)
	{}

	void setMaxTasks(int count)
	{
		m_maxTasks = count;
	}

	void addTask(const Delegate<void()>& action)
	{
		while (m_tasks.getCount() >= m_maxTasks)
		{
			m_tasks.getFront()->wait();
			m_tasks.removeAt(0);	// TODO: removeFront
		}

		m_tasks.add(tr::Task::run(action));
	}

	void wait()
	{
		for (auto& t : m_tasks)
		{
			t->wait();
		}
		m_tasks.clear();
	}

private:
	int					m_maxTasks;
	List<tr::TaskPtr>	m_tasks;
};

/*
	giflib と jo_gif のハイブリッド実装。
	giflib はイイ感じのリアルタイム減色機能を持っていないので jo_gif の機能を使う。
	jo_gif は lzw のデコードが giflib と比べて遅いので、デコード&ファイル保存は giflib を使う。
	※デコードの速度は 50ms〜150ms vs 20ms くらい。写真のように色数が増えると、jo_gif は遅くなっていく。

	"キーフレーム" : パレットを更新するタイミングのこと。ある程度で実施しないと色が偏る。
*/
class DynamicPaletteGifContext
	: public CapturerContext
{
public:
	struct GifTaskData
	{
		//fcPixelFormat raw_pixel_format = fcPixelFormat_Unknown;
		//Buffer raw_pixels;
		//Buffer rgba8_pixels;
		ByteBuffer	rgba8Pixels;
		jo_gif_frame_t*	gitFrame = nullptr;
		int frame = 0;
		bool local_palette = true;
		double timestamp = 0.0;
		uint32_t	elapsedMSecs;
	};

	DynamicPaletteGifContext()
		: m_keyframeInterval(30)
		, m_colorCount(128)
		, m_loopCount(0)
		, m_keyFrameSpan(300)
		, m_frameCount(0)
		, m_maxTask(8)
		, m_forceKeyframe(false)
	{
	}

	virtual ~DynamicPaletteGifContext()
	{
		close();
	}

	virtual void open(const PathName& filePath, const SizeI& size) override
	{
		m_imageSize = size;
		//m_framePixels.resize(size.width * size.height * 4);	// 4 component. RGBX format, where X is unused
		m_frameCount = 0;

		giflib_openFile(filePath);

		m_joGif = jo_gif_start(size.width, size.height, 0, m_colorCount);

		// ready task buffers
		m_taskManager.setMaxTasks(m_maxTask);
		m_taskDatas.resize(m_maxTask);
		for (auto& data : m_taskDatas)
		{
			// TODO: Pixelformat
			data.rgba8Pixels.resize(size.width * size.height * 4);
			m_taskDatasUnused.add(&data);
		}
	}

	virtual void close() override
	{
		m_taskManager.wait();


		for (int i = 0; i < m_bufferingGitFrames.getCount(); i++)
		{
			giflib_writeFrame(m_bufferingGitFrames[i].get(), i);
		}


		jo_gif_end(&m_joGif);

		giflib_closeFile();
	}

	virtual void addFrame(Bitmap* bitmap, int delayMS, double timestamp) override
	{
		if (delayMS <= 0) return;

		struct RGBX
		{
			byte_t r, g, b, x;
		};

		GifTaskData* data = getTaskData();
		data->timestamp = timestamp;
		data->elapsedMSecs = delayMS;

		// Task の中で jo_gif で減色を行うため、ピクセルフォーマットを変換しつつ作業領域にビットマップデータを取り出す
		const SizeI& bmpSize = bitmap->getSize();
		RGBX* framePixels = (RGBX*)data->rgba8Pixels.getData();
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
					Color32 c = bitmap->getPixel(x, y);
					p->r = c.r;
					p->g = c.g;
					p->b = c.b;
					p->x = 0xFF;
				}
			}
		}


		// kick task
		{
			auto frame = std::make_shared<jo_gif_frame_t>();
			m_bufferingGitFrames.add(frame);

			data->gitFrame = frame.get();
			data->gitFrame->timestamp = data->timestamp;
			data->gitFrame->elapsedMSecs = data->elapsedMSecs;
			data->frame = m_frameCount;
			data->local_palette = false;

			// キーフレームを作るタイミングかを確認
			if (data->frame == 0 || (m_keyframeInterval > 0 && data->frame % m_keyframeInterval == 0) || m_forceKeyframe)
			{
				data->local_palette = true;
				m_forceKeyframe = false;
			}

			if (data->local_palette)
			{
				// キーフレームを作る場合は共有データ (パレット) を変更するため同期的に行う必要がある
				m_taskManager.wait();
				addGifFrame(data);
			}
			else
			{
				m_taskManager.addTask([this, data]()
				{
					addGifFrame(data);
				});
			}

			m_frameCount++;
		}
	}

private:
	GifTaskData* getTaskData()
	{
		GifTaskData* data = nullptr;
		for (;;)
		{
			{
				MutexScopedLock lock(m_taskDatasMutex);
				if (!m_taskDatasUnused.isEmpty())
				{
					data = m_taskDatasUnused.getLast();
					m_taskDatasUnused.removeLast();
					break;
				}
			}

			Thread::sleep(1);
		}
		return data;
	}

	void releaseTaskData(GifTaskData* data)
	{
		MutexScopedLock lock(m_taskDatasMutex);
		m_taskDatasUnused.add(data);
	}

	void addGifFrame(GifTaskData* data)
	{
		// 今 addFrame() で行っている ピクセルフォーマット変換 は、速度が気になるようならここでやってもいい。
		// その場合は GifTaskData にビットマップを持っておく必要があるが。

		// TODO: no const_cast
		unsigned char* src = const_cast<unsigned char*>(data->rgba8Pixels.getConstData());

		// ビットマップデータからインデックスデータと、必要に応じてパレットを作る
		jo_gif_frame(&m_joGif, data->gitFrame, src, data->frame, data->local_palette);

		// DataBuffer 返却
		releaseTaskData(data);
	}

	void giflib_openFile(const StringRef& filePath)
	{
		LN_REQUIRE(m_gif == nullptr);

		StringA fn = StringA::fromNativeCharString(filePath.getBegin(), filePath.getLength());	// TODO: StringRef だけ渡して、getLength とかいらないようにしたい。
		int error = 0;
		m_gif = EGifOpenFileName(fn.c_str(), false, &error);
		LN_ENSURE(m_gif != nullptr, "EGifOpenFileName %d", error);
	}

	void giflib_writeFrame(jo_gif_frame_t* gifFrame, int frameNumber)
	{
		LN_REQUIRE(m_gif != nullptr);

		// 使うパレットを選択する
		ColorMapObject* palette = nullptr;
		if (gifFrame->palette.getSize() != 0)
		{
			// jo_gif のパレットを giflib のパレットに変換
			m_lastPalette.reset(GifMakeMapObject(m_colorCount, (GifColorType*)gifFrame->palette.getConstData()));
			palette = m_lastPalette.get();
		}

		// 初回フレームではいろいろ情報を書き込む必要がある
		if (frameNumber == 0)
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
		ext[1] = (gifFrame->elapsedMSecs / 10) % 256;
		ext[2] = (gifFrame->elapsedMSecs / 10) / 256;
		EGifPutExtension(m_gif, GRAPHICS_EXT_FUNC_CODE, 4, ext);

		// Image Descriptor
		EGifPutImageDesc(m_gif, 0, 0, m_imageSize.width, m_imageSize.height, false, palette);

		// ピクセルインデックス出力
		const byte_t* indices = gifFrame->indexed_pixels.getConstData();
		for (int y = 0; y < m_imageSize.height; ++y)
		{
			EGifPutLine(m_gif, (GifPixelType*)&indices[m_imageSize.width * y], m_imageSize.width);
		}
	}

	void giflib_closeFile()
	{
		LN_REQUIRE(m_gif != nullptr);
		int error = 0;
		int ret = EGifCloseFile(m_gif, &error);
		LN_ENSURE(ret != GIF_OK, "EGifCloseFile %d", error);
	}


	// settings
	int	m_keyframeInterval;
	int								m_maxTask;


	int								m_colorCount;
	int								m_loopCount;		// 0 で無限ループ
	int								m_keyFrameSpan;		// パレットを更新するフレーム数間隔


	jo_gif_t						m_joGif;
	SizeI							m_imageSize;
	//ByteBuffer						m_framePixels;
	Ref<BinaryWriter>			m_writer;
	int								m_frameCount;
	//jo_gif_frame_t					m_gifFrame;			// 1フレームの作業領域。前のフレームを保存するなら配列化する

	GifTaskManager					m_taskManager;
	List<GifTaskData>				m_taskDatas;
	List<GifTaskData*>				m_taskDatasUnused;
	Mutex							m_taskDatasMutex;
	List<std::shared_ptr<jo_gif_frame_t>>	m_bufferingGitFrames;
	bool							m_forceKeyframe;

	// giflib 関係
	GifFileType*					m_gif;				// gifファイルを作るための giflib コンテキスト
	std::shared_ptr<ColorMapObject>	m_lastPalette;
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
		close();
	}

	virtual void open(const PathName& filePath, const SizeI& size) override
	{
		m_imageSize = size;
		m_line.alloc(sizeof(GifPixelType) * m_imageSize.width);

		StringA f = StringA::fromNativeCharString(filePath);
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

	virtual void close() override
	{
		int error;
		EGifCloseFile(m_gif, &error);

		GifFreeMapObject(m_globalPalette);
	}

	virtual void addFrame(Bitmap* bitmap, int delayMS, double timestamp) override
	{
		if (delayMS <= 0) return;

		// Graphic Control Extension
		byte_t ext[4] = { 0x04, 0x00, 0x00, 0xff };
		ext[1] = (delayMS / 10) % 256;
		ext[2] = (delayMS / 10) / 256;
		EGifPutExtension(m_gif, GRAPHICS_EXT_FUNC_CODE, 4, ext);

		// Image Descriptor
		EGifPutImageDesc(m_gif, 0, 0, m_imageSize.width, m_imageSize.height, false, nullptr);

		const SizeI& bmpSize = bitmap->getSize();
		GifPixelType* line = (GifPixelType*)m_line.getData();
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
					*p = PeekR4G2B4Index(bitmap->getPixel(x, y));
				}
			}

			EGifPutLine(m_gif, line, m_imageSize.width);
		}
	}

	uint8_t PeekR4G2B4Index(const Color32& c)
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
	SizeI			m_imageSize;
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
FrameCapturerPtr FrameCapturer::create()
{
	auto ptr = FrameCapturerPtr::makeRef();
	ptr->initialize(detail::GraphicsManager::getInstance());
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
void FrameCapturer::initialize(detail::GraphicsManager* manager)
{
	m_manager = manager;
	m_gifContext = LN_NEW detail::DynamicPaletteGifContext();//;GifContext

	setCapturerTarget(m_manager->getMainSwapChain()->getBackBuffer());
}

//------------------------------------------------------------------------------
void FrameCapturer::setCapturerTarget(RenderTargetTexture* renderTarget)
{
	LN_REFOBJ_SET(m_capturerTarget, renderTarget);
}

//------------------------------------------------------------------------------
void FrameCapturer::startRecording()
{
	m_requestedState = State::Recording;
}

//------------------------------------------------------------------------------
void FrameCapturer::stopRecording()
{
	m_requestedState = State::Stoped;
}

//------------------------------------------------------------------------------
void FrameCapturer::record()
{
	if (LN_CHECK_STATE(m_capturerTarget != nullptr)) return;


	Ref<FrameCapturer> fc = this;
	Driver::ITexture* target = m_capturerTarget->resolveDeviceObject();
	State newState = m_requestedState;

	LN_ENQUEUE_RENDER_COMMAND_3(
		FrameCapturer_Record, m_manager,
		Ref<FrameCapturer>, fc,
		Ref<Driver::ITexture>, target,
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
			m_gifContext->close();
		}
		else if (newState == State::Recording)
		{
			PathName filePath(_T("FrameCapturer.gif"));
			m_gifContext->open(filePath, target->getSize());
			m_lastTick = 0;
		}
		m_currentState = newState;
	}

	// 録画
	if (m_currentState == State::Recording)
	{
		// 差分時間計算
		uint64_t deltaTick = 0;
		uint64_t curTick = Environment::getTickCount();
		if (m_lastTick != 0) deltaTick = curTick - m_lastTick;

		if (m_lastTick == 0 || deltaTick > 64)	// FPS15 くらいでプロットする場合はコレ (TODO: fps指定)
		{
			// RenderTargetTexture の内容を読み取る
			Bitmap* bmp = target->lock();	//TODO: Scoped
			m_gifContext->addFrame(bmp, deltaTick, (double)curTick / 1000000);
			target->unlock();

			m_lastTick = curTick;
		}
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
