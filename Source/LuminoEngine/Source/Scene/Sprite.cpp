
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>	// TODO: いらない
#include <Lumino/Graphics/Rendering.h>
#include "SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Sprite.h>
#include "../Graphics/PrimitiveRenderer.h"	// todo

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// SpriteComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteComponent, VisualComponent);

//------------------------------------------------------------------------------
SpriteComponent::SpriteComponent()
	: VisualComponent()
	, m_size()
	, m_srcRect()
	, m_flipMode(FlipMode_None)
{
}

//------------------------------------------------------------------------------
SpriteComponent::~SpriteComponent()
{
}

//------------------------------------------------------------------------------
void SpriteComponent::Initialize(SceneGraph* owner)
{
	if (LN_CHECK_ARG(owner != nullptr)) return;

	VisualComponent::Initialize(owner, 1);
	m_srcRect.Set(0, 0, -1, -1);
	SetSize(Size(-1, -1));

	SetBlendMode(BlendMode::Alpha);

	owner->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void SpriteComponent::SetTexture(Texture* texture)
{
	if (LN_CHECK_ARG(m_materialList != nullptr)) return;
	m_materialList->GetAt(0)->SetMaterialTexture(texture);
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void SpriteComponent::SetSize(const Size& size)
{
	m_size = size;
}

//------------------------------------------------------------------------------
Texture* SpriteComponent::GetTexture() const
{
	if (LN_CHECK_ARG(m_materialList != nullptr)) return nullptr;
	return m_materialList->GetAt(0)->GetMaterialTexture(nullptr);
}

//------------------------------------------------------------------------------
void SpriteComponent::SetTextureRect(const RectF& rect)
{
	m_srcRect = rect;
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void SpriteComponent::SetTextureRect(float x, float y, float width, float height)
{
	SetTextureRect(RectF(x, y, width, height));
}

//------------------------------------------------------------------------------
void SpriteComponent::SetAnchorPoint(const Vector2& ratio)
{
	m_anchor = ratio;
}

//------------------------------------------------------------------------------
void SpriteComponent::SetAnchorPoint(float ratioX, float ratioY)
{
	m_anchor.Set(ratioX, ratioY);
}

//------------------------------------------------------------------------------
void SpriteComponent::RenderSprite(DrawList* renderer, SpriteBaseDirection dir)
{
	Material* mat = GetMainMaterial();
	Color colorScale = GetColorScale();
	colorScale.a *= GetOpacity();
	renderer->SetTransform(m_combinedGlobalMatrix);
	renderer->DrawSprite(Vector3::Zero, m_renderSize, m_anchor, GetTexture(), m_renderSourceRect, colorScale, dir, GetMainMaterial());
}

//------------------------------------------------------------------------------
void SpriteComponent::UpdateVertexData()
{
	// 転送元矩形が負値ならテクスチャ全体を転送する
	Texture* tex = GetTexture();
	const SizeI& texSize = (tex != nullptr) ? tex->GetSize() : SizeI::Zero;
	m_renderSourceRect = m_srcRect;
	if (m_renderSourceRect.width < 0 && m_renderSourceRect.height < 0)
	{
		m_renderSourceRect.width = texSize.width;
		m_renderSourceRect.height = texSize.height;
	}
	m_renderSize = m_size;
	if (m_renderSize.width < 0 && m_renderSize.height < 0)
	{
		m_renderSize.width = m_renderSourceRect.width;
		m_renderSize.height = m_renderSourceRect.height;
	}
}


//==============================================================================
// Sprite2DComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite2DComponent, SpriteComponent);

//------------------------------------------------------------------------------
Sprite2DComponentPtr Sprite2DComponent::Create()
{
	auto obj = Sprite2DComponentPtr::MakeRef();
	obj->Initialize();
	return obj;
}

//------------------------------------------------------------------------------
Sprite2DComponentPtr Sprite2DComponent::Create(const StringRef& filePath)
{
	auto tex = Texture2D::Create(filePath);	// TODO: from asset
	return Create(tex);
}

//------------------------------------------------------------------------------
Sprite2DComponentPtr Sprite2DComponent::Create(Texture* texture)
{
	auto obj = Create();
	obj->SetTexture(texture);
	return obj;
}

//------------------------------------------------------------------------------
Sprite2DComponent::Sprite2DComponent()
{
}

//------------------------------------------------------------------------------
Sprite2DComponent::~Sprite2DComponent()
{
}

//------------------------------------------------------------------------------
void Sprite2DComponent::Initialize()
{
	SpriteComponent::Initialize(detail::EngineDomain::GetDefaultSceneGraph2D());
}

//------------------------------------------------------------------------------
void Sprite2DComponent::Initialize(Texture* texture)
{
	Initialize();
	SetTexture(texture);
}

//------------------------------------------------------------------------------
void Sprite2DComponent::OnRender2(DrawList* renderer)
{
	RenderSprite(renderer, SpriteBaseDirection::Basic2D);
}


//==============================================================================
// Sprite3DComponent
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite3DComponent, SpriteComponent);

//------------------------------------------------------------------------------
Sprite3DComponentPtr Sprite3DComponent::Create()
{
	auto obj = Sprite3DComponentPtr::MakeRef();
	obj->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D());
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DComponentPtr Sprite3DComponent::Create(float width, float height)
{
	auto obj = Sprite3DComponentPtr::MakeRef();
	obj->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D());
	obj->SetSize(Size(width, height));
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DComponentPtr Sprite3DComponent::Create(float width, float height, Texture* texture)
{
	auto obj = Sprite3DComponentPtr::MakeRef();
	obj->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D());
	obj->SetSize(Size(width, height));
	obj->SetTexture(texture);
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DComponent::Sprite3DComponent()
{
}

//------------------------------------------------------------------------------
Sprite3DComponent::~Sprite3DComponent()
{
}

//------------------------------------------------------------------------------
void Sprite3DComponent::Initialize(SceneGraph* owner)
{
	SpriteComponent::Initialize(owner);
}

//------------------------------------------------------------------------------
void Sprite3DComponent::OnRender2(DrawList* renderer)
{
	RenderSprite(renderer, SpriteBaseDirection::ZMinus);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
