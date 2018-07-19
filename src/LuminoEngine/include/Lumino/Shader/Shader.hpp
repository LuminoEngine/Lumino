
#pragma once
#include "Common.hpp"
#include "../Graphics/GraphicsResource.hpp"

namespace ln {
class DiagnosticsManager;
class ShaderParameter;
class ShaderConstantBuffer;
class ShaderTechnique;
class ShaderPass;
class GraphicsContext;

class Texture
	: public GraphicsResource
{
};

namespace detail {
class IShaderPass;
class IShaderUniformBuffer;

class ShaderParameterValue
{
public:
	ShaderParameterValue();

	void reset(ShaderVariableType type, int elements);

	ShaderVariableType type() const { return m_type; }

	void setBool(bool value);
	bool getBool() const { return m_data.Bool; }
	void setInt(int value);
	int getInt() const { return m_data.Int; }
	void setBoolArray(const bool* values, int count);
	const bool* getBoolArray() const { return reinterpret_cast<const bool*>(m_buffer.data()); }
	void setFloat(float value);
	float getFloat() const { return m_data.Float; }
	void setFloatArray(const float* values, int count);
	const float* getFloatArray() const { return reinterpret_cast<const float*>(m_buffer.data()); }
	void setVector(const Vector4& vec);
	const Vector4& getVector() const { return *reinterpret_cast<const Vector4*>(m_data.Vector4); }
	void setVectorArray(const Vector4* vectors, int count);
	const Vector4* getVectorArray() const { return reinterpret_cast<const Vector4*>(m_buffer.data()); }
	void setMatrix(const Matrix& matrix);
	const Matrix& getMatrix() const { return *reinterpret_cast<const Matrix*>(m_data.Matrix4x4); }
	void setMatrixArray(const Matrix* matrices, int count);
	const Matrix* getMatrixArray() const { return reinterpret_cast<const Matrix*>(m_buffer.data()); }
	void setTexture(Texture* texture);
	Texture* getTexture() const { return m_data.texture; }
	void setPointer(void* value);
	void* getPointer() const { return m_data.Pointer; }

	int getDataByteSize() const;
	int getArrayLength() const;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(ShaderParameterValue);

	void dirty();
	void allocValueBuffer(size_t size);
	static bool isShortSizeType(ShaderVariableType type);

	union Data
	{
		bool Bool;
		int				Int;
		float			Float;
		float		Vector4[4];
		float	Matrix4x4[16];
		Texture* texture;
		void* Pointer;		// any object pointer (e.g Texture)
	};

	ShaderVariableType m_type;
	Data m_data;
	ByteBuffer m_buffer;
};

class ShaderValueSerializer
{
public:
	static size_t measureBufferSize(const ShaderPass* pass);

	ShaderValueSerializer(void* buffer, size_t size);

	//void beginWriteValues(void* buffer);
	void writeValue(const ShaderParameterValue& value);
	//void endWriteValues();

private:
	MemoryStream m_stream;
	BinaryWriter m_writer;

};

class ShaderValueDeserializer
{
public:
	ShaderValueDeserializer(const void* buffer, size_t size);
	const void* readValue(size_t* outSize, ShaderVariableType* outType);	// return nullptr if eof
	//void endReadValues();

private:
	MemoryStream m_stream;
	BinaryReader m_reader;
};

} // namespace detail

enum class ShaderCodeType
{
	RawGLSL,
};

class LN_API Shader
	: public GraphicsResource
{
public:
	static Ref<Shader> create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath);

	void setBool(const StringRef& name, bool value);
	void setInt(const StringRef& name, int value);
	void setBoolArray(const StringRef& name, const bool* value, int count);
	void setFloat(const StringRef& name, float value);
	void setFloatArray(const StringRef& name, const float* value, int count);
	void setVector(const StringRef& name, const Vector4& value);
	void setVectorArray(const StringRef& name, const Vector4* value, int count);
	void setMatrix(const StringRef& name, const Matrix& value);
	void setMatrixArray(const StringRef& name, const Matrix* value, int count);
	void setTexture(const StringRef& name, Texture* value);

