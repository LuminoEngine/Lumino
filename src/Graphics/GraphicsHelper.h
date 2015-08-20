
#pragma once
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/GlyphRun.h>

namespace Lumino
{
namespace Graphics
{

// internal クラス。公開禁止。
class Helper
{
public:
	inline static Device::IGraphicsDevice*	GetGraphicsDevice(GraphicsManager* manager) { return manager->m_graphicsDevice; }
	inline static RenderingThread*			GetRenderingThread(GraphicsManager* manager) { return manager->m_renderingThread; }
	inline static bool						IsPlatformTextureLoading(GraphicsManager* manager) { return manager->m_platformTextureLoading; }
	inline static RenderingCommandList*		GetPrimaryRenderingCommandList(GraphicsManager* manager) { return manager->m_renderer->m_primaryCommandList; }
	inline static PainterEngine*			GetPainterEngine(GraphicsManager* manager) { return manager->m_painterEngine; }
	inline static Device::ITexture*			GetDummyTexture(GraphicsManager* manager) { return manager->m_dummyTexture; }

	inline static Device::ITexture*			GetDeviceObject(Texture* texture) { return texture->m_deviceObj; }
	inline static Device::IVertexBuffer*	GetDeviceObject(VertexBuffer* vb) { return vb->m_deviceObj; }
	inline static Device::IIndexBuffer*		GetDeviceObject(IndexBuffer* ib) { return ib->m_deviceObj; }

	inline static void						AttachGlyphTextureCache(GlyphRun* gr, Internal::FontGlyphTextureCache* cache) { gr->AttachGlyphTextureCache(cache); }
	inline static Internal::FontGlyphTextureCache* GetGlyphTextureCache(GlyphRun* gr) { return gr->m_glyphTextureCache; }
	inline static TextLayoutResult*			GetGlyphData(GlyphRun* gr) { return gr->m_glyphData; }
};

} // namespace Graphics
} // namespace Lumino
