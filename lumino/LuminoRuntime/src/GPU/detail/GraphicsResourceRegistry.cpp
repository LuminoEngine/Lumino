﻿#include <LuminoEngine/GraphicsRHI/RHIResource.hpp>
#include <LuminoEngine/GPU/detail/GraphicsResourceRegistry.hpp>
#include <LuminoEngine/GPU/GraphicsResource.hpp>

namespace ln {

namespace detail {

GraphicsResourceRegistry::GraphicsResourceRegistry()
    : m_resourceList()
    , m_idStack()
    , m_rhiRegistries() {
}

GraphicsResourceRegistry::~GraphicsResourceRegistry() {
}

void GraphicsResourceRegistry::registerResource(IGraphicsResource* resource) {
    if (LN_ASSERT(resource->m_id == 0)) return;
    if (m_idStack.empty()) {
        GraphicsResourceId id = m_resourceList.size();
        resource->m_id = id;
        m_resourceList.push(resource);
    }
    else {
        GraphicsResourceId id = m_idStack.top();
        resource->m_id = id;
        m_idStack.pop();
        m_resourceList[id] = resource;
    }
}

void GraphicsResourceRegistry::unregisterResource(GraphicsResourceId id) {
    if (LN_ASSERT(id < m_resourceList.size())) return;
    IGraphicsResource* resource = m_resourceList[id];
    if (LN_ASSERT(resource)) return;

    for (RHIGraphicsResourceRegistry* rhiRegistry : m_rhiRegistries) {
        rhiRegistry->unregisterResource(id);
    }

    m_resourceList[id] = nullptr;
    m_idStack.push(id);
    resource->m_id = 0;
}

void GraphicsResourceRegistry::subscribe(RHIGraphicsResourceRegistry* rhiRegistry) {
    m_rhiRegistries.push(rhiRegistry);
}

void GraphicsResourceRegistry::unsubscribe(RHIGraphicsResourceRegistry* rhiRegistry) {
    m_rhiRegistries.remove(rhiRegistry);
}

RHIGraphicsResourceRegistry::RHIGraphicsResourceRegistry(GraphicsResourceRegistry* ownerRegistry)
    : m_ownerRegistry(ownerRegistry)
    , m_rhiResourceList() {
    m_ownerRegistry->subscribe(this);
}

RHIGraphicsResourceRegistry::~RHIGraphicsResourceRegistry() {
    if (m_ownerRegistry) {
        m_ownerRegistry->unsubscribe(this);
        m_ownerRegistry = nullptr;
    }
}

void RHIGraphicsResourceRegistry::registerResource(IGraphicsResource* resource, RHIResource* rhiResource) {
    m_rhiResourceList.ensureResize(resource->m_id + 1);
    m_rhiResourceList[resource->m_id] = rhiResource;
    rhiResource->m_objectId = resource->m_id;
}

void RHIGraphicsResourceRegistry::unregisterResource(GraphicsResourceId id) {
    if (LN_ASSERT(id < m_rhiResourceList.size())) return;
    RHIResource* rhiResource = m_rhiResourceList[id];
    if (LN_ASSERT(rhiResource)) return;
    if (LN_ASSERT(rhiResource->m_ownerId == id)) return;
    m_rhiResourceList[id] = nullptr;
    rhiResource->m_ownerId = 0;
}

} // namespace detail
} // namespace ln
