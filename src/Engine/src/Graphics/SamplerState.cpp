
#include "Internal.hpp"
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include "RHIs/GraphicsDeviceContext.hpp"
#include "GraphicsManager.hpp"

namespace ln {

const detail::SamplerStateData detail::SamplerStateData::defaultState =
{
    TextureFilterMode::Point,
    TextureAddressMode::Repeat,
    false,
};

//=============================================================================
// SamplerState

Ref<SamplerState> SamplerState::create()
{
    return makeObject<SamplerState>();
}

Ref<SamplerState> SamplerState::create(TextureFilterMode filter)
{
    return makeObject<SamplerState>(filter);
}

Ref<SamplerState> SamplerState::create(TextureFilterMode filter, TextureAddressMode address)
{
    return makeObject<SamplerState>(filter, address);
}

Ref<SamplerState> SamplerState::create(TextureFilterMode filter, TextureAddressMode address, bool anisotropyEnabled)
{
    return makeObject<SamplerState>(filter, address, anisotropyEnabled);
}

SamplerState* SamplerState::pointClamp()
{
    return detail::EngineDomain::graphicsManager()->pointClampSamplerState();
}

SamplerState* SamplerState::linearClamp()
{
    return detail::EngineDomain::graphicsManager()->linearClampSamplerState();
}

SamplerState::SamplerState()
    : m_manager(nullptr)
    , m_rhiObject(nullptr)
    , m_desc(detail::SamplerStateData::defaultState)
    , m_modified(true)
    , m_frozen(false)
{
}

SamplerState::~SamplerState()
{
}

void SamplerState::init()
{
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);
}

void SamplerState::init(TextureFilterMode filter)
{
    init();
    setFilterMode(filter);
}

void SamplerState::init(TextureFilterMode filter, TextureAddressMode address)
{
    init();
    setFilterMode(filter);
    setAddressMode(address);
}

void SamplerState::init(TextureFilterMode filter, TextureAddressMode address, bool anisotropyEnabled)
{
    init();
    setFilterMode(filter);
    setAddressMode(address);
    setAnisotropyEnabled(anisotropyEnabled);
}

void SamplerState::onDispose(bool explicitDisposing)
{
    m_rhiObject.reset();

    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

void SamplerState::setFilterMode(TextureFilterMode value)
{
    if (LN_REQUIRE(!m_frozen)) return;
    if (m_desc.filter != value) {
        m_desc.filter = value;
        m_modified = true;
    }
}

void SamplerState::setAddressMode(TextureAddressMode value)
{
    if (LN_REQUIRE(!m_frozen)) return;
    if (m_desc.address != value) {
        m_desc.address = value;
        m_modified = true;
    }
}

void SamplerState::setAnisotropyEnabled(bool value)
{
    if (LN_REQUIRE(!m_frozen)) return;
    if (m_desc.anisotropy != value) {
        m_desc.anisotropy = value;
        m_modified = true;
    }
}

detail::ISamplerState* SamplerState::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
    *outModified = m_modified;

    if (m_modified) {
        m_rhiObject = detail::GraphicsResourceInternal::manager(this)->deviceContext()->createSamplerState(m_desc);
        m_modified = false;
    }

    return m_rhiObject;
}

void SamplerState::onChangeDevice(detail::IGraphicsDevice* device)
{
    if (device) {
        m_modified = true;
    } else {
        m_rhiObject.reset();
    }
}

} // namespace ln
