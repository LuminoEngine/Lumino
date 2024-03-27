
#include "Internal.hpp"
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/GPU/IndexBuffer.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/GPU/ShaderInterfaceFramework.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "SpriteRenderFeature.hpp"

namespace ln {
namespace detail {

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
	//m_batchData.spriteOffset = 0;
	//m_batchData.spriteCount = 0;
	m_spriteCount = 0;
}

RequestBatchResult SpriteRenderFeature2::drawRequest(
	RenderFeatureBatchList* batchList,
	const RLIBatchState& batchState,
	GraphicsCommandList* context,
	const Matrix& transform,
	const Vector2& size,
	const Vector2& anchorRatio,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType,
	SpriteFlipFlags flipFlags)
{
	Batch* batch;
	if (batchList->equalsLastBatchState(this, batchState)) {
		batch = static_cast<Batch*>(batchList->lastBatch());
	}
	else {
		batch = batchList->addNewBatch<Batch>(this, batchState);
		batch->data.spriteOffset = m_spriteCount;
		batch->data.spriteCount = 0;
		batch->finalMaterial()->m_worldTransform = nullptr;	// VertexShade での World 座標変換は不要
	}


	// TODO: buffer おおきくする

	m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->writableData());
	//auto* vertices = m_mappedVertices + ((batch->data.spriteOffset + batch->data.spriteCount) * 4);
	auto* vertices = m_mappedVertices + (m_spriteCount * 4);

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
#ifdef LN_COORD_RH
		case SpriteBaseDirection::XPlus:
			vertices[0].setPosition(LN_WRITE_V3(0, t, l));     // 左上
			vertices[1].setPosition(LN_WRITE_V3(0, b, l));     // 左下
			vertices[2].setPosition(LN_WRITE_V3(0, t, r));     // 右上
			vertices[3].setPosition(LN_WRITE_V3(0, b, r));     // 右下
			normal = Vector3(1, 0, 0);
			break;
		case SpriteBaseDirection::YPlus:
			vertices[0].setPosition(LN_WRITE_V3(l, 0, t));
			vertices[1].setPosition(LN_WRITE_V3(l, 0, b));
			vertices[2].setPosition(LN_WRITE_V3(r, 0, t));
			vertices[3].setPosition(LN_WRITE_V3(r, 0, b));
			normal = Vector3(0, 1, 0);
			break;
		case SpriteBaseDirection::ZPlus:
			vertices[0].setPosition(LN_WRITE_V3(l, t, 0));
			vertices[1].setPosition(LN_WRITE_V3(l, b, 0));
			vertices[2].setPosition(LN_WRITE_V3(r, t, 0));
			vertices[3].setPosition(LN_WRITE_V3(r, b, 0));
			normal = Vector3(0, 0, 1);
			break;
		case SpriteBaseDirection::XMinus:
			vertices[0].setPosition(LN_WRITE_V3(0, t, r));
			vertices[1].setPosition(LN_WRITE_V3(0, b, r));
			vertices[2].setPosition(LN_WRITE_V3(0, t, l));
			vertices[3].setPosition(LN_WRITE_V3(0, b, l));
			normal = Vector3(-1, 0, 0);
			break;
		case SpriteBaseDirection::YMinus:
			vertices[0].setPosition(LN_WRITE_V3(r, 0, t));
			vertices[1].setPosition(LN_WRITE_V3(r, 0, b));
			vertices[2].setPosition(LN_WRITE_V3(l, 0, t));
			vertices[3].setPosition(LN_WRITE_V3(l, 0, b));
			normal = Vector3(0, -1, 0);
			break;
		case SpriteBaseDirection::ZMinus:
			vertices[0].setPosition(LN_WRITE_V3(l, t, 0));
			vertices[1].setPosition(LN_WRITE_V3(l, b, 0));
			vertices[2].setPosition(LN_WRITE_V3(r, t, 0));
			vertices[3].setPosition(LN_WRITE_V3(r, b, 0));
			normal = Vector3(0, 0, -1);
			break;
		}
#else
		case SpriteBaseDirection::XPlus:
			vertices[0].setPosition(LN_WRITE_V3(0, t, l));     // 左上
			vertices[1].setPosition(LN_WRITE_V3(0, t, r));     // 右上
			vertices[2].setPosition(LN_WRITE_V3(0, b, l));     // 左下
			vertices[3].setPosition(LN_WRITE_V3(0, b, r));     // 右下
			normal = Vector3(1, 0, 0);
			break;
		case SpriteBaseDirection::YPlus:
			vertices[0].setPosition(LN_WRITE_V3(l, 0, t));
			vertices[1].setPosition(LN_WRITE_V3(r, 0, t));
			vertices[2].setPosition(LN_WRITE_V3(l, 0, b));
			vertices[3].setPosition(LN_WRITE_V3(r, 0, b));
			normal = Vector3(0, 1, 0);
			break;
		case SpriteBaseDirection::ZPlus:
			vertices[0].setPosition(LN_WRITE_V3(r, t, 0));
			vertices[1].setPosition(LN_WRITE_V3(l, t, 0));
			vertices[2].setPosition(LN_WRITE_V3(r, b, 0));
			vertices[3].setPosition(LN_WRITE_V3(l, b, 0));
			normal = Vector3(0, 0, 1);
			break;
		case SpriteBaseDirection::XMinus:
			vertices[0].setPosition(LN_WRITE_V3(0, t, r));
			vertices[1].setPosition(LN_WRITE_V3(0, t, l));
			vertices[2].setPosition(LN_WRITE_V3(0, b, r));
			vertices[3].setPosition(LN_WRITE_V3(0, b, l));
			normal = Vector3(-1, 0, 0);
			break;
		case SpriteBaseDirection::YMinus:
			vertices[0].setPosition(LN_WRITE_V3(r, 0, t));
			vertices[1].setPosition(LN_WRITE_V3(l, 0, t));
			vertices[2].setPosition(LN_WRITE_V3(r, 0, b));
			vertices[3].setPosition(LN_WRITE_V3(l, 0, b));
			normal = Vector3(0, -1, 0);
			break;
		case SpriteBaseDirection::ZMinus:
			vertices[0].setPosition(LN_WRITE_V3(l, t, 0));
			vertices[1].setPosition(LN_WRITE_V3(r, t, 0));
			vertices[2].setPosition(LN_WRITE_V3(l, b, 0));
			vertices[3].setPosition(LN_WRITE_V3(r, b, 0));
			normal = Vector3(0, 0, -1);
			break;
		}
