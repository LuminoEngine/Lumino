
#pragma once
#include <Lumino/Graphics/Common.h>
#include "Vertex.h"

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

LN_INTERNAL_ACCESS:
	using ShaderValuePtr = std::shared_ptr<ShaderValue>;

	struct ValuePair
	{
		ShaderVariable*	variable;
		ShaderValuePtr	value;
	};

	const Array<ValuePair>& GetLinkedVariableList() { return m_linkedVariableList; }

private:
	void LinkVariables();
	ShaderValue* FindShaderValue(const StringRef& name);

	RefPtr<Shader>						m_shader;
	SortedArray<String, ShaderValuePtr>	m_valueList;
	Array<ValuePair>					m_linkedVariableList;
};

/**
	@brief
*/
class StaticMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

LN_INTERNAL_ACCESS:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(GraphicsManager* manager);
	void CreateBox(const Vector3& size);

	void Draw(GraphicsContext* g);

	 
	GraphicsManager*			m_manager;
	RefPtr<VertexDeclaration>	m_vertexDeclaration;
	RefPtr<VertexBuffer>		m_vertexBuffer;
	RefPtr<IndexBuffer>			m_indexBuffer;
	int							m_triangleCount;
};

LN_NAMESPACE_END
