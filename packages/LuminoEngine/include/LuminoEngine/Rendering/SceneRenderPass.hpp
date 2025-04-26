#pragma once
#include "Common.hpp"

namespace ln {

class SceneRenderPass final : public Object {
    // NOTE: 以前の RenderingContext のような、ハイレベル描画クラス。
    //   SpriteRenderer を自分でコントロールして begin/end する方法だと、
    //   それさえまたぐ状況でのZソートが出来なくなるため、改めて用意した。
    //   RenderPass の begin~end の間で使用できる。
    //
    // NOTE: 設計イメージは WebGPU の RenderPassEncoder.
    //   今はゲームシーン用の Sprite しかないけど、以前そうだったように
    //   2D専用の機能が必要になった時は同列で CanvasRenderPass のようなものを用意する。
    //   これは godot と似たイメージ。　 (RendererSceneCull, RendererCanvasCull)
    //
public:
    static Ref<SceneRenderPass> create(detail::RenderingManager* manager);

    void reset(SurfaceContext* context, RenderPass* renderPass, const RenderViewPoint* viewPoint);
    void render(CommandList* commandList); // TODO: 実際に欲しいのは Collector だけのはず

    void drawSprite(Material* material, const Matrix& worldTransform, const SpriteData& data);

    SurfaceContext* currentContext() const { return m_currentContext; }
    RenderPass* currentRenderPass() const { return m_currentRenderPass; }
    RendererServer* rendererServer() const;

private:
    enum class DrawElementType {
        Sprite,
    };

    // SceneNode と考えてOK。Zソートの対象となる。
    struct DrawElement {
        DrawElementType type;
        DrawElement* next;
        float zDistance;
        bool visible;
        DrawElement(DrawElementType t)
            : type(t)
            , next(nullptr)
            , zDistance(0.0f)
            , visible(true) {}
        virtual ~DrawElement() {}
        virtual const Matrix& worldMatrix() = 0;
        virtual void onRender(RendererServer* rendererServer, CommandList* commandList) = 0;
    };

    SceneRenderPass(detail::RenderingManager* manager);
    void onDispose(bool explicitDisposing) override;
    void destructDrawElementList();
    void addDrawElement(DrawElement* instance);
    void setupElement(DrawElement* instance);

    // Not call destructor.
    template<class T, class... TArgs>
    T* newDrawElement() {
        void* buffer = m_dataAllocator->allocate(sizeof(T));
        T* data = new (buffer) T(std::forward<TArgs>(args)...);
        addDrawElement(data);
        return data;
    }

    detail::RenderingManager* m_manager;
    Ref<detail::LinearAllocator> m_dataAllocator;
    URef<RendererServer> m_rendererServer;
    DrawElement* m_headDrawElement;
    DrawElement* m_tailDrawElement;
    int m_visibleDrawElementCount;
    SurfaceContext* m_currentContext;
    RenderPass* m_currentRenderPass;
    const RenderViewPoint* m_currentViewPoint;
    detail::ZSortDistanceBase m_zSortDistanceBase;

};


} // namespace ln

