
#pragma once
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Renderer.h>

namespace Lumino
{
namespace Graphics
{

// internal クラス。公開禁止。
class Helper
{
public:
	static Device::IGraphicsDevice*	GetGraphicsDevice(GraphicsManager* manager) { return manager->m_graphicsDevice; }
	static RenderingThread*			GetRenderingThread(GraphicsManager* manager) { return manager->m_renderingThread; }
	static bool						IsPlatformTextureLoading(GraphicsManager* manager) { return manager->m_platformTextureLoading; }
	static RenderingCommandList*	GetPrimaryRenderingCommandList(GraphicsManager* manager) { return manager->m_renderer->m_primaryCommandList; }
	static PainterEngine*			GetPainterEngine(GraphicsManager* manager) { return manager->m_painterEngine; }
	static Device::ITexture*		GetDummyTexture(GraphicsManager* manager) { return manager->m_dummyTexture; }
};

} // namespace Graphics
} // namespace Lumino
