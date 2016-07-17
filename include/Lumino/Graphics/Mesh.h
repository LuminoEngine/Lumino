
#pragma once
#include <Lumino/Graphics/Common.h>

LN_NAMESPACE_BEGIN
class VertexDeclaration;
class StaticMeshModel;
class Material3;
using StaticMeshModelPtr = RefPtr<StaticMeshModel>;
using MaterialPtr = RefPtr<Material3>;


/**
	@brief
*/
class Material3	// TODO: Scene の Material のベースクラスにしたい
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	static MaterialPtr Create();

public:
	void SetShader(Shader* shader);
	Shader* GetShader() const { return m_shader; }
	
	void SetIntParameter(const StringRef& name, int value);
	void SetFloatParameter(const StringRef& name, float value);
	void SetVectorParameter(const StringRef& name, const Vector4& value);
	void SetMatrixParameter(const StringRef& name, const Matrix& value);
	void SetTextureParameter(const StringRef& name, Texture* value);
	
protected:
	Material3();
	virtual ~Material3();

private:
	ShaderValue* FindShaderValue(const StringRef& name);

	using ShaderValuePtr = std::shared_ptr<ShaderValue>;
	Shader*								m_shader;
	SortedArray<String, ShaderValuePtr>	m_valueList;
};

/**
	@brief
*/
class StaticMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

private:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(GraphicsManager* manager);

LN_INTERNAL_ACCESS:
	void Draw(GraphicsContext* g);

	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	RefPtr<VertexBuffer>		m_positionVertexBuffer;
	RefPtr<VertexBuffer>		m_textureUVVertexBuffer;
	RefPtr<VertexBuffer>		m_colorVertexBuffer;
	RefPtr<IndexBuffer>			m_indexBuffer;
};

LN_NAMESPACE_END
