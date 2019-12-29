
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Rendering/RenderingManager.hpp"
#include "ShapesRenderFeature.hpp"


/*
全体の流れ
--------------------
1. ベースポイント (m_basePoints) を作る
2. ベースポイントを元に、アウトラインポイント (m_outlinePoints) と Path を作る
3. アウトラインポイントと Path を元に、頂点バッファ (m_vertexCache) とインデックスバッファ (m_indexCache) を作る
構築された各バッファは Flush で描画された後クリアされる。
Flush までの間にたくさんシェイプを書くときは、書くバッファにどんどんたまっていく。

なお、(歴史的な理由で) 上記フローではシェイプを反時計回りをベースに構築する。
Flush で最終的な頂点バッファに書き込むときに、RHI のデフォルトの方向に合わせる。


ベースポイント (m_basePoints) を作る
--------------------
### ベースポイント
描画したいシェイプの明確な境界線を構成する点。

この時点ではアンチエイリアス(AA)やシャドウは考慮しないが、
AAのためにどの方向へ押し出せばよいのか（点ごとに、外側方向はどちらか）は計算しておく。


### BorderComponent
シェイプのエッジを、どこからどこまで上下左右とみなすかどうか。 (Range オブジェクト)
この範囲に色をつける。

四角形の場合
lttttr
l    r
l    r
l    r
l    r
lbbbbr

RoundRectangle の場合
   tttttt
  t      t		← 円弧の途中が境界となる
 l        r
l          r
l          r
l          r
l          r
l          r
l          r
 l        r
  b      b
   bbbbbb


ベースポイントを元に、アウトラインポイント (m_outlinePoints) と Path を作る
--------------------
### アウトラインポイント
線の太さ、AA、シャドウなどの境界線を構成する点。



*/


namespace ln {
namespace detail {

//==============================================================================
// ShapesRendererCommandList

void ShapesRendererCommandList::addCommandNode(ListNode* cmd, CommandType type, const Matrix& transform)
{
	cmd->type = type;
	cmd->next = nullptr;
	if (!head) {
		head = cmd;
	}
	if (tail) {
		tail->next = cmd;
	}
	tail = cmd;
	cmd->transform = transform;
}

void ShapesRendererCommandList::addDrawBoxBackground(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const CornerRadius& cornerRadius, const Color& color)
{
	auto* cmd = reinterpret_cast<DrawBoxBackgroundCommand*>(allocator->allocate(sizeof(DrawBoxBackgroundCommand)));
	addCommandNode(cmd, Cmd_DrawBoxBackground, transform);
	cmd->rect = rect;
	cmd->cornerRadius = cornerRadius;
	cmd->color = color;
}

//void ShapesRendererCommandList::addDrawBoxBorder(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const Thickness& thickness, const CornerRadius& cornerRadius, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const Color& shadowColor, float shadowBlur, float shadowWidth, bool shadowInset, bool borderInset)
//{
//    auto* cmd = reinterpret_cast<DrawBoxBorderCommand*>(allocator->allocate(sizeof(DrawBoxBorderCommand)));
//    cmd->type = Cmd_DrawBoxBorder;
//    cmd->next = nullptr;
//    if (!head) {
//        head = cmd;
//    }
//    if (tail) {
//        tail->next = cmd;
//    }
//    tail = cmd;
//    cmd->transform = transform;
//
//    cmd->rect = rect;
//    cmd->thickness = thickness;
//    cmd->cornerRadius = cornerRadius;
//    cmd->leftColor = leftColor;
//    cmd->topColor = topColor;
//    cmd->rightColor = rightColor;
//    cmd->bottomColor = bottomColor;
//    cmd->shadowColor = shadowColor;
//    cmd->shadowBlur = shadowBlur;
//    cmd->shadowWidth = shadowWidth;
//    cmd->shadowInset = shadowInset;
//    cmd->borderInset = borderInset;
//}

void ShapesRendererCommandList::drawBoxBorderLine(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const Thickness& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset)
{
    auto* cmd = reinterpret_cast<DrawBoxBorderLineCommand*>(allocator->allocate(sizeof(DrawBoxBorderLineCommand)));
	addCommandNode(cmd, Cmd_DrawBoxBorderLine, transform);
    cmd->rect = rect;
    cmd->thickness = thickness;
    cmd->cornerRadius = cornerRadius;
    cmd->leftColor = leftColor;
    cmd->topColor = topColor;
    cmd->rightColor = rightColor;
    cmd->bottomColor = bottomColor;
    cmd->borderInset = borderInset;
}

void ShapesRendererCommandList::addDrawBoxShadow(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const CornerRadius& cornerRadius, const Vector2& offset, const Color& color, float blur, float width, bool inset)
{
	auto* cmd = reinterpret_cast<DrawBoxShadowCommand*>(allocator->allocate(sizeof(DrawBoxShadowCommand)));
	addCommandNode(cmd, Cmd_DrawBoxShadow, transform);
	cmd->rect = rect;
	cmd->cornerRadius = cornerRadius;
	cmd->offset = offset;
	cmd->color = color;
	cmd->blur = blur;
	cmd->width = width;
	cmd->inset = inset;
}

//==============================================================================
// ShapesRenderFeature

static const int g_finalOffset = 0.0;

ShapesRenderFeature::ShapesRenderFeature()
	: m_manager(nullptr)
	, m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_basePoints()
	, m_outlinePoints()
	, m_vertexCache()
	, m_indexCache()
{
}

//------------------------------------------------------------------------------
//ShapesRenderFeature::~ShapesRenderFeature()
//{
//	//LN_SAFE_RELEASE(m_vertexBuffer);
//	//LN_SAFE_RELEASE(m_indexBuffer);
//}

//------------------------------------------------------------------------------
void ShapesRenderFeature::init(RenderingManager* manager)
{
	m_manager = manager;
	m_basePoints.clearAndReserve(4096);
	m_outlinePoints.clearAndReserve(4096);
	m_vertexCache.clearAndReserve(4096);
	m_indexCache.clearAndReserve(4096);
	m_vertexLayout = m_manager->standardVertexDeclaration();
	m_vertexBuffer = makeObject<VertexBuffer>(4096 * sizeof(Vertex), GraphicsResourceUsage::Dynamic);
	m_indexBuffer = makeObject<IndexBuffer>(4096, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
}

//------------------------------------------------------------------------------
RequestBatchResult ShapesRenderFeature::requestDrawCommandList(GraphicsContext* context, ShapesRendererCommandList* commandList/*, detail::BrushRawData* fillBrush*/)
{
	extractBasePoints(commandList);
	calcExtrudedDirection();


	for (int iPath = 0; iPath < m_pathes.size(); iPath++)
	{
		Path& path = m_pathes[iPath];

		//if (path.attribute == PathAttribute::Background)
		//{
		//	path.color *= fillBrush->color;
		//}

		switch (path.type)
		{
		case PathType::Convex:
			expandFill(path);
			break;
		case PathType::Strip2Point:
			expandStrip2PointStroke(path);
			break;
		case PathType::Strip3Point:
			expandStrip3PointStroke(path);
			break;
		}
	}

	//for (int i = 0; i < dataCount; i++)
	//{
	//	Vertex* vb;
	//	uint16_t* ib;
	//	uint16_t beginVertexIndex;
	//	requestBuffers(
	//		cache->getVertexCount(dataList[i].cacheGlyphInfoHandle),
	//		cache->getIndexCount(dataList[i].cacheGlyphInfoHandle),
	//		&vb, &ib, &beginVertexIndex);
	//	cache->generateMesh(
	//		dataList[i].cacheGlyphInfoHandle, Vector3(dataList[i].origin.x, dataList[i].origin.y, 0), dataList[i].transform,
	//		vb, ib, beginVertexIndex);
	//}

	// TODO: このへん PrimitiveRenderFeature と同じ。共通にできないか？
	{
		//Driver::IRenderer* renderer = m_manager->getGraphicsDevice()->getRenderer();

		// サイズが足りなければ再作成
        {
			// VertexBuffer
			int vertexBufferRequestSize = (m_vertexUsedCount + m_vertexCache.getCount()) * sizeof(Vertex);
			if (!m_vertexBuffer) {
				m_vertexBuffer = makeObject<VertexBuffer>(vertexBufferRequestSize, GraphicsResourceUsage::Dynamic);
			}
			else if (m_vertexBuffer->size() < vertexBufferRequestSize) {
                auto newSize = std::max(m_vertexBuffer->size() * 2, vertexBufferRequestSize);
				m_vertexBuffer->resize(newSize);
			}

			// IndexBuffer
			int indexBufferRequestCount = (m_indexUsedCount + m_indexCache.getCount());
			if (!m_indexBuffer) {
				m_indexBuffer = makeObject<IndexBuffer>(indexBufferRequestCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
			}
			else if (m_indexBuffer->size() < indexBufferRequestCount) {
                auto newSize = std::max(m_indexBuffer->size() * 2, indexBufferRequestCount);
				m_indexBuffer->resize(newSize);
			}
        }

		// 面方向を反転
		for (int i = 0; i < m_indexCache.getCount(); i += 3)
		{
			std::swap(m_indexCache.getAt(i + 1), m_indexCache.getAt(i + 2));
		}


		{
			//TODO: context に setData つくる



#if 0

#endif
			auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));
			//memcpy(ib + m_indexUsedCount, m_indexCache.getBuffer(), m_indexCache.getCount() * sizeof(uint16_t));
			auto head = ib + m_indexUsedCount;
			for (int i = 0; i < m_indexCache.getCount(); i++) {
				head[i] = m_vertexUsedCount + ((uint16_t*)m_indexCache.getBuffer())[i];
			}
			m_indexUsedCount += m_indexCache.getCount();


			auto vb = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
			memcpy(vb + m_vertexUsedCount, m_vertexCache.getBuffer(), m_vertexCache.getCount() * sizeof(Vertex));
			m_vertexUsedCount += m_vertexCache.getCount();


			//context->setSubData(m_vertexBuffer, 0, m_vertexCache.getBuffer(), m_vertexCache.getBufferUsedByteCount());
			//context->setSubData(m_indexBuffer, 0, m_indexCache.getBuffer(), m_indexCache.getBufferUsedByteCount());

			m_batchData.indexCount += m_indexCache.getCount();
		}

		// 描画する

		//{
  //          context->setVertexLayout(m_vertexLayout);
  //          context->setVertexBuffer(0, m_vertexBuffer);
  //          context->setIndexBuffer(m_indexBuffer);
  //          context->drawPrimitiveIndexed(0, m_indexCache.getCount() / 3);
		//}
	}

	// キャッシュクリア
	m_vertexCache.clear();
	m_indexCache.clear();
	m_basePoints.clear();
	m_outlinePoints.clear();
	m_pathes.clear();

	return RequestBatchResult::Staging;
}

void ShapesRenderFeature::beginRendering()
{
	m_vertexUsedCount = 0;
	m_indexUsedCount = 0;
	m_batchData.indexOffset = 0;
	m_batchData.indexCount = 0;
}

void ShapesRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	//if (m_mappedVertices) {
	//	// TODO: unmap (今は自動だけど、明示した方が安心かも)
	//}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.indexOffset = m_batchData.indexOffset + m_batchData.indexCount;
	m_batchData.indexCount = 0;
}

void ShapesRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);

	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.indexOffset, localBatch->data.indexCount / 3);
}

//------------------------------------------------------------------------------
//void ShapesRenderFeature::releaseCommandList(ShapesRendererCommandList* commandList)
//{
//	commandList->clear();
//	m_manager->getShapesRendererCommandListCache()->releaseCommandList(commandList);
//}

//void ShapesRenderFeature::prepareBuffers(GraphicsContext* context, int triangleCount)
//{
//	//if (context) {
//	//	// TODO: 実行中の map は context->map 用意した方がいいかも
//	//	LN_NOTIMPLEMENTED();
//	//}
//
//	if (m_buffersReservedTriangleCount < triangleCount)
//	{
//		size_t vertexCount = triangleCount * 3;
//		if (LN_ENSURE(vertexCount < 0xFFFF)) {
//			return;
//		}
//
//		// VertexBuffer
//		size_t vertexBufferSize = sizeof(Vertex) * vertexCount;
//		if (!m_vertexBuffer)
//			m_vertexBuffer = makeObject<VertexBuffer>(vertexBufferSize, GraphicsResourceUsage::Dynamic);
//		else
//			m_vertexBuffer->resize(vertexBufferSize);
//
//		// IndexBuffer
//		size_t indexBufferSize = spriteCount * 3;
//		if (!m_indexBuffer)
//			m_indexBuffer = makeObject<IndexBuffer>(indexBufferSize, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
//		else
//			m_indexBuffer->resize(indexBufferSize);
//		auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));	// TODO: 部分 map
//		int idx = 0;
//		int i2 = 0;
//		for (int i = 0; i < spriteCount; ++i)
//		{
//			i2 = i * 6;
//			idx = i * 4;
//			ib[i2 + 0] = idx;
//			ib[i2 + 1] = idx + 1;
//			ib[i2 + 2] = idx + 2;
//			ib[i2 + 3] = idx + 2;
//			ib[i2 + 4] = idx + 1;
//			ib[i2 + 5] = idx + 3;
//		}
//
//		m_buffersReservedSpriteCount = spriteCount;
//	}
//}

