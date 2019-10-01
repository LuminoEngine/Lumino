﻿/*
	移行 Note:
		map を各 GraphicsResource から CommandBuffer へ移動する。
		※ ネイティブの map (transfar) は RenderPass の外側でなければ使えないので、順序制御するため CommandBuffer に統合したい

		CommandBuffer と RenderPass は全て pool からインスタンスを得る。インスタンスを外側で保持し続けてはならない。
*/
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include "GraphicsManager.hpp"
#include "GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// RenderPass

RenderPass::RenderPass()
    : m_rhiObject()
	, m_renderTargets{}
	, m_depthBuffer()
	, m_clearFlags(ClearFlags::None)
	, m_clearColor(0, 0, 0, 0)
	, m_clearDepth(1.0f)
	, m_clearStencil(0x00)
	, m_dirty(true)
	, m_active(false)
{
}

RenderPass::~RenderPass()
{
}

void RenderPass::init()
{
    GraphicsResource::init();
}

void RenderPass::init(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	init();
	setRenderTarget(0, renderTarget);
	setDepthBuffer(depthBuffer);
}

void RenderPass::onDispose(bool explicitDisposing)
{
	releaseRHI();
    GraphicsResource::onDispose(explicitDisposing);
}

void RenderPass::setRenderTarget(int index, RenderTargetTexture* value)
{
	if (LN_REQUIRE(!m_active)) return;
	if (LN_REQUIRE_RANGE(index, 0, GraphicsContext::MaxMultiRenderTargets)) return;

	if (m_renderTargets[index] != value) {
		m_renderTargets[index] = value;
		m_dirty = true;
	}
}

RenderTargetTexture* RenderPass::renderTarget(int index) const
{
	if (LN_REQUIRE_RANGE(index, 0, GraphicsContext::MaxMultiRenderTargets)) return nullptr;
	return m_renderTargets[index];
}

void RenderPass::setDepthBuffer(DepthBuffer* value)
{
	if (LN_REQUIRE(!m_active)) return;
	if (m_depthBuffer != value) {
		m_depthBuffer = value;
		m_dirty = true;
	}
}

void RenderPass::setClearFlags(ClearFlags value)
{
	if (LN_REQUIRE(!m_active)) return;
	if (m_clearFlags != value) {
		m_clearFlags = value;
		m_dirty = true;
	}
}

void RenderPass::setClearColor(const Color& value)
{
	if (LN_REQUIRE(!m_active)) return;
	if (m_clearColor != value) {
		m_clearColor = value;
		m_dirty = true;
	}
}

void RenderPass::setClearDepth(float value)
{
	if (LN_REQUIRE(!m_active)) return;
	if (m_clearDepth != value) {
		m_clearDepth = value;
		m_dirty = true;
	}
}

void RenderPass::setClearStencil(uint8_t value)
{
	if (LN_REQUIRE(!m_active)) return;
	if (m_clearStencil != value) {
		m_clearStencil = value;
		m_dirty = true;
	}
}

void RenderPass::setClearValues(ClearFlags flags, const Color& color, float depth, uint8_t stencil)
{
	if (LN_REQUIRE(!m_active)) return;
	setClearFlags(flags);
	setClearColor(color);
	setClearDepth(depth);
	setClearStencil(stencil);
}

DepthBuffer* RenderPass::depthBuffer() const
{
	return m_depthBuffer;
}

void RenderPass::onChangeDevice(detail::IGraphicsDevice* device)
{
	if (LN_REQUIRE(!m_active)) return;
	if (!device) {
		releaseRHI();
	}
	else {
		m_dirty = true;	// create with next resolveRHIObject
	}
}

detail::IRenderPass* RenderPass::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	if (m_dirty) {
		releaseRHI();

		detail::NativeRenderPassCache::FindKey key;
		for (auto i = 0; i < m_renderTargets.size(); i++) {
			key.renderTargets[i] = detail::GraphicsResourceInternal::resolveRHIObject<detail::ITexture>(context, m_renderTargets[i], nullptr);
		}
		key.depthBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::IDepthBuffer>(context, m_depthBuffer, nullptr);
		key.clearFlags = m_clearFlags;
		key.clearColor = m_clearColor;
		key.clearDepth = m_clearDepth;
		key.clearStencil = m_clearStencil;

		auto device = detail::GraphicsResourceInternal::manager(this)->deviceContext();
		m_rhiObject = device->renderPassCache()->findOrCreate(key);

		m_dirty = false;
	}
	return m_rhiObject;
}

void RenderPass::releaseRHI()
{
	if (m_rhiObject) {
		auto device = detail::GraphicsResourceInternal::manager(this)->deviceContext();
		device->renderPassCache()->release(m_rhiObject);
		m_rhiObject = nullptr;
	}
}

////==============================================================================
//// RenderPassPool
//namespace detail {
//
//RenderPassPool::RenderPassPool(GraphicsManager* manager)
//{
//}
//
//RenderPass* RenderPassPool::findOrCreate(const FindKey& key)
//{
//	uint64_t hash = computeHash(key);
//	auto itr = m_hashMap.find(hash);
//	if (itr != m_hashMap.end()) {
//		return itr->second;
//	}
//	else {
//		// TODO: Pool を使い切ったら、使っていないものを消す
//
//		auto pipeline = m_device->createPipeline(key.renderPass, key.state);
//		if (!pipeline) {
//			return nullptr;
//		}
//
//		m_hashMap.insert({ hash, pipeline });
//		return pipeline;
//	}
//}
//
//uint64_t RenderPassPool::computeHash(const FindKey& key)
//{
//}
//
//} // namespace detail
} // namespace ln
