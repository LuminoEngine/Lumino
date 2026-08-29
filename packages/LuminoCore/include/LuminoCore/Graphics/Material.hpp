#pragma once
#include <LuminoBase/math/Math.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <functional>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

namespace ln {

/**
 * サンプラー設定 (テクスチャのフィルタリングとアドレッシング)。
 *
 * mag/min フィルタと U/V/W アドレッシングを個別に指定する必要は現状無いため、
 * それぞれ 1 つの値で全軸をまとめて指定します。細かい制御が必要になった場合は
 * rhi::SamplerDesc へフィールドを追加する形で拡張します。
 * @see LNTextureFilterMode, LNTextureAddressMode
 */
struct SamplerState {
    /** 拡大/縮小フィルタ。既定は Linear。 */
    rhi::FilterMode filter = rhi::FilterMode::Linear;
    /**
     * UV が 0..1 の範囲外に出たときの回り込み方法。既定は ClampToEdge。
     *
     * Repeat を既定にすると、ポストエフェクトが画面端の近傍をサンプルしたときに
     * 反対側の端から色が漏れる、シャドウマップがライトの視錐台の外で回り込む、
     * といった気付きにくい不具合を生む。タイリングが必要な場合に明示的に
     * Repeat を指定する運用とする。
     */
    rhi::AddressMode address = rhi::AddressMode::ClampToEdge;

    bool operator==(const SamplerState& o) const {
        return filter == o.filter && address == o.address;
    }
    bool operator!=(const SamplerState& o) const { return !(*this == o); }

    /** rhi レイヤの SamplerDesc へ変換します。 */
    rhi::SamplerDesc toSamplerDesc() const {
        rhi::SamplerDesc desc;
        desc.magFilter = filter;
        desc.minFilter = filter;
        desc.addressU = address;
        desc.addressV = address;
        desc.addressW = address;
        return desc;
    }
};

/** @see LNBlendMode */
enum class BlendMode {
    Normal = 0,
    Alpha = 1,
    Add = 2,
    Subtract = 3,
    Multiply = 4,
};

/** GPU アライメント済みのビューパラメータ (Set N - カメラ)。シェーダの ViewParams 構造体と一致させること。 */
struct ViewParamsUBO {
    float viewProj[16];
    float cameraPos[4];
    float view[16];         // ビュー行列 (ワールド -> ビュー)
    float proj[16];         // 射影行列 (ビュー -> クリップ)
    float invViewProj[16]; // viewProj の逆行列 (クリップ -> ワールド)
    float invProj[16];     // proj の逆行列 (クリップ -> ビュー)
    float screenSize[4];   // (width, height, 1/width, 1/height)
};

/** GPU アライメント済みのシーンパラメータ (Set N - ライティング)。シェーダの SceneParams 構造体と一致させること。 */
struct SceneParamsUBO {
    float lightDir[4];
    float lightColor[4];
    float ambientColor[4];
};

/** GPU アライメント済みのオブジェクトパラメータ (Set N)。シェーダの ObjectParams 構造体と一致させること。 */
struct ObjectParamsUBO {
    float world[16];
    float normalMatrix[16];
};

/**
 * Material: シェーダ + パラメータ + レンダーステート + テクスチャ。
 * RenderPipeline とマテリアル単位の BindGroup を管理します。
 *
 * パラメータの格納はシェーダリフレクションに基づきます:
 * - $Global CB のメンバはバイトバッファ (m_paramBuffer) に格納されます
 * - Texture/Sampler スロットは m_baseTexture に格納されます (後で拡張可能)
 */
class Material : public Object {
public:
    ~Material() override = default;

    /** マテリアルの $Global 定数バッファ内の名前付き float4 パラメータを設定します。 */
    void setFloat4(const std::string& name, const float* values);

    /** マテリアルの $Global 定数バッファ内の名前付き float パラメータを設定します。 */
    void setFloat(const std::string& name, float value);

