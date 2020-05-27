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
class ShaderParameter2;
class ShaderTechnique;
class ShaderPass;
class ShaderDescriptorLayout;
class ShaderCompilationProperties;
class GraphicsContext;
namespace detail {
class ShaderHelper;
class ShaderManager;
class UnifiedShader;
class IShaderPass;
class IShaderUniformBuffer;
class ShaderTechniqueSemanticsManager;
}

// UniformBuffer, sampler など、Shader の Data を保持する。
// 本来は Pass に持たせるべきだが、そうするとユーザープログラムから Material(=Effect) として使いたい場合、
// 全く同じ ShaderDescriptor がたくさん作られることになり、メモリの無駄が多くなる。#136 の問題も出てくる。
// そのため ShaderPass と関係を持つのではなく、Shader と関係を持つようにする。
class ShaderDescriptor final
    : public Object
{
public:
    //int findUniformBufferIndex(const ln::StringRef& name) const;
    //int findTextureIndex(const ln::StringRef& name) const;
    //int findSamplerIndex(const ln::StringRef& name) const;
    ShaderDescriptorLayout* descriptorLayout() const;


    ShaderParameter2* findParameter2(const StringRef& name) const;

    void setData(int uniformBufferIndex, const void* data, size_t size);

    /** 整数値を設定します。 */
    void setInt(int memberIndex, int value);

    /** 整数値の配列を設定します。 */
    void setIntArray(int memberIndex, const int* value, int count);

    /** 浮動小数点値を設定します。 */
    void setFloat(int memberIndex, float value);

    /** 浮動小数点値の配列を設定します。 */
    void setFloatArray(int memberIndex, const float* value, int count);

    /** ベクトルを設定します。 */
    void setVector(int memberIndex, const Vector4& value);

    /** ベクトルの配列を設定します。 */
    void setVectorArray(int memberIndex, const Vector4* value, int count);

    /** 行列を設定します。 */
    void setMatrix(int memberIndex, const Matrix& value);

    /** 行列の配列を設定します。 */
    void setMatrixArray(int memberIndex, const Matrix* value, int count);

    /** テクスチャを設定します。 */
    void setTexture(int textureIndex, Texture* value);

    /** テクスチャを設定します。 */
    void setSampler(int textureIndex, Texture* value);

    /** SamplerState を設定します。 */
    void setSamplerState(int samplerIndex, SamplerState* value);

    const Ref<Shader> ownerShader() const { return m_ownerShader; }

LN_CONSTRUCT_ACCESS:
    ShaderDescriptor();
    bool init(Shader* ownerShader);

private:
    Shader* m_ownerShader;
    List<ByteBuffer> m_buffers;
    List<Ref<Texture>> m_textures;
    List<Ref<SamplerState>> m_samplers;
    List<Ref<ShaderParameter2>> m_parameters;
    int m_revision = 0;

    friend class ShaderPass;
};

// ShaderDescriptor と 1:1。つまり Global 情報。
// m_buffers, m_textures, m_samplers は ShaderDescriptor のそれぞれ と要素番号が一致する。
// 名前から UniformBuffer, Texture, SamplerState, Sampler を求める。
// 名前から UniformBuffer の Member を求める。
class ShaderDescriptorLayout final
    : public Object
{
public:
    int uniformBufferRegisterCount() const { return m_buffers.size(); }
    int textureRegisterCount() const { return m_textures.size(); }
    int samplerRegisterCount() const { return m_samplers.size(); }

    int findUniformBufferRegisterIndex(const ln::StringRef& name) const;
    int findUniformMemberIndex(const ln::StringRef& name) const;
    int findTextureRegisterIndex(const ln::StringRef& name) const;
    int findSamplerRegisterIndex(const ln::StringRef& name) const;

LN_CONSTRUCT_ACCESS:
    ShaderDescriptorLayout();
    bool init(const detail::DescriptorLayout& layout);

private:
    struct UniformBufferRegisterInfo
    {
        String name;
        size_t size;
        std::vector<int> members;
    };

    struct UniformMemberInfo
    {
        int uniformBufferRegisterIndex;
        String name;
        detail::ShaderUniformTypeDesc desc;
    };

    struct TextureRegisterInfo
    {
        String name;
    };

    struct SamplerRegisterInfo
    {
        String name;
    };

    Ref<Shader> m_ownerShader;
    List<UniformBufferRegisterInfo> m_buffers;
    List<UniformMemberInfo> m_members;
    List<TextureRegisterInfo> m_textures;
    List<SamplerRegisterInfo> m_samplers;

    friend class ShaderDescriptor;
    friend class ShaderPass;
    friend class ShaderParameter2;
    friend class detail::ShaderTechniqueSemanticsManager;

    /*
        Note:
        // DirectX 12
        // https://docs.microsoft.com/en-us/windows/win32/direct3d12/resource-binding-in-hlsl
        // SamplerState と sampler は s レジスタを使用する。
        Texture2D<float4> tex0 : register(t0);
        SamplerState samp0 : register(s0);
        sampler2D input : register(s0);

        Note:
        Vulkan は Inside RenderPass では vkCmdCopyBuffer 禁止なので、転送用の一時バッファが必要になる。
        効率よくバッファ確保するのに SingleFrameAllocator の仕組みを入れたいところ。
        ただ、このような制約があるのはぱっと見 Vulkan だけ。なので　Vulkan driver の中で隠蔽してみる。
    */
};

