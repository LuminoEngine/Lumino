
#pragma once
#include "Common.hpp"
#include "../Graphics/GraphicsResource.hpp"

namespace ln {
class ShaderParameter;
class ShaderTechnique;
class ShaderPass;


class Texture
	: public GraphicsResource
{
};

namespace detail {
class IShaderPass;

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
}

enum class ShaderCodeType
{
	RawGLSL,
};

class LN_API Shader
	: public GraphicsResource
{
public:
	static Ref<Shader> create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath);

LN_CONSTRUCT_ACCESS:
	Shader();
	virtual ~Shader();
	void initialize();
	void initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType);

private:
	ShaderParameter* getShaderParameter(const detail::ShaderUniformTypeDesc& desc, const String& name);

	List<Ref<ShaderParameter>> m_parameters;
	List<Ref<ShaderTechnique>> m_techniques;

	friend class ShaderPass;
};


class LN_API ShaderParameter
	: public Object
{
LN_CONSTRUCT_ACCESS:
	ShaderParameter();
	virtual ~ShaderParameter();
	void initialize(const detail::ShaderUniformTypeDesc& desc, const String& name);

private:
	const detail::ShaderUniformTypeDesc& desc() const { return m_desc; }
	const String& name() const { return m_name; }

	detail::ShaderUniformTypeDesc m_desc;
	String m_name;
	detail::ShaderParameterValue m_value;

	friend class Shader;
};

class LN_API ShaderTechnique
	: public Object
{
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

private:
	void setOwner(ShaderTechnique* owner) { m_owner = owner; }
	void setupParameters();

	ShaderTechnique* m_owner;
	detail::IShaderPass* m_rhiPass;
	List<ShaderParameter*> m_parameters;

	friend class Shader;
	friend class ShaderTechnique;
};



} // namespace ln
