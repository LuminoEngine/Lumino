
#include "Internal.hpp"
#include <LuminoEngine/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/GPU/VertexLayout.hpp>
#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/GPU/IndexBuffer.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "FrameRectRenderFeature.hpp"

namespace ln {
namespace detail {

//==============================================================================
// FrameRectRenderFeature

FrameRectRenderFeature::FrameRectRenderFeature()
    : m_buffersReservedSpriteCount(0)
    , m_worldTransform(nullptr)
{
}

void FrameRectRenderFeature::init(RenderingManager* manager)
{
	RenderFeature::init();
    m_vertexLayout = manager->standardVertexDeclaration();
}

RequestBatchResult FrameRectRenderFeature::drawRequest(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState,
	const Rect& rect,
	const Matrix& worldTransform,
	const Thickness& borderThickness,
	const Rect& srcRect,
	Sprite9DrawMode wrapMode,
	const SizeI& srcTextureSize)
{
	Batch* batch;
	if (batchList->equalsLastBatchState(this, batchState)) {
		batch = static_cast<Batch*>(batchList->lastBatch());
	}
	else {
		batch = batchList->addNewBatch<Batch>(this, batchState);
		batch->data.spriteOffset = 0;
		batch->data.spriteCount = 0;
		batch->finalMaterial()->m_worldTransform = nullptr;	// VertexShade での World 座標変換は不要
	}


	if (rect.isEmpty()) return RequestBatchResult::Staging;
    m_worldTransform = &worldTransform;

	if (wrapMode == Sprite9DrawMode::StretchedSingleImage) {
		Size rec(1.0f / srcTextureSize.width, 1.0f / srcTextureSize.height);
		Rect uvRect(srcRect.x * rec.width, srcRect.y * rec.height, srcRect.width * rec.width, srcRect.height * rec.height);
		putRectangleStretch(batch, rect, uvRect);
	}
	else if (wrapMode == Sprite9DrawMode::RepeatedSingleImage) {
		Size rec(1.0f / srcTextureSize.width, 1.0f / srcTextureSize.height);
		Rect uvRect(srcRect.x * rec.width, srcRect.y * rec.height, srcRect.width * rec.width, srcRect.height * rec.height);
		putRectangleTiling(batch, rect, srcRect, uvRect);
	}
	else if (
		wrapMode == Sprite9DrawMode::StretchedBoxFrame ||
		wrapMode == Sprite9DrawMode::RepeatedBoxFrame ||
		wrapMode == Sprite9DrawMode::StretchedBorderFrame ||
		wrapMode == Sprite9DrawMode::RepeatedBorderFrame) {
		// 枠
		{
			// TODO: thickness が left しか対応できていない
			putFrameRectangle(batch, rect, borderThickness, srcRect, wrapMode, srcTextureSize);
		}

		// Inner
		if (wrapMode == Sprite9DrawMode::RepeatedBoxFrame ||
			wrapMode == Sprite9DrawMode::StretchedBoxFrame) {
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

			putRectangle(batch, dstRect, innterSrcRect, uvSrcRect, wrapMode);
		}

	}


	return RequestBatchResult::Staging;
}

void FrameRectRenderFeature::beginRendering()
{
}

void FrameRectRenderFeature::submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) {
#ifdef LN_RLI_BATCH
	LN_UNREACHABLE();
#else
	if (m_mappedVertices) {
		// TODO: unmap (今は自動だけど、明示した方が安心かも)
	}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.spriteOffset = m_batchData.spriteOffset + m_batchData.spriteCount;
	m_batchData.spriteCount = 0;
#endif
}

void FrameRectRenderFeature::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch) {
	auto localBatch = static_cast<Batch*>(batch);
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.spriteOffset * 6, localBatch->data.spriteCount * 2);
}