//------------------------------------------------------------------------------
ShapesRenderFeature::Path* ShapesRenderFeature::addPath(PathType type, const Matrix* transform, const Color& color, PathWinding winding, PathAttribute attribute)
{
	m_pathes.add(Path{ type, m_outlinePoints.getCount(), 0, color, winding, attribute, transform });
	return &m_pathes.back();
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::endPath(Path* path)
{
	path->pointCount = m_outlinePoints.getCount() - path->pointStart;
}

//------------------------------------------------------------------------------

void ShapesRenderFeature::extractBasePoints(ShapesRendererCommandList* commandList)
{
	ShapesRendererCommandList::ListNode* node = commandList->head;
	while (node)
	{
		extractBasePoints(node);
		node = node->next;
	}
}

void ShapesRenderFeature::extractBasePoints(ShapesRendererCommandList::ListNode* command)
{
	switch (command->type)
	{
		//------------------------------------------------------------------
		case ShapesRendererCommandList::Cmd_DrawBoxBackground:
		{
			auto* cmd = reinterpret_cast<ShapesRendererCommandList::DrawBoxBackgroundCommand*>(command);

			BorderComponent components[4];
			makeBasePointsAndBorderComponent(
				cmd->rect,//Rect(cmd[1], cmd[2], cmd[3], cmd[4]),
				Thickness(0, 0, 0, 0),
				cmd->cornerRadius,//CornerRadius(cmd[5], cmd[6], cmd[7], cmd[8]),
				components);

			// center box
			{
				auto* path = addPath(PathType::Convex, &cmd->transform, cmd->color, PathWinding::CCW, PathAttribute::Background);
				for (int iComp = 0; iComp < 4; iComp++)
				{
					for (int i = components[iComp].firstPoint; i < components[iComp].lastPoint; i++)	// 終点は次の Componet の開始点と一致するので必要ない
					{
						BasePoint& pt = m_basePoints.getAt(i);
						m_outlinePoints.add({ pt.pos, getAAExtDir(pt), 1.0f });
					}
				}
				endPath(path);
			}
			break;
		}
		//------------------------------------------------------------------
		case ShapesRendererCommandList::Cmd_DrawBoxBorderLine:
		{
			auto* cmd = reinterpret_cast<ShapesRendererCommandList::DrawBoxBorderLineCommand*> (command);

			BorderComponent components[4];
			makeBasePointsAndBorderComponent(
				cmd->rect,	//Rect(cmd[1], cmd[2], cmd[3], cmd[4]),
				cmd->thickness,// Thickness(cmd[5], cmd[6], cmd[7], cmd[8]),
				cmd->cornerRadius,//CornerRadius(cmd[25], cmd[26], cmd[27], cmd[28]),
				components);
			

			bool borderInset = cmd->borderInset;//(cmd[29] != 0.0f);
			struct BorderInfo
			{
				int		componentIndex;
				Color	color;
				float	width;
			};
			BorderInfo intos[4] =
			{
				{ 0, cmd->leftColor },// Color(cmd[9], cmd[10], cmd[11], cmd[12]), cmd[5] },	// left border
				{ 3, cmd->bottomColor },//Color(cmd[21], cmd[22], cmd[23], cmd[24]), cmd[8] },	// bottom border
				{ 2, cmd->rightColor },//Color(cmd[17], cmd[18], cmd[19], cmd[20]), cmd[7] },	// right border
				{ 1, cmd->topColor },//Color(cmd[13], cmd[14], cmd[15], cmd[16]), cmd[6] },	// top border
			};

			for (int iInfo = 0; iInfo < 4; iInfo++)
			{
				const BorderComponent& component = components[intos[iInfo].componentIndex];
				auto* path = addPath(PathType::Convex, &cmd->transform, intos[iInfo].color, PathWinding::CCW);

				if (borderInset)
				{
					// right-dir
					for (int i = component.firstPoint; i <= component.lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						m_outlinePoints.add({ pt.pos, getAAExtDir(pt), 1.0f });
					}
					// left-dir
					for (int i = component.lastPoint; i >= component.firstPoint; i--)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						m_outlinePoints.add({ getExtPos(pt, -1.0f, intos[iInfo].width), -getAAExtDir(pt), 1.0f });
					}
				}
				else
				{
					// right-dir
					for (int i = component.firstPoint; i <= component.lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						m_outlinePoints.add({ getExtPos(pt, 1.0f, intos[iInfo].width), getAAExtDir(pt), 1.0f });
					}
					// left-dir
					for (int i = component.lastPoint; i >= component.firstPoint; i--)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						m_outlinePoints.add({ pt.pos, -getAAExtDir(pt), 1.0f });
					}
				}
				endPath(path);
			}
			break;
		}

		case ShapesRendererCommandList::Cmd_DrawBoxShadow:
		{
			auto* cmd = reinterpret_cast<ShapesRendererCommandList::DrawBoxShadowCommand*>(command);

			float ltRad = cmd->cornerRadius.topLeft;
			float rtRad = cmd->cornerRadius.topRight;
			float lbRad = cmd->cornerRadius.bottomRight;
			float rbRad = cmd->cornerRadius.bottomLeft;

			Color shadowColor = cmd->color;
			float shadowBlur = cmd->blur;
			float shadowWidth = cmd->width;
			bool shadowInset = cmd->inset;

			float shadowFill = shadowBlur * 2;
			float shadowBlurWidth = (shadowWidth - shadowBlur) + shadowBlur * 2;	// base からシャドウのもっとも外側まで

			BorderComponent components[4];
			Vector2 lt[4];
			Vector2 rt[4];
			Vector2 lb[4];
			Vector2 rb[4];
			// basis
			lt[1] = Vector2(cmd->rect.x, cmd->rect.y);
			rt[1] = Vector2(cmd->rect.x + cmd->rect.width, cmd->rect.y);
			lb[1] = Vector2(cmd->rect.x, cmd->rect.y + cmd->rect.height);
			rb[1] = Vector2(cmd->rect.x + cmd->rect.width, cmd->rect.y + cmd->rect.height);
			// inner
			lt[2] = Vector2(lt[1].x + ltRad, lt[1].y + ltRad);
			rt[2] = Vector2(rt[1].x - rtRad, rt[1].y + rtRad);
			lb[2] = Vector2(lb[1].x + lbRad, lb[1].y - lbRad);
			rb[2] = Vector2(rb[1].x - rbRad, rb[1].y - rbRad);
			// shadow outer
			lt[3] = Vector2(lt[1].x - shadowBlurWidth, lt[1].y - shadowBlurWidth);
			rt[3] = Vector2(rt[1].x + shadowBlurWidth, rt[1].y - shadowBlurWidth);
			lb[3] = Vector2(lb[1].x - shadowBlurWidth, lb[1].y + shadowBlurWidth);
			rb[3] = Vector2(rb[1].x + shadowBlurWidth, rb[1].y + shadowBlurWidth);

			// left-side component
			components[0].firstPoint = m_basePoints.getCount();
			// left-top
			plotCornerBasePointsBezier(Vector2(lt[2].x, lt[3].y), Vector2(-1, 0), Vector2(lt[3].x, lt[2].y), Vector2(0, -1), 0.5, 1.0, lt[2]);
			// left-bottom
			plotCornerBasePointsBezier(Vector2(lb[3].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[3].y), Vector2(-1, 0), 0.0, 0.5, lb[2]);
			components[0].lastPoint = m_basePoints.getCount() - 1;

			// bottom-side component
			components[1].firstPoint = m_basePoints.getCount();
			// left-bottom
			plotCornerBasePointsBezier(Vector2(lb[3].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[3].y), Vector2(-1, 0), 0.5, 1.0, lb[2]);
			// right-bottom
			plotCornerBasePointsBezier(Vector2(rb[2].x, rb[3].y), Vector2(1, 0), Vector2(rb[3].x, rb[2].y), Vector2(0, 1), 0.0, 0.5, rb[2]);
			components[1].lastPoint = m_basePoints.getCount() - 1;

			// right-side component
			components[2].firstPoint = m_basePoints.getCount();
			// right-bottom
			plotCornerBasePointsBezier(Vector2(rb[2].x, rb[3].y), Vector2(1, 0), Vector2(rb[3].x, rb[2].y), Vector2(0, 1), 0.5, 1.0, rb[2]);
			// right-top
			plotCornerBasePointsBezier(Vector2(rt[3].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[3].y), Vector2(1, 0), 0.0, 0.5, rt[2]);
			components[2].lastPoint = m_basePoints.getCount() - 1;

			// top-side component
			components[3].firstPoint = m_basePoints.getCount();
			// right-top
			plotCornerBasePointsBezier(Vector2(rt[3].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[3].y), Vector2(1, 0), 0.5, 1.0, rt[2]);
			// left-top
			plotCornerBasePointsBezier(Vector2(lt[2].x, lt[3].y), Vector2(-1, 0), Vector2(lt[3].x, lt[2].y), Vector2(0, -1), 0.0, 0.5, lt[2]);
			components[3].lastPoint = m_basePoints.getCount() - 1;


			auto& offset = cmd->offset;

			if (shadowInset)
			{
				for (int iComp = 0; iComp < 4; iComp++)
				{
					auto* path = addPath(PathType::Strip3Point, &cmd->transform, shadowColor);
					for (int i = components[iComp].firstPoint; i <= components[iComp].lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth - pt.exDir * shadowBlurWidth + offset, getAAExtDir(pt), 0.0f });		// alpha=0
						m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth - pt.exDir * (shadowBlurWidth - shadowFill) + offset, getAAExtDir(pt), 1.0f }); // alpha=1
						m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth, getAAExtDir(pt), 1.0f });	// alpha=1

					}
					endPath(path);
				}
			}
			else
			{
				for (int iComp = 0; iComp < 4; iComp++)
				{
					auto* path = addPath(PathType::Strip3Point, &cmd->transform, shadowColor);
					for (int i = components[iComp].firstPoint; i <= components[iComp].lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);

						// left-dir
						m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth, getAAExtDir(pt), 1.0f });
						// right-dir
						m_outlinePoints.add({ pt.pos - pt.exDir * shadowFill + offset, getAAExtDir(pt), 1.0f });
						// right-dir
						m_outlinePoints.add({ pt.pos + offset, getAAExtDir(pt), 0.0f });

					}
					endPath(path);
				}
			}

			break;
		}

		default:
			LN_UNREACHABLE();
			break;
	}
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::makeBasePointsAndBorderComponent(const Rect& rect, const Thickness& thickness, const CornerRadius& cornerRadius, BorderComponent components[4])
{
	float tlRad = cornerRadius.topLeft;
	float trRad = cornerRadius.topRight;
	float blRad = cornerRadius.bottomLeft;
	float brRad = cornerRadius.bottomRight;
	Vector2 lt[3];
	Vector2 rt[3];
	Vector2 lb[3];
	Vector2 rb[3];
	// basis
	lt[1] = rect.getTopLeft();
	rt[1] = rect.getTopRight();
	lb[1] = rect.getBottomLeft();
	rb[1] = rect.getBottomRight();
	// outer
	lt[0] = Vector2(lt[1].x - thickness.left, lt[1].y - thickness.top);
	rt[0] = Vector2(rt[1].x + thickness.right, rt[1].y - thickness.top);
	lb[0] = Vector2(lb[1].x - thickness.left, lb[1].y + thickness.bottom);
	rb[0] = Vector2(rb[1].x + thickness.right, rb[1].y + thickness.bottom);
	// inner
	lt[2] = Vector2(lt[1].x + tlRad, lt[1].y + tlRad);
	rt[2] = Vector2(rt[1].x - trRad, rt[1].y + trRad);
	lb[2] = Vector2(lb[1].x + blRad, lb[1].y - blRad);
	rb[2] = Vector2(rb[1].x - brRad, rb[1].y - brRad);


	// left-side component
	components[0].firstPoint = m_basePoints.getCount();
	// left-top
	if (tlRad == 0.0f)
		m_basePoints.add({ lt[1], lt[0] - lt[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(lt[2].x, lt[1].y), Vector2(-1, 0), Vector2(lt[1].x, lt[2].y), Vector2(0, -1), 0.5, 1.0, lt[2]);
	// left-bottom
	if (blRad == 0.0f)
		m_basePoints.add({ lb[1], lb[0] - lb[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(lb[1].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[1].y), Vector2(-1, 0), 0.0, 0.5, lb[2]);
	components[0].lastPoint = m_basePoints.getCount() - 1;

	// bottom-side component
	components[1].firstPoint = m_basePoints.getCount();
	// left-bottom
	if (blRad == 0.0f)
		m_basePoints.add({ lb[1], lb[0] - lb[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(lb[1].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[1].y), Vector2(-1, 0), 0.5, 1.0, lb[2]);
	// right-bottom
	if (brRad == 0.0f)
		m_basePoints.add({ rb[1], rb[0] - rb[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(rb[2].x, rb[1].y), Vector2(1, 0), Vector2(rb[1].x, rb[2].y), Vector2(0, 1), 0.0, 0.5, rb[2]);
	components[1].lastPoint = m_basePoints.getCount() - 1;

	// right-side component
	components[2].firstPoint = m_basePoints.getCount();
	// right-bottom
	if (brRad == 0.0f)
		m_basePoints.add({ rb[1], rb[0] - rb[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(rb[2].x, rb[1].y), Vector2(1, 0), Vector2(rb[1].x, rb[2].y), Vector2(0, 1), 0.5, 1.0, rb[2]);
	// right-top
	if (trRad == 0.0f)
		m_basePoints.add({ rt[1], rt[0] - rt[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(rt[1].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[1].y), Vector2(1, 0), 0.0, 0.5, rt[2]);
	components[2].lastPoint = m_basePoints.getCount() - 1;

	// top-side component
	components[3].firstPoint = m_basePoints.getCount();
	// right-top
	if (trRad == 0.0f)
		m_basePoints.add({ rt[1], rt[0] - rt[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(rt[1].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[1].y), Vector2(1, 0), 0.5, 1.0, rt[2]);
	// left-top
	if (tlRad == 0.0f)
		m_basePoints.add({ lt[1], lt[0] - lt[1], false, true });
	else
		plotCornerBasePointsBezier(Vector2(lt[2].x, lt[1].y), Vector2(-1, 0), Vector2(lt[1].x, lt[2].y), Vector2(0, -1), 0.0, 0.5, lt[2]);
	components[3].lastPoint = m_basePoints.getCount() - 1;
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::calcExtrudedDirection()
{

}

//------------------------------------------------------------------------------
void ShapesRenderFeature::expandVertices(const Path& path)
{
	for (int i = 0; i < path.pointCount; i++)
	{
		const OutlinePoint& pt = m_outlinePoints.getAt(path.pointStart + i);
		Vertex v;
		v.position = Vector3::transformCoord(Vector3(pt.pos + g_finalOffset, 0), *path.transform);
		v.normal = Vector3::UnitZ;
		v.uv = Vector2::Zero;
		v.color = path.color;
		v.color.a *= pt.alpha;
		m_vertexCache.add(v);
	}
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::expandFill(const Path& path)
{
	int startIndex = m_vertexCache.getCount();

	expandVertices(path);

	// make IndexBuffer (反時計回り)
	int ib = startIndex;
	int i0 = 0;
	int i1 = 1;
	int i2 = path.pointCount - 1;
	for (int iPt = 0; iPt < path.pointCount - 2; iPt++)
	{
		if (path.winding == PathWinding::CCW)
		{
			m_indexCache.add(ib + i0);
			m_indexCache.add(ib + i1);
			m_indexCache.add(ib + i2);
		}
		else
		{
			m_indexCache.add(ib + i0);
			m_indexCache.add(ib + i2);
			m_indexCache.add(ib + i1);
		}

		if (iPt & 1) {	// 奇数回
			i0 = i2;
			--i2;
		}
		else {	// 偶数回
			i0 = i1;
			++i1;
		}
		/*
			↑の概要：
			頂点は反時計回りに並んでいることを前提とし、
			前後それぞれの方向からカーソルを進めるようにして三角形を作っていく。

			- 0回目、0,1,5 を結ぶ
			0-5 4
			|/
			1 2 3

			- 1回目、1,2,5 を結ぶ
			0-5 4
			|/|
			1-2 3

			- 3回目、5,2,4 を結ぶ
			0-5-4
			|/|/
			1-2 3

			- 4回目、2,3,4 を結ぶ
			0-5-4
			|/|/|
			1-2-3
		*/
	}

	expandAntiAliasStroke(path, startIndex);
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::expandStrip2PointStroke(const Path& path)
{
	/*
		0-2-4
		|/|/|	-> front
		1-3-5
	*/

	int startIndex = m_vertexCache.getCount();

	expandVertices(path);

	for (int i = 0; i < (path.pointCount / 2) - 1; i++)
	{
		int ib = startIndex + i * 2;
		m_indexCache.add(ib + 0);
		m_indexCache.add(ib + 1);
		m_indexCache.add(ib + 2);
		m_indexCache.add(ib + 2);
		m_indexCache.add(ib + 1);
		m_indexCache.add(ib + 3);
	}
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::expandStrip3PointStroke(const Path& path)
{
	/*
		0-3-6
		|/|/|
		1-4-7	-> front
		|/|/|
		2-5-8
	*/
	int startIndex = m_vertexCache.getCount();

	expandVertices(path);

	for (int i = 0; i < (path.pointCount / 3) - 1; i++)
	{
		int ib = startIndex + i * 3;
		m_indexCache.add(ib + 0);
		m_indexCache.add(ib + 1);
		m_indexCache.add(ib + 3);

		m_indexCache.add(ib + 3);
		m_indexCache.add(ib + 1);
		m_indexCache.add(ib + 4);

		m_indexCache.add(ib + 1);
		m_indexCache.add(ib + 2);
		m_indexCache.add(ib + 4);

		m_indexCache.add(ib + 4);
		m_indexCache.add(ib + 2);
		m_indexCache.add(ib + 5);
	}
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::expandAntiAliasStroke(const Path& path, int startIndex)
{
	const float ext = 0.25f;
	const float extAA = 0.5f;

	// 凸面周囲を右回りする。右に押し出す。
	int startAA = m_vertexCache.getCount();



	for (int i = 0; i < path.pointCount / 2; i++)
	{
		auto& p1 = m_vertexCache.getAt(startIndex + i);
		auto& p2 = m_vertexCache.getAt(startIndex + (path.pointCount / 2) + i);
		float len = (p1.position - p2.position).length() - (ext * 2);
		if (len < 1.0f)
		{
			p1.color.a = 0.25f;
			p2.color.a = 0.25f;
		}
	}

	for (int i = 0; i < path.pointCount; i++)
	{
		//int vi = startIndex + i;


		OutlinePoint& pt = m_outlinePoints.getAt(path.pointStart + i);

		Vector2 extDir(0, 0);
		if (std::abs(pt.exDirAA.x) > 0.9f || std::abs(pt.exDirAA.y) > 0.9f)
		{
		}
		else
		{
			extDir = pt.exDirAA;
		}

		//if (pt.exDirAA.x != 0 || pt.exDirAA.y != 0)
		//{

			//m_vertexCache.GetAt(startIndex + i).color.a = 0.5f;
		//}

		pt.pos -= extDir * ext;

		// TODO: 普通の頂点作成でも同じように transformCoord しているのでちょっと無駄が多い
		m_vertexCache.getAt(startIndex + i).position = Vector3::transformCoord(Vector3(pt.pos + g_finalOffset, 0), *path.transform);

		Vertex v;
		v.position = Vector3::transformCoord(Vector3(pt.pos + extDir * extAA + g_finalOffset, 0), *path.transform);// , *path.transform;
		v.color = path.color;
		v.color.a = 0;
		m_vertexCache.add(v);
	}

	/*
		0-1-2	- base
		|/|/|	-> front
		3-4-5	- ext
	*/

	for (int i = 0; i < path.pointCount - 1; i++)
	{
		int b = startIndex + i;
		int e = startAA + i;

		if (path.winding == PathWinding::CCW)
		{
			m_indexCache.add(b + 0);
			m_indexCache.add(e + 0);
			m_indexCache.add(b + 1);

			m_indexCache.add(b + 1);
			m_indexCache.add(e + 0);
			m_indexCache.add(e + 1);
		}
		else
		{
			m_indexCache.add(b + 0);
			m_indexCache.add(b + 1);
			m_indexCache.add(e + 0);

			m_indexCache.add(b + 1);
			m_indexCache.add(e + 1);
			m_indexCache.add(e + 0);
		}
	}
}

/*
	円弧を描く点を作成して m_basePoints に追加する
	firstCp		: 開始制御点
	firstCpDir	: 開始制御点の方向
	lastCp		: 終了制御点
	lastCpDir	: 終了制御点の方向
	firstT		: firstCp ～ lastCp を 0.0～1.0 としたとき、どこから点を打ち始めるか
	lastT		: firstCp ～ lastCp を 0.0～1.0 としたとき、どこまで点を打つか
	center		: 生成する円弧の中心点 (AAのための押し出し方向を点に設定するために使用)
*/
void ShapesRenderFeature::plotCornerBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center)
{
	assert(firstT < lastT);
	const int tess = 8;
	const float rtir = 0.55228f;	// https://cat-in-136.github.io/2014/03/bezier-1-kappa.html
	Vector2 d(std::abs(last.x - first.x), std::abs(last.y - first.y));
	Vector2 cp2 = (first)+(d * firstCpDir) * rtir;
	Vector2 cp3 = (last)+(d * lastCpDir) * rtir;
	float step = (lastT - firstT) / tess;
	for (int i = 0; i < tess; i++)
	{
		float t = firstT + (step * i);
		BasePoint pt;
		pt.pos = Vector2(
			Math::cubicBezier(first.x, cp2.x, cp3.x, last.x, t),
			Math::cubicBezier(first.y, cp2.y, cp3.y, last.y, t));
		pt.exDir = Vector2::normalize(pt.pos - center);
		pt.enabledAA = (0.0f < t && t < 1.0f);//true;
		pt.exDirect = false;
		m_basePoints.add(pt);
	}
	BasePoint pt;
	pt.pos = Vector2(
		Math::cubicBezier(first.x, cp2.x, cp3.x, last.x, lastT),
		Math::cubicBezier(first.y, cp2.y, cp3.y, last.y, lastT));
	pt.exDir = Vector2::normalize(pt.pos - center);
	pt.enabledAA = (0.0f < lastT && lastT < 1.0f);
	pt.exDirect = false;
	m_basePoints.add(pt);
}

#if 0
//==============================================================================
// ShapesRenderFeature

ShapesRenderFeature::ShapesRenderFeature()
	: m_internal(nullptr)
{
}

void ShapesRenderFeature::init(RenderingManager* manager)
{
    RenderFeature::init();
    m_internal = makeRef<ShapesRenderFeature>();
    m_internal->init(manager);
}

void ShapesRenderFeature::renderCommandList(GraphicsContext* context, const ShapesRendererCommandList& commandList)
{
    // commandList が持っているポインタは RenderingCommandList の LinearAllocator で確保したものなのでそのまま RenderCommand に乗せてOK

    GraphicsManager* manager = m_internal->manager()->graphicsManager();
	ICommandList* c = GraphicsContextInternal::commitState(context);
    LN_ENQUEUE_RENDER_COMMAND_3(
        ShapesRenderFeature_renderCommandList, context,
        ShapesRenderFeature*, m_internal,
		ICommandList*, c,
        ShapesRendererCommandList, commandList,
        {
            m_internal->renderCommandList(c, &commandList);
        });
}

void ShapesRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	// TODO:
}

void ShapesRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	LN_NOTIMPLEMENTED();
}
#endif



//==============================================================================
// BoxElementShapeBuilder

/*

座標系のルール
----------

- Y を下方向とする左手系とする。
- 面方向や処理順は時計回りで作っていく。


Component
----------
四辺の各情報を表す単位。

次の図は Box の右上を示しているが、Component の境界は B と C の境界のように、斜線で区切られる。
これによって、丸められた Border 間を自然につないだり、CSS で三角形を作るように Box を描画することができるようになる。

```
-------*
	  /|
 B   / |
    /  |
---* C |
 A |   |
```




Path の基本要素
----------

参考ページ：
http://www.bad-company.jp/box-shadow/

### BaseLine
各種 Path (Box、Border、Shadow 及び、その中でさらに分割された要素) の基準となる頂点リスト。

以下の種類があり、それぞれ始点と終点は一を共有した循環パスとなる。
ただし、Component 間は頂点を共有しない。（処理簡略化と、Border の色を頂点カラーで分ける場合に備える）


- Background 境界 (通常、Border の一辺と共有する)
- Border 境界
- Shadow の Box (CSS の shadow-offset 対策)
- Shadow のぼかしの境界
- Shadow の外側

#### Background 境界

Border 無しまたは、Border が Outset の場合、このサイズは入力となる BaseRect のサイズと等しくなる。

Border が Inset の場合、BaseRect の境界から内側に縮退した分が、Background 境界となる。


#### Border 境界

Inset か Outset かによって位置は異なるが、一部の頂点を Background と共有することでメモリ削減を狙う。


#### Shadow の Box

基本は BaseRect と同一だが、shadow-offset によって平行移動の修正が入る。

また Background や Border の色を頂点カラーで表すが、Shadow とそれらの色は通常異なるため、頂点を共有することはできない。


Outline
----------

BaseLine に対して、アンチエイリアスを施した頂点データを Outline とする。

実際に RHI の頂点バッファに書き込む位置データは、この Outline が持つ情報から作る。


```
.------.
|\    /|
| *--* |
| |  | |
| |  | |
| *--* |
|/    \|
.------.
```

上の図の * と . はペアとなっていて、OutlinePoint 構造体で管理される。
. は * から、1px 分、外側に押し出すことで作成する。

* の alpha 値は BackgroundColor は ShadowColor の alpha 値だが、. の alpha 値は 0 となる。


### Shadow の場合

* は box との接合点、 . はShadow のぼかしの境界、+ はShadow の外側(alpha 0)となる。

```
+----------+
|\        /|
| .------. |
| |\    /| |
| | *--* | |
| | |  | | |
| | |  | | |
| | *--* | |
| |/    \| |
| .------. |
|/        \|
+----------+
```

box との接合点 については、border の有無、border の inset, shadow の inset によって変わるが、基本的に何らかの頂点を共有する。



Tessellation
----------

### Background (Box)

中央に１点、後は Outline の頂点をなぞって TriangleFan で作る (実際の Toporogy は、他の Path とバッチ描画できるように TriangleList)

CornerRadius が無い場合は次のようになる。

```
*-----*
|\   /|
| \ / |
|  *  |
| / \ |
|/   \|
*-----*
```


Shadow
----------

Shadow は、Background と Border の部分についてはステンシルバッファを用いてフィルタリングしたように、描画することはない。
例えば Background と Border を半透明にすると、その後ろの Shadow は描画されていないことがわかる。

これに対応するために、
- まずは Shadow の根本＝ShadowBasePoint (outset なら box の外周、inset なら内周など) と頂点を共有して、普通に Outline を作る
- ShadowBasePoint 以外（外周とぼかしの境界）を ShadowOffset で移動する (ShadowBasePoint はそのまま、box と頂点を共有)
- 外周とぼかしの境界をそれぞれ、ShadowBasePoint との距離と、ShadowBasePointの進行右手方向との内積によって、位置と alpha 値を調節する。
  - 例えば Shadow が outset なら、外周頂点を内側には移動させない。＝ShadowBasePointと同じ位置にする。
    - 面は見えなくなるが、頂点は削除せずに縮退面で表現する。（削除まで絡めると処理が複雑になりすぎるため）

*/

BoxElementShapeBuilder::BoxElementShapeBuilder()
{
}

void BoxElementShapeBuilder::init()
{
	reset();
}

void BoxElementShapeBuilder::reset()
{
	//m_transform = Matrix::Identity;
	//m_baseRect = Rect::Zero;
	//m_cornerRadius = CornerRadius::Zero;
	//m_boxColor = Color::White;
	//m_borderThickness = Thickness::Zero;
	//m_borderLeftColor = Color::Black;
	//m_borderTopColor = Color::Black;
	//m_borderRightColor = Color::Black;
	//m_borderBottomColor = Color::Black;
	//m_borderInset = false;
	//m_shadowOffset = Vector2::Zero;
	//m_shadowColor = Color(1.0f, 1.0f, 1.0f, 0.5f);
	//m_shadowWidth = 0.0f;
	//m_shadowBlur = 0.0f;
	//m_shadowInset = false;
	//m_aligndLineAntiAlias = false;	// 軸に平行な辺にも AA を適用するか (回転させたい場合に使用)
	m_baseStyle.transform = Matrix::Identity;
	m_baseStyle.baseRect = Rect::Zero;
	m_baseStyle.cornerRadius = CornerRadius::Zero;
	m_baseStyle.aligndLineAntiAlias = false;
	m_backgroundEnabled = false;
	m_borderEnabled = false;
	m_shadowEnabled = false;

    m_baselinePointBuffer.clear();
    m_outlinePointBuffer.clear();
    m_outlinePaths.clear();
    m_vertexCache.clear();
    m_indexCache.clear();
}

void BoxElementShapeBuilder::setBaseRect(const BoxElementShapeBaseStyle& style)
{
	m_baseStyle = style;
}

void BoxElementShapeBuilder::setFillBox(const BoxElementShapeBackgroundStyle& style)
{
	m_backgroundStyle = style;
	m_backgroundEnabled = true;
}

void BoxElementShapeBuilder::setBoxBorderLine(const BoxElementShapeBorderStyle& style)
{
	m_borderStyle = style;
	m_borderEnabled = true;
}

void BoxElementShapeBuilder::setBoxShadow(const BoxElementShapeShadowStyle& style)
{
	m_shadowStyle = style;
	m_shadowEnabled = true;
}

void BoxElementShapeBuilder::build()
{
	if (Math::nearEqual(m_baseStyle.cornerRadius.topLeft, 0.0f) &&
		Math::nearEqual(m_baseStyle.cornerRadius.topRight, 0.0f) &&
		Math::nearEqual(m_baseStyle.cornerRadius.bottomRight, 0.0f) &&
		Math::nearEqual(m_baseStyle.cornerRadius.bottomLeft, 0.0f) &&
		!m_baseStyle.aligndLineAntiAlias) {
		m_shapeAAEnabled = false;	// Axis-Aligned な辺だけで外部から Anti-alias も要求されていないため、全体的に Anti-alias は不要
	}
	else {
		m_shapeAAEnabled = true;
	}


	// Make shape outer baseline and component
	{
		// Make shape outer rect
		// Shape の外周となる shape outer 矩形を確定する
		{
			m_shapeOuterRect.rect = m_baseStyle.baseRect;
			m_shapeOuterRect.rect.width = std::max(0.0f, m_shapeOuterRect.rect.width);
			m_shapeOuterRect.rect.height = std::max(0.0f, m_shapeOuterRect.rect.height);
			if (m_borderEnabled) {
				if (m_borderStyle.borderInset) {
					// Note: CSS はデフォルトで inset。 Box のサイズは、Border の合計サイズよりも小さくすることはできない。(Style で width: 0px とかはできるが、表示時に調整される)
					m_shapeOuterRect.rect.width = std::max(m_shapeOuterRect.rect.width, m_borderStyle.borderThickness.width());
					m_shapeOuterRect.rect.height = std::max(m_shapeOuterRect.rect.height, m_borderStyle.borderThickness.height());
				}
				else {
					m_shapeOuterRect.rect.width += m_borderStyle.borderThickness.width();
					m_shapeOuterRect.rect.height += m_borderStyle.borderThickness.height();
				}
			}
		}

		// Round の合計よりも Shape のサイズが小さい場合の対策。shape outer の Round サイズを調整する
        {
			m_shapeOuterRect.corner = m_baseStyle.cornerRadius;
            float l = std::max(m_shapeOuterRect.corner.topLeft, m_shapeOuterRect.corner.bottomLeft);
            float r = std::max(m_shapeOuterRect.corner.topRight, m_shapeOuterRect.corner.bottomRight);
            float t = std::max(m_shapeOuterRect.corner.topLeft, m_shapeOuterRect.corner.topRight);
            float b = std::max(m_shapeOuterRect.corner.bottomLeft, m_shapeOuterRect.corner.bottomRight);
            float radiusMaxWidth = l + r;
            float radiusMaxHeight = t + b;

            if (m_shapeOuterRect.rect.width < radiusMaxWidth) {
                float lr = l / radiusMaxWidth;
                float rr = r / radiusMaxWidth;
                m_shapeOuterRect.corner.topLeft = std::min(m_shapeOuterRect.rect.width * lr, m_shapeOuterRect.corner.topLeft);
                m_shapeOuterRect.corner.bottomLeft = std::min(m_shapeOuterRect.rect.width * lr, m_shapeOuterRect.corner.bottomLeft);
                m_shapeOuterRect.corner.topRight = std::min(m_shapeOuterRect.rect.width * rr, m_shapeOuterRect.corner.topRight);
                m_shapeOuterRect.corner.bottomRight = std::min(m_shapeOuterRect.rect.width * rr, m_shapeOuterRect.corner.bottomRight);
            }

            if (m_shapeOuterRect.rect.height < radiusMaxHeight) {
                float tr = t / radiusMaxHeight;
                float br = b / radiusMaxHeight;
                m_shapeOuterRect.corner.topLeft = std::min(m_shapeOuterRect.rect.height * tr, m_shapeOuterRect.corner.topLeft);
                m_shapeOuterRect.corner.topRight = std::min(m_shapeOuterRect.rect.height * tr, m_shapeOuterRect.corner.topRight);
                m_shapeOuterRect.corner.bottomLeft = std::min(m_shapeOuterRect.rect.height * br, m_shapeOuterRect.corner.bottomLeft);
                m_shapeOuterRect.corner.bottomRight = std::min(m_shapeOuterRect.rect.height * br, m_shapeOuterRect.corner.bottomRight);
            }
        }

		// ShapeInnerRect を確定する
		{
			const float tlRad = m_shapeOuterRect.corner.topLeft;
			const float trRad = m_shapeOuterRect.corner.topRight;
			const float blRad = m_shapeOuterRect.corner.bottomLeft;
			const float brRad = m_shapeOuterRect.corner.bottomRight;
			const float tw = (m_borderEnabled) ? m_borderStyle.borderThickness.top : 0.0f;
			const float rw = (m_borderEnabled) ? m_borderStyle.borderThickness.right : 0.0f;
			const float bw = (m_borderEnabled) ? m_borderStyle.borderThickness.bottom : 0.0f;
			const float lw = (m_borderEnabled) ? m_borderStyle.borderThickness.left : 0.0f;

			// top-left
			if (lw >= tlRad || tw >= tlRad)	// どちらかの BorderThickness が Radius を超えている場合、inner の角を丸める必要はない
				m_shapeInnerRect.corner.topLeft = 0;
			else                            // border の押し出し分だけ半径を調整する
				m_shapeInnerRect.corner.topLeft = std::max(m_shapeOuterRect.corner.topLeft - std::max(lw, tw), 0.0f);
			// top-right
			if (tw >= trRad || rw >= trRad)
				m_shapeInnerRect.corner.topRight = 0;
			else
				m_shapeInnerRect.corner.topRight = std::max(m_shapeOuterRect.corner.topRight - std::max(tw, rw), 0.0f);
			// bottom-right
			if (rw >= brRad || bw >= brRad)
				m_shapeInnerRect.corner.bottomRight = 0;
			else
				m_shapeInnerRect.corner.bottomRight = std::max(m_shapeOuterRect.corner.bottomRight - std::max(rw, bw), 0.0f);
			// bottom-left
			if (bw >= blRad || lw >= blRad)
				m_shapeInnerRect.corner.bottomLeft = 0;
			else
				m_shapeInnerRect.corner.bottomLeft = std::max(m_shapeOuterRect.corner.bottomLeft - std::max(bw, lw), 0.0f);

			m_shapeInnerRect.rect = m_shapeOuterRect.rect.makeDeflate(m_borderStyle.borderThickness);
		}

		// ShadowRect を確定する
		{
			const float w = m_shadowStyle.shadowWidth;

			if (m_shadowStyle.shadowInset) {
				// shadowWidth の押し出し分だけ半径を調整する
				m_shadowBaseRect.corner.topLeft = std::max(m_shapeInnerRect.corner.topLeft - w, 0.0f);
				m_shadowBaseRect.corner.topRight = std::max(m_shapeInnerRect.corner.topRight - w, 0.0f);
				m_shadowBaseRect.corner.bottomRight = std::max(m_shapeInnerRect.corner.bottomRight - w, 0.0f);
				m_shadowBaseRect.corner.bottomLeft = std::max(m_shapeInnerRect.corner.bottomLeft - w, 0.0f);
				m_shadowBaseRect.rect = m_shapeInnerRect.rect.makeDeflate(Thickness(w));
			}
			else {
				// shadowWidth の押し出し分だけ半径を調整する
				m_shadowBaseRect.corner.topLeft = m_shapeOuterRect.corner.topLeft + w, 0.0f;
				m_shadowBaseRect.corner.topRight = m_shapeOuterRect.corner.topRight + w, 0.0f;
				m_shadowBaseRect.corner.bottomRight = m_shapeOuterRect.corner.bottomRight + w, 0.0f;
				m_shadowBaseRect.corner.bottomLeft = m_shapeOuterRect.corner.bottomLeft + w, 0.0f;
				m_shadowBaseRect.rect = m_shapeInnerRect.rect.makeInflate(Thickness(w));
			}
		}

		if (m_borderEnabled) {
			m_borderComponents[Top].width = m_borderStyle.borderThickness.top;
			m_borderComponents[Top].color = m_borderStyle.borderTopColor;
			m_borderComponents[Right].width = m_borderStyle.borderThickness.right;
			m_borderComponents[Right].color = m_borderStyle.borderRightColor;
			m_borderComponents[Bottom].width = m_borderStyle.borderThickness.bottom;
			m_borderComponents[Bottom].color = m_borderStyle.borderBottomColor;
			m_borderComponents[Left].width = m_borderStyle.borderThickness.left;
			m_borderComponents[Left].color = m_borderStyle.borderLeftColor;
		}


		// Make edge info
		if (m_borderEnabled) {
			m_edgeInfo.thickness = m_borderStyle.borderThickness;
			auto innerRect = m_shapeOuterRect;
			auto center = Vector2(m_shapeOuterRect.rect.x + (m_shapeOuterRect.rect.width / 2), m_shapeOuterRect.rect.y + (m_shapeOuterRect.rect.height / 2));
			
			auto d = m_shapeOuterRect.rect.getTopLeft() - innerRect.rect.getTopLeft();
			m_edgeInfo.outerDirs[0] = (!Vector2::nearEqual(d, Vector2::Zero)) ? Vector2::normalize(d) : Vector2::normalize(m_shapeOuterRect.rect.getTopLeft() - center);

			d = m_shapeOuterRect.rect.getTopRight() - innerRect.rect.getTopRight();
			m_edgeInfo.outerDirs[1] = (!Vector2::nearEqual(d, Vector2::Zero)) ? Vector2::normalize(d) : Vector2::normalize(m_shapeOuterRect.rect.getTopRight() - center);

			d = m_shapeOuterRect.rect.getBottomRight() - innerRect.rect.getBottomRight();
			m_edgeInfo.outerDirs[2] = (!Vector2::nearEqual(d, Vector2::Zero)) ? Vector2::normalize(d) : Vector2::normalize(m_shapeOuterRect.rect.getBottomRight() - center);

			d = m_shapeOuterRect.rect.getBottomLeft() - innerRect.rect.getBottomLeft();
			m_edgeInfo.outerDirs[3] = (!Vector2::nearEqual(d, Vector2::Zero)) ? Vector2::normalize(d) : Vector2::normalize(m_shapeOuterRect.rect.getBottomLeft() - center);
		}
        else if (m_shapeOuterRect.rect.width <= 0.0f && m_shapeOuterRect.rect.height <= 0.0f) {
            m_edgeInfo.outerDirs[0] = Vector2(-1, -1);
            m_edgeInfo.outerDirs[1] = Vector2(1, -1);
            m_edgeInfo.outerDirs[2] = Vector2(1, 1);
            m_edgeInfo.outerDirs[3] = Vector2(-1, 1);
        }
		else {
			m_edgeInfo.thickness = Thickness::Zero;
			auto center = Vector2(m_shapeOuterRect.rect.x + (m_shapeOuterRect.rect.width / 2), m_shapeOuterRect.rect.y + (m_shapeOuterRect.rect.height / 2));
			m_edgeInfo.outerDirs[0] = Vector2::normalize(Vector2(m_shapeOuterRect.rect.getTopLeft()) - center);
			m_edgeInfo.outerDirs[1] = Vector2::normalize(Vector2(m_shapeOuterRect.rect.getTopRight()) - center);
			m_edgeInfo.outerDirs[2] = Vector2::normalize(Vector2(m_shapeOuterRect.rect.getBottomRight()) - center);
			m_edgeInfo.outerDirs[3] = Vector2::normalize(Vector2(m_shapeOuterRect.rect.getBottomLeft()) - center);
		}


	}

	// Make baseline phase
	{
		makeBasePointsAndBorderComponent(m_shapeOuterRect.rect, m_shapeOuterRect.corner, m_borderComponents);
		//calculateBasePointsNextDirection();


	// Make innter baseline
		if (m_borderEnabled) {
			m_innerBaselinePath.pointStart = m_baselinePointBuffer.getCount();

			const float tlRad = m_shapeOuterRect.corner.topLeft;
			const float trRad = m_shapeOuterRect.corner.topRight;
			const float blRad = m_shapeOuterRect.corner.bottomLeft;
			const float brRad = m_shapeOuterRect.corner.bottomRight;
			const float tw = (m_borderEnabled) ? m_borderStyle.borderThickness.top : 0.0f;
			const float rw = (m_borderEnabled) ? m_borderStyle.borderThickness.right : 0.0f;
			const float bw = (m_borderEnabled) ? m_borderStyle.borderThickness.bottom : 0.0f;
			const float lw = (m_borderEnabled) ? m_borderStyle.borderThickness.left : 0.0f;
			const auto& outerRect = m_shapeOuterRect.rect;

			// top-side
			{
				m_borderComponents[Top].innterPointStart = m_baselinePointBuffer.getCount();
				// top-left
				if (lw >= tlRad || tw >= tlRad)	// どちらかの BorderThickness が Radius を超えている場合、inner の角を丸める必要はない
					m_baselinePointBuffer.add({ Vector2(outerRect.getLeft() + lw, outerRect.getTop() + tw), -m_edgeInfo.outerDirs[0], 1.0f, Vector2(0, 1) });
				else
					plotInnerBasePoints(m_borderComponents[Top].outerCornerStart1, m_borderComponents[Top].outerCornerCount1(), lw, tw);
				// top-right
				if (tw >= trRad || rw >= trRad)
					m_baselinePointBuffer.add({ Vector2(outerRect.getRight() - rw, outerRect.getTop() + tw), -m_edgeInfo.outerDirs[1], 0.0f, Vector2(0, 1) });
				else
					plotInnerBasePoints(m_borderComponents[Top].outerCornerStart2, m_borderComponents[Top].outerCornerCount2(), tw, rw);
				m_borderComponents[Top].innterPointCount = m_baselinePointBuffer.getCount() - m_borderComponents[Top].innterPointStart;
			}
			// right-side
			{
				m_borderComponents[Right].innterPointStart = m_baselinePointBuffer.getCount();
				// top-right
				if (tw >= trRad || rw >= trRad)
					m_baselinePointBuffer.add({ Vector2(outerRect.getRight() - rw, outerRect.getTop() + tw), -m_edgeInfo.outerDirs[1], 1.0f, Vector2(-1, 0) });
				else
					plotInnerBasePoints(m_borderComponents[Right].outerCornerStart1, m_borderComponents[Right].outerCornerCount1(), tw, rw);
				// bottom-right
				if (rw >= brRad || bw >= brRad)
					m_baselinePointBuffer.add({ Vector2(outerRect.getRight() - rw, outerRect.getBottom() - bw), -m_edgeInfo.outerDirs[2], 0.0f, Vector2(-1, 0) });
				else
					plotInnerBasePoints(m_borderComponents[Right].outerCornerStart2, m_borderComponents[Right].outerCornerCount2(), rw, bw);
				m_borderComponents[Right].innterPointCount = m_baselinePointBuffer.getCount() - m_borderComponents[Right].innterPointStart;
			}
			// bottom-side
			{
				m_borderComponents[Bottom].innterPointStart = m_baselinePointBuffer.getCount();
				// bottom-right
				if (rw >= brRad || bw >= brRad)
					m_baselinePointBuffer.add({ Vector2(outerRect.getRight() - rw, outerRect.getBottom() - bw), -m_edgeInfo.outerDirs[2], 1.0f, Vector2(0, -1) });
				else
					plotInnerBasePoints(m_borderComponents[Bottom].outerCornerStart1, m_borderComponents[Bottom].outerCornerCount1(), rw, bw);
				// bottom-left
				if (bw >= blRad || lw >= blRad)
					m_baselinePointBuffer.add({ Vector2(outerRect.getLeft() + lw, outerRect.getBottom() - bw), -m_edgeInfo.outerDirs[3], 0.0f, Vector2(0, -1) });
				else
					plotInnerBasePoints(m_borderComponents[Bottom].outerCornerStart2, m_borderComponents[Bottom].outerCornerCount2(), bw, lw);
				m_borderComponents[Bottom].innterPointCount = m_baselinePointBuffer.getCount() - m_borderComponents[Bottom].innterPointStart;
			}
			// left-side
			{
				m_borderComponents[Left].innterPointStart = m_baselinePointBuffer.getCount();
				// bottom-left
				if (bw >= blRad || lw >= blRad)
					m_baselinePointBuffer.add({ Vector2(outerRect.getLeft() + lw, outerRect.getBottom() - bw), -m_edgeInfo.outerDirs[3], 1.0f, Vector2(1, 0) });
				else
					plotInnerBasePoints(m_borderComponents[Left].outerCornerStart1, m_borderComponents[Left].outerCornerCount1(), bw, lw);
				// top-left
				if (lw >= tlRad || tw >= tlRad)
					m_baselinePointBuffer.add({ Vector2(outerRect.getLeft() + lw, outerRect.getTop() + tw), -m_edgeInfo.outerDirs[0], 0.0f, Vector2(1, 0) });
				else
					plotInnerBasePoints(m_borderComponents[Left].outerCornerStart2, m_borderComponents[Left].outerCornerCount2(), lw, tw);
				m_borderComponents[Left].innterPointCount = m_baselinePointBuffer.getCount() - m_borderComponents[Left].innterPointStart;
			}

			m_innerBaselinePath.pointCount = m_baselinePointBuffer.getCount() - m_innerBaselinePath.pointStart;
		}
		else {
			// inter は outer と同一とする
			m_innerBaselinePath = m_outerBaselinePath;
		}

		// shadow
		if (m_shadowEnabled) {
			makeShadowBaseline();
		}
	}




	// - Shadow 全体の厚さよりもぼかしの範囲が大きい場合は middlePoint が衝突するため調整が必要
	// - 非ぼかし部分よりも offset が大きい場合は Shape と middlePoint が衝突するため調整が必要
	bool adjustMiddlePoints =
		(m_shadowStyle.shadowBlur > m_shadowStyle.shadowWidth) ||
		((m_shadowStyle.shadowWidth - m_shadowStyle.shadowBlur) < m_shadowStyle.shadowOffset.length());

	// Outset shadow
	if (m_shadowEnabled && !m_shadowStyle.shadowInset) {

		auto* path = beginOutlinePath(OutlinePathType::Strip3Point, m_shadowStyle.shadowColor, PathWinding::CCW);//;

		for (int i = 0; i < m_shadowBaselinePath.pointCount; i++) {
			int baseIndex = m_shadowBaselinePath.pointStart + i;
			const auto& basePt = baselinePoint(baseIndex);

			addOutlinePoint({ baseIndex, basePt.pos, 0.0f, -1, Vector2::Zero, Vector2::Zero });
			addOutlinePoint({ baseIndex, basePt.pos, 0.0f, -1, Vector2::Zero, Vector2::Zero });
			addOutlinePoint({ baseIndex, basePt.pos, 0.0f, -1, Vector2::Zero, Vector2::Zero });

			//addOutlinePoint({ baseIndex, Vector2(m_shapeOuterRect.getLeft(), m_shapeOuterRect.getTop()), 1.0f, -1, Vector2(-1, -1) });

		}

		endOutlinePath(path);
#if 0
		{

			const float tw = m_shadowStyle.shadowWidth;// (m_borderEnabled) ? m_borderStyle.borderThickness.top : 0.0f;
			const float rw = m_shadowStyle.shadowWidth;// (m_borderEnabled) ? m_borderStyle.borderThickness.right : 0.0f;
			const float bw = m_shadowStyle.shadowWidth;// (m_borderEnabled) ? m_borderStyle.borderThickness.bottom : 0.0f;
			const float lw = m_shadowStyle.shadowWidth;// (m_borderEnabled) ? m_borderStyle.borderThickness.left : 0.0f;

			//
			auto* path = beginOutlinePath(OutlinePathType::Strip3Point, m_shadowStyle.shadowColor, PathWinding::CCW);//;
			{
				path->stripeClosing = true;

				// top-left
				{
					if (m_shapeOuterRect.corner.topLeft <= 0.0f) {
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getTop() - tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getTop() - tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft(), m_shapeOuterRect.getTop()), 1.0f, -1, Vector2(-1, -1) });
					}
					else {
						for (int i = 0; i < m_borderComponents[Left].outerCornerCount2(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Left].outerCornerStart2 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
						for (int i = 0; i < m_borderComponents[Top].outerCornerCount1(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Top].outerCornerStart1 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
					}
				}
				// top-right
				{
					if (m_shapeOuterRect.corner.topRight <= 0.0f) {
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getTop() - tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getTop() - tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight(), m_shapeOuterRect.getTop()), 1.0f, -1, Vector2(1, -1) });
					}
					else {
						for (int i = 0; i < m_borderComponents[Top].outerCornerCount2(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Top].outerCornerStart2 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
						for (int i = 0; i < m_borderComponents[Right].outerCornerCount1(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Right].outerCornerStart1 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
					}
				}
				// bottom-right
				{
					if (m_shapeOuterRect.corner.bottomRight <= 0.0f) {
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getBottom() + tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getBottom() + tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight(), m_shapeOuterRect.getBottom()), 1.0f, -1, Vector2(1, 1) });
					}
					else {
						for (int i = 0; i < m_borderComponents[Right].outerCornerCount2(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Right].outerCornerStart2 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
						for (int i = 0; i < m_borderComponents[Bottom].outerCornerCount1(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Bottom].outerCornerStart1 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
					}
				}
				// bottom-right
				{
					if (m_shapeOuterRect.corner.bottomLeft <= 0.0f) {
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getBottom() + tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getBottom() + tw) });
						addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft(), m_shapeOuterRect.getBottom()), 1.0f, -1, Vector2(-1, 1) });
					}
					else {
						for (int i = 0; i < m_borderComponents[Bottom].outerCornerCount2(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Bottom].outerCornerStart2 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
						for (int i = 0; i < m_borderComponents[Left].outerCornerCount1(); i++) {
							const auto& basePt = baselinePoint(m_borderComponents[Left].outerCornerStart1 + i);
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
							addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
						}
					}
				}

			}
			endOutlinePath(path);

			// Middle-point と alpha の調整
			{
				for (int i = 0; i < path->pointCount / 3; i++) {
					auto& outer = outlinePoint(path->pointStart + (i * 3) + 0);
					auto& middle = outlinePoint(path->pointStart + (i * 3) + 1);
					auto& base = outlinePoint(path->pointStart + (i * 3) + 2);

					outer.pos += m_shadowStyle.shadowOffset;
					middle.pos = (outer.pos - (base.antiAliasDir * m_shadowStyle.shadowBlur)) + m_shadowStyle.shadowOffset;
					middle.alpha = 1.0f;
					outer.alpha = 0.0f;


					if (adjustMiddlePoints)
					{
						Plane plane(Vector3(base.pos, 0), Vector3(base.antiAliasDir, 0));

						// middle を Shape の頂点にスナップし、alpha を調整する。
						// ※shape と middle の間は縮退面となるが、頂点の結合までやると複雑になるので今は若干無駄な頂点が増えている。
						//auto md = Vector2::normalize(middlePoint - base.pos);
						//auto dot = Vector2::dot(md, -basePt.infrateDir);
						//if (dot >= 0) {
						float d = plane.getDistanceToPoint(Vector3(middle.pos, 0));
						if (d < 0) {
							middle.pos = base.pos;
						}
						

						Plane plane2(Vector3(outer.pos, 0), Vector3(Vector2::normalize(-base.antiAliasDir), 0));
					
						float d2 = (outer.pos - middle.pos).length();
						float d3 = plane2.getDistanceToPoint(Vector3(middle.pos, 0));// (Vector3::dot(plane2.normal, Vector3(middle.pos, 0))) + sqrt(plane2.distance);//sqrt( plane2.getDistanceToPoint(Vector3(middle.pos, 0)));
						middle.alpha = std::min((d3 / m_shadowStyle.shadowBlur), 1.0f);
					}
				}
			}
		}





#endif








#if 0
		const auto ofs = m_shadowStyle.shadowOffset;

		auto* path = beginOutlinePath(OutlinePathType::Strip3Point, m_shadowStyle.shadowColor);
		for (int i = 0; i < m_outerBaselinePath.pointCount; i++) {
			int baseIndex = m_outerBaselinePath.pointStart + i;
			auto& basePt = m_baselinePointBuffer.getAt(baseIndex);

			// Shadow の一番外側、width まで押し出したところに点を打つ
			auto outerPoint = basePt.pos + basePt.infrateDir * m_shadowStyle.shadowWidth;
			// Shadow のぼかしはじめ位置。外側から、 blur 戻ったところに点を打つ
			auto middlePoint = ofs + (outerPoint - (basePt.infrateDir * m_shadowStyle.shadowBlur));
			auto middleAlpha = 1.0f;

			if (adjustMiddlePoints)
			{
				// middle を Shape の頂点にスナップし、alpha を調整する。
				// ※shape と middle の間は縮退面となるが、頂点の結合までやると複雑になるので今は若干無駄な頂点が増えている。
				auto md = Vector2::normalize(middlePoint - basePt.pos);
				auto dot = Vector2::dot(md, -basePt.infrateDir);
				if (dot >= 0) {
					middleAlpha = ((outerPoint - basePt.pos).length() / m_shadowStyle.shadowBlur);
					middlePoint = basePt.pos;
				}
			}

			int pair1 = addOutlinePoint({ baseIndex, basePt.pos, 1.0f, -1 });
			int pair2 = addOutlinePoint({ baseIndex, middlePoint, middleAlpha, pair1 });
			addOutlinePoint({ baseIndex, ofs + outerPoint, 0.0f, pair2 });
		}
		endOutlinePath(path);
#endif
	}

    if (m_backgroundEnabled) {
		// inner に面を張る
        auto* backgroundPath = beginOutlinePath(OutlinePathType::Convex, m_backgroundStyle.color);
        for (int i = 0; i < m_innerBaselinePath.pointCount; i++) {
            int baseIndex = m_innerBaselinePath.pointStart + i;
            auto& basePt = m_baselinePointBuffer.getAt(baseIndex);
            addOutlinePoint({ baseIndex, basePt.pos, 1.0f, -1 });
        }
        endOutlinePath(backgroundPath);

		if (m_shapeAAEnabled) {
			// Border が無ければ、Background の色を使って外周に AA を作る
			if (!m_borderEnabled) {
                LN_NOTIMPLEMENTED();
			}
		}
    }


	// Inset shadow
	if (m_shadowEnabled && m_shadowStyle.shadowInset) {
		const auto ofs = m_shadowStyle.shadowOffset;

		auto* path = beginOutlinePath(OutlinePathType::Strip3Point, m_shadowStyle.shadowColor, PathWinding::CCW);
		for (int i = 0; i < m_innerBaselinePath.pointCount; i++) {
			int baseIndex = m_innerBaselinePath.pointStart + i;
			auto& basePt = m_baselinePointBuffer.getAt(baseIndex);

			// Shadow の一番内側、width まで押し出したところに点を打つ
			auto innnerPoint = basePt.pos - (basePt.infrateDir * m_shadowStyle.shadowWidth);
			// Shadow のぼかしはじめ位置。外側から、 blur 戻ったところに点を打つ
			auto middlePoint = ofs + (innnerPoint + (basePt.infrateDir * m_shadowStyle.shadowBlur));
			auto middleAlpha = 1.0f;

			if (adjustMiddlePoints)
			{
				// middle を Shape の頂点にスナップし、alpha を調整する。
				// ※shape と middle の間は縮退面となるが、頂点の結合までやると複雑になるので今は若干無駄な頂点が増えている。
				auto md = Vector2::normalize(middlePoint - basePt.pos);
				auto dot = Vector2::dot(md, -basePt.infrateDir);
				if (dot <= 0) {
					middleAlpha = ((innnerPoint - basePt.pos).length() / m_shadowStyle.shadowBlur);
					middlePoint = basePt.pos;
				}
			}

			int pair1 = addOutlinePoint({ baseIndex, basePt.pos, 1.0f, -1 });
			int pair2 = addOutlinePoint({ baseIndex, middlePoint, middleAlpha, pair1 });
			addOutlinePoint({ baseIndex, ofs + innnerPoint, 0.0f, pair2 });
		}
		endOutlinePath(path);
	}

    if (m_borderEnabled) {
        for (int iComponent = 0; iComponent < 4; iComponent++) {
            auto& cmp = m_borderComponents[iComponent];
			if (cmp.width > 0.0f) {
				auto* path = beginOutlinePath(OutlinePathType::Convex, cmp.color);

                //int p0 = m_outlinePointBuffer.getCount();

				// outer
				for (int i = 0; i < cmp.pointCount; i++) {
					int baseIndex = cmp.startPoint + i;
					auto& basePt = m_baselinePointBuffer.getAt(baseIndex);
					addOutlinePoint({ baseIndex, basePt.pos, 1.0f, -1, basePt.infrateDir, basePt.rightDir });
				}

                //int p1 = m_outlinePointBuffer.getCount() - 1;
                //int p2 = m_outlinePointBuffer.getCount();

				// inner (面張りのため逆順)
				for (int i = cmp.innterPointCount - 1; i >= 0; i--) {
					int baseIndex = cmp.innterPointStart + i;
					auto& basePt = m_baselinePointBuffer.getAt(baseIndex);
					addOutlinePoint({ baseIndex, basePt.pos, 1.0f, -1, basePt.infrateDir, basePt.rightDir });
				}

                //int p3 = m_outlinePointBuffer.getCount() - 1;

                // TODO: border と border 間の AA

                //auto& pt1 = m_outlinePointBuffer.getAt(p1);
                //auto& pt2 = m_outlinePointBuffer.getAt(p2);
                //auto& pt3 = m_outlinePointBuffer.getAt(p3);
                //auto& pt0 = m_outlinePointBuffer.getAt(p0);


                //pt1.antiAliasDir = pt2.antiAliasDir = 
                //    pt1.rightDir = pt2.rightDir =
                //    Vector3::cross(Vector3(Vector2::normalize(pt2.pos - pt1.pos), 0), Vector3::UnitZ).xy();// *Vector2(1, -1);
                //
                //pt3.antiAliasDir = pt0.antiAliasDir =
                //    pt3.rightDir = pt0.rightDir = Vector3::cross(Vector3(Vector2::normalize(pt0.pos - pt3.pos), 0), Vector3::UnitZ).xy();// *Vector2(1, -1);



				endOutlinePath(path);

                makeOutlineAntiAlias(path, 0, path->pointCount);
			}
        }
    }

    
    expandPathes();
}

int BoxElementShapeBuilder::vertexCount() const
{
	return m_vertexCache.getCount();
}

int BoxElementShapeBuilder::indexCount() const
{
    return m_indexCache.getCount();
}

void BoxElementShapeBuilder::writeToBuffer(Vertex* vertexBuffer, uint16_t* indexBuffer, uint16_t indexOffset)
{
    memcpy(vertexBuffer, m_vertexCache.getBuffer(), m_vertexCache.getCount() * sizeof(Vertex));

    for (int i = 0; i < m_indexCache.getCount(); i++) {
        indexBuffer[i] = indexOffset + ((uint16_t*)m_indexCache.getBuffer())[i];
    }
}

void BoxElementShapeBuilder::makeBasePointsAndBorderComponent(const Rect& rect, const CornerRadius& cornerRadius, BorderComponent components[4])
{
    m_outerBaselinePath.pointStart = m_baselinePointBuffer.getCount();

	float tlRad = cornerRadius.topLeft;
	float trRad = cornerRadius.topRight;
	float blRad = cornerRadius.bottomLeft;
	float brRad = cornerRadius.bottomRight;
	Vector2 lt[2];
	Vector2 rt[2];
	Vector2 lb[2];
	Vector2 rb[2];
	// outer
	lt[0] = rect.getTopLeft();
	rt[0] = rect.getTopRight();
	lb[0] = rect.getBottomLeft();
	rb[0] = rect.getBottomRight();
	// inner (cornerRadius がある場合、その中心点)
	lt[1] = Vector2(lt[0].x + tlRad, lt[0].y + tlRad);
	rt[1] = Vector2(rt[0].x - trRad, rt[0].y + trRad);
	lb[1] = Vector2(lb[0].x + blRad, lb[0].y - blRad);
	rb[1] = Vector2(rb[0].x - brRad, rb[0].y - brRad);

	// top-side component
	{
		components[Top].startPoint = m_baselinePointBuffer.getCount();
		components[Top].outerCornerStart1 = m_baselinePointBuffer.getCount();
		// top-left
		if (tlRad == 0.0f)
			m_baselinePointBuffer.add({ lt[0], m_edgeInfo.outerDirs[0], 1.0f, Vector2(0, -1) });
		else
			plotCornerBasePointsBezier(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.5f, 1.0f, lt[1]);
		components[Top].outerCornerStart2 = m_baselinePointBuffer.getCount();
		// top-right
		if (trRad == 0.0f)
			m_baselinePointBuffer.add({ rt[0], m_edgeInfo.outerDirs[1], 0.0f, Vector2(0, -1) });
		else
			plotCornerBasePointsBezier(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.0f, 0.5f, rt[1]);
		components[Top].pointCount = m_baselinePointBuffer.getCount() - components[Top].startPoint;
	}

	// right-side component
	{
		components[Right].startPoint = m_baselinePointBuffer.getCount();
		components[Right].outerCornerStart1 = m_baselinePointBuffer.getCount();
		// top-right
		if (trRad == 0.0f)
			m_baselinePointBuffer.add({ rt[0], m_edgeInfo.outerDirs[1], 1.0f, Vector2(1, 0) });
		else
			plotCornerBasePointsBezier(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.5f, 1.0f, rt[1]);
		components[Right].outerCornerStart2 = m_baselinePointBuffer.getCount();
		// bottom-right
		if (brRad == 0.0f)
			m_baselinePointBuffer.add({ rb[0], m_edgeInfo.outerDirs[2], 0.0f, Vector2(1, 0) });
		else
			plotCornerBasePointsBezier(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.0f, 0.5f, rb[1]);
		components[Right].pointCount = m_baselinePointBuffer.getCount() - components[Right].startPoint;
	}

	// bottom-side component
	{
		components[Bottom].startPoint = m_baselinePointBuffer.getCount();
		components[Bottom].outerCornerStart1 = m_baselinePointBuffer.getCount();
		// bottom-right
		if (brRad == 0.0f)
			m_baselinePointBuffer.add({ rb[0], m_edgeInfo.outerDirs[2], 1.0f, Vector2(0, 1) });
		else
			plotCornerBasePointsBezier(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.5f, 1.0f, rb[1]);
		components[Bottom].outerCornerStart2 = m_baselinePointBuffer.getCount();
		// bottom-left
		if (blRad == 0.0f)
			m_baselinePointBuffer.add({ lb[1], m_edgeInfo.outerDirs[3], 0.0f, Vector2(0, 1) });
		else
			plotCornerBasePointsBezier(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.0f, 0.5f, lb[1]);
		components[Bottom].pointCount = m_baselinePointBuffer.getCount() - components[Bottom].startPoint;
	}

	// left-side component
	{
		components[Left].startPoint = m_baselinePointBuffer.getCount();
		components[Left].outerCornerStart1 = m_baselinePointBuffer.getCount();
		// bottom-left
		if (blRad == 0.0f)
			m_baselinePointBuffer.add({ lb[0], m_edgeInfo.outerDirs[3], 1.0f, Vector2(-1, 0) });
		else
			plotCornerBasePointsBezier(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.5f, 1.0f, lb[1]);
		components[Left].outerCornerStart2 = m_baselinePointBuffer.getCount();
		// top-left
		if (tlRad == 0.0f)
			m_baselinePointBuffer.add({ lt[0], m_edgeInfo.outerDirs[0], 0.0f, Vector2(-1, 0) });
		else
			plotCornerBasePointsBezier(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.0f, 0.5f, lt[1]);
		components[Left].pointCount = m_baselinePointBuffer.getCount() - components[Left].startPoint;
	}

    m_outerBaselinePath.pointCount = m_baselinePointBuffer.getCount() - m_outerBaselinePath.pointStart;
}

/*
	円弧を描く点を作成して m_basePoints に追加する
	firstCp		: 開始制御点
	firstCpDir	: 開始制御点の方向
	lastCp		: 終了制御点
	lastCpDir	: 終了制御点の方向
	firstT		: firstCp ～ lastCp を 0.0～1.0 としたとき、どこから点を打ち始めるか
	lastT		: firstCp ～ lastCp を 0.0～1.0 としたとき、どこまで点を打つか
	center		: 生成する円弧の中心点 (AAのための押し出し方向を点に設定するために使用)
*/
void BoxElementShapeBuilder::plotCornerBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center)
{
	assert(firstT < lastT);
	const int tess = 8;
	const float rtir = 0.55228f;	// https://cat-in-136.github.io/2014/03/bezier-1-kappa.html
	Vector2 d(std::abs(last.x - first.x), std::abs(last.y - first.y));
	Vector2 cp2 = (first)+(d * firstCpDir) * rtir;
	Vector2 cp3 = (last)+(d * lastCpDir) * rtir;
	float step = (lastT - firstT) / tess;
	for (int i = 0; i < tess; i++)
	{
		float t = firstT + (step * i);
		auto pos = Vector2(Math::cubicBezier(first.x, cp2.x, cp3.x, last.x, t), Math::cubicBezier(first.y, cp2.y, cp3.y, last.y, t));
		auto dir = Vector2::normalize(pos - center);
		m_baselinePointBuffer.add({ pos, dir, t, dir });
	}

	// end point
	auto pos = Vector2(Math::cubicBezier(first.x, cp2.x, cp3.x, last.x, lastT), Math::cubicBezier(first.y, cp2.y, cp3.y, last.y, lastT));
	auto dir = Vector2::normalize(pos - center);
	m_baselinePointBuffer.add({ pos, dir, 1.0f, dir });
}

void BoxElementShapeBuilder::plotInnerBasePoints(int pointStart, int pointCount, float startWidth, float endWidth)
{
    for (int i = 0; i < pointCount; i++) {
        auto& basePt = m_baselinePointBuffer.getAt(pointStart + i);
        float w = Math::lerp(startWidth, endWidth, basePt.cornerRatio);	// 始点から終点に向かって厚さを合わせていく
        m_baselinePointBuffer.add({ (basePt.pos + (-basePt.infrateDir) * w), -basePt.infrateDir, basePt.cornerRatio, basePt.rightDir });
    }
}

//void BoxElementShapeBuilder::calculateBasePointsNextDirection()
//{
//	for (int i = 0; i < m_baselinePointBuffer.getCount(); i++) {
//		BaselinePoint* pt = &m_baselinePointBuffer.getAt(i);
//		BaselinePoint* nextPt = (i == m_baselinePointBuffer.getCount() - 1) ? &m_baselinePointBuffer.getAt(0) : &m_baselinePointBuffer.getAt(i + 1);
//		pt->nextDir = Vector2::normalize(nextPt->pos - pt->pos);
//	}
//}

// shadowWidth だけ移動したものを作成する。blur はこの時点では関係ない
void BoxElementShapeBuilder::makeShadowBaseline()
{
	const float w = m_shadowStyle.shadowWidth;
	const auto& rect = m_shadowBaseRect.rect;

	float tlRad = m_shadowBaseRect.corner.topLeft;
	float trRad = m_shadowBaseRect.corner.topRight;
	float blRad = m_shadowBaseRect.corner.bottomLeft;
	float brRad = m_shadowBaseRect.corner.bottomRight;
	Vector2 lt[2];
	Vector2 rt[2];
	Vector2 lb[2];
	Vector2 rb[2];
	// outer
	lt[0] = rect.getTopLeft();
	rt[0] = rect.getTopRight();
	lb[0] = rect.getBottomLeft();
	rb[0] = rect.getBottomRight();
	// inner (cornerRadius がある場合、その中心点)
	lt[1] = Vector2(lt[0].x + tlRad, lt[0].y + tlRad);
	rt[1] = Vector2(rt[0].x - trRad, rt[0].y + trRad);
	lb[1] = Vector2(lb[0].x + blRad, lb[0].y - blRad);
	rb[1] = Vector2(rb[0].x - brRad, rb[0].y - brRad);

	m_shadowBaselinePath.pointStart = m_baselinePointBuffer.getCount();

	// top-left
	if (m_shadowBaseRect.corner.topLeft <= 0.0f)
		m_baselinePointBuffer.add({ Vector2(rect.getLeft(), rect.getTop()), Vector2(-1, -1), 0.0, Vector2::Zero });
	else
		plotCornerBasePointsBezier(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.0f, 1.0f, Vector2(lt[1]));
	
	// top-right
	if (m_shadowBaseRect.corner.topRight <= 0.0f)
		m_baselinePointBuffer.add({ Vector2(rect.getRight(), rect.getTop()), Vector2(1, -1), 0.0, Vector2::Zero });
	else
		plotCornerBasePointsBezier(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.0f, 1.0f, Vector2(rt[1]));
	
	// bottom-right
	if (m_shadowBaseRect.corner.bottomRight <= 0.0f)
		m_baselinePointBuffer.add({ Vector2(rect.getRight(), rect.getBottom()), Vector2(1, 1), 0.0, Vector2::Zero });
	else
		plotCornerBasePointsBezier(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.0f, 1.0f, Vector2(rb[1]));

	// bottom-left
	if (m_shadowBaseRect.corner.bottomLeft <= 0.0f)
		m_baselinePointBuffer.add({ Vector2(rect.getLeft(), rect.getBottom()), Vector2(-1, 1), 0.0, Vector2::Zero });
	else
		plotCornerBasePointsBezier(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.0f, 1.0f, Vector2(lb[1]));

	m_shadowBaselinePath.pointCount = m_baselinePointBuffer.getCount() - m_shadowBaselinePath.pointStart;


	//
	//auto* path = beginOutlinePath(OutlinePathType::Strip3Point, m_shadowStyle.shadowColor, PathWinding::CCW);//;
	//{
	//	path->stripeClosing = true;

	//	// top-left
	//	{
	//		if (m_shapeOuterRect.corner.topLeft <= 0.0f) {
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getTop() - tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getTop() - tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft(), m_shapeOuterRect.getTop()), 1.0f, -1, Vector2(-1, -1) });
	//		}
	//		else {
	//			for (int i = 0; i < m_borderComponents[Left].outerCornerCount2(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Left].outerCornerStart2 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//			for (int i = 0; i < m_borderComponents[Top].outerCornerCount1(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Top].outerCornerStart1 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//		}
	//	}
	//	// top-right
	//	{
	//		if (m_shapeOuterRect.corner.topRight <= 0.0f) {
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getTop() - tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getTop() - tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight(), m_shapeOuterRect.getTop()), 1.0f, -1, Vector2(1, -1) });
	//		}
	//		else {
	//			for (int i = 0; i < m_borderComponents[Top].outerCornerCount2(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Top].outerCornerStart2 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//			for (int i = 0; i < m_borderComponents[Right].outerCornerCount1(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Right].outerCornerStart1 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//		}
	//	}
	//	// bottom-right
	//	{
	//		if (m_shapeOuterRect.corner.bottomRight <= 0.0f) {
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getBottom() + tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight() + lw, m_shapeOuterRect.getBottom() + tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getRight(), m_shapeOuterRect.getBottom()), 1.0f, -1, Vector2(1, 1) });
	//		}
	//		else {
	//			for (int i = 0; i < m_borderComponents[Right].outerCornerCount2(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Right].outerCornerStart2 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//			for (int i = 0; i < m_borderComponents[Bottom].outerCornerCount1(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Bottom].outerCornerStart1 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//		}
	//	}
	//	// bottom-right
	//	{
	//		if (m_shapeOuterRect.corner.bottomLeft <= 0.0f) {
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getBottom() + tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft() - lw, m_shapeOuterRect.getBottom() + tw) });
	//			addOutlinePoint({ -1, Vector2(m_shapeOuterRect.getLeft(), m_shapeOuterRect.getBottom()), 1.0f, -1, Vector2(-1, 1) });
	//		}
	//		else {
	//			for (int i = 0; i < m_borderComponents[Bottom].outerCornerCount2(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Bottom].outerCornerStart2 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//			for (int i = 0; i < m_borderComponents[Left].outerCornerCount1(); i++) {
	//				const auto& basePt = baselinePoint(m_borderComponents[Left].outerCornerStart1 + i);
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.infrateDir * m_shadowStyle.shadowWidth });
	//				addOutlinePoint({ -1, basePt.pos, 1.0f, -1, basePt.infrateDir });
	//			}
	//		}
	//	}

	//}

}

