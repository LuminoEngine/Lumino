
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "FrameRectRenderFeature.hpp"

namespace ln {
namespace detail {

#ifdef LN_RENDERING_MIGRATION

//==============================================================================
// FrameRectRenderFeature

FrameRectRenderFeature::FrameRectRenderFeature()
{
}

void FrameRectRenderFeature::init(RenderingManager* manager)
{
	RenderFeature::init();
}

RequestBatchResult FrameRectRenderFeature::drawRequest(GraphicsContext* context, const Rect& rect, const Matrix& worldTransform, BrushImageDrawMode imageDrawMode, const Thickness& borderThickness, const Rect& srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize)
{
	if (rect.isEmpty()) return RequestBatchResult::Staging;

	// 枠
	{
		// TODO: thickness が left しか対応できていない
		putFrameRectangle(context, rect, borderThickness, srcRect, wrapMode, srcTextureSize);
	}

	// Inner
	if (imageDrawMode == BrushImageDrawMode::BoxFrame)
	{
		Rect dstRect = rect;
		dstRect.x += borderThickness.left;
		dstRect.y += borderThickness.top;
		dstRect.width -= borderThickness.width();
		dstRect.height -= borderThickness.height();

		Rect innterSrcRect = srcRect;
		innterSrcRect.x += borderThickness.left;
		innterSrcRect.y += borderThickness.top;
		innterSrcRect.width -= borderThickness.width();
		innterSrcRect.height -= borderThickness.height();

		Size texSize((float)srcTextureSize.width, (float)srcTextureSize.height);
		texSize.width = 1.0f / texSize.width;
		texSize.height = 1.0f / texSize.height;
		Rect uvSrcRect(innterSrcRect.x * texSize.width, innterSrcRect.y * texSize.height, innterSrcRect.width * texSize.width, innterSrcRect.height * texSize.height);

		putRectangle(context, dstRect, innterSrcRect, uvSrcRect, wrapMode);
	}


	return RequestBatchResult::Staging;
}

// TODO: SpriteRenderer 共通
void FrameRectRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	if (m_mappedVertices) {
		// TODO: unmap (今は自動だけど、明示した方が安心かも)
	}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.spriteOffset = m_batchData.spriteOffset + m_batchData.spriteCount;
	m_batchData.spriteCount = 0;
}

// TODO: SpriteRenderer 共通
void FrameRectRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.spriteOffset * 6, localBatch->data.spriteCount * 2);

	// TODO: ここじゃなくて begin か end で
	m_batchData.spriteOffset = 0;
	m_batchData.spriteCount = 0;
}

