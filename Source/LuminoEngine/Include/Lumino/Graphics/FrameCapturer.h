
#pragma once

LN_NAMESPACE_BEGIN
namespace detail { class GraphicsManager; }
namespace detail { class CapturerContext; }
namespace Driver { class ITexture; }
class RenderTargetTexture;
class SwapChain;

/**
	@brief	
*/
class FrameCapturer
{
public:
	static void startRecording(double time = 0.0);
	static void stopRecording();
};


// TODO: internal
class FrameCapturerContext
	: public Object
{
public:
	void setCapturerTarget(RenderTargetTexture* renderTarget);
	void startRecording(double time);
	void stopRecording();
	void updateOnRender();

LN_INTERNAL_ACCESS:
	FrameCapturerContext();
	virtual ~FrameCapturerContext();
	void initialize(SwapChain* swapChain);

private:
	enum class State
	{
		Stoped,
		Recording,
	};

	void RecordCommand(Driver::ITexture* target, State newState);

	detail::GraphicsManager*	m_manager;
	RenderTargetTexture*		m_capturerTarget;
	uint64_t					m_lastTimeMSecs;
	int							m_timeMSecs;
	State						m_requestedState;
	State						m_currentState_main;
	
	PathNameA					m_savingDir;
	detail::CapturerContext*	m_gifContext;
	State						m_currentState_render;
	uint64_t					m_lastTick;
};

LN_NAMESPACE_END
