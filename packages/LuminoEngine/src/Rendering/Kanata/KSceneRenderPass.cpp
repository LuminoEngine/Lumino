#include <LuminoEngine/Base/MixHash.hpp>
#include <LuminoEngine/Graphics/GraphicsManager.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderDescriptor.hpp>
#include <LuminoEngine/Graphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawEvent.hpp>
#include <LuminoEngine/Rendering/Kanata/KSceneRenderPass.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Mesh/MeshModeEntity.hpp>
#include "../SceneRenderer.hpp"

namespace ln {
namespace kanata {

SceneRenderPass::SceneRenderPass(
    detail::RenderingManager* manager,
    Shader* fallbackShader,
    kokage::ShaderTechniqueClass_Phase phase)
    : m_manager(manager)
    , m_fallbackShader(fallbackShader)
    , m_phase(phase) {
}

void SceneRenderPass::buildDrawEvents(
    detail::SceneRenderer* sceneRenderer,
    const BatchCollector* batchList,
    GraphicsCommandList* descriptorAllocator,
    RenderPass* renderPass,
    const detail::RenderViewInfo& renderViewInfo,
    const detail::SceneInfo& sceneInfo,
    DrawEventList* drawEventList) {
    Shader* fallbackShader = this->fallbackShader();
    RenderPass* currentRenderPass = nullptr;

    if (!batchList->batches().isEmpty()) {
        for (const Batch* batch : batchList->batches()) {
            // RenderPass
            //   無駄な beginRenderPass を避けるため、引数で受け取った renderPass をいきなり begin せず、
            //   Batch に付いている renderPass を begin する。
            {
                auto* batchRenderPass = (batch->renderPass) ? batch->renderPass : renderPass;
                if (batchRenderPass != currentRenderPass) {
                    // End render pass.
                    if (currentRenderPass) {
                        auto* ev = drawEventList->newDrawEvent<EndRenderPassDrawEvent>();
                        ev->type = DrawEventType::EndRenderPass;
                        ev->renderPass = currentRenderPass;
                    }

                    LN_DCHECK(batchRenderPass);
                    currentRenderPass = batchRenderPass;

                    // Begin render pass.
                    {
                        auto* ev = drawEventList->newDrawEvent<BeginRenderPassDrawEvent>();
                        ev->type = DrawEventType::BeginRenderPass;
                        ev->renderPass = currentRenderPass;
                    }
                }
            }

            const BatchMaterial& batchMaterial = batch->material;
            Material* material = batchMaterial.material;
            LN_DCHECK(material);

            // Make info per Batch
            detail::ElementInfo elementInfo;
            elementInfo.objectId = 0; // TODO:
            elementInfo.viewProjMatrix = &renderViewInfo.cameraInfo.viewProjMatrix;
            elementInfo.WorldMatrix = batch->worldTransform;
            elementInfo.WorldViewProjectionMatrix = elementInfo.WorldMatrix * (*elementInfo.viewProjMatrix);
            elementInfo.boneLocalQuaternionTexture = nullptr; // TODO: (MMD SDEF)
            if (batch->skeleton) {
                batch->skeleton->updateSkinningMatrices(descriptorAllocator);
                elementInfo.boneTexture = batch->skeleton->skinningMatricesTexture();
            }
            else {
                elementInfo.boneTexture = nullptr;
            }

            for (int32_t iBatchElement = 0; iBatchElement < batch->elementsCount; iBatchElement++) {
                const BatchElement& batchElement = batch->elemets2[iBatchElement];

                DrawEvent* drawEvent = drawEventList->newDrawEvent<DrawEvent>();
                drawEvent->type = DrawEventType::DrawPrimitive;
                drawEvent->vertexBuffers = batchElement.vertexBuffers;
                drawEvent->indexBuffer = batchElement.indexBuffer;
                drawEvent->pipelineState.vertexLayout = batch->vertexLayout;
                drawEvent->pipelineState.primitiveTopology = batch->primitiveTopology;
                drawEvent->firstIndex = batchElement.firstIndex;
                drawEvent->firstVertex = batchElement.firstVertex;
                drawEvent->primitiveCount = batchElement.primitiveCount;
                drawEvent->instanceCount = batchElement.instanceCount;

                // RenderState
                {
                    // BlendState
                    {
                        drawEvent->pipelineState.blendState.independentBlendEnable = false;
                        makeBlendMode(batchMaterial.blendMode, &drawEvent->pipelineState.blendState.renderTargets[0]);
                    }
                    // RasterizerState
                    {
                        drawEvent->pipelineState.rasterizerState.fillMode = FillMode::Solid;
                        drawEvent->pipelineState.rasterizerState.cullMode = batchMaterial.cullingMode;
                    }
                    // DepthStencilState
                    {
                        drawEvent->pipelineState.depthStencilState.depthTestFunc = (batchMaterial.depthTestEnabled) ? ComparisonFunc::LessEqual : ComparisonFunc::Always;
                        drawEvent->pipelineState.depthStencilState.depthWriteEnabled = batchMaterial.depthWriteEnabled;
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
                    // detail::ShaderTechniqueRequestClasses requester = {
                    //     (elementInfo.boneTexture) ? kokage::ShaderTechniqueClass_MeshProcess::SkinnedMesh : kokage::ShaderTechniqueClass_MeshProcess::StaticMesh,
                    //     (batchElement.instanceCount > 0) ? kokage::ShaderTechniqueClass_DrawMode::Instancing : kokage::ShaderTechniqueClass_DrawMode::Primitive,
                    //     (subsetInfo.normalMap) ? kokage::ShaderTechniqueClass_Normal::NormalMap : kokage::ShaderTechniqueClass_Normal::Default,
                    //     (subsetInfo.metallicRoughnessTexture) ? kokage::ShaderTechniqueClass_Roughness::RoughnessMap : kokage::ShaderTechniqueClass_Roughness::Default,
                    // };

                    // kokage::ShaderTechniqueClass classSet;
                    // classSet.defaultTechnique = false;
                    // classSet.phase = kokage::ShaderTechniqueClass_Phase::Forward;
                    // classSet.meshProcess = requester.meshProcess;
                    // classSet.drawMode = requester.drawMode;
                    // classSet.shadingModel = kokage::ShaderTechniqueClass_ShadingModel::Default;
                    // classSet.normalClass = requester.normal;       // Normal を出力したいので考慮する
                    // classSet.roughnessClass = requester.roughness; // Roughness を出力したいので考慮する
                    tech = getShaderTechnique(fallbackShader, batch, &batchElement);
                    // if (!tech)
                    //     tech = fallbackShader->techniques()->front();
                }
#else
                ShaderTechnique* tech = actualShader->techniques()[0]; // shader->findTechniqueByVariantKey(0); // TODO:
#endif
                if (LN_ASSERT(tech)) return;

                // Resource fallback
                {
                    if (batch->overrideTexture) {
                        subsetInfo.materialTexture = batch->overrideTexture;
                        if (batch->overrideSamplerState) {
                            subsetInfo.materialTexture->setSamplerState(batch->overrideSamplerState);
                        }
                    }

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

                // Select ShaderPass
                ShaderPass* shaderPass = tech->passes()[0]; // TODO:
                const detail::ShaderPassSemanticsManager* semanticsManager = shaderPass->semanticsManager();

                // Allocate descriptor
                ShaderDescriptor* descriptor = descriptorAllocator->allocateDescriptor(shaderPass, true);
                drawEvent->shaderDescriptor = descriptor;
                drawEvent->shaderPass = shaderPass;

                // Setup descriptor
                semanticsManager->updateRenderViewVariables(descriptor, renderViewInfo, sceneInfo); // TODO: ここだと element ごとに呼ばれるのでかなり無駄が多い。事前計算しておいて、memcpy で済ませたい
                semanticsManager->updateElementVariables(descriptor, renderViewInfo.cameraInfo, elementInfo);
                semanticsManager->updateSubsetVariables(descriptor, subsetInfo);

                // Commit final material
                {
                    detail::PbrMaterialData pbrMaterialData = material->getPbrMaterialData();
                    semanticsManager->updateSubsetVariables_PBR(descriptor, pbrMaterialData);
                    material->updateShaderVariables2(descriptorAllocator, descriptor);

                    // TODO: これが実質 ClusteredShadingSceneRenderer 専用なので、なんとかしたいところ。
                    if (sceneRenderer) {
                        sceneRenderer->onSetAdditionalShaderPassVariables2(descriptor, shaderPass);
                    }
                }

                if (overrideCommand) {
                    overrideCommand(drawEvent);
                }
            }
        }
    }
    else {
        // Batch がひとつも無い場合は 引数で受け取った renderPass を開始しておかないと、クリア自体がされなくなる。
        auto* ev = drawEventList->newDrawEvent<BeginRenderPassDrawEvent>();
        ev->type = DrawEventType::BeginRenderPass;
        ev->renderPass = renderPass;
        currentRenderPass = renderPass;
    }
  
    // End render pass.
    if (currentRenderPass) {
        auto* ev = drawEventList->newDrawEvent<EndRenderPassDrawEvent>();
        ev->type = DrawEventType::EndRenderPass;
        ev->renderPass = currentRenderPass;
    }
}

ShaderTechnique* SceneRenderPass::getShaderTechnique(Shader* fallbackShader, const Batch* batch, const BatchElement* batchElement) {
    Material* material = batch->material.material;
    
    // Select shader
    Shader* actualShader = fallbackShader;
    if (material) {
        Shader* materialShader = material->shader();
        if (materialShader) {
            actualShader = materialShader;
        }
    }

    bool hasNormalMap = false;
    bool hasMetallicRoughnessTexture = false;
    if (material) {
        hasNormalMap = material->normalMap() != nullptr;
        hasMetallicRoughnessTexture = material->metallicRoughnessTexture() != nullptr;
    }

    //kokage::ShaderTechniqueClass classSet;
    //classSet.phase = m_phase;
    //classSet.meshProcess = (batch->skeleton) ? kokage::ShaderTechniqueClass_MeshProcess::SkinnedMesh : kokage::ShaderTechniqueClass_MeshProcess::StaticMesh;
    //classSet.drawMode = (batchElement->instanceCount > 0) ? kokage::ShaderTechniqueClass_DrawMode::Instancing : kokage::ShaderTechniqueClass_DrawMode::Primitive;
    //classSet.normalClass = (hasNormalMap) ? kokage::ShaderTechniqueClass_Normal::NormalMap : kokage::ShaderTechniqueClass_Normal::Default; // Normal を出力したいので考慮する
    //classSet.roughnessClass = (hasMetallicRoughnessTexture) ? kokage::ShaderTechniqueClass_Roughness::RoughnessMap : kokage::ShaderTechniqueClass_Roughness::Default; // Roughness を出力したいので考慮する

    // TODO: constexpr にしたい
    static constexpr uint32_t LN_USE_INSTANCING = detail::MixHash::computeSingle("LN_USE_INSTANCING");
    static constexpr uint32_t LN_USE_SKINNING = detail::MixHash::computeSingle("LN_USE_SKINNING");
    static const uint32_t LN_USE_NORMALMAP = detail::MixHash::computeSingle("LN_USE_NORMALMAP");
    static const uint32_t LN_SHADING_DEFAULT = detail::MixHash::computeSingle("LN_SHADINGMODEL_DEFAULT");
    uint32_t LN_SHADING_UNLIT = detail::MixHash::computeSingle("LN_SHADINGMODEL_UNLIT");
    detail::MixHash variantKey;
    if (actualShader->hasAffectVariantKey(LN_USE_INSTANCING)) {
        if (batchElement->instanceCount > 0) {
            variantKey.addHash(LN_USE_INSTANCING);
        }
    }
    if (actualShader->hasAffectVariantKey(LN_USE_SKINNING)) {
        if (batch->skeleton) {
            variantKey.addHash(LN_USE_SKINNING);
        }
    }
    if (actualShader->hasAffectVariantKey(LN_USE_NORMALMAP)) {
        if (hasNormalMap) {
            variantKey.addHash(LN_USE_NORMALMAP);
        }
    }


    if (m_phase == kokage::ShaderTechniqueClass_Phase::Forward) {
        // ShadingMode は Default phase (Render to color-buffer) のみ有効
        switch (batch->material.shadingModel) {
            case ShadingModel::Default:
                //classSet.shadingModel = kokage::ShaderTechniqueClass_ShadingModel::Default;
                if (actualShader->hasAffectVariantKey(LN_SHADING_DEFAULT)) {
                    variantKey.addHash(LN_SHADING_DEFAULT);
                }
                break;
            case ShadingModel::Unlit:
                //classSet.shadingModel = kokage::ShaderTechniqueClass_ShadingModel::Unlit;
                if (actualShader->hasAffectVariantKey(LN_SHADING_UNLIT)) {
                    variantKey.addHash(LN_SHADING_UNLIT);
                }
                break;
            default:
                LN_UNREACHABLE();
                break;
        }


        // Default phase の場合は、ユーザー定義のシェーダに、条件にマッチする Technique が無かったとしても、Default にフォールバックしてみる。
        // つまり、少なくともユーザーがカスタマイズしたいと思った表現に近づけるようにする。
        ShaderTechnique* tech = actualShader->findTechniqueByVariantKey(variantKey.value(), false);
        //classSet.defaultTechnique = true;
        //ShaderTechnique* tech = detail::ShaderInternal::findTechniqueByClass(actualShader, classSet);
        LN_DCHECK(tech);
        return tech;
    }
    else {
        // Default phase 以外は、G-Buffer 作成などレンダリングエンジンの都合に合わせた Output が必要となるため、
        // ユーザーのシェーダが厳格にそれをオーバーライドしたい場合を除いて、fallbackShader にフォールバックする。
        ShaderTechnique* tech = actualShader->findTechniqueByVariantKey(variantKey.value(), true);
        //classSet.shadingModel = kokage::ShaderTechniqueClass_ShadingModel::Default;
        //ShaderTechnique* tech = detail::ShaderInternal::findTechniqueByClass(actualShader, classSet);
        if (tech)
            return tech;
        else
            return fallbackShader->techniques()->front();
    }
}

void SceneRenderPass::makeBlendMode(BlendMode mode, RenderTargetBlendDesc* state) {
    // もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
    // TODO: アルファも一緒のブレンド方式にしているので、個別指定で改善できそう
    switch (mode) {
        case BlendMode::Normal:
            state->blendEnable = false;
            state->sourceBlend = BlendFactor::One;
            state->destinationBlend = BlendFactor::Zero;
            state->blendOp = BlendOp::Add;
            state->sourceBlendAlpha = BlendFactor::One;
            state->destinationBlendAlpha = BlendFactor::Zero;
            state->blendOpAlpha = BlendOp::Add;
            break;
        case BlendMode::Alpha:
            state->blendEnable = true;
            state->sourceBlend = BlendFactor::SourceAlpha;
            state->destinationBlend = BlendFactor::InverseSourceAlpha;
            state->blendOp = BlendOp::Add;
            state->sourceBlendAlpha = BlendFactor::SourceAlpha;
            //state->destinationBlendAlpha = BlendFactor::InverseSourceAlpha;
            state->destinationBlendAlpha = BlendFactor::DestinationAlpha;
            state->blendOpAlpha = BlendOp::Add;
            break;
        case BlendMode::Add:
            state->blendEnable = true;
            state->sourceBlend = BlendFactor::SourceAlpha;
            state->destinationBlend = BlendFactor::One;
            state->blendOp = BlendOp::Add;
            state->sourceBlendAlpha = BlendFactor::SourceAlpha;
            state->destinationBlendAlpha = BlendFactor::One;
            state->blendOpAlpha = BlendOp::Add;
            break;
        case BlendMode::Subtract:
            state->blendEnable = true;
            state->sourceBlend = BlendFactor::SourceAlpha;
            state->destinationBlend = BlendFactor::One;
            state->blendOp = BlendOp::ReverseSubtract;
            state->sourceBlendAlpha = BlendFactor::SourceAlpha;
            state->destinationBlendAlpha = BlendFactor::One;
            state->blendOpAlpha = BlendOp::Add;
            break;
        case BlendMode::Multiply:
            state->blendEnable = true;
            state->sourceBlend = BlendFactor::
                Zero; // AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
            state->destinationBlend = BlendFactor::SourceColor;
            state->blendOp = BlendOp::Add;
            state->sourceBlendAlpha = BlendFactor::SourceAlpha;
            state->destinationBlendAlpha = BlendFactor::One;
            state->blendOpAlpha = BlendOp::Add;
            break;
            //case BlendMode_Screen:
            //	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            //	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
            //	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
            //	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
            //	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
            //	break;
            //case BlendMode_Reverse:
            //	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
            //	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
            //	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
            //	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
            //	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
            //	break;
        default:
            LN_UNREACHABLE();
            break;
    }
}

} // namespace kanata
} // namespace ln