    /** 簡易版: "u_params" という名前の CB の "color" フィールドを設定します。 */
    void setColor(const Color& color);

    /** 簡易版: BasicLit 系シェーダ向けに色とスペキュラを設定します。 */
    void setSpecular(const Color& color, float shininess);

    void setTexture(rhi::Texture* texture);

    /** シェーダのバインディング名 (例: "u_sceneColor") でテクスチャを設定します。 */
    void setNamedTexture(const std::string& name, rhi::Texture* texture);

    // サンプラー設定

    /**
     * このマテリアルの全テクスチャに適用される既定のサンプラー設定を指定します。
     * 名前付きの上書き (setNamedSamplerState) があるスロットにはそちらが優先されます。
     */
    void setSamplerState(const SamplerState& state);

    /**
     * 名前付きテクスチャ 1 スロットだけのサンプラー設定を指定します。
     * マテリアル単位の設定 (setSamplerState) を上書きします。
     *
     * @param name  シェーダ内の Texture2D のバインディング名 (例: "u_sceneColor")。
     *              対応する SamplerState の名前ではなくテクスチャ側の名前を指定します。
     */
    void setNamedSamplerState(const std::string& name, const SamplerState& state);

    // レンダーステート
    void setBlendMode(BlendMode mode);
    void setCullMode(rhi::CullMode mode);
    void setDepthTestEnabled(bool enabled);
    void setDepthWriteEnabled(bool enabled);

    // ShaderPass のアクセサ
    /** 既定 (主) の ShaderPass - 最初に登録されたパスで、通常は "Forward"。
     *  マテリアルパラメータのレイアウトと、フォールバックのアクセサとして使用します。 */
    ShaderPass* shaderPass() const { return m_defaultShaderPass.get(); }

    /** 名前 (例: "Forward", "GBuffer") で ShaderPass を検索します。無ければ nullptr を返します。 */
    ShaderPass* findPass(const std::string& name) const {
        for (const auto& pass : m_shaderPasses) {
            if (pass->passName() == name) return pass.get();
        }
        return nullptr;
    }

    /** このマテリアルが指定した名前の ShaderPass を持つかどうか。 */
    bool hasPass(const std::string& name) const { return findPass(name) != nullptr; }

    /** このマテリアルが持つ全パス (登録順)。 */
    const std::vector<Ref<ShaderPass>>& shaderPasses() const { return m_shaderPasses; }

    // シェーダ / レンダーステートのアクセサ (PipelineCache のキー構築で使用)
    rhi::ShaderModule* vertexShader() const { return m_defaultShaderPass->vertexShader(); }
    rhi::ShaderModule* fragmentShader() const { return m_defaultShaderPass->fragmentShader(); }
    const std::string& vertexEntry() const { return m_defaultShaderPass->vertexEntry(); }
    const std::string& fragmentEntry() const { return m_defaultShaderPass->fragmentEntry(); }
    rhi::CullMode cullMode() const { return m_cullMode; }
    BlendMode blendMode() const { return m_blendMode; }
    bool depthTestEnabled() const { return m_depthTestEnabled; }
    bool depthWriteEnabled() const { return m_depthWriteEnabled; }

    /** $Material 定数バッファの内容のバージョン。setFloat/setColor 等で進みます (UBO の書き直しのみ必要)。 */
    uint64_t paramVersion() const { return m_paramVersion; }

    /**
     * BindGroup の構成 (テクスチャ/サンプラー) のバージョン。setTexture/setSamplerState 等で進みます。
     * paramVersion() と分けることで、パラメータだけを毎フレーム更新するマテリアルで
     * BindGroup の再生成 (GPU/JS オブジェクトの生成と破棄) が走らないようにしています。
     * 初期値が 1 なのは、Renderer 側の 0 を「まだ一度も解決していない」印にするためです。
     */
    uint64_t bindingVersion() const { return m_bindingVersion; }

