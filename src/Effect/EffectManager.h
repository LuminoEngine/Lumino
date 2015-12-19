
#pragma once
#include <array>
#include <Lumino/Threading/Task.h>
#include <Lumino/Input/Common.h>

LN_NAMESPACE_BEGIN
class FileManager;
class GraphicsManager;
class AudioManagerImpl;

namespace detail
{
class EffectEngine;

class EffectManager
	: public RefObject
{
public:
	struct Settings
	{
		FileManager*		fileManager = nullptr;
		GraphicsManager*	graphicsManager = nullptr;
		AudioManagerImpl*		audioManager = nullptr;
	};

public:
	EffectManager();
	virtual ~EffectManager();

	void Initialize(const Settings& settings);
	void Finalize();

	void PreRender();
	void Render();

	FileManager*		GetFileManager() { return m_fileManager; }
	GraphicsManager*	GetGraphicsManager() { return m_graphicsManager; }
	AudioManagerImpl*		GetAudioManager() { return m_audioManager; }

	EffectEngine*		GetEffectEngine() { return m_engine; }

private:
	void Thread_UpdateFrame();

private:
	FileManager*		m_fileManager;
	GraphicsManager*	m_graphicsManager;
	AudioManagerImpl*		m_audioManager;

	EffectEngine*	m_engine;	// •¡”ƒc[ƒ‹“¯‘Î‰‚Æ‚©‚È‚Á‚½‚ç•¡”‚Å‚«‚é

	//Threading::DelegateThread	m_threadUpdateFrame;
	tr::TaskPtr	m_taskUpdateFrame;
};

} // namespace detail
LN_NAMESPACE_END
