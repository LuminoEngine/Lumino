#pragma once

#include <LuminoBase/RefObject.hpp>
#include <cstdint>

namespace ln {

class ObjectRegistry;

/**
 * C-API で公開するオブジェクトの基底クラス。
 * RefObject を継承し、ObjectRegistry でのハンドル管理に必要なスロット情報を保持する。
 */
class Object : public RefObject {
public:
    Object() = default;
    ~Object() override;

    uint16_t registryIndex() const { return m_registryIndex; }
    uint16_t generation() const { return m_generation; }

private:
    friend class ObjectRegistry;
    uint16_t m_registryIndex = 0;
    uint16_t m_generation = 0;
};

} // namespace ln