BoxElementShapeBuilder::OutlinePath* BoxElementShapeBuilder::beginOutlinePath(OutlinePathType type, const Color& color, PathWinding winding)
{
    m_outlinePaths.add(OutlinePath{ type, m_outlinePointBuffer.getCount(), 0, color, winding });
    return &m_outlinePaths.back();
}

void BoxElementShapeBuilder::endOutlinePath(OutlinePath* path)
{
    path->pointCount = m_outlinePointBuffer.getCount() - path->pointStart;
}

int BoxElementShapeBuilder::addOutlinePoint(const OutlinePoint& point)
{
    m_outlinePointBuffer.add(point);
    return m_outlinePointBuffer.getCount();
}

void BoxElementShapeBuilder::makeOutlineAntiAlias(const OutlinePath* path, int start, int count)
{
    const float length = 1.0;
    int startPoint = path->pointStart;

    auto* aaPath = beginOutlinePath(OutlinePathType::PairdStripe, path->color, PathWinding::CW);
    aaPath->stripeClosing = true;
    for (int i = 0; i < count; i++) {
        int outlineIndex = startPoint + start + i;
        auto& pt = m_outlinePointBuffer.getAt(outlineIndex);
        //auto& basePt = m_baselinePointBuffer.getAt(pt.basePoint);

        Vector2 pos = pt.pos + (pt.antiAliasDir * length);
        if (!m_baseStyle.aligndLineAntiAlias) {
            float d = std::acos(std::abs(pt.rightDir.x)) / (Math::PIDiv2);	// 0.0(dig0) ~ 1.0(dig90) になる
            d = std::abs((d - 0.5f) * 2.0f);										// dig45 に近ければ 0.0, dig0 か dig90 に近ければ 1.0
            pos = Vector2::lerp(pos, pt.pos, d);
        }

        addOutlinePoint({ pt.basePoint, pos, 0.0f, outlineIndex });
    }
    endOutlinePath(aaPath);
}

