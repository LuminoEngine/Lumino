
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Scene/Camera.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/VisualNode.h>
#include <Lumino/Scene/SceneGraph.h>
#include "../EngineDiagCore.h"
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

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
		auto m = RefPtr<Material>::MakeRef();	// TODO
		SetAt(i, m);
	}

	if (createMainMaterial)
	{
		m_mainMaterial = RefPtr<Material>::MakeRef();	// TODO
	}
}

//------------------------------------------------------------------------------
void MaterialList2::CopyShared(MaterialList* srcList, bool createMainMaterial)
{
	if (LN_CHECK_ARG(srcList != nullptr)) return;

	Resize(srcList->GetCount());
	for (int i = 0; i < srcList->GetCount(); ++i)
	{
		SetAt(i, srcList->GetAt(i));
	}


	if (createMainMaterial)
	{
		m_mainMaterial = RefPtr<Material>::MakeRef();
	}
	else if (GetCount() > 0)
	{
		m_mainMaterial = GetAt(0);
	}
}

//------------------------------------------------------------------------------
Material* MaterialList2::GetMainMaterial() const
{
	return m_mainMaterial;
}


//==============================================================================
// VisualComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(VisualComponent, SceneNode);

//------------------------------------------------------------------------------
VisualComponent::VisualComponent()
	: m_blendMode(BlendMode::Normal)
	, m_cullingMode(CullingMode::Back)
	, m_depthTestEnabled(true)
	, m_depthWriteEnabled(true)
{
}

//------------------------------------------------------------------------------
VisualComponent::~VisualComponent()
{
}

//------------------------------------------------------------------------------
void VisualComponent::Initialize(SceneGraph* owner/*, int subsetCount*/)
{
	SceneNode::Initialize(owner);
	//m_materialList = RefPtr<MaterialList2>::MakeRef();
	//m_materialList->Initialize(subsetCount, true);	// TODO: 今はとりあえず必ず mainMaterial 有り (メモリ効率悪い)
}

////------------------------------------------------------------------------------
//Material* VisualComponent::GetMainMaterial() const
//{
//	return m_materialList->GetMainMaterial();
//}
//
////------------------------------------------------------------------------------
//tr::ReflectionObjectList<Material*>* VisualComponent::GetMaterials() const
//{
//	return m_materialList;
//}

//------------------------------------------------------------------------------
void VisualComponent::SetOpacity(float value)
{
	m_builtinEffectData.SetOpacity(value);
}

//------------------------------------------------------------------------------
float VisualComponent::GetOpacity() const
{
	return m_builtinEffectData.GetOpacity();
}

//------------------------------------------------------------------------------
void VisualComponent::SetColorScale(const Color& value)
{
	m_builtinEffectData.SetColorScale(value);
}

//------------------------------------------------------------------------------
const Color& VisualComponent::GetColorScale() const
{
	return m_builtinEffectData.GetColorScale();
}

//------------------------------------------------------------------------------
void VisualComponent::SetBlendColor(const Color& value)
{
	m_builtinEffectData.SetBlendColor(value);
}

//------------------------------------------------------------------------------
const Color& VisualComponent::GetBlendColor() const
{
	return m_builtinEffectData.GetBlendColor();
}

//------------------------------------------------------------------------------
void VisualComponent::SetTone(const ToneF& value)
{
	m_builtinEffectData.SetTone(value);
}

//------------------------------------------------------------------------------
const ToneF& VisualComponent::GetTone() const
{
	return m_builtinEffectData.GetTone();
}

//------------------------------------------------------------------------------
void VisualComponent::SetShader(Shader* value)
{
	m_builtinEffectData.SetShader(value);
}

//------------------------------------------------------------------------------
Shader* VisualComponent::GetShader() const
{
	return m_builtinEffectData.GetShader();
}

