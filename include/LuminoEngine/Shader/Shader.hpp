// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "../Graphics/GraphicsResource.hpp"
#include "ShaderHelper.hpp"

namespace ln {
class DiagnosticsManager;
class Texture;
class Shader;
class ShaderConstantBuffer;
class ShaderParameter;
class ShaderTechnique;
class ShaderPass;
class ShaderCompilationProperties;
class GraphicsContext;
namespace detail {
class ShaderHelper;
class ShaderManager;
class IShaderPass;
class IShaderUniformBuffer;
}

/**
 * シェーダーを表すクラスです。
 *
 * このクラスは DirectX9 ～ 11 世代に提供されていた「エフェクト」と同様の、シェーダーステージをまとめて管理・適用する機能を持っています。
 */
class LN_API Shader final
    : public GraphicsResource
{
public:
    /*
     * 事前コンパイル済みシェーダファイルまたはシェーダプログラムファイルから Shader オブジェクトを作成します。
     *
     * @param[in]   filePath : 入力ファイル名
     *
     * シェーダプログラムファイル の読み込み機能はデバッグを目的として用意されています。
     *
     * シェーダプログラムファイルを読み込むことができるのは、デスクトップターゲットのみです。
     * モバイルターゲット、Web ターゲットでは事前コンパイル済みシェーダファイルのみを読み込むことができます。
     */
    static Ref<Shader> create(const StringRef& filePath, ShaderCompilationProperties* properties = nullptr);

    /*
     * Lumino の独自拡張 (technique 構文など) を使用しない HLSL シェーダをコンパイルし、Shader オブジェクトを作成します。
     * 
     * 作成された Shader は、1 つの ShaderTechnique と 1 つの ShaderPass を持ちます。
     */
    static Ref<Shader> create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCompilationProperties* properties = nullptr);

    /*
     * 名前を指定してこの Shader に含まれる ShaderParameter を検索します。
     *
     * @param[in]   name : パラメータの名前
     * @return      一致した ShaderParameter。見つからない場合は nullptr。
     *
     * この関数の検索対象は次の通りです。
     * - "_Global" という名前の付いた ShaderConstantBuffer (cbuffer 構文を使用せず、グローバルスコープに直接定義した変数)
     * - texture 型のパラメータ
     *
     * @attention 現在、SamplerState の設定は未サポートです。将来的には、この関数で SamplerState 型のパラメータを検索できるようにする予定です。
     */
    ShaderParameter* findParameter(const StringRef& name) const;

    /*
     * 名前を指定してこの Shader に含まれる ShaderConstantBuffer を検索します。
     *
     * @param[in]   name : 定数バッファの名前
     * @return      一致した ShaderConstantBuffer。見つからない場合は nullptr。
     */
    ShaderConstantBuffer* findConstantBuffer(const StringRef& name) const;

    /*
     * 名前を指定してこの Shader に含まれる ShaderTechnique を検索します。
     *
     * @param[in]   name : 定数バッファの名前
     * @return      一致した ShaderConstantBuffer。見つからない場合は nullptr。
     */
    ShaderTechnique* findTechnique(const StringRef& name) const;

    /** この Shader に含まれる ShaderTechnique を取得します。 */
    Ref<ReadOnlyList<Ref<ShaderTechnique>>> techniques() const;

    virtual void dispose() override;

protected:
    virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

