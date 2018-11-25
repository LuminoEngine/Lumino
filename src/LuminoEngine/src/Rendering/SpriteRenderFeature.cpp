
#include "Internal.hpp"
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

void InternalSpriteRenderer::initialize(RenderingManager* manager)
{
	//m_device = manager->graphicsManager()->deviceContext();
	m_buffersReservedSpriteCount = 0;
	m_vertexDeclaration = manager->standardVertexDeclaration()->resolveRHIObject();
	
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
    const Size& textureRealSize)
{
	SpriteData sprite;

	Vector2 center(size.x * anchorRatio.x, size.y * anchorRatio.y);
	Vector3 normal = Vector3::UnitZ;

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
	if (Math::nearEqual(textureRealSize.width, 0) || Math::nearEqual(textureRealSize.height, 0))
	{
		sprite.vertices[0].uv.x = 0;
		sprite.vertices[0].uv.y = 0;
		sprite.vertices[1].uv.x = 1;
		sprite.vertices[1].uv.y = 0;
		sprite.vertices[2].uv.x = 0;
		sprite.vertices[2].uv.y = 1;
		sprite.vertices[3].uv.x = 1;
		sprite.vertices[3].uv.y = 1;
	}
	else
	{
		//Vector2 texSizeInv(1.0f / textureRealSize.width, 1.0f / textureRealSize.height);
		Rect sr(srcRect);
		//float l = sr.x * texSizeInv.x;
		//float t = sr.y * texSizeInv.y;
		//float r = (sr.x + sr.width) * texSizeInv.x;
		//float b = (sr.y + sr.height) * texSizeInv.y;
        float l = sr.x;
        float t = sr.y;
        float r = (sr.x + sr.width);
        float b = (sr.y + sr.height);
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

void InternalSpriteRenderer::flush(IGraphicsDeviceContext* context)
{
	int spriteCount = m_spriteDataList.size();
	if (spriteCount == 0) {
		return;
	}

	// Allocate vertex buffer and index buffer, if needed.
	prepareBuffers(context, m_spriteDataList.size());

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
	Vertex* vb = static_cast<Vertex*>(m_vertexBuffer->map());
	for (int iSprite = 0, iVertex = 0; iSprite < spriteCount; iSprite++)
	{
		int iData = m_spriteIndexList[iSprite];
		memcpy(&vb[iVertex], m_spriteDataList[iData].vertices, sizeof(SpriteData::vertices));
		iVertex += 4;
	}
	m_vertexBuffer->unmap();

	// Render
	context->setVertexDeclaration(m_vertexDeclaration);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(PrimitiveType::TriangleList, 0, spriteCount * 2);

	// Cleanup
	clear();
}

void InternalSpriteRenderer::clear()
{
	m_spriteDataList.clear();
}

void InternalSpriteRenderer::prepareBuffers(IGraphicsDeviceContext* context, int spriteCount)
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

void SpriteRenderFeature::initialize(RenderingManager* manager)
{
	RenderFeature::initialize();
	m_manager = manager;
	m_internal = makeRef<InternalSpriteRenderer>();
	m_internal->initialize(manager);
}

void SpriteRenderFeature::setSortInfo(
	SpriteSortMode sortMode,
	SortingDistanceBasis sortingBasis)
{
	m_state.sortMode = sortMode;
	m_state.sortingBasis = sortingBasis;
}

void SpriteRenderFeature::drawRequest(
	const Matrix& transform,
	const Vector2& size,
	const Vector2& anchorRatio,
	const Rect& srcRect,
	const Color& color,
	SpriteBaseDirection baseDirection,
	BillboardType billboardType,
    const Size& textureRealSize)
{
	GraphicsManager* manager = m_manager->graphicsManager();
    Vector4 sizeAndTextureRealSize(size.x, size.y, textureRealSize.width, textureRealSize.height);
	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderFeature_drawRequest, manager,
		InternalSpriteRenderer*, m_internal,
		Matrix, transform,
		Vector4, sizeAndTextureRealSize,
		Vector2, anchorRatio,
		Rect, srcRect,
		Color, color,
		SpriteBaseDirection, baseDirection,
		BillboardType, billboardType,
		{
			m_internal->drawRequest(transform, sizeAndTextureRealSize.xy(), anchorRatio, srcRect, color, baseDirection, billboardType, Size(sizeAndTextureRealSize.z, sizeAndTextureRealSize.w));
		});
}

void SpriteRenderFeature::onActiveRenderFeatureChanged(const detail::CameraInfo& mainCameraInfo)
{
    // TODO: CameraInfo は SceneRenderer 開始時点で確定しているので、RenderStage 変更ごとに設定する必要はない。少し最適化できそう。

    m_state.viewMatrix = mainCameraInfo.viewMatrix;
    m_state.projMatrix = mainCameraInfo.projMatrix;

    GraphicsManager* manager = m_manager->graphicsManager();
    LN_ENQUEUE_RENDER_COMMAND_2(
        SpriteRenderFeature_setState, manager,
        InternalSpriteRenderer*, m_internal,
        InternalSpriteRenderer::State, m_state,
        {
            m_internal->setState(m_state);
        });
}

void SpriteRenderFeature::flush(GraphicsContext* context)
{
	GraphicsManager* manager = m_manager->graphicsManager();
	IGraphicsDeviceContext* deviceContext = context->commitState();
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderFeature_flush, manager,
		IGraphicsDeviceContext*, deviceContext,
		InternalSpriteRenderer*, m_internal,
		{
			m_internal->flush(deviceContext);
		});
}

void SpriteRenderFeature::makeRenderSizeAndSourceRectHelper(Texture* texture, const Size& size, const Rect& sourceRect, Size* outSize, Rect* outSourceRect)
{
    LN_DCHECK(outSize);
    LN_DCHECK(outSourceRect);

    // 転送元矩形が負値ならテクスチャ全体を転送する
    const SizeI& texSize = (texture != nullptr) ? texture->size() : SizeI::Zero;
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

} // namespace detail
} // namespace ln

