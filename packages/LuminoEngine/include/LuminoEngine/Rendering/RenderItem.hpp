
#pragma once
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Rendering/SceneRenderPass.hpp>
#include "Common.hpp"

namespace ln {
class Texture;

class RenderItem final : public SceneRenderPass::DrawElement {
	Material* material;
	Matrix worldTransform;
	SpriteDrawElement()
		: DrawElement(DrawElementType::Sprite) {}
	const Matrix& worldMatrix() override { return worldTransform; }
	void onRender(RendererServer* rendererServer, CommandList* commandList) override {
		//const URef<kanata::BatchCollector>& batchCollector() const { return m_batchCollector; }
		Batch* batch = commandList->batchCollector()->newBatch();

		
    // Make batch
    Batch* batch = collector->newBatch<Batch>(1, material);
    for (int i = 0; i < vbCount; ++i) {
        batch->elemets2[0].vertexBuffers[i] = vb[i];
    }
    batch->elemets2[0].indexBuffer = ib;
    batch->elemets2[0].firstIndex = section.startIndex;
    batch->elemets2[0].primitiveCount = section.primitiveCount;
    batch->vertexLayout = layout;
    batch->primitiveTopology = section.topology;
    batch->skeleton = skeleton;


    if (transform) {
        batch->worldTransform = Matrix::multiply(batch->worldTransform, *transform);
    }
		// MeshRenderFeature::drawMesh
	}
};

} // namespace ln
