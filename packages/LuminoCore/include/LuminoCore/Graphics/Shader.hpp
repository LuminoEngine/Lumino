#pragma once
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace ln {

class GraphicsContext;
class GraphicsModule;

/**
 * Shader: コンパイル済みシェーダ (.lcsh) から構築した ShaderPass 群を保持する共有オブジェクト。
 *
 * GPU シェーダモジュールとパイプラインレイアウトの生成はこのオブジェクトの作成時に
 * 1 度だけ行われ、ここから作られた Material はすべて同じ ShaderPass を共有します。
 * そのため、同一シェーダで複数の Material が必要な場合 (フレーム内で異なる
 * パラメータを使いたい、テクスチャごとに Material を分けたい等) でも、
 * Material の数に比例して GPU リソースが増えることはありません。
 *
 * ```cpp
 * auto shader = *Shader::createFromCompiledShader(ctx, data, size);
 * auto mat1 = *MaterialFactory::createFromShader(ctx, shader);
 * auto mat2 = *MaterialFactory::createFromShader(ctx, shader);  // モジュールは共有される
 * ```
 *
 * 共有されているかどうかは ShaderPass::liveCount() (C-API では
 * LNGraphicsProfiler::shaderPassCount) で計測できます。
 *
 * @see MaterialFactory::createFromShader
 */
class Shader : public Object {
public:
    ~Shader() override = default;

    /** コンパイル済みシェーダ (.lcsh) のバイナリデータから作成します。 */
    static Result<Ref<Shader>> createFromCompiledShader(
        GraphicsModule* module, const void* data, size_t size);

    /** コンパイル済みシェーダ (.lcsh) のバイナリデータから作成します。 */
    static Result<Ref<Shader>> createFromCompiledShader(
        GraphicsContext* ctx, const void* data, size_t size);

    /**
     * .slang ソースファイルを実行時コンパイルして作成します。
     * LUMINO_USE_SLANG が有効なデスクトップビルドでのみ動作します。
     * @param ctx            GraphicsContext
     * @param shaderFilePath コンパイルする .slang ファイルのパス
     * @param searchPath     lumino.slang を含むディレクトリのパス (空文字列で無効)
     */
    static Result<Ref<Shader>> createFromShaderSourceFile(
        GraphicsContext* ctx,
        std::string_view shaderFilePath,
        std::string_view searchPath);

    /** このシェーダが持つ全パス (シェーダ内の宣言順)。 */
    const std::vector<Ref<ShaderPass>>& passes() const { return m_passes; }

    /** 既定 (先頭) のパス。通常は "Forward"。 */
    ShaderPass* defaultPass() const { return m_passes.empty() ? nullptr : m_passes.front().get(); }

    /** 名前でパスを検索します。見つからない場合は nullptr。 */
    ShaderPass* findPass(const std::string& name) const;

private:
    Shader() = default;

    /** 1 度デシリアライズした UnifiedShader2 から全パスを構築する共通処理。 */
    static Result<Ref<Shader>> buildFromUnifiedShader(
        shader::UnifiedShader2* unifiedShader, rhi::Device* device);

    std::vector<Ref<ShaderPass>> m_passes;
};

} // namespace ln
