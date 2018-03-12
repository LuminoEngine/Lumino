
#include "../Internal.h"
#include "PmxSkinnedMesh.h"

LN_NAMESPACE_BEGIN


//==============================================================================
// PmxMaterialResource
//==============================================================================

//------------------------------------------------------------------------------
Ref<CommonMaterial> PmxMaterialResource::MakeCommonMaterial() const
{
	auto m = Ref<PhongMaterial>::makeRef();
	m->setBuiltinColorParameter(
		CommonMaterial::DiffuseParameter,
		Diffuse.r,
		Diffuse.g,
		Diffuse.b,
		Diffuse.a);
	m->setBuiltinColorParameter(
		CommonMaterial::AmbientParameter,
		Ambient.r,
		Ambient.g,
		Ambient.b,
		Ambient.a);
	m->setBuiltinColorParameter(
		CommonMaterial::SpecularParameter,
		Specular.r,
		Specular.g,
		Specular.b,
		Specular.a);
	m->setBuiltinColorParameter(
		CommonMaterial::EmissiveParameter,
		Emissive.r,
		Emissive.g,
		Emissive.b,
		Emissive.a);
	m->setBuiltinFloatParameter(
		CommonMaterial::PowerParameter,
		Power);
	m->setBuiltinTextureParameter(
		CommonMaterial::MaterialTextureParameter,
		Texture);

	//detail::PhongMaterialData ddd;
	//m->translateToPhongMaterialData(&ddd);
	return m;
}

//==============================================================================
// PmxBoneResource
//==============================================================================

//------------------------------------------------------------------------------
PmxBoneResource::PmxBoneResource(PmxSkinnedMeshResource* owner, int boneIndex)
	: m_owner(owner)
	, m_boneIndex(boneIndex)
	, Name()
	//String			EnglishName;				///< ボーン英名
	, OrgPosition()
	, ParentBoneIndex(-1)
	, TransformLevel(0)
	, BoneConnect(BoneConnectType_PositionOffset)
	, CanRotate(false)
	, CanMove(false)
	, isVisible(false)
	, CanOperate(false)
	, IsIK(false)
	, LocalProvide(LocalProvideType_UserTransformValue)
	, IsRotateProvided(false)
	, IsMoveProvided(false)
	, IsFixAxis(false)
	, IsLocalAxis(false)
	, TransformAfterPhysics(false)
	, ParentTransform(false)
	, PositionOffset()
	, ConnectedBoneIndex(0)
	, ProvidedParentBoneIndex(0)
	, ProvidedRatio(0)
	, AxisDirectionVector()
	, DimentionXDirectionVector()
	, DimentionZDirectionVector()
	, KeyValue(0)
{
}

//------------------------------------------------------------------------------
void PmxBoneResource::refreshInitialValues()
{
	if (ParentBoneIndex >= 0)
	{
		m_offsetFromParent = OrgPosition - m_owner->bones[ParentBoneIndex]->OrgPosition;
	}
	else
	{
		m_offsetFromParent = OrgPosition;	// モデル原点からのオフセット
	}
		
	m_initialTranstormInv = Matrix::makeTranslation(-OrgPosition);
}

//==============================================================================
// PmxMorphBaseResource
//==============================================================================

void PmxMorphBaseResource::addVertex(int vertexIndex, const Vector3& pos, const Vector4& uv)
{
	auto itr = m_vertexMap.find(vertexIndex);
	if (itr == m_vertexMap.end())
	{
		m_vertexMap[vertexIndex] = BaseVertex{ pos, uv };
	}
}

void PmxMorphBaseResource::apply(MeshResource* mesh)
{
	// TODO: vector に直しておいたほうが 8 倍くらい早いこともある
	for (auto& v : m_vertexMap)
	{
		mesh->setPosition(v.first, v.second.pos);
		mesh->setUV(v.first, v.second.uv.getXY());
	}
}

//==============================================================================
// ModelCore
//==============================================================================

//------------------------------------------------------------------------------
PmxSkinnedMeshResource::~PmxSkinnedMeshResource()
{
}

//------------------------------------------------------------------------------
void PmxSkinnedMeshResource::refreshInitialValues()
{
	for (Ref<PmxBoneResource>& b : bones)
	{
		b->refreshInitialValues();
	}
}

LN_NAMESPACE_END
