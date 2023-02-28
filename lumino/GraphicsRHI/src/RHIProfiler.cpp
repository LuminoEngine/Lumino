#include <LuminoGraphicsRHI/RHIProfiler.hpp>

namespace ln {
namespace detail {

//==============================================================================
// RHIProfiler

RHIProfiler::RHIProfiler()
    : m_vertexLayoutCount(0)
    , m_vertexBufferCount(0)
    , m_indexBufferCount(0)
    , m_texture2DCount(0)
    , m_renderTargetCount(0)
    , m_depthBufferCount(0)
    , m_samplerStateCount(0)
    , m_shaderPassCount(0)
    , m_uniformBufferCount(0)
    , m_descriptorPoolCount(0)
    , m_renderPassCount(0)
    , m_pipelineStateCount(0)
    , m_commandListCount(0)
    , m_swapChainCount(0)
//, m_drawCall(0)
{
}

void RHIProfiler::addVertexLayout(IVertexDeclaration* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_vertexLayoutCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeVertexLayout(IVertexDeclaration* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_vertexLayoutCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addVertexBuffer(RHIResource* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_vertexBufferCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeVertexBuffer(RHIResource* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_vertexBufferCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addIndexBuffer(RHIResource* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_indexBufferCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeIndexBuffer(RHIResource* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_indexBufferCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addTexture2D(RHIResource* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_texture2DCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeTexture2D(RHIResource* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_texture2DCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addRenderTarget(RHIResource* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_renderTargetCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeRenderTarget(RHIResource* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_renderTargetCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addDepthBuffer(RHIResource* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_depthBufferCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeDepthBuffer(RHIResource* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_depthBufferCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addSamplerState(ISamplerState* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_samplerStateCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeSamplerState(ISamplerState* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_samplerStateCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addShaderPass(IShaderPass* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_shaderPassCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeShaderPass(IShaderPass* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_shaderPassCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addUniformBuffer(RHIResource* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_uniformBufferCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeUniformBuffer(RHIResource* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_uniformBufferCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addDescriptorPool(IDescriptorPool* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_descriptorPoolCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeDescriptorPool(IDescriptorPool* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_descriptorPoolCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addRenderPass(IRenderPass* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_renderPassCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeRenderPass(IRenderPass* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_renderPassCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addPipelineState(IPipeline* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_pipelineStateCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removePipelineState(IPipeline* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_pipelineStateCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addCommandList(ICommandList* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_commandListCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeCommandList(ICommandList* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_commandListCount--;
    obj->m_profiling = false;
}

void RHIProfiler::addSwapChain(ISwapChain* obj) {
    if (LN_REQUIRE(!obj->m_profiling)) return;
    m_swapChainCount++;
    obj->m_profiling = true;
}

void RHIProfiler::removeSwapChain(ISwapChain* obj) {
    if (LN_REQUIRE(obj->m_profiling)) return;
    m_swapChainCount--;
    obj->m_profiling = false;
}

} // namespace detail
} // namespace ln