void BoxElementShapeBuilder::expandPathes()
{
    for (int iPath = 0; iPath < m_outlinePaths.size(); iPath++)
    {
        auto& path = m_outlinePaths[iPath];
        switch (path.type)
        {
        case OutlinePathType::Convex:
            expandFill(path);
            break;
        case OutlinePathType::PairdStripe:
			expandPairdStripeStroke(path);
            break;
		case OutlinePathType::Strip3Point:
			expandStrip3PointStroke(path);
			break;
        default:
            LN_UNREACHABLE();
            break;
        }
    }
}

void BoxElementShapeBuilder::expandVertices(const OutlinePath& path)
{
    for (int i = 0; i < path.pointCount; i++)
    {
        const OutlinePoint& pt = m_outlinePointBuffer.getAt(path.pointStart + i);
        Vertex v;
        v.position = Vector3::transformCoord(Vector3(pt.pos + g_finalOffset, 0), m_baseStyle.transform);
        v.normal = Vector3::UnitZ;
        v.uv = Vector2::Zero;
        v.color = path.color;
        v.color.a *= pt.alpha;
        m_vertexCache.add(v);
    }
}

void BoxElementShapeBuilder::expandFill(const OutlinePath& path)
{
    int startIndex = m_vertexCache.getCount();

    expandVertices(path);

    // make IndexBuffer (反時計回り)
    int ib = startIndex;
    int i0 = 0;
    int i1 = 1;
    int i2 = path.pointCount - 1;
    for (int iPt = 0; iPt < path.pointCount - 2; iPt++)
    {
        if (path.winding == PathWinding::CW)
        {
            m_indexCache.add(ib + i0);
            m_indexCache.add(ib + i1);
            m_indexCache.add(ib + i2);
        }
        else
        {
            m_indexCache.add(ib + i0);
            m_indexCache.add(ib + i2);
            m_indexCache.add(ib + i1);
        }

        if (iPt & 1) {	// 奇数回
            i0 = i1;
            ++i1;
        }
        else {	// 偶数回
            i0 = i2;
            --i2;
        }
        /*
            頂点は時計回りに並んでいることを前提とし、
            前後それぞれの方向からカーソルを進めるようにして三角形を作っていく。

            - 0回目、0,1,5 を結ぶ
            0-1 2
            |/
            5 4 3

            - 1回目、5,1,4 を結ぶ
            0-1 2
            |/|
            5-4 3

            - 3回目、1,2,4 を結ぶ
            0-1-2
            |/|/
            5-4 3

            - 4回目、4,2,3 を結ぶ
            0-1-2
            |/|/|
            5-4-3
        */
    }
}