    LN_CONSTRUCT_ACCESS : Shader();
    virtual ~Shader();
    void init();
    void init(const StringRef& filePath, ShaderCompilationProperties* properties = nullptr);
    void init(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCompilationProperties* properties = nullptr);
    void init(const String& name, Stream* stream);

private:
    detail::ShaderSemanticsManager* semanticsManager() { return &m_semanticsManager; }
    ShaderTechnique* findTechniqueByClass(const detail::ShaderTechniqueClass& techniqueClass) const;
    void createFromUnifiedShader(Stream* stream, DiagnosticsManager* diag);
    Ref<detail::IShaderPass> createShaderPass(
        const char* vsData,
        size_t vsLen,
        const char* vsEntryPoint,
        const char* psData,
        size_t psLen,
        const char* psEntryPoint,
        DiagnosticsManager* diag,
        ShaderCompilationProperties* properties);
    Ref<detail::IShaderPass> createRHIShaderPass(
        const char* vsData,
        size_t vsLen,
        const char* psData,
        size_t psLen,
        DiagnosticsManager* diag);
    void createSinglePassShader(const char* vsData, size_t vsLen, const char* psData, size_t psLen, DiagnosticsManager* diag, ShaderCompilationProperties* properties);
    void postInitialize();
    ShaderConstantBuffer* getOrCreateConstantBuffer(detail::IShaderUniformBuffer* buffer);
    ShaderParameter* getOrCreateTextureParameter(const String& name);

    detail::ShaderManager* m_manager;
    String m_name;
    List<Ref<ShaderConstantBuffer>> m_buffers;
    Ref<List<Ref<ShaderTechnique>>> m_techniques;
    List<Ref<ShaderParameter>> m_textureParameters;
    ShaderConstantBuffer* m_globalConstantBuffer;
    detail::ShaderSemanticsManager m_semanticsManager;

    friend class ShaderPass;
    friend class detail::ShaderHelper;
};

/**
 * シェーダプログラムに含まれる 1 つの入力パラメーターを表します。
 */
class LN_API ShaderParameter final
    : public Object
{
public:
    /** パラメータの名前を取得します。 */
    const String& name() const { return m_name; }

    /** bool 値を設定します。 */
    void setBool(bool value);

    /** 整数値を設定します。 */
    void setInt(int value);

    /** 整数値の配列を設定します。 */
    void setIntArray(const int* value, int count);

    /** 浮動小数点値を設定します。 */
    void setFloat(float value);

    /** 浮動小数点値の配列を設定します。 */
    void setFloatArray(const float* value, int count);

    /** ベクトルを設定します。 */
    void setVector(const Vector4& value);

    /** ベクトルの配列を設定します。 */
    void setVectorArray(const Vector4* value, int count);

    /** 行列を設定します。 */
    void setMatrix(const Matrix& value);

    /** 行列の配列を設定します。 */
    void setMatrixArray(const Matrix* value, int count);

    /** テクスチャを設定します。 */
    void setTexture(Texture* value);

    virtual void dispose() override;

private:
    LN_INTERNAL_NEW_OBJECT;
    ShaderParameter();
    virtual ~ShaderParameter() = default;
    void init(ShaderConstantBuffer* owner, const detail::ShaderUniformTypeDesc& desc, const String& name);
    void init(ShaderParameterClass parameterClass, const String& name);

    const detail::ShaderUniformTypeDesc& desc() const { return m_desc; }
    const Ref<Texture>& texture() const { return m_textureValue; }

    ShaderConstantBuffer* m_owner;
    ShaderParameterClass m_class;
    detail::ShaderUniformTypeDesc m_desc;
    String m_name;
    Ref<Texture> m_textureValue;

    friend class Shader;
    friend class ShaderPass;
    friend class detail::ShaderValueSerializer;
};

/**
 * シェーダプログラムに含まれる 1 つの定数バッファを表します。
 */
