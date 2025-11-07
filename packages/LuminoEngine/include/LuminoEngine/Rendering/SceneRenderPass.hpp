#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
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

    enum class DrawElementType {
        Sprite,
    };

    // SceneNode と考えてOK。Zソートの対象となる。
    // NOTE: 後々この構造は公開して、Godot の CanvasItem のように構築済みの instruction として使えるようにしたい。
    //   現在の、とりあえず drawSprite で雑に描けて勝手に DrawElement を作ってくれる仕組みはそのまま維持でよいだろう。
    //   適材適所がある。
    //   - CanvasItem のような方式は
    //   - 事前準備無しでで雑に描画する方法は
    // NOTE: ちなみに Godot は CanvasItem が z_index を持っていて、この単位で描画順を決められる。
    //   なお TileMap は個々の TileData が z_index を持てるが、
    //   これは  TileMapLayer::_rendering_update() にて 異なる z_index の Tile を描画しようとしたとき、
    //   新たな CanvasItem を作成していた。
    struct DrawElement {
        DrawElementType type;
        DrawElement* next;
        float zDistance;
        bool visible;
        bool singleFrameResource;
        DrawElement(DrawElementType t)
            : type(t)
            , next(nullptr)
            , zDistance(0.0f)
            , visible(true)
            , singleFrameResource(true) {}
        virtual ~DrawElement() {}
        virtual const Matrix& worldMatrix() = 0;
        virtual void onRender(RendererServer* rendererServer, CommandList* commandList) = 0;
    };
    void addDrawElement(DrawElement* instance);
    void setupElement(DrawElement* instance);


    static Ref<SceneRenderPass> create(detail::RenderingManager* manager);

    void reset(SurfaceContext* context, RenderPass* renderPass, const RenderViewPoint* viewPoint);
    void render(CommandList* commandList); // TODO: 実際に欲しいのは Collector だけのはず

    void drawSprite(Material* material, const Matrix& worldTransform, const SpriteData& data);

    SurfaceContext* currentContext() const { return m_currentContext; }
    RenderPass* currentRenderPass() const { return m_currentRenderPass; }
    RendererServer* rendererServer() const;

private:
    SceneRenderPass(detail::RenderingManager* manager);
    void onDispose(bool explicitDisposing) override;
    void destructDrawElementList();

    // Not call destructor.
    template<class T, class... TArgs>
    T* newDrawElement() {
        void* buffer = m_dataAllocator->allocate(sizeof(T));
        T* data = new (buffer) T();
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

