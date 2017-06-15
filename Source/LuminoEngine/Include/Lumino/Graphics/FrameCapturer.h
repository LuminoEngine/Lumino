
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class CapturerContext; }
class RenderTargetTexture;

class FrameCapturer;
using FrameCapturerPtr = RefPtr<FrameCapturer>;

class FrameCapturer
	: public Object
{
public:
	static FrameCapturerPtr create();

public:
	void setCapturerTarget(RenderTargetTexture* renderTarget);
	void startRecording();
	void stopRecording();
	void record();

LN_INTERNAL_ACCESS:
	FrameCapturer();
	virtual ~FrameCapturer();
	void initialize(detail::GraphicsManager* manager);

private:
	enum class State
	{
		Stoped,
		Recording,
	};

	void RecordCommand(Driver::ITexture* target, State newState);

	detail::GraphicsManager*	m_manager;
	RenderTargetTexture*		m_capturerTarget;
	State						m_requestedState;
	
	PathNameA					m_savingDir;
	detail::CapturerContext*	m_gifContext;
	State						m_currentState;
	uint64_t					m_lastTick;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
