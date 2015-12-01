/*
	[2015/12/1]
		Scene の TileMapNode 側で SpriteRenderer 使うか頂点バッファに直接書き込むか決めたいので、
		仮想クラスとしておく。
*/
#include "../Internal.h"
#include <Lumino/Graphics/SpriteRenderer.h>
#include <Lumino/TileMap/TileMapRenderer.h>
#include <Lumino/TileMap/TileMap.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// TileMapRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::Draw(TileMap* tileMap, const RectF& boundingRect)
{
	LN_CHECK_ARGS_RETURN(tileMap != nullptr);
	for (TileLayer* layer : *tileMap->GetLayers()) {
		DrawLayer(layer, boundingRect, tileMap->GetTileSet());
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TileMapRenderer::DrawLayer(TileLayer* layer, const RectF& boundingRect, TileSet* tileSet)
{
	LN_CHECK_ARGS_RETURN(layer != nullptr);
	LN_CHECK_ARGS_RETURN(tileSet != nullptr);
	const Size& tileSize = tileSet->SetTileSize();

	// boundingRect 全体を埋めるように描画するべきセル範囲を決める
	int x = boundingRect.X / tileSize.Width;
	int y = boundingRect.Y / tileSize.Height;
	int w = (boundingRect.Width + tileSize.Width) / tileSize.Width;
	int h = (boundingRect.Height + tileSize.Height) / tileSize.Height;

	Vector3 pos;
	Vector2 size;
	Texture* texture;
	Rect srcRect;

	for (; x < w; ++x)
	{
		for (; y < h; ++y)
		{
			tileSet->LookupTileImage(layer->GetTileId(x, y), &texture, &srcRect);
			pos.Set(x, y, 0);
			size.Set(srcRect.Width, srcRect.Height);
			DrawTile(pos, size, texture, srcRect);
		}
	}
}

//=============================================================================
// SpriteTileMapRenderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteTileMapRenderer::SpriteTileMapRenderer(GraphicsManager* manager)
	: m_spriteRenderer(nullptr)
{
	m_spriteRenderer = SpriteRenderer::Create(MaxSpriteCount, manager);
	m_spriteRenderer->SetViewPixelSize(Size(640, 480));	// TODO
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SpriteTileMapRenderer::~SpriteTileMapRenderer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteTileMapRenderer::SetTransform(const Matrix& matrix)
{
	m_spriteRenderer->SetTransform(matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteTileMapRenderer::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
{
	m_spriteRenderer->SetViewProjMatrix(view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteTileMapRenderer::SetRenderState(const RenderState& state)
{
	m_spriteRenderer->SetRenderState(state);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteTileMapRenderer::Begin()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteTileMapRenderer::End()
{
	m_spriteRenderer->Flush();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void SpriteTileMapRenderer::DrawTile(
	const Vector3& position,
	const Vector2& size,
	Texture* texture,
	const Rect& srcRect)
{
	m_spriteRenderer->DrawRequest3D(position, Vector3::Zero, size, texture, srcRect, nullptr, AxisDirection_RZ);
}


LN_NAMESPACE_END
