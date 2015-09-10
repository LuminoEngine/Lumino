
#include "../Internal.h"
#include <Lumino/Scene/Light.h>

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// Light
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Light::Light(SceneGraphManager* manager, LightType type)
	: m_type(LightType_Directional)
	, m_diffuse(1.0f, 1.0f, 1.0f, 1.0f)
	, m_ambient(1.0f, 1.0f, 1.0f, 1.0f)
	, m_specular(1.0f, 1.0f, 1.0f, 1.0f)
	, m_enabled(true)
	, m_spotAngle(Math::PI * 0.25f)
	, m_viewMatrix(Matrix::Identity)
	, m_projMatrix(Matrix::Identity)
	, m_viewProjMatrix(Matrix::Identity)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Light::~Light()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Light::UpdateMatrices(const SizeF& viewSize)
{
	// 正面方向
	Vector3 direction = Vector3::TransformCoord(Vector3(0, 0, 1), m_combinedGlobalMatrix);

	// 注視点
	Vector3 lookAt = m_combinedGlobalMatrix.GetPosition() + direction;

	// ビュー行列
	Vector3 up = Vector3(0, 1, 0);
	m_viewMatrix = Matrix::LookAtLH(m_combinedGlobalMatrix.GetPosition(), lookAt, up);

	// プロジェクション行列の更新
	// TODO: 視野角とnear,far
	// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
	m_projMatrix = Matrix::PerspectiveFovLH(Math::PI / 4.0f, viewSize.Width / viewSize.Height, 0.01f, 1000.0f);

	m_viewProjMatrix = m_viewMatrix * m_projMatrix;

	m_direction = Vector4(direction, 0.0f);

	m_viewMatrixI = Matrix::Inverse(m_viewMatrix);
	m_projMatrixI = Matrix::Inverse(m_projMatrix);
	m_viewProjMatrixI = Matrix::Inverse(m_viewProjMatrix);
	m_viewMatrixT = Matrix::Transpose(m_viewMatrix);
	m_projMatrixT = Matrix::Transpose(m_projMatrix);
	m_viewProjMatrixT = Matrix::Transpose(m_viewProjMatrix);
	m_viewMatrixIT = Matrix::Transpose(m_viewMatrixI);
	m_projMatrixIT = Matrix::Transpose(m_projMatrixI);
	m_viewProjMatrixIT = Matrix::Transpose(m_viewProjMatrixI);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Light::UpdateViewFlustumHierarchy(Camera* camera, SceneNodeList* renderingNodeList, LightNodeList* renderingLightList)
{
	// ライトをリストアップ
	renderingLightList->Add(this);

	SceneNode::UpdateViewFlustumHierarchy(camera, renderingNodeList, renderingLightList);
}

LN_NAMESPACE_SCENE_END
} // namespace Lumino