    // Renderer 側で BindGroup を構築するためのアクセサ
    rhi::Texture* baseTexture() const { return m_baseTexture.get(); }
    uint64_t materialParamBufferSize() const { return m_defaultShaderPass->materialParamBufferSize(); }
    const Color& baseColor() const { return m_baseColor; }

    /** 名前付きテクスチャのマップ (リフレクションに基づく BindGroup 構築用)。 */
    const std::unordered_map<std::string, Ref<rhi::Texture>>& namedTextures() const { return m_namedTextures; }

    /** マテリアル単位の既定サンプラー設定。 */
    const SamplerState& samplerState() const { return m_samplerState; }

    /** 名前付きサンプラー設定の上書きマップ (キーはテクスチャのバインディング名)。 */
    const std::unordered_map<std::string, SamplerState>& namedSamplerStates() const { return m_namedSamplerStates; }

    /**
     * 指定テクスチャバインディング名に適用されるサンプラー設定を解決します。
     * 名前付きの上書きがあればそれを、無ければマテリアル単位の設定を返します。
     * @param textureName テクスチャのバインディング名 (空文字列ならマテリアル単位の設定)
     */
    const SamplerState& resolveSamplerState(const std::string& textureName) const {
        if (!textureName.empty()) {
            auto it = m_namedSamplerStates.find(textureName);
            if (it != m_namedSamplerStates.end()) return it->second;
        }
        return m_samplerState;
    }

    /** マテリアルの UBO データを、指定したマップ済みポインタへ書き込みます。 */
    void writeMaterialUBO(void* dst) const;

    // ---- 破棄通知 ----

    /** Material が破棄される直前に呼び出されるコールバック。 */
    using DestroyCallback = std::function<void(Material*)>;

    /** この Material の破棄を通知するコールバックを登録します (解除はできません)。 */
    void addDestroyCallback(DestroyCallback callback);

protected:
    void finalize() override;

private:
    Material();
    friend class MaterialFactory;

    std::vector<DestroyCallback> m_destroyCallbacks;

    // 登録順に並べた ShaderPass 群 (名前は ShaderPass::passName() が持つ)。
    // 同じマテリアルを別のレンダーパスで描くときは findPass() で名前引きする。
    // findPass() はドローコールごとに呼ばれるが、1 マテリアルのパス数は通常 1-3 なので
    // ハッシュマップより線形走査の方が速い (Shader::findPass() も同じ形)。
    // パスが 10 個を超えるシェーダが出てきた場合はこの前提を見直すこと。
    // 前提: 同一マテリアル内の全パスは $Material の layout (params/テクスチャ slot) を共有する。
    std::vector<Ref<ShaderPass>> m_shaderPasses;

    // 既定 (主) のパス - 通常は最初に登録されたもので、マテリアルパラメータのレイアウトに使う。
    // ホットパスのアクセサ (shaderPass()/materialParamBufferSize) がマップ検索をしなくて済むよう
    // 別の Ref として保持する。
    Ref<ShaderPass> m_defaultShaderPass;

    uint64_t m_paramVersion;
    uint64_t m_bindingVersion;

    // 汎用のマテリアルパラメータバッファ (リフレクションで得た $Global CB のレイアウトに一致)
    std::vector<uint8_t> m_paramBuffer;

    // キャッシュしたベースカラー (簡易アクセサ用)
    Color m_baseColor;

    // テクスチャ
    Ref<rhi::Texture> m_baseTexture;

    // 名前付きテクスチャスロット (キーはシェーダのバインディング名。例: "u_sceneColor")
    std::unordered_map<std::string, Ref<rhi::Texture>> m_namedTextures;

    // サンプラー設定 (マテリアル単位の既定と、テクスチャバインディング名ごとの上書き)
    SamplerState m_samplerState;
    std::unordered_map<std::string, SamplerState> m_namedSamplerStates;

    // レンダーステート
    rhi::CullMode m_cullMode;
    BlendMode m_blendMode;
    bool m_depthTestEnabled;
    bool m_depthWriteEnabled;

