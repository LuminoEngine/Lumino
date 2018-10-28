
#include "Internal.hpp"
#include <LuminoEngine/Visual/LightComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
//#include "../Rendering/ClusteredShadingSceneRenderer.h"

namespace ln {

#if 0
//==============================================================================
// LightComponent

//------------------------------------------------------------------------------
LightComponent::LightComponent()
	: VisualComponent()
	, m_lightInfo(nullptr)
	, m_enabled(true)
	, m_spotAngle(Math::PI * 0.25f)
	//, m_viewMatrix(Matrix::Identity)
	//, m_projMatrix(Matrix::Identity)
	//, m_viewProjMatrix(Matrix::Identity)
{
}

//------------------------------------------------------------------------------
LightComponent::~LightComponent()
{
}

//------------------------------------------------------------------------------
void LightComponent::initialize(LightType type)
{
    VisualComponent::initialize();
	m_lightInfo = makeRef<detail::DynamicLightInfo>();
	//m_lightInfo->m_type = type;
	//m_lightInfo->m_color.set(1.0f, 1.0f, 1.0f, 1.0f);
	//m_lightInfo->m_ambient.set(1.0f, 1.0f, 1.0f, 1.0f);
	//m_lightInfo->m_specular.set(1.0f, 1.0f, 1.0f, 1.0f);
	//m_lightInfo->m_shadowZFar = 1000.0f;
}

//------------------------------------------------------------------------------
//void LightComponent::updateMatrices(/*const Size& viewSize*/)
//{
//	const Matrix& worldMatrix = getOwnerObject()->transform.getWorldMatrix();
//
//	// 正面方向
//	Vector3 direction = Vector3::transformCoord(Vector3(0, 0, 1), worldMatrix);
//
//	// 注視点
//	Vector3 lookAt = worldMatrix.getPosition() + direction;
//
//	// ビュー行列
//	Vector3 up = Vector3(0, 1, 0);
//	//m_viewMatrix = Matrix::makeLookAtLH(worldMatrix.getPosition(), lookAt, up);
//
//	// プロジェクション行列の更新
//	// TODO: 視野角とnear,far
//	// https://sites.google.com/site/mmereference/home/Annotations-and-Semantics-of-the-parameter/2-1-geometry-translation
//	//m_projMatrix = Matrix::makePerspectiveFovLH(Math::PI / 4.0f, viewSize.width / viewSize.height, 0.01f, 1000.0f);
//
//	//m_worldViewProjMatrix = getMatrix() * m_viewMatrix * m_projMatrix;
//	//m_viewProjMatrix = m_viewMatrix * m_projMatrix;
//
//	m_lightInfo->m_direction = direction;
//
//	//m_viewMatrixI = Matrix::makeInverse(m_viewMatrix);
//	//m_projMatrixI = Matrix::makeInverse(m_projMatrix);
//	//m_viewProjMatrixI = Matrix::makeInverse(m_viewProjMatrix);
//	//m_viewMatrixT = Matrix::makeTranspose(m_viewMatrix);
//	//m_projMatrixT = Matrix::makeTranspose(m_projMatrix);
//	//m_viewProjMatrixT = Matrix::makeTranspose(m_viewProjMatrix);
//	//m_viewMatrixIT = Matrix::makeTranspose(m_viewMatrixI);
//	//m_projMatrixIT = Matrix::makeTranspose(m_projMatrixI);
//	//m_viewProjMatrixIT = Matrix::makeTranspose(m_viewProjMatrixI);
//}
//
//------------------------------------------------------------------------------
void LightComponent::onPreRender(DrawList* context)
{
	if (m_enabled)
	{
		//updateMatrices();
		const Matrix& t = getOwnerObject()->transform.getWorldMatrix();
		m_lightInfo->m_position = t.getPosition();
		m_lightInfo->m_direction = t.getFront();
		context->addDynamicLightInfo(m_lightInfo);
	}
}
#endif

//==============================================================================
// AmbientLightComponent

AmbientLightComponent::AmbientLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

AmbientLightComponent::~AmbientLightComponent()
{
}

void AmbientLightComponent::initialize()
{
	VisualComponent::initialize();
	m_lightInfo = makeRef<detail::DynamicLightInfo>();
	m_lightInfo->m_type = LightType::Ambient;
    m_lightInfo->m_intensity = 0.5f;
}

void AmbientLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
		context->addDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// HemisphereLightComponent

HemisphereLightComponent::HemisphereLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

HemisphereLightComponent::~HemisphereLightComponent()
{
}

void HemisphereLightComponent::initialize()
{
	VisualComponent::initialize();
	m_lightInfo = makeRef<detail::DynamicLightInfo>();
	m_lightInfo->m_type = LightType::Hemisphere;
    m_lightInfo->m_intensity = 0.5f;
}

void HemisphereLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
		context->addDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// DirectionalLightComponent

DirectionalLightComponent::DirectionalLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

DirectionalLightComponent::~DirectionalLightComponent()
{
}

void DirectionalLightComponent::initialize()
{
	VisualComponent::initialize();
	m_lightInfo = makeRef<detail::DynamicLightInfo>();
	m_lightInfo->m_type = LightType::Directional;
    m_lightInfo->m_intensity = 0.5f;
}

void DirectionalLightComponent::setShadowCast(bool enabled)
{
    LN_NOTIMPLEMENTED();
	//if (enabled)
	//{
	//	m_shadowCasterPass = newObject<detail::ShadowCasterPass>();
	//}
	//else
	//{
	//	m_shadowCasterPass.safeRelease();
	//}
}

bool DirectionalLightComponent::isShadowCast() const
{
	return m_shadowCasterPass != nullptr;
}

void DirectionalLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
        const Matrix& t = transrom()->worldMatrix();
		m_lightInfo->m_position = t.position();
		m_lightInfo->m_direction = t.front();
		m_lightInfo->m_shadowCasterPass = m_shadowCasterPass;
		context->addDynamicLightInfo(m_lightInfo);

