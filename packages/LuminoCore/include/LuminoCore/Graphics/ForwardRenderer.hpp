#pragma once
#include <memory>
#include <vector>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/Camera.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/MeshLoader.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>

namespace ln {

class GraphicsContext;

/** Lighting parameters for the forward renderer. */
struct DirectionalLight {
    Vector3 direction = Vector3{0.0f, -1.0f, 0.5f};
    Color color       = Color::white();
    Color ambient     = Color{0.15f, 0.15f, 0.15f, 1.0f};
};


/**
 * Renderer をベースに構築されたシングルパスフォワードレンダラー。
 *
 * ライティングデータ（Scene UBO、set=1 BindGroup）を管理し、
 * フレーム全体をレンダリングするためのシンプルなワンコールAPIを提供します。また、
 * マルチパス Renderer API を委譲することで、呼び出し元は余分なホップなしでアクセスできます。
 * カメラデータ（set=0）は Renderer が管理し、beginRenderPass(camera) で自動アップロードされます。
 *
 * カスタムのマルチパスレンダリングには、renderer() を直接使用してください。
 */
class ForwardRenderer : public RefObject {
public:
    static Result<Ref<ForwardRenderer>> create(GraphicsContext* ctx);

    // ---- Access the underlying Renderer ----

    /** The core Renderer. Use this to implement custom multi-pass rendering. */
    Renderer* renderer() const { return m_renderer; }

    // ---- Forwarded Renderer API (convenience) ----

    rhi::TextureFormat      colorFormat()           const { return m_renderer->colorFormat(); }
    rhi::TextureFormat      depthFormat()           const { return m_renderer->depthFormat(); }

    void beginFrame() { m_renderer->beginFrame(); }
    void endFrame() { m_renderer->endFrame(); }
    void beginRenderPass(rhi::TextureView* c, rhi::TextureView* d,
                         const Camera& camera, const Color& clear = Color{0,0,0,1})                    { m_renderer->beginRenderPass(c, d, camera, clear); }
    void beginRenderPass(rhi::TextureView* c, rhi::TextureView* d,
                         const Color& clear = Color{0,0,0,1})                                          { m_renderer->beginRenderPass(c, d, clear); }
    void endRenderPass()                                                                                { m_renderer->endRenderPass(); }
    void setPassBindGroup(u32 set, rhi::BindGroup* bg,
                          u32 dynOffset = 0, u32 dynOffsetCount = 0)                                   { m_renderer->setPassBindGroup(set, bg, dynOffset, dynOffsetCount); }
    Result<void> drawMesh(Mesh* mesh, const Transform& t)                                              { return m_renderer->drawMesh(mesh, t); }
    Result<void> drawMesh(Mesh* mesh, const Transform& t, Material* mat)                               { return m_renderer->drawMesh(mesh, t, mat); }
    Result<void> drawScreenRect(Material* mat)                                                         { return m_renderer->drawScreenRect(mat); }

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


private:
    ForwardRenderer() = default;

    Renderer* m_renderer = nullptr;  // non-owning; owned by GraphicsContext

    // Per-frame scene data allocator (lighting, set=1) — double-buffered via DynamicUniformAllocator
    std::unique_ptr<DynamicUniformAllocator> m_sceneAllocator;

    DirectionalLight m_light;

    GraphicsContext* m_ctx = nullptr;
};

} // namespace ln