	ShaderParameter* findParameter(const StringRef& name);
	ShaderConstantBuffer* findConstantBuffer(const StringRef& name);

	const List<Ref<ShaderTechnique>>& techniques() const { return m_techniques; }

LN_CONSTRUCT_ACCESS:
	Shader();
	virtual ~Shader();
	void initialize();
	void initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType);
	virtual void dispose() override;

	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	ShaderParameter* getShaderParameter(const detail::ShaderUniformTypeDesc& desc, const String& name);
	ShaderConstantBuffer* getOrCreateConstantBuffer(detail::IShaderUniformBuffer* buffer);

	Ref<DiagnosticsManager> m_diag;
	List<Ref<ShaderParameter>> m_parameters;
	List<Ref<ShaderConstantBuffer>> m_buffers;
	List<Ref<ShaderTechnique>> m_techniques;

	friend class ShaderPass;
};


class LN_API ShaderParameter
	: public Object
{
public:
	void setBool(bool value);
	void setInt(int value);
	void setBoolArray(const bool* value, int count);
	void setFloat(float value);
	void setFloatArray(const float* value, int count);
	void setVector(const Vector4& value);
	void setVectorArray(const Vector4* value, int count);
	void setMatrix(const Matrix& value);
	void setMatrixArray(const Matrix* value, int count);
	void setTexture(Texture* value);
	void setPointer(void* value);

LN_CONSTRUCT_ACCESS:
	ShaderParameter();
	virtual ~ShaderParameter();
	void initialize(const detail::ShaderUniformTypeDesc& desc, const String& name);
	virtual void dispose() override;

private:
	const detail::ShaderUniformTypeDesc& desc() const { return m_desc; }
	const String& name() const { return m_name; }

	detail::ShaderUniformTypeDesc m_desc;
	String m_name;
	detail::ShaderParameterValue m_value;

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

LN_CONSTRUCT_ACCESS:	// TODO: 内部でしか new しないから private とかでいい気がする
	ShaderConstantBuffer();
	virtual ~ShaderConstantBuffer();
	void initialize(Shader* owner, detail::IShaderUniformBuffer* rhiObject);

private:
	Shader* owner() const { return m_owner; }
	detail::IShaderUniformBuffer* getRhiObject() const { return m_rhiObject; }
	void commit();

	Shader* m_owner;
	detail::IShaderUniformBuffer* m_rhiObject;
	String m_name;
	ByteBuffer m_buffer;

	friend class Shader;
	friend class ShaderPass;
};

class LN_API ShaderTechnique
	: public Object
{
public:

	const List<Ref<ShaderPass>>& passes() const { return m_passes; }

LN_CONSTRUCT_ACCESS:
	ShaderTechnique();
	virtual ~ShaderTechnique();
	void initialize();

private:
	void setOwner(Shader* owner) { m_owner = owner; }
	Shader* owner() const { return m_owner; }
	void addShaderPass(ShaderPass* pass);

	Shader* m_owner;
	List<Ref<ShaderPass>> m_passes;

	friend class Shader;
	friend class ShaderPass;
};

class LN_API ShaderPass
	: public Object
{
LN_CONSTRUCT_ACCESS:
	ShaderPass();
	virtual ~ShaderPass();
	void initialize(detail::IShaderPass* rhiPass);
	virtual void dispose() override;

private:
	void setOwner(ShaderTechnique* owner) { m_owner = owner; }
	void setupParameters();
	void commit();

	ShaderTechnique* m_owner;
	Ref<detail::IShaderPass> m_rhiPass;
	List<ShaderParameter*> m_parameters;
	List<ShaderConstantBuffer*> m_buffers;

	friend class Shader;
	friend class ShaderTechnique;
	friend class detail::ShaderValueSerializer;
	friend class GraphicsContext;
};



} // namespace ln
