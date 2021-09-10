
#include "Internal.hpp"
#include <LuminoEngine/Base/MixHash.hpp>
#include "GraphicsDeviceContext.hpp"
#include "RHIObjectCache.hpp"

namespace ln {
namespace detail {
	
//=============================================================================
// NativeRenderPassCache

NativeRenderPassCache::NativeRenderPassCache(IGraphicsDevice* device)
	: m_device(device)
	, m_hashMap()
{
	assert(m_device);
}

void NativeRenderPassCache::clear()
{
	for (auto& pair : m_hashMap) {
		pair.second.value->dispose();
	}
	m_hashMap.clear();
	DiagnosticsManager::activeDiagnostics()->setCounterValue(ProfilingItem::Graphics_RenderPassCount, m_hashMap.size());
}

IRenderPass* NativeRenderPassCache::findOrCreate(const FindKey& key)
{
	uint64_t hash = computeHash(key);
	auto itr = m_hashMap.find(hash);
	if (itr != m_hashMap.end()) {
		itr->second.referenceCount++;
		return itr->second.value;
	}
	else {
		 DeviceFramebufferState buffers;
		size_t i = 0;
		for (; i < key.renderTargets.size(); i++) {
			if (!key.renderTargets[i]) break;
			buffers.renderTargets[i] = key.renderTargets[i];
		}
		buffers.depthBuffer = key.depthBuffer;

		auto renderPass = m_device->createRenderPass(buffers, key.clearFlags, key.clearColor, key.clearDepth, key.clearStencil);
		if (!renderPass) {
			return nullptr;
		}

		m_hashMap.insert({ hash, { 1, renderPass } });
		DiagnosticsManager::activeDiagnostics()->setCounterValue(ProfilingItem::Graphics_RenderPassCount, m_hashMap.size());
		return renderPass;
	}
}

void NativeRenderPassCache::invalidate(RHIResource* renderTarget)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second.value->containsRenderTarget(renderTarget)) {
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void NativeRenderPassCache::invalidate(IDepthBuffer* depthBuffer)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second.value->containsDepthBuffer(depthBuffer)) {
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}
//
//void NativeRenderPassCache::release(IRenderPass* value)
//{
//	if (value) {
//		auto itr = m_hashMap.find(value->cacheKeyHash);
//		if (itr != m_hashMap.end()) {
//			itr->second.referenceCount--;
//			if (itr->second.referenceCount <= 0) {
//				// TODO: 削除しない。すぐに削除されても、また同じパラメータで使うかもしれない。
//				// 後々最大数の設定は必要だろう。その時も、次回 create するときに空きが無ければ消すようにしたい。
//				//m_hashMap.erase(itr);
//			}
//		}
//	}
//}
//
uint64_t NativeRenderPassCache::computeHash(const FindKey& key)
{
	MixHash hash;
	for (size_t i = 0; i < key.renderTargets.size(); i++) {
		// TODO: Format だけでもいいかも。Vulkan はそうだった。
		hash.add(key.renderTargets[i]);
	}
	hash.add(key.depthBuffer);  // TODO: Format だけでもいいかも。Vulkan はそうだった。
	hash.add(key.clearFlags);   // TODO: 以下、一つの構造体にまとめれば少し高速化できそう
    hash.add(key.clearColor);
    hash.add(key.clearDepth);
    hash.add(key.clearStencil);
	return hash.value();
}

//=============================================================================
// NativePipelineCache

NativePipelineCache::NativePipelineCache(IGraphicsDevice* device)
	: m_device(device)
	, m_hashMap()
{
	assert(m_device);
}

void NativePipelineCache::clear()
{
	for (auto& pair : m_hashMap) {
		pair.second->dispose();
	}
	m_hashMap.clear();
	m_device = nullptr;
}

IPipeline* NativePipelineCache::findOrCreate(const FindKey& key)
{
	if (LN_REQUIRE(m_device)) return nullptr;

	uint64_t hash = computeHash(key);
	auto itr = m_hashMap.find(hash);
	if (itr != m_hashMap.end()) {
		return itr->second;
	}
	else {
		auto pipeline = m_device->createPipeline(key);
		if (!pipeline) {
			return nullptr;
		}

		m_hashMap.insert({ hash, pipeline });
		return pipeline;
	}
}

void NativePipelineCache::invalidate(IVertexDeclaration* value)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second->m_sourceVertexLayout == value) {
			itr->second->dispose();
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void NativePipelineCache::invalidate(IRenderPass* value)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second->m_sourceRenderPass == value) {
			itr->second->dispose();
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}

void NativePipelineCache::invalidate(IShaderPass* value)
{
	for (auto itr = m_hashMap.begin(); itr != m_hashMap.end(); ) {
		if (itr->second->m_sourceShaderPass == value) {
			itr->second->dispose();
			itr = m_hashMap.erase(itr);
		}
		else {
			++itr;
		}
	}
}

//void NativePipelineCache::invalidate(IPipeline* value)
//{
//	if (value) {
//		m_hashMap.erase(value->cacheKeyHash);
//	}
//}

uint64_t NativePipelineCache::computeHash(const FindKey& key)
{
	uint64_t vertexDeclarationHash = (key.vertexDeclaration) ? key.vertexDeclaration->hash() : 0;

	MixHash hash;
	hash.add(key.blendState);
	hash.add(key.rasterizerState);
	hash.add(key.depthStencilState);
	hash.add(key.topology);
	hash.add(vertexDeclarationHash);
	hash.add(key.shaderPass);
	hash.add(key.renderPass);
	return hash.value();


	//uint64_t vertexDeclarationHash = (key.state.pipelineState.vertexDeclaration) ? key.state.pipelineState.vertexDeclaration->hash() : 0;

	//MixHash hash;
	//hash.add(key.state.pipelineState.blendState);
	//hash.add(key.state.pipelineState.rasterizerState);
	//hash.add(key.state.pipelineState.depthStencilState);
	//hash.add(key.state.pipelineState.topology);
	//hash.add(vertexDeclarationHash);
	//hash.add(key.state.shaderPass);
	//hash.add(key.renderPass);
	//return hash.value();
}

} // namespace detail
} // namespace ln

