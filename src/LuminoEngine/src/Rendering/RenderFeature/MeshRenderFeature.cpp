﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Shader/ShaderDescriptor.hpp>
#include <LuminoEngine/Mesh/MeshModel.hpp>
#include "../../Graphics/GraphicsManager.hpp"
#include "../../Mesh/MeshModelInstance.hpp"
#include "../RenderingManager.hpp"
#include "MeshRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// MeshRenderFeature

MeshRenderFeature::MeshRenderFeature()
{
}

void MeshRenderFeature::init(RenderingManager* manager)
{
	RenderFeature::init();
}

RequestBatchResult MeshRenderFeature::drawMesh(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, MeshResource* mesh, int sectionIndex)
{
	if (LN_REQUIRE(mesh != nullptr)) return RequestBatchResult::Staging;

	auto result = attemptSubmitBatch(context, batchList, true);

	MeshSection section;
	VertexLayout* layout;
	VertexBuffer* vb[MaxVertexStreams] = {};
	int vbCount;
	IndexBuffer* ib;
	mesh->commitRenderData(sectionIndex, &section, &layout, vb, &vbCount, &ib);

	DrawMeshData data;
	data.vertexLayout = layout;
	for (int i = 0; i < vbCount; ++i) {
		data.vertexBuffers[i] = vb[i];
	}
	data.vertexBuffersCount = vbCount;
	data.indexBuffer = ib;
	data.startIndex = section.startIndex;
	data.primitiveCount = section.primitiveCount;
	data.primitiveType = PrimitiveTopology::TriangleList;
	data.instanceCount = 0;

	if (LN_REQUIRE(data.vertexBuffers[0])) return RequestBatchResult::Staging;
	if (data.primitiveCount <= 0) return RequestBatchResult::Staging;

	m_meshes.push_back(std::move(data));
	m_batchData.count++;

	return result;
}

