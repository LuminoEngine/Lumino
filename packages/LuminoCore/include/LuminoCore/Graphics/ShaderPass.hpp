#pragma once
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <string>
#include <vector>

namespace ln {
namespace shader { class UnifiedShader2; }

/** マテリアル定数バッファ内の単一メンバ情報 (シェーダリフレクションから取得) */
struct MaterialMemberInfo {
    std::string name;       // 例: "u_time"
    int16_t offset;         // CB内のバイトオフセット
    int16_t size;           // バイトサイズ
};

/**
 * ShaderPass: シェーダモジュールとPipelineLayoutをまとめて保持する。
 *
 * セットインデックスはシェーダのリフレクション情報から動的に決定される:
 * - $Material (bare uniform) → Set N (通常 0)
 * - viewData  (ParameterBlock) → Set ?N
 * - sceneData (ParameterBlock) → Set ?N
 * - objectData(ParameterBlock) → Set ?N
 */
class ShaderPass : public RefObject {
public:
    inline static const char* kViewDataParameterBlockName = "viewData";

    /**
     * .lcshバイナリから指定インデックスのパス (GlobalShaderPass) のシェーダモジュールと
     * PipelineLayoutを一括構築する。
     * view/scene/objectのBindGroupLayoutDescは共有のものを引数で受け取り、
     * リフレクション情報に基づくセットインデックスに配置する。
     *
     * @param passIndex UnifiedShader2 内の GlobalShaderPass インデックス (デフォルト 0)
     */
    static Result<Ref<ShaderPass>> createFromCompiledShader(
        const void* data, size_t size,
        rhi::Device* device,
        size_t passIndex = 0);

#ifdef LUMINO_USE_SLANG
    /**
     * コンパイル済み UnifiedShader2 から指定インデックスのパスを構築する。
     * ShaderCompiler2 でコンパイルした結果を直接渡すことで、
     * シリアライズ/デシリアライズを省略できる。LUMINO_USE_SLANG が有効な場合のみ使用可能。
     * @param passIndex UnifiedShader2 内の GlobalShaderPass インデックス (デフォルト 0)
     */
    static Result<Ref<ShaderPass>> createFromUnifiedShader(
        shader::UnifiedShader2* unifiedShader,
        rhi::Device* device,
        size_t passIndex = 0);
#endif // LUMINO_USE_SLANG

    // アクセサ

    /** このパスの名前 (例: "Forward", "GBuffer"). */
    const std::string& passName() const { return m_passName; }

    rhi::ShaderModule* vertexShader() const { return m_vertShader.get(); }
    rhi::ShaderModule* fragmentShader() const { return m_fragShader.get(); }
    const std::string& vertexEntry() const { return m_vertEntry; }
    const std::string& fragmentEntry() const { return m_fragEntry; }
    rhi::PipelineLayout* pipelineLayout() const { return m_pipelineLayout.get(); }
    uint64_t materialParamBufferSize() const { return m_materialParamBufferSize; }

    /** マテリアル ($Material) BindGroup のディスクリプタセットインデックス。 */
    int16_t materialSetIndex() const { return m_materialSetIndex; }

    /** 名前ベースのパラメータ設定のための $Global CB メンバレイアウト。 */
    const std::vector<MaterialMemberInfo>& materialMembers() const { return m_materialMembers; }

    /** マテリアル ($Material) セットの BindGroupLayoutDesc。 */
    const rhi::BindGroupLayoutDesc& materialLayoutDesc() const { return m_materialLayoutDesc; }

    /** マテリアルセット内のバインディング名 (materialLayoutDesc().entries と対応)。 */
    const std::vector<std::string>& materialBindingNames() const { return m_materialBindingNames; }

    /** ビューデータのディスクリプタセットインデックス (シェーダリフレクションから取得)。 */
    int16_t viewSetIndex() const { return m_viewSetIndex; }

    /** シーンデータのディスクリプタセットインデックス (シェーダリフレクションから取得)。 */
    int16_t sceneSetIndex() const { return m_sceneSetIndex; }

    /** オブジェクトデータのディスクリプタセットインデックス (シェーダリフレクションから取得)。 */
    int16_t objectSetIndex() const { return m_objectSetIndex; }

    /** ビュー/カメラデータの共有 BindGroupLayoutDesc。 */
    const rhi::BindGroupLayoutDesc& viewLayoutDesc() const { return m_viewLayoutDesc; }

    /** シーン/ライティングデータの共有 BindGroupLayoutDesc。 */
    const rhi::BindGroupLayoutDesc& sceneLayoutDesc() const { return m_sceneLayoutDesc; }

    /** オブジェクト単位データの共有 BindGroupLayoutDesc。 */
    const rhi::BindGroupLayoutDesc& objectLayoutDesc() const { return m_objectLayoutDesc; }

    /** シェーダリフレクションから取得したオブジェクト単位の UBO サイズ。 */
    uint64_t objectUBOSize() const { return m_objectUBOSize; }

private:
    ShaderPass() = default;

    /** 内部用: createFromCompiledShader と createFromUnifiedShader が共有する実装。 */
    static Result<Ref<ShaderPass>> buildFromUnifiedShader(
        shader::UnifiedShader2* unifiedShader, rhi::Device* device, size_t passIndex);

    std::string m_passName;
    Ref<rhi::ShaderModule> m_vertShader;
    Ref<rhi::ShaderModule> m_fragShader;
    std::string m_vertEntry;
    std::string m_fragEntry;
    Ref<rhi::PipelineLayout> m_pipelineLayout;
    uint64_t m_materialParamBufferSize = 0;
    int16_t m_materialSetIndex = -1;
    std::vector<MaterialMemberInfo> m_materialMembers;

    // シェーダリフレクションから取得したディスクリプタセットインデックス
    int16_t m_viewSetIndex = -1;
    int16_t m_sceneSetIndex = -1;
    int16_t m_objectSetIndex = -1;

    // リフレクションから取得したマテリアルセットレイアウト
    rhi::BindGroupLayoutDesc m_materialLayoutDesc;
    std::vector<std::string> m_materialBindingNames;

    // 共有 BindGroupLayoutDesc (値型、GPUオブジェクトは持たない)
    rhi::BindGroupLayoutDesc m_viewLayoutDesc;
    rhi::BindGroupLayoutDesc m_sceneLayoutDesc;
    rhi::BindGroupLayoutDesc m_objectLayoutDesc;
    uint64_t m_objectUBOSize = 0;
};

} // namespace ln