void BoxElementShapeBuilder::expandPairdStripeStroke(const OutlinePath& path)
{
    /*
        0-1-2
        |/|/|	-> front
        x-y-z
    */

    int startIndex = m_vertexCache.getCount();

    expandVertices(path);

    for (int i = 0; i < path.pointCount - 1; i++)
    {
        int ib = path.pointStart + i;// TODO: 頂点indexは OutlinePoint に持たせた方がいいかな。今は m_outlinePointBuffer.add した順に並ぶことを前提としている
        int x = m_outlinePointBuffer.getAt(path.pointStart + i).stripePair;
        int y = m_outlinePointBuffer.getAt(path.pointStart + i + 1).stripePair;

        if (path.winding == PathWinding::CW) {
            m_indexCache.add(ib + 0);
            m_indexCache.add(ib + 1);
            m_indexCache.add(x);
            m_indexCache.add(x);
            m_indexCache.add(ib + 1);
            m_indexCache.add(y);
        }
        else {
            m_indexCache.add(ib + 0);
            m_indexCache.add(x);
            m_indexCache.add(ib + 1);
            m_indexCache.add(x);
            m_indexCache.add(y);
            m_indexCache.add(ib + 1);
        }
    }


    if (path.stripeClosing) {
        const auto& endPt = m_outlinePointBuffer.getAt(path.pointStart + path.pointCount - 1);
        const auto& startPt = m_outlinePointBuffer.getAt(path.pointStart);

        /*
            0-1
            |/|
            2-3
        */
        int p0 = path.pointStart + path.pointCount - 1;// TODO: 頂点indexは OutlinePoint に持たせた方がいいかな。今は m_outlinePointBuffer.add した順に並ぶことを前提としている
        int p1 = path.pointStart;   // TODO: 頂点indexは OutlinePoint に持たせた方がいいかな。今は m_outlinePointBuffer.add した順に並ぶことを前提としている
        int p2 = endPt.stripePair;
        int p3 = startPt.stripePair;

        if (path.winding == PathWinding::CW) {
            m_indexCache.add(p0);
            m_indexCache.add(p1);
            m_indexCache.add(p2);
            m_indexCache.add(p2);
            m_indexCache.add(p1);
            m_indexCache.add(p3);
        }
        else {
            m_indexCache.add(p0);
            m_indexCache.add(p2);
            m_indexCache.add(p1);
            m_indexCache.add(p2);
            m_indexCache.add(p3);
            m_indexCache.add(p1);
        }
    }
}

void BoxElementShapeBuilder::expandStrip3PointStroke(const OutlinePath& path)
{
	/*
		0-3-6
		|/|/|
		1-4-7	-> front
		|/|/|
		2-5-8
	*/
	int startIndex = m_vertexCache.getCount();

	expandVertices(path);

	for (int i = 0; i < (path.pointCount / 3) - 1; i++)
	{
		int ib = startIndex + i * 3;

		if (path.winding == PathWinding::CW) {
			m_indexCache.add(ib + 0);
			m_indexCache.add(ib + 1);
			m_indexCache.add(ib + 3);

			m_indexCache.add(ib + 3);
			m_indexCache.add(ib + 1);
			m_indexCache.add(ib + 4);

			m_indexCache.add(ib + 1);
			m_indexCache.add(ib + 2);
			m_indexCache.add(ib + 4);

			m_indexCache.add(ib + 4);
			m_indexCache.add(ib + 2);
			m_indexCache.add(ib + 5);
		}
		else {
			m_indexCache.add(ib + 0);
			m_indexCache.add(ib + 3);
			m_indexCache.add(ib + 1);

			m_indexCache.add(ib + 3);
			m_indexCache.add(ib + 4);
			m_indexCache.add(ib + 1);

			m_indexCache.add(ib + 1);
			m_indexCache.add(ib + 4);
			m_indexCache.add(ib + 2);

			m_indexCache.add(ib + 4);
			m_indexCache.add(ib + 5);
			m_indexCache.add(ib + 2);
		}
	}

	if (path.stripeClosing) {
		/*
			6-0
			|/|
			7-1	-> front
			|/|
			8-2
		*/

		int p0 = startIndex + path.pointCount - 3;
		int p1 = startIndex + path.pointCount - 2;
		int p2 = startIndex + path.pointCount - 1;
		int p3 = startIndex + 0;
		int p4 = startIndex + 1;
		int p5 = startIndex + 2;

		if (path.winding == PathWinding::CW) {
			m_indexCache.add(p0);
			m_indexCache.add(p1);
			m_indexCache.add(p3);

			m_indexCache.add(p1);
			m_indexCache.add(p4);
			m_indexCache.add(p3);

			m_indexCache.add(p1);
			m_indexCache.add(p2);
			m_indexCache.add(p4);

			m_indexCache.add(p2);
			m_indexCache.add(p5);
			m_indexCache.add(p4);
		}
		else {
			m_indexCache.add(p0);
			m_indexCache.add(p3);
			m_indexCache.add(p1);

			m_indexCache.add(p1);
			m_indexCache.add(p3);
			m_indexCache.add(p4);

			m_indexCache.add(p1);
			m_indexCache.add(p4);
			m_indexCache.add(p2);

			m_indexCache.add(p2);
			m_indexCache.add(p4);
			m_indexCache.add(p5);
		}
	}
}

//==============================================================================
// BoxElementShapeCommandList

void BoxElementShapeCommandList::addCommandNode(ListNode* cmd, CommandType type)
{
    cmd->type = type;
    cmd->next = nullptr;
    if (!head) {
        head = cmd;
    }
    if (tail) {
        tail->next = cmd;
    }
    tail = cmd;
}

void BoxElementShapeCommandList::addResetCommand(LinearAllocator* allocator)
{
    auto* cmd = reinterpret_cast<ListNode*>(allocator->allocate(sizeof(ListNode)));
    addCommandNode(cmd, Cmd_Reset);
}

void BoxElementShapeCommandList::addBaseCommand(LinearAllocator* allocator, const BoxElementShapeBaseStyle& style)
{
    auto* cmd = reinterpret_cast<BaseCommand*>(allocator->allocate(sizeof(BaseCommand)));
    addCommandNode(cmd, Cmd_Base);
    cmd->style = style;
}