		if (m_shadowCasterPass != nullptr)
		{
			m_shadowCasterPass->view.makePerspective(
				m_lightInfo->m_position, m_lightInfo->m_direction,
				Math::PI / 2.0f,
				//m_shadowCasterPass->m_shadowMap->getSize().toFloatSize(),
				Size(1024.0/8, 1024.0 / 8),	// TODO: LightMapSize
				0.5f, 100.0f);	// TODO: clip range
		}
	}
}

//==============================================================================
// PointLightComponent

PointLightComponent::PointLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

PointLightComponent::~PointLightComponent()
{
}

void PointLightComponent::initialize()
{
	VisualComponent::initialize();
	m_lightInfo = makeRef<detail::DynamicLightInfo>();
	m_lightInfo->m_type = LightType::Point;
	m_lightInfo->m_intensity = 1.0f;
}

void PointLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
        const Matrix& t = transrom()->worldMatrix();
        m_lightInfo->m_position = t.position();
        m_lightInfo->m_direction = t.front();
		context->addDynamicLightInfo(m_lightInfo);
	}
}

//==============================================================================
// SpotLightComponent

SpotLightComponent::SpotLightComponent()
	: m_lightInfo(nullptr)
	, m_enabled(true)
{
}

SpotLightComponent::~SpotLightComponent()
{
}

void SpotLightComponent::initialize()
{
	VisualComponent::initialize();
	m_lightInfo = makeRef<detail::DynamicLightInfo>();
	m_lightInfo->m_type = LightType::Spot;
	m_lightInfo->m_intensity = 1.0f;
}

void SpotLightComponent::onPrepareRender(RenderingContext* context)
{
	if (m_enabled)
	{
        const Matrix& t = transrom()->worldMatrix();
        m_lightInfo->m_position = t.position();
        m_lightInfo->m_direction = t.front();
		context->addDynamicLightInfo(m_lightInfo);
	}
}

} // namespace ln
