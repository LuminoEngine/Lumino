#pragma once
#include <memory>
#include <vector>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/Camera.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/Transform.hpp>

namespace ln {

class GraphicsContext;

/** 描画対象のオブジェクト。 */
struct RenderObject {
    Ref<Mesh> mesh;
    Transform transform;
};

/** フォワードレンダラーのライティングパラメータ。 */
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

    // ---- 下層の Renderer へのアクセス ----

    /** コアの Renderer。カスタムのマルチパスレンダリングを実装するときに使用します。 */
    Renderer* renderer() const { return m_renderer; }

    // ---- Renderer API の転送 (簡易版) ----

    rhi::TextureFormat      colorFormat()           const { return m_renderer->colorFormat(); }
    rhi::TextureFormat      depthFormat()           const { return m_renderer->depthFormat(); }

    void beginFrame() { m_renderer->beginFrame(); }
    void endFrame() { m_renderer->endFrame(); }
    void beginRenderPass(rhi::TextureView* c, rhi::TextureView* d,
                         const Camera& camera, const Color& clear = Color{0,0,0,1})                    { m_renderer->beginRenderPass(c, d, camera, clear); }
    void beginRenderPass(rhi::TextureView* c, rhi::TextureView* d,
                         const Color& clear = Color{0,0,0,1})                                          { m_renderer->beginRenderPass(c, d, clear); }
    void endRenderPass()                                                                                { m_renderer->endRenderPass(); }
    void setPassBindGroup(uint32_t set, rhi::BindGroup* bg,
                          uint32_t dynOffset = 0, uint32_t dynOffsetCount = 0)                                   { m_renderer->setPassBindGroup(set, bg, dynOffset, dynOffsetCount); }
    Result<void> drawMesh(Mesh* mesh, const Transform& t)                                              { return m_renderer->drawMeshImmediate(mesh, t); }
    Result<void> drawScreenRect(Material* mat)                                                         { return m_renderer->drawScreenRect(mat); }

    // ---- ForwardRenderer 固有の API ----

    /** renderFrame() が使用する平行光源を設定します。 */
    void setLight(const DirectionalLight& light) { m_light = light; }

    /**
     * シングルパスのフレームを描画します (簡易ラッパー)。
     * カメラとライティングのデータを View UBO にアップロードし、
     * beginFrame / beginRenderPass / drawMesh x N / endRenderPass / endFrame を呼び出します。
     *
     * 次のコードと等価です:
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
    /// @param objects  描画対象の配列。呼び出し側が毎フレーム vector を組み立てずに済むよう
    ///                 ポインタと個数で受け取る。
    Result<void> renderFrame(
        rhi::TextureView* colorTarget,
        rhi::TextureView* depthTarget,
        const Camera& camera,
        const RenderObject* objects,
        size_t objectCount,
        const Color& clearColor = Color{0.1f, 0.1f, 0.1f, 1.0f});


private:
    ForwardRenderer() = default;

    Renderer* m_renderer = nullptr;  // 非所有。GraphicsContext が所有する

    // フレームごとのシーンデータ (ライティング) アロケータ - DynamicUniformAllocator でダブルバッファリング
    std::unique_ptr<DynamicUniformAllocator> m_sceneAllocator;

    // リフレクションから得たシーンセットのインデックス
    int16_t m_sceneSetIndex = -1;

    DirectionalLight m_light;

    GraphicsContext* m_ctx = nullptr;
};

} // namespace ln