class ShaderParameter2 final
    : public Object
{
public:
    const String name() const;

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

    /** SamplerState を設定します。 */
    void setSamplerState(SamplerState* value);

    void setData(const void* data, size_t size);

    enum class IndexType
    {
        UniformBuffer,
        UniformMember,
        Texture,
        SamplerState,
    };

LN_INTERNAL_NEW_OBJECT;
    ShaderParameter2();
    bool init(ShaderDescriptor* owner, IndexType type, int dataIndex);

private:
    ShaderDescriptor* m_owner;
    IndexType m_indexType;
    int m_dataIndex = 0;     // Index of ShaderDescriptorLayout::m_buffers,m_members,m_textures,m_samplers
};

// ShaderPass が持つレイアウト情報。
// ShaderDescriptorLayout が共通でもっている各データを、どの binding に割り当てるかを管理する。
struct ShaderPassDescriptorLayout
{
    struct UniformBufferRegisterInfo
    {
        int8_t dataIndex; // Index of ShaderDescriptorLayout.m_buffers
        int8_t bindingIndex;
        uint8_t stageFlags; // ShaderStageFlags
    };

    struct TextureRegisterInfo
    {
        int8_t dataIndex; // Index of ShaderDescriptorLayout.m_textures
        int8_t bindingIndex;
        uint8_t stageFlags; // ShaderStageFlags
    };

    struct SamplerRegisterInfo
    {
        int8_t dataIndex; // Index of ShaderDescriptorLayout.m_samplers
        int8_t bindingIndex;
        uint8_t stageFlags; // ShaderStageFlags
    };

    List<UniformBufferRegisterInfo> m_buffers;
    List<TextureRegisterInfo> m_textures;
    List<SamplerRegisterInfo> m_samplers;

    void init(const detail::DescriptorLayout& layout, const ShaderDescriptorLayout* globalLayout);
};







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
#ifdef LN_NEW_SHADER_UBO
    ShaderParameter2* findParameter(const StringRef& name) const;
#else
    ShaderParameter* findParameter(const StringRef& name) const;

    /*
     * 名前を指定してこの Shader に含まれる ShaderConstantBuffer を検索します。
     *
     * @param[in]   name : 定数バッファの名前
     * @return      一致した ShaderConstantBuffer。見つからない場合は nullptr。
     */
    ShaderConstantBuffer* findConstantBuffer(const StringRef& name) const;
#endif

    /*
     * 名前を指定してこの Shader に含まれる ShaderTechnique を検索します。
     *
     * @param[in]   name : 定数バッファの名前
     * @return      一致した ShaderConstantBuffer。見つからない場合は nullptr。
     */
    ShaderTechnique* findTechnique(const StringRef& name) const;

    /** この Shader に含まれる ShaderTechnique を取得します。 */
    Ref<ReadOnlyList<Ref<ShaderTechnique>>> techniques() const;

    /** この Shader の DescriptorLayout をもとに、ShaderDescriptor を作成します。 */
    //Ref<ShaderDescriptor> createDescriptor();

    const Ref<ShaderDescriptor>& descriptor() const { return m_descriptor; }
    const Ref<ShaderDescriptorLayout>& descriptorLayout() const { return m_descriptorLayout; }


protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	Shader();
    virtual ~Shader();
    void init();
    void init(const StringRef& filePath, ShaderCompilationProperties* properties = nullptr);
    void init(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCompilationProperties* properties = nullptr);
    void init(const String& name, Stream* stream);

private:
    ShaderTechnique* findTechniqueByClass(const detail::ShaderTechniqueClass& techniqueClass) const;
    void createFromStream(Stream* stream, DiagnosticsManager* diag);
	void createFromUnifiedShader(detail::UnifiedShader* unifiedShader, DiagnosticsManager* diag);
    ShaderConstantBuffer* getOrCreateConstantBuffer(detail::IShaderUniformBuffer* buffer);
    ShaderParameter* getOrCreateTextureParameter(const String& name);

    detail::ShaderManager* m_manager;
    String m_name;
    Ref<ShaderDescriptorLayout> m_descriptorLayout;
    Ref<ShaderDescriptor> m_descriptor;
    List<Ref<ShaderConstantBuffer>> m_buffers;
    Ref<List<Ref<ShaderTechnique>>> m_techniques;
    List<Ref<ShaderParameter>> m_textureParameters;
    ShaderConstantBuffer* m_globalConstantBuffer;

    friend class ShaderPass;
    friend class detail::ShaderHelper;
    friend class ShaderDescriptor;
};

/**
 * シェーダプログラムに含まれる 1 つの入力パラメーターを表します。
 */
// TODO: deprecated
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

protected:
    virtual void onDispose(bool explicitDisposing) override;

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
    friend class ShaderConstantBuffer;
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

    /** 
     * 定数バッファにデータを設定します。通常、シェーダプログラムで定義したレイアウトと同じ構造体を転送します。
     *
     * データ内の float4x4 は自動的に転置されます。それ以外の行列型は呼び出し側で転置しないと、mul(v, M) の計算順に適合しません。
     */
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
    void commit(GraphicsContext* graphicsContext, detail::IShaderUniformBuffer* rhiObject);

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

    detail::ShaderTechniqueSemanticsManager* semanticsManager2() { return m_semanticsManager.get(); }

private:
    LN_INTERNAL_NEW_OBJECT;
    ShaderTechnique();
    virtual ~ShaderTechnique();
    void init(const String& name);
    void setupSemanticsManager();

    void setOwner(Shader* owner) { m_owner = owner; }
    void addShaderPass(ShaderPass* pass);

    Shader* m_owner;
    String m_name;
    Ref<List<Ref<ShaderPass>>> m_passes;
    detail::ShaderTechniqueClass m_techniqueClass;
    std::unique_ptr<detail::ShaderTechniqueSemanticsManager> m_semanticsManager;

    friend class Shader;
    friend class ShaderPass;
    friend class detail::ShaderHelper;
    friend class detail::ShaderTechniqueSemanticsManager;
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


    // TODO: for test
    const ShaderPassDescriptorLayout& descriptorLayout() const { return m_descriptorLayout; }

protected:
    virtual void onDispose(bool explicitDisposing) override;

private:
    LN_INTERNAL_NEW_OBJECT;
    ShaderPass();
    virtual ~ShaderPass();
    void init(const String& name, detail::IShaderPass* rhiPass, detail::ShaderRenderState* renderState, const detail::DescriptorLayout& layout, const ShaderDescriptorLayout* globalLayout);

    void setOwner(ShaderTechnique* owner) { m_owner = owner; }
    detail::IShaderPass* resolveRHIObject(GraphicsContext* graphicsContext, bool* outModified);
    void submitShaderDescriptor(GraphicsContext* graphicsContext, detail::ICommandList* commandList, const ShaderDescriptor* descripter, bool* outModified);

    ShaderTechnique* m_owner;
    String m_name;
    Ref<detail::IShaderPass> m_rhiPass;
    ShaderPassDescriptorLayout m_descriptorLayout;

    Ref<detail::ShaderRenderState> m_renderState;
    const ShaderDescriptor* m_lastShaderDescriptor = nullptr;
    int m_lastShaderDescriptorRevision = 0;

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
