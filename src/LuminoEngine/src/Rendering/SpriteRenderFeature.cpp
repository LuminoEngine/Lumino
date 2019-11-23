
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Shader/ShaderInterfaceFramework.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "SpriteRenderFeature.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// InternalSpriteRenderer

InternalSpriteRenderer::InternalSpriteRenderer()
{
}

void InternalSpriteRenderer::init(RenderingManager* manager)
{
	//m_device = manager->graphicsManager()->deviceContext();
	m_buffersReservedSpriteCount = 0;
    m_vertexDeclaration = manager->standardVertexDeclarationRHI();
	
	// reserve buffers.
	m_spriteDataList.reserve(2048);
	prepareBuffers(manager->graphicsManager()->deviceContext(), 2048);
}

void InternalSpriteRenderer::setState(const State& state)
{
	m_state = state;
	m_viewDirection.set(m_state.viewMatrix.m[0][2], m_state.viewMatrix.m[1][2], m_state.viewMatrix.m[2][2]);
	m_viewInverseMatrix = Matrix::makeInverse(m_state.viewMatrix);
	m_viewPosition = m_viewInverseMatrix.position();
}

void InternalSpriteRenderer::drawRequest(
	const Matrix& transform,
	const Vector2& size,
	const Vector2& anchorRatio,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDir,
	BillboardType billboardType,
    SpriteFlipFlags flipFlags)
{
	SpriteData sprite;

	Vector2 center(size.x * anchorRatio.x, size.y * anchorRatio.y);
	Vector3 normal = Vector3::UnitZ;

	// 3D の場合の頂点座標
	if (baseDir != SpriteBaseDirection::Basic2D)
	{
		//Vector3 origin(-center);
        float l, t, r, b;
#if 1   // 原点左下、povot.y+ が↑
        r = size.x;
        b = 0;
        l = 0;
        t = size.y;

        l -= center.x;
        r -= center.x;
        t -= center.y;
        b -= center.y;
#else
        // 原点中央 povot.y+ が↓
		Vector2 harf_size(size * 0.5f);
		r = harf_size.x;
		b = -harf_size.y;
		l = -r;
		t = -b;

        l -= center.x;
        r -= center.x;
        t -= center.y;
        b -= center.y;
#endif

#define LN_WRITE_V3( x_, y_, z_ ) x_, y_, z_

		switch (baseDir)
		{
		case SpriteBaseDirection::XPlus:
			sprite.vertices[0].position.set(LN_WRITE_V3(0, t, l));     // 左上
			sprite.vertices[1].position.set(LN_WRITE_V3(0, t, r));     // 右上
			sprite.vertices[2].position.set(LN_WRITE_V3(0, b, l));     // 左下
			sprite.vertices[3].position.set(LN_WRITE_V3(0, b, r));     // 右下
			normal = Vector3(1, 0, 0);
			break;
		case SpriteBaseDirection::YPlus:
			sprite.vertices[0].position.set(LN_WRITE_V3(l, 0, t));
			sprite.vertices[1].position.set(LN_WRITE_V3(r, 0, t));
			sprite.vertices[2].position.set(LN_WRITE_V3(l, 0, b));
			sprite.vertices[3].position.set(LN_WRITE_V3(r, 0, b));
			normal = Vector3(0, 1, 0);
			break;
		case SpriteBaseDirection::ZPlus:
			sprite.vertices[0].position.set(LN_WRITE_V3(r, t, 0));
			sprite.vertices[1].position.set(LN_WRITE_V3(l, t, 0));
			sprite.vertices[2].position.set(LN_WRITE_V3(r, b, 0));
			sprite.vertices[3].position.set(LN_WRITE_V3(l, b, 0));
			normal = Vector3(0, 0, 1);
			break;
		case SpriteBaseDirection::XMinus:
			sprite.vertices[0].position.set(LN_WRITE_V3(0, t, r));
			sprite.vertices[1].position.set(LN_WRITE_V3(0, t, l));
			sprite.vertices[2].position.set(LN_WRITE_V3(0, b, r));
			sprite.vertices[3].position.set(LN_WRITE_V3(0, b, l));
			normal = Vector3(-1, 0, 0);
			break;
		case SpriteBaseDirection::YMinus:
			sprite.vertices[0].position.set(LN_WRITE_V3(r, 0, t));
			sprite.vertices[1].position.set(LN_WRITE_V3(l, 0, t));
			sprite.vertices[2].position.set(LN_WRITE_V3(r, 0, b));
			sprite.vertices[3].position.set(LN_WRITE_V3(l, 0, b));
			normal = Vector3(0, -1, 0);
			break;
		case SpriteBaseDirection::ZMinus:
			sprite.vertices[0].position.set(LN_WRITE_V3(l, t, 0));
			sprite.vertices[1].position.set(LN_WRITE_V3(r, t, 0));
			sprite.vertices[2].position.set(LN_WRITE_V3(l, b, 0));
			sprite.vertices[3].position.set(LN_WRITE_V3(r, b, 0));
			normal = Vector3(0, 0, -1);
			break;
		}
#undef LN_WRITE_V3
	}
	// 2D の場合の頂点座標
	else
	{
		Vector2 origin(-center);
		sprite.vertices[0].position.set(origin.x, origin.y, 0);
		sprite.vertices[1].position.set(origin.x + size.x, origin.y, 0);
		sprite.vertices[2].position.set(origin.x, origin.y + size.y, 0);
		sprite.vertices[3].position.set(origin.x + size.x, origin.y + size.y, 0);
		normal = Vector3(0, 0, -1);
	}

	for (int i = 0; i < 4; i++)
		sprite.vertices[i].normal = normal;

	const Vector3& worldPoint = transform.position();

	Matrix actualTransform;
	{

		// ビルボード
		if (billboardType == BillboardType::ToCameraPoint)
		{
			Vector3 f = Vector3::normalize(m_viewPosition - worldPoint);
			Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
			Vector3 u = Vector3::cross(f, r);
			actualTransform = Matrix(
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
			actualTransform = Matrix(
				r.x, r.y, r.z, 0.0f,
				u.x, u.y, u.z, 0.0f,
				f.x, f.y, f.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}
		// ビルボード・Y 軸のみに適用
		else if (billboardType == BillboardType::RotY)
		{
			LN_NOTIMPLEMENTED();

			//if (m_viewDirection.x > 0.0f)
			//{
			//	rotMat.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
			//}
			//else if (m_viewDirection.x == 0.0f)
			//{
			//	//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
			//}
			//else
			//{
			//	rotMat.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
			//}
		}
		// ビルボードではない
		else
		{
			actualTransform = transform.getRotationMatrix();
		}

		actualTransform.translate(worldPoint);
	}

	// 座標変換
	sprite.vertices[0].position.transformCoord(actualTransform);
	sprite.vertices[1].position.transformCoord(actualTransform);
	sprite.vertices[2].position.transformCoord(actualTransform);
	sprite.vertices[3].position.transformCoord(actualTransform);

	// 色
	sprite.vertices[0].color = color;
	sprite.vertices[1].color = color;
	sprite.vertices[2].color = color;
	sprite.vertices[3].color = color;

	// テクスチャ
	{
        float l = srcRect.x;
        float t = srcRect.y;
        float r = (srcRect.x + srcRect.width);
        float b = (srcRect.y + srcRect.height);

        if (testFlag(flipFlags, SpriteFlipFlags::FlipX)) {
            std::swap(l, r);
        }
        if (testFlag(flipFlags, SpriteFlipFlags::FlipY)) {
            std::swap(t, b);
        }

        sprite.vertices[0].uv.x = l;
        sprite.vertices[0].uv.y = t;
        sprite.vertices[1].uv.x = r;
        sprite.vertices[1].uv.y = t;
        sprite.vertices[2].uv.x = l;
        sprite.vertices[2].uv.y = b;
        sprite.vertices[3].uv.x = r;
        sprite.vertices[3].uv.y = b;
	}

	// カメラからの距離をソート用Z値にする場合
	if (m_state.sortingBasis == SortingDistanceBasis::ViewPont) {
		sprite.depth = (m_viewPosition - worldPoint).lengthSquared();
	}
	else {
		sprite.depth = worldPoint.z;
	}

	m_spriteDataList.add(sprite);
}

// Z 値の大きい方から小さい方へソートする比較
class InternalSpriteRenderer::SpriteCmpDepthBackToFront
{
public:
	List<SpriteData>* spriteList;

	bool operator()(int l_, int r_)
	{
		const SpriteData& lsp = spriteList->at(l_);
		const SpriteData& rsp = spriteList->at(r_);

		if (lsp.priority == rsp.priority)
		{
			return lsp.depth > rsp.depth;
		}
		return lsp.priority < rsp.priority;
	}
};

// Z 値の小さい方から大きい方へソートする比較
class InternalSpriteRenderer::SpriteCmpDepthFrontToBack
{
public:
	List<SpriteData>* spriteList;

	bool operator()(int l_, int r_)
	{
		const SpriteData& lsp = spriteList->at(l_);
		const SpriteData& rsp = spriteList->at(r_);

		if (lsp.priority == rsp.priority)
		{
			return lsp.depth < rsp.depth;
		}
		return lsp.priority < rsp.priority;
	}
};

void InternalSpriteRenderer::flush(ICommandList* context)
{
	int spriteCount = m_spriteDataList.size();
	if (spriteCount == 0) {
		return;
	}

	// Allocate vertex buffer and index buffer, if needed.
	prepareBuffers(context->device(), m_spriteDataList.size());

	// Initialize sprite index list.
	for (int i = 0; i < spriteCount; ++i) {
		m_spriteIndexList[i] = i;
	}

	// Sort of sprite index.
	{
		if (m_state.sortMode == SpriteSortMode::DepthBackToFront)
		{
			SpriteCmpDepthBackToFront cmp;
			cmp.spriteList = &m_spriteDataList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (m_state.sortMode == SpriteSortMode::DepthFrontToBack)
		{
			SpriteCmpDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteDataList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}

	// Copy vertex data.
	Vertex* vb = static_cast<Vertex*>(context->map(m_vertexBuffer, 0, m_vertexBuffer->getBytesSize()));
	for (int iSprite = 0, iVertex = 0; iSprite < spriteCount; iSprite++)
	{
		int iData = m_spriteIndexList[iSprite];
		memcpy(&vb[iVertex], m_spriteDataList[iData].vertices, sizeof(SpriteData::vertices));
		iVertex += 4;
	}
    context->unmap(m_vertexBuffer);

	// Render
	context->setVertexDeclaration(m_vertexDeclaration);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->setPrimitiveTopology(PrimitiveTopology::TriangleList);
	context->drawPrimitiveIndexed(0, spriteCount * 2);

	// Cleanup
	clear();
}

void InternalSpriteRenderer::clear()
{
	m_spriteDataList.clear();
}

void InternalSpriteRenderer::prepareBuffers(IGraphicsDevice* context, int spriteCount)
{
	if (m_buffersReservedSpriteCount < spriteCount)
	{
		size_t vertexCount = spriteCount * 4;
		if (LN_ENSURE(vertexCount < 0xFFFF)) {
			return;
		}

		size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
		m_vertexBuffer = context->createVertexBuffer(GraphicsResourceUsage::Dynamic, vertexBufferSize, nullptr);

		size_t indexBufferSize = spriteCount * 6;
		std::vector<size_t> indexBuf(sizeof(uint16_t) * indexBufferSize, false);
		uint16_t* ib = (uint16_t*)indexBuf.data();
		int idx = 0;
		int i2 = 0;
		for (int i = 0; i < spriteCount; ++i)
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
		m_indexBuffer = context->createIndexBuffer(
			GraphicsResourceUsage::Dynamic, IndexBufferFormat::UInt16,
			spriteCount * 6, ib);

		m_spriteIndexList.resize(spriteCount);

		m_buffersReservedSpriteCount = spriteCount;
	}
}

//==============================================================================
// SpriteRenderFeature

SpriteRenderFeature::SpriteRenderFeature()
	: m_manager(nullptr)
	, m_state()
	, m_internal()
{
}

void SpriteRenderFeature::init(RenderingManager* manager)
{
	RenderFeature::init();
	m_manager = manager;
	m_internal = makeRef<InternalSpriteRenderer>();
	m_internal->init(manager);
    m_stateChanged = true;
}

void SpriteRenderFeature::setSortInfo(
	SpriteSortMode sortMode,
	SortingDistanceBasis sortingBasis)
{
	m_state.sortMode = sortMode;
	m_state.sortingBasis = sortingBasis;
}

void SpriteRenderFeature::drawRequest(
	GraphicsContext* context,
	const Matrix& transform,
	const Vector2& size,
	const Vector2& anchorRatio,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType,
    SpriteFlipFlags flipFlags)
{
    // onActiveRenderFeatureChanged では GraphicsContext に触れないので、state はここで送る。
    // （onActiveRenderFeatureChanged の呼び出し元で渡すようにしてもいいが、ひとまずこれで）
    if (m_stateChanged) {
        LN_ENQUEUE_RENDER_COMMAND_2(
            SpriteRenderFeature_setState, context,
            InternalSpriteRenderer*, m_internal,
            InternalSpriteRenderer::State, m_state,
            {
                m_internal->setState(m_state);
            });
        m_stateChanged = false;
    }

    Vector4 sizeAndAnchor(size.x, size.y, anchorRatio.x, anchorRatio.y);
	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderFeature_drawRequest, context,
		InternalSpriteRenderer*, m_internal,
		Matrix, transform,
        Vector4, sizeAndAnchor,
		Rect, srcRect,
		Color, color,
		SpriteBaseDirection, baseDirection,
		BillboardType, billboardType,
        SpriteFlipFlags, flipFlags,
		{
			m_internal->drawRequest(transform, Vector2(sizeAndAnchor.x, sizeAndAnchor.y), Vector2(sizeAndAnchor.z, sizeAndAnchor.w), srcRect, color, baseDirection, billboardType, flipFlags);
		});
}

void SpriteRenderFeature::onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo)
{
    // TODO: CameraInfo は SceneRenderer 開始時点で確定しているので、RenderStage 変更ごとに設定する必要はない。少し最適化できそう。

    m_state.viewMatrix = mainCameraInfo.viewMatrix;
    m_state.projMatrix = mainCameraInfo.projMatrix;
    m_stateChanged = true;

}

void SpriteRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	GraphicsManager* manager = m_manager->graphicsManager();
	ICommandList* c = GraphicsContextInternal::commitState(context);
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderFeature_flush, context,
		ICommandList*, c,
		InternalSpriteRenderer*, m_internal,
		{
			m_internal->flush(c);
		});

	// TODO:
}

void SpriteRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	LN_NOTIMPLEMENTED();
}

void SpriteRenderFeature::makeRenderSizeAndSourceRectHelper(Texture* texture, const Size& size, const Rect& sourceRect, Size* outSize, Rect* outSourceRect)
{
    LN_DCHECK(outSize);
    LN_DCHECK(outSourceRect);

    // 転送元矩形が負値ならテクスチャ全体を転送する
    //const SizeI& texSize = (texture != nullptr) ? texture->size() : SizeI::Zero;
	SizeI texSize(
		(texture != nullptr) ? texture->width() : 0,
		(texture != nullptr) ? texture->height() : 0);
    Rect renderSourceRect = sourceRect;
    if (renderSourceRect.width < 0 && renderSourceRect.height < 0)
    {
        renderSourceRect.width = texSize.width;
        renderSourceRect.height = texSize.height;
    }
    Size renderSize = size;
    if (renderSize.width < 0 && renderSize.height < 0)
    {
        renderSize.width = renderSourceRect.width;
        renderSize.height = renderSourceRect.height;
    }

    renderSourceRect.x /= texSize.width;
    renderSourceRect.width /= texSize.width;
    renderSourceRect.y /= texSize.height;
    renderSourceRect.height /= texSize.height;

    *outSize = renderSize;
    *outSourceRect = renderSourceRect;
}




