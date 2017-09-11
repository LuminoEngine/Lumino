
#include "Internal.h"
#include <math.h>
#include <Lumino/Graphics/GraphicsException.h>
#include "SpriteRenderFeature.h"
#include "../Graphics/RenderingCommand.h"
#include "../Graphics/GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//==============================================================================
// SpriteRenderFeature
//==============================================================================

//------------------------------------------------------------------------------
SpriteRenderFeature* SpriteRenderFeature::create(int maxSpriteCount, GraphicsManager* manager)
{
	return LN_NEW SpriteRenderFeature(manager, maxSpriteCount);
}

//------------------------------------------------------------------------------
SpriteRenderFeature::SpriteRenderFeature(GraphicsManager* manager, int maxSpriteCount)
	: m_manager(manager)
	, m_internal(nullptr)
	, m_spriteSortMode(/*SpriteSortMode::Texture | */SpriteSortMode::DepthBackToFront)
{
	m_internal = LN_NEW SpriteRendererImpl();
	m_internal->initialize(manager, maxSpriteCount);
}

//------------------------------------------------------------------------------
SpriteRenderFeature::~SpriteRenderFeature()
{
	LN_SAFE_RELEASE(m_internal);
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::setTransform(const Matrix& matrix)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		Matrix, matrix,
		{
			m_internal->setTransform(matrix);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::setState(const RenderState& renderState)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		RenderState, renderState,
		{
			m_internal->setRenderState(renderState);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::setViewInfo(const Size& size, const Matrix& view, const Matrix& proj)
{
	LN_ENQUEUE_RENDER_COMMAND_4(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		Size, size,
		Matrix, view,
		Matrix, proj,
		{
			m_internal->setViewPixelSize(size);
			m_internal->setViewProjMatrix(view, proj);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::setSortMode(SpriteSortMode flags, SortingDistanceBasis basis)
{
	m_spriteSortMode = flags;
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::drawRequest2D(
	const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const Rect& srcRect,
	const Color& color,
	BillboardType billboardType)
{
	SpriteColorTable ct = { { color, color, color, color } };
	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->resolveDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector2, size,
		const Vector2, anchorRatio,
		Ref<Driver::ITexture>, deviceTexture,
		const Rect, srcRect,
		SpriteColorTable, ct,
		BillboardType, billboardType,
		{
			m_internal->drawRequestInternal(position, size, anchorRatio, deviceTexture, srcRect, ct, SpriteBaseDirection::Basic2D, billboardType);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::drawRequest(
	const Vector3& position,
	const Vector2& size,
	const Vector2& anchorRatio,
	Texture* texture,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType)
{
	SpriteColorTable ct = { { color, color, color, color } };
	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->resolveDeviceObject() : nullptr;

	struct Options
	{
		SpriteBaseDirection baseDirection;
		BillboardType billboardType;
	} opt;
	opt.baseDirection = baseDirection;
	opt.billboardType = billboardType;

	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector2, size,
		const Vector2, anchorRatio,
		Ref<Driver::ITexture>, deviceTexture,
		const Rect, srcRect,
		SpriteColorTable, ct,
		Options, opt,
		{
			m_internal->drawRequestInternal(position, size, anchorRatio, deviceTexture, srcRect, ct, opt.baseDirection, opt.billboardType);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::flush()
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_Flush, m_manager,
		SpriteRendererImpl*, m_internal,
		SpriteSortMode, m_spriteSortMode,
		{
			m_internal->flush(m_spriteSortMode);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderFeature::makeBoundingSphere(const Vector2& size, SpriteBaseDirection baseDir, detail::Sphere* sphere)
{
	Vector2 half = 0.5f * size;
	sphere->radius = half.getLength();

	if (baseDir == SpriteBaseDirection::Basic2D)
	{
		sphere->center.set(half.x, half.y, 0);
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
//void SpriteRendererImpl::onChangeDevice(Driver::IGraphicsDevice* device)
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
void SpriteRendererImpl::initialize(GraphicsManager* manager, int maxSpriteCount)
{
	if (LN_REQUIRE(manager != nullptr)) return;
	m_manager = manager;
	m_maxSprites = maxSpriteCount;
	auto* device = manager->getGraphicsDevice();

	//-----------------------------------------------------
	// 頂点バッファとインデックスバッファ
	m_vertexDeclaration.attach(device->createVertexDeclaration(BatchSpriteVertex::Elements(), BatchSpriteVertex::ElementCount));
	m_vertexBuffer.attach(device->createVertexBuffer(sizeof(BatchSpriteVertex) * m_maxSprites * 4, NULL, ResourceUsage::Dynamic));

#if 1
	ByteBuffer indexBuf(sizeof(uint16_t) * m_maxSprites * 6, false);
	uint16_t* ib = (uint16_t*)indexBuf.getData();
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
	m_indexBuffer.attach(device->createIndexBuffer(
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
	m_shader.Shader.attach(device->createShader(g_SpriteRenderer_fx_Data, g_SpriteRenderer_fx_Len, &r));
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);

	m_shader.varTexture = m_shader.Shader->getVariableByName(_LT("gMaterialTexture"));
	m_shader.varViewProjMatrix = m_shader.Shader->getVariableByName(_LT("gViewProjMatrix"));
	m_shader.varViewPixelSize = m_shader.Shader->getVariableByName(_LT("gViewportSize"));
	m_shader.varTexture = m_shader.Shader->getVariableByName(_LT("gMaterialTexture"));
	m_shader.techMainDraw = m_shader.Shader->getTechnique(0);

	//-----------------------------------------------------
	// メモリ確保と各種初期値

	m_spriteRequestList.resize(m_maxSprites);
	m_spriteIndexList.resize(m_maxSprites);

	// ステートにはデフォルトをひとつ詰めておく
	m_renderStateList.add(RenderState());

	//if (m_3DSystem) {
	//	// 視点からの距離が大きいものを先に描画する
	//	setSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_ViewPont);
	//}
	//else {
	//	// スプライトの Z 値が小さいものを先に描画する
	//	setSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_RawZ);
	//}

	m_spriteRequestListUsedCount = 0;
	m_currentRenderStateIndex = 0;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::setTransform(const Matrix& matrix)
{
	m_transformMatrix = matrix;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::setViewProjMatrix(const Matrix& view, const Matrix& proj)
{
	m_viewDirection.set(view.m[0][2], view.m[1][2], view.m[2][2]);
	m_viewInverseMatrix = Matrix::makeInverse(view);
	m_viewPosition = m_viewInverseMatrix.getPosition();
	m_viewProjMatrix = (view * proj);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::setViewPixelSize(const Size& size)
{
	m_viewPixelSize.set(size.width, size.height);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::setRenderState(const RenderState& state)
{
	// 同じものがあればカレントに
	size_t count = m_renderStateList.getCount();
	for (size_t i = 0; i < count; ++i)
	{
		if (state == m_renderStateList[i]) {
			m_currentRenderStateIndex = i;
			return;
		}
	}

	// 見つからなかったら登録
	m_renderStateList.add(state);
	m_currentRenderStateIndex = count;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::setSortMode(uint32_t flags, SortingDistanceBasis basis)
{
	m_sortingBasis = basis;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::drawRequestInternal(
    const Vector3& position,
    const Vector2& size,
	const Vector2& anchorRatio,
	Driver::ITexture* texture,
    const Rect& srcRect,
	const SpriteColorTable& colorTable,
	SpriteBaseDirection baseDir,
	BillboardType billboardType)
{
	if (LN_REQUIRE(m_spriteRequestListUsedCount < m_maxSprites)) return;

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
				sprite.Vertices[0].Position.set(LN_WRITE_V3(0, t, l));     // 左上
				sprite.Vertices[1].Position.set(LN_WRITE_V3(0, b, l));     // 左下
				sprite.Vertices[2].Position.set(LN_WRITE_V3(0, t, r));     // 右上
				sprite.Vertices[3].Position.set(LN_WRITE_V3(0, b, r));     // 右下
				break;
			case SpriteBaseDirection::YPlus:
				sprite.Vertices[0].Position.set(LN_WRITE_V3(l, 0, t));
				sprite.Vertices[1].Position.set(LN_WRITE_V3(l, 0, b));
				sprite.Vertices[2].Position.set(LN_WRITE_V3(r, 0, t));
				sprite.Vertices[3].Position.set(LN_WRITE_V3(r, 0, b));
				break;
			case SpriteBaseDirection::ZPlus:
				sprite.Vertices[0].Position.set(LN_WRITE_V3(r, t, 0));
				sprite.Vertices[1].Position.set(LN_WRITE_V3(r, b, 0));
				sprite.Vertices[2].Position.set(LN_WRITE_V3(l, t, 0));
				sprite.Vertices[3].Position.set(LN_WRITE_V3(l, b, 0));
				break;
			case SpriteBaseDirection::XMinus:
				sprite.Vertices[0].Position.set(LN_WRITE_V3(0, t, r));
				sprite.Vertices[1].Position.set(LN_WRITE_V3(0, b, r));
				sprite.Vertices[2].Position.set(LN_WRITE_V3(0, t, l));
				sprite.Vertices[3].Position.set(LN_WRITE_V3(0, b, l));
				break;
			case SpriteBaseDirection::YMinus:
				sprite.Vertices[0].Position.set(LN_WRITE_V3(r, 0, t));
				sprite.Vertices[1].Position.set(LN_WRITE_V3(r, 0, b));
				sprite.Vertices[2].Position.set(LN_WRITE_V3(l, 0, t));
				sprite.Vertices[3].Position.set(LN_WRITE_V3(l, 0, b));
				break;
			case SpriteBaseDirection::ZMinus:
				sprite.Vertices[0].Position.set(LN_WRITE_V3(l, t, 0));
				sprite.Vertices[1].Position.set(LN_WRITE_V3(l, b, 0));
				sprite.Vertices[2].Position.set(LN_WRITE_V3(r, t, 0));
				sprite.Vertices[3].Position.set(LN_WRITE_V3(r, b, 0));
				break;
		}
#undef LN_WRITE_V3
	}
	// 2D の場合の頂点座標
	else
	{
		Vector2 origin(-center);
		sprite.Vertices[0].Position.set(origin.x, origin.y, 0);
		sprite.Vertices[1].Position.set(origin.x, origin.y + size.y, 0);
		sprite.Vertices[2].Position.set(origin.x + size.x, origin.y, 0);
		sprite.Vertices[3].Position.set(origin.x + size.x, origin.y + size.y, 0);
	}

	Matrix mat = m_transformMatrix.getRotationMatrix();

	//m_viewPosition.print();

	// TODO: もう position もらわなくていいかも。TextRenderer とおなじく、matrix 直接もらってそれ使おう。
	Vector3 worldPoint = Vector3::transformCoord(position, m_transformMatrix);

	// ビルボード
	if (billboardType == BillboardType::ToCameraPoint)
	{
		Vector3 f = Vector3::normalize(m_viewPosition - worldPoint);
		Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
		Vector3 u = Vector3::cross(f, r);
		mat = Matrix(
			r.x, r.y, r.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			f.x, f.y, f.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (billboardType == BillboardType::ToScreen)
	{
		// ↑がカメラ位置を基準にするのに対し、こちらはビュー平面に垂直に交差する点を基準とする。

		// ビュー平面との距離
		float d = Vector3::dot(worldPoint - m_viewPosition, m_viewDirection);

		// left-hand coord
		Vector3 f = Vector3::normalize(m_viewDirection * d);
		Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
		Vector3 u = Vector3::cross(f, r);
		mat = Matrix(
			r.x, r.y, r.z, 0.0f,
			u.x, u.y, u.z, 0.0f,
			f.x, f.y, f.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);
	}
	// ビルボード・Y 軸のみに適用
	else if (0)
	{
		if (m_viewDirection.x > 0.0f)
		{
			mat.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
		}
		else if (m_viewDirection.x == 0.0f)
		{
			//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
		}
		else
		{
			mat.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
		}

	}
	// ビルボードではない
	else
	{
	}

	mat.translate(position);
	mat.translate(m_transformMatrix.getPosition());

	// 座標変換
	sprite.Vertices[0].Position.transformCoord(mat);
	sprite.Vertices[1].Position.transformCoord(mat);
	sprite.Vertices[2].Position.transformCoord(mat);
	sprite.Vertices[3].Position.transformCoord(mat);

	// 色
	sprite.Vertices[0].Color = colorTable.colors[0];
	sprite.Vertices[1].Color = colorTable.colors[1];
	sprite.Vertices[2].Color = colorTable.colors[2];
	sprite.Vertices[3].Color = colorTable.colors[3];

	// テクスチャ
	if (texture != nullptr)
	{
		// テクスチャ座標
		const SizeI& texSize = texture->getRealSize();
		Vector2 texSizeInv(1.0f / texSize.width, 1.0f / texSize.height);
		Rect sr(srcRect);
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
		sprite.Texture = m_manager->getDummyDeviceTexture();
	}

	// カメラからの距離をソート用Z値にする場合
	if (m_sortingBasis == SortingDistanceBasis_ViewPont) {
		sprite.Depth = (m_viewPosition - position).getLengthSquared();
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
		const BatchSpriteData& lsp = spriteList->getAt(l_);
		const BatchSpriteData& rsp = spriteList->getAt(r_);

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
		const BatchSpriteData& lsp = spriteList->getAt(l_);
		const BatchSpriteData& rsp = spriteList->getAt(r_);

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
		const BatchSpriteData& lsp = spriteList->getAt(l_);
		const BatchSpriteData& rsp = spriteList->getAt(r_);

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
		const BatchSpriteData& lsp = spriteList->getAt(l_);
		const BatchSpriteData& rsp = spriteList->getAt(r_);

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
void SpriteRendererImpl::flush(SpriteSortMode sortFlags)
{
	int spriteCount = m_spriteRequestListUsedCount;	// 描画するスプライトの数
	if (spriteCount == 0) { return; }				// 0 個ならなにもしない

	// 描画するスプライトのインデックスリストを初期化する
	for (int i = 0; i < spriteCount; ++i) {
		m_spriteIndexList[i] = i;
	}
	//memcpy(m_spriteIndexList, mSpriteIndexArraySource, sizeof(*m_spriteIndexList) * mLastSpriteNum);

	// インデックスを並び替える
	if (sortFlags.TestFlag(SpriteSortMode::Texture))
	{
		if (sortFlags.TestFlag(SpriteSortMode::DepthBackToFront))
		{
			SpriteCmpTexDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (sortFlags.TestFlag(SpriteSortMode::DepthFrontToBack))
		{
			SpriteCmpTexDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}
	else
	{
		if (sortFlags.TestFlag(SpriteSortMode::DepthBackToFront))
		{
			SpriteCmpDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (sortFlags.TestFlag(SpriteSortMode::DepthFrontToBack))
		{
			SpriteCmpDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}

	int si = 0;     // Sprite Index (m_spriteIndexList)
	int vi = 0;     // Vertex Index
	int ri = 0;     // request Index (m_spriteRequestList)
	int start_idx = 0;
	int prim_num = 0;
	Driver::ITexture* current_tex = m_spriteRequestList[m_spriteIndexList[0]].Texture;
	int currnetRenderStateIndex = m_spriteRequestList[m_spriteIndexList[0]].RenderStateIndex;

	//-----------------------------------------------------
	// 属性リストを作る

	m_attributeList.clear();
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
		m_attributeList.add(attr);

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

	BatchSpriteVertex* vb = static_cast< BatchSpriteVertex* >(m_vertexBuffer->lock());
	si = 0;
	vi = 0;
	for (; si < spriteCount; ++si)
	{
		ri = m_spriteIndexList[si];
		memcpy(&vb[vi], m_spriteRequestList[ri].Vertices, sizeof(m_spriteRequestList[0].Vertices));
		vi += 4;
	}
	m_vertexBuffer->unlock();

	//-----------------------------------------------------
	// 描画

	auto* r = m_manager->getGraphicsDevice()->getRenderer();
	m_shader.varViewProjMatrix->setMatrix(m_viewProjMatrix);
	if (m_shader.varViewPixelSize != nullptr)
		m_shader.varViewPixelSize->setVector(Vector4(m_viewPixelSize.x, m_viewPixelSize.y, 0, 0));

	Driver::IShaderPass* pass = m_shader.techMainDraw->getPass(0);

	AttributeList::iterator itr = m_attributeList.begin();
	AttributeList::iterator end = m_attributeList.end();
	for (; itr != end; ++itr)
	{
		//r->SetRenderState(m_renderStateList[itr->RenderStateIndex]);
		m_shader.varTexture->setTexture(itr->Texture);
		r->setShaderPass(pass);
		r->setVertexDeclaration(m_vertexDeclaration);
		r->setVertexBuffer(0, m_vertexBuffer);
		r->setIndexBuffer(m_indexBuffer);
		r->drawPrimitiveIndexed(PrimitiveType_TriangleList, itr->StartIndex, itr->PrimitiveNum);
	}

	//-----------------------------------------------------
	// クリーンアップ

	clear();
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::clear()
{
	m_renderStateList.clear();
	m_renderStateList.add(RenderState());
	m_currentRenderStateIndex = 0;
	m_spriteRequestListUsedCount = 0;
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
