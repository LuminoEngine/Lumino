#pragma once
#include <array>
#include <string_view>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Material.hpp>

namespace ln {

class GraphicsContext;
class Renderer;

/**
 * 画面左上にビットマップフォントでデバッグ文字列を描画するクラス。
 *
 * - ASCII 文字のみ使用可。
 * - 1 フレームあたり最大 512 文字 (改行含む) まで蓄積可能。
 * - render() を呼び出すことで正射影オーバーレイとして描画される。
 * - GraphicsContext が内部で所有し、EndFrame 内で自動的に呼び出される。
 */
class DebugPrint : public Object {
public:
    static Result<Ref<DebugPrint>> create(GraphicsContext* ctx);

    /**
     * デバッグ文字列をバッファに蓄積します。末尾に改行が自動付加されます。
     * フレームをまたいでリセットされます。
     */
    void print(std::string_view text);

    /**
     * 蓄積された文字列を画面に描画し、バッファをクリアします。
     * EndFrame 内から呼び出されます。
     */
    Result<void> render(GraphicsContext* ctx);

private:
    DebugPrint() = default;

    // 最大文字数 (改行含む)
    static constexpr uint32_t kMaxChars = 512;
    // 1 文字あたりの頂点数 (2三角形 = 6頂点)
    static constexpr uint32_t kVertsPerChar = 6;

    Ref<rhi::Texture>  m_fontTexture;
    Ref<Material>      m_material;
    Ref<Mesh>          m_mesh;

    std::array<char, kMaxChars> m_textBuffer = {};
    uint32_t m_textBufferUsed = 0;
};

} // namespace ln