void BoxElementShapeCommandList::addBackgroundCommand(LinearAllocator* allocator, const BoxElementShapeBackgroundStyle& style)
{
    auto* cmd = reinterpret_cast<BackgroundCommand*>(allocator->allocate(sizeof(BackgroundCommand)));
    addCommandNode(cmd, Cmd_Background);
    cmd->style = style;
}

void BoxElementShapeCommandList::addBorderCommand(LinearAllocator* allocator, const BoxElementShapeBorderStyle& style)
{
    auto* cmd = reinterpret_cast<BorderCommand*>(allocator->allocate(sizeof(BorderCommand)));
    addCommandNode(cmd, Cmd_Border);
    cmd->style = style;
}

void BoxElementShapeCommandList::addShadowCommand(LinearAllocator* allocator, const BoxElementShapeShadowStyle& style)
{
    auto* cmd = reinterpret_cast<ShadowCommand*>(allocator->allocate(sizeof(ShadowCommand)));
    addCommandNode(cmd, Cmd_Shadow);
    cmd->style = style;
}

void BoxElementShapeCommandList::addSubmitCommand(LinearAllocator* allocator)
{
    auto* cmd = reinterpret_cast<ListNode*>(allocator->allocate(sizeof(ListNode)));
    addCommandNode(cmd, Cmd_Submit);
}




//==============================================================================
// BoxElementShapeBuilder2

/*

ver3
----------

従来は Baseline という仮想的な頂点リストを作り、それを押し出すことで、その法線上にいくつかの実際の頂点を作るようにしていた。
また、Path はインデックスバッファ相当のデータを作らずに必要な頂点が配列上で隣接していることを前提としていた。

しかし次のような問題により、共有の頂点を作る意味が薄れメモリを圧迫するだけになったり、既存の頂点からさらに押し出しを行うなどでインデックスが必要になってきた。

- Border と Border の間の Aniti-alias
- CornerRadius 無しの時の ShadowBlur

Border と Border の間の Aniti-alias
----------

次の様に接合されている時の斜線の部分に AA を適用したい。

```
*--------*
|\      /|
| *----* |
| |    | |
```

AA によって shape の上下左右にははみ出したくないので、次の様に押し出し方向を定める必要がある。

```
↓--------↓
 \      /
  ←----→ 
```

ただしこのままだと、Shape 自体の回転に備えて軸平行辺にも AA を適用したい場合にそれができなくなる。
この AA は Shape 中心から放射状に法線を定義したい。

そのため Border 間の AA については頂点に "第2 AA 用法線" という情報を持たせて対応する。


CornerRadius 無しの時の ShadowBlur
----------

次の様に、Baseline と Shadow外周line で頂点数が異なるケースがあるため、単純に Baseline の押し出しで対応できない。
 
```
↓ここの面取りが必要になる
  --------- ← ShadowBlur で生成した Shadow 外周
 /
| *--------	← ShadowBaseline. Corner 無し (ShadoRect, 実際には頂点を生成しない)
| |
| | *------ ← ShadowBlur で生成した Shadow 内周. Corner 無し
| | |
| | | *---- ← Shape の外周. Corner 無し
| | | |

```

頂点数が異なると、従来の Strip Path が使えなくなり、Convex Path を使う必要が出てくる。
しかし従来の Convex Path は頂点が配列上で隣接していることを前提としているため、新しくインデックスバッファが必要となる。


*/

/*
	円弧を描く点を作成して m_basePoints に追加する
	firstCp		: 開始制御点
	firstCpDir	: 開始制御点の方向
	lastCp		: 終了制御点
	lastCpDir	: 終了制御点の方向
	firstT		: firstCp ～ lastCp を 0.0～1.0 としたとき、どこから点を打ち始めるか
	lastT		: firstCp ～ lastCp を 0.0～1.0 としたとき、どこまで点を打つか
	center		: 生成する円弧の中心点 (AAのための押し出し方向を点に設定するために使用)
*/
template<class TCallback>
void Utils_plotCornerPointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center, TCallback callback)
{
	assert(firstT < lastT);
	//const int tess = 8;
    const int tess = (int)Math::clamp((std::max(std::abs(last.x - first.x), std::abs(last.y - first.y)) / 2) * (lastT - firstT), 2.0f, 8.0f);
	const float rtir = 0.55228f;	// https://cat-in-136.github.io/2014/03/bezier-1-kappa.html
	Vector2 d(std::abs(last.x - first.x), std::abs(last.y - first.y));
	Vector2 cp2 = (first)+(d * firstCpDir) * rtir;
	Vector2 cp3 = (last)+(d * lastCpDir) * rtir;
	float step = (lastT - firstT) / tess;
	for (int i = 0; i < tess; i++)
	{
		float t = firstT + (step * i);
		auto pos = Vector2(Math::cubicBezier(first.x, cp2.x, cp3.x, last.x, t), Math::cubicBezier(first.y, cp2.y, cp3.y, last.y, t));
		auto infrateDir = Vector2::normalize(pos - center);
		callback(pos, infrateDir, t);
		//m_baselinePointBuffer.add({ pos, dir, t, dir });
	}

	// end point
	auto pos = Vector2(Math::cubicBezier(first.x, cp2.x, cp3.x, last.x, lastT), Math::cubicBezier(first.y, cp2.y, cp3.y, last.y, lastT));
	auto infrateDir = Vector2::normalize(pos - center);
	callback(pos, infrateDir, 1.0f);
	//m_baselinePointBuffer.add({ pos, dir, 1.0f, dir });
}

BoxElementShapeBuilder2::BoxElementShapeBuilder2()
{
}

void BoxElementShapeBuilder2::init()
{
	reset();
}

void BoxElementShapeBuilder2::reset()
{
	m_baseStyle.transform = Matrix::Identity;
	m_baseStyle.baseRect = Rect::Zero;
	m_baseStyle.cornerRadius = CornerRadius::Zero;
	m_baseStyle.aligndLineAntiAlias = false;
	m_backgroundEnabled = false;
	m_borderEnabled = false;
	m_shadowEnabled = false;

	m_outlinePointBuffer.clear();
	m_outlinePaths.clear();
	m_outlineIndices.clear();
	m_vertexCache.clear();
	m_indexCache.clear();
}

void BoxElementShapeBuilder2::setBaseRect(const BoxElementShapeBaseStyle& style)
{
	m_baseStyle = style;
}

void BoxElementShapeBuilder2::setFillBox(const BoxElementShapeBackgroundStyle& style)
{
	m_backgroundStyle = style;
	m_backgroundEnabled = true;
}

void BoxElementShapeBuilder2::setBoxBorderLine(const BoxElementShapeBorderStyle& style)
{
	m_borderStyle = style;
	m_borderEnabled = true;
}

void BoxElementShapeBuilder2::setBoxShadow(const BoxElementShapeShadowStyle& style)
{
	m_shadowStyle = style;
	m_shadowEnabled = true;
}

void BoxElementShapeBuilder2::build()
{
    if (Math::nearEqual(m_baseStyle.cornerRadius.topLeft, 0.0f) &&
        Math::nearEqual(m_baseStyle.cornerRadius.topRight, 0.0f) &&
        Math::nearEqual(m_baseStyle.cornerRadius.bottomRight, 0.0f) &&
        Math::nearEqual(m_baseStyle.cornerRadius.bottomLeft, 0.0f) &&
        !m_baseStyle.aligndLineAntiAlias) {
        m_shapeAAEnabled = false;	// Axis-Aligned な辺だけで外部から Anti-alias も要求されていないため、全体的に Anti-alias は不要
    }
    else {
        m_shapeAAEnabled = true;
    }

    {
        if (m_borderEnabled) {
            m_shapeOuterComponents[Top].width = m_borderStyle.borderThickness.top;
            m_shapeOuterComponents[Top].color = m_borderStyle.borderTopColor;
            m_shapeOuterComponents[Right].width = m_borderStyle.borderThickness.right;
            m_shapeOuterComponents[Right].color = m_borderStyle.borderRightColor;
            m_shapeOuterComponents[Bottom].width = m_borderStyle.borderThickness.bottom;
            m_shapeOuterComponents[Bottom].color = m_borderStyle.borderBottomColor;
            m_shapeOuterComponents[Left].width = m_borderStyle.borderThickness.left;
            m_shapeOuterComponents[Left].color = m_borderStyle.borderLeftColor;
        }
    }

	setupBaseRects();

	makeBaseOuterPointsAndBorderComponent(m_shapeOuterRect, 1.0f, m_shapeOuterComponents, &m_shapeOuterBasePath);



	if (m_shadowEnabled && !m_shadowStyle.shadowInset) {
        const float wd2 = m_shadowStyle.shadowBlur / 2;

        BaseRect farShadowRect;
        farShadowRect.rect = m_shadowBaseRect.rect.makeInflate(wd2);
        farShadowRect.rect.x += m_shadowStyle.shadowOffset.x;
        farShadowRect.rect.y += m_shadowStyle.shadowOffset.y;
        const float maxFarShadowCornerSize = std::min(farShadowRect.rect.width, farShadowRect.rect.height) / 2;
        farShadowRect.corner = m_shadowBaseRect.corner;
        farShadowRect.corner.topLeft = std::min(farShadowRect.corner.topLeft + wd2, maxFarShadowCornerSize);
        farShadowRect.corner.topRight = std::min(farShadowRect.corner.topRight + wd2, maxFarShadowCornerSize);
        farShadowRect.corner.bottomRight = std::min(farShadowRect.corner.bottomRight + wd2, maxFarShadowCornerSize);
        farShadowRect.corner.bottomLeft = std::min(farShadowRect.corner.bottomLeft + wd2, maxFarShadowCornerSize);

        BaseRect nearShadowRect;
        nearShadowRect.rect = m_shadowBaseRect.rect.makeDeflate(wd2);
        nearShadowRect.rect.x += m_shadowStyle.shadowOffset.x;
        nearShadowRect.rect.y += m_shadowStyle.shadowOffset.y;
        nearShadowRect.corner = m_shadowBaseRect.corner;
        nearShadowRect.corner.topLeft = std::max(nearShadowRect.corner.topLeft - wd2, m_shapeOuterRect.corner.topLeft);
        nearShadowRect.corner.topRight = std::max(nearShadowRect.corner.topRight - wd2, m_shapeOuterRect.corner.topRight);
        nearShadowRect.corner.bottomRight = std::max(nearShadowRect.corner.bottomRight - wd2, m_shapeOuterRect.corner.bottomRight);
        nearShadowRect.corner.bottomLeft = std::max(nearShadowRect.corner.bottomLeft - wd2, m_shapeOuterRect.corner.bottomLeft);

        BorderComponent shapeShadowComponent[4];
        BasePath shapeShadowBasePath;
        makeBaseOuterPointsAndBorderComponent(m_shapeOuterRect, 1.0f, shapeShadowComponent, &shapeShadowBasePath);
        applyColorToShadowComponents(shapeShadowComponent, 1.0f);

        BorderComponent nearShadowComponents[4];
        BasePath nearShadowBasePath;
        makeBaseOuterPointsAndBorderComponent(nearShadowRect, 1.0f, nearShadowComponents, &nearShadowBasePath);
        applyColorToShadowComponents(nearShadowComponents, 1.0f);

        BorderComponent farShadowComponents[4];
        BasePath farShadowBasePath;
        makeBaseOuterPointsAndBorderComponent(farShadowRect, 1.0f, farShadowComponents, &farShadowBasePath);
        applyColorToShadowComponents(farShadowComponents, 0.0f);

        makeStripePointPair(nearShadowComponents, shapeShadowComponent);
        makeStripePointPair(farShadowComponents, nearShadowComponents);

        // 位置オフセットや blur の適用によって、ShapeOuterRect の内側に入り込んだ Point の位置と alpha を調整する
        for (int iId = nearShadowBasePath.begin(); iId < nearShadowBasePath.end(); iId++) {
            auto& iPt = outlinePoint(iId);
            auto& vPt = outlinePoint(iPt.stripePairPointId);

            // middle を Shape の頂点にスナップし、alpha を調整する。
            if (m_shapeOuterRect.rect.contains(iPt.pos)) {
                Plane plane(Vector3(vPt.pos, 0), Vector3(vPt.rightDir, 0));
                float d = plane.getDistanceToPoint(Vector3(iPt.pos, 0));
                iPt.pos = vPt.pos;
                float a = Math::clamp01(1.0f - (std::abs(d) / m_shadowStyle.shadowBlur));
                iPt.color.a *= a;
            }
        }

        // near shadow
        makeStripePath(nearShadowComponents, shapeShadowComponent);

        // far shadow
        if (m_shadowStyle.shadowBlur > 0.0f) {
            makeStripePath(farShadowComponents, nearShadowComponents);
        }
	}

    if (m_backgroundEnabled)
    {
        auto* path = beginOutlinePath(OutlinePathType::Convex, Color::Red);
        for (int i = m_shapeOuterBasePath.begin(); i < m_shapeOuterBasePath.end(); i++) {
            addOutlineIndex(i);
            outlinePoint(i).color = m_backgroundStyle.color;
        }
        endOutlinePath(path);


        if (m_shapeAAEnabled) {
            // Border が無ければ、Background の色を使って外周に AA を作る
            if (!m_borderEnabled) {
                makeOutlineAntiAlias(path);
            }
        }
    }

    if (m_borderEnabled) {

        BorderComponent shapeInnerComponent[4];
        makeBaseOuterPointsAndBorderComponent(m_shapeInnerRect, -1.0f, shapeInnerComponent, &m_shapeInnerBasePath);

        for (int iCmp = 0; iCmp < 4; iCmp++) {
            if (m_shapeOuterComponents[iCmp].width > 0.0f) {
                auto* path = beginOutlinePath(OutlinePathType::Convex, Color::Black);

                for (int i = m_shapeOuterComponents[iCmp].beginOuter(); i < m_shapeOuterComponents[iCmp].endOuter(); i++) {
                    addOutlineIndex(i);
                    outlinePoint(i).color = m_shapeOuterComponents[iCmp].color;
                }

                for (int i = shapeInnerComponent[iCmp].endOuter() - 1; i >= shapeInnerComponent[iCmp].beginOuter(); i--) {
                    addOutlineIndex(i);
                    outlinePoint(i).color = m_shapeOuterComponents[iCmp].color;
                }

                endOutlinePath(path);

                if (m_shapeAAEnabled) {
                    makeOutlineAntiAlias(path);
                }
            }
        }
    }

    // Inset shadow
    if (m_shadowEnabled && m_shadowStyle.shadowInset) {
        const float wd2 = m_shadowStyle.shadowBlur / 2;


        BaseRect nearShadowRect;
        nearShadowRect.rect = m_shadowBaseRect.rect.makeInflate(wd2);
        nearShadowRect.rect.x += m_shadowStyle.shadowOffset.x;
        nearShadowRect.rect.y += m_shadowStyle.shadowOffset.y;
        nearShadowRect.corner = m_shadowBaseRect.corner; //farShadowRect.corner;//
        nearShadowRect.corner.topLeft = std::max(nearShadowRect.corner.topLeft + wd2, m_shapeInnerRect.corner.topLeft);
        nearShadowRect.corner.topRight = std::max(nearShadowRect.corner.topRight + wd2, m_shapeInnerRect.corner.topRight);
        nearShadowRect.corner.bottomRight = std::max(nearShadowRect.corner.bottomRight + wd2, m_shapeInnerRect.corner.bottomRight);
        nearShadowRect.corner.bottomLeft = std::max(nearShadowRect.corner.bottomLeft + wd2, m_shapeInnerRect.corner.bottomLeft);

        BaseRect farShadowRect;
        farShadowRect.rect = m_shadowBaseRect.rect.makeDeflate(wd2);
        farShadowRect.rect.x += m_shadowStyle.shadowOffset.x;
        farShadowRect.rect.y += m_shadowStyle.shadowOffset.y;
        //const float maxFarShadowCornerSize = std::min(farShadowRect.rect.width, farShadowRect.rect.height) / 2;
        //farShadowRect.corner = nearShadowRect.corner;
        farShadowRect.corner = m_shadowBaseRect.corner;
        farShadowRect.corner.topLeft = std::min(wd2, m_shadowStyle.shadowWidth);//farShadowRect.corner.topLeft + wd2;// std::min(farShadowRect.corner.topLeft + wd2, maxFarShadowCornerSize);
        farShadowRect.corner.topRight = std::min(wd2, m_shadowStyle.shadowWidth);//ffarShadowRect.corner.topRight + wd2;//std::min(farShadowRect.corner.topRight + wd2, maxFarShadowCornerSize);
        farShadowRect.corner.bottomRight = std::min(wd2, m_shadowStyle.shadowWidth);//ffarShadowRect.corner.bottomRight + wd2;//std::min(farShadowRect.corner.bottomRight + wd2, maxFarShadowCornerSize);
        farShadowRect.corner.bottomLeft = std::min(wd2, m_shadowStyle.shadowWidth);//ffarShadowRect.corner.bottomLeft + wd2;//std::min(farShadowRect.corner.bottomLeft + wd2, maxFarShadowCornerSize);


        BorderComponent shapeShadowComponent[4];
        BasePath shapeShadowBasePath;
        makeBaseOuterPointsAndBorderComponent(m_shapeInnerRect, 1.0f, shapeShadowComponent, &shapeShadowBasePath);
        applyColorToShadowComponents(shapeShadowComponent, 1.0f);

        BorderComponent nearShadowComponents[4];
        BasePath nearShadowBasePath;
        makeBaseOuterPointsAndBorderComponent(nearShadowRect, 1.0f, nearShadowComponents, &nearShadowBasePath);
        applyColorToShadowComponents(nearShadowComponents, 1.0f);

        BorderComponent farShadowComponents[4];
        BasePath farShadowBasePath;
        makeBaseOuterPointsAndBorderComponent(farShadowRect, 1.0f, farShadowComponents, &farShadowBasePath);
        applyColorToShadowComponents(farShadowComponents, 0.0f);


        // near shadow
        makeStripePointPair(nearShadowComponents, shapeShadowComponent);

#if 1
        std::function<bool(const OutlinePoint& iPt, const OutlinePoint& vPt)> checkOuterFunc1[4] = {
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x < vPt.pos.x || iPt.pos.y < vPt.pos.y; },   // TopLeft
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x > vPt.pos.x || iPt.pos.y < vPt.pos.y; },   // TopRight
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x > vPt.pos.x || iPt.pos.y > vPt.pos.y; },   // BottomRight
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x < vPt.pos.x || iPt.pos.y > vPt.pos.y; },   // BottomLeft
        };
        std::function<bool(const OutlinePoint& iPt, const OutlinePoint& vPt)> checkOuterFunc2[4] = {
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x > vPt.pos.x || iPt.pos.y < vPt.pos.y; },   // TopRight
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x > vPt.pos.x || iPt.pos.y > vPt.pos.y; },   // BottomRight
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x < vPt.pos.x || iPt.pos.y > vPt.pos.y; },   // BottomLeft
            [](const OutlinePoint& iPt, const OutlinePoint& vPt) { return iPt.pos.x < vPt.pos.x || iPt.pos.y < vPt.pos.y; },   // TopLeft
        };

        for (int iCmp = 0; iCmp < 4; iCmp++) {
            auto& cmp = nearShadowComponents[iCmp];


            for (int id = cmp.beginIds1(); id < cmp.endIds1(); id++) {
                auto& iPt = outlinePoint(id);
                auto& vPt = outlinePoint(iPt.stripePairPointId);
                if (checkOuterFunc1[iCmp](iPt, vPt)) {
                    float d = (vPt.pos - iPt.pos).length();
                    float a = Math::clamp01(1.0f - (std::abs(d) / m_shadowStyle.shadowBlur));
                    iPt.color.a *= a;
                    iPt.pos = vPt.pos;
                }
            }

            for (int id = cmp.beginIds2(); id < cmp.endIds2(); id++) {
                auto& iPt = outlinePoint(id);
                auto& vPt = outlinePoint(iPt.stripePairPointId);
                if (checkOuterFunc2[iCmp](iPt, vPt)) {
                    float d = (vPt.pos - iPt.pos).length();
                    float a = Math::clamp01(1.0f - (std::abs(d) / m_shadowStyle.shadowBlur));
                    iPt.color.a *= a;
                    iPt.pos = vPt.pos;
                }
            }
        }