////------------------------------------------------------------------------------
//void VisualComponent::SetOpacity(float value, int subsetIndex)
//{
//	if (LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr)) return;
//	// TODO: サブマテリアルの設定
//	//m_materialList->GetMainMaterial()->SetOpacity(opacity);
//	m_builtinEffectData.SetOpacity(value);
//}
//float VisualComponent::GetOpacity() const
//{
//	return m_builtinEffectData.GetOpacity();
//}
//
//void VisualComponent::SetColorScale(const Color& value, int subsetIndex)
//{
//	if (LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr)) return;
//	// TODO: サブマテリアルの設定
//	//m_materialList->GetMainMaterial()->SetColorScale(color);
//	m_builtinEffectData.SetColorScale(value);
//}
//void VisualComponent::SetColorScale(float r, float g, float b, float a, int subsetIndex)
//{
//	SetColorScale(Color(r, g, b, a));
//}
//void VisualComponent::SetColor(const Color32& color)
//{
//	SetColorScale(Color(color));
//}
//void VisualComponent::SetColor(int r, int g, int b, int a)
//{
//	SetColor(Color32(r, g, b, a));
//}
//const Color& VisualComponent::GetColorScale() const
//{
//	return m_builtinEffectData.GetColorScale();
//}
//void VisualComponent::SetBlendColor(const Color& color, int subsetIndex)
//{
//	if (LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr)) return;
//	// TODO: サブマテリアルの設定
//	//m_materialList->GetMainMaterial()->SetBlendColor(color);
//	m_builtinEffectData.SetBlendColor(color);
//}
//void VisualComponent::SetTone(const ToneF& tone, int subsetIndex)
//{
//	if (LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr)) return;
//	// TODO: サブマテリアルの設定
//	//m_materialList->GetMainMaterial()->SetTone(tone);
//	m_builtinEffectData.SetTone(tone);
//}
//void VisualComponent::SetShader(Shader* value, int subsetIndex)
//{
//	if (LN_CHECK_STATE(m_materialList->GetMainMaterial() != nullptr)) return;
//	// TODO: サブマテリアルの設定
//	//m_materialList->GetMainMaterial()->SetShader(shader);
//	m_builtinEffectData.SetShader(value);
//}

//------------------------------------------------------------------------------
void VisualComponent::SetBlendMode(BlendMode mode) { m_blendMode = mode; }

//------------------------------------------------------------------------------
//BlendMode VisualComponent::GetBlendMode() const { return GetMainMaterial()->GetBlendMode(); }

//------------------------------------------------------------------------------
void VisualComponent::SetCullingMode(CullingMode mode) { m_cullingMode = mode; }

//------------------------------------------------------------------------------
//CullingMode VisualComponent::GetCullingMode() const { return GetMainMaterial()->GetCullingMode(); }

//------------------------------------------------------------------------------
void VisualComponent::SetDepthTestEnabled(bool enabled) { m_depthTestEnabled = enabled; }

//------------------------------------------------------------------------------
//bool VisualComponent::IsDepthTestEnabled() const { return GetMainMaterial()->IsDepthTestEnabled(); }

//------------------------------------------------------------------------------
void VisualComponent::SetDepthWriteEnabled(bool enabled) { m_depthWriteEnabled = enabled; }

//------------------------------------------------------------------------------
//bool VisualComponent::IsDepthWriteEnabled() const { return GetMainMaterial()->IsDepthWriteEnabled(); }

//------------------------------------------------------------------------------
void VisualComponent::UpdateFrameHierarchy(SceneNode* parent, float deltaTime)
{
	// TODO: 描画関係のデータは UpdateFrame でやるべきではないような気もする。
	//m_materialList->UpdateMaterialInstances(m_ownerSceneGraph);

	SceneNode::UpdateFrameHierarchy(parent, deltaTime);
}

//------------------------------------------------------------------------------
detail::Sphere VisualComponent::GetBoundingSphere()
{
	const detail::Sphere s{ Vector3::Zero, -1 };
	return s;
}

////------------------------------------------------------------------------------
//Shader* VisualComponent::GetPrimaryShader() const
//{
//	// TODO: main が無ければ [0] のをつかう
//	return m_materialList->GetMainMaterial()->GetShader();
//}

