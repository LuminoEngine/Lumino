
#pragma once

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class CapturerContext; }
class RenderTarget;

class FrameCapturer;
using FrameCapturerPtr = RefPtr<FrameCapturer>;

class FrameCapturer
	: public Object
{
public:
	static FrameCapturerPtr Create();

public:
	void SetCapturerTarget(RenderTarget* renderTarget);
	void StartRecording();
	void StopRecording();
	void Record();

LN_INTERNAL_ACCESS:
	FrameCapturer();
	virtual ~FrameCapturer();
	void Initialize(detail::GraphicsManager* manager);

private:
	enum class State
	{
		Stoped,
		Recording,
	};

	void RecordCommand(Driver::ITexture* target, State newState);

	detail::GraphicsManager*	m_manager;
	RenderTarget*				m_capturerTarget;
	State						m_requestedState;
	
	PathNameA					m_savingDir;
	detail::CapturerContext*	m_gifContext;
	State						m_currentState;
	uint64_t					m_lastTick;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
