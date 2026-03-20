// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#if 0
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include "Common.hpp"

namespace ln {
class GraphicsCommandList;
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class RenderTargetTexture;
class DepthBuffer;
class Shader;
class ShaderPass;
class SwapChain;
namespace detail {
class RenderingQueue;
class IRenderPass;
}

/*
 * グラフィクスデバイスへの描画呼出しを発行するためのクラスです。
 */
class LN_API GraphicsCommandList
    : public Object {



    // TODO: intenral
    const Ref<GraphicsCommandList>& commandList() const { return m_commandList; }

protected:
    virtual void onDispose(bool explicitDisposing) override;

private:
    LN_INTERNAL_NEW_OBJECT;
    GraphicsContext();
    virtual ~GraphicsContext();
    void init();


    friend class detail::GraphicsContextInternal;
    friend class detail::RenderingQueue;
};
#endif
} // namespace ln
