
#pragma once

#include "GraphicsDevice.h"
#include "SwapChain.h"

namespace Lumino
{
namespace Graphics
{
class Renderer;
class RenderingThread;

struct GraphicsManagerConfigData
{
	Platform::Window*		MainWindow;
	Lumino::FileManager*	FileManager;
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
	RenderingCommandList* GetPrimaryRenderingCommandList();

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
	
	RefPtr<GraphicsDevice>	m_graphicsDevice;
	RefPtr<SwapChain>		m_mainSwapChain;

	Renderer*				m_renderer;
	RenderingThread*		m_renderingThread;
};

} // namespace Graphics
} // namespace Lumino