void FrameRectRenderFeature::prepareBuffers(int spriteCount)
{
	if (m_buffersReservedSpriteCount < spriteCount)
	{
		size_t vertexCount = spriteCount * 4;
		if (LN_ENSURE(vertexCount < 0xFFFF)) {
			return;
		}

		// VertexBuffer
		size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
		if (!m_vertexBuffer)
			m_vertexBuffer = makeObject_deprecated<VertexBuffer>(vertexBufferSize, GraphicsResourceUsage::Dynamic);
		else
			m_vertexBuffer->resize(vertexBufferSize);

		// IndexBuffer
		size_t indexBufferSize = spriteCount * 6;
		if (!m_indexBuffer)
			m_indexBuffer = makeObject_deprecated<IndexBuffer>(indexBufferSize, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
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

void FrameRectRenderFeature::addSprite(Batch* batch, const Vector3& pos0, const Vector2& uv0, const Vector3& pos1, const Vector2& uv1, const Vector3& pos2, const Vector2& uv2, const Vector3& pos3, const Vector2& uv3)
{
	int count = batch->data.spriteOffset + batch->data.spriteCount + 1;
	prepareBuffers(count);

	//uint64_t lockOffset = m_batchData.spriteOffset * 4;
	//uint64_t lockSize = (m_batchData.spriteCount + 1) * 4;
	//m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
	uint64_t offset = ((batch->data.spriteOffset + batch->data.spriteCount) * 4);
	m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->writableData(0, offset * sizeof(Vertex)));
	auto* vertices = m_mappedVertices + offset;
	vertices[0].set(pos0, Vector3::UnitZ, uv0, Color::White);
	vertices[1].set(pos1, Vector3::UnitZ, uv1, Color::White);
	vertices[2].set(pos2, Vector3::UnitZ, uv2, Color::White);
	vertices[3].set(pos3, Vector3::UnitZ, uv3, Color::White);

    // transform
    vertices[0].transformPosition(*m_worldTransform);
    vertices[1].transformPosition(*m_worldTransform);
    vertices[2].transformPosition(*m_worldTransform);
    vertices[3].transformPosition(*m_worldTransform);

	batch->data.spriteCount++;
}

void FrameRectRenderFeature::putRectangleStretch(Batch* batch, const Rect& rect, const Rect& srcUVRect)
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
		batch,
		Vector3(pos_l, pos_t, 0), Vector2(uv_l, uv_t),	// top-left
		Vector3(pos_l, pos_b, 0), Vector2(uv_l, uv_b),	// bottom-left
		Vector3(pos_r, pos_t, 0), Vector2(uv_r, uv_t),	// top-right
		Vector3(pos_r, pos_b, 0), Vector2(uv_r, uv_b));	// bottom-right
}

void FrameRectRenderFeature::putRectangleTiling(Batch* batch, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect)
{
	if (rect.isEmpty()) return;		// 矩形がつぶれているので書く必要はない

	int blockCountW = std::ceil(rect.width / srcPixelRect.width);		// 横方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
	int blockCountH = std::ceil(rect.height / srcPixelRect.height);	// 縦方向にいくつのタイルを並べられるか (0.5 など、端数も含む)

	float bw = (float)srcPixelRect.width;
	float bh = (float)srcPixelRect.height;

	ln::Vertex v;
	v.color.set(1, 1, 1, 1);
	v.normal.set(0, 0, 1, 0);

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
				batch,
				Vector3(rect.x + pos_l, rect.y + pos_t, 0), Vector2(uv_l, uv_t),	// top-left
				Vector3(rect.x + pos_l, rect.y + pos_b, 0), Vector2(uv_l, uv_b),	// bottom-left
				Vector3(rect.x + pos_r, rect.y + pos_t, 0), Vector2(uv_r, uv_t),	// top-right
				Vector3(rect.x + pos_r, rect.y + pos_b, 0), Vector2(uv_r, uv_b));	// bottom-right
		}
	}
}

void FrameRectRenderFeature::putRectangle(Batch* batch, const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, Sprite9DrawMode wrapMode)
{
	if (wrapMode == Sprite9DrawMode::StretchedBorderFrame || wrapMode == Sprite9DrawMode::StretchedBoxFrame)
	{
		putRectangleStretch(batch, rect, srcUVRect);
	}
	else if (wrapMode == Sprite9DrawMode::RepeatedBorderFrame || wrapMode == Sprite9DrawMode::RepeatedBoxFrame)
	{
		putRectangleTiling(batch, rect, srcPixelRect, srcUVRect);
	}
}