#endif
#undef LN_WRITE_V3
	}
	// 2D の場合の頂点座標
	else
	{
#ifdef LN_COORD_RH
		Vector2 origin(-center);
		vertices[0].setPosition(origin.x, origin.y, 0);
		vertices[1].setPosition(origin.x, origin.y + size.y, 0);
		vertices[2].setPosition(origin.x + size.x, origin.y, 0);
		vertices[3].setPosition(origin.x + size.x, origin.y + size.y, 0);
#else
		Vector2 origin(-center);
		vertices[0].setPosition(origin.x, origin.y, 0);
		vertices[1].setPosition(origin.x + size.x, origin.y, 0);
		vertices[2].setPosition(origin.x, origin.y + size.y, 0);
		vertices[3].setPosition(origin.x + size.x, origin.y + size.y, 0);
#endif
		normal = Vector3(0, 0, -1);
	}

	for (int i = 0; i < 4; i++)
		vertices[i].setNormal(normal);

	const Vector3& worldPoint = transform.position();

	Matrix actualTransform;
	{
		// ビルボード
		if (billboardType == BillboardType::ToCameraPoint)
		{
			Vector3 f = Vector3::normalize(batchList->m_mainCameraInfo->viewPosition - worldPoint);
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
			float d = Vector3::dot(worldPoint - batchList->m_mainCameraInfo->viewPosition, batchList->m_mainCameraInfo->viewDirection);

			// left-hand coord
			Vector3 f = Vector3::normalize(batchList->m_mainCameraInfo->viewDirection * d);
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
			auto posDiff = Vector3(worldPoint - batchList->m_mainCameraInfo->viewPosition);
			auto dir = batchList->m_mainCameraInfo->viewDirection;
			posDiff.y = 0.0f;
			dir.y = 0.0f;

			// ビュー平面との距離
			float d = Vector3::dot(posDiff, dir);

			// left-hand coord
			Vector3 f = Vector3::normalize(dir * d);
			Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
			Vector3 u = Vector3::cross(f, r);
			actualTransform = Matrix(
				r.x, r.y, r.z, 0.0f,
				u.x, u.y, u.z, 0.0f,
				f.x, f.y, f.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
			//LN_NOTIMPLEMENTED();

			//if (m_viewDirection.x > 0.0f)
			//{
			//	actualTransform.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
			//}
			//else if (m_viewDirection.x == 0.0f)
			//{
			//	//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
			//}
			//else
			//{
			//	actualTransform.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
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
	vertices[0].transformPosition(actualTransform);
	vertices[1].transformPosition(actualTransform);
	vertices[2].transformPosition(actualTransform);
	vertices[3].transformPosition(actualTransform);

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

#ifdef LN_COORD_RH
		vertices[0].uv.x = l;
		vertices[0].uv.y = t;
		vertices[1].uv.x = l;
		vertices[1].uv.y = b;
		vertices[2].uv.x = r;
		vertices[2].uv.y = t;
		vertices[3].uv.x = r;
		vertices[3].uv.y = b;
#else
		vertices[0].uv.x = l;
		vertices[0].uv.y = t;
		vertices[1].uv.x = r;
		vertices[1].uv.y = t;
		vertices[2].uv.x = l;
		vertices[2].uv.y = b;
		vertices[3].uv.x = r;
		vertices[3].uv.y = b;
#endif
	}

	m_spriteCount++;
	batch->data.spriteCount++;
	return RequestBatchResult::Staging;
}

void SpriteRenderFeature2::onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo)
{
	m_viewMatrix = mainCameraInfo.viewMatrix;
	m_projMatrix = mainCameraInfo.projMatrix;
	//m_viewDirection.set(m_viewMatrix.m[0][2], m_viewMatrix.m[1][2], m_viewMatrix.m[2][2]);
	//m_viewInverseMatrix = Matrix::makeInverse(m_viewMatrix);
	//m_viewPosition = m_viewInverseMatrix.position();
}

void SpriteRenderFeature2::submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList)
{
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

void SpriteRenderFeature2::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);
	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.spriteOffset * 6, localBatch->data.spriteCount * 2);

	//m_batchData.spriteOffset = 0;
	//m_batchData.spriteCount = 0;
	m_spriteCount = 0;
}

void SpriteRenderFeature2::makeRenderSizeAndSourceRectHelper(Texture* texture, const Size& size, const Rect& sourceRect, Size* outSize, Rect* outSourceRect)
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

void SpriteRenderFeature2::prepareBuffers(GraphicsCommandList* context, int spriteCount)
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
			ib[i2 + 0] = idx + 0;
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

