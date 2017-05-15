
#include "../Internal.h"
#include <Lumino/Scene/Light.h>
#include <Lumino/Scene/WorldObject.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// LightComponent
//==============================================================================

//------------------------------------------------------------------------------
LightComponent::LightComponent()
	: SceneNode()
	, m_lightInfo(false)
	, m_enabled(true)
	, m_spotAngle(Math::PI * 0.25f)
	, m_viewMatrix(Matrix::Identity)
	//, m_projMatrix(Matrix::Identity)
	//, m_viewProjMatrix(Matrix::Identity)
{
}

//------------------------------------------------------------------------------
LightComponent::~LightComponent()
{
}

//------------------------------------------------------------------------------
void LightComponent::Initialize(SceneGraph* owner, LightType type)
{
	SceneNode::Initialize(owner);
	m_lightInfo = RefPtr<detail::DynamicLightInfo>::MakeRef();
	m_lightInfo->m_type = type;
	m_lightInfo->m_diffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_ambient.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_specular.Set(1.0f, 1.0f, 1.0f, 1.0f);
	m_lightInfo->m_shadowZFar = 1000.0f;

	// Unity Default
	Quaternion rot;
	rot.RotateX(Math::DegreesToRadians(50));
	rot.RotateY(Math::DegreesToRadians(-30));
	SetRotation(rot);
}

//------------------------------------------------------------------------------
void LightComponent::UpdateMatrices(/*const Size& viewSize*/)
{
	const Matrix& worldMatrix = GetOwnerObject()->transform.GetWorldMatrix();

	// 正面方向
	Vector3 direction = Vector3::TransformCoord(Vector3(0, 0, 1), worldMatrix);

	// 注視点
	Vector3 lookAt = worldMatrix.GetPosition() + direction;

	// ビュー行列
	Vector3 up = Vector3(0, 1, 0);
	m_viewMatrix = Matrix::MakeLookAtLH(worldMatrix.GetPosition(), lookAt, up);

	// プロジェクション行列の更新
	// TODO: 視野角とnear,far
	// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
	//m_projMatrix = Matrix::MakePerspectiveFovLH(Math::PI / 4.0f, viewSize.width / viewSize.height, 0.01f, 1000.0f);

	//m_worldViewProjMatrix = GetMatrix() * m_viewMatrix * m_projMatrix;
	//m_viewProjMatrix = m_viewMatrix * m_projMatrix;

	m_lightInfo->m_direction = direction;

	//m_viewMatrixI = Matrix::MakeInverse(m_viewMatrix);
	//m_projMatrixI = Matrix::MakeInverse(m_projMatrix);
	//m_viewProjMatrixI = Matrix::MakeInverse(m_viewProjMatrix);
	//m_viewMatrixT = Matrix::MakeTranspose(m_viewMatrix);
	//m_projMatrixT = Matrix::MakeTranspose(m_projMatrix);
	//m_viewProjMatrixT = Matrix::MakeTranspose(m_viewProjMatrix);
	//m_viewMatrixIT = Matrix::MakeTranspose(m_viewMatrixI);
	//m_projMatrixIT = Matrix::MakeTranspose(m_projMatrixI);
	//m_viewProjMatrixIT = Matrix::MakeTranspose(m_viewProjMatrixI);
}

//------------------------------------------------------------------------------
void LightComponent::OnRender(DrawList* renderer)
{
	if (m_enabled)
	{
		UpdateMatrices();
		m_lightInfo->transform = GetOwnerObject()->transform.GetWorldMatrix();
		renderer->AddDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// Light
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Light, WorldObject);

//------------------------------------------------------------------------------
Light::Light()
	: WorldObject()
	, m_component(nullptr)
{
}

//------------------------------------------------------------------------------
Light::~Light()
{
}

//------------------------------------------------------------------------------
void Light::Initialize(SceneGraph* owner)
{
	WorldObject::Initialize();
	m_component = NewObject<LightComponent>(owner, LightType_Directional);
	AddComponent(m_component);
}

//------------------------------------------------------------------------------
LightComponent* Light::GetLightComponent() const
{
	return m_component;
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
