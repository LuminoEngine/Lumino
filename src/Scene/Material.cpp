
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Scene/Material.h>
#include <Lumino/Scene/SceneGraph.h>
#include "MME/MmdMaterial.h"	// TODO

LN_NAMESPACE_BEGIN

//==============================================================================
//// Material
////==============================================================================
//
////------------------------------------------------------------------------------
//Material2::Material2(int materialTypeId)
//	: m_materialTypeId(materialTypeId)
//	, m_opacity(1.0f)
//	, m_colorScale(1, 1, 1, 1)
//	, m_blendColor(0, 0, 0, 0)
//	, m_tone()
//	, m_texture(nullptr)
//	, m_shader(nullptr)
//{
//}
//
////------------------------------------------------------------------------------
//Material2::~Material2()
//{
//	LN_SAFE_RELEASE(m_texture);
//	LN_SAFE_RELEASE(m_shader);
//}
//
////------------------------------------------------------------------------------
//void Material2::SetTexture(Texture* texture)
//{
//	LN_REFOBJ_SET(m_texture, texture);
//}
//
////------------------------------------------------------------------------------
//void Material2::SetShader(Shader* shader)
//{
//	LN_REFOBJ_SET(m_shader, shader);
//}

namespace detail
{

MaterialInstance::MaterialInstance(int materialTypeId)
	: m_materialTypeId(materialTypeId)
{

}

MaterialInstance::~MaterialInstance()
{

}

void MaterialInstance::Combine(Material3* owner, Material3* parent)
{
	if (m_owner == nullptr || owner != m_owner || m_owner->m_modifiedForMaterialInstance)
	{
		// set
		m_owner = owner;
		OnCombine(owner, parent);
		m_owner->m_modifiedForMaterialInstance = false;
	}
}

}

//==============================================================================
// MaterialList2
//==============================================================================

//------------------------------------------------------------------------------
MaterialList2::MaterialList2()
{
}

//------------------------------------------------------------------------------
MaterialList2::~MaterialList2()
{
}

//------------------------------------------------------------------------------
void MaterialList2::Initialize(int subMaterialCount, bool createMainMaterial)
{
	Resize(subMaterialCount);
	for (int i = 0; i < subMaterialCount; ++i)
	{
		auto m = RefPtr<Material3>::MakeRef();	// TODO
		SetAt(i, m);
	}

	if (createMainMaterial)
	{
		m_mainMaterial = RefPtr<Material3>::MakeRef();	// TODO
	}
}

//------------------------------------------------------------------------------
void MaterialList2::CopyShared(MaterialList3* srcList)
{
	LN_CHECK_ARG(srcList != nullptr);

	Resize(srcList->GetCount());
	for (int i = 0; i < srcList->GetCount(); ++i)
	{
		SetAt(i, srcList->GetAt(i));
	}
}

//------------------------------------------------------------------------------
void MaterialList2::UpdateMaterialInstances(SceneGraph* sceneGraph)
{
	// m_mainMaterial は親として使える？
	Material3* parent = nullptr;
	if (m_mainMaterial != nullptr)
	{
		parent = m_mainMaterial;
	}

	// m_instanceList のサイズをそろえる
	int subCount = GetCount();
	if (m_instanceList.GetCount() != subCount)
	{
		int oldCount = m_instanceList.GetCount();
		int d = subCount - oldCount;
		m_instanceList.Resize(subCount);
		if (d > 0)
		{
			for (int i = 0; i < d; ++i)
			{
				m_instanceList[oldCount + i] = RefPtr<detail::MaterialInstance>(sceneGraph->CreateMaterialInstance(), false);
			}
		}
	}

	// m_instanceList の内容を作っていく
	if (subCount > 0)
	{
		for (int i = 0; i < subCount; ++i)
		{
			m_instanceList[i]->Combine(GetAt(i), parent);
		}
	}
	else if (parent != nullptr)
	{
		// parent はあるけど SubMaterial が1つも無い。parent を使う。
		if (m_instanceList.GetCount() != 1) m_instanceList.Resize(1);
		m_instanceList[0]->Combine(parent, nullptr);
	}
	else
	{
		// parent も SubMaterial も無い。デフォルトのを使う。
		if (m_instanceList.GetCount() != 1) m_instanceList.Resize(1);
		LN_NOTIMPLEMENTED();
		//m_instanceList[0].Combine(parent, nullptr);
	}
}

LN_NAMESPACE_END