    void markParamDirty() { ++m_paramVersion; }
    void markBindingDirty() { ++m_bindingVersion; }

    /** $Global CB 内の名前付きメンバのオフセットを検索する。見つからなければ -1 を返す。 */
    int findMemberOffset(const std::string& name) const;
};

class GraphicsContext;
class GraphicsModule;
class Shader;
enum class BuiltinShader;

/** プリコンパイル済みシェーダから組み込みマテリアルを作成するファクトリ。 */
class MaterialFactory {
public:
    /** Unlit マテリアルを作成します (テクスチャ * 色、ライティングなし)。 */
    static Result<Ref<Material>> createUnlit(GraphicsModule* module);

    /** GraphicsContext から Unlit マテリアルを作成します。 */
    static Result<Ref<Material>> createUnlit(GraphicsContext* ctx);

    /** BasicLit マテリアルを作成します (Blinn-Phong、平行光源 1 つ)。 */
    static Result<Ref<Material>> createBasicLit(GraphicsModule* module);

    /** GraphicsContext から BasicLit マテリアルを作成します。 */
    static Result<Ref<Material>> createBasicLit(GraphicsContext* ctx);

    /** StencilMask マテリアルを作成します (アルファテスト付きステンシル書き込み、カラー出力なし)。 */
    static Result<Ref<Material>> createStencilMask(GraphicsModule* module);

    /** GraphicsContext から StencilMask マテリアルを作成します。 */
    static Result<Ref<Material>> createStencilMask(GraphicsContext* ctx);

    /**
     * 生成済みの Shader からマテリアルを作成します。
     *
     * GPU シェーダモジュールとパイプラインレイアウトは Shader が保持しているものを
     * 共有するため、同一 Shader から Material を何個作っても GPU リソースは増えません。
     * フレーム内で異なるパラメータを使いたい場合やテクスチャごとに Material を
     * 分けたい場合は、この経路で Material を量産してください。
     * @see Shader::createFromCompiledShader
     */
    static Result<Ref<Material>> createFromShader(GraphicsModule* module, Shader* shader);

    /** 生成済みの Shader からマテリアルを作成します。 */
    static Result<Ref<Material>> createFromShader(GraphicsContext* ctx, Shader* shader);

    /**
     * コンパイル済みシェーダバイナリ (.lcsh) からマテリアルを作成します。
     *
     * この関数は呼び出しごとに GPU シェーダモジュールとパイプラインレイアウトを
     * 新規生成します。同一シェーダから複数の Material を作る場合は
     * Shader::createFromCompiledShader + createFromShader を使ってください。
     */
    static Result<Ref<Material>> createFromCompiledShader(
        GraphicsModule* module, const void* data, size_t size);

    /** GraphicsContext 経由でコンパイル済みシェーダバイナリ (.lcsh) からマテリアルを作成します。 */
    static Result<Ref<Material>> createFromCompiledShader(
        GraphicsContext* ctx, const void* data, size_t size);

    /**
     * .slang ソースファイルを実行時コンパイルしてマテリアルを作成します。
     * LUMINO_USE_SLANG が有効なデスクトップビルドでのみ動作します。
     * @param ctx            GraphicsContext のハンドル
     * @param shaderFilePath コンパイルする .slang ファイルのパス
     * @param searchPath     lumino.slang を含むディレクトリのパス (空文字列で無効)
     */
    static Result<Ref<Material>> createFromShaderSourceFile(
        GraphicsContext* ctx,
        std::string_view shaderFilePath,
        std::string_view searchPath);

private:
    static Result<Ref<Material>> createMaterialFromBuiltin(
        GraphicsModule* module, BuiltinShader shader);

    // 共通ヘルパー (MaterialFactory の friend 権限で Material の内部にアクセスする)。
    static void registerPass(Material* mat, Ref<ShaderPass> pass);
    static void initParamBufferFromDefaultPass(Material* mat);
};

} // namespace ln