//==============================================================================
// SpriteRenderFeature

SpriteRenderFeature2::SpriteRenderFeature2()
	: m_buffersReservedSpriteCount(0)
	, m_mappedVertices(nullptr)
{
}

void SpriteRenderFeature2::init(RenderingManager* manager)
{
	RenderFeature::init();
	m_manager = manager;
	m_vertexLayout = manager->standardVertexDeclaration();
	prepareBuffers(nullptr, 2048);
	m_batchData.spriteOffset = 0;
	m_batchData.spriteCount = 0;
}

RequestBatchResult SpriteRenderFeature2::drawRequest(
	detail::RenderFeatureBatchList* batchList,
	GraphicsContext* context,
	const Matrix& transform,
	const Vector2& size,
	const Vector2& anchorRatio,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType,
	SpriteFlipFlags flipFlags)
{
	// TODO: buffer おおきくする

	m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
	auto* vertices = m_mappedVertices + ((m_batchData.spriteOffset + m_batchData.spriteCount) * 4);

	Vector2 center(size.x * anchorRatio.x, size.y * anchorRatio.y);
	Vector3 normal = Vector3::UnitZ;

	// 3D の場合の頂点座標
	if (baseDirection != SpriteBaseDirection::Basic2D)
	{
		//Vector3 origin(-center);
		float l, t, r, b;
#if 1   // 原点左下、povot.y+ が↑
		r = size.x;
		b = 0;
		l = 0;
		t = size.y;

		l -= center.x;
		r -= center.x;
		t -= center.y;
		b -= center.y;
#else
		// 原点中央 povot.y+ が↓
		Vector2 harf_size(size * 0.5f);
		r = harf_size.x;
		b = -harf_size.y;
		l = -r;
		t = -b;

		l -= center.x;
		r -= center.x;
		t -= center.y;
		b -= center.y;
#endif

#define LN_WRITE_V3( x_, y_, z_ ) x_, y_, z_

		switch (baseDirection)
		{
		case SpriteBaseDirection::XPlus:
			vertices[0].position.set(LN_WRITE_V3(0, t, l));     // 左上
			vertices[1].position.set(LN_WRITE_V3(0, t, r));     // 右上
			vertices[2].position.set(LN_WRITE_V3(0, b, l));     // 左下
			vertices[3].position.set(LN_WRITE_V3(0, b, r));     // 右下
			normal = Vector3(1, 0, 0);
			break;
		case SpriteBaseDirection::YPlus:
			vertices[0].position.set(LN_WRITE_V3(l, 0, t));
			vertices[1].position.set(LN_WRITE_V3(r, 0, t));
			vertices[2].position.set(LN_WRITE_V3(l, 0, b));
			vertices[3].position.set(LN_WRITE_V3(r, 0, b));
			normal = Vector3(0, 1, 0);
			break;
		case SpriteBaseDirection::ZPlus:
			vertices[0].position.set(LN_WRITE_V3(r, t, 0));
			vertices[1].position.set(LN_WRITE_V3(l, t, 0));
			vertices[2].position.set(LN_WRITE_V3(r, b, 0));
			vertices[3].position.set(LN_WRITE_V3(l, b, 0));
			normal = Vector3(0, 0, 1);
			break;
		case SpriteBaseDirection::XMinus:
			vertices[0].position.set(LN_WRITE_V3(0, t, r));
			vertices[1].position.set(LN_WRITE_V3(0, t, l));
			vertices[2].position.set(LN_WRITE_V3(0, b, r));
			vertices[3].position.set(LN_WRITE_V3(0, b, l));
			normal = Vector3(-1, 0, 0);
			break;
		case SpriteBaseDirection::YMinus:
			vertices[0].position.set(LN_WRITE_V3(r, 0, t));
			vertices[1].position.set(LN_WRITE_V3(l, 0, t));
			vertices[2].position.set(LN_WRITE_V3(r, 0, b));
			vertices[3].position.set(LN_WRITE_V3(l, 0, b));
			normal = Vector3(0, -1, 0);
			break;
		case SpriteBaseDirection::ZMinus:
			vertices[0].position.set(LN_WRITE_V3(l, t, 0));
			vertices[1].position.set(LN_WRITE_V3(r, t, 0));
			vertices[2].position.set(LN_WRITE_V3(l, b, 0));
			vertices[3].position.set(LN_WRITE_V3(r, b, 0));
			normal = Vector3(0, 0, -1);
			break;
		}
#undef LN_WRITE_V3
	}
	// 2D の場合の頂点座標
	else
	{
		Vector2 origin(-center);
		vertices[0].position.set(origin.x, origin.y, 0);
		vertices[1].position.set(origin.x + size.x, origin.y, 0);
		vertices[2].position.set(origin.x, origin.y + size.y, 0);
		vertices[3].position.set(origin.x + size.x, origin.y + size.y, 0);
		normal = Vector3(0, 0, -1);
	}

	for (int i = 0; i < 4; i++)
		vertices[i].normal = normal;

	const Vector3& worldPoint = transform.position();

	Matrix actualTransform;
	{

		// ビルボード
		if (billboardType == BillboardType::ToCameraPoint)
		{
			Vector3 f = Vector3::normalize(m_viewPosition - worldPoint);
			Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
			Vector3 u = Vector3::cross(f, r);
			actualTransform = Matrix(
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
			actualTransform = Matrix(
				r.x, r.y, r.z, 0.0f,
				u.x, u.y, u.z, 0.0f,
				f.x, f.y, f.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}
		// ビルボード・Y 軸のみに適用
		else if (billboardType == BillboardType::RotY)
		{
			LN_NOTIMPLEMENTED();

			//if (m_viewDirection.x > 0.0f)
			//{
			//	rotMat.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
			//}
			//else if (m_viewDirection.x == 0.0f)
			//{
			//	//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
			//}
			//else
			//{
			//	rotMat.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
			//}
		}
		// ビルボードではない
		else
		{
			actualTransform = transform.getRotationMatrix();
		}

		actualTransform.translate(worldPoint);
	}

	// 座標変換
	vertices[0].position.transformCoord(actualTransform);
	vertices[1].position.transformCoord(actualTransform);
	vertices[2].position.transformCoord(actualTransform);
	vertices[3].position.transformCoord(actualTransform);

	// 色
	vertices[0].color = color;
	vertices[1].color = color;
	vertices[2].color = color;
	vertices[3].color = color;

	// テクスチャ
	{
		float l = srcRect.x;
		float t = srcRect.y;
		float r = (srcRect.x + srcRect.width);
		float b = (srcRect.y + srcRect.height);

		if (testFlag(flipFlags, SpriteFlipFlags::FlipX)) {
			std::swap(l, r);
		}
		if (testFlag(flipFlags, SpriteFlipFlags::FlipY)) {
			std::swap(t, b);
		}

		vertices[0].uv.x = l;
		vertices[0].uv.y = t;
		vertices[1].uv.x = r;
		vertices[1].uv.y = t;
		vertices[2].uv.x = l;
		vertices[2].uv.y = b;
		vertices[3].uv.x = r;
		vertices[3].uv.y = b;
	}

	m_batchData.spriteCount++;
	return RequestBatchResult::Staging;
}

void SpriteRenderFeature2::onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo)
{
	m_viewMatrix = mainCameraInfo.viewMatrix;
	m_projMatrix = mainCameraInfo.projMatrix;
	m_viewDirection.set(m_viewMatrix.m[0][2], m_viewMatrix.m[1][2], m_viewMatrix.m[2][2]);
	m_viewInverseMatrix = Matrix::makeInverse(m_viewMatrix);
	m_viewPosition = m_viewInverseMatrix.position();
}

void SpriteRenderFeature2::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	if (m_mappedVertices) {
		// TODO: unmap (今は自動だけど、明示した方が安心かも)
	}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.spriteOffset = m_batchData.spriteOffset + m_batchData.spriteCount;
	m_batchData.spriteCount = 0;
}

