
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>	// TODO: いらない
#include <Lumino/Rendering/Rendering.h>
#include "SceneGraphManager.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Sprite.h>
#include <Lumino/World.h>

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
void SpriteComponent::initialize()
{
	VisualComponent::initialize();
	m_srcRect.set(0, 0, -1, -1);
	setSize(Size(-1, -1));

	setBlendMode(BlendMode::Alpha);

	m_material = newObject<Material>();

	//owner->GetRootNode()->AddChild(this);
	setAutoRemove(true);
}

//------------------------------------------------------------------------------
void SpriteComponent::setTexture(Texture* texture)
{
	//if (LN_CHECK_ARG(m_materialList != nullptr)) return;
	//m_materialList->GetAt(0)->setMaterialTexture(texture);
	m_texture = texture;
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void SpriteComponent::setSize(const Size& size)
{
	m_size = size;
}

//------------------------------------------------------------------------------
Texture* SpriteComponent::getTexture() const
{
	//if (LN_CHECK_ARG(m_materialList != nullptr)) return nullptr;
	//return m_materialList->GetAt(0)->getMaterialTexture(nullptr);
	return m_texture;
}

//------------------------------------------------------------------------------
void SpriteComponent::setSourceRect(const Rect& rect)
{
	m_srcRect = rect;
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void SpriteComponent::setSourceRect(float x, float y, float width, float height)
{
	setSourceRect(Rect(x, y, width, height));
}

//------------------------------------------------------------------------------
void SpriteComponent::SetAnchorPoint(const Vector2& ratio)
{
	m_anchor = ratio;
}

//------------------------------------------------------------------------------
void SpriteComponent::SetAnchorPoint(float ratioX, float ratioY)
{
	m_anchor.set(ratioX, ratioY);
}

//------------------------------------------------------------------------------
void SpriteComponent::RenderSprite(DrawList* renderer, SpriteBaseDirection dir)
{
	//Material* mat = GetMainMaterial();
	Color colorScale = getColorScale();
	colorScale.a *= getOpacity();
	renderer->SetTransform(getOwnerObject()->transform.getWorldMatrix());
	renderer->DrawSprite(
		Vector3::Zero,
		m_renderSize,
		m_anchor,
		getTexture(),
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
	Texture* tex = getTexture();
	const SizeI& texSize = (tex != nullptr) ? tex->getSize() : SizeI::Zero;
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
Sprite2DComponentPtr Sprite2DComponent::create()
{
	auto obj = Sprite2DComponentPtr::makeRef();
	obj->initialize();
	return obj;
}

//------------------------------------------------------------------------------
Sprite2DComponentPtr Sprite2DComponent::create(const StringRef& filePath)
{
	auto tex = Texture2D::create(filePath);	// TODO: from asset
	return create(tex);
}

//------------------------------------------------------------------------------
Sprite2DComponentPtr Sprite2DComponent::create(Texture* texture)
{
	auto obj = create();
	obj->setTexture(texture);
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
void Sprite2DComponent::initialize()
{
	SpriteComponent::initialize();
}

//------------------------------------------------------------------------------
void Sprite2DComponent::initialize(Texture* texture)
{
	initialize();
	setTexture(texture);
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
Sprite3DComponentPtr Sprite3DComponent::create()
{
	auto obj = Sprite3DComponentPtr::makeRef();
	obj->initialize();
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DComponentPtr Sprite3DComponent::create(float width, float height)
{
	auto obj = Sprite3DComponentPtr::makeRef();
	obj->initialize();
	obj->setSize(Size(width, height));
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DComponentPtr Sprite3DComponent::create(float width, float height, Texture* texture)
{
	auto obj = Sprite3DComponentPtr::makeRef();
	obj->initialize();
	obj->setSize(Size(width, height));
	obj->setTexture(texture);
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
void Sprite3DComponent::initialize()
{
	SpriteComponent::initialize();
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
void SpriteBase::initialize()
{
	VisualObject::initialize();
}

//------------------------------------------------------------------------------
void SpriteBase::setTexture(Texture* texture)
{
	GetSpriteComponent()->setTexture(texture);
}

//------------------------------------------------------------------------------
Texture* SpriteBase::getTexture() const
{
	return GetSpriteComponent()->getTexture();
}

//------------------------------------------------------------------------------
void SpriteBase::setSourceRect(const Rect& rect)
{
	GetSpriteComponent()->setSourceRect(rect);
}

//------------------------------------------------------------------------------
void SpriteBase::setSourceRect(float x, float y, float width, float height)
{
	GetSpriteComponent()->setSourceRect(x, y, width, height);
}

//------------------------------------------------------------------------------
const Rect& SpriteBase::getSourceRect() const
{
	return GetSpriteComponent()->getSourceRect();
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
RefPtr<Sprite2D> Sprite2D::create()
{
	return newObject<Sprite2D>();
}

//------------------------------------------------------------------------------
RefPtr<Sprite2D> Sprite2D::create(const StringRef& filePath)
{
	return newObject<Sprite2D>(filePath);
}

//------------------------------------------------------------------------------
RefPtr<Sprite2D> Sprite2D::create(Texture* texture)
{
	return newObject<Sprite2D>(texture);
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
void Sprite2D::initialize()
{
	SpriteBase::initialize();
	m_component = Sprite2DComponent::create();
	AddComponent(m_component);
	detail::EngineDomain::getDefaultWorld2D()->addWorldObject(this, true);
}

//------------------------------------------------------------------------------
void Sprite2D::initialize(const StringRef& filePath)
{
	SpriteBase::initialize();
	m_component = Sprite2DComponent::create(filePath);
	AddComponent(m_component);
	detail::EngineDomain::getDefaultWorld2D()->addWorldObject(this, true);
}

//------------------------------------------------------------------------------
void Sprite2D::initialize(Texture* texture)
{
	SpriteBase::initialize();
	m_component = Sprite2DComponent::create(texture);
	AddComponent(m_component);
	detail::EngineDomain::getDefaultWorld2D()->addWorldObject(this, true);
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
RefPtr<Sprite3D> Sprite3D::create()
{
	return newObject<Sprite3D>();
}

//------------------------------------------------------------------------------
RefPtr<Sprite3D> Sprite3D::create(float width, float height, Texture* texture)
{
	return newObject<Sprite3D>(width, height, texture);
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
void Sprite3D::initialize()
{
	SpriteBase::initialize();
	m_component = Sprite3DComponent::create();
	AddComponent(m_component);
	detail::EngineDomain::getDefaultWorld3D()->addWorldObject(this, true);
}

//------------------------------------------------------------------------------
void Sprite3D::initialize(float width, float height, Texture* texture)
{
	SpriteBase::initialize();
	m_component = Sprite3DComponent::create(width, height, texture);
	AddComponent(m_component);
	detail::EngineDomain::getDefaultWorld3D()->addWorldObject(this, true);
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
