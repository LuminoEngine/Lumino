
#include "Internal.h"
#include "Device/ShaderVariableBase.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Mesh.h>

LN_NAMESPACE_BEGIN

class BoxMeshFactory
{
public:
	BoxMeshFactory(const Vector3& size)
	{
		m_size = size;
	}

	int GetVertexCount() const
	{
		return 8;
	}

	int GetIndexCount() const
	{
		return 36;
	}

	void Generate(Vector3* outPoints, uint16_t* outIndices)
	{
		Vector3 minPos = -(m_size / 2);
		Vector3 maxPos = (m_size / 2);

		// 手前 (Z-)
		outPoints[0].Set(minPos.x, maxPos.y, minPos.z);	// 左上
		outPoints[1].Set(minPos.x, minPos.y, minPos.z);	// 左下
		outPoints[2].Set(maxPos.x, maxPos.y, minPos.z);	// 右上
		outPoints[3].Set(maxPos.x, minPos.y, minPos.z);	// 右下
		// 奥 (Z+)
		outPoints[4].Set(minPos.x, maxPos.y, maxPos.z);	// 左上
		outPoints[5].Set(minPos.x, minPos.y, maxPos.z);	// 左下
		outPoints[6].Set(maxPos.x, maxPos.y, maxPos.z);	// 右上
		outPoints[7].Set(maxPos.x, minPos.y, maxPos.z);	// 右下

		uint16_t indices[] =
		{
			// near
			0, 1, 2,
			2, 1, 3,
			// far
			6, 7, 4,
			4, 7, 5,
			// left
			4, 5, 0,
			0, 5, 1,
			// right
			2, 3, 6,
			6, 3, 7,
			// top
			4, 0, 6,
			6, 0, 2,
			// bottom (Z- が面として上方向)
			1, 5, 3,
			3, 5, 7,
		};
		memcpy(outIndices, indices, sizeof(indices));
	}

private:
	Vector3	m_size;
};






//==============================================================================
// Material
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Material3, Object);

//------------------------------------------------------------------------------
MaterialPtr Material3::Create()
{
	auto ptr = MaterialPtr::MakeRef();
	return ptr;
}

//------------------------------------------------------------------------------
Material3::Material3()
	: m_shader(nullptr)
	, m_valueList()
{
}

//------------------------------------------------------------------------------
Material3::~Material3()
{
}

//------------------------------------------------------------------------------
void Material3::SetShader(Shader* shader)
{
}

//------------------------------------------------------------------------------
void Material3::SetIntParameter(const StringRef& name, int value)
{
	FindShaderValue(name)->SetInt(value);
}

//------------------------------------------------------------------------------
void Material3::SetFloatParameter(const StringRef& name, float value)
{
	FindShaderValue(name)->SetFloat(value);
}

//------------------------------------------------------------------------------
void Material3::SetVectorParameter(const StringRef& name, const Vector4& value)
{
	FindShaderValue(name)->SetVector(value);
}

//------------------------------------------------------------------------------
void Material3::SetMatrixParameter(const StringRef& name, const Matrix& value)
{
	FindShaderValue(name)->SetMatrix(value);
}

//------------------------------------------------------------------------------
void Material3::SetTextureParameter(const StringRef& name, Texture* value)
{
	FindShaderValue(name)->SetDeviceTexture((value) ? value->GetDeviceObject() : nullptr);
}

//------------------------------------------------------------------------------
ShaderValue* Material3::FindShaderValue(const StringRef& name)
{
	ShaderValuePtr v;
	if (!m_valueList.TryGetValue(name, &v))
	{
		v = std::make_shared<ShaderValue>();
		m_valueList.Add(name, v);
	}
	return v.get();
}

//==============================================================================
// StaticMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMeshModel, Object);

//------------------------------------------------------------------------------
StaticMeshModel::StaticMeshModel()
{
}

//------------------------------------------------------------------------------
StaticMeshModel::~StaticMeshModel()
{
}

//------------------------------------------------------------------------------
void StaticMeshModel::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
}

//------------------------------------------------------------------------------
void StaticMeshModel::CreateBox(const Vector3& size)
{
	BoxMeshFactory factory(size);


}

//------------------------------------------------------------------------------
void StaticMeshModel::Draw(GraphicsContext* g)
{

}

LN_NAMESPACE_END

