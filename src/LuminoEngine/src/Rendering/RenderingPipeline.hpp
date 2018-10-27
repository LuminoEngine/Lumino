#pragma once
#include <LuminoEngine/Rendering/RenderView.hpp>

namespace ln {
class GraphicsContext;
namespace detail {
class ClusteredShadingSceneRenderer;

/*
 * 描画リストやポストエフェクトなど描画に関わる全てのパラメータを受け取り、
 * RenderView から覗くことができる 1 枚の絵を描き上げる。
 *
 * 将来的な話だが、Deffered と Forward 混在のような複数の SceneRenderer をコントロールするのはこのクラスの役目。
 */
class RenderingPipeline
	: public RefObject
{
public:
    void initialize();

    const SizeI& renderingFrameBufferSize() const { return m_renderingFrameBufferSize; }
    const Ref<detail::FrameBufferCache>& frameBufferCache() const { return m_frameBufferCache; }
    const detail::CameraInfo* mainCameraInfo() const { return m_mainCameraInfo; }

    const List<detail::DrawElementListCollector*>* elementListManagers() const { return m_elementListManagers; }

protected:
    const detail::CameraInfo* m_mainCameraInfo;
    Ref<detail::FrameBufferCache> m_frameBufferCache;
    SizeI m_renderingFrameBufferSize;	// render() の内側だけで使える
    const List<detail::DrawElementListCollector*>* m_elementListManagers;
};

class SceneRenderingPipeline
    : public RenderingPipeline
{
public:
    SceneRenderingPipeline();

    void initialize();
    void render(
        GraphicsContext* graphicsContext,
        const FrameBuffer& frameBuffer,
        const detail::CameraInfo* mainCameraInfo,
        const List<detail::DrawElementListCollector*>* elementListManagers);


private:
    Ref<detail::ClusteredShadingSceneRenderer> m_sceneRenderer;
};

class UIRenderingPipeline
    : public RefObject
{
public:
};

} // namespace detail
} // namespace ln

