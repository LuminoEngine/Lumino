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

    /**
     * このオブジェクトが作成された時点のデバイス世代
     * (GraphicsModule::deviceGeneration)。
     * GPU リソースを保持するオブジェクト (Texture / Mesh / Material) の
     * stale 判定に使用する。デバイスロスト復旧後、世代が現在と一致しない
     * オブジェクトは描画からスキップされる。
     */
    uint32_t deviceGeneration() const { return m_deviceGeneration; }
    void setDeviceGeneration(uint32_t g) { m_deviceGeneration = g; }

private:
    friend class ObjectRegistry;
    uint16_t m_registryIndex = 0;
    uint16_t m_generation = 0;
    uint32_t m_deviceGeneration = 0;
};

} // namespace ln
