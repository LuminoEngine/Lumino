// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include <LuminoEngine/Asset/AssetObject.hpp>
#include <LuminoGraphics/ShaderCompiler/ShaderHelper.hpp>
#include "GraphicsResource.hpp"
#include "ShaderInterfaceFramework.hpp"

namespace ln {
class AssetImportSettings;
class DiagnosticsManager;
class GraphicsCommandList;
class Texture;
class Shader;
class ShaderParameter2;
class ShaderTechnique;
class ShaderPass;
class ShaderDescriptorLayout;
class ShaderCompilationProperties;
class GraphicsCommandList;
namespace detail {
class IShaderPass;
class IDescriptorPool;
class ShaderTechniqueSemanticsManager;
class ShaderValueSerializer;
class ShaderInternal;
}
namespace kokage {
class UnifiedShader;
}

// UniformBuffer, sampler など、Shader の Data を保持する。
// 本来は Pass に持たせるべきだが、そうするとユーザープログラムから Material(=Effect) として使いたい場合、
// 全く同じ ShaderDefaultDescriptor がたくさん作られることになり、メモリの無駄が多くなる。#136 の問題も出てくる。
// そのため ShaderPass と関係を持つのではなく、Shader と関係を持つようにする。
/*
  [2020/12/15] ShaderDefaultDescriptor 計画
  ----------
    ShaderDefaultDescriptor = DescriptorSet。
    正しくやるなら、これは通常、SceneRenderer からのドローコール1つに付き1つのインスタンスが必要になる。
    それへ、Objectごとに変わるUniformBuffer, Viewごとに変わるUniformBuffer, マテリアルのUniformBufferなどをアタッチして使う。
    Material 1つ = ShaderDefaultDescriptor 1 つ、ではない。
    つまり Pool 必須。VulkanDriver とほぼ同じ実装が必要になる。

    少しずつ移行するなら…

    まず UniformBuffer を扱えるようにする。
    現状 m_descriptor->setData() で struct を設定している個所で、SingleFrame な UniformBuffer を
    GraphicsCommandList から作り、それ経由でデータをセットするように変更する。

    極限まで最適化するなら SingleFrame 以外の UniformBuffer もほしいが、ひとまず保留。
    というか Vulkan の allocationCount の問題があったけど、Buffer 自体はそれほど多く作れるものではない。
    またMaterial から使うときも、結局 Material 自体は UniformBuffer を持たずに map で値を持っていて、
    SceneRenderer から submit するときに その map のデータを SingleFrame UniformBuffer へ転記する。
    ※こうしないと Material のシリアライズが大変。

    次に ShaderDefaultDescriptor の Pool を作る。
    必須ではない気もするけど、UniformBuffer は個別にしたのに DescriptorSet は Shader ごとに唯一なインスタンスなのが不自然。
    RHI 側の肥大化も防止したい。DX12 や Metal 対応の前にやっておきたいところ。

    ShaderDefaultDescriptor はすべて SingleFrame なオブジェクトなので、GraphicsCommandList に shader を与えて生成してもらう感じがいいかな。

    ### Shader 自体が値を保持する必要は？

    無し、でいいと思う。その役割は Material が負う。

    でも HC4 では ひとつのShaderをたくさんの Material にセットするとき、シーン内で唯一の意味を持つ値を、
    全部の Material 側に設定するのがイヤだったから Shder に直接設定していた。
    実際これはメモリ削減に有効に働いた。手間も少なかったし。

    「Materialに値が設定されなったときの規定値」として Shader が値を持つのはアリかも。

    「規定値を設定しておく ShaderDescriptor」を1つ、Shader に持たせる方法がよさそう？
    ただこの人は UniformBuffer ではなく普通の ByteArray を持つようにしなければならない。


    SceneRenderer 辺りからの使われ方は、だいたい次のようになる。
    ```
    viewUniformBuffer = シーン描画開始時に確定して共有

    // Subset 単位
    auto* descriptor = shader->allocateDescriptor();
    descriptor->setUniformBuffer("LNRenderViewBuffer", viewUniformBuffer);
    descriptor->setUniformBuffer("LNRenderElementBuffer", elementUniformBuffer);
    descriptor->setUniformBuffer("LNEffectColorBuffer", effectUniformBuffer);
    descriptor->setUniformBuffer("$Global", Materialが持つユーザー指定のParams);
    descriptor->setUniformBuffer("xxxx", Material::setData()で指定されたユーザー指定のstructデータから作ったUB);
    ```
    "$Global" と "xxxx" にはデフォルト値を使いたい。


    [2020/12/16] そもそも ShaderDescriptor をたくさん作れるようにする必要はある？
    ----------
    SceneRenderer から本気でマルチスレッドでコマンドリスト作る場合…かな。

    Vulkan は API の仕様上 Pool からたくさんの Descriptor を作る必要があるけど、
    ラッパー側では 092f21e7507a6480d61249480ad267bb1916e22f 時点のように
    単一の固定長 array を持った struct に UniformBuffer や Texture をセットして、
    submit するだけでかまわない。

    モダンな GraphicsAPI としては正しいのかもしれないけど、
    結局 RHI とラッパーで同じようなデータ構造を用意する必要があり、
    複雑さが上がる一方、メモリ消費・実行速度的に嬉しいことがあまりない。

    ゲームのように複雑なシーンで動く絵を出すためには、実質ドローコールのたびに
    Descriptor を作らなければならないためかなりのメモリ消費になる。

    またラッパー側で GraphicsCommandList をバックバッファの数だけ持つようにして
    ローテーション組む用になっているが、このときどの GraphicsCommandList と DescriptorPool を
    対応付けるか、という問題も出てくる。(VulkanDriver と同じような方法で解決せざるを得ないけど)
    単純に複雑さが上がる。


    [2020/12/16] それでも ShaderDescriptor が必要？
    ----------
    UniformBuffer をサポートしたことで Shader へデータをセットする API が複雑になってきたので、
    エンジンの 内部から使うものと、外部のユーザープログラムから使うものを区別したい。

    Unity とかは Shader に対して直接フィールドをセットすることはできないけど、
    前述の通り直接設定したほうが作業効率が良いこともある。

    ただ GraphicsCommandList を使うときにもこれが必要になるので、どのみち公開することになる。
    Binding への公開はそもそも Graphics 全体の課題なのでいいとしても、
    Descriptor 自体はある程度本来の使い方に似せたデザインにしたい。

    
*/
class ShaderDefaultDescriptor final
    : public Object
{
public:
    //int findUniformBufferIndex(const ln::StringView& name) const;
    //int findTextureIndex(const ln::StringView& name) const;
    //int findSamplerIndex(const ln::StringView& name) const;
    ShaderDescriptorLayout* descriptorLayout() const;


    ShaderParameter2* findParameter2(const StringView& name) const;

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
    ShaderDefaultDescriptor();
    bool init(Shader* ownerShader);

 public:    // TODO:
    Shader* m_ownerShader;
    List<ByteBuffer> m_buffers;
    List<Ref<Texture>> m_textures;
    List<Ref<SamplerState>> m_samplers;
    List<Ref<ShaderParameter2>> m_parameters;
    int m_revision = 0;

    friend class ShaderPass;
};

// ShaderDefaultDescriptor と 1:1。つまり Global 情報。
// m_buffers, m_textures, m_samplers は ShaderDefaultDescriptor のそれぞれ と要素番号が一致する。
// 名前から UniformBuffer, Texture, SamplerState, Sampler を求める。
// 名前から UniformBuffer の Member を求める。
class ShaderDescriptorLayout final
    : public Object
{
public:
    int uniformBufferRegisterCount() const { return m_buffers.size(); }
    int textureRegisterCount() const { return m_textures.size(); }
    int samplerRegisterCount() const { return m_samplers.size(); }

    int findUniformBufferRegisterIndex(const ln::StringView& name) const;
    int findUniformMemberIndex(const ln::StringView& name) const;
    int findTextureRegisterIndex(const ln::StringView& name) const;
    int findSamplerRegisterIndex(const ln::StringView& name) const;
    int findStorageRegisterIndex(const ln::StringView& name) const;

LN_CONSTRUCT_ACCESS:
    ShaderDescriptorLayout();
    bool init(const kokage::DescriptorLayout& layout);

public: // TODO:
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

    struct StorageRegisterInfo
    {
        String name;
    };

    Ref<Shader> m_ownerShader;
    List<UniformBufferRegisterInfo> m_buffers;
    List<UniformMemberInfo> m_members;
    List<TextureRegisterInfo> m_textures;
    List<SamplerRegisterInfo> m_samplers;
    List<StorageRegisterInfo> m_storages;

    friend class ShaderDefaultDescriptor;
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
    void setInt(int value, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

    /** 整数値の配列を設定します。 */
    void setIntArray(const int* value, int count, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

    /** 浮動小数点値を設定します。 */
    void setFloat(float value, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

    /** 浮動小数点値の配列を設定します。 */
    void setFloatArray(const float* value, int count, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

    /** ベクトルを設定します。 */
    void setVector(const Vector4& value, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

    /** ベクトルの配列を設定します。 */
    void setVectorArray(const Vector4* value, int count, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

    /** 行列を設定します。 */
    void setMatrix(const Matrix& value, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

    /** 行列の配列を設定します。 */
    void setMatrixArray(const Matrix* value, int count, detail::ShaderSecondaryDescriptor* descriptor = nullptr);

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
    bool init(ShaderDefaultDescriptor* owner, IndexType type, int dataIndex);

public: // TODO:
    ShaderDefaultDescriptor* m_owner;
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

    struct StorageRegisterInfo
    {
        int8_t dataIndex; // Index of ShaderDescriptorLayout.m_storages
        int8_t bindingIndex;
        uint8_t stageFlags; // ShaderStageFlags
    };

    List<UniformBufferRegisterInfo> m_buffers;
    List<TextureRegisterInfo> m_textures;
    List<SamplerRegisterInfo> m_samplers;
    List<StorageRegisterInfo> m_storages;

    void init(const kokage::DescriptorLayout& layout, const ShaderDescriptorLayout* globalLayout);
};







/**
 * シェーダーを表すクラスです。
 *
 * このクラスは DirectX9 ～ 11 世代に提供されていた「エフェクト」と同様の、シェーダーステージをまとめて管理・適用する機能を持っています。
 *
 * @note
 * パラメータは通常 Material 経由で設定するが、Shader に直接設定することも可能。
 * その場合 Material 側で持つデータ量や転送のオーバーヘッドは少なくなるが、以下の点に注意すること。
 * - 値をシリアライズすることはできなくなる。
 * - 複数の Material から参照される場合、値は共有される。
 *
 */
LN_CLASS()
class LN_API Shader
    : public AssetObject
    , public IGraphicsResource
{
public:
    /**
     * 事前コンパイル済みシェーダファイルまたはシェーダプログラムファイルから Shader オブジェクトを作成します。
     *
     * @param[in]   filePath : 入力ファイル名
     *
     * シェーダプログラムファイル の読み込み機能はデバッグを目的として用意されています。
     *
     * シェーダプログラムファイルを読み込むことができるのは、デスクトップターゲットのみです。
     * モバイルターゲット、Web ターゲットでは事前コンパイル済みシェーダファイルのみを読み込むことができます。
     */
    static Ref<Shader> create(const StringView& filePath, ShaderCompilationProperties* properties = nullptr);

    /**
     * load
     */
    LN_METHOD()
    static Ref<Shader> load(const StringView& filePath, AssetImportSettings* settings = nullptr);

    /**
     * Lumino の独自拡張 (technique 構文など) を使用しない HLSL シェーダをコンパイルし、Shader オブジェクトを作成します。
     * 
     * 作成された Shader は、1 つの ShaderTechnique と 1 つの ShaderPass を持ちます。
     */
    static Ref<Shader> create(const StringView& vertexShaderFilePath, const StringView& pixelShaderFilePath, ShaderCompilationProperties* properties = nullptr);

    /**
     * 名前を指定してこの Shader に含まれる ShaderParameter を検索します。
     *
     * @param[in]   name : パラメータの名前
     * @return      一致した ShaderParameter。見つからない場合は nullptr。
     *
     * @attention 現在、SamplerState の設定は未サポートです。将来的には、この関数で SamplerState 型のパラメータを検索できるようにする予定です。
     */
    ShaderParameter2* findParameter(const StringView& name) const;

    /**
     * 名前を指定してこの Shader に含まれる ShaderTechnique を検索します。
     *
     * @param[in]   name : 定数バッファの名前
     * @return      一致した ShaderConstantBuffer。見つからない場合は nullptr。
     */
    ShaderTechnique* findTechnique(const StringView& name) const;

    /** この Shader に含まれる ShaderTechnique を取得します。 */
    Ref<ReadOnlyList<Ref<ShaderTechnique>>> techniques() const;


    /** 浮動小数点値を設定します。 */
    LN_METHOD()
    void setFloat(const StringView& parameterName, float value);
    
    /** ベクトル値を設定します。 */
    LN_METHOD(OverloadPostfix = "3")
    void setVector(const StringView& parameterName, const Vector3& value);

    /** ベクトル値を設定します。 */
    LN_METHOD(OverloadPostfix = "4")
    void setVector(const StringView& parameterName, const Vector4& value);

    /** setTexture */
    LN_METHOD()
    void setTexture(const StringView& parameterName, Texture* value);

    // ↑このあたりは HC4 のエフェクトで、ひとつの Shader をたくさんの Material から参照するときの共通パラメータを設定したいため、公開した。

    /** この Shader の DescriptorLayout をもとに、ShaderDescriptor を作成します。 */
    //Ref<ShaderDefaultDescriptor> createDescriptor();
    Ref<detail::ShaderSecondaryDescriptor> acquireDescriptor();

    const Ref<ShaderDefaultDescriptor>& descriptor() const { return m_descriptor; }
    const Ref<ShaderDescriptorLayout>& descriptorLayout() const { return m_descriptorLayout; }


protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;
    void onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath) override;

LN_CONSTRUCT_ACCESS:
	Shader();
    virtual ~Shader();
    void init();
    void init(const StringView& filePath, ShaderCompilationProperties* properties = nullptr);
    void init(const StringView& vertexShaderFilePath, const StringView& pixelShaderFilePath, ShaderCompilationProperties* properties = nullptr);
    void init(const String& name, Stream* stream);
    void init(kokage::UnifiedShader* unifiedShader, DiagnosticsManager* diag);

private:
    ShaderTechnique* findTechniqueByClass(const kokage::ShaderTechniqueClass& techniqueClass) const;
    bool loadFromStream(const detail::AssetPath& path, Stream* stream, ShaderCompilationProperties* properties);
    void createFromStream(Stream* stream, DiagnosticsManager* diag);
    void createFromUnifiedShader(kokage::UnifiedShader* unifiedShader, DiagnosticsManager* diag);

    detail::GraphicsManager* m_graphicsManager;
    String m_name;
    Ref<ShaderDescriptorLayout> m_descriptorLayout;
    Ref<ShaderDefaultDescriptor> m_descriptor;
    Ref<detail::ShaderSecondaryDescriptor> m_descriptor2;
    Ref<List<Ref<ShaderTechnique>>> m_techniques;

    friend class ShaderPass;
    friend class detail::ShaderInternal;
    friend class ShaderDefaultDescriptor;
    friend class detail::GraphicsResourceInternal;
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
    void init(const String& name, const kokage::ShaderTechniqueClass& techniqueClass);
    void setupSemanticsManager();

    void setOwner(Shader* owner) { m_owner = owner; }
    void addShaderPass(ShaderPass* pass);

    Shader* m_owner;
    String m_name;
    Ref<List<Ref<ShaderPass>>> m_passes;
    kokage::ShaderTechniqueClass m_techniqueClass;
    std::unique_ptr<detail::ShaderTechniqueSemanticsManager> m_semanticsManager;

    friend class Shader;
    friend class ShaderPass;
    friend class detail::ShaderInternal;
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

    // CommandBuffer に対するインターフェイス
    Ref<detail::IDescriptorPool> getDescriptorSetsPool();
    void releaseDescriptorSetsPool(detail::IDescriptorPool* pool);

    bool isComputeShader() const { return !m_descriptorLayout.m_storages.isEmpty(); }

protected:
    virtual void onDispose(bool explicitDisposing) override;

private:
    LN_INTERNAL_NEW_OBJECT;
    ShaderPass();
    virtual ~ShaderPass();
    void init(const String& name, detail::IShaderPass* rhiPass, kokage::ShaderRenderState* renderState, const kokage::DescriptorLayout& layout, const ShaderDescriptorLayout* globalLayout);

    void setOwner(ShaderTechnique* owner) { m_owner = owner; }
    detail::IShaderPass* resolveRHIObject(GraphicsCommandList* graphicsContext, bool* outModified);
    void submitShaderDescriptor2(GraphicsCommandList* graphicsContext, const detail::ShaderSecondaryDescriptor* descripter, bool* outModified);

    ShaderTechnique* m_owner;
    String m_name;
    Ref<detail::IShaderPass> m_rhiPass;
    ShaderPassDescriptorLayout m_descriptorLayout;

    Ref<kokage::ShaderRenderState> m_renderState;
    const ShaderDefaultDescriptor* m_lastShaderDescriptor = nullptr;
    int m_lastShaderDescriptorRevision = 0;

    std::vector<Ref<detail::IDescriptorPool>> m_descriptorSetsPools;

    friend class Shader;
    friend class ShaderTechnique;
    friend class detail::ShaderValueSerializer;
    friend class GraphicsCommandList;
    friend class detail::ShaderInternal;
};

class ShaderCompilationProperties
    : public Object
{
public:
    void addIncludeDirectory(const StringView& value);
    void addDefinition(const StringView& value);
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

namespace detail {
class ShaderInternal
{
public:
    static kokage::ShaderRenderState* getShaderRenderState(ShaderPass* pass);
    static ShaderTechnique* findTechniqueByClass(const Shader* shader, const kokage::ShaderTechniqueClass& techniqueClass);
    static const kokage::ShaderTechniqueClass& techniqueClass(ShaderTechnique* technique);
};
} // namespace detail
} // namespace ln
