#include <LuminoGraphics/RHI/ShaderDescriptor.hpp>
#include <LuminoGraphics/RHI/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawCommand.hpp>
#include <LuminoEngine/Rendering/Kanata/KSceneRenderPass.hpp>
#include "../RenderingManager.hpp"
#include "../SceneRenderer.hpp"
#include "../RLIs/RLIMaterial.hpp"
#include "../../Mesh/MeshModelInstance.hpp"
#include "../../../../Graphics/src/GraphicsManager.hpp"

namespace ln {
namespace kanata {

SceneRenderPass::SceneRenderPass(detail::RenderingManager* manager, Shader* fallbackShader)
    : m_manager(manager)
    , m_fallbackShader(fallbackShader) {
}

void SceneRenderPass::buildDrawCommands(
    detail::SceneRenderer* sceneRenderer,
    const BatchCollector* batchList,
    GraphicsCommandList* descriptorAllocator,
    RenderPass* renderPass,
    const detail::RenderViewInfo& renderViewInfo,
    const detail::SceneInfo& sceneInfo,
    DrawCommandList* drawCommandList) {
    Shader* fallbackShader = this->fallbackShader();
    RenderPass* currentRenderPass = nullptr;

    for (const Batch* batch : batchList->batches()) {

        // RenderPass
        {
            auto* batchRenderPass = (batch->renderPass) ? batch->renderPass : renderPass;
            if (batchRenderPass != currentRenderPass) {
                // End render pass.
                if (currentRenderPass) {
                    auto* command = drawCommandList->newCommand<EndRenderPassCommand>();
                    command->type = DrawCommandType::EndRenderPass;
                    command->renderPass = currentRenderPass;
                }

                LN_DCHECK(batchRenderPass);
                currentRenderPass = batchRenderPass;

                // Begin render pass.
                {
                    auto* command = drawCommandList->newCommand<BeginRenderPassCommand>();
                    command->type = DrawCommandType::BeginRenderPass;
                    command->renderPass = currentRenderPass;
                }
            }
        }

        Material* material = batch->material;
        LN_DCHECK(material);

        // Make info per Batch
        detail::ElementInfo elementInfo;
        elementInfo.objectId = 0; // TODO:
        elementInfo.viewProjMatrix = &renderViewInfo.cameraInfo.viewProjMatrix;
        elementInfo.WorldMatrix = batch->worldTransform;
        elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
        elementInfo.boneLocalQuaternionTexture = nullptr; // TODO: (MMD SDEF)
        if (batch->skeleton) {
            elementInfo.boneTexture = batch->skeleton->skinningMatricesTexture();
        }
        else {
            elementInfo.boneTexture = nullptr;
        }

        for (const BatchElement& batchElement : batch->elemets) {
            DrawCommand* command = drawCommandList->newCommand<DrawCommand>();
            command->type = DrawCommandType::DrawPrimitive;
            command->vertexBuffers = batchElement.vertexBuffers;
            command->indexBuffer = batchElement.indexBuffer;
            command->pipelineState.rasterizerState = {}; // TODO:
            command->pipelineState.depthStencilState = {}; // TODO:
            command->pipelineState.vertexLayout = batch->vertexLayout;
            command->pipelineState.primitiveTopology = batch->primitiveTopology;
            command->firstIndex = batchElement.firstIndex;
            command->firstVertex = batchElement.firstVertex;
            command->primitiveCount = batchElement.primitiveCount;
            command->instanceCount = batchElement.instanceCount;

            // RenderState
            {
                // BlendState
                {
                    auto blendMode = BlendMode::Normal; // TODO:
                    command->pipelineState.blendState.independentBlendEnable = false;
                    detail::RLIMaterial::makeBlendMode(blendMode, &command->pipelineState.blendState.renderTargets[0]);
                }
                // RasterizerState
                {
                    auto cullingMode = CullMode::Back; // TODO:
                    command->pipelineState.rasterizerState.fillMode = FillMode::Solid;
                    command->pipelineState.rasterizerState.cullMode = cullingMode;
                }
                // DepthStencilState
                {
                    auto depthTestEnabled = true; // TODO:
                    auto depthWriteEnabled = true; // TODO:
                    command->pipelineState.depthStencilState.depthTestFunc = (depthTestEnabled) ? ComparisonFunc::LessEqual : ComparisonFunc::Always;
                    command->pipelineState.depthStencilState.depthWriteEnabled = depthWriteEnabled;
                }
            }

            // Select shader
            Shader* actualShader = fallbackShader;
            if (material) {
                Shader* materialShader = material->shader();
                if (materialShader) {
                    actualShader = materialShader;
                }
            }
            
            // Make info per Subset
            detail::SubsetInfo subsetInfo;
            if (material) {
                subsetInfo.materialTexture = material->mainTexture();
                subsetInfo.normalMap = material->normalMap();
                subsetInfo.metallicRoughnessTexture = material->metallicRoughnessTexture();
                subsetInfo.occlusionTexture = material->occlusionTexture();
            }
            
            subsetInfo.opacity = batch->builtinEffectData.opacity;
            subsetInfo.colorScale = batch->builtinEffectData.colorScale;
            subsetInfo.blendColor = batch->builtinEffectData.blendColor;
            subsetInfo.tone = batch->builtinEffectData.tone;
 #if 1
            ShaderTechnique* tech = nullptr;
            {
                detail::ShaderTechniqueRequestClasses requester = {
                    (elementInfo.boneTexture) ? kokage::ShaderTechniqueClass_MeshProcess::SkinnedMesh : kokage::ShaderTechniqueClass_MeshProcess::StaticMesh,
                    (batchElement.instanceCount > 0) ? kokage::ShaderTechniqueClass_DrawMode::Instancing : kokage::ShaderTechniqueClass_DrawMode::Primitive,
                    (subsetInfo.normalMap) ? kokage::ShaderTechniqueClass_Normal::NormalMap : kokage::ShaderTechniqueClass_Normal::Default,
                    (subsetInfo.metallicRoughnessTexture) ? kokage::ShaderTechniqueClass_Roughness::RoughnessMap : kokage::ShaderTechniqueClass_Roughness::Default,
                };

                kokage::ShaderTechniqueClass classSet;
                classSet.defaultTechnique = false;
                classSet.phase = kokage::ShaderTechniqueClass_Phase::Forward;
                classSet.meshProcess = requester.meshProcess;
                classSet.drawMode = requester.drawMode;
                classSet.shadingModel = kokage::ShaderTechniqueClass_ShadingModel::Unlit;
                classSet.normalClass = requester.normal;       // Normal を出力したいので考慮する
                classSet.roughnessClass = requester.roughness; // Roughness を出力したいので考慮する
                tech = detail::ShaderInternal::findTechniqueByClass(actualShader, classSet);
                if (!tech)
                    tech = fallbackShader->techniques()->front();
            }
#else
            ShaderTechnique* tech = actualShader->techniques()[0]; // shader->findTechniqueByVariantKey(0); // TODO:
#endif
            if (LN_ASSERT(tech)) return;
			detail::ShaderTechniqueSemanticsManager* semanticsManager = tech->semanticsManager2();

            // Resource fallback
            {
                if (!subsetInfo.materialTexture) {
                    subsetInfo.materialTexture = m_manager->graphicsManager()->whiteTexture();
                }
                if (!subsetInfo.normalMap) {
                    subsetInfo.normalMap = m_manager->graphicsManager()->defaultNormalMap();
                }
                if (!subsetInfo.metallicRoughnessTexture) {
                    // ダミーは WhiteTexture でよい。
                    //   R=遮蔽率の逆。(1.0 に近づくほど、完全な光の下、オリジナルに近い色になる)
                    //   G=Metallic. マテリアル自体の Metallic と乗算されるため、1.0 でよい。
                    //   B=Roughness. マテリアル自体の Roughness と乗算されるため、1.0 でよい。
                    //   A=未使用
                    subsetInfo.metallicRoughnessTexture = m_manager->graphicsManager()->whiteTexture();
                }
                if (!subsetInfo.occlusionTexture) {
                    subsetInfo.occlusionTexture = m_manager->graphicsManager()->whiteTexture();
                }
            }

            // Allocate descriptor
			detail::ShaderSecondaryDescriptor* descriptor = descriptorAllocator->acquireShaderDescriptor(tech->shader());
            descriptor->reallocFromSemanticsManager(descriptorAllocator, semanticsManager);
            command->shaderDescriptor = descriptor;
            command->shaderPass = tech->passes()[0];    // TODO:

            // Setup descriptor
	        semanticsManager->updateRenderViewVariables(descriptor, renderViewInfo, sceneInfo); // TODO: ここだと element ごとに呼ばれるのでかなり無駄が多い。事前計算しておいて、memcpy で済ませたい
            semanticsManager->updateElementVariables(descriptor, renderViewInfo.cameraInfo, elementInfo);
            semanticsManager->updateSubsetVariables(descriptor, subsetInfo);


            // Commit final material
            {
                detail::PbrMaterialData pbrMaterialData = material->getPbrMaterialData();
                semanticsManager->updateSubsetVariables_PBR(descriptor, pbrMaterialData);
                material->updateShaderVariables(descriptorAllocator, descriptor);

                // TODO: これが実質 ClusteredShadingSceneRenderer 専用なので、なんとかしたいところ。
                sceneRenderer->onSetAdditionalShaderPassVariables(descriptor, tech);
            }
        }
    }
  
    // End render pass.
    if (currentRenderPass) {
        auto* command = drawCommandList->newCommand<EndRenderPassCommand>();
        command->type = DrawCommandType::EndRenderPass;
        command->renderPass = currentRenderPass;
    }
}

} // namespace kanata
} // namespace ln

