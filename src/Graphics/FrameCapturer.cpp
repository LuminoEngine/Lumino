/*
	GIF フォーマット
	https://www.w3.org/Graphics/GIF/spec-gif87.txt
*/
#include "Internal.h"
#include "../../../external/jo_gif/jo_gif.cpp"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/FrameCapturer.h>
#include "GraphicsManager.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// FrameCapturer
//==============================================================================

struct FrameCapturer::GifContext
{
	jo_gif_t	gif;
	ByteBuffer	framePixels;
	Size		frameSize;
	int			frameCount = 0;
};

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

	if (m_gifContext != nullptr)
	{
		jo_gif_write_footer(m_stream.get(), &m_gifContext->gif);
		jo_gif_end(&m_gifContext->gif);
	}
	LN_SAFE_DELETE(m_gifContext);
}

//------------------------------------------------------------------------------
void FrameCapturer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_gifContext = LN_NEW GifContext;
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
			jo_gif_write_footer(m_stream.get(), &m_gifContext->gif);
			jo_gif_end(&m_gifContext->gif);
			m_stream = nullptr;//.SafeRelease();
		}
		else if (newState == State::Recording)
		{
			PathName filePath("FrameCapturer.gif");
			m_stream = std::make_shared<BinaryWriter>(FileStream::Create(filePath, FileOpenMode::Write | FileOpenMode::Truncate));//.Attach(LN_NEW BinaryWriter(FileStream::Create(filePath, FileOpenMode::Write | FileOpenMode::Truncate)), false);

			const Size& size = target->GetSize();
			m_gifContext->gif = jo_gif_start(size.width, size.height, 0, 256);
			m_gifContext->framePixels.Resize(size.width * size.height * 4);	// 4 component. RGBX format, where X is unused
			m_gifContext->frameSize = size;
			m_lastTick = 0;

			jo_gif_write_header(m_stream.get(), &m_gifContext->gif);
		}
		m_currentState = newState;
	}

	// 録画
	if (m_currentState == State::Recording)
	{
		struct RGBX
		{
			byte_t r, g, b, x;
		};

		// RenderTarget の内容を読み取る
		Bitmap* bmp = target->Lock();
		const Size& bmpSize = bmp->GetSize();
		RGBX* frame = (RGBX*)m_gifContext->framePixels.GetData();
		for (int y = 0; y < m_gifContext->frameSize.height; ++y)
		{
			for (int x = 0; x < m_gifContext->frameSize.width; ++x)
			{
				RGBX* p = &frame[y * m_gifContext->frameSize.width + x];
				if (x >= bmpSize.width || y >= bmpSize.height)
				{
					p->r = 0x00;
					p->g = 0x00;
					p->b = 0x00;
					p->x = 0xFF;
				}
				else
				{
					Color c = bmp->GetPixel(x, y);
					p->r = c.r;
					p->g = c.g;
					p->b = c.b;
					p->x = 0xFF;
				}
			}
		}
		target->Unlock();

		// 差分時間計算
		uint64_t deltaTick = 0;
		uint64_t curTick = Environment::GetTickCount();
		if (m_lastTick != 0) deltaTick = curTick - m_lastTick;

		// 書き込み
		bool local_palette = (m_gifContext->frameCount == 0);
		jo_gif_frame_t gifFrame;
		jo_gif_frame(&m_gifContext->gif, &gifFrame, (unsigned char*)frame, m_gifContext->frameCount, local_palette);
		//jo_gif_frame(&m_gifContext->gif, (unsigned char*)frame, deltaTick / 10, false);	// milliseconds to centi-seconds
        jo_gif_write_frame(m_stream.get(), &m_gifContext->gif, &gifFrame, nullptr, m_gifContext->frameCount++, deltaTick / 10);

		m_lastTick = curTick;
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