void FrameRectRenderFeature::prepareBuffers(GraphicsContext* context, int spriteCount)
{
	// TODO: 実行中の map は context->map 用意した方がいいかも
//if (context) {
//	LN_NOTIMPLEMENTED();
//}

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

void FrameRectRenderFeature::addSprite(GraphicsContext* context, const Vector3& pos0, const Vector2& uv0, const Vector3& pos1, const Vector2& uv1, const Vector3& pos2, const Vector2& uv2, const Vector3& pos3, const Vector2& uv3)
{
	int count = m_batchData.spriteOffset + m_batchData.spriteCount + 1;
	prepareBuffers(context, count);

	m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
	auto* vertices = m_mappedVertices + ((m_batchData.spriteOffset + m_batchData.spriteCount) * 4);

	vertices[0].position = pos0;
	vertices[0].uv = uv0;
	vertices[0].normal = Vector3::UnitZ;
	vertices[0].color = Color::White;
	vertices[1].position = pos1;
	vertices[1].uv = uv1;
	vertices[1].normal = Vector3::UnitZ;
	vertices[1].color = Color::White;
	vertices[2].position = pos2;
	vertices[2].uv = uv2;
	vertices[2].normal = Vector3::UnitZ;
	vertices[2].color = Color::White;
	vertices[3].position = pos3;
	vertices[3].uv = uv3;
	vertices[3].normal = Vector3::UnitZ;
	vertices[3].color = Color::White;

	m_batchData.spriteCount++;
}

void FrameRectRenderFeature::putRectangleStretch(GraphicsContext* context, const Rect& rect, const Rect& srcUVRect)
{
	if (rect.isEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	float pos_l = rect.getLeft();
	float pos_r = rect.getRight();
	float pos_t = rect.getTop();
	float pos_b = rect.getBottom();
	float uv_l = srcUVRect.getLeft();
	float uv_r = srcUVRect.getRight();
	float uv_t = srcUVRect.getTop();
	float uv_b = srcUVRect.getBottom();

	addSprite(
		context,
		Vector3(pos_l, pos_t, 0), Vector2(uv_l, uv_t),	// top-left
		Vector3(pos_r, pos_t, 0), Vector2(uv_r, uv_t),	// top-right
		Vector3(pos_l, pos_b, 0), Vector2(uv_l, uv_b),	// bottom-left
		Vector3(pos_r, pos_b, 0), Vector2(uv_r, uv_b));	// bottom-right
}

void FrameRectRenderFeature::putRectangleTiling(GraphicsContext* context, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect)
{
	if (rect.isEmpty()) return;		// 矩形がつぶれているので書く必要はない

	int blockCountW = std::ceil(rect.width / srcPixelRect.width);		// 横方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
	int blockCountH = std::ceil(rect.height / srcPixelRect.height);	// 縦方向にいくつのタイルを並べられるか (0.5 など、端数も含む)

	float bw = (float)srcPixelRect.width;
	float bh = (float)srcPixelRect.height;

	ln::Vertex v;
	v.color.set(1, 1, 1, 1);
	v.normal.set(0, 0, 1);

	float uv_l = srcUVRect.x;
	float uv_t = srcUVRect.y;

	for (int y = 0; y < blockCountH; y++)
	{
		float pos_t = bh * y;
		float pos_b = pos_t + bh;
		float uv_b = srcUVRect.getBottom();
		if (pos_b > rect.height)
		{
			float ratio = (1.0f - (pos_b - rect.height) / bh);
			pos_b = rect.height;
			uv_b = srcUVRect.y + srcUVRect.height * ratio;
		}

		for (int x = 0; x < blockCountW; x++)
		{
			float pos_l = bw * x;
			float pos_r = pos_l + bw;
			float uv_r = srcUVRect.getRight();
			if (pos_r > rect.width)
			{
				float ratio = (1.0f - (pos_r - rect.width) / bw);
				pos_r = rect.width;
				uv_r = srcUVRect.x + srcUVRect.width * ratio;
			}

			addSprite(
				context,
				Vector3(rect.x + pos_l, rect.y + pos_t, 0), Vector2(uv_l, uv_t),	// top-left
				Vector3(rect.x + pos_r, rect.y + pos_t, 0), Vector2(uv_r, uv_t),	// top-right
				Vector3(rect.x + pos_l, rect.y + pos_b, 0), Vector2(uv_l, uv_b),	// bottom-left
				Vector3(rect.x + pos_r, rect.y + pos_b, 0), Vector2(uv_r, uv_b));	// bottom-right
		}
	}
}

void FrameRectRenderFeature::putRectangle(GraphicsContext* context, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, BrushWrapMode wrapMode)
{
	if (wrapMode == BrushWrapMode::Stretch)
	{
		putRectangleStretch(context, rect, srcUVRect);
	}
	else if (wrapMode == BrushWrapMode::Tile)
	{
		putRectangleTiling(context, rect, srcPixelRect, srcUVRect);
	}
}

void FrameRectRenderFeature::putFrameRectangle(GraphicsContext* context, const Rect& rect, const Thickness& borderThickness, Rect srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize)
{
	if (srcRect.isEmpty()) return;
	if (srcTextureSize.width == 0) return;
	if (srcTextureSize.height == 0) return;

	if (srcRect.width == INT_MAX) {
		srcRect.width = srcTextureSize.width;
	}
	if (srcRect.height == INT_MAX) {
		srcRect.height = srcTextureSize.height;
	}

	Size texSize((float)srcTextureSize.width, (float)srcTextureSize.height);
	texSize.width = 1.0f / texSize.width;
	texSize.height = 1.0f / texSize.height;
	Rect uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

	Thickness dstFrame;
	Thickness uvFrame;
	ThicknessI srcFrame;
	{
		Thickness baseThickness = borderThickness;

		// 横幅が小さいため、枠幅も狭めなければならない
		if (rect.width < baseThickness.left + baseThickness.right)
		{
			baseThickness.left = rect.width / 2;
			baseThickness.right = rect.width / 2;
		}
		// 縦幅が小さいため、枠幅も狭めなければならない
		if (rect.height < baseThickness.top + baseThickness.bottom)
		{
			baseThickness.top = rect.height / 2;
			baseThickness.bottom = rect.height / 2;
		}

		dstFrame.left = baseThickness.left;
		dstFrame.right = baseThickness.right;
		dstFrame.top = baseThickness.top;
		dstFrame.bottom = baseThickness.bottom;
		uvFrame.left = baseThickness.left * texSize.width;
		uvFrame.right = baseThickness.right * texSize.width;
		uvFrame.top = baseThickness.top * texSize.height;
		uvFrame.bottom = baseThickness.bottom * texSize.height;
		srcFrame.left = (int)baseThickness.left;	// 型変換回数を減らすため、あらかじめ int 化しておく
		srcFrame.right = (int)baseThickness.right;
		srcFrame.top = (int)baseThickness.top;
		srcFrame.bottom = (int)baseThickness.bottom;
	}

	Rect outerRect = rect;
	Rect innerRect(outerRect.x + dstFrame.left, outerRect.y + dstFrame.top, outerRect.width - (dstFrame.left + dstFrame.right), outerRect.height - (dstFrame.top + dstFrame.bottom));
	Rect outerUVRect = uvSrcRect;
	Rect innerUVRect(outerUVRect.x + uvFrame.left, outerUVRect.y + uvFrame.top, outerUVRect.width - (uvFrame.left + uvFrame.right), outerUVRect.height - (uvFrame.top + uvFrame.bottom));
	Rect  outerSrcRect = srcRect;
	Rect  innerSrcRect(outerSrcRect.x + srcFrame.left, outerSrcRect.y + srcFrame.top, outerSrcRect.width - (srcFrame.left + srcFrame.right), outerSrcRect.height - (srcFrame.top + srcFrame.bottom));

	// 左上	■□□
	//		□　□
	//		□□□
	putRectangle(
		context,
		Rect(outerRect.getLeft(), outerRect.getTop(), dstFrame.left, dstFrame.top),
		Rect(outerSrcRect.getLeft(), outerSrcRect.getTop(), srcFrame.left, srcFrame.top),
		Rect(outerUVRect.getLeft(), outerUVRect.getTop(), uvFrame.left, uvFrame.top),
		wrapMode);

	// 上	□■□
	//		□　□
	//		□□□
	putRectangle(
		context,
		Rect(innerRect.getLeft(), outerRect.getTop(), innerRect.width, dstFrame.top),
		Rect(innerSrcRect.getLeft(), outerSrcRect.getTop(), innerSrcRect.width, srcFrame.top),
		Rect(innerUVRect.getLeft(), outerUVRect.getTop(), innerUVRect.width, uvFrame.top),
		wrapMode);

	// 右上	□□■
	//		□　□
	//		□□□
	putRectangle(
		context,
		Rect(innerRect.getRight(), outerRect.getTop(), dstFrame.right, dstFrame.top),
		Rect(innerSrcRect.getRight(), outerSrcRect.getTop(), srcFrame.right, srcFrame.top),
		Rect(innerUVRect.getRight(), outerUVRect.getTop(), uvFrame.right, uvFrame.top),
		wrapMode);

	// 右	□□□
	//		□　■
	//		□□□
	putRectangle(
		context,
		Rect(innerRect.getRight(), innerRect.getTop(), dstFrame.right, innerRect.height),
		Rect(innerSrcRect.getRight(), innerSrcRect.getTop(), srcFrame.right, innerSrcRect.height),
		Rect(innerUVRect.getRight(), innerUVRect.getTop(), uvFrame.right, innerUVRect.height),
		wrapMode);

	// 右下	□□□
	//		□　□
	//		□□■
	putRectangle(
		context,
		Rect(innerRect.getRight(), innerRect.getBottom(), dstFrame.right, dstFrame.bottom),
		Rect(innerSrcRect.getRight(), innerSrcRect.getBottom(), srcFrame.right, srcFrame.bottom),
		Rect(innerUVRect.getRight(), innerUVRect.getBottom(), uvFrame.right, uvFrame.bottom),
		wrapMode);

	// 下	□□□
	//		□　□
	//		□■□
	putRectangle(
		context,
		Rect(innerRect.getLeft(), innerRect.getBottom(), innerRect.width, dstFrame.bottom),
		Rect(innerSrcRect.getLeft(), innerSrcRect.getBottom(), innerSrcRect.width, srcFrame.bottom),
		Rect(innerUVRect.getLeft(), innerUVRect.getBottom(), innerUVRect.width, uvFrame.bottom),
		wrapMode);

	// 左下	□□□
	//		□　□
	//		■□□
	putRectangle(
		context,
		Rect(outerRect.getLeft(), innerRect.getBottom(), dstFrame.left, dstFrame.bottom),
		Rect(outerSrcRect.getLeft(), innerSrcRect.getBottom(), srcFrame.left, srcFrame.bottom),
		Rect(outerUVRect.getLeft(), innerUVRect.getBottom(), uvFrame.left, uvFrame.bottom),
		wrapMode);

	// 左	□□□
	//		■　□
	//		□□□
	putRectangle(
		context,
		Rect(outerRect.getLeft(), innerRect.getTop(), dstFrame.left, innerRect.height),
		Rect(outerSrcRect.getLeft(), innerSrcRect.getTop(), srcFrame.left, innerSrcRect.height),
		Rect(outerUVRect.getLeft(), innerUVRect.getTop(), uvFrame.left, innerUVRect.height),
		wrapMode);
}

#else
//==============================================================================
// InternalFrameRectRenderer

InternalFrameRectRenderer::InternalFrameRectRenderer()
	: m_manager(nullptr)
	, m_vertexDeclaration(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
    , m_buffersReservedSpriteCount(0)
{
}

void InternalFrameRectRenderer::init(RenderingManager* manager)
{
    m_manager = manager;
    m_vertexDeclaration = m_manager->standardVertexDeclarationRHI();

    m_vertexAllocator = makeRef<LinearAllocator>(m_manager->graphicsManager()->linearAllocatorPageManager());
    prepareBuffers(512);
}

void InternalFrameRectRenderer::draw(const Rect& rect, const Matrix& worldTransform, BrushImageDrawMode imageDrawMode, const Thickness& borderThickness, const Rect& srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize)
{
    if (rect.isEmpty()) return;

    // 枠
    {
        // TODO: thickness が left しか対応できていない
        putFrameRectangle(rect, borderThickness, srcRect, wrapMode, srcTextureSize);
    }

    // Inner
    if (imageDrawMode == BrushImageDrawMode::BoxFrame)
    {
        Rect dstRect = rect;
        dstRect.x += borderThickness.left;
        dstRect.y += borderThickness.top;
        dstRect.width -= borderThickness.width();
        dstRect.height -= borderThickness.height();

        Rect innterSrcRect = srcRect;
        innterSrcRect.x += borderThickness.left;
        innterSrcRect.y += borderThickness.top;
        innterSrcRect.width -= borderThickness.width();
        innterSrcRect.height -= borderThickness.height();

        Size texSize((float)srcTextureSize.width, (float)srcTextureSize.height);
        texSize.width = 1.0f / texSize.width;
        texSize.height = 1.0f / texSize.height;
        Rect uvSrcRect(innterSrcRect.x * texSize.width, innterSrcRect.y * texSize.height, innterSrcRect.width * texSize.width, innterSrcRect.height * texSize.height);

        putRectangle(dstRect, innterSrcRect, uvSrcRect, wrapMode);
    }
}

void InternalFrameRectRenderer::InternalFrameRectRenderer::flush(ICommandList* context)
{
    prepareBuffers(m_vertices.size() / 4);

    Vertex* vb = reinterpret_cast<Vertex*>(context->map(m_vertexBuffer, 0, m_vertexBuffer->getBytesSize()));
    for (size_t i = 0; i < m_vertices.size(); i++) {
        vb[i] = *m_vertices[i];
    }
    context->unmap(m_vertexBuffer);

    context->setVertexDeclaration(m_vertexDeclaration);
    context->setVertexBuffer(0, m_vertexBuffer);
    context->setIndexBuffer(m_indexBuffer);
    context->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    context->drawPrimitiveIndexed(0, m_vertices.size() / 2);

    m_vertexAllocator->cleanup();
    m_vertices.clear();
}

// TODO: TextRenderer と全く同じなので共通化していいかも
void InternalFrameRectRenderer::prepareBuffers(int spriteCount)
{
	IGraphicsDevice* context = m_manager->graphicsManager()->deviceContext();

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

		m_buffersReservedSpriteCount = spriteCount;
	}
}

void InternalFrameRectRenderer::addVertex(const Vector3& pos, const Vector2& uv)
{
    Vertex* v = (Vertex*)m_vertexAllocator->allocate(sizeof(Vertex));
    v->position = pos;
    v->uv = uv;
    v->normal = Vector3(0, 0, 1);
    v->color = Color::White;
    m_vertices.add(v);
}

void InternalFrameRectRenderer::putRectangleStretch(const Rect& rect, const Rect& srcUVRect)
{
    if (rect.isEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

    //uint16_t i = m_vertexCache.getCount();
    //m_indexCache.add(i + 0);
    //m_indexCache.add(i + 1);
    //m_indexCache.add(i + 2);
    //m_indexCache.add(i + 2);
    //m_indexCache.add(i + 1);
    //m_indexCache.add(i + 3);

    float pos_l = rect.getLeft();
    float pos_r = rect.getRight();
    float pos_t = rect.getTop();
    float pos_b = rect.getBottom();
    float uv_l = srcUVRect.getLeft();
    float uv_r = srcUVRect.getRight();
    float uv_t = srcUVRect.getTop();
    float uv_b = srcUVRect.getBottom();

    addVertex(Vector3(pos_l, pos_t, 0), Vector2(uv_l, uv_t));   // top-left
    addVertex(Vector3(pos_r, pos_t, 0), Vector2(uv_r, uv_t));   // top-right
    addVertex(Vector3(pos_l, pos_b, 0), Vector2(uv_l, uv_b));   // bottom-left
    addVertex(Vector3(pos_r, pos_b, 0), Vector2(uv_r, uv_b));   // bottom-right
}

void InternalFrameRectRenderer::putRectangleTiling(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect)
{
    if (rect.isEmpty()) return;		// 矩形がつぶれているので書く必要はない

    int blockCountW = std::ceil(rect.width / srcPixelRect.width);		// 横方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
    int blockCountH = std::ceil(rect.height / srcPixelRect.height);	// 縦方向にいくつのタイルを並べられるか (0.5 など、端数も含む)

    float bw = (float)srcPixelRect.width;
    float bh = (float)srcPixelRect.height;

    ln::Vertex v;
    v.color.set(1, 1, 1, 1);
    v.normal.set(0, 0, 1);

    float uv_l = srcUVRect.x;
    float uv_t = srcUVRect.y;

    for (int y = 0; y < blockCountH; y++)
    {
        float pos_t = bh * y;
        float pos_b = pos_t + bh;
        float uv_b = srcUVRect.getBottom();
        if (pos_b > rect.height)
        {
            float ratio = (1.0f - (pos_b - rect.height) / bh);
            pos_b = rect.height;
            uv_b = srcUVRect.y + srcUVRect.height * ratio;
        }

        for (int x = 0; x < blockCountW; x++)
        {
            float pos_l = bw * x;
            float pos_r = pos_l + bw;
            float uv_r = srcUVRect.getRight();
            if (pos_r > rect.width)
            {
                float ratio = (1.0f - (pos_r - rect.width) / bw);
                pos_r = rect.width;
                uv_r = srcUVRect.x + srcUVRect.width * ratio;
            }

            //uint16_t i = m_vertexCache.getCount();
            //m_indexCache.add(i + 0);
            //m_indexCache.add(i + 1);
            //m_indexCache.add(i + 2);
            //m_indexCache.add(i + 2);
            //m_indexCache.add(i + 1);
            //m_indexCache.add(i + 3);

            addVertex(Vector3(rect.x + pos_l, rect.y + pos_t, 0), Vector2(uv_l, uv_t));   // top-left
            addVertex(Vector3(rect.x + pos_r, rect.y + pos_t, 0), Vector2(uv_r, uv_t));   // top-right
            addVertex(Vector3(rect.x + pos_l, rect.y + pos_b, 0), Vector2(uv_l, uv_b));   // bottom-left
            addVertex(Vector3(rect.x + pos_r, rect.y + pos_b, 0), Vector2(uv_r, uv_b));   // bottom-right
        }
    }
}

void InternalFrameRectRenderer::putRectangle(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, BrushWrapMode wrapMode)
{
    if (wrapMode == BrushWrapMode::Stretch)
    {
        putRectangleStretch(rect, srcUVRect);
    }
    else if (wrapMode == BrushWrapMode::Tile)
    {
        putRectangleTiling(rect, srcPixelRect, srcUVRect);
    }
}

void InternalFrameRectRenderer::putFrameRectangle(const Rect& rect, const Thickness& borderThickness, Rect srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize)
{
    if (srcRect.isEmpty()) return;
    if (srcTextureSize.width == 0) return;
    if (srcTextureSize.height == 0) return;

    if (srcRect.width == INT_MAX) {
        srcRect.width = srcTextureSize.width;
    }
    if (srcRect.height == INT_MAX) {
        srcRect.height = srcTextureSize.height;
    }

    Size texSize((float)srcTextureSize.width, (float)srcTextureSize.height);
    texSize.width = 1.0f / texSize.width;
    texSize.height = 1.0f / texSize.height;
    Rect uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

    Thickness dstFrame;
    Thickness uvFrame;
    ThicknessI srcFrame;
    {
        Thickness baseThickness = borderThickness;

        // 横幅が小さいため、枠幅も狭めなければならない
        if (rect.width < baseThickness.left + baseThickness.right)
        {
            baseThickness.left = rect.width / 2;
            baseThickness.right = rect.width / 2;
        }
        // 縦幅が小さいため、枠幅も狭めなければならない
        if (rect.height < baseThickness.top + baseThickness.bottom)
        {
            baseThickness.top = rect.height / 2;
            baseThickness.bottom = rect.height / 2;
        }

        dstFrame.left = baseThickness.left;
        dstFrame.right = baseThickness.right;
        dstFrame.top = baseThickness.top;
        dstFrame.bottom = baseThickness.bottom;
        uvFrame.left = baseThickness.left * texSize.width;
        uvFrame.right = baseThickness.right * texSize.width;
        uvFrame.top = baseThickness.top * texSize.height;
        uvFrame.bottom = baseThickness.bottom * texSize.height;
        srcFrame.left = (int)baseThickness.left;	// 型変換回数を減らすため、あらかじめ int 化しておく
        srcFrame.right = (int)baseThickness.right;
        srcFrame.top = (int)baseThickness.top;
        srcFrame.bottom = (int)baseThickness.bottom;
    }

    Rect outerRect = rect;
    Rect innerRect(outerRect.x + dstFrame.left, outerRect.y + dstFrame.top, outerRect.width - (dstFrame.left + dstFrame.right), outerRect.height - (dstFrame.top + dstFrame.bottom));
    Rect outerUVRect = uvSrcRect;
    Rect innerUVRect(outerUVRect.x + uvFrame.left, outerUVRect.y + uvFrame.top, outerUVRect.width - (uvFrame.left + uvFrame.right), outerUVRect.height - (uvFrame.top + uvFrame.bottom));
    Rect  outerSrcRect = srcRect;
    Rect  innerSrcRect(outerSrcRect.x + srcFrame.left, outerSrcRect.y + srcFrame.top, outerSrcRect.width - (srcFrame.left + srcFrame.right), outerSrcRect.height - (srcFrame.top + srcFrame.bottom));

    // 左上	■□□
    //		□　□
    //		□□□
    putRectangle(
        Rect(outerRect.getLeft(), outerRect.getTop(), dstFrame.left, dstFrame.top),
        Rect(outerSrcRect.getLeft(), outerSrcRect.getTop(), srcFrame.left, srcFrame.top),
        Rect(outerUVRect.getLeft(), outerUVRect.getTop(), uvFrame.left, uvFrame.top),
        wrapMode);

    // 上	□■□
    //		□　□
    //		□□□
    putRectangle(
        Rect(innerRect.getLeft(), outerRect.getTop(), innerRect.width, dstFrame.top),
        Rect(innerSrcRect.getLeft(), outerSrcRect.getTop(), innerSrcRect.width, srcFrame.top),
        Rect(innerUVRect.getLeft(), outerUVRect.getTop(), innerUVRect.width, uvFrame.top),
        wrapMode);

    // 右上	□□■
    //		□　□
    //		□□□
    putRectangle(
        Rect(innerRect.getRight(), outerRect.getTop(), dstFrame.right, dstFrame.top),
        Rect(innerSrcRect.getRight(), outerSrcRect.getTop(), srcFrame.right, srcFrame.top),
        Rect(innerUVRect.getRight(), outerUVRect.getTop(), uvFrame.right, uvFrame.top),
        wrapMode);

    // 右	□□□
    //		□　■
    //		□□□
    putRectangle(
        Rect(innerRect.getRight(), innerRect.getTop(), dstFrame.right, innerRect.height),
        Rect(innerSrcRect.getRight(), innerSrcRect.getTop(), srcFrame.right, innerSrcRect.height),
        Rect(innerUVRect.getRight(), innerUVRect.getTop(), uvFrame.right, innerUVRect.height),
        wrapMode);

    // 右下	□□□
    //		□　□
    //		□□■
    putRectangle(
        Rect(innerRect.getRight(), innerRect.getBottom(), dstFrame.right, dstFrame.bottom),
        Rect(innerSrcRect.getRight(), innerSrcRect.getBottom(), srcFrame.right, srcFrame.bottom),
        Rect(innerUVRect.getRight(), innerUVRect.getBottom(), uvFrame.right, uvFrame.bottom),
        wrapMode);

    // 下	□□□
    //		□　□
    //		□■□
    putRectangle(
        Rect(innerRect.getLeft(), innerRect.getBottom(), innerRect.width, dstFrame.bottom),
        Rect(innerSrcRect.getLeft(), innerSrcRect.getBottom(), innerSrcRect.width, srcFrame.bottom),
        Rect(innerUVRect.getLeft(), innerUVRect.getBottom(), innerUVRect.width, uvFrame.bottom),
        wrapMode);

    // 左下	□□□
    //		□　□
    //		■□□
    putRectangle(
        Rect(outerRect.getLeft(), innerRect.getBottom(), dstFrame.left, dstFrame.bottom),
        Rect(outerSrcRect.getLeft(), innerSrcRect.getBottom(), srcFrame.left, srcFrame.bottom),
        Rect(outerUVRect.getLeft(), innerUVRect.getBottom(), uvFrame.left, uvFrame.bottom),
        wrapMode);

    // 左	□□□
    //		■　□
    //		□□□
    putRectangle(
        Rect(outerRect.getLeft(), innerRect.getTop(), dstFrame.left, innerRect.height),
        Rect(outerSrcRect.getLeft(), innerSrcRect.getTop(), srcFrame.left, innerSrcRect.height),
        Rect(outerUVRect.getLeft(), innerUVRect.getTop(), uvFrame.left, innerUVRect.height),
        wrapMode);
}

//==============================================================================
// FrameRectRenderFeature

FrameRectRenderFeature::FrameRectRenderFeature()
	: m_internal(nullptr)
{
}

void FrameRectRenderFeature::init(RenderingManager* manager)
{
    RenderFeature::init();
    m_internal = makeRef<InternalFrameRectRenderer>();
    m_internal->init(manager);
}

void FrameRectRenderFeature::draw(GraphicsContext* context, const Rect& rect, const Matrix& worldTransform, BrushImageDrawMode imageDrawMode, const Thickness& borderThickness, const Rect& srcRect, BrushWrapMode wrapMode, const SizeI& srcTextureSize)
{
    LN_ENQUEUE_RENDER_COMMAND_8(
        rameRectRenderFeature_draw, context,
        InternalFrameRectRenderer*, m_internal,
        Rect, rect,
        Matrix, worldTransform,
        BrushImageDrawMode, imageDrawMode,
        Thickness, borderThickness,
        Rect, srcRect,
        BrushWrapMode, wrapMode,
        SizeI, srcTextureSize,
        {
            m_internal->draw(rect, worldTransform, imageDrawMode, borderThickness, srcRect, wrapMode, srcTextureSize);
        });
}

void FrameRectRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	GraphicsManager* manager = m_internal->manager()->graphicsManager();
	ICommandList* c = GraphicsContextInternal::commitState(context);
	LN_ENQUEUE_RENDER_COMMAND_2(
		FrameRectRenderFeature_flush, context,
		InternalFrameRectRenderer*, m_internal,
		ICommandList*, c,
		{
			m_internal->flush(c);
		});

	// TODO: add batch
}

void FrameRectRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	LN_NOTIMPLEMENTED();
}
#endif

} // namespace detail
} // namespace ln

