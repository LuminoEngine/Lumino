
#include "../Internal.h"
#include <Lumino/Graphics/RenderingContext.h>
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraphRenderingContext.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Sprite.h>
#include "../Graphics/PrimitiveRenderer.h"	// todo

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// Sprite
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite, VisualNode);
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//Sprite* Sprite::Create()
//{
//	RefPtr<Sprite> obj(LN_NEW Sprite(), false);
//	obj->CreateCore(SceneGraphManager::Instance, SpriteCoord_2D);
//	obj.SafeAddRef();
//	return obj;
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//Sprite* Sprite::Create3D()
//{
//	RefPtr<Sprite> obj(LN_NEW Sprite(), false);
//	obj->CreateCore(SceneGraphManager::Instance, SpriteCoord_RZ);
//	obj.SafeAddRef();
//	return obj;
//}

//------------------------------------------------------------------------------
Sprite::Sprite()
	: VisualNode()
	, m_spriteCoord(SpriteCoord_RZ)
	, m_size()
	, m_srcRect()
	, m_flipMode(FlipMode_None)
	, m_upperLeft()
	, m_lowerRight()
	, m_upperLeftUV()
	, m_lowerRightUV()
{
}

//------------------------------------------------------------------------------
Sprite::~Sprite()
{
}

//------------------------------------------------------------------------------
void Sprite::Initialize(SceneGraph* owner, SpriteCoord spriteCoord)
{
	VisualNode::Initialize(owner, 1);
	m_spriteCoord = spriteCoord;
	m_srcRect.Set(0, 0, -1, -1);
	SetSize(SizeF(-1, -1));

	// TODO: もらった owner に追加する、で。
	if (spriteCoord == SpriteCoord_2D)
	{
		owner->GetManager()->GetDefault2DSceneGraph()->GetRootNode()->AddChild(this);
		SetAutoRemove(true);
	}
	else
	{
		owner->GetManager()->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
		SetAutoRemove(true);
	}
}

//------------------------------------------------------------------------------
void Sprite::SetTexture(Texture* texture)
{
	m_materialList.GetAt(0)->SetTexture(texture);
	UpdateTexUV();
	UpdateVertexData();
}

//------------------------------------------------------------------------------
Texture* Sprite::GetTexture() const
{
	return m_materialList.GetAt(0)->GetTexture();
}

//------------------------------------------------------------------------------
void Sprite::SetSrcRect(const Rect& rect)
{
	m_srcRect = rect;
	UpdateTexUV();
	UpdateVertexData();
}

//------------------------------------------------------------------------------
void Sprite::UpdateTexUV()
{
	Texture* tex = GetTexture();
	if (tex)
	{
		// 転送元矩形が負値ならテクスチャ全体を転送する
		if (m_srcRect.width < 0 && m_srcRect.height < 0)
		{
			Rect srcRect(0, 0, tex->GetSize().width, tex->GetSize().height);
			NormalizeSrcRect(
				srcRect, tex->GetRealSize(),
				&m_upperLeftUV.x, &m_upperLeftUV.y, &m_lowerRightUV.x, &m_lowerRightUV.y);
		}
		else
		{
			float l, t, r, b;
			NormalizeSrcRect(m_srcRect, tex->GetRealSize(), &l, &t, &r, &b);

			// 垂直反転
			if (m_flipMode == FlipMode_V || m_flipMode == FlipMode_HV)
			{
				m_upperLeftUV.y = b;
				m_lowerRightUV.y = t;
			}
			else
			{
				m_upperLeftUV.y = t;
				m_lowerRightUV.y = b;
			}
			// 水平反転
			if (m_flipMode == FlipMode_H || m_flipMode == FlipMode_HV)
			{
				m_upperLeftUV.x = r;
				m_lowerRightUV.x = l;
			}
			else
			{
				m_upperLeftUV.x = l;
				m_lowerRightUV.x = r;
			}
		}
	}
	else
	{
		m_upperLeftUV.x = 0.0f;
		m_upperLeftUV.y = 0.0f;
		m_lowerRightUV.x = 1.0f;
		m_lowerRightUV.y = 1.0f;
	}
}

