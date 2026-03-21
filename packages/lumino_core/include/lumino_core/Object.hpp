#pragma once

#include <lumino_base/RefCounted.hpp>
#include <cstdint>

namespace ln {

class ObjectRegistry;

/**
 * C-API で公開するオブジェクトの基底クラス。
 * RefCounted を継承し、ObjectRegistry でのハンドル管理に必要なスロット情報を保持する。
 */
class Object : public RefCounted {
public:
    Object() = default;
    ~Object() override;

    uint16_t registryIndex() const { return registryIndex_; }
    uint16_t generation() const { return generation_; }

private:
    friend class ObjectRegistry;
    uint16_t registryIndex_ = 0;
    uint16_t generation_ = 0;
};

} // namespace ln
