#include <LuminoCore/Runtime/ObjectRegistry.hpp>

namespace ln {

ObjectRegistry::ObjectRegistry() {
    // インデックス 0 は LN_NULL_HANDLE として予約
    m_slots.push_back(Slot{});
}

ObjectRegistry::~ObjectRegistry() = default;

LNHandle ObjectRegistry::registerObject(Object* obj) {
    if (!obj) return LN_NULL_HANDLE;

    std::lock_guard<std::mutex> lock(m_mutex);

    uint16_t index;
    if (!m_freeList.empty()) {
        index = m_freeList.back();
        m_freeList.pop_back();
    } else {
        if (m_slots.size() >= 0xFFFF) {
            return LN_NULL_HANDLE;
        }
        index = static_cast<uint16_t>(m_slots.size());
        m_slots.push_back(Slot{});
    }

    // Object のデストラクタから unregister が呼ばれるが、
    // ここで先にスロット情報をクリアしておくことで二重解放を防ぐ
    Slot& slot = m_slots[index];
    slot.object = obj;
    slot.object->m_registryIndex = index;
    slot.object->m_generation = slot.generation;

    return makeHandle(index, slot.generation);
}

LNHandle ObjectRegistry::wrapOrRegisterObject(Object* obj) {
    if (!obj) return LN_NULL_HANDLE;
    // registerObject も m_mutex を取るため、ロックを保持したまま呼ぶと
    // デッドロックする。既登録チェックのスコープでロックを解放してから呼ぶ。
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (obj->m_registryIndex != 0) {
            uint16_t index = obj->m_registryIndex;
            if (index < m_slots.size() && m_slots[index].object == obj) {
                return makeHandle(index, m_slots[index].generation);
            }
        }
    }
    return registerObject(obj);
}

Object* ObjectRegistry::resolve(LNHandle handle) const {
    if (handle == LN_NULL_HANDLE) return nullptr;

    uint16_t index = handleIndex(handle);
    uint16_t gen = handleGeneration(handle);

    std::lock_guard<std::mutex> lock(m_mutex);

    if (index >= m_slots.size()) return nullptr;

    const Slot& slot = m_slots[index];
    if (slot.generation != gen || !slot.object) return nullptr;

    return slot.object;
}

bool ObjectRegistry::release(LNHandle handle) {
    if (handle == LN_NULL_HANDLE) return false;

    uint16_t index = handleIndex(handle);
    uint16_t gen = handleGeneration(handle);

    // object のデストラクタから release() が再帰的に呼ばれる可能性があるため、
    // ここではスロットのクリアと free list への追加だけ行い、実際の release() はロック外で行う。
    Object* object = nullptr;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (index >= m_slots.size()) return false;

        Slot& slot = m_slots[index];
        if (slot.generation != gen || !slot.object) return false;

        object = slot.object;
        slot.object->m_registryIndex = 0;
        slot.object = nullptr;
        slot.generation++;
        m_freeList.push_back(index);
    }

    object->release();
    return true;
}

void ObjectRegistry::unregister(Object* obj) {
    if (!obj || obj->m_registryIndex == 0) return;

    std::lock_guard<std::mutex> lock(m_mutex);

    uint16_t index = obj->m_registryIndex;
    if (index >= m_slots.size()) return;

    Slot& slot = m_slots[index];
    if (slot.object != obj) return;

    slot.object = nullptr; // ここで release() は呼ばない（デストラクタ中なので）
    slot.generation++;
    m_freeList.push_back(index);
}

size_t ObjectRegistry::size() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    size_t count = 0;
    for (size_t i = 1; i < m_slots.size(); ++i) {
        if (m_slots[i].object) ++count;
    }
    return count;
}

} // namespace ln
