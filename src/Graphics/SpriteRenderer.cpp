
#include "Internal.h"
#include <math.h>
#include <Lumino/Graphics/GraphicsException.h>
#include "SpriteRenderer.h"
#include "RenderingCommand.h"
#include "GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//==============================================================================
// SpriteRenderer
//==============================================================================

//------------------------------------------------------------------------------
SpriteRenderer* SpriteRenderer::Create(int maxSpriteCount, GraphicsManager* manager)
{
	return LN_NEW SpriteRenderer(manager, maxSpriteCount);
}

//------------------------------------------------------------------------------
SpriteRenderer::SpriteRenderer(GraphicsManager* manager, int maxSpriteCount)
	: m_manager(manager)
	, m_internal(nullptr)
	, m_spriteSortMode(/*SpriteSortMode::Texture | */SpriteSortMode::DepthBackToFront)
{
	m_internal = LN_NEW SpriteRendererImpl();
	m_internal->Initialize(manager, maxSpriteCount);
}

//------------------------------------------------------------------------------
SpriteRenderer::~SpriteRenderer()
{
	LN_SAFE_RELEASE(m_internal);
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetTransform(const Matrix& matrix)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		Matrix, matrix,
		{
			m_internal->SetTransform(matrix);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetState(const RenderState& renderState)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		RenderState, renderState,
		{
			m_internal->SetRenderState(renderState);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetViewInfo(const SizeF& size, const Matrix& view, const Matrix& proj)
{
	LN_ENQUEUE_RENDER_COMMAND_4(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		SizeF, size,
		Matrix, view,
		Matrix, proj,
		{
			m_internal->SetViewPixelSize(size);
			m_internal->SetViewProjMatrix(view, proj);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetSortMode(SpriteSortMode flags, SortingDistanceBasis basis)
{
	m_spriteSortMode = flags;
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest2D(
	const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const RectF& srcRect,
	const Color* colorTable)
{
	SpriteColorTable ct = { {Color::White, Color::White, Color::White, Color::White} };
	if (colorTable != nullptr)
	{
		ct.colors[0] = colorTable[0];
		ct.colors[1] = colorTable[1];
		ct.colors[2] = colorTable[2];
		ct.colors[3] = colorTable[3];
	}

	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_7(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector2, size,
		const Vector2, anchorRatio,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		{
			m_internal->DrawRequest2D(position, size, anchorRatio, deviceTexture, srcRect, ct);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest2D(
	const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const RectF& srcRect,
	const Color& color)
{
	SpriteColorTable ct = { { color, color, color, color } };
	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_7(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector2, size,
		const Vector2, anchorRatio,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		{
			m_internal->DrawRequest2D(position, size, anchorRatio, deviceTexture, srcRect, ct);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest3D(
	const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const RectF& srcRect,
	const Color* colorTable,
	SpriteBaseDirection baseDirection)
{
	SpriteColorTable ct = { { Color::White, Color::White, Color::White, Color::White } };
	if (colorTable != nullptr)
	{
		ct.colors[0] = colorTable[0];
		ct.colors[1] = colorTable[1];
		ct.colors[2] = colorTable[2];
		ct.colors[3] = colorTable[3];
	}

	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector2, size,
		const Vector2, anchorRatio,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		SpriteBaseDirection, baseDirection,
		{
			m_internal->DrawRequest3D(position, size, anchorRatio, deviceTexture, srcRect, ct, baseDirection);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest3D(
	const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const RectF& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection)
{
	SpriteColorTable ct = { { color, color, color, color } };
	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector2, size,
		const Vector2, anchorRatio,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		SpriteBaseDirection, baseDirection,
		{
			m_internal->DrawRequest3D(position, size, anchorRatio, deviceTexture, srcRect, ct, baseDirection);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::Flush()
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_Flush, m_manager,
		SpriteRendererImpl*, m_internal,
		SpriteSortMode, m_spriteSortMode,
		{
			m_internal->Flush(m_spriteSortMode);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::MakeBoundingSphere(const Vector2& size, SpriteBaseDirection baseDir, detail::Sphere* sphere)
{
	Vector2 half = 0.5f * size;
	sphere->radius = half.GetLength();

	if (baseDir == SpriteBaseDirection::Basic2D)
	{
		sphere->center.Set(half.x, half.y, 0);
	}
	else
	{
		sphere->center = Vector3::Zero;
	}
}

//==============================================================================
// SpriteRendererImpl
//==============================================================================
	
static const byte_t g_SpriteRenderer_fx_Data[] =
{
#include "Resource/SpriteRenderer.lfx.h"
};
static const size_t g_SpriteRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_SpriteRenderer_fx_Data);

//------------------------------------------------------------------------------
SpriteRendererImpl::SpriteRendererImpl()
	: m_vertexBuffer()
	, m_indexBuffer()
	, m_maxSprites(0)
	, m_spriteRequestList()
	, m_spriteRequestListUsedCount(0)
	, m_spriteIndexList()
	, m_renderStateList()
	, m_currentRenderStateIndex(0)
	, m_attributeList()
	, m_transformMatrix()
	, m_viewDirection()
	, m_viewInverseMatrix()
	, m_viewPosition()
	, m_viewProjMatrix()
	, m_viewPixelSize()
	, m_sortingBasis(SortingDistanceBasis_RawZ)
{
	//CreateInternal();
}

//------------------------------------------------------------------------------
SpriteRendererImpl::~SpriteRendererImpl()
{
}

////------------------------------------------------------------------------------
//void SpriteRendererImpl::OnChangeDevice(Driver::IGraphicsDevice* device)
//{
//	if (device == NULL)
//	{
//		m_vertexBuffer.SafeRelease();
//		m_indexBuffer.SafeRelease();
//		m_shader.Shader.SafeRelease();
//	}
//	else
//	{
//		CreateInternal();
//	}
//}

//------------------------------------------------------------------------------
void SpriteRendererImpl::Initialize(GraphicsManager* manager, int maxSpriteCount)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_maxSprites = maxSpriteCount;
	auto* device = manager->GetGraphicsDevice();

	//-----------------------------------------------------
	// 頂点バッファとインデックスバッファ
	m_vertexDeclaration.Attach(device->CreateVertexDeclaration(BatchSpriteVertex::Elements(), BatchSpriteVertex::ElementCount));
	m_vertexBuffer.Attach(device->CreateVertexBuffer(sizeof(BatchSpriteVertex) * m_maxSprites * 4, NULL, ResourceUsage::Dynamic));

#if 1
	ByteBuffer indexBuf(sizeof(uint16_t) * m_maxSprites * 6, false);
	uint16_t* ib = (uint16_t*)indexBuf.GetData();
	int idx = 0;
	int i2 = 0;
	for (int i = 0; i < m_maxSprites; ++i)
	{
		i2 = i * 6;
		idx = i * 4;
		ib[i2 + 0] = idx;
		ib[i2 + 1] = idx + 1;
		ib[i2 + 2] = idx + 2;
		ib[i2 + 3] = idx + 2;
		ib[i2 + 4] = idx + 1;
		ib[i2 + 5] = idx + 3;
	}
	m_indexBuffer.Attach(device->CreateIndexBuffer(
		m_maxSprites * 6, ib, IndexBufferFormat_UInt16, ResourceUsage::Dynamic));
#else
	m_indexBuffer.Attach(device->CreateIndexBuffer(
		m_maxSprites * 6, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic));

	// インデックスバッファの値は最初から固定でOKなのでここで書き込んでしまう
	uint16_t* ib;
	size_t lockedSize;	// dummy
	m_indexBuffer->Lock((void**)&ib, &lockedSize);
	int idx = 0;
	int i2 = 0;
	for (int i = 0; i < m_maxSprites; ++i)
	{
		i2 = i * 6;
		idx = i * 4;
		ib[i2 + 0] = idx;
		ib[i2 + 1] = idx + 1;
		ib[i2 + 2] = idx + 2;
		ib[i2 + 3] = idx + 2;
		ib[i2 + 4] = idx + 1;
		ib[i2 + 5] = idx + 3;
	}
	m_indexBuffer->Unlock();
#endif

	//-----------------------------------------------------
	// シェーダ

	ShaderCompileResult r;
	m_shader.Shader.Attach(device->CreateShader(g_SpriteRenderer_fx_Data, g_SpriteRenderer_fx_Len, &r));
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);

	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("gMaterialTexture"));
	m_shader.varViewProjMatrix = m_shader.Shader->GetVariableByName(_T("gViewProjMatrix"));
	m_shader.varViewPixelSize = m_shader.Shader->GetVariableByName(_T("gViewportSize"));
	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("gMaterialTexture"));
	m_shader.techMainDraw = m_shader.Shader->GetTechnique(0);

	//-----------------------------------------------------
	// メモリ確保と各種初期値

	m_spriteRequestList.Resize(m_maxSprites);
	m_spriteIndexList.Resize(m_maxSprites);

	// ステートにはデフォルトをひとつ詰めておく
	m_renderStateList.Add(RenderState());

	//if (m_3DSystem) {
	//	// 視点からの距離が大きいものを先に描画する
	//	SetSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_ViewPont);
	//}
	//else {
	//	// スプライトの Z 値が小さいものを先に描画する
	//	SetSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_RawZ);
	//}

	m_spriteRequestListUsedCount = 0;
	m_currentRenderStateIndex = 0;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetTransform(const Matrix& matrix)
{
	m_transformMatrix = matrix;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
{
	m_viewDirection.Set(view.m[0][2], view.m[1][2], view.m[2][2]);
	m_viewInverseMatrix = Matrix::MakeInverse(view);
	m_viewPosition = m_viewInverseMatrix.GetPosition();
	m_viewProjMatrix = (view * proj);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetViewPixelSize(const SizeF& size)
{
	m_viewPixelSize.Set(size.width, size.height);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetRenderState(const RenderState& state)
{
	// 同じものがあればカレントに
	size_t count = m_renderStateList.GetCount();
	for (size_t i = 0; i < count; ++i)
	{
		if (state == m_renderStateList[i]) {
			m_currentRenderStateIndex = i;
			return;
		}
	}

	// 見つからなかったら登録
	m_renderStateList.Add(state);
	m_currentRenderStateIndex = count;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetSortMode(uint32_t flags, SortingDistanceBasis basis)
{
	m_sortingBasis = basis;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest2D(
    const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Driver::ITexture* texture,
	const RectF& srcRect,
	const SpriteColorTable& colorTable)
{
	DrawRequestInternal(position, size, anchorRatio, texture, srcRect, colorTable, SpriteBaseDirection::Basic2D);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest3D(
	const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Driver::ITexture* texture,
	const RectF& srcRect,
	const SpriteColorTable& colorTable,
	SpriteBaseDirection baseDir)
{
	DrawRequestInternal(position, size, anchorRatio, texture, srcRect, colorTable, baseDir);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequestInternal(
    const Vector3& position,
    const Vector2& size,
	const Vector2& anchorRatio,
	Driver::ITexture* texture,
    const RectF& srcRect,
	const SpriteColorTable& colorTable,
	SpriteBaseDirection baseDir)
{
	LN_THROW(m_spriteRequestListUsedCount < m_maxSprites, InvalidOperationException);

	BatchSpriteData& sprite = m_spriteRequestList[m_spriteRequestListUsedCount];

	Vector2 center(size.x * anchorRatio.x, size.y * anchorRatio.y);

	// 3D の場合の頂点座標
	if (baseDir != SpriteBaseDirection::Basic2D)
	{
		//Vector3 origin(-center);
		Vector2 harf_size(size * 0.5f);
		float l, t, r, b;
		r = harf_size.x;
		b = -harf_size.y;
		l = -r;
		t = -b;

		l -= center.x;
		r -= center.x;
		t -= center.y;
		b -= center.y;

#define LN_WRITE_V3( x_, y_, z_ ) x_, y_, z_

		switch (baseDir)
		{
			case SpriteBaseDirection::XPlus:
				sprite.Vertices[0].Position.Set(LN_WRITE_V3(0, t, l));     // 左上
				sprite.Vertices[1].Position.Set(LN_WRITE_V3(0, b, l));     // 左下
				sprite.Vertices[2].Position.Set(LN_WRITE_V3(0, t, r));     // 右上
				sprite.Vertices[3].Position.Set(LN_WRITE_V3(0, b, r));     // 右下
				break;
			case SpriteBaseDirection::YPlus:
				sprite.Vertices[0].Position.Set(LN_WRITE_V3(l, 0, t));
				sprite.Vertices[1].Position.Set(LN_WRITE_V3(l, 0, b));
				sprite.Vertices[2].Position.Set(LN_WRITE_V3(r, 0, t));
				sprite.Vertices[3].Position.Set(LN_WRITE_V3(r, 0, b));
				break;
			case SpriteBaseDirection::ZPlus:
				sprite.Vertices[0].Position.Set(LN_WRITE_V3(r, t, 0));
				sprite.Vertices[1].Position.Set(LN_WRITE_V3(r, b, 0));
				sprite.Vertices[2].Position.Set(LN_WRITE_V3(l, t, 0));
				sprite.Vertices[3].Position.Set(LN_WRITE_V3(l, b, 0));
				break;
			case SpriteBaseDirection::XMinus:
				sprite.Vertices[0].Position.Set(LN_WRITE_V3(0, t, r));
				sprite.Vertices[1].Position.Set(LN_WRITE_V3(0, b, r));
				sprite.Vertices[2].Position.Set(LN_WRITE_V3(0, t, l));
				sprite.Vertices[3].Position.Set(LN_WRITE_V3(0, b, l));
				break;
			case SpriteBaseDirection::YMinus:
				sprite.Vertices[0].Position.Set(LN_WRITE_V3(r, 0, t));
				sprite.Vertices[1].Position.Set(LN_WRITE_V3(r, 0, b));
				sprite.Vertices[2].Position.Set(LN_WRITE_V3(l, 0, t));
				sprite.Vertices[3].Position.Set(LN_WRITE_V3(l, 0, b));
				break;
			case SpriteBaseDirection::ZMinus:
				sprite.Vertices[0].Position.Set(LN_WRITE_V3(l, t, 0));
				sprite.Vertices[1].Position.Set(LN_WRITE_V3(l, b, 0));
				sprite.Vertices[2].Position.Set(LN_WRITE_V3(r, t, 0));
				sprite.Vertices[3].Position.Set(LN_WRITE_V3(r, b, 0));
				break;
		}
#undef LN_WRITE_V3
	}
	// 2D の場合の頂点座標
	else
	{
		Vector2 origin(-center);
		sprite.Vertices[0].Position.Set(origin.x, origin.y, 0);
		sprite.Vertices[1].Position.Set(origin.x, origin.y + size.y, 0);
		sprite.Vertices[2].Position.Set(origin.x + size.x, origin.y, 0);
		sprite.Vertices[3].Position.Set(origin.x + size.x, origin.y + size.y, 0);
	}

	Matrix mat = m_transformMatrix.GetRotationMatrix();


	// ビルボード (Scene から使う場合は SceneNode が面倒見てるので、Scene 以外で必要になるまで保留…)
	if (0)
	{
		// TODO:
		//mat.setMul3x3( mViewInverseMatrix );
	}
	// ビルボード・Y 軸のみに適用
	else if (0)
	{
		if (m_viewDirection.x > 0.0f)
		{
			mat.RotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
		}
		else if (m_viewDirection.x == 0.0f)
		{
			//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
		}
		else
		{
			mat.RotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
		}

	}
	// ビルボードではない
	else
	{
	}

	mat.Translate(position);
	mat.Translate(m_transformMatrix.GetPosition());

	// 座標変換
	sprite.Vertices[0].Position.TransformCoord(mat);
	sprite.Vertices[1].Position.TransformCoord(mat);
	sprite.Vertices[2].Position.TransformCoord(mat);
	sprite.Vertices[3].Position.TransformCoord(mat);

	// 色
	sprite.Vertices[0].Color = colorTable.colors[0];
	sprite.Vertices[1].Color = colorTable.colors[1];
	sprite.Vertices[2].Color = colorTable.colors[2];
	sprite.Vertices[3].Color = colorTable.colors[3];

	// テクスチャ
	if (texture != nullptr)
	{
		// テクスチャ座標
		const SizeI& texSize = texture->GetRealSize();
		Vector2 texSizeInv(1.0f / texSize.width, 1.0f / texSize.height);
		RectF sr(srcRect);
		float l = sr.x * texSizeInv.x;
		float t = sr.y * texSizeInv.y;
		float r = (sr.x + sr.width) * texSizeInv.x;
		float b = (sr.y + sr.height) * texSizeInv.y;
		sprite.Vertices[0].TexUV.x = l;
		sprite.Vertices[0].TexUV.y = t;
		sprite.Vertices[1].TexUV.x = l;
		sprite.Vertices[1].TexUV.y = b;
		sprite.Vertices[2].TexUV.x = r;
		sprite.Vertices[2].TexUV.y = t;
		sprite.Vertices[3].TexUV.x = r;
		sprite.Vertices[3].TexUV.y = b;

		// テクスチャ
		sprite.Texture = texture;	// TOOD: AddRef するべきかも？
	}
	else
	{
		sprite.Vertices[0].TexUV.x = 0;
		sprite.Vertices[0].TexUV.y = 0;
		sprite.Vertices[1].TexUV.x = 0;
		sprite.Vertices[1].TexUV.y = 1;
		sprite.Vertices[2].TexUV.x = 1;
		sprite.Vertices[2].TexUV.y = 0;
		sprite.Vertices[3].TexUV.x = 1;
		sprite.Vertices[3].TexUV.y = 1;
		sprite.Texture = m_manager->GetDummyDeviceTexture();
	}

	// カメラからの距離をソート用Z値にする場合
	if (m_sortingBasis == SortingDistanceBasis_ViewPont) {
		sprite.Depth = (m_viewPosition - position).GetLengthSquared();
	}
	else {
		sprite.Depth = position.z;
	}

	sprite.Visible = true;
	sprite.Assigned = false;
	sprite.Priority = 0;
	sprite.RenderStateIndex = m_currentRenderStateIndex;

	++m_spriteRequestListUsedCount;
}


//------------------------------------------------------------------------------
// Z 値の大きい方から小さい方へソートする比較
class SpriteRendererImpl::SpriteCmpDepthBackToFront
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				return lsp.Depth > rsp.Depth;
				//if (lsp.Depth > rsp.Depth)
				//{
				//	return true;
				//}
				//if (lsp.Depth == rsp.Depth)
				//{
				//	return lsp.Texture < rsp.Texture;
				//}
				//return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z 値の小さい方から大きい方へソートする比較
class SpriteRendererImpl::SpriteCmpDepthFrontToBack
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				return lsp.Depth < rsp.Depth;
				//if (lsp.Depth < rsp.Depth)
				//{
				//	return true;
				//}
				//if (lsp.Depth == rsp.Depth)
				//{
				//	return lsp.Texture < rsp.Texture;
				//}
				//return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z 値の大きい方から小さい方へソートする比較 (テクスチャ優先)
class SpriteRendererImpl::SpriteCmpTexDepthBackToFront
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Texture < rsp.Texture)
				{
					return true;
				}
				if (lsp.Texture == rsp.Texture)
				{
					// Depth 降順
					return lsp.Depth > rsp.Depth;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z 値の小さい方から大きい方へソートする比較 (テクスチャ優先)
class SpriteRendererImpl::SpriteCmpTexDepthFrontToBack
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Texture < rsp.Texture)
				{
					return true;
				}
				if (lsp.Texture == rsp.Texture)
				{
					return lsp.Depth < rsp.Depth;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};


//------------------------------------------------------------------------------
void SpriteRendererImpl::Flush(SpriteSortMode sortFlags)
{
	int spriteCount = m_spriteRequestListUsedCount;	// 描画するスプライトの数
	if (spriteCount == 0) { return; }				// 0 個ならなにもしない

	// 描画するスプライトのインデックスリストを初期化する
	for (int i = 0; i < spriteCount; ++i) {
		m_spriteIndexList[i] = i;
	}
	//memcpy(m_spriteIndexList, mSpriteIndexArraySource, sizeof(*m_spriteIndexList) * mLastSpriteNum);

	// インデックスを並び替える
	if (sortFlags & SpriteSortMode::Texture)
	{
		if (sortFlags & SpriteSortMode::DepthBackToFront)
		{
			SpriteCmpTexDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (sortFlags & SpriteSortMode::DepthFrontToBack)
		{
			SpriteCmpTexDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}
	else
	{
		if (sortFlags & SpriteSortMode::DepthBackToFront)
		{
			SpriteCmpDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (sortFlags & SpriteSortMode::DepthFrontToBack)
		{
			SpriteCmpDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}

	int si = 0;     // Sprite Index (m_spriteIndexList)
	int vi = 0;     // Vertex Index
	int ri = 0;     // Request Index (m_spriteRequestList)
	int start_idx = 0;
	int prim_num = 0;
	Driver::ITexture* current_tex = m_spriteRequestList[m_spriteIndexList[0]].Texture;
	int currnetRenderStateIndex = m_spriteRequestList[m_spriteIndexList[0]].RenderStateIndex;

	//-----------------------------------------------------
	// 属性リストを作る

	m_attributeList.Clear();
	while (true)
	{
		while (true)
		{
			++si;
			++prim_num;
#if 1
			// 次のスプライトのテクスチャが、処理中のテクスチャと異なる場合は次の属性作成に移る
			if (si >= spriteCount ||
				m_spriteRequestList[m_spriteIndexList[si]].Texture != current_tex ||
				m_spriteRequestList[m_spriteIndexList[si]].RenderStateIndex != currnetRenderStateIndex)
			{
				break;
			}
#else
			// 次のスプライトのテクスチャが、処理中のテクスチャと異なる場合は次の属性作成に移る
			if (si >= spriteCount || m_spriteRequestList[m_spriteIndexList[si]].Texture != current_tex)
			{
				break;
			}
#endif
		}

		Attribute attr;
		attr.StartIndex = start_idx;
		attr.PrimitiveNum = prim_num * 2;
		attr.Texture = current_tex;
		attr.RenderStateIndex = currnetRenderStateIndex;
		m_attributeList.Add(attr);

		if (si >= spriteCount)
		{
			break;
		}

		start_idx = si * 6;
		prim_num = 0;
		current_tex = m_spriteRequestList[m_spriteIndexList[si]].Texture;
		currnetRenderStateIndex = m_spriteRequestList[m_spriteIndexList[si]].RenderStateIndex;
	}

	//-----------------------------------------------------
	// 頂点データをコピー

	BatchSpriteVertex* vb = static_cast< BatchSpriteVertex* >(m_vertexBuffer->Lock());
	si = 0;
	vi = 0;
	for (; si < spriteCount; ++si)
	{
		ri = m_spriteIndexList[si];
		memcpy(&vb[vi], m_spriteRequestList[ri].Vertices, sizeof(m_spriteRequestList[0].Vertices));
		vi += 4;
	}
	m_vertexBuffer->Unlock();

	//-----------------------------------------------------
	// 描画

	auto* r = m_manager->GetGraphicsDevice()->GetRenderer();
	m_shader.varViewProjMatrix->SetMatrix(m_viewProjMatrix);
	m_shader.varViewPixelSize->SetVector(Vector4(m_viewPixelSize.x, m_viewPixelSize.y, 0, 0));

	Driver::IShaderPass* pass = m_shader.techMainDraw->GetPass(0);

	AttributeList::iterator itr = m_attributeList.begin();
	AttributeList::iterator end = m_attributeList.end();
	for (; itr != end; ++itr)
	{
		//r->SetRenderState(m_renderStateList[itr->RenderStateIndex]);
		m_shader.varTexture->SetTexture(itr->Texture);
		r->SetShaderPass(pass);
		r->SetVertexDeclaration(m_vertexDeclaration);
		r->SetVertexBuffer(0, m_vertexBuffer);
		r->SetIndexBuffer(m_indexBuffer);
		r->DrawPrimitiveIndexed(PrimitiveType_TriangleList, itr->StartIndex, itr->PrimitiveNum);
	}

	//-----------------------------------------------------
	// クリーンアップ

	Clear();
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::Clear()
{
	m_renderStateList.Clear();
	m_renderStateList.Add(RenderState());
	m_currentRenderStateIndex = 0;
	m_spriteRequestListUsedCount = 0;
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
