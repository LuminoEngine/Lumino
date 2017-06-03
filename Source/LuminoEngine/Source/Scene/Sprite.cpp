
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>	// TODO: いらない
#include <Lumino/Graphics/Rendering.h>
#include "SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Sprite.h>
#include <Lumino/World.h>
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
	, m_texture(nullptr)
{
}

//------------------------------------------------------------------------------
SpriteComponent::~SpriteComponent()
{
}

//------------------------------------------------------------------------------
void SpriteComponent::Initialize()
{
	VisualComponent::Initialize();
	m_srcRect.Set(0, 0, -1, -1);
	SetSize(Size(-1, -1));

	SetBlendMode(BlendMode::Alpha);

	m_material = NewObject<Material>();

	//owner->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void SpriteComponent::SetTexture(Texture* texture)
{
	//if (LN_CHECK_ARG(m_materialList != nullptr)) return;
	//m_materialList->GetAt(0)->SetMaterialTexture(texture);
	m_texture = texture;
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
	//if (LN_CHECK_ARG(m_materialList != nullptr)) return nullptr;
	//return m_materialList->GetAt(0)->GetMaterialTexture(nullptr);
	return m_texture;
}

//------------------------------------------------------------------------------
void SpriteComponent::SetSourceRect(const Rect& rect)
{
	m_srcRect = rect;
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void SpriteComponent::SetSourceRect(float x, float y, float width, float height)
{
	SetSourceRect(Rect(x, y, width, height));
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
	//Material* mat = GetMainMaterial();
	Color colorScale = GetColorScale();
	colorScale.a *= GetOpacity();
	renderer->SetTransform(GetOwnerObject()->transform.GetWorldMatrix());
	renderer->DrawSprite(
		Vector3::Zero,
		m_renderSize,
		m_anchor,
		GetTexture(),
		m_renderSourceRect,
		colorScale,
		dir,
		GetBillboardType(),
		m_material);
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
	SpriteComponent::Initialize();
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
	obj->Initialize();
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DComponentPtr Sprite3DComponent::Create(float width, float height)
{
	auto obj = Sprite3DComponentPtr::MakeRef();
	obj->Initialize();
	obj->SetSize(Size(width, height));
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DComponentPtr Sprite3DComponent::Create(float width, float height, Texture* texture)
{
	auto obj = Sprite3DComponentPtr::MakeRef();
	obj->Initialize();
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
void Sprite3DComponent::Initialize()
{
	SpriteComponent::Initialize();
}

//------------------------------------------------------------------------------
void Sprite3DComponent::OnRender2(DrawList* renderer)
{
	RenderSprite(renderer, SpriteBaseDirection::ZMinus);
}


//==============================================================================
// Sprite2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(SpriteBase, VisualObject);

//------------------------------------------------------------------------------
SpriteBase::SpriteBase()
	: VisualObject()
{
}

//------------------------------------------------------------------------------
SpriteBase::~SpriteBase()
{
}

//------------------------------------------------------------------------------
void SpriteBase::Initialize()
{
	VisualObject::Initialize();
}

//------------------------------------------------------------------------------
void SpriteBase::SetTexture(Texture* texture)
{
	GetSpriteComponent()->SetTexture(texture);
}

//------------------------------------------------------------------------------
Texture* SpriteBase::GetTexture() const
{
	return GetSpriteComponent()->GetTexture();
}

//------------------------------------------------------------------------------
void SpriteBase::SetSourceRect(const Rect& rect)
{
	GetSpriteComponent()->SetSourceRect(rect);
}

//------------------------------------------------------------------------------
void SpriteBase::SetSourceRect(float x, float y, float width, float height)
{
	GetSpriteComponent()->SetSourceRect(x, y, width, height);
}

//------------------------------------------------------------------------------
const Rect& SpriteBase::GetSourceRect() const
{
	return GetSpriteComponent()->GetSourceRect();
}

//------------------------------------------------------------------------------
void SpriteBase::SetAnchorPoint(const Vector2& ratio)
{
	GetSpriteComponent()->SetAnchorPoint(ratio);
}

//------------------------------------------------------------------------------
void SpriteBase::SetAnchorPoint(float ratioX, float ratioY)
{
	GetSpriteComponent()->SetAnchorPoint(ratioX, ratioY);
}

//==============================================================================
// Sprite2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite2D, SpriteBase);

//------------------------------------------------------------------------------
RefPtr<Sprite2D> Sprite2D::Create()
{
	return NewObject<Sprite2D>();
}

//------------------------------------------------------------------------------
RefPtr<Sprite2D> Sprite2D::Create(const StringRef& filePath)
{
	return NewObject<Sprite2D>(filePath);
}

//------------------------------------------------------------------------------
RefPtr<Sprite2D> Sprite2D::Create(Texture* texture)
{
	return NewObject<Sprite2D>(texture);
}

//------------------------------------------------------------------------------
Sprite2D::Sprite2D()
	: SpriteBase()
	, m_component(nullptr)
{
}

//------------------------------------------------------------------------------
Sprite2D::~Sprite2D()
{
}

//------------------------------------------------------------------------------
void Sprite2D::Initialize()
{
	SpriteBase::Initialize();
	m_component = Sprite2DComponent::Create();
	AddComponent(m_component);
	detail::EngineDomain::GetDefaultWorld2D()->AddWorldObject(this, true);
}

//------------------------------------------------------------------------------
void Sprite2D::Initialize(const StringRef& filePath)
{
	SpriteBase::Initialize();
	m_component = Sprite2DComponent::Create(filePath);
	AddComponent(m_component);
	detail::EngineDomain::GetDefaultWorld2D()->AddWorldObject(this, true);
}

//------------------------------------------------------------------------------
void Sprite2D::Initialize(Texture* texture)
{
	SpriteBase::Initialize();
	m_component = Sprite2DComponent::Create(texture);
	AddComponent(m_component);
	detail::EngineDomain::GetDefaultWorld2D()->AddWorldObject(this, true);
}

//------------------------------------------------------------------------------
VisualComponent* Sprite2D::GetMainVisualComponent() const
{
	return m_component;
}

//------------------------------------------------------------------------------
SpriteComponent* Sprite2D::GetSpriteComponent() const
{
	return m_component;
}


//==============================================================================
// Sprite3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite3D, SpriteBase);

//------------------------------------------------------------------------------
RefPtr<Sprite3D> Sprite3D::Create()
{
	return NewObject<Sprite3D>();
}

//------------------------------------------------------------------------------
RefPtr<Sprite3D> Sprite3D::Create(float width, float height, Texture* texture)
{
	return NewObject<Sprite3D>(width, height, texture);
}

//------------------------------------------------------------------------------
Sprite3D::Sprite3D()
	: SpriteBase()
	, m_component(nullptr)
{
}

//------------------------------------------------------------------------------
Sprite3D::~Sprite3D()
{
}

//------------------------------------------------------------------------------
void Sprite3D::Initialize()
{
	SpriteBase::Initialize();
	m_component = Sprite3DComponent::Create();
	AddComponent(m_component);
	detail::EngineDomain::GetDefaultWorld3D()->AddWorldObject(this, true);
}

//------------------------------------------------------------------------------
void Sprite3D::Initialize(float width, float height, Texture* texture)
{
	SpriteBase::Initialize();
	m_component = Sprite3DComponent::Create(width, height, texture);
	AddComponent(m_component);
	detail::EngineDomain::GetDefaultWorld3D()->AddWorldObject(this, true);
}

//------------------------------------------------------------------------------
VisualComponent* Sprite3D::GetMainVisualComponent() const
{
	return m_component;
}

//------------------------------------------------------------------------------
SpriteComponent* Sprite3D::GetSpriteComponent() const
{
	return m_component;
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
