#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KMeshRenderFeature.hpp>
//#include "../RenderingManager.hpp"
//#include "../../Mesh/MeshGeneraters/MeshGenerater.hpp"
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include <LuminoEngine/Rendering/InstancedMeshesModel.hpp>
#include <LuminoEngine/Mesh/MeshModeEntity.hpp>

namespace ln {
namespace kanata {

MeshRenderFeature::MeshRenderFeature(detail::RenderingManager* manager) {
}

void MeshRenderFeature::drawMesh(
    BatchCollector* collector,
    Material* material,
    MeshPrimitive* mesh,
    int sectionIndex,
    const Matrix* transform,
    detail::SkeletonInstance* skeleton,
    detail::MorphInstance* morph) {
    LN_DCHECK(material);
    
    MeshSection2 section;
    VertexLayout* layout;
    std::array<VertexBuffer*, detail::MaxVertexStreams> vb = {};
    int vbCount;
    IndexBuffer* ib;
    mesh->commitRenderData(sectionIndex, morph, &section, &layout, &vb, &vbCount, &ib);
    LN_DCHECK(vb[0]);
    LN_DCHECK(section.primitiveCount > 0);

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

#if 0   // TODO: morph
	if (morph) {
        std::array<float, MaxRenderMorphTargets> weights;
        bool hasAnyWeight = morph->getMorphWeights(&weights);
        if (hasAnyWeight) {

            const auto& commandList = context;
            const int targetCount = mesh->morphTargetCount();

            struct BlendInfo {
                Vector4 _Weights;
                int32_t _TargetCount;
                int32_t _VertexCount;
            };
            BlendInfo blendInfo;
            blendInfo._Weights.x = weights[0];
            blendInfo._Weights.y = weights[1];
            blendInfo._Weights.z = weights[2];
            blendInfo._Weights.w = weights[3];
            blendInfo._TargetCount = mesh->morphTargetCount();
            blendInfo._VertexCount = mesh->vertexCount();
            detail::ConstantBufferView view = commandList->allocateUniformBuffer(sizeof(blendInfo));

            std::array<VertexBuffer*, MaxRenderMorphTargets> targets;
            mesh->commitMorphTargets(morph, &targets);

            const auto& shader = detail::RenderingManager::instance()->blendShapeShader;
            ShaderSecondaryDescriptor* descriptor = commandList->acquireShaderDescriptor(shader.shader);
            descriptor->setStorageData(shader.dstVerticesGID, morph->m_blendResult);
            descriptor->setTexture(shader.srcVerticesGID, vb[0]);
            if (targetCount >= 1)
                descriptor->setTexture(shader.target0GID, targets[0]);
            else
                descriptor->setTexture(shader.target0GID, nullptr);
            if (targetCount >= 2)
                descriptor->setTexture(shader.target1GID, targets[1]);
            else
                descriptor->setTexture(shader.target1GID, targets[0]);
            if (targetCount >= 3)
                descriptor->setTexture(shader.target2GID, targets[2]);
            else
                descriptor->setTexture(shader.target2GID, targets[0]);
            if (targetCount >= 4)
                descriptor->setTexture(shader.target3GID, targets[3]);
            else
                descriptor->setTexture(shader.target3GID, targets[0]);
            descriptor->setUniformBuffer(shader.blendInfoGID, view);
            descriptor->setUniformBufferData(shader.blendInfoGID, &blendInfo, sizeof(blendInfo));

            context->setShaderPass(shader.shaderPass);
            context->setShaderDescriptor(descriptor);
            context->dispatch(std::max(1, mesh->vertexCount() / 64), 1, 1);

            // descriptor->setst

            //	batch->morph->getMorphWeights(&elementInfo.morphWeights);

            data.vertexBuffers[0] = morph->m_blendResult;
        }
    }
#endif
}

void MeshRenderFeature::drawMeshInstanced(BatchCollector* collector, Material* material, InstancedMeshList* list) const {
    // Commit dirty data
    MeshSection2 section;
    VertexLayout* layout;
    std::array<VertexBuffer*, detail::MaxVertexStreams> vb = {};
    int vbCount;
    IndexBuffer* ib;
    list->commitRenderData(&section, &layout, &vb, &vbCount, &ib);
    LN_DCHECK(vb[0]);
    LN_DCHECK(section.primitiveCount > 0);
    LN_DCHECK(list->instanceCount() > 0);

    // Make batch
    Batch* batch = collector->newBatch<Batch>(1, material);
    for (int i = 0; i < vbCount; ++i) {
        batch->elemets2[0].vertexBuffers[i] = vb[i];
    }
    batch->elemets2[0].indexBuffer = ib;
    batch->elemets2[0].firstIndex = section.startIndex;
    batch->elemets2[0].primitiveCount = section.primitiveCount;
    batch->elemets2[0].instanceCount = list->instanceCount();
    batch->vertexLayout = layout;
    batch->primitiveTopology = section.topology;
}

} // namespace kanata
} // namespace ln

