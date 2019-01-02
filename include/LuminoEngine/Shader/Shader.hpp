
#pragma once
#include "Common.hpp"
#include "../Graphics/GraphicsResource.hpp"
#include "ShaderInterfaceFramework.hpp"
#include "ShaderHelper.hpp"

namespace ln {
class DiagnosticsManager;
//class ShaderParameter;
class Texture;
class Shader;
class ShaderConstantBuffer;
class ShaderParameter;
class ShaderTechnique;
class ShaderPass;
class GraphicsContext;
namespace detail {
class ShaderManager;
class IShaderPass;
class IShaderUniformBuffer;
//class HLSLPass;
}


enum class ShaderCodeType
{
	RawGLSL,
};

class ShaderCompilationProperties
	: public Object
{
public:
	void addIncludeDirectory(const StringRef& value);
	void addDefinition(const StringRef& value);
	void setDiagnostics(DiagnosticsManager* diag);

LN_CONSTRUCT_ACCESS:
	ShaderCompilationProperties();
	virtual ~ShaderCompilationProperties();
	void initialize();

private:
	List<String> m_includeDirectories;
	List<String> m_definitions;
	Ref<DiagnosticsManager> m_diag;

	friend class Shader;
};

/**
 * プログラマブルシェーダーを表すクラスです。
 *
 * このクラスは DirectX9 ～ 11 世代に提供されていた「エフェクト」と同様の、シェーダーステージをまとめて管理・適用する機能を持っています。
 */
class LN_API Shader
	: public GraphicsResource
{
public:
	static Ref<Shader> create(const StringRef& hlslEffectFilePath, ShaderCompilationProperties* properties = nullptr);

    /*
     * Lumino の独自拡張 (technique 構文など) を使用しない HLSL シェーダをコンパイルし、Shader オブジェクトを作成します。
     * 
     * 作成された Shader は、1 つの ShaderTechnique と 1 つの ShaderPass を持ちます。
     */
	static Ref<Shader> create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCompilationProperties* properties = nullptr);

    /*
     * 名前を指定して ShaderParameter を検索します。
     *
     * @param[in] name : パラメータの名前
     *
     * この関数の検索対象は次の通りです。
     * - "_Global" という名前の付いた ShaderConstantBuffer (cbuffer 構文を使用せず、グローバルスコープに直接定義した変数)
     * - texture 型のパラメータ
     *
     * @attention 現在、SamplerState の設定は未サポートです。将来的には、この関数で SamplerState 型のパラメータを検索できるようにする予定です。
     */
	ShaderParameter* findParameter(const StringRef& name) const;

	ShaderConstantBuffer* findConstantBuffer(const StringRef& name) const;
	ShaderTechnique* findTechnique(const StringRef& name) const;

	const List<Ref<ShaderTechnique>>& techniques() const { return m_techniques; }

	// TODO: inernal
	detail::ShaderSemanticsManager* semanticsManager() { return& m_semanticsManager; }
	ShaderTechnique* findTechniqueByClass(const detail::ShaderTechniqueClass& techniqueClass) const;

LN_CONSTRUCT_ACCESS:
	Shader();
	virtual ~Shader();
	void initialize();
	void initialize(const StringRef& hlslEffectFilePath, ShaderCompilationProperties* properties = nullptr);
	void initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType, ShaderCompilationProperties* properties = nullptr);
    void initialize(Stream* stream);
	virtual void dispose() override;

	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
    void createFromUnifiedShader(Stream* stream, DiagnosticsManager* diag);
	Ref<detail::IShaderPass> createShaderPass(
		const char* vsData, size_t vsLen, const char* vsEntryPoint,
		const char* psData, size_t psLen, const char* psEntryPoint,
		DiagnosticsManager* diag, ShaderCompilationProperties* properties);
	Ref<detail::IShaderPass> createRHIShaderPass(
		const char* vsData, size_t vsLen,
		const char* psData, size_t psLen,
		DiagnosticsManager* diag);
	void createSinglePassShader(const char* vsData, size_t vsLen, const char* psData, size_t psLen, DiagnosticsManager* diag, ShaderCompilationProperties* properties);
	void postInitialize();
	//ShaderParameter* getShaderParameter(const detail::ShaderUniformTypeDesc& desc, const String& name);
	ShaderConstantBuffer* getOrCreateConstantBuffer(detail::IShaderUniformBuffer* buffer);
	ShaderParameter* getOrCreateTextureParameter(const String& name);

	detail::ShaderManager* m_manager;
	//Ref<DiagnosticsManager> m_diag;
	//List<Ref<ShaderParameter>> m_parameters;
	List<Ref<ShaderConstantBuffer>> m_buffers;
	List<Ref<ShaderTechnique>> m_techniques;

	List<Ref<ShaderParameter>> m_textureParameters;

	ShaderConstantBuffer* m_globalConstantBuffer;
	detail::ShaderSemanticsManager m_semanticsManager;

	friend class ShaderPass;
};