//------------------------------------------------------------------------------
void VisualComponent::Render(DrawList* context)
{
	if (m_isVisible)
	{
		context->SetBlendMode(m_blendMode);
		context->SetCullingMode(m_cullingMode);
		context->SetDepthTestEnabled(m_depthTestEnabled);
		context->SetDepthWriteEnabled(m_depthWriteEnabled);
		context->SetBuiltinEffectData(m_builtinEffectData);
		OnRender(context);
		OnRender2(context);
	}
}



//==============================================================================
// VisualObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(VisualObject, WorldObject);

//------------------------------------------------------------------------------
VisualObject::VisualObject()
{
}

//------------------------------------------------------------------------------
VisualObject::~VisualObject()
{
}

//------------------------------------------------------------------------------
void VisualObject::Initialize()
{
	WorldObject::Initialize();
}

//------------------------------------------------------------------------------
void VisualObject::SetOpacity(float value)
{
	GetMainVisualComponent()->SetOpacity(value);
}

//------------------------------------------------------------------------------
float VisualObject::GetOpacity() const
{
	return GetMainVisualComponent()->GetOpacity();
}

//------------------------------------------------------------------------------
void VisualObject::SetColorScale(const Color& value)
{
	GetMainVisualComponent()->SetColorScale(value);
}

//------------------------------------------------------------------------------
void VisualObject::SetColorScale(float r, float g, float b, float a)
{
	SetColorScale(Color(r, g, b, a));
}

//------------------------------------------------------------------------------
const Color& VisualObject::GetColorScale() const
{
	return GetMainVisualComponent()->GetColorScale();
}

//------------------------------------------------------------------------------
void VisualObject::SetBlendColor(const Color& value)
{
	GetMainVisualComponent()->SetBlendColor(value);
}

//------------------------------------------------------------------------------
const Color& VisualObject::GetBlendColor() const
{
	return GetMainVisualComponent()->GetBlendColor();
}

//------------------------------------------------------------------------------
void VisualObject::SetTone(const ToneF& value)
{
	GetMainVisualComponent()->SetTone(value);
}

//------------------------------------------------------------------------------
const ToneF& VisualObject::GetTone() const
{
	return GetMainVisualComponent()->GetTone();
}

//------------------------------------------------------------------------------
void VisualObject::SetShader(Shader* value)
{
	GetMainVisualComponent()->SetShader(value);
}

//------------------------------------------------------------------------------
Shader* VisualObject::GetShader() const
{
	return GetMainVisualComponent()->GetShader();
}

//------------------------------------------------------------------------------
void VisualObject::SetBlendMode(BlendMode mode)
{
	GetMainVisualComponent()->SetBlendMode(mode);
}

//------------------------------------------------------------------------------
BlendMode VisualObject::GetBlendMode() const
{
	return GetMainVisualComponent()->GetBlendMode();
}

//------------------------------------------------------------------------------
void VisualObject::SetCullingMode(CullingMode mode)
{
	GetMainVisualComponent()->SetCullingMode(mode);
}

//------------------------------------------------------------------------------
CullingMode VisualObject::GetCullingMode() const
{
	return GetMainVisualComponent()->GetCullingMode();
}

//------------------------------------------------------------------------------
void VisualObject::SetDepthTestEnabled(bool enabled)
{
	GetMainVisualComponent()->SetDepthTestEnabled(enabled);
}

//------------------------------------------------------------------------------
bool VisualObject::IsDepthTestEnabled() const
{
	return GetMainVisualComponent()->IsDepthTestEnabled();
}

//------------------------------------------------------------------------------
void VisualObject::SetDepthWriteEnabled(bool enabled)
{
	GetMainVisualComponent()->SetDepthWriteEnabled(enabled);
}

//------------------------------------------------------------------------------
bool VisualObject::IsDepthWriteEnabled() const
{
	return GetMainVisualComponent()->IsDepthWriteEnabled();
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