class LN_API ShaderConstantBuffer final
    : public Object
{
public:
    /** 定数バッファの名前を取得します。 */
    const String& name() const { return m_name; }

    /** 定数バッファにデータを設定します。通常、シェーダプログラムで定義したレイアウトと同じ構造体を転送します。 */
    void setData(const void* data, int size);

    /** 定数バッファのバイトサイズを取得します。 */
    int size() const { return m_buffer.size(); }

    /*
     * 名前を指定してこの定数バッファに含まれる ShaderParameter を検索します。
     *
     * @param[in]   name : パラメータの名前
     * @return      一致した ShaderParameter。見つからない場合は nullptr。
     */
    ShaderParameter* findParameter(const StringRef& name) const;

private:
    LN_INTERNAL_NEW_OBJECT;
    ShaderConstantBuffer();
    virtual ~ShaderConstantBuffer() = default;
    void init(Shader* owner, detail::IShaderUniformBuffer* rhiObject);

    Shader* owner() const { return m_owner; }
    const std::string& asciiName() const { return m_asciiName; }
    ByteBuffer& buffer() { return m_buffer; }
    void commit(detail::IShaderUniformBuffer* rhiObject);

    Shader* m_owner;
    String m_name;
    std::string m_asciiName;
    ByteBuffer m_buffer;
    List<Ref<ShaderParameter>> m_parameters;

    friend class Shader;
    friend class ShaderParameter;
    friend class ShaderPass;
};

/**
 * シェーダプログラムに含まれる 1 つのテクニックを表します。
 */
class LN_API ShaderTechnique final
    : public Object
{
public:
    /** この ShaderPass が含まれている Shader を取得します。 */
    Shader* shader() const { return m_owner; }

    /** テクニックの名前を取得します。 */
    const String& name() const { return m_name; }

    /** このテクニックに含まれる ShaderPass を取得します。 */
    Ref<ReadOnlyList<Ref<ShaderPass>>> passes() const;

private:
    LN_INTERNAL_NEW_OBJECT;
    ShaderTechnique();
    virtual ~ShaderTechnique();
    void init(const String& name);

    void setOwner(Shader* owner) { m_owner = owner; }
    void addShaderPass(ShaderPass* pass);

    Shader* m_owner;
    String m_name;
    Ref<List<Ref<ShaderPass>>> m_passes;
    detail::ShaderTechniqueClass m_techniqueClass;

    friend class Shader;
    friend class ShaderPass;
    friend class detail::ShaderHelper;
};

/**
 * シェーダプログラムに含まれる 1 つのパスを表します。
 */
class LN_API ShaderPass final
    : public Object
{
public:
    /** この ShaderPass が含まれている Shader を取得します。 */
    Shader* shader() const;

    /** パスの名前を取得します。 */
    const String& name() const { return m_name; }

    virtual void dispose() override;

private:
    LN_INTERNAL_NEW_OBJECT;
    ShaderPass();
    virtual ~ShaderPass();
    void init(const String& name, detail::IShaderPass* rhiPass, detail::ShaderRenderState* renderState = nullptr);

    struct ConstantBufferEntry
    {
        ShaderConstantBuffer* buffer;
        Ref<detail::IShaderUniformBuffer> rhiObject;
    };

    void setOwner(ShaderTechnique* owner) { m_owner = owner; }
    void setupParameters();
    void commit();
    void commitContantBuffers();
    detail::IShaderPass* resolveRHIObject();

    ShaderTechnique* m_owner;
    String m_name;
    Ref<detail::IShaderPass> m_rhiPass;
    List<ConstantBufferEntry> m_bufferEntries;
    List<ShaderParameter*> m_textureParameters;
    Ref<detail::ShaderRenderState> m_renderState;

    friend class Shader;
    friend class ShaderTechnique;
    friend class detail::ShaderValueSerializer;
    friend class GraphicsContext;
    friend class detail::ShaderHelper;
};

class ShaderCompilationProperties
    : public Object
{
public:
    void addIncludeDirectory(const StringRef& value);
    void addDefinition(const StringRef& value);
    void setDiagnostics(DiagnosticsManager* diag);

    LN_CONSTRUCT_ACCESS : ShaderCompilationProperties();
    virtual ~ShaderCompilationProperties();
    void init();

private:
    List<String> m_includeDirectories;
    List<String> m_definitions;
    Ref<DiagnosticsManager> m_diag;

    friend class Shader;
};

} // namespace ln
