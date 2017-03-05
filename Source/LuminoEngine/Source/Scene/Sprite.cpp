
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>	// TODO: いらない
#include <Lumino/Graphics/Rendering.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"	// TODO: いらない
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Sprite.h>
#include "../Graphics/PrimitiveRenderer.h"	// todo

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// Sprite
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite, VisualNode);

//------------------------------------------------------------------------------
Sprite::Sprite()
	: VisualNode()
	, m_size()
	, m_srcRect()
	, m_flipMode(FlipMode_None)
{
}

//------------------------------------------------------------------------------
Sprite::~Sprite()
{
}

//------------------------------------------------------------------------------
void Sprite::Initialize(SceneGraph* owner)
{
	LN_FAIL_CHECK_ARG(owner != nullptr) return;

	VisualNode::Initialize(owner, 1);
	m_srcRect.Set(0, 0, -1, -1);
	SetSize(Size(-1, -1));

	SetBlendMode(BlendMode::Alpha);

	owner->GetRootNode()->AddChild(this);
	SetAutoRemove(true);
}

//------------------------------------------------------------------------------
void Sprite::SetTexture(Texture* texture)
{
	LN_FAIL_CHECK_ARG(m_materialList != nullptr) return;
	m_materialList->GetAt(0)->SetMaterialTexture(texture);
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void Sprite::SetSize(const Size& size)
{
	m_size = size;
}

//------------------------------------------------------------------------------
Texture* Sprite::GetTexture() const
{
	LN_FAIL_CHECK_ARG(m_materialList != nullptr) return nullptr;
	return m_materialList->GetAt(0)->GetMaterialTexture(nullptr);
}

//------------------------------------------------------------------------------
void Sprite::SetTextureRect(const RectF& rect)
{
	m_srcRect = rect;
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void Sprite::SetTextureRect(float x, float y, float width, float height)
{
	SetTextureRect(RectF(x, y, width, height));
}

//------------------------------------------------------------------------------
void Sprite::SetAnchorPoint(const Vector2& ratio)
{
	m_anchor = ratio;
}

//------------------------------------------------------------------------------
void Sprite::SetAnchorPoint(float ratioX, float ratioY)
{
	m_anchor.Set(ratioX, ratioY);
}

//------------------------------------------------------------------------------
void Sprite::RenderSprite(DrawList* renderer, SpriteBaseDirection dir)
{
	Material* mat = GetMainMaterial();
	Color colorScale = mat->GetColorScale();
	colorScale.a *= mat->GetOpacity();
	renderer->SetTransform(m_combinedGlobalMatrix);
	renderer->DrawSprite(Vector3::Zero, m_renderSize, m_anchor, GetTexture(), m_renderSourceRect, colorScale, dir, GetMainMaterial());
}

//------------------------------------------------------------------------------
void Sprite::UpdateVertexData()
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
// Sprite2D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite2D, Sprite);

//------------------------------------------------------------------------------
Sprite2DPtr Sprite2D::Create()
{
	auto obj = Sprite2DPtr::MakeRef();
	obj->Initialize();
	return obj;
}

//------------------------------------------------------------------------------
Sprite2DPtr Sprite2D::Create(const StringRef& filePath)
{
	auto tex = Texture2D::Create(filePath);	// TODO: from asset
	return Create(tex);
}

//------------------------------------------------------------------------------
Sprite2DPtr Sprite2D::Create(Texture* texture)
{
	auto obj = Create();
	obj->SetTexture(texture);
	return obj;
}

//------------------------------------------------------------------------------
Sprite2D::Sprite2D()
{
}

//------------------------------------------------------------------------------
Sprite2D::~Sprite2D()
{
}

//------------------------------------------------------------------------------
void Sprite2D::Initialize()
{
	Sprite::Initialize(detail::EngineDomain::GetDefaultSceneGraph2D());
}

//------------------------------------------------------------------------------
void Sprite2D::Initialize(Texture* texture)
{
	Initialize();
	SetTexture(texture);
}

//------------------------------------------------------------------------------
void Sprite2D::OnRender2(DrawList* renderer)
{
	RenderSprite(renderer, SpriteBaseDirection::Basic2D);
}


//==============================================================================
// Sprite3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite3D, Sprite);

//------------------------------------------------------------------------------
Sprite3DPtr Sprite3D::Create()
{
	auto obj = Sprite3DPtr::MakeRef();
	obj->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D());
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DPtr Sprite3D::Create(float width, float height)
{
	auto obj = Sprite3DPtr::MakeRef();
	obj->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D());
	obj->SetSize(Size(width, height));
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DPtr Sprite3D::Create(float width, float height, Texture* texture)
{
	auto obj = Sprite3DPtr::MakeRef();
	obj->Initialize(detail::EngineDomain::GetDefaultSceneGraph3D());
	obj->SetSize(Size(width, height));
	obj->SetTexture(texture);
	return obj;
}

//------------------------------------------------------------------------------
Sprite3D::Sprite3D()
{
}

//------------------------------------------------------------------------------
Sprite3D::~Sprite3D()
{
}

//------------------------------------------------------------------------------
void Sprite3D::Initialize(SceneGraph* owner)
{
	Sprite::Initialize(owner);
}

//------------------------------------------------------------------------------
void Sprite3D::OnRender2(DrawList* renderer)
{
	RenderSprite(renderer, SpriteBaseDirection::ZMinus);
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
