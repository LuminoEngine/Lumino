/*
	[2015/12/1]
		Scene の TileMapNode 側で SpriteRenderer 使うか頂点バッファに直接書き込むか決めたいので、
		仮想クラスとしておく。
*/
#include "../Internal.h"
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/TileMap/TileMapRenderer.h>
#include <Lumino/TileMap/TileMapModel.h>

LN_NAMESPACE_BEGIN

struct TileMapVertex
{
	Vector3		Position;
	Vector2		TexUV;

	static VertexElement* Elements()
	{
		static VertexElement elements[] =
		{
			{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
		};
		return elements;
	}
	static const int ElementCount = 2;
};

//=============================================================================
// TileMapRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileMapRenderer::TileMapRenderer(GraphicsManager* manager)
	: m_graphicsManager(manager)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TileMapRenderer::~TileMapRenderer()
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::Draw(SpriteRenderer* spriteRenderer, TileMap* tileMap, const RectF& boundingRect, const ViewFrustum& cameraFrustum)
{
	LN_CHECK_ARGS_RETURN(tileMap != nullptr);
	m_spriteRenderer = spriteRenderer;

	// TODO: 原点と正面方向
	Plane plane(Vector3(0, 0, 0), Vector3(0, 0, -1));
	Vector3 corners[8];
	Vector3 pt;
	cameraFrustum.GetCornerPoints(corners);

	// TileMap の平面とカメラの視錐台から描画するべき範囲を求める
	float l, t, r, b;
	for (int i = 0; i < 4; ++i)
	{
		plane.Intersects(corners[i], corners[i+4], &pt);
		if (i == 0)
		{
			l = pt.X;
			t = pt.Y;
			r = pt.X;
			b = pt.Y;
		}
		else
		{
			l = std::min(l, pt.X);
			r = std::max(r, pt.X);
			t = std::max(t, pt.Y);
			b = std::min(b, pt.Y);
		}
	}

	// TODO: ローカル座標→タイルマップ配列インデックスへの座標変換

	const Size& tileSize = tileMap->GetTileSet()->GetTileSize();
	Rect renderRange;
	renderRange.X = (int)l / tileSize.Width;
	renderRange.Y = (int)t / tileSize.Height;
	renderRange.Width = renderRange.X + ((int)r / tileSize.Width);
	renderRange.Height = renderRange.Y - ((int)b / tileSize.Height);


	Begin();
	for (TileLayer* layer : *tileMap->GetLayers())
	{
		Rect r = renderRange;
		// TODO: 3D で Y0 を下端とする
		r.Y = layer->GetSize().Height - r.Y;

		// ループしない
		// TODO:
		if (1)
		{
			Rect maxSize(0, 0, layer->GetSize());
			r.Clip(maxSize);
		}

		DrawLayer(layer, boundingRect, tileMap->GetTileSet(), r);
	}
	End();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::Begin()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::End()
{
	m_spriteRenderer->Flush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::DrawTile(
	const Vector3& position,
	const Vector2& size,
	Texture* texture,
	const Rect& srcRect)
{
	m_spriteRenderer->DrawRequest2D(position, Vector3::Zero, size, texture, srcRect, nullptr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::DrawLayer(TileLayer* layer, const RectF& boundingRect, TileSet* tileSet, const Rect& renderRange)
{
	LN_CHECK_ARGS_RETURN(layer != nullptr);
	LN_CHECK_ARGS_RETURN(tileSet != nullptr);

	int allocedTileCount = 0;
	if (m_vertexBuffer != nullptr && m_indexBuffer != nullptr)
	{
		allocedTileCount = m_vertexBuffer->GetVertexCount() / 4;
	}

	int tileCount = renderRange.Width * renderRange.Height;
	if (tileCount > allocedTileCount)
	{
		LN_SAFE_RELEASE(m_vertexBuffer);
		LN_SAFE_RELEASE(m_indexBuffer);
		m_vertexBuffer = LN_NEW VertexBuffer(m_graphicsManager, TileMapVertex::Elements(), TileMapVertex::ElementCount, tileCount * 4, nullptr, DeviceResourceUsage_Dynamic);
		m_indexBuffer = LN_NEW IndexBuffer(m_graphicsManager, tileCount * 6, nullptr, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic);

		// インデックスバッファは途中で変わらないので先に埋めておく
		ByteBuffer* buf = m_indexBuffer->Lock();
		uint16_t* ib = (uint16_t*)buf->GetData();
		for (int iTile = 0; iTile < tileCount; ++iTile)
		{
			int i = iTile * 6;
			int v = iTile * 4;
			ib[i + 0] = v + 0;
			ib[i + 1] = v + 1;
			ib[i + 2] = v + 2;
			ib[i + 3] = v + 2;
			ib[i + 4] = v + 1;
			ib[i + 5] = v + 3;
		}
		m_indexBuffer->Unlock();
	}






	const Size& tileSize = tileSet->GetTileSize();

	// boundingRect 全体を埋めるように描画するべきセル範囲を決める
	int ox = boundingRect.X / tileSize.Width;
	int oy = boundingRect.Y / tileSize.Height;
	int w = (boundingRect.Width + tileSize.Width) / tileSize.Width;
	int h = (boundingRect.Height + tileSize.Height) / tileSize.Height;

	Vector3 pos;
	Vector2 size;
	Texture* texture;
	Rect srcRect;
	const Size& layerSize = layer->GetSize();

	for (int x = ox; x < w; ++x)
	{
		for (int y = oy; y < h; ++y)
		{
			if (x < layerSize.Width && y < layerSize.Height)
			{
				tileSet->LookupTileImage(layer->GetTileId(x, y), &texture, &srcRect);
				pos.Set(x * tileSize.Width, y * tileSize.Height, 0);
				size.Set(srcRect.Width, srcRect.Height);
				DrawTile(pos, size, texture, srcRect);
			}
		}
	}
}
//
////=============================================================================
//// SpriteTileMapRenderer
////=============================================================================
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SpriteTileMapRenderer::SpriteTileMapRenderer(GraphicsManager* manager)
//	: m_spriteRenderer(nullptr)
//{
//	m_spriteRenderer = SpriteRenderer::Create(MaxSpriteCount, manager);
//	m_spriteRenderer->SetViewPixelSize(Size(640, 480));	// TODO
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//SpriteTileMapRenderer::~SpriteTileMapRenderer()
//{
//	LN_SAFE_RELEASE(m_spriteRenderer);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetTransform(const Matrix& matrix)
//{
//	m_spriteRenderer->SetTransform(matrix);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
//{
//	m_spriteRenderer->SetViewProjMatrix(view, proj);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::SetRenderState(const RenderState& state)
//{
//	m_spriteRenderer->SetRenderState(state);
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::Begin()
//{
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::End()
//{
//	m_spriteRenderer->Flush();
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void SpriteTileMapRenderer::DrawTile(
//	const Vector3& position,
//	const Vector2& size,
//	Texture* texture,
//	const Rect& srcRect)
//{
//	m_spriteRenderer->DrawRequest2D(position, Vector3::Zero, size, texture, srcRect, nullptr);
//}


LN_NAMESPACE_END
