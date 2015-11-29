
#pragma once
#include "SceneGraphManager.h"
#include "RenderingPass.h"
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Sprite.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//=============================================================================
// Sprite
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(Sprite, VisualNode);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite* Sprite::Create()
{
	RefPtr<Sprite> obj(LN_NEW Sprite(), false);
	obj->CreateCore(SceneGraphManager::Instance, SpriteCoord_2D);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite* Sprite::Create3D()
{
	RefPtr<Sprite> obj(LN_NEW Sprite(), false);
	obj->CreateCore(SceneGraphManager::Instance, SpriteCoord_RZ);
	obj.SafeAddRef();
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Sprite::~Sprite()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite::CreateCore(SceneGraphManager* manager, SpriteCoord spriteCoord)
{
	VisualNode::CreateCore(manager, 1);
	m_spriteCoord = spriteCoord;
	m_srcRect.Set(0, 0, -1, -1);
	SetSize(SizeF(-1, -1));

	if (spriteCoord == SpriteCoord_2D) {
		manager->GetDefault2DSceneGraph()->GetRootNode()->AddChild(this);
	}
	else {
		manager->GetDefault3DSceneGraph()->GetRootNode()->AddChild(this);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite::SetTexture(Texture* texture)
{
	m_visualNodeParams.GetSubsetParams(0).Material.Texture = texture;
	UpdateTexUV();
	UpdateVertexData();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Sprite::GetTexture() const
{
	return m_visualNodeParams.GetSubsetParams(0).Material.Texture;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite::SetSrcRect(const Rect& rect)
{
	m_srcRect = rect;
	UpdateTexUV();
	UpdateVertexData();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite::UpdateTexUV()
{
	Texture* tex = GetTexture();
	if (tex)
	{
		// 転送元矩形が負値ならテクスチャ全体を転送する
		if (m_srcRect.Width < 0 && m_srcRect.Height < 0)
		{
			Rect srcRect(0, 0, tex->GetSize().Width, tex->GetSize().Height);
			NormalizeSrcRect(
				srcRect, tex->GetRealSize(),
				&m_upperLeftUV.X, &m_upperLeftUV.Y, &m_lowerRightUV.X, &m_lowerRightUV.Y);
		}
		else
		{
			float l, t, r, b;
			NormalizeSrcRect(m_srcRect, tex->GetRealSize(), &l, &t, &r, &b);

			// 垂直反転
			if (m_flipMode == FlipMode_V || m_flipMode == FlipMode_HV)
			{
				m_upperLeftUV.Y = b;
				m_lowerRightUV.Y = t;
			}
			else
			{
				m_upperLeftUV.Y = t;
				m_lowerRightUV.Y = b;
			}
			// 水平反転
			if (m_flipMode == FlipMode_H || m_flipMode == FlipMode_HV)
			{
				m_upperLeftUV.X = r;
				m_lowerRightUV.X = l;
			}
			else
			{
				m_upperLeftUV.X = l;
				m_lowerRightUV.X = r;
			}
		}
	}
	else
	{
		m_upperLeftUV.X = 0.0f;
		m_upperLeftUV.Y = 0.0f;
		m_lowerRightUV.X = 1.0f;
		m_lowerRightUV.Y = 1.0f;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite::UpdateVertexData()
{
	// サイズが負値     → 転送矩形を使う
	// 転送矩形が負値   → テクスチャサイズを使う
	// テクスチャが無い → サイズ 0,0
	Vector2 realSize(m_size.Width, m_size.Height);
	if (m_size.Width < 0.0 && m_size.Height < 0.0)
	{
		if (m_srcRect.Width < 0.0 && m_srcRect.Height < 0.0)
		{
			Texture* tex = GetTexture();
			if (tex)
				realSize.Set((float)tex->GetSize().Width, (float)tex->GetSize().Height);
			else
				realSize = Vector2::Zero;
		}
		else
		{
			realSize.Set((float)m_srcRect.Width, (float)m_srcRect.Height);
		}
	}

	// 2D 空間用スプライト
	if (m_spriteCoord == SpriteCoord_2D)
	{
		float r = realSize.X;
		float b = realSize.Y;
		float l = 0;
		float t = 0;
		m_upperLeft.Set(l, t, 0);
		m_lowerRight.Set(r, b, 0);
	}
	// 3D 空間用スプライト
	else
	{
		float r = realSize.X * 0.5f;  // +
		float b = -realSize.Y * 0.5f;  // -
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

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite::NormalizeSrcRect(const Rect& srcRect, const Size& textureSize, float* l, float* t, float* r, float* b)
{
	float tex_rw = 1.0f / textureSize.Width;
	float tex_rh = 1.0f / textureSize.Height;
	*l = static_cast<float>(srcRect.GetLeft()) * tex_rw;
	*t = static_cast<float>(srcRect.GetTop()) * tex_rh;
	*r = static_cast<float>(srcRect.GetRight()) * tex_rw;
	*b = static_cast<float>(srcRect.GetBottom()) * tex_rh;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Sprite::DrawSubset(RenderingParams& params, int subsetIndex)
{
	if (subsetIndex == 0)
	{
		params.GeometryRenderer->DrawSquare(
			m_upperLeft.X, m_upperLeft.Y, m_upperLeft.Z, m_upperLeftUV.X, m_upperLeftUV.Y, ColorF::White,
			m_lowerRight.X, m_upperLeft.Y, m_upperLeft.Z, m_lowerRightUV.X, m_upperLeftUV.Y, ColorF::White,
			m_upperLeft.X, m_lowerRight.Y, m_lowerRight.Z, m_upperLeftUV.X, m_lowerRightUV.Y, ColorF::White,
			m_lowerRight.X, m_lowerRight.Y, m_lowerRight.Z, m_lowerRightUV.X, m_lowerRightUV.Y, ColorF::White);
	}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
