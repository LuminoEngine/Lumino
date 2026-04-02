#include <LuminoCore/graphics/PipelineCache.hpp>
#include <LuminoCore/graphics/Vertex.hpp>
#include <functional>

namespace ln {

//------------------------------------------------------------------------------
// boost-style hash mixing
//------------------------------------------------------------------------------

template<typename T>
static void hash_combine(size_t& seed, const T& val) {
    seed ^= std::hash<T>{}(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

//------------------------------------------------------------------------------
// PipelineCacheKey
//------------------------------------------------------------------------------

bool PipelineCacheKey::operator==(const PipelineCacheKey& o) const {
    if (vertexShader   != o.vertexShader)   return false;
    if (fragmentShader != o.fragmentShader) return false;
    if (vertexEntry    != o.vertexEntry)    return false;
    if (fragmentEntry  != o.fragmentEntry)  return false;
    if (cullMode       != o.cullMode)       return false;
    if (blendEnabled   != o.blendEnabled)   return false;
    if (blendEnabled) {
        if (blendState.enabled  != o.blendState.enabled)  return false;
        if (blendState.srcColor != o.blendState.srcColor) return false;
        if (blendState.dstColor != o.blendState.dstColor) return false;
        if (blendState.colorOp  != o.blendState.colorOp)  return false;
        if (blendState.srcAlpha != o.blendState.srcAlpha) return false;
        if (blendState.dstAlpha != o.blendState.dstAlpha) return false;
        if (blendState.alphaOp  != o.blendState.alphaOp)  return false;
    }
    if (depthTestEnabled  != o.depthTestEnabled)  return false;
    if (depthWriteEnabled != o.depthWriteEnabled) return false;
    if (stencilTestEnabled != o.stencilTestEnabled) return false;
    if (stencilTestEnabled) {
        if (stencilFront.compare     != o.stencilFront.compare)     return false;
        if (stencilFront.failOp      != o.stencilFront.failOp)      return false;
        if (stencilFront.depthFailOp != o.stencilFront.depthFailOp) return false;
        if (stencilFront.passOp      != o.stencilFront.passOp)      return false;
        if (stencilBack.compare      != o.stencilBack.compare)      return false;
        if (stencilBack.failOp       != o.stencilBack.failOp)       return false;
        if (stencilBack.depthFailOp  != o.stencilBack.depthFailOp)  return false;
        if (stencilBack.passOp       != o.stencilBack.passOp)       return false;
        if (stencilReadMask  != o.stencilReadMask)  return false;
        if (stencilWriteMask != o.stencilWriteMask) return false;
    }
    if (colorWriteEnabled != o.colorWriteEnabled) return false;
    if (pipelineLayout    != o.pipelineLayout)    return false;
    if (topology          != o.topology)          return false;
    if (colorFormat       != o.colorFormat)       return false;
    if (depthStencilFormat != o.depthStencilFormat) return false;
    if (sampleCount       != o.sampleCount)       return false;
    return true;
}

size_t PipelineCacheKeyHash::operator()(const PipelineCacheKey& key) const {
    size_t seed = 0;
    hash_combine(seed, reinterpret_cast<uintptr_t>(key.vertexShader));
    hash_combine(seed, reinterpret_cast<uintptr_t>(key.fragmentShader));
    hash_combine(seed, key.vertexEntry);
    hash_combine(seed, key.fragmentEntry);
    hash_combine(seed, static_cast<uint32_t>(key.cullMode));
    hash_combine(seed, key.blendEnabled);
    if (key.blendEnabled) {
        hash_combine(seed, key.blendState.enabled);
        hash_combine(seed, static_cast<uint32_t>(key.blendState.srcColor));
        hash_combine(seed, static_cast<uint32_t>(key.blendState.dstColor));
        hash_combine(seed, static_cast<uint32_t>(key.blendState.colorOp));
        hash_combine(seed, static_cast<uint32_t>(key.blendState.srcAlpha));
        hash_combine(seed, static_cast<uint32_t>(key.blendState.dstAlpha));
        hash_combine(seed, static_cast<uint32_t>(key.blendState.alphaOp));
    }
    hash_combine(seed, key.depthTestEnabled);
    hash_combine(seed, key.depthWriteEnabled);
    hash_combine(seed, key.stencilTestEnabled);
    if (key.stencilTestEnabled) {
        hash_combine(seed, static_cast<uint32_t>(key.stencilFront.compare));
        hash_combine(seed, static_cast<uint32_t>(key.stencilFront.failOp));
        hash_combine(seed, static_cast<uint32_t>(key.stencilFront.depthFailOp));
        hash_combine(seed, static_cast<uint32_t>(key.stencilFront.passOp));
        hash_combine(seed, static_cast<uint32_t>(key.stencilBack.compare));
        hash_combine(seed, static_cast<uint32_t>(key.stencilBack.failOp));
        hash_combine(seed, static_cast<uint32_t>(key.stencilBack.depthFailOp));
        hash_combine(seed, static_cast<uint32_t>(key.stencilBack.passOp));
        hash_combine(seed, key.stencilReadMask);
        hash_combine(seed, key.stencilWriteMask);
    }
    hash_combine(seed, key.colorWriteEnabled);
    hash_combine(seed, reinterpret_cast<uintptr_t>(key.pipelineLayout));
    hash_combine(seed, static_cast<uint32_t>(key.topology));
    hash_combine(seed, static_cast<uint32_t>(key.colorFormat));
    hash_combine(seed, static_cast<uint32_t>(key.depthStencilFormat));
    hash_combine(seed, key.sampleCount);
    return seed;
}

//------------------------------------------------------------------------------
// PipelineCache
//------------------------------------------------------------------------------

PipelineCache::PipelineCache(rhi::Device* device)
    : m_device(device), m_alive(std::make_shared<bool>(true)) {}

PipelineCache::~PipelineCache() {
    *m_alive = false;
}

Result<rhi::RenderPipeline*> PipelineCache::getOrCreate(const PipelineCacheKey& key) {
    auto it = m_cache.find(key);
    if (it != m_cache.end()) {
        return it->second.get();
    }

    rhi::RenderPipelineDesc rpDesc;
    rpDesc.layout          = key.pipelineLayout;
    rpDesc.vertexShader    = key.vertexShader;
    rpDesc.fragmentShader  = key.fragmentShader;
    rpDesc.vertexEntry     = key.vertexEntry;
    rpDesc.fragmentEntry   = key.fragmentEntry;
    rpDesc.vertexBuffers   = {standardVertexLayout()};
    rpDesc.topology        = key.topology;
    rpDesc.cullMode        = key.cullMode;
    rpDesc.colorFormats    = {key.colorFormat};
    rpDesc.depthStencilFormat              = key.depthStencilFormat;
    rpDesc.depthStencil.depthTestEnable    = key.depthTestEnabled;
    rpDesc.depthStencil.depthWriteEnable   = key.depthWriteEnabled;
    rpDesc.depthStencil.stencilTestEnable  = key.stencilTestEnabled;
    rpDesc.depthStencil.stencilFront       = key.stencilFront;
    rpDesc.depthStencil.stencilBack        = key.stencilBack;
    rpDesc.depthStencil.stencilReadMask    = key.stencilReadMask;
    rpDesc.depthStencil.stencilWriteMask   = key.stencilWriteMask;
    rpDesc.sampleCount     = key.sampleCount;

    if (key.blendEnabled) {
        rhi::BlendState bs = key.blendState;
        bs.colorWriteEnabled = key.colorWriteEnabled;
        rpDesc.blendStates = {bs};
    } else {
        rhi::BlendState bs;
        bs.colorWriteEnabled = key.colorWriteEnabled;
        rpDesc.blendStates = {bs};
    }

    auto result = m_device->createRenderPipeline(rpDesc);
    if (!result) return tl::make_unexpected(result.error());

    auto [it2, inserted] = m_cache.emplace(key, std::move(*result));
    if (inserted) {
        trackObject(key.vertexShader);
        trackObject(key.fragmentShader);
        trackObject(key.pipelineLayout);
    }
    return it2->second.get();
}

void PipelineCache::clear() {
    m_cache.clear();
    m_trackedObjects.clear();
}

void PipelineCache::trackObject(rhi::RHIObject* obj) {
    if (!obj) return;
    if (m_trackedObjects.count(obj)) return;
    m_trackedObjects.insert(obj);
    auto alive = m_alive;
    obj->addDestroyCallback([this, alive](rhi::RHIObject* o) {
        if (!*alive) {
            LN_LOG_ERROR("PipelineCache already destroyed.");
            return;
        }
        evictByObject(o);
    });
}

void PipelineCache::evictByObject(rhi::RHIObject* obj) {
    for (auto it = m_cache.begin(); it != m_cache.end(); ) {
        const auto& k = it->first;
        if (k.vertexShader == obj || k.fragmentShader == obj || k.pipelineLayout == obj) {
            it = m_cache.erase(it);
        } else {
            ++it;
        }
    }
    m_trackedObjects.erase(obj);
}

} // namespace ln
