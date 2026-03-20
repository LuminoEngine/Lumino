#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/GPU/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/VertexLayout.hpp>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/SceneRenderPass.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include "Common.hpp"

namespace ln {
class Texture;
class Mesh;

namespace detail {
class MeshDrawElement final : public SceneRenderPass::DrawElement {
public:
    std::unique_ptr<kanata::BatchProxyState> m_batchProxyState; // Dummy
    //Material* material;
    Matrix worldTransform;
    Ref<Mesh> mesh;
    MeshDrawElement();
    ~MeshDrawElement() override;
    const Matrix& worldMatrix() override { return worldTransform; }
    void onRender(RendererServer* rendererServer, CommandList* commandList) override;
};
} // namespace detail

class RenderItem final : public Object {
public:
    static Ref<RenderItem> create();
    void setMesh(Mesh* mesh);
    void setTransform(const Matrix& matrix);

public:
    RenderItem();
    std::unique_ptr<detail::MeshDrawElement> m_drawElement;
};

} // namespace ln