void SpriteRenderFeature2::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.spriteOffset * 6, localBatch->data.spriteCount * 2);

	m_batchData.spriteOffset = 0;
	m_batchData.spriteCount = 0;
}

void SpriteRenderFeature2::prepareBuffers(GraphicsContext* context, int spriteCount)
{
	if (context) {
		// TODO: 実行中の map は context->map 用意した方がいいかも
		LN_NOTIMPLEMENTED();
	}

	if (m_buffersReservedSpriteCount < spriteCount)
	{
		size_t vertexCount = spriteCount * 4;
		if (LN_ENSURE(vertexCount < 0xFFFF)) {
			return;
		}

		// VertexBuffer
		size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
		if (!m_vertexBuffer)
			m_vertexBuffer = makeObject<VertexBuffer>(vertexBufferSize, GraphicsResourceUsage::Dynamic);
		else
			m_vertexBuffer->resize(vertexBufferSize);

		// IndexBuffer
		size_t indexBufferSize = spriteCount * 6;
		if (!m_indexBuffer)
			m_indexBuffer = makeObject<IndexBuffer>(indexBufferSize, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
		else
			m_indexBuffer->resize(indexBufferSize);
		auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));	// TODO: 部分 map
		int idx = 0;
		int i2 = 0;
		for (int i = 0; i < spriteCount; ++i)
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

		m_buffersReservedSpriteCount = spriteCount;
	}
}

} // namespace detail
} // namespace ln

