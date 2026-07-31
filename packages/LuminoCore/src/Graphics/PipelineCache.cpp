#include <LuminoCore/Graphics/PipelineCache.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
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
    if (shaderPass      != o.shaderPass)      return false;
    if (cullMode        != o.cullMode)        return false;
    if (blendState.enabled != o.blendState.enabled) return false;
    if (blendState.enabled) {
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
    if (topology          != o.topology)          return false;
    if (renderPass        != o.renderPass)        return false;
    return true;
}

size_t PipelineCacheKeyHash::operator()(const PipelineCacheKey& key) const {
    size_t seed = 0;
    hash_combine(seed, reinterpret_cast<uintptr_t>(key.shaderPass));
    hash_combine(seed, static_cast<uint32_t>(key.cullMode));
    hash_combine(seed, key.blendState.enabled);
    if (key.blendState.enabled) {
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
    hash_combine(seed, static_cast<uint32_t>(key.topology));
    hash_combine(seed, reinterpret_cast<uintptr_t>(key.renderPass));
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

    if (!key.shaderPass) {
        return LN_MAKE_ERROR("PipelineCacheKey has no shaderPass.");
    }

    rhi::RenderPipelineDesc rpDesc;
    // debugName は WebGPU オブジェクトのラベルになる。パイプライン生成の失敗は
    // `[Invalid RenderPipeline "<label>"]` として報告されるため、
    // どのシェーダのどのパスなのかが分かる名前を入れる。
    rpDesc.debugName       = key.shaderPass->debugLabel();
    rpDesc.layout          = key.shaderPass->pipelineLayout();
    rpDesc.vertexShader    = key.shaderPass->vertexShader();
    rpDesc.fragmentShader  = key.shaderPass->fragmentShader();
    rpDesc.vertexEntry     = key.shaderPass->vertexEntry();
    rpDesc.fragmentEntry   = key.shaderPass->fragmentEntry();
    rpDesc.vertexBuffers   = {standardVertexLayout()};
    rpDesc.topology        = key.topology;
    rpDesc.cullMode        = key.cullMode;
    rpDesc.renderPass      = key.renderPass;
    rpDesc.depthStencil.depthTestEnable    = key.depthTestEnabled;
    rpDesc.depthStencil.depthWriteEnable   = key.depthWriteEnabled;
    rpDesc.depthStencil.stencilTestEnable  = key.stencilTestEnabled;
    rpDesc.depthStencil.stencilFront       = key.stencilFront;
    rpDesc.depthStencil.stencilBack        = key.stencilBack;
    rpDesc.depthStencil.stencilReadMask    = key.stencilReadMask;
    rpDesc.depthStencil.stencilWriteMask   = key.stencilWriteMask;

    {
        rhi::BlendState bs = key.blendState;
        bs.colorWriteEnabled = key.colorWriteEnabled;
        rpDesc.blendStates = {bs};
    }

    auto result = m_device->createRenderPipeline(rpDesc);
    if (!result) {
        // シェーダ名とパス名を必ず載せる。WGSL の不正などでシェーダモジュールが
        // 無効になっている場合、バックエンドのメッセージだけでは
        // どのシェーダが原因なのか分からないことが多い。
        return LN_MAKE_ERROR(
            "Failed to create render pipeline. (shader: %s, pass: %s, vertexEntry: %s, fragmentEntry: %s) %s",
            key.shaderPass->shaderName().c_str(),
            key.shaderPass->passName().c_str(),
            key.shaderPass->vertexEntry().c_str(),
            key.shaderPass->fragmentEntry().c_str(),
            result.error().message.c_str());
    }

    auto [it2, inserted] = m_cache.emplace(key, std::move(*result));
    if (inserted) {
        trackObject(key.shaderPass->vertexShader());
        trackObject(key.shaderPass->fragmentShader());
        trackObject(key.shaderPass->pipelineLayout());
        trackObject(key.renderPass);
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
        bool match = (k.renderPass == obj);
        if (!match && k.shaderPass) {
            match = (k.shaderPass->vertexShader() == obj ||
                     k.shaderPass->fragmentShader() == obj ||
                     k.shaderPass->pipelineLayout() == obj);
        }
        if (match) {
            it = m_cache.erase(it);
        } else {
            ++it;
        }
    }
    m_trackedObjects.erase(obj);
}

} // namespace ln
