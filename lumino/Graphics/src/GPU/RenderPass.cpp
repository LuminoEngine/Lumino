/*
	移行 Note:
		map を各 GraphicsResource から CommandBuffer へ移動する。
		※ ネイティブの map (transfar) は RenderPass の外側でなければ使えないので、順序制御するため CommandBuffer に統合したい

		CommandBuffer と RenderPass は全て pool からインスタンスを得る。インスタンスを外側で保持し続けてはならない。
*/
#include "Internal.hpp"
#include <LuminoGraphics/detail/GraphicsManager.hpp>
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/GPU/DepthBuffer.hpp>
#include <LuminoGraphics/GPU/RenderPass.hpp>
#include "RenderPassCache.hpp"
#include "GraphicsProfiler.hpp"
#include <LuminoEngine/GraphicsRHI/GraphicsDeviceContext.hpp>

namespace ln {

//==============================================================================
// RenderPass

RenderPass* RenderPass::get(RenderTargetTexture* renderTarget) {
    detail::RenderPassCache::FindKey key;
    key.renderTargets[0] = renderTarget;
    return detail::GraphicsManager::instance()->renderPassCache()->getOrCreate(key);
}

RenderPass* RenderPass::get(
    RenderTargetTexture* renderTarget,
    DepthBuffer* depthBuffer,
    ClearFlags clearFlags,
    const Color& clearColor,
    float clearDepth,
    uint8_t clearStencil) {
    detail::RenderPassCache::FindKey key;
    key.renderTargets[0] = renderTarget;
    key.depthBuffer = depthBuffer;
    key.clearFlags = clearFlags;
    key.clearColor = clearColor;
    key.clearDepth = clearDepth;
    key.clearStencil = clearStencil;
    return detail::GraphicsManager::instance()->renderPassCache()->getOrCreate(key);
}

RenderPass::RenderPass()
    : m_manager(nullptr)
    , m_rhiObject()
    , m_renderTargets{}
    , m_depthBuffer()
    , m_clearFlags(ClearFlags::None)
    , m_clearColor(0, 0, 0, 0)
    , m_clearDepth(1.0f)
    , m_clearStencil(0x00)
    , m_dirty(true)
    , m_active(false)
    , m_externalRHIRenderPass(false)
    , m_freezed(false) {
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
    detail::GraphicsResourceInternal::manager(this)->profiler()->addRenderPass(this);
}

RenderPass::~RenderPass() {
    detail::GraphicsResourceInternal::manager(this)->profiler()->removeRenderPass(this);
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
}

void RenderPass::init() {
    Object::init();
}

void RenderPass::init(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer) {
    init();

    setRenderTarget(0, renderTarget);
    setDepthBuffer(depthBuffer);
}

void RenderPass::init(detail::IRenderPass* rhiRenderPass) {
    init();
    m_rhiObject = rhiRenderPass;
    m_rhiObjectNoClear = rhiRenderPass;
    auto viewSize = rhiRenderPass->viewSize();
    m_viewSize = SizeI(viewSize.width, viewSize.height);
    
    auto* rhiRenderTarget = rhiRenderPass->m_renderTargets[0];
    LN_CHECK(rhiRenderTarget);
    auto renderTarget = makeObject_deprecated<RenderTargetTexture>();
    detail::TextureInternal::resetRHIObject(renderTarget, rhiRenderTarget);
    setRenderTarget(0, renderTarget);

    m_externalRHIRenderPass = true;
}

void RenderPass::onDispose(bool explicitDisposing) {
    releaseRHI();

    Object::onDispose(explicitDisposing);
}

void RenderPass::setRenderTarget(int index, RenderTargetTexture* value) {
    if (LN_ASSERT(!m_freezed)) return;
    if (LN_ASSERT(!m_externalRHIRenderPass)) return;
    if (LN_ASSERT(!m_active)) return;
    if (LN_REQUIRE_RANGE(index, 0, GraphicsCommandList::MaxMultiRenderTargets)) return;

    if (m_renderTargets[index] != value) {
        m_renderTargets[index] = value;
        m_dirty = true;

        if (value && index == 0) {
            m_viewSize.width = value->width();
            m_viewSize.height = value->height();
        }
    }
}

RenderTargetTexture* RenderPass::renderTarget(int index) const {
    if (LN_REQUIRE_RANGE(index, 0, GraphicsCommandList::MaxMultiRenderTargets)) return nullptr;
    return m_renderTargets[index];
}

void RenderPass::setDepthBuffer(DepthBuffer* value) {
    if (LN_ASSERT(!m_freezed)) return;
    if (LN_ASSERT(!m_externalRHIRenderPass)) return;
    if (LN_ASSERT(!m_active)) return;
    if (m_depthBuffer != value) {
        m_depthBuffer = value;
        m_dirty = true;
    }
}

void RenderPass::setClearFlags(ClearFlags value) {
    if (LN_ASSERT(!m_freezed)) return;
    if (LN_ASSERT(!m_active)) return;
    if (m_clearFlags != value) {
        m_clearFlags = value;
        m_dirty = true;
    }
}

void RenderPass::setClearColor(const Color& value) {
    if (LN_ASSERT(!m_freezed)) return;
    if (LN_ASSERT(!m_active)) return;
    if (m_clearColor != value) {
        m_clearColor = value;
        m_dirty = true;
    }
}

void RenderPass::setClearDepth(float value) {
    if (LN_ASSERT(!m_freezed)) return;
    if (LN_ASSERT(!m_active)) return;
    if (m_clearDepth != value) {
        m_clearDepth = value;
        m_dirty = true;
    }
}

void RenderPass::setClearStencil(uint8_t value) {
    if (LN_ASSERT(!m_freezed)) return;
    if (LN_ASSERT(!m_active)) return;
    if (m_clearStencil != value) {
        m_clearStencil = value;
        m_dirty = true;
    }
}

void RenderPass::setClearValues(ClearFlags flags, const Color& color, float depth, uint8_t stencil) {
    if (LN_ASSERT(!m_freezed)) return;
    if (LN_ASSERT(!m_active)) return;
    setClearFlags(flags);
    setClearColor(color);
    setClearDepth(depth);
    setClearStencil(stencil);
}

DepthBuffer* RenderPass::depthBuffer() const {
    return m_depthBuffer;
}

bool RenderPass::containsRenderTarget(RenderTargetTexture* renderTarget) const {
    return std::find(m_renderTargets.begin(), m_renderTargets.end(), renderTarget) != m_renderTargets.end();
}

bool RenderPass::containsDepthBuffer(DepthBuffer* depthBuffer) const {
    return m_depthBuffer == depthBuffer;
}

void RenderPass::onChangeDevice(detail::IGraphicsDevice* device) {
    if (LN_ASSERT(!m_active)) return;
    if (!device) {
        releaseRHI();
    }
    else {
        m_dirty = true; // create with next resolveRHIObject
    }
}

detail::IRenderPass* RenderPass::resolveRHIObject(GraphicsCommandList* context, bool* outModified) {
    if (m_externalRHIRenderPass) {
        return m_rhiObject;
    }

    if (m_dirty) {
        releaseRHI();
        m_dirty = false;

        const RenderTargetTexture* primaryTarget = m_renderTargets[0];
        if (LN_ASSERT(primaryTarget, "RenderPass: [0] Invalid render target.")) return nullptr;
        const SizeI primarySize(primaryTarget->width(), primaryTarget->height());

        detail::NativeRenderPassCache::FindKey key;
        for (auto i = 0; i < m_renderTargets.size(); i++) {
            RenderTargetTexture* rt = m_renderTargets[i];
            if (rt) {
                if (LN_ASSERT(rt->width() == primarySize.width && rt->height() == primarySize.height, _TT("RenderPass: Invalid render target dimensions."))) return nullptr;
            }

            key.renderTargets[i] = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(context, rt, nullptr);
        }
        key.depthBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(context, m_depthBuffer, nullptr);
        key.clearFlags = m_clearFlags;
        key.clearColor = m_clearColor;
        key.clearDepth = m_clearDepth;
        key.clearStencil = m_clearStencil;

        if (!m_renderTargets[0]->m_cleared && !testFlag(key.clearFlags, ClearFlags::Color)) {
            key.clearFlags = Flags<ClearFlags>(key.clearFlags) | ClearFlags::Color;
            m_dirty = true;
            //LN_ERROR(); // 自動的に RenderTarget クリア用の RenderPass に差し替えると、キャッシュとの整合性をとるのが大変になる
        }
        m_renderTargets[0]->m_cleared = true;

        if (m_depthBuffer) {
            if (LN_ASSERT(m_depthBuffer->width() == primarySize.width && m_depthBuffer->height() == primarySize.height, _TT("RenderPass: Invalid depth buffer dimensions."))) return nullptr;

            if (!m_depthBuffer->m_cleared && !testFlag(key.clearFlags, Flags<ClearFlags>(ClearFlags::Depth | ClearFlags::Stencil).get())) {
                key.clearFlags = Flags<ClearFlags>(key.clearFlags) | ClearFlags::Depth | ClearFlags::Stencil;
                m_dirty = true;
            }
            m_depthBuffer->m_cleared = true;
        }

        auto device = detail::GraphicsResourceInternal::manager(this)->deviceContext();
        m_rhiObject = device->renderPassCache()->findOrCreate(key);

        key.clearFlags = ClearFlags::None;
        key.clearColor = Color(0, 0, 0, 0);
        key.clearDepth = 1.0f;
        key.clearStencil = 0x00;
        m_rhiObjectNoClear = device->renderPassCache()->findOrCreate(key);
    }
    return m_rhiObject;
}

// [2019/10/5]
// GraphicsCommandList はデータ転送を遅延実行するため、各種 resolve (vkCmdCopyBuffer()) が呼ばれるタイミングが RenderPass の内側に入ってしまう。
//
// 多分グラフィックスAPIとして正しいであろう対策は、GraphicsCommandList に map, unmap, setData 等を実装して、resolve の遅延実行をやめること。
// ただ、dynamic なリソース更新するところすべてで GraphicsCommandList が必要になるので、描画に制限が多くなるし、GraphicsCommandList の取り回しを考えないとならない。
// 制限として厄介なのは DebugRendering. 各 onUpdate() の時点で何か描きたいときは GraphicsCommandList が確定していないのでいろいろ制約を考える必要がある。
// 特に、onUpdate 1度に対して複数 SwapChain から world を覗きたいときとか。
//
// もうひとつ、泥臭いけど今のところあまり時間掛けないで回避できるのが、この方法。
detail::IRenderPass* RenderPass::resolveRHIObjectNoClear(GraphicsCommandList* context, bool* outModified) {
    if (m_externalRHIRenderPass) {
        return m_rhiObjectNoClear;
    }

    resolveRHIObject(context, outModified);
    return m_rhiObjectNoClear;
}

void RenderPass::releaseRHI() {
    if (m_rhiObject) {
        //auto device = detail::GraphicsResourceInternal::manager(this)->deviceContext();
        //device->renderPassCache()->release(m_rhiObject);
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