template class LN_API List<Ref<ShaderConstantBuffer>>;


enum class ShaderParameterClass
{
	Constant,
	Texture,
	Sampler,
};

class LN_API ShaderParameter
	: public Object
{
public:
	const String& name() const { return m_name; }

	void setBool(bool value);
	//void setBoolArray(const bool* value, int count);
	void setInt(int value);
	void setIntArray(const int* value, int count);
	void setFloat(float value);
	void setFloatArray(const float* value, int count);
	void setVector(const Vector4& value);
	void setVectorArray(const Vector4* value, int count);
	void setMatrix(const Matrix& value);
	void setMatrixArray(const Matrix* value, int count);
	void setTexture(Texture* value);
	void setPointer(void* value);

    // TODO: internal
    //void setShaderParameterValue(const detail::ShaderParameterValue& value) { m_value = value; }

LN_CONSTRUCT_ACCESS:
	ShaderParameter();
	virtual ~ShaderParameter();
	void initialize(ShaderConstantBuffer* owner, const detail::ShaderUniformTypeDesc& desc, const String& name);
	void initialize(ShaderParameterClass parameterClass, const String& name);
	virtual void dispose() override;

private:
	const detail::ShaderUniformTypeDesc& desc() const { return m_desc; }
	const Ref<Texture>& texture() const { return m_textureValue; }
	
	ShaderParameterClass m_class;
	ShaderConstantBuffer* m_owner;
	detail::ShaderUniformTypeDesc m_desc;
	String m_name;
	//detail::ShaderParameterValue m_value;

	Ref<Texture> m_textureValue;

	friend class Shader;
	friend class ShaderPass;
	friend class detail::ShaderValueSerializer;
};

class LN_API ShaderConstantBuffer
	: public Object
{
public:
	const String& name() const { return m_name; }
	void setData(const void* data, int size);
    int size() const { return m_buffer.size(); }

	ShaderParameter* findParameter(const StringRef& name) const;

LN_CONSTRUCT_ACCESS:	// TODO: 内部でしか new しないから private とかでいい気がする
	ShaderConstantBuffer();
	virtual ~ShaderConstantBuffer();
	void initialize(Shader* owner, detail::IShaderUniformBuffer* rhiObject);

private:
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

class LN_API ShaderTechnique
	: public Object
{
public:
	Shader* shader() const { return m_owner; }
	const String& name() const { return m_name; }

	const List<Ref<ShaderPass>>& passes() const { return m_passes; }

	// TODO: internal
	const detail::ShaderTechniqueClass& techniqueClass() const { return m_techniqueClass; }

LN_CONSTRUCT_ACCESS:
	ShaderTechnique();
	virtual ~ShaderTechnique();
	void initialize(const String& name);

private:
	void setOwner(Shader* owner) { m_owner = owner; }
	void addShaderPass(ShaderPass* pass);

	Shader* m_owner;
	String m_name;
	List<Ref<ShaderPass>> m_passes;
	detail::ShaderTechniqueClass m_techniqueClass;

	friend class Shader;
	friend class ShaderPass;
};

class LN_API ShaderPass
	: public Object
{
public:
	/** この ShaderPass が含まれている Shader を取得します。 */
	Shader* shader() const;

LN_CONSTRUCT_ACCESS:
	ShaderPass();
	virtual ~ShaderPass();
	void initialize(detail::IShaderPass* rhiPass, detail::ShaderRenderState* renderState = nullptr);
	virtual void dispose() override;

private:
    struct ConstantBufferEntry
    {
        ShaderConstantBuffer* buffer;
        Ref<detail::IShaderUniformBuffer> rhiObject;
    };

	void setOwner(ShaderTechnique* owner) { m_owner = owner; }
	void setupParameters();
	void commit();
	detail::IShaderPass* resolveRHIObject();

	ShaderTechnique* m_owner;
	Ref<detail::IShaderPass> m_rhiPass;
	//List<ShaderParameter*> m_parameters;
	List<ConstantBufferEntry> m_bufferEntries;
	List<ShaderParameter*> m_textureParameters;
    Ref<detail::ShaderRenderState> m_renderState;

	friend class Shader;
	friend class ShaderTechnique;
	friend class detail::ShaderValueSerializer;
	friend class GraphicsContext;
    friend class detail::ShaderHelper;
};



} // namespace ln