RequestBatchResult MeshRenderFeature::drawMesh(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, MeshPrimitive* mesh, int sectionIndex, detail::SkeletonInstance* skeleton, detail::MorphInstance* morph)
{
    if (LN_REQUIRE(mesh != nullptr)) return RequestBatchResult::Staging;

	auto result = attemptSubmitBatch(context, batchList, true);

    MeshSection2 section;
    VertexLayout* layout;
    std::array<VertexBuffer*, MaxVertexStreams> vb = {};
    int vbCount;
    IndexBuffer* ib;
    mesh->commitRenderData(sectionIndex, morph, &section, &layout, &vb, &vbCount, &ib);

    DrawMeshData data;
    data.vertexLayout = layout;
    for (int i = 0; i < vbCount; ++i) {
        data.vertexBuffers[i] = vb[i];
    }
    data.vertexBuffersCount = vbCount;
    data.indexBuffer = ib;
    data.startIndex = section.startIndex;
    data.primitiveCount = section.primitiveCount;
    data.primitiveType = section.topology;
	data.instanceCount = 0;

    if (LN_REQUIRE(data.vertexBuffers[0])) return RequestBatchResult::Staging;
    if (data.primitiveCount <= 0) return RequestBatchResult::Staging;



	if (morph) {
		std::array<float, MaxRenderMorphTargets> weights;
		bool hasAnyWeight = morph->getMorphWeights(&weights);
		if (hasAnyWeight) {

			const auto& commandList = context->commandList();
			const int targetCount = mesh->morphTargetCount();



			struct BlendInfo
			{
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

			const auto& shader = detail::EngineDomain::renderingManager()->blendShapeShader;
			ShaderSecondaryDescriptor* descriptor = commandList->acquireShaderDescriptor(shader.shader);
			descriptor->setStorageData(shader.dstVerticesGID, morph->m_blendResult);
			descriptor->setTexture(shader.srcVerticesGID, vb[0]);
			if (targetCount >= 1) descriptor->setTexture(shader.target0GID, targets[0]); else descriptor->setTexture(shader.target0GID, nullptr);
			if (targetCount >= 2) descriptor->setTexture(shader.target1GID, targets[1]); else descriptor->setTexture(shader.target1GID, targets[0]);
			if (targetCount >= 3) descriptor->setTexture(shader.target2GID, targets[2]); else descriptor->setTexture(shader.target2GID, targets[0]);
			if (targetCount >= 4) descriptor->setTexture(shader.target3GID, targets[3]); else descriptor->setTexture(shader.target3GID, targets[0]);
			descriptor->setUniformBuffer(shader.blendInfoGID, view);
			descriptor->setUniformBufferData(shader.blendInfoGID, &blendInfo, sizeof(blendInfo));

			context->setShaderPass(shader.shaderPass);
			context->setShaderDescriptor(descriptor);
			context->dispatch(std::max(1, mesh->vertexCount() / 64), 1, 1);

			//descriptor->setst

			//	batch->morph->getMorphWeights(&elementInfo.morphWeights);

			data.vertexBuffers[0] = morph->m_blendResult;
		}

	}
	//else {
	//	std::fill(elementInfo.morphWeights.begin(), elementInfo.morphWeights.end(), 0.0f);
	//}


    m_meshes.push_back(std::move(data));
    m_batchData.count++;
	m_batchData.skeleton = skeleton;
	m_batchData.morph = morph;


	return result;
}

RequestBatchResult MeshRenderFeature::drawMeshInstanced(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, InstancedMeshList* list)
{
	if (LN_REQUIRE(list != nullptr)) return RequestBatchResult::Staging;


	auto result = attemptSubmitBatch(context, batchList, true);

	MeshSection2 section;
	VertexLayout* layout;
	std::array<VertexBuffer*, MaxVertexStreams> vb = {};
	int vbCount;
	IndexBuffer* ib;
	list->commitRenderData(&section, &layout, &vb, &vbCount, &ib);

	DrawMeshData data;
	data.vertexLayout = layout;
	for (int i = 0; i < vbCount; ++i) {
		data.vertexBuffers[i] = vb[i];
	}
	data.vertexBuffersCount = vbCount;
	data.indexBuffer = ib;
	data.startIndex = section.startIndex;
	data.primitiveCount = section.primitiveCount;
	data.primitiveType = section.topology;
	data.instanceCount = list->instanceCount();

	if (LN_REQUIRE(data.vertexBuffers[0])) return RequestBatchResult::Staging;
	if (data.primitiveCount <= 0) return RequestBatchResult::Staging;

	m_meshes.push_back(std::move(data));
	m_batchData.count++;
	m_batchData.instanced = true;

	return result;
}

RequestBatchResult MeshRenderFeature::attemptSubmitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList, bool instanced)
{
	if (m_batchData.count == 0) {
		return RequestBatchResult::Staging;	// 初回なので Submit する必要なし
	}
	//else if (batchList->lastBatch()->instancing != instanced) {
	else if (m_batchData.instanced != instanced) {
		submitBatch(context, batchList);
		return RequestBatchResult::Submitted;
	}
	else {
		return RequestBatchResult::Staging;
	}
}

void MeshRenderFeature::beginRendering()
{
	m_meshes.clear();
	m_batchData.offset = 0;
	m_batchData.count = 0;
	m_batchData.instanced = false;
	m_batchData.skeleton = nullptr;
	m_batchData.morph = nullptr;
}

void MeshRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;
	batch->instancing = m_batchData.instanced;
	batch->skeleton = m_batchData.skeleton;
	batch->morph = m_batchData.morph;

	m_batchData.offset = m_batchData.offset + m_batchData.count;
	m_batchData.count = 0;
	m_batchData.instanced = false;
	m_batchData.skeleton = nullptr;
	m_batchData.morph = nullptr;
}

void MeshRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);

	for (int i = 0; i < localBatch->data.count; i++) {
		auto& data = m_meshes[localBatch->data.offset + i];

		context->setVertexLayout(data.vertexLayout);
		for (int i = 0; i < data.vertexBuffersCount; ++i) {
			context->setVertexBuffer(i, data.vertexBuffers[i]);
		}
		context->setPrimitiveTopology(data.primitiveType);  // TODO: これは RenderingContext がわでやるようになっている
		if (data.indexBuffer) {
			context->setIndexBuffer(data.indexBuffer);
			context->drawPrimitiveIndexed(data.startIndex, data.primitiveCount, data.instanceCount);
		}
		else {
			context->drawPrimitive(data.startIndex, data.primitiveCount);
		}
	}
}

} // namespace detail
} // namespace ln