#else
        // 位置オフセットや blur の適用によって、ShapeOuterRect の内側に入り込んだ Point の位置と alpha を調整する
        for (int iId = nearShadowBasePath.begin(); iId < nearShadowBasePath.end(); iId++) {
            auto& iPt = outlinePoint(iId);
            auto& vPt = outlinePoint(iPt.stripePairPointId);

            // middle を Shape の頂点にスナップし、alpha を調整する。
            Plane plane(Vector3(vPt.pos, 0), Vector3(vPt.rightDir, 0));
            float d = plane.getDistanceToPoint(Vector3(iPt.pos, 0));
            if (d >= 0) {
            //if (!m_shapeInnerRect.rect.contains(iPt.pos)) {
                // ShapeInner が Radius を持たない場合、スナップしてもすべての頂点が四隅に集まってしまい、描画が崩れる。
                // 最寄りの辺にスナップさせる。
                iPt.pos -= vPt.rightDir * std::abs(d);//vPt.pos;
                float a = Math::clamp01(1.0f - (std::abs(d) / m_shadowStyle.shadowBlur));
                iPt.color.a *= a;

                //iPt.color.a = 0.5;
            }
        }
#endif

        makeStripePath(nearShadowComponents, shapeShadowComponent, PathWinding::CCW);
        //// Point 数が多い方をベースに Path を作る必要がある。
        //if (nearShadowComponents[0].outerPointCount >= shapeShadowComponent[0].outerPointCount) {
        //    makeStripePointPair(nearShadowComponents, shapeShadowComponent);
        //    makeStripePath(nearShadowComponents, shapeShadowComponent, PathWinding::CCW);
        //}
        //else {
        //    makeStripePointPair(shapeShadowComponent, nearShadowComponents);
        //    makeStripePath(shapeShadowComponent, nearShadowComponents, PathWinding::CCW);
        //}

        // far shadow
        if (m_shadowStyle.shadowBlur > 0.0f) {
            if (farShadowComponents[0].outerPointCount >= nearShadowComponents[0].outerPointCount) {
                makeStripePointPair(farShadowComponents, nearShadowComponents);
                makeStripePath(farShadowComponents, nearShadowComponents, PathWinding::CW);
            }
            else {
                makeStripePointPair(nearShadowComponents, farShadowComponents);
                makeStripePath(nearShadowComponents, farShadowComponents, PathWinding::CW);
            }
        }
#if 0

        // near 側の方が基本的に半径が大きくなるので、そちらから pair を作る
        makeStripePointPair(nearShadowComponents, shapeShadowComponent);
        makeStripePointPair(farShadowComponents, nearShadowComponents);

        // 位置オフセットや blur の適用によって、ShapeOuterRect の内側に入り込んだ Point の位置と alpha を調整する
        //for (int iId = nearShadowBasePath.begin(); iId < nearShadowBasePath.end(); iId++) {
        //    auto& iPt = outlinePoint(iId);
        //    auto& vPt = outlinePoint(iPt.stripePairPointId);

        //    // middle を Shape の頂点にスナップし、alpha を調整する。
        //    if (!m_shapeInnerRect.rect.contains(iPt.pos)) {
        //        Plane plane(Vector3(vPt.pos, 0), Vector3(vPt.rightDir, 0));
        //        float d = plane.getDistanceToPoint(Vector3(iPt.pos, 0));
        //        iPt.pos = vPt.pos;
        //        float a = Math::clamp01(1.0f - (std::abs(d) / m_shadowStyle.shadowBlur));
        //        iPt.color.a *= a;
        //    }
        //}

        // near shadow
        makeStripePath(nearShadowComponents, shapeShadowComponent, PathWinding::CCW);

        // far shadow
        if (m_shadowStyle.shadowBlur > 0.0f) {
            makeStripePath(farShadowComponents, nearShadowComponents, PathWinding::CCW);
        }
#endif
    }

	expandPathes();
}

int BoxElementShapeBuilder2::vertexCount() const
{
	return m_vertexCache.getCount();
	//return 4;
}

int BoxElementShapeBuilder2::indexCount() const
{
	return m_indexCache.getCount();
	//return 6;
}

void BoxElementShapeBuilder2::writeToBuffer(Vertex* vertexBuffer, uint16_t* indexBuffer, uint16_t indexOffset)
{
	memcpy(vertexBuffer, m_vertexCache.getBuffer(), m_vertexCache.getCount() * sizeof(Vertex));

	for (int i = 0; i < m_indexCache.getCount(); i++) {
		indexBuffer[i] = indexOffset + ((uint16_t*)m_indexCache.getBuffer())[i];
	}
	//vertexBuffer[0].position = Vector3(0, 0, 0);
	//vertexBuffer[0].color = Color::Red;
	//vertexBuffer[1].position = Vector3(100, 0, 0);
	//vertexBuffer[1].color = Color::Red;
	//vertexBuffer[2].position = Vector3(0, 100, 0);
	//vertexBuffer[2].color = Color::Red;
	//vertexBuffer[3].position = Vector3(100, 100, 0);
	//vertexBuffer[3].color = Color::Red;
	//indexBuffer[0] = 0;
	//indexBuffer[1] = 1;
	//indexBuffer[2] = 2;
	//indexBuffer[3] = 2;
	//indexBuffer[4] = 1;
	//indexBuffer[5] = 3;
}

void BoxElementShapeBuilder2::setupBaseRects()
{
	// Make shape outer rect
	// Shape の外周となる shape outer 矩形を確定する
	{
		m_shapeOuterRect.rect = m_baseStyle.baseRect;
		m_shapeOuterRect.rect.width = std::max(0.0f, m_shapeOuterRect.rect.width);
		m_shapeOuterRect.rect.height = std::max(0.0f, m_shapeOuterRect.rect.height);
		if (m_borderEnabled) {
			if (m_borderStyle.borderInset) {
				// Note: CSS はデフォルトで inset。 Box のサイズは、Border の合計サイズよりも小さくすることはできない。(Style で width: 0px とかはできるが、表示時に調整される)
				m_shapeOuterRect.rect.width = std::max(m_shapeOuterRect.rect.width, m_borderStyle.borderThickness.width());
				m_shapeOuterRect.rect.height = std::max(m_shapeOuterRect.rect.height, m_borderStyle.borderThickness.height());
			}
			else {
				m_shapeOuterRect.rect.width += m_borderStyle.borderThickness.width();
				m_shapeOuterRect.rect.height += m_borderStyle.borderThickness.height();
			}
		}
	}

	// Round の合計よりも Shape のサイズが小さい場合の対策。比率によって shape outer の Round サイズを調整する
	{
		m_shapeOuterRect.corner = m_baseStyle.cornerRadius;
		float l = std::max(m_shapeOuterRect.corner.topLeft, m_shapeOuterRect.corner.bottomLeft);
		float r = std::max(m_shapeOuterRect.corner.topRight, m_shapeOuterRect.corner.bottomRight);
		float t = std::max(m_shapeOuterRect.corner.topLeft, m_shapeOuterRect.corner.topRight);
		float b = std::max(m_shapeOuterRect.corner.bottomLeft, m_shapeOuterRect.corner.bottomRight);
		float radiusMaxWidth = l + r;
		float radiusMaxHeight = t + b;

        CornerRadius orgRads = m_shapeOuterRect.corner;

		if (m_shapeOuterRect.rect.width < radiusMaxWidth) {
			float lr = l / radiusMaxWidth;
			float rr = r / radiusMaxWidth;
			m_shapeOuterRect.corner.topLeft = std::min(m_shapeOuterRect.rect.width * lr, m_shapeOuterRect.corner.topLeft);
			m_shapeOuterRect.corner.bottomLeft = std::min(m_shapeOuterRect.rect.width * lr, m_shapeOuterRect.corner.bottomLeft);
			m_shapeOuterRect.corner.topRight = std::min(m_shapeOuterRect.rect.width * rr, m_shapeOuterRect.corner.topRight);
			m_shapeOuterRect.corner.bottomRight = std::min(m_shapeOuterRect.rect.width * rr, m_shapeOuterRect.corner.bottomRight);
		}

		if (m_shapeOuterRect.rect.height < radiusMaxHeight) {
			float tr = t / radiusMaxHeight;
			float br = b / radiusMaxHeight;
			m_shapeOuterRect.corner.topLeft = std::min(m_shapeOuterRect.rect.height * tr, m_shapeOuterRect.corner.topLeft);
			m_shapeOuterRect.corner.topRight = std::min(m_shapeOuterRect.rect.height * tr, m_shapeOuterRect.corner.topRight);
			m_shapeOuterRect.corner.bottomLeft = std::min(m_shapeOuterRect.rect.height * br, m_shapeOuterRect.corner.bottomLeft);
			m_shapeOuterRect.corner.bottomRight = std::min(m_shapeOuterRect.rect.height * br, m_shapeOuterRect.corner.bottomRight);
		}

        // 縮小された場合、どの程度の割合であるかを覚えておく。m_shapeOuterRect 以外の Path での Corner の最大値の計算に使用する。
        m_cornerRatios.topLeft = m_shapeOuterRect.corner.topLeft / orgRads.topLeft;
        m_cornerRatios.topRight = m_shapeOuterRect.corner.topRight / orgRads.topRight;
        m_cornerRatios.bottomLeft = m_shapeOuterRect.corner.bottomLeft / orgRads.bottomLeft;
        m_cornerRatios.bottomRight = m_shapeOuterRect.corner.bottomRight / orgRads.bottomRight;
	}

	// ShapeInnerRect を確定する
	{
		const float tlRad = m_shapeOuterRect.corner.topLeft;
		const float trRad = m_shapeOuterRect.corner.topRight;
		const float blRad = m_shapeOuterRect.corner.bottomLeft;
		const float brRad = m_shapeOuterRect.corner.bottomRight;
		const float tw = (m_borderEnabled) ? m_borderStyle.borderThickness.top : 0.0f;
		const float rw = (m_borderEnabled) ? m_borderStyle.borderThickness.right : 0.0f;
		const float bw = (m_borderEnabled) ? m_borderStyle.borderThickness.bottom : 0.0f;
		const float lw = (m_borderEnabled) ? m_borderStyle.borderThickness.left : 0.0f;

		// top-left
		if (lw >= tlRad || tw >= tlRad)	// どちらかの BorderThickness が Radius を超えている場合、inner の角を丸める必要はない
			m_shapeInnerRect.corner.topLeft = 0;
		else                            // border の押し出し分だけ半径を調整する
			m_shapeInnerRect.corner.topLeft = std::max(m_shapeOuterRect.corner.topLeft - std::max(lw, tw), 0.0f);
		// top-right
		if (tw >= trRad || rw >= trRad)
			m_shapeInnerRect.corner.topRight = 0;
		else
			m_shapeInnerRect.corner.topRight = std::max(m_shapeOuterRect.corner.topRight - std::max(tw, rw), 0.0f);
		// bottom-right
		if (rw >= brRad || bw >= brRad)
			m_shapeInnerRect.corner.bottomRight = 0;
		else
			m_shapeInnerRect.corner.bottomRight = std::max(m_shapeOuterRect.corner.bottomRight - std::max(rw, bw), 0.0f);
		// bottom-left
		if (bw >= blRad || lw >= blRad)
			m_shapeInnerRect.corner.bottomLeft = 0;
		else
			m_shapeInnerRect.corner.bottomLeft = std::max(m_shapeOuterRect.corner.bottomLeft - std::max(bw, lw), 0.0f);

		m_shapeInnerRect.rect = m_shapeOuterRect.rect.makeDeflate(m_borderStyle.borderThickness);
	}

	// ShadowRect を確定する
	{

		if (m_shadowStyle.shadowInset) {
            const float sw = m_shadowStyle.shadowWidth;
            m_shadowBaseRect.corner.topLeft = (sw >= m_shapeInnerRect.corner.topLeft) ? 0.0f : (sw - m_shapeInnerRect.corner.topLeft);
			m_shadowBaseRect.corner.topRight = (sw >= m_shapeInnerRect.corner.topRight) ? 0.0f : (sw - m_shapeInnerRect.corner.topRight);
			m_shadowBaseRect.corner.bottomRight = (sw >= m_shapeInnerRect.corner.bottomRight) ? 0.0f : (sw - m_shapeInnerRect.corner.bottomRight);
			m_shadowBaseRect.corner.bottomLeft = (sw >= m_shapeInnerRect.corner.bottomLeft) ? 0.0f : (sw - m_shapeInnerRect.corner.bottomLeft);

            const float w = m_shadowStyle.shadowBlur / 2;
            m_shadowBaseRect.corner.topLeft = m_shadowBaseRect.corner.topLeft + w;
            m_shadowBaseRect.corner.topRight = m_shadowBaseRect.corner.topRight + w;
            m_shadowBaseRect.corner.bottomRight = m_shadowBaseRect.corner.bottomRight + w;
            m_shadowBaseRect.corner.bottomLeft = m_shadowBaseRect.corner.bottomLeft + w;

			m_shadowBaseRect.rect = m_shapeInnerRect.rect.makeDeflate(Thickness(m_shadowStyle.shadowWidth));
		}
		else {
            const float w = m_shadowStyle.shadowBlur / 2;//m_shadowStyle.shadowWidth;//
			// shadowWidth の押し出し分だけ半径を調整する
			m_shadowBaseRect.corner.topLeft = m_shapeOuterRect.corner.topLeft + w;
			m_shadowBaseRect.corner.topRight = m_shapeOuterRect.corner.topRight + w;
			m_shadowBaseRect.corner.bottomRight = m_shapeOuterRect.corner.bottomRight + w;
			m_shadowBaseRect.corner.bottomLeft = m_shapeOuterRect.corner.bottomLeft + w;
			m_shadowBaseRect.rect = m_shapeInnerRect.rect.makeInflate(Thickness(m_shadowStyle.shadowWidth));
		}
	}
}

int BoxElementShapeBuilder2::addOutlinePoint(const Vector2& pos, const Vector2& infrateDir, float cornerRatio)
{
    m_outlinePointBuffer.add({ pos, Color::Black, {infrateDir, infrateDir}, infrateDir, cornerRatio });
	return m_outlinePointBuffer.getCount() - 1;
}

void BoxElementShapeBuilder2::makeBaseOuterPointsAndBorderComponent(const BaseRect& baseRect, float dirSign, BorderComponent components[4], BasePath* outBasePath)
{
	outBasePath->start = m_outlinePointBuffer.getCount();

	float tlRad = baseRect.corner.topLeft;
	float trRad = baseRect.corner.topRight;
	float blRad = baseRect.corner.bottomLeft;
	float brRad = baseRect.corner.bottomRight;
	Vector2 lt[2];
	Vector2 rt[2];
	Vector2 lb[2];
	Vector2 rb[2];
	// outer
	lt[0] = baseRect.rect.getTopLeft();
	rt[0] = baseRect.rect.getTopRight();
	lb[0] = baseRect.rect.getBottomLeft();
	rb[0] = baseRect.rect.getBottomRight();
	// inner (cornerRadius がある場合、その中心点)
	lt[1] = Vector2(lt[0].x + tlRad, lt[0].y + tlRad);
	rt[1] = Vector2(rt[0].x - trRad, rt[0].y + trRad);
	lb[1] = Vector2(lb[0].x + blRad, lb[0].y - blRad);
	rb[1] = Vector2(rb[0].x - brRad, rb[0].y - brRad);

	auto addPointCallback = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t); };

	// top-side component
	{
		components[Top].outerPointStart = m_outlinePointBuffer.getCount();
		components[Top].outerCornerStart1 = m_outlinePointBuffer.getCount();
		// top-left
		if (tlRad <= 0.0f)
			addOutlinePoint(lt[0], Vector2(0, -1), 0.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.5f, 1.0f, lt[1], addPointCallback);
		components[Top].outerCornerStart2 = m_outlinePointBuffer.getCount();
		// top-right
		if (trRad <= 0.0f)
			addOutlinePoint(rt[0], Vector2(0, -1), 1.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.0f, 0.5f, rt[1], addPointCallback);
		components[Top].outerPointCount = m_outlinePointBuffer.getCount() - components[Top].outerPointStart;
	}

	// right-side component
	{
		components[Right].outerPointStart = m_outlinePointBuffer.getCount();
		components[Right].outerCornerStart1 = m_outlinePointBuffer.getCount();
		// top-right
		if (trRad <= 0.0f)
			addOutlinePoint(rt[0], Vector2(1, 0), 0.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.5f, 1.0f, rt[1], addPointCallback);
		components[Right].outerCornerStart2 = m_outlinePointBuffer.getCount();
		// bottom-right
		if (brRad <= 0.0f)
			addOutlinePoint(rb[0], Vector2(1, 0), 1.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.0f, 0.5f, rb[1], addPointCallback);
		components[Right].outerPointCount = m_outlinePointBuffer.getCount() - components[Right].outerPointStart;
	}

	// bottom-side component
	{
		components[Bottom].outerPointStart = m_outlinePointBuffer.getCount();
		components[Bottom].outerCornerStart1 = m_outlinePointBuffer.getCount();
		// bottom-right
		if (brRad <= 0.0f)
			addOutlinePoint(rb[0], Vector2(0, 1), 0.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.5f, 1.0f, rb[1], addPointCallback);
		components[Bottom].outerCornerStart2 = m_outlinePointBuffer.getCount();
		// bottom-left
		if (blRad <= 0.0f)
			addOutlinePoint(lb[0], Vector2(0, 1), 1.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.0f, 0.5f, lb[1], addPointCallback);
		components[Bottom].outerPointCount = m_outlinePointBuffer.getCount() - components[Bottom].outerPointStart;
	}

	// left-side component
	{
		components[Left].outerPointStart = m_outlinePointBuffer.getCount();
		components[Left].outerCornerStart1 = m_outlinePointBuffer.getCount();
		// bottom-left
		if (blRad <= 0.0f)
			addOutlinePoint(lb[0], Vector2(-1, 0), 0.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.5f, 1.0f, lb[1], addPointCallback);
		components[Left].outerCornerStart2 = m_outlinePointBuffer.getCount();
		// top-left
		if (tlRad <= 0.0f)
			addOutlinePoint(lt[0], Vector2(-1, 0), 1.0f);
		else
			Utils_plotCornerPointsBezier(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.0f, 0.5f, lt[1], addPointCallback);
		components[Left].outerPointCount = m_outlinePointBuffer.getCount() - components[Left].outerPointStart;
	}

	outBasePath->count = m_outlinePointBuffer.getCount() - outBasePath->start;
}

BoxElementShapeBuilder2::OutlinePath* BoxElementShapeBuilder2::beginOutlinePath(OutlinePathType type, const Color& color, PathWinding winding)
{
	m_outlinePaths.add(OutlinePath{ type, m_outlineIndices.getCount(), 0, color, winding });
	return &m_outlinePaths.back();
}

void BoxElementShapeBuilder2::endOutlinePath(OutlinePath* path)
{
	path->pointCount = m_outlineIndices.getCount() - path->pointStart;
}

