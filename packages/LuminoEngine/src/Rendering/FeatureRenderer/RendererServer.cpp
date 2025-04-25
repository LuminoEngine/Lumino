#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/RendererServer.hpp>

namespace ln {

URef<RendererServer> RendererServer::create() {
    URef<RendererServer> instance(LN_NEW RendererServer());
    instance->init();
    return instance;
}

RendererServer::RendererServer()
    : m_spriteRenderer(nullptr) {

}
MaybeResult RendererServer::init() {
    m_spriteRenderer = Ref<BatchRenderer>(LN_NEW BatchRenderer(), false);
    if (!m_spriteRenderer->init()) {
        return LN_MAKE_ERROR();
    }
    return LN_MAKE_SUCCESS();
}

BatchRenderer* RendererServer::spriteRenderer() const {
    return m_spriteRenderer;
}

void RendererServer::reset() {
    m_activeRenderer = nullptr;
}

void RendererServer::activate(BatchRenderer* renderer, CommandList* commandList, Material* material) {
    if (!m_activeRenderer) {
        m_activeRenderer = renderer;
        m_activeRenderer->begin(commandList, material);
    }
    else if (m_activeRenderer != renderer) {
        m_activeRenderer->end();
        m_activeRenderer = renderer;
        m_activeRenderer->begin(commandList, material);
    }
    else if (!m_activeRenderer->currentMaterial()->equalStatus(material)) {
        m_activeRenderer->end();
        m_activeRenderer->begin(commandList, material);
    }
    else {
        // Use m_activeRenderer
    }
}

void RendererServer::flush() {
    if (m_activeRenderer) {
        m_activeRenderer->end();
        m_activeRenderer = nullptr;
    }
}

} // namespace ln
