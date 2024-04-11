#include <LuminoEngine/GraphicsRHI/RHIObject.hpp>
#include <LuminoEngine/GPU/detail/GraphicsObjectRegistry.hpp>
#include <LuminoEngine/GPU/GraphicsResource.hpp>

namespace ln {

namespace detail {

GraphicsObjectRegistry::GraphicsObjectRegistry()
    : m_resourceList()
    , m_idStack()
    , m_rhiRegistries() {
}

GraphicsObjectRegistry::~GraphicsObjectRegistry() {
}

void GraphicsObjectRegistry::registerObject(IGraphicsObject* resource) {
    if (LN_ASSERT(resource->m_id == 0)) return;
    if (m_idStack.empty()) {
        GraphicsObjectId id = m_resourceList.size();
        resource->m_id = id;
        m_resourceList.push(resource);
    }
    else {
        GraphicsObjectId id = m_idStack.top();
        resource->m_id = id;
        m_idStack.pop();
        m_resourceList[id] = resource;
    }
}

void GraphicsObjectRegistry::unregisterObject(GraphicsObjectId id) {
    if (LN_ASSERT(id < m_resourceList.size())) return;
    IGraphicsObject* resource = m_resourceList[id];
    if (LN_ASSERT(resource)) return;

    for (RHIGraphicsObjectRegistry* rhiRegistry : m_rhiRegistries) {
        rhiRegistry->unregisterObject(id);
    }

    m_resourceList[id] = nullptr;
    m_idStack.push(id);
    resource->m_id = 0;
}

void GraphicsObjectRegistry::subscribe(RHIGraphicsObjectRegistry* rhiRegistry) {
    m_rhiRegistries.push(rhiRegistry);
}

void GraphicsObjectRegistry::unsubscribe(RHIGraphicsObjectRegistry* rhiRegistry) {
    m_rhiRegistries.remove(rhiRegistry);
}

RHIGraphicsObjectRegistry::RHIGraphicsObjectRegistry(GraphicsObjectRegistry* ownerRegistry)
    : m_ownerRegistry(ownerRegistry)
    , m_rhiObjectList() {
    m_ownerRegistry->subscribe(this);
}

RHIGraphicsObjectRegistry::~RHIGraphicsObjectRegistry() {
    if (m_ownerRegistry) {
        m_ownerRegistry->unsubscribe(this);
        m_ownerRegistry = nullptr;
    }
}

void RHIGraphicsObjectRegistry::registerObject(IGraphicsObject* resource, RHIDeviceObject* rhiObject) {
    m_rhiObjectList.ensureResize(resource->m_id + 1);
    m_rhiObjectList[resource->m_id] = rhiObject;
    rhiObject->m_objectId = resource->m_id;
}

void RHIGraphicsObjectRegistry::unregisterObject(GraphicsObjectId id) {
    if (LN_ASSERT(id < m_rhiObjectList.size())) return;
    RHIDeviceObject* rhiObject = m_rhiObjectList[id];
    if (LN_ASSERT(rhiObject)) return;
    if (LN_ASSERT(rhiObject->m_ownerId == id)) return;
    m_rhiObjectList[id] = nullptr;
    rhiObject->m_ownerId = 0;
}

} // namespace detail
} // namespace ln
