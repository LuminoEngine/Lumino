#include <lumino_core/ObjectRegistry.hpp>

namespace ln {

ObjectRegistry::ObjectRegistry() {
    // インデックス 0 は LN_NULL_HANDLE として予約
    slots_.push_back(Slot{});
}

ObjectRegistry::~ObjectRegistry() = default;

LNHandle ObjectRegistry::registerObject(Ref<Object> obj) {
    if (!obj) return LN_NULL_HANDLE;

    std::lock_guard<std::mutex> lock(mutex_);

    uint16_t index;
    if (!freeList_.empty()) {
        index = freeList_.back();
        freeList_.pop_back();
    } else {
        if (slots_.size() >= 0xFFFF) {
            return LN_NULL_HANDLE;
        }
        index = static_cast<uint16_t>(slots_.size());
        slots_.push_back(Slot{});
    }

    // Object のデストラクタから unregister が呼ばれるが、
    // ここで先にスロット情報をクリアしておくことで二重解放を防ぐ
    Slot& slot = slots_[index];
    slot.object = std::move(obj);
    slot.object->registryIndex_ = index;
    slot.object->generation_ = slot.generation;

    return makeHandle(index, slot.generation);
}

Object* ObjectRegistry::resolve(LNHandle handle) const {
    if (handle == LN_NULL_HANDLE) return nullptr;

    uint16_t index = handleIndex(handle);
    uint16_t gen = handleGeneration(handle);

    std::lock_guard<std::mutex> lock(mutex_);

    if (index >= slots_.size()) return nullptr;

    const Slot& slot = slots_[index];
    if (slot.generation != gen || !slot.object) return nullptr;

    return slot.object.get();
}

bool ObjectRegistry::release(LNHandle handle) {
    if (handle == LN_NULL_HANDLE) return false;

    uint16_t index = handleIndex(handle);
    uint16_t gen = handleGeneration(handle);

    std::lock_guard<std::mutex> lock(mutex_);

    if (index >= slots_.size()) return false;

    Slot& slot = slots_[index];
    if (slot.generation != gen || !slot.object) return false;

    slot.object->registryIndex_ = 0;
    slot.object.reset();
    slot.generation++;
    freeList_.push_back(index);

    return true;
}

void ObjectRegistry::unregister(Object* obj) {
    if (!obj || obj->registryIndex_ == 0) return;

    std::lock_guard<std::mutex> lock(mutex_);

    uint16_t index = obj->registryIndex_;
    if (index >= slots_.size()) return;

    Slot& slot = slots_[index];
    if (slot.object.get() != obj) return;

    slot.object = nullptr; // ここで release() は呼ばない（デストラクタ中なので）
    slot.generation++;
    freeList_.push_back(index);
}

size_t ObjectRegistry::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    size_t count = 0;
    for (size_t i = 1; i < slots_.size(); ++i) {
        if (slots_[i].object) ++count;
    }
    return count;
}

} // namespace ln
