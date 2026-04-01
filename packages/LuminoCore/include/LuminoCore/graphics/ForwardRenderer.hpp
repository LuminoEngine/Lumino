#pragma once
#include <memory>
#include <vector>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/graphics/Camera.hpp>
#include <LuminoCore/graphics/Material.hpp>
#include <LuminoCore/graphics/Mesh.hpp>
#include <LuminoCore/graphics/MeshLoader.hpp>
#include <LuminoCore/graphics/Renderer.hpp>

namespace ln {

class GraphicsContext;

/** Lighting parameters for the forward renderer. */
struct DirectionalLight {
    Vector3 direction = Vector3{0.0f, -1.0f, 0.5f};
    Color color       = Color::white();
    Color ambient     = Color{0.15f, 0.15f, 0.15f, 1.0f};
};

/**
 * Single-pass forward renderer built on top of Renderer.
 *
 * Manages camera/lighting data (View UBO, set=0 BindGroup) and exposes a
 * simple one-call API for rendering a full frame. Also delegates the
 * multi-pass Renderer API so callers can access it without an extra hop.
 *
 * For custom multi-pass rendering, use renderer() directly.
 */
class ForwardRenderer : public RefObject {
public:
    static Result<Ref<ForwardRenderer>> create(GraphicsContext* ctx);

    // ---- Access the underlying Renderer ----

    /** The core Renderer. Use this to implement custom multi-pass rendering. */
    Renderer* renderer() const { return m_renderer.get(); }

    // ---- Forwarded Renderer API (convenience) ----

    rhi::PipelineLayout*    pipelineLayout()       const { return m_renderer->pipelineLayout(); }
    rhi::BindGroupLayout*   viewBindGroupLayout()   const { return m_renderer->viewBindGroupLayout(); }
    rhi::BindGroupLayout*   objectBindGroupLayout() const { return m_renderer->objectBindGroupLayout(); }
    rhi::TextureFormat      colorFormat()           const { return m_renderer->colorFormat(); }
    rhi::TextureFormat      depthFormat()           const { return m_renderer->depthFormat(); }

    void beginFrame()                                                       { m_renderer->beginFrame(); }
    void endFrame()                                                         { m_renderer->endFrame(); }
    void beginRenderPass(rhi::TextureView* c, rhi::TextureView* d,
                         const Color& clear = Color{0,0,0,1})               { m_renderer->beginRenderPass(c, d, clear); }
    void endRenderPass()                                                    { m_renderer->endRenderPass(); }
    void setPassBindGroup(u32 set, rhi::BindGroup* bg)                      { m_renderer->setPassBindGroup(set, bg); }
    Result<void> drawMesh(Mesh* mesh, const Transform& t)                   { return m_renderer->drawMesh(mesh, t); }
    Result<void> drawMesh(Mesh* mesh, const Transform& t, Material* mat)    { return m_renderer->drawMesh(mesh, t, mat); }
    Result<void> drawScreenRect(Material* mat)                              { return m_renderer->drawScreenRect(mat); }

    // ---- ForwardRenderer-specific API ----

    /** Set the directional light used by renderFrame(). */
    void setLight(const DirectionalLight& light) { m_light = light; }

    /**
     * Render a single-pass frame (convenience wrapper).
     * Uploads camera and lighting data to the View UBO, then calls
     * beginFrame / beginRenderPass / drawMesh × N / endRenderPass / endFrame.
     *
     * Equivalent to:
     * @code
     *   updateViewUBO(camera);
     *   renderer()->beginFrame();
     *   renderer()->beginRenderPass(colorTarget, depthTarget, clearColor);
     *   renderer()->setPassBindGroup(0, viewBindGroup());
     *   for (auto& obj : objects) renderer()->drawMesh(obj.mesh.get(), obj.transform);
     *   renderer()->endRenderPass();
     *   renderer()->endFrame();
     * @endcode
     */
    Result<void> renderFrame(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Camera& camera,
        const std::vector<RenderObject>& objects,
        const Color& clearColor = Color{0.1f, 0.1f, 0.1f, 1.0f});

    /** The BindGroup for the per-frame View UBO (set=0). */
    rhi::BindGroup* viewBindGroup() const { return m_viewBindGroup.get(); }

private:
    ForwardRenderer() = default;

    Ref<Renderer> m_renderer;

    // Per-frame view data (camera + lighting), uploaded each renderFrame() call.
    u64 m_viewUBOSize = 0;
    Ref<rhi::Buffer>    m_viewUBO;
    Ref<rhi::BindGroup> m_viewBindGroup;

    DirectionalLight m_light;

    GraphicsContext* m_ctx = nullptr;
};

} // namespace ln
