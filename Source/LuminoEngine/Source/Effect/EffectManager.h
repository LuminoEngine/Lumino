
#pragma once
#include <array>
#include <Lumino/Threading/Task.h>
#include <Lumino/Input/Common.h>
#include "../Graphics/GraphicsManager.h"

LN_NAMESPACE_BEGIN
class FileManager;
class AudioManager;

namespace detail
{
class EffectEngine;

class EffectManager
	: public RefObject
	, public IDeviceResetListener
{
public:
	struct Settings
	{
		FileManager*		fileManager = nullptr;
		GraphicsManager*	graphicsManager = nullptr;
		AudioManager*		audioManager = nullptr;
	};

public:
	EffectManager();
	virtual ~EffectManager();

	void initialize(const Settings& settings);
	void Finalize();

	EffectEngine* CreateEffectWorld();
	void ReleaseEffectWorld(EffectEngine* world);

	//void PreRender();
	//void Render();
	virtual void OnLostDevice() override;
	virtual void OnResetDevice() override;

	FileManager*		GetFileManager() { return m_fileManager; }
	GraphicsManager*	GetGraphicsManager() { return m_graphicsManager; }
	AudioManager*		GetAudioManager() { return m_audioManager; }

	//EffectEngine*		GetEffectEngine() { return m_engine; }

//private:
//	void Thread_UpdateFrame();

private:
	FileManager*		m_fileManager;
	GraphicsManager*	m_graphicsManager;
	AudioManager*		m_audioManager;
	List<RefPtr<EffectEngine>>	m_effectWorldList;

	//tr::TaskPtr	m_taskUpdateFrame;
};

} // namespace detail
LN_NAMESPACE_END