void FrameRectRenderFeature::putFrameRectangle(Batch* batch, const Rect& rect, const Thickness& borderThickness, Rect srcRect, Sprite9DrawMode wrapMode, const SizeI& srcTextureSize)
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
		batch,
		Rect(outerRect.getLeft(), outerRect.getTop(), dstFrame.left, dstFrame.top),
		Rect(outerSrcRect.getLeft(), outerSrcRect.getTop(), srcFrame.left, srcFrame.top),
		Rect(outerUVRect.getLeft(), outerUVRect.getTop(), uvFrame.left, uvFrame.top),
		wrapMode);

	// 上	□■□
	//		□　□
	//		□□□
	putRectangle(
		batch,
		Rect(innerRect.getLeft(), outerRect.getTop(), innerRect.width, dstFrame.top),
		Rect(innerSrcRect.getLeft(), outerSrcRect.getTop(), innerSrcRect.width, srcFrame.top),
		Rect(innerUVRect.getLeft(), outerUVRect.getTop(), innerUVRect.width, uvFrame.top),
		wrapMode);

	// 右上	□□■
	//		□　□
	//		□□□
	putRectangle(
		batch,
		Rect(innerRect.getRight(), outerRect.getTop(), dstFrame.right, dstFrame.top),
		Rect(innerSrcRect.getRight(), outerSrcRect.getTop(), srcFrame.right, srcFrame.top),
		Rect(innerUVRect.getRight(), outerUVRect.getTop(), uvFrame.right, uvFrame.top),
		wrapMode);

	// 右	□□□
	//		□　■
	//		□□□
	putRectangle(
		batch,
		Rect(innerRect.getRight(), innerRect.getTop(), dstFrame.right, innerRect.height),
		Rect(innerSrcRect.getRight(), innerSrcRect.getTop(), srcFrame.right, innerSrcRect.height),
		Rect(innerUVRect.getRight(), innerUVRect.getTop(), uvFrame.right, innerUVRect.height),
		wrapMode);

	// 右下	□□□
	//		□　□
	//		□□■
	putRectangle(
		batch,
		Rect(innerRect.getRight(), innerRect.getBottom(), dstFrame.right, dstFrame.bottom),
		Rect(innerSrcRect.getRight(), innerSrcRect.getBottom(), srcFrame.right, srcFrame.bottom),
		Rect(innerUVRect.getRight(), innerUVRect.getBottom(), uvFrame.right, uvFrame.bottom),
		wrapMode);

	// 下	□□□
	//		□　□
	//		□■□
	putRectangle(
		batch,
		Rect(innerRect.getLeft(), innerRect.getBottom(), innerRect.width, dstFrame.bottom),
		Rect(innerSrcRect.getLeft(), innerSrcRect.getBottom(), innerSrcRect.width, srcFrame.bottom),
		Rect(innerUVRect.getLeft(), innerUVRect.getBottom(), innerUVRect.width, uvFrame.bottom),
		wrapMode);

	// 左下	□□□
	//		□　□
	//		■□□
	putRectangle(
		batch,
		Rect(outerRect.getLeft(), innerRect.getBottom(), dstFrame.left, dstFrame.bottom),
		Rect(outerSrcRect.getLeft(), innerSrcRect.getBottom(), srcFrame.left, srcFrame.bottom),
		Rect(outerUVRect.getLeft(), innerUVRect.getBottom(), uvFrame.left, uvFrame.bottom),
		wrapMode);

	// 左	□□□
	//		■　□
	//		□□□
	putRectangle(
		batch,
		Rect(outerRect.getLeft(), innerRect.getTop(), dstFrame.left, innerRect.height),
		Rect(outerSrcRect.getLeft(), innerSrcRect.getTop(), srcFrame.left, innerSrcRect.height),
		Rect(outerUVRect.getLeft(), innerUVRect.getTop(), uvFrame.left, innerUVRect.height),
		wrapMode);
}

} // namespace detail
} // namespace ln

