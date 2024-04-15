#include <LuminoEngine/GraphicsRHI/RHIObject.hpp>
#include <LuminoEngine/GPU/detail/GraphicsObjectRegistry.hpp>
#include <LuminoEngine/GPU/GraphicsResource.hpp>

namespace ln {

namespace detail {

GraphicsObjectRegistry::GraphicsObjectRegistry()
    : m_resourceList{nullptr} // [0] is dummy.
    , m_idStack()
    , m_rhiRegistries() {
}

GraphicsObjectRegistry::~GraphicsObjectRegistry() {
}

void GraphicsObjectRegistry::registerObject(IGraphicsObject* object) {
    if (LN_ASSERT(object->m_id == 0)) return;
    if (m_idStack.empty()) {
        GraphicsObjectId id = m_resourceList.size();
        object->m_id = id;
        m_resourceList.push(object);
    }
    else {
        GraphicsObjectId id = m_idStack.top();
        object->m_id = id;
        m_idStack.pop();
        m_resourceList[id] = object;
    }
}

void GraphicsObjectRegistry::unregisterObject(IGraphicsObject* object) {
    GraphicsObjectId id = object->m_id;
    if (LN_ASSERT(id < m_resourceList.size())) return;
    IGraphicsObject* resource = m_resourceList[id];
    if (LN_ASSERT(resource == object)) return;

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

    // Check: all have been released.
    for (const auto& x : m_rhiObjectList) {
        if (x) {
            LN_ERROR();
        }
    }

    if (m_ownerRegistry) {
        m_ownerRegistry->unsubscribe(this);
        m_ownerRegistry = nullptr;
    }
}

void RHIGraphicsObjectRegistry::registerObject(IGraphicsObject* resource, RHIDeviceObject* rhiObject) {
    if (LN_ASSERT(resource)) return;
    if (LN_ASSERT(resource->m_id > 0)) return;
    m_rhiObjectList.ensureResize(resource->m_id + 1);
    RHIDeviceObject* oldRHIObject = m_rhiObjectList[resource->m_id];
    if (oldRHIObject != nullptr) {
        //oldRHIObject->destroy();
    }
    m_rhiObjectList[resource->m_id] = rhiObject;
    rhiObject->m_ownerId = resource->m_id;
}

void RHIGraphicsObjectRegistry::unregisterObject(GraphicsObjectId id) {
    if (id >= m_rhiObjectList.size()) return;
    RHIDeviceObject* rhiObject = m_rhiObjectList[id];
    if (!rhiObject) return;
    if (LN_ASSERT(rhiObject->m_ownerId == id)) return;
    rhiObject->m_ownerId = 0;
    m_rhiObjectList[id] = nullptr;
}

void RHIGraphicsObjectRegistry::unregisterAllObjects() {
    for (const Ref<RHIDeviceObject>& rhiObject : m_rhiObjectList) {
        if (rhiObject) {
            //rhiObject->destroy();
            rhiObject->m_ownerId = 0;
		}
	}
	m_rhiObjectList.clear();
}

RHIDeviceObject* RHIGraphicsObjectRegistry::get(IGraphicsObject* object) const {
    if (object->m_id >= m_rhiObjectList.size()) return nullptr;
    return m_rhiObjectList[object->m_id];
}

} // namespace detail
} // namespace ln
