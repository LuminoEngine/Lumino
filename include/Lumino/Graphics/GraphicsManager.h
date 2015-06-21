
#pragma once

#include "Common.h"
#include "../Imaging/FontManager.h"
#include "GraphicsDevice.h"
#include "SwapChain.h"

namespace Lumino
{
namespace Graphics
{
class Renderer;
class RenderingThread;
class PainterEngine;

struct GraphicsManagerConfigData
{
	Platform::Window*		MainWindow;
	Lumino::FileManager*	FileManager;
	bool					PlatformTextureLoading;

	GraphicsManagerConfigData()
		: MainWindow(NULL)
		, FileManager(NULL)
		, PlatformTextureLoading(false)
	{}
};

/**
	@brief		グラフィックス機能の管理クラス
*/
class GraphicsManager
	: public RefObject
{
public:
	GraphicsManager(const GraphicsManagerConfigData& configData);
	~GraphicsManager();

public:
	RenderingType GetRenderingType() const { return RenderingType_Deferred; }
	GraphicsDevice* GetGraphicsDevice() { return m_graphicsDevice; }
	Renderer* GetRenderer() { return m_renderer; }
	SwapChain* GetMainSwapChain() { return m_mainSwapChain; }
	Imaging::FontManager* GetFontManager() const { return m_fontManager; }
	RenderingCommandList* GetPrimaryRenderingCommandList();

	PainterEngine* GetPainterEngine() { return m_painterEngine; }
	bool IsPlatformTextureLoading() const { return m_platformTextureLoading; }

	Device::ITexture* GetDummyTexture() { return m_dummyTexture; }

	/// (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void PauseDevice();
	/// (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void ResumeDevice();


private:
	FileManager* GetFileManager() { return m_fileManager; }

private:
	friend class SwapChain;
	friend class Renderer;
	friend class Texture;

	FileManager*			m_fileManager;
	RefPtr<Imaging::FontManager>	m_fontManager;
	
	RefPtr<GraphicsDevice>	m_graphicsDevice;
	RefPtr<SwapChain>		m_mainSwapChain;
	Device::ITexture*		m_dummyTexture;		///< public にはしないので RefPtr は使えない

	Renderer*				m_renderer;
	RenderingThread*		m_renderingThread;

	PainterEngine*			m_painterEngine;
	bool					m_platformTextureLoading;
};

} // namespace Graphics
} // namespace Lumino