//------------------------------------------------------------------------------
void Sprite::UpdateVertexData()
{
	// サイズが負値     → 転送矩形を使う
	// 転送矩形が負値   → テクスチャサイズを使う
	// テクスチャが無い → サイズ 0,0
	Vector2 realSize(m_size.width, m_size.height);
	if (m_size.width < 0.0 && m_size.height < 0.0)
	{
		if (m_srcRect.width < 0.0 && m_srcRect.height < 0.0)
		{
			Texture* tex = GetTexture();
			if (tex)
				realSize.Set((float)tex->GetSize().width, (float)tex->GetSize().height);
			else
				realSize = Vector2::Zero;
		}
		else
		{
			realSize.Set((float)m_srcRect.width, (float)m_srcRect.height);
		}
	}

	// 2D 空間用スプライト
	if (m_spriteCoord == SpriteCoord_2D)
	{
		float r = realSize.x;
		float b = realSize.y;
		float l = 0;
		float t = 0;
		m_upperLeft.Set(l, t, 0);
		m_lowerRight.Set(r, b, 0);
	}
	// 3D 空間用スプライト
	else
	{
		float r = realSize.x * 0.5f;  // +
		float b = -realSize.y * 0.5f;  // -
		float l = -r;                  // -
		float t = -b;                  // +

		switch (m_spriteCoord)
		{
		case SpriteCoord_X:
			m_upperLeft.Set(0, t, r);
			m_lowerRight.Set(0, b, l);
			break;
		case SpriteCoord_Y:
			m_upperLeft.Set(l, 0, b);
			m_lowerRight.Set(r, 0, t);
			break;
		case SpriteCoord_Z:
			m_upperLeft.Set(r, t, 0);
			m_lowerRight.Set(l, b, 0);
			break;
		case SpriteCoord_RX:
			m_upperLeft.Set(0, t, l);
			m_lowerRight.Set(0, b, r);
			break;
		case SpriteCoord_RY:
			m_upperLeft.Set(r, 0, b);
			m_lowerRight.Set(l, 0, t);
			break;
		case SpriteCoord_RZ:
			m_upperLeft.Set(l, t, 0);
			m_lowerRight.Set(r, b, 0);
			break;
		}
	}
}

//------------------------------------------------------------------------------
void Sprite::NormalizeSrcRect(const Rect& srcRect, const Size& textureSize, float* l, float* t, float* r, float* b)
{
	float tex_rw = 1.0f / textureSize.width;
	float tex_rh = 1.0f / textureSize.height;
	*l = static_cast<float>(srcRect.GetLeft()) * tex_rw;
	*t = static_cast<float>(srcRect.GetTop()) * tex_rh;
	*r = static_cast<float>(srcRect.GetRight()) * tex_rw;
	*b = static_cast<float>(srcRect.GetBottom()) * tex_rh;
}

//------------------------------------------------------------------------------
void Sprite::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	if (subsetIndex == 0)
	{
		dc->GetRenderingContext()->DrawSquare(
			Vector3(m_upperLeft.x, m_upperLeft.y, m_upperLeft.z), Vector2(m_upperLeftUV.x, m_upperLeftUV.y), ColorF::White,
			Vector3(m_upperLeft.x, m_lowerRight.y, m_lowerRight.z), Vector2(m_upperLeftUV.x, m_lowerRightUV.y), ColorF::White,
			Vector3(m_lowerRight.x, m_lowerRight.y, m_lowerRight.z), Vector2(m_lowerRightUV.x, m_lowerRightUV.y), ColorF::White,
			Vector3(m_lowerRight.x, m_upperLeft.y, m_upperLeft.z), Vector2(m_lowerRightUV.x, m_upperLeftUV.y), ColorF::White);
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
	obj->Initialize(SceneGraphManager::Instance->GetDefault2DSceneGraph());
	return obj;
}

//------------------------------------------------------------------------------
Sprite2DPtr Sprite2D::Create(const StringRef& filePath)
{
	auto tex = Texture2D::Create(filePath);
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
void Sprite2D::Initialize(SceneGraph* owner)
{
	Sprite::Initialize(owner, SpriteCoord_2D);
}


//==============================================================================
// Sprite3D
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Sprite3D, Sprite);

//------------------------------------------------------------------------------
Sprite3DPtr Sprite3D::Create()
{
	auto obj = Sprite3DPtr::MakeRef();
	obj->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph());
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DPtr Sprite3D::Create(float width, float height)
{
	auto obj = Sprite3DPtr::MakeRef();
	obj->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph());
	obj->SetSize(SizeF(width, height));
	return obj;
}

//------------------------------------------------------------------------------
Sprite3DPtr Sprite3D::Create(float width, float height, Texture* texture)
{
	auto obj = Sprite3DPtr::MakeRef();
	obj->Initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph());
	obj->SetSize(SizeF(width, height));
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
	Sprite::Initialize(owner, SpriteCoord_RZ);
}


LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