int BoxElementShapeBuilder2::addOutlineIndex(int index)
{
	m_outlineIndices.add(index);
	return m_outlineIndices.getCount();
}

// ある Path(mainCmps) 上の　Point の stripePairPointId へ、その内側の Path(innerCmps) 上の対応する PointId を設定する
void BoxElementShapeBuilder2::makeStripePointPair(BorderComponent* mainCmps, const BorderComponent* otherCmps)
{
    for (int iCmp = 0; iCmp < 4; iCmp++) {
        /*if (mainCmps[iCmp].outerPointCount == otherCmps[iCmp].outerPointCount) {
        }
        else */if (mainCmps[iCmp].outerPointCount >= otherCmps[iCmp].outerPointCount) {

            auto& outerCmp = mainCmps[iCmp];
            const auto& innerCmp = otherCmps[iCmp];

            {
                int innerId = innerCmp.beginIds1();
                //int ie = innerCmp.endIds1();
                for (int id = outerCmp.beginIds1(); id < outerCmp.endIds1(); id++) {
                    //if (innerId < innerCmp.endIds1() - 1 &&
                    //    outlinePoint(id).cornerRatio >= outlinePoint(innerId + 1).cornerRatio) {
                    //    innerId++;
                    //}
                    if (innerId < innerCmp.endIds1() - 1) {
                        auto& outerPt = outlinePoint(id);
                        auto& innerPt = outlinePoint(innerId + 1);
                        if (outerPt.cornerRatio >= innerPt.cornerRatio) {
                            innerId++;
                        }
                    }

                    assert(innerCmp.beginIds1() <= innerId && innerId < innerCmp.endIds1());
                    if (id == outerCmp.endIds1() - 1) {
                        assert(innerId == innerCmp.endIds1() - 1);  // 最後の頂点があっている？
                    }
                    outlinePoint(id).stripePairPointId = innerId;
                }
            }

            {
                int innerId = innerCmp.beginIds2();
                for (int id = outerCmp.beginIds2(); id < outerCmp.endIds2(); id++) {
                    if (innerId < innerCmp.endIds2() - 1 &&
                        outlinePoint(id).cornerRatio >= outlinePoint(innerId + 1).cornerRatio) {
                        innerId++;
                    }
                    assert(innerCmp.beginIds2() <= innerId && innerId < innerCmp.endIds2());
                    if (id == outerCmp.endIds2() - 1) {
                        assert(innerId == innerCmp.endIds2() - 1);  // 最後の頂点があっている？
                    }
                    outlinePoint(id).stripePairPointId = innerId;
                }
            }
        }
        else {
            LN_UNREACHABLE();
            //auto& mainCmp = mainCmps[iCmp];
            //const auto& otherCmp = otherCmps[iCmp];

            //{
            //    int mainId = mainCmp.beginIds1();
            //    for (int otherId = otherCmp.beginIds1(); otherId < otherCmp.endIds1(); otherId++) {
            //        if (mainId < mainCmp.endIds1() - 1 &&
            //            outlinePoint(otherId).cornerRatio >= outlinePoint(mainId + 1).cornerRatio) {
            //            mainId++;
            //        }
            //        assert(mainCmp.beginIds1() <= mainId && mainId < mainCmp.endIds1());
            //        if (otherId == otherCmp.endIds1() - 1) {
            //            assert(mainId == mainCmp.endIds1() - 1);
            //        }
            //        if (outlinePoint(mainId).stripePairPointId < 0)
            //            outlinePoint(mainId).stripePairPointId = otherId;
            //    }
            //}

            //{
            //    int mainId = mainCmp.beginIds2();
            //    for (int otherId = otherCmp.beginIds2(); otherId < otherCmp.endIds2(); otherId++) {
            //        if (mainId < mainCmp.endIds2() - 1 &&
            //            outlinePoint(otherId).cornerRatio >= outlinePoint(mainId + 1).cornerRatio) {
            //            mainId++;
            //        }
            //        assert(mainCmp.beginIds2() <= mainId && mainId < mainCmp.endIds2());
            //        if (otherId == otherCmp.endIds2() - 1) {
            //            assert(mainId == mainCmp.endIds2() - 1);
            //        }
            //        if (outlinePoint(mainId).stripePairPointId < 0)
            //            outlinePoint(mainId).stripePairPointId = otherId;
            //    }
            //}
        }
    }
}

void BoxElementShapeBuilder2::makeOutlineAntiAlias(const OutlinePath* path/*, int start, int count*/)
{
    const float length = 1.0;
    int startPoint = path->pointStart;
    int count = path->pointCount;

    for (int iAA = 0; iAA < path->antiAliasCount; iAA++) {
        auto* aaPath = beginOutlinePath(OutlinePathType::Stripe, path->color, PathWinding::CW);
        aaPath->stripeClosing = true;
        for (int i = 0; i < count; i++) {
            int idIndex = startPoint + i;
            auto& pt = outlinePoint(outlineIndex(idIndex));

            Vector2 pos = pt.pos + (pt.antiAliasDir[iAA] * length);
            if (!m_baseStyle.aligndLineAntiAlias) {
                float d = std::acos(std::abs(pt.rightDir.x)) / (Math::PIDiv2);	// 0.0(dig0) ~ 1.0(dig90) になる
                d = std::abs((d - 0.5f) * 2.0f);										// dig45 に近ければ 0.0, dig0 か dig90 に近ければ 1.0
                pos = Vector2::lerp(pos, pt.pos, d);
            }

            int newId = addOutlinePoint(pos, pt.rightDir, pt.cornerRatio);
            outlinePoint(newId).color = pt.color.withAlpha(0.0f);

            addOutlineIndex(newId);
            addOutlineIndex(outlineIndex(idIndex));
        }
        endOutlinePath(aaPath);
    }
}

void BoxElementShapeBuilder2::makeStripePath(const BorderComponent* outerCmps, const BorderComponent* innerCmps, PathWinding winding)
{
    for (int iCmp = 0; iCmp < 4; iCmp++) {
        const auto& outerCmp = outerCmps[iCmp];
        const auto& innerCmp = innerCmps[iCmp];

        auto* path = beginOutlinePath(OutlinePathType::Stripe, Color::Black, winding);

        for (int id = outerCmp.beginIds1(); id < outerCmp.endIds1(); id++) {
            addOutlineIndex(id);
            addOutlineIndex(outlinePoint(id).stripePairPointId);
        }

        for (int id = outerCmp.beginIds2(); id < outerCmp.endIds2(); id++) {
            addOutlineIndex(id);
            addOutlineIndex(outlinePoint(id).stripePairPointId);
        }

        endOutlinePath(path);
    }
}

void BoxElementShapeBuilder2::applyColorToShadowComponents(BorderComponent components[4], float alpha)
{
    const float ones[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    for (int iCmp = 0; iCmp < 4; iCmp++) {
        const auto& cmp = components[iCmp];
        for (int id = cmp.beginOuter(); id < cmp.endOuter(); id++) {
            outlinePoint(id).color = m_shadowStyle.shadowColor;
            outlinePoint(id).color.a *= alpha;
        }
    }
}

void BoxElementShapeBuilder2::expandPathes()
{
	for (int i = 0; i < m_outlinePointBuffer.getCount(); i++)
	{
		const OutlinePoint& pt = m_outlinePointBuffer.getAt(i);
		Vertex v;
		v.position = Vector3::transformCoord(Vector3(pt.pos + g_finalOffset, 0), m_baseStyle.transform);
		v.normal = Vector3::UnitZ;
		v.uv = Vector2::Zero;
        v.color = pt.color;
		m_vertexCache.add(v);
	}

	for (int iPath = 0; iPath < m_outlinePaths.size(); iPath++)
	{
		auto& path = m_outlinePaths[iPath];
		switch (path.type)
		{
		case OutlinePathType::Convex:
			expandConvex(path);
			break;
		case OutlinePathType::Stripe:
			expandStripeStroke(path);
			break;
		default:
			LN_UNREACHABLE();
			break;
		}
	}
}

void BoxElementShapeBuilder2::expandConvex(const OutlinePath& path)
{
	// make IndexBuffer (反時計回り)
	int ib = path.pointStart;
	int i0 = 0;
	int i1 = 1;
	int i2 = path.pointCount - 1;
	for (int iPt = 0; iPt < path.pointCount - 2; iPt++)
	{
		if (path.winding == PathWinding::CW)
		{
			m_indexCache.add(outlineIndex(ib + i0));
			m_indexCache.add(outlineIndex(ib + i1));
			m_indexCache.add(outlineIndex(ib + i2));
		}
		else
		{
			m_indexCache.add(outlineIndex(ib + i0));
			m_indexCache.add(outlineIndex(ib + i2));
			m_indexCache.add(outlineIndex(ib + i1));
		}

		if (iPt & 1) {	// 奇数回
			i0 = i1;
			++i1;
		}
		else {	// 偶数回
			i0 = i2;
			--i2;
		}
		/*
			頂点は時計回りに並んでいることを前提とし、
			前後それぞれの方向からカーソルを進めるようにして三角形を作っていく。

			- 0回目、0,1,5 を結ぶ
			0-1 2
			|/
			5 4 3

			- 1回目、5,1,4 を結ぶ
			0-1 2
			|/|
			5-4 3

			- 3回目、1,2,4 を結ぶ
			0-1-2
			|/|/
			5-4 3

			- 4回目、4,2,3 を結ぶ
			0-1-2
			|/|/|
			5-4-3
		*/
	}
}

void BoxElementShapeBuilder2::expandStripeStroke(const OutlinePath& path)
{
	/*
		0-1-2
		|/|/|	-> front
		x-y-z
	*/

	for (int i = 0; i < (path.pointCount / 2) - 1; i++)
	{
		//int ib = path.pointStart + i;// TODO: 頂点indexは OutlinePoint に持たせた方がいいかな。今は m_outlinePointBuffer.add した順に並ぶことを前提としている
		
        int p0 = outlineIndex(path.pointStart + (i * 2) + 0);
        int p1 = outlineIndex(path.pointStart + (i * 2) + 2);
        int p2 = outlineIndex(path.pointStart + (i * 2) + 1);   // x
        int p3 = outlineIndex(path.pointStart + (i * 2) + 3);   // y

		if (path.winding == PathWinding::CW) {
			m_indexCache.add(p0);
			m_indexCache.add(p1);
			m_indexCache.add(p2);
			m_indexCache.add(p2);
			m_indexCache.add(p1);
			m_indexCache.add(p3);
		}
		else {
            m_indexCache.add(p0);
            m_indexCache.add(p2);
            m_indexCache.add(p1);
            m_indexCache.add(p2);
            m_indexCache.add(p3);
            m_indexCache.add(p1);
		}
	}

	if (path.stripeClosing) {
		/*
			0-1
			|/|
			2-3
		*/
        // TODO: 頂点indexは OutlinePoint に持たせた方がいいかな。今は m_outlinePointBuffer.add した順に並ぶことを前提としている
		int p0 = outlineIndex(path.pointStart + (0));
        int p1 = outlineIndex(path.pointStart + (path.pointCount - 2));
		int p2 = outlineIndex(path.pointStart + (1));
        int p3 = outlineIndex(path.pointStart + (path.pointCount - 1));

		if (path.winding == PathWinding::CW) {
			m_indexCache.add(p0);
			m_indexCache.add(p2);
			m_indexCache.add(p1);
			m_indexCache.add(p2);
			m_indexCache.add(p3);
			m_indexCache.add(p1);
		}
		else {
			m_indexCache.add(p0);
			m_indexCache.add(p1);
			m_indexCache.add(p2);
			m_indexCache.add(p2);
			m_indexCache.add(p1);
			m_indexCache.add(p3);
		}
	}
}


//==============================================================================
// ShapesRenderFeature2

ShapesRenderFeature2::ShapesRenderFeature2()
	: m_manager(nullptr)
	, m_batchData()
{
}

void ShapesRenderFeature2::init(RenderingManager* manager)
{
	m_manager = manager;
	m_batchData.indexOffset = 0;
	m_batchData.indexCount = 0;

	m_vertexLayout = m_manager->standardVertexDeclaration();
	m_vertexBuffer = makeObject<VertexBuffer>(4096 * sizeof(Vertex), GraphicsResourceUsage::Dynamic);
	m_indexBuffer = makeObject<IndexBuffer>(4096, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
}

RequestBatchResult ShapesRenderFeature2::requestDrawCommandList(ShapesRendererCommandList* commandList)
{
	ShapesRendererCommandList::ListNode* node = commandList->head;
	while (node)
	{
		{
			m_shapeBuilder.reset();

			switch (node->type)
			{
				case ShapesRendererCommandList::Cmd_DrawBoxBackground:
				{
					auto* cmd = reinterpret_cast<ShapesRendererCommandList::DrawBoxBackgroundCommand*>(node);
					{
						BoxElementShapeBaseStyle s;
						s.baseRect = cmd->rect;
						s.transform = cmd->transform;
						s.cornerRadius = cmd->cornerRadius;
						m_shapeBuilder.setBaseRect(s);
					}
					{
						BoxElementShapeBackgroundStyle s;
						s.color = cmd->color;
						m_shapeBuilder.setFillBox(s);
					}
					break;
				}
				case ShapesRendererCommandList::Cmd_DrawBoxBorderLine:
				{
					auto* cmd = reinterpret_cast<ShapesRendererCommandList::DrawBoxBorderLineCommand*>(node);
					{
						BoxElementShapeBaseStyle s;
						s.baseRect = cmd->rect;
						s.transform = cmd->transform;
						s.cornerRadius = cmd->cornerRadius;
						m_shapeBuilder.setBaseRect(s);
					}
					{
						BoxElementShapeBorderStyle s;
						s.borderLeftColor = cmd->leftColor;
						s.borderTopColor = cmd->topColor;
						s.borderRightColor = cmd->rightColor;
						s.borderBottomColor = cmd->bottomColor;
						s.borderThickness = cmd->thickness;
						s.borderInset = cmd->borderInset;
						m_shapeBuilder.setBoxBorderLine(s);
					}
					break;
				}
				case ShapesRendererCommandList::Cmd_DrawBoxShadow:
				{
					auto* cmd = reinterpret_cast<ShapesRendererCommandList::DrawBoxShadowCommand*>(node);
					{
						BoxElementShapeBaseStyle s;
						s.baseRect = cmd->rect;
						s.transform = cmd->transform;
						s.cornerRadius = cmd->cornerRadius;
						m_shapeBuilder.setBaseRect(s);
					}
					{
						BoxElementShapeShadowStyle s;
						s.shadowOffset = cmd->offset;
						s.shadowColor = cmd->color;
						s.shadowWidth = cmd->width;
						s.shadowBlur = cmd->blur;
						s.shadowInset = cmd->inset;
						m_shapeBuilder.setBoxShadow(s);
					}
					break;
				}

				default:
					LN_UNREACHABLE();
					break;
			}



			m_shapeBuilder.build();


			// サイズが足りなければ再作成
			{
				// VertexBuffer
				int vertexBufferRequestSize = (m_vertexUsedCount + m_shapeBuilder.vertexCount()) * sizeof(Vertex);
				if (!m_vertexBuffer) {
					m_vertexBuffer = makeObject<VertexBuffer>(vertexBufferRequestSize, GraphicsResourceUsage::Dynamic);
				}
				else if (m_vertexBuffer->size() < vertexBufferRequestSize) {
					auto newSize = std::max(m_vertexBuffer->size() * 2, vertexBufferRequestSize);
					m_vertexBuffer->resize(newSize);
				}

				// IndexBuffer
				int indexBufferRequestCount = m_indexUsedCount + m_shapeBuilder.indexCount();
				if (!m_indexBuffer) {
					m_indexBuffer = makeObject<IndexBuffer>(indexBufferRequestCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
				}
				else if (m_indexBuffer->size() < indexBufferRequestCount) {
					auto newSize = std::max(m_indexBuffer->size() * 2, indexBufferRequestCount);
					m_indexBuffer->resize(newSize);
				}
			}


			auto vb = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
			auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));
			m_shapeBuilder.writeToBuffer(vb + m_vertexUsedCount, ib + m_indexUsedCount, m_indexUsedCount);


			m_vertexUsedCount += m_shapeBuilder.vertexCount();
			m_indexUsedCount += m_shapeBuilder.indexCount();

			m_batchData.indexCount += m_shapeBuilder.indexCount();

		}
		node = node->next;
	}




	return RequestBatchResult::Staging;
}

RequestBatchResult ShapesRenderFeature2::requestDrawCommandList(BoxElementShapeCommandList* commandList)
{
    BoxElementShapeCommandList::ListNode* node = commandList->head;
    while (node)
    {
        switch (node->type)
        {
            case BoxElementShapeCommandList::Cmd_Reset:
            {
                m_shapeBuilder.reset();
                break;
            }
            case BoxElementShapeCommandList::Cmd_Base:
            {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::BaseCommand*>(node);
                m_shapeBuilder.setBaseRect(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Background:
            {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::BackgroundCommand*>(node);
                m_shapeBuilder.setFillBox(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Border:
            {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::BorderCommand*>(node);
                m_shapeBuilder.setBoxBorderLine(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Shadow:
            {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::ShadowCommand*>(node);
                m_shapeBuilder.setBoxShadow(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Submit:
            {
                m_shapeBuilder.build();

                // サイズが足りなければ再作成
                {
                    // VertexBuffer
                    int vertexBufferRequestSize = (m_vertexUsedCount + m_shapeBuilder.vertexCount()) * sizeof(Vertex);
                    if (!m_vertexBuffer) {
                        m_vertexBuffer = makeObject<VertexBuffer>(vertexBufferRequestSize, GraphicsResourceUsage::Dynamic);
                    }
                    else if (m_vertexBuffer->size() < vertexBufferRequestSize) {
                        auto newSize = std::max(m_vertexBuffer->size() * 2, vertexBufferRequestSize);
                        m_vertexBuffer->resize(newSize);
                    }

                    // IndexBuffer
                    int indexBufferRequestCount = m_indexUsedCount + m_shapeBuilder.indexCount();
                    if (!m_indexBuffer) {
                        m_indexBuffer = makeObject<IndexBuffer>(indexBufferRequestCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
                    }
                    else if (m_indexBuffer->size() < indexBufferRequestCount) {
                        auto newSize = std::max(m_indexBuffer->size() * 2, indexBufferRequestCount);
                        m_indexBuffer->resize(newSize);
                    }
                }


                auto vb = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
                auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));
                m_shapeBuilder.writeToBuffer(vb + m_vertexUsedCount, ib + m_indexUsedCount, m_vertexUsedCount);


                m_vertexUsedCount += m_shapeBuilder.vertexCount();
                m_indexUsedCount += m_shapeBuilder.indexCount();

                m_batchData.indexCount += m_shapeBuilder.indexCount();
                break;
            }
            default:
                LN_UNREACHABLE();
                break;
        }

        node = node->next;
    }

    return RequestBatchResult::Staging;
}

void ShapesRenderFeature2::beginRendering()
{
	m_batchData.indexOffset = 0;
	m_batchData.indexCount = 0;
	m_vertexUsedCount = 0;
	m_indexUsedCount = 0;
}

void ShapesRenderFeature2::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	//if (m_mappedVertices) {
	//	// TODO: unmap (今は自動だけど、明示した方が安心かも)
	//}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.indexOffset = m_batchData.indexOffset + m_batchData.indexCount;
	m_batchData.indexCount = 0;
}

void ShapesRenderFeature2::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);



	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.indexOffset, localBatch->data.indexCount / 3);
}



} // namespace detail
} // namespace ln

