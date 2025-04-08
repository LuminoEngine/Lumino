#pragma once
#include "Common.hpp"
#include "Kanata/KDrawEvent.hpp"

namespace ln {

class SurfaceContext : public Object {
    // NOTE: CommandList は Create じゃなくて Context あたりからの Get にできないか？
    //   Release 不要にできるし、 Vulkan は Create じゃなくて Allocate な感じだし、 WebGPU も毎フレーム作れる CommandEncoder だし。
    //   → 内部的には ln::GraphicsContext なので、 Rendering モジュールの CommandList まで持つのは違和感。
    //   それなら Window に GraphicsContext と CommandList を持たせるか？
    //   → Window は統合先によっては存在しないことがある。
    //   それなら RenderingContext を作るか？
    //   → ハイレベルなところで、いろんなオブジェクトをまとめる人は必要になりそう。でも名前がこれなのは微妙な気がする。
    //     RenderingContext は Webの CanvasRenderingContext2D をはじめとして、実際に図形を描く関数を持つクラスが多い。
    //     GraphicsContext も、まぁ同じ。
    //     WebGPU の GPUCanvasContext や SurfaceContext が代案だろうか。
    // 　  Skia は GrContext。 https://skia.org/docs/user/special/vulkan/
    //
    // NOTE: Present は誰が行う？
    //   現状だと、Window または GraphicsContext が適当。
    //   SwapChain はこれらと 1:1 なので、できればこの中に入れてしまいたい。
    //   glfw 的には Window.
    //   他アプリのへの Integration を考えるなら、 Window に持たせておいた方がよいかも。
    //   Window インスタンスを作っていないときは Lumino 側から Present はできない、という事にできるので。
    
public:
    static Result_deprecated<Ref<SurfaceContext>> createFromWindow(detail::RenderingManager* renderingManager, PlatformWindow* window);
    static Ref<SurfaceContext> createFromExternal(
        detail::RenderingManager* renderingManager,
        GraphicsContext* externalContext);

    SurfaceContext();
    ~SurfaceContext() override;

    const Ref<GraphicsContext>& context() const { return m_context; }
    GraphicsCommandList* commandList() const;
    const Ref<CommandList>& renderingContext() const { return m_renderingContext; }
    const URef<kanata::DrawEventList>& drawEventList() const { return m_drawEventList; }

private:
    Ref<GraphicsContext> m_context;
    Ref<CommandList> m_renderingContext;
    URef<kanata::DrawEventList> m_drawEventList;
};

} // namespace ln
