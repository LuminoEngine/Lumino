
#include "Internal.hpp"
#include <LuminoGraphics/VertexLayout.hpp>
#include <LuminoGraphics/VertexBuffer.hpp>
#include <LuminoGraphics/IndexBuffer.hpp>
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Rendering/Vertex.hpp>
#include "../../../../Graphics/src/RHIs/GraphicsDeviceContext.hpp"
#include "../../../../Graphics/src/GraphicsManager.hpp"
#include "../../Rendering/RenderingManager.hpp"
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

static const int g_finalOffset = 0.0;

#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
#else
//==============================================================================
// ShapesRenderFeature


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
RequestBatchResult ShapesRenderFeature::requestDrawCommandList(GraphicsCommandList* context, ShapesRendererCommandList* commandList/*, detail::BrushRawData* fillBrush*/)
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


			auto vb = static_cast<Vertex*>(m_vertexBuffer->writableData());
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

void ShapesRenderFeature::submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList)
{
	//if (m_mappedVertices) {
	//	// TODO: unmap (今は自動だけど、明示した方が安心かも)
	//}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.indexOffset = m_batchData.indexOffset + m_batchData.indexCount;
	m_batchData.indexCount = 0;
}

void ShapesRenderFeature::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch)
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

//void ShapesRenderFeature::prepareBuffers(GraphicsCommandList* context, int triangleCount)
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
        Vertex v(Vector3::transformCoord(
            Vector3(pt.pos + g_finalOffset, 0), *path.transform), Vector3::UnitZ, Vector2::Zero, 
            path.color.withAlpha(path.color.a * pt.alpha));
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
		m_vertexCache.getAt(startIndex + i).setPosition(Vector3::transformCoord(Vector3(pt.pos + g_finalOffset, 0), *path.transform));

		Vertex v;
		v.setPosition(Vector3::transformCoord(Vector3(pt.pos + extDir * extAA + g_finalOffset, 0), *path.transform));// , *path.transform;
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
#endif

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

void ShapesRenderFeature::renderCommandList(GraphicsCommandList* context, const ShapesRendererCommandList& commandList)
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

void ShapesRenderFeature::submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList)
{
	// TODO:
}

void ShapesRenderFeature::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch)
{
	LN_NOTIMPLEMENTED();
}
#endif



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
// BoxElementShapeBuilderCommon

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
static void Utils_plotCornerPointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center, TCallback callback)
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

template<class TCallback>
static void Utils_plotCornerPointsBezier2(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center, int tess, TCallback callback)
{
    assert(firstT < lastT);
    //const int tess = 8;
    //const int tess = (int)Math::clamp((std::max(std::abs(last.x - first.x), std::abs(last.y - first.y)) / 2) * (lastT - firstT), 2.0f, 8.0f);
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

void BoxElementShapeBuilderCommon::resetComon()
{
    m_baseStyle.transform = Matrix::Identity;
    m_baseStyle.baseRect = Rect::Zero;
    m_baseStyle.cornerRadius = CornerRadius::Zero;
    m_baseStyle.aligndLineAntiAlias = false;
    m_backgroundEnabled = false;
    m_borderEnabled = false;
    m_shadowEnabled = false;

    for (int iCmp = 0; iCmp < 4; iCmp++) m_commonComponents[iCmp].width = 0.0f;

    m_outlinePointBuffer.clear();
    m_outlinePaths.clear();
    m_outlineIndices.clear();

    m_vertexCache.clear();
    m_indexCache.clear();
}

void BoxElementShapeBuilderCommon::setBaseRect(const BoxElementShapeBaseStyle& style)
{
    m_baseStyle = style;
}

void BoxElementShapeBuilderCommon::setFillBox(const BoxElementShapeBackgroundStyle& style)
{
    m_backgroundStyle = style;
    m_backgroundEnabled = true;
}

void BoxElementShapeBuilderCommon::setBoxBorderLine(const BoxElementShapeBorderStyle& style)
{
    m_borderStyle = style;
    m_borderEnabled = true;
}

void BoxElementShapeBuilderCommon::setBoxShadow(const BoxElementShapeShadowStyle& style)
{
    m_shadowStyle = style;
    m_shadowEnabled = true;
}

int BoxElementShapeBuilderCommon::vertexCount() const
{
    return m_vertexCache.getCount();
    //return 4;
}

int BoxElementShapeBuilderCommon::indexCount() const
{
    return m_indexCache.getCount();
    //return 6;
}

void BoxElementShapeBuilderCommon::writeToBuffer(Vertex* vertexBuffer, uint16_t* indexBuffer, uint16_t indexOffset)
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

void BoxElementShapeBuilderCommon::setupGuideline()
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

    //Rect actualBaseRect = m_baseStyle.baseRect;

    //// ピクセルスナップを利かせないと、Corner が歪んで見得てしまうため、修正を入れる
    //actualBaseRect.x = std::floor(actualBaseRect.x);
    //actualBaseRect.y = std::floor(actualBaseRect.y);

    // Make shape outer rect
    // Shape の外周となる shape outer 矩形を確定する
    {
        m_shapeOuterGuide.rect = m_baseStyle.baseRect;
        m_shapeOuterGuide.rect.width = std::max(0.0f, m_shapeOuterGuide.rect.width);
        m_shapeOuterGuide.rect.height = std::max(0.0f, m_shapeOuterGuide.rect.height);
        if (m_borderEnabled) {
            if (m_borderStyle.borderInset) {
                // Note: Box のサイズは、Border の合計サイズよりも小さくならない。
                // (Style で width: 0px とかはできるが、表示時に調整される)
                m_shapeOuterGuide.rect.width = std::max(m_shapeOuterGuide.rect.width, m_borderStyle.borderThickness.width());
                m_shapeOuterGuide.rect.height = std::max(m_shapeOuterGuide.rect.height, m_borderStyle.borderThickness.height());
            }
            else {
                m_shapeOuterGuide.rect.width += m_borderStyle.borderThickness.width();
                m_shapeOuterGuide.rect.height += m_borderStyle.borderThickness.height();
            }
        }

        m_shapeOuterGuide.corner = m_baseStyle.cornerRadius;

        // Radius をサイズ内に収まるように調整する
        ajustGuidelineCorners(&m_shapeOuterGuide);
    }

    // ShapeInnerRect を確定する
    {
        const float tlRad = m_shapeOuterGuide.corner.topLeft;
        const float trRad = m_shapeOuterGuide.corner.topRight;
        const float blRad = m_shapeOuterGuide.corner.bottomLeft;
        const float brRad = m_shapeOuterGuide.corner.bottomRight;
        const float tw = (m_borderEnabled) ? m_borderStyle.borderThickness.top : 0.0f;
        const float rw = (m_borderEnabled) ? m_borderStyle.borderThickness.right : 0.0f;
        const float bw = (m_borderEnabled) ? m_borderStyle.borderThickness.bottom : 0.0f;
        const float lw = (m_borderEnabled) ? m_borderStyle.borderThickness.left : 0.0f;

        // top-left
        if (lw >= tlRad || tw >= tlRad)	// どちらかの BorderThickness が Radius を超えている場合、inner の角を丸める必要はない
            m_shapeInnerGuide.corner.topLeft = 0;
        else                            // border の押し出し分だけ半径を調整する
            m_shapeInnerGuide.corner.topLeft = std::max(m_shapeOuterGuide.corner.topLeft - std::max(lw, tw), 0.0f);
        // top-right
        if (tw >= trRad || rw >= trRad)
            m_shapeInnerGuide.corner.topRight = 0;
        else
            m_shapeInnerGuide.corner.topRight = std::max(m_shapeOuterGuide.corner.topRight - std::max(tw, rw), 0.0f);
        // bottom-right
        if (rw >= brRad || bw >= brRad)
            m_shapeInnerGuide.corner.bottomRight = 0;
        else
            m_shapeInnerGuide.corner.bottomRight = std::max(m_shapeOuterGuide.corner.bottomRight - std::max(rw, bw), 0.0f);
        // bottom-left
        if (bw >= blRad || lw >= blRad)
            m_shapeInnerGuide.corner.bottomLeft = 0;
        else
            m_shapeInnerGuide.corner.bottomLeft = std::max(m_shapeOuterGuide.corner.bottomLeft - std::max(bw, lw), 0.0f);

        if (m_borderEnabled)
            m_shapeInnerGuide.rect = m_shapeOuterGuide.rect.makeDeflate(m_borderStyle.borderThickness);
        else
            m_shapeInnerGuide.rect = m_shapeOuterGuide.rect;

        ajustGuidelineCorners(&m_shapeInnerGuide);
    }

    if (m_borderEnabled) {
        m_commonComponents[Top].width = m_shapeInnerGuide.rect.getTop() - m_shapeOuterGuide.rect.getTop();
        m_commonComponents[Top].color = m_borderStyle.borderTopColor;
        m_commonComponents[Right].width = m_shapeOuterGuide.rect.getRight() - m_shapeInnerGuide.rect.getRight();
        m_commonComponents[Right].color = m_borderStyle.borderRightColor;
        m_commonComponents[Bottom].width = m_shapeOuterGuide.rect.getBottom() - m_shapeInnerGuide.rect.getBottom();
        m_commonComponents[Bottom].color = m_borderStyle.borderBottomColor;
        m_commonComponents[Left].width = m_shapeInnerGuide.rect.getLeft() - m_shapeOuterGuide.rect.getLeft();
        m_commonComponents[Left].color = m_borderStyle.borderLeftColor;
        assert(m_commonComponents[Top].width >= 0.0f);
        assert(m_commonComponents[Right].width >= 0.0f);
        assert(m_commonComponents[Bottom].width >= 0.0f);
        assert(m_commonComponents[Left].width >= 0.0f);
    }

    // Outer shadow
    if (outerShadowEnabled())
    {
        const float hw = m_shadowStyle.shadowBlur / 2;

        GuideArea baseGuide;
        baseGuide.rect = m_shapeOuterGuide.rect.makeInflate(Thickness(m_shadowStyle.shadowWidth));
        //baseGuide.corner = m_shapeOuterGuide.corner;
        baseGuide.corner.topLeft = (m_shapeOuterGuide.corner.topLeft > 0.0f) ? (m_shapeOuterGuide.corner.topLeft + m_shadowStyle.shadowWidth) : hw;
        baseGuide.corner.topRight = (m_shapeOuterGuide.corner.topRight > 0.0f) ? (m_shapeOuterGuide.corner.topRight + m_shadowStyle.shadowWidth) : hw;
        baseGuide.corner.bottomRight = (m_shapeOuterGuide.corner.bottomRight > 0.0f) ? (m_shapeOuterGuide.corner.bottomRight + m_shadowStyle.shadowWidth) : hw;
        baseGuide.corner.bottomLeft = (m_shapeOuterGuide.corner.bottomLeft > 0.0f) ? (m_shapeOuterGuide.corner.bottomLeft + m_shadowStyle.shadowWidth) : hw;
        ajustGuidelineCorners(&baseGuide);

        // NearGuide は Blur が大きい場合、ShapeOuter よりも小さい矩形となることがある
        m_outerShadowNearGuide.rect = baseGuide.rect.makeDeflate(Thickness(hw));
        //m_outerShadowNearGuide.corner = baseGuide.corner;
        m_outerShadowNearGuide.corner.topLeft = std::max(baseGuide.corner.topLeft - hw, 0.0f);
        m_outerShadowNearGuide.corner.topRight = std::max(baseGuide.corner.topRight - hw, 0.0f);
        m_outerShadowNearGuide.corner.bottomRight = std::max(baseGuide.corner.bottomRight - hw, 0.0f);
        m_outerShadowNearGuide.corner.bottomLeft = std::max(baseGuide.corner.bottomLeft - hw, 0.0f);

        m_outerShadowFarGuide.rect = baseGuide.rect.makeInflate(Thickness(hw));
        m_outerShadowFarGuide.corner.topLeft = baseGuide.corner.topLeft + hw;
        m_outerShadowFarGuide.corner.topRight = baseGuide.corner.topRight + hw;
        m_outerShadowFarGuide.corner.bottomRight = baseGuide.corner.bottomRight + hw;
        m_outerShadowFarGuide.corner.bottomLeft = baseGuide.corner.bottomLeft + hw;
    }

    // Inner shadow
    if (insetShadowEnabled())
    {
        const float hw = m_shadowStyle.shadowBlur / 2;

        // Innre shadow の Corner は InnerRect の Corner の影響を受けるが、
        // 影響を受けるのは InnerRect と NearGuide の幅が shadowWidth を割ったときだけ。
        // shadowWidth がたくさんある場合は InnerRect の Corner をちょっと変えても、NearGuide の Corner は変わらない。
        GuideArea baseGuide;
        baseGuide.rect = m_shapeInnerGuide.rect.makeDeflate(Thickness(m_shadowStyle.shadowWidth));
        baseGuide.corner.topLeft = (m_shapeInnerGuide.corner.topLeft > m_shadowStyle.shadowWidth) ? m_shapeInnerGuide.corner.topLeft - m_shadowStyle.shadowWidth : 0.0f;
        baseGuide.corner.topRight = (m_shapeInnerGuide.corner.topRight > m_shadowStyle.shadowWidth) ? m_shapeInnerGuide.corner.topRight - m_shadowStyle.shadowWidth : 0.0f;
        baseGuide.corner.bottomRight = (m_shapeInnerGuide.corner.bottomRight > m_shadowStyle.shadowWidth) ? m_shapeInnerGuide.corner.bottomRight - m_shadowStyle.shadowWidth : 0.0f;
        baseGuide.corner.bottomLeft = (m_shapeInnerGuide.corner.bottomLeft > m_shadowStyle.shadowWidth) ? m_shapeInnerGuide.corner.bottomLeft - m_shadowStyle.shadowWidth : 0.0f;

        // NearGuide は Blur が大きい場合、ShapeInner よりも大きい矩形となることがある
        const float near = m_shadowStyle.shadowWidth - hw;
        m_innerShadowNearGuide.rect = baseGuide.rect.makeInflate(Thickness(hw));
        m_innerShadowNearGuide.corner.topLeft = baseGuide.corner.topLeft + hw;
        m_innerShadowNearGuide.corner.topRight = baseGuide.corner.topRight + hw;
        m_innerShadowNearGuide.corner.bottomRight = baseGuide.corner.bottomRight + hw;
        m_innerShadowNearGuide.corner.bottomLeft = baseGuide.corner.bottomLeft + hw;
        ajustGuidelineCorners(&m_innerShadowNearGuide);

        // FarGuide は Shadow が角張って見えるのを防ぐため、Corner の基本は shadowWidth とする
        const float far = m_shadowStyle.shadowWidth + hw;
        m_innerShadowFarGuide.rect = baseGuide.rect.makeDeflate(Thickness(hw));
        if (m_innerShadowFarGuide.rect.width <= 0.0f) m_innerShadowFarGuide.rect.x = m_innerShadowNearGuide.rect.x + m_innerShadowNearGuide.rect.width / 2;
        if (m_innerShadowFarGuide.rect.height <= 0.0f) m_innerShadowFarGuide.rect.y = m_innerShadowNearGuide.rect.y + m_innerShadowNearGuide.rect.height / 2;

        m_innerShadowFarGuide.corner.topLeft = m_shadowStyle.shadowWidth;
        m_innerShadowFarGuide.corner.topRight = m_shadowStyle.shadowWidth;
        m_innerShadowFarGuide.corner.bottomRight = m_shadowStyle.shadowWidth;
        m_innerShadowFarGuide.corner.bottomLeft = m_shadowStyle.shadowWidth;
        ajustGuidelineCorners(&m_innerShadowFarGuide);
    }

    assert(m_shapeOuterGuide.verify());
    assert(m_shapeInnerGuide.verify());
    assert(m_outerShadowNearGuide.verify());
    assert(m_outerShadowFarGuide.verify());
    assert(m_innerShadowNearGuide.verify());
    assert(m_innerShadowFarGuide.verify());
}

void BoxElementShapeBuilderCommon::ajustGuidelineCorners(GuideArea* guide)
{
    float l = std::max(guide->corner.topLeft, guide->corner.bottomLeft);
    float r = std::max(guide->corner.topRight, guide->corner.bottomRight);
    float t = std::max(guide->corner.topLeft, guide->corner.topRight);
    float b = std::max(guide->corner.bottomLeft, guide->corner.bottomRight);
    float radiusMaxWidth = l + r;
    float radiusMaxHeight = t + b;

    if (guide->rect.width < radiusMaxWidth) {
        float lr = l / radiusMaxWidth;
        float rr = r / radiusMaxWidth;
        guide->corner.topLeft = std::min(guide->rect.width * lr, guide->corner.topLeft);
        guide->corner.bottomLeft = std::min(guide->rect.width * lr, guide->corner.bottomLeft);
        guide->corner.topRight = std::min(guide->rect.width * rr, guide->corner.topRight);
        guide->corner.bottomRight = std::min(guide->rect.width * rr, guide->corner.bottomRight);
    }

    if (guide->rect.height < radiusMaxHeight) {
        float tr = t / radiusMaxHeight;
        float br = b / radiusMaxHeight;
        guide->corner.topLeft = std::min(guide->rect.height * tr, guide->corner.topLeft);
        guide->corner.topRight = std::min(guide->rect.height * tr, guide->corner.topRight);
        guide->corner.bottomLeft = std::min(guide->rect.height * br, guide->corner.bottomLeft);
        guide->corner.bottomRight = std::min(guide->rect.height * br, guide->corner.bottomRight);
    }
}

int BoxElementShapeBuilderCommon::addOutlinePoint(const Vector2& pos, const Vector2& infrateDir, float cornerRatio, Corner cornerGroup)
{
    m_outlinePointBuffer.add({ pos, Color::Black, {infrateDir, infrateDir}, infrateDir, cornerRatio, cornerGroup });
    return m_outlinePointBuffer.getCount() - 1;
}

BoxElementShapeBuilderCommon::OutlinePath* BoxElementShapeBuilderCommon::beginOutlinePath(OutlinePathType type, PathWinding winding)
{
    m_outlinePaths.add(OutlinePath{ type, m_outlineIndices.getCount(), 0, winding });
    return &m_outlinePaths.back();
}

void BoxElementShapeBuilderCommon::endOutlinePath(OutlinePath* path)
{
    path->indexCount = m_outlineIndices.getCount() - path->indexStart;
}

int BoxElementShapeBuilderCommon::addOutlineIndex(int pointId)
{
    m_outlineIndices.add(pointId);
    return m_outlineIndices.getCount();
}

void BoxElementShapeBuilderCommon::makeOutlineAntiAlias(const OutlinePath* path/*, int start, int count*/)
{
    const float length = 1.0;
    int startIndex = path->indexStart;
    int indexCount = path->indexCount;

    for (int iAA = 0; iAA < path->antiAliasCount; iAA++) {
        auto* aaPath = beginOutlinePath(OutlinePathType::Stripe, PathWinding::CW);
        aaPath->stripeClosing = true;
        for (int i = 0; i < indexCount; i++) {
            int idIndex = startIndex + i;
            auto& pt = outlinePoint(outlineIndex(idIndex));

            Vector2 pos = pt.pos + (pt.antiAliasDir[iAA] * length);
            if (!m_baseStyle.aligndLineAntiAlias) {
                float d = std::acos(std::abs(pt.rightDir.x)) / (Math::PIDiv2);	// 0.0(dig0) ~ 1.0(dig90) になる
                d = std::abs((d - 0.5f) * 2.0f);										// dig45 に近ければ 0.0, dig0 か dig90 に近ければ 1.0
                pos = Vector2::lerp(pos, pt.pos, d);
            }

            int newId = addOutlinePoint(pos, pt.rightDir, pt.cornerRatio, pt.cornerGroup);
            outlinePoint(newId).color = pt.color.withAlpha(0.0f);

            addOutlineIndex(newId);
            addOutlineIndex(outlineIndex(idIndex));
        }
        endOutlinePath(aaPath);
    }
}

void BoxElementShapeBuilderCommon::expandPathes()
{
    for (int i = 0; i < m_outlinePointBuffer.getCount(); i++)
    {
        const OutlinePoint& pt = m_outlinePointBuffer.getAt(i);
        Vertex v(
            Vector3::transformCoord(Vector3(pt.pos + g_finalOffset, 0), m_baseStyle.transform),
            Vector3::UnitZ, Vector2::Zero, pt.color);
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

void BoxElementShapeBuilderCommon::expandConvex(const OutlinePath& path)
{
    // make IndexBuffer (反時計回り)
    int ib = path.indexStart;
    int i0 = 0;
    int i1 = 1;
    int i2 = path.indexCount - 1;
    for (int iPt = 0; iPt < path.indexCount - 2; iPt++)
    {
#ifdef LN_COORD_RH
        if (path.winding == PathWinding::CW)
        {
            m_indexCache.add(outlineIndex(ib + i0));
            m_indexCache.add(outlineIndex(ib + i2));
            m_indexCache.add(outlineIndex(ib + i1));
        }
        else
        {
            m_indexCache.add(outlineIndex(ib + i0));
            m_indexCache.add(outlineIndex(ib + i1));
            m_indexCache.add(outlineIndex(ib + i2));
        }
#else
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
#endif

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

void BoxElementShapeBuilderCommon::expandStripeStroke(const OutlinePath& path)
{
    /*
        0-1-2
        |/|/|	-> front
        x-y-z
    */

    for (int i = 0; i < (path.indexCount / 2) - 1; i++)
    {
        //int ib = path.pointStart + i;// TODO: 頂点indexは OutlinePoint に持たせた方がいいかな。今は m_outlinePointBuffer.add した順に並ぶことを前提としている

        int p0 = outlineIndex(path.indexStart + (i * 2) + 0);
        int p1 = outlineIndex(path.indexStart + (i * 2) + 2);
        int p2 = outlineIndex(path.indexStart + (i * 2) + 1);   // x
        int p3 = outlineIndex(path.indexStart + (i * 2) + 3);   // y
#ifdef LN_COORD_RH
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
#else
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
#endif
    }

    if (path.stripeClosing) {
        /*
            0-1
            |/|
            2-3
        */
        // TODO: 頂点indexは OutlinePoint に持たせた方がいいかな。今は m_outlinePointBuffer.add した順に並ぶことを前提としている
        int p0 = outlineIndex(path.indexStart + (0));
        int p1 = outlineIndex(path.indexStart + (path.indexCount - 2));
        int p2 = outlineIndex(path.indexStart + (1));
        int p3 = outlineIndex(path.indexStart + (path.indexCount - 1));
#ifdef LN_COORD_RH
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
#else
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
#endif
    }
}

bool BoxElementShapeBuilderCommon::GuideArea::verify() const
{
    return
        (rect.width >= 0) &&
        (rect.height >= 0) &&
        (corner.topLeft >= 0) &&
        (corner.topRight >= 0) &&
        (corner.bottomRight >= 0) &&
        (corner.bottomLeft >= 0) &&
        (corner.topLeft + corner.topRight <= rect.width) &&
        (corner.bottomLeft + corner.bottomRight <= rect.width) &&
        (corner.topLeft + corner.bottomLeft <= rect.height) &&
        (corner.topRight + corner.bottomRight <= rect.height);
}

#if 0
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


BoxElementShapeBuilder2::BoxElementShapeBuilder2()
{
}

void BoxElementShapeBuilder2::init()
{
	reset();
}

void BoxElementShapeBuilder2::reset()
{
    resetComon();

	m_outlinePointBuffer.clear();
	m_outlinePaths.clear();
	m_outlineIndices.clear();
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
                makeStripePath(farShadowComponents, nearShadowComponents, PathWinding::CCW);
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

void BoxElementShapeBuilder2::setupBaseRects()
{
    setupGuideline();

    m_shapeOuterRect = m_shapeOuterGuide;


	// Round の合計よりも Shape のサイズが小さい場合の対策。比率によって shape outer の Round サイズを調整する
	{
		m_shapeOuterRect.corner = m_baseStyle.cornerRadius;
        ajustGuidelineCorners(&m_shapeOuterRect);
		//float l = std::max(m_shapeOuterRect.corner.topLeft, m_shapeOuterRect.corner.bottomLeft);
		//float r = std::max(m_shapeOuterRect.corner.topRight, m_shapeOuterRect.corner.bottomRight);
		//float t = std::max(m_shapeOuterRect.corner.topLeft, m_shapeOuterRect.corner.topRight);
		//float b = std::max(m_shapeOuterRect.corner.bottomLeft, m_shapeOuterRect.corner.bottomRight);
		//float radiusMaxWidth = l + r;
		//float radiusMaxHeight = t + b;

  //      CornerRadius orgRads = m_shapeOuterRect.corner;

		//if (m_shapeOuterRect.rect.width < radiusMaxWidth) {
		//	float lr = l / radiusMaxWidth;
		//	float rr = r / radiusMaxWidth;
		//	m_shapeOuterRect.corner.topLeft = std::min(m_shapeOuterRect.rect.width * lr, m_shapeOuterRect.corner.topLeft);
		//	m_shapeOuterRect.corner.bottomLeft = std::min(m_shapeOuterRect.rect.width * lr, m_shapeOuterRect.corner.bottomLeft);
		//	m_shapeOuterRect.corner.topRight = std::min(m_shapeOuterRect.rect.width * rr, m_shapeOuterRect.corner.topRight);
		//	m_shapeOuterRect.corner.bottomRight = std::min(m_shapeOuterRect.rect.width * rr, m_shapeOuterRect.corner.bottomRight);
		//}

		//if (m_shapeOuterRect.rect.height < radiusMaxHeight) {
		//	float tr = t / radiusMaxHeight;
		//	float br = b / radiusMaxHeight;
		//	m_shapeOuterRect.corner.topLeft = std::min(m_shapeOuterRect.rect.height * tr, m_shapeOuterRect.corner.topLeft);
		//	m_shapeOuterRect.corner.topRight = std::min(m_shapeOuterRect.rect.height * tr, m_shapeOuterRect.corner.topRight);
		//	m_shapeOuterRect.corner.bottomLeft = std::min(m_shapeOuterRect.rect.height * br, m_shapeOuterRect.corner.bottomLeft);
		//	m_shapeOuterRect.corner.bottomRight = std::min(m_shapeOuterRect.rect.height * br, m_shapeOuterRect.corner.bottomRight);
		//}

        // 縮小された場合、どの程度の割合であるかを覚えておく。m_shapeOuterRect 以外の Path での Corner の最大値の計算に使用する。
        //m_cornerRatios.topLeft = m_shapeOuterRect.corner.topLeft / orgRads.topLeft;
        //m_cornerRatios.topRight = m_shapeOuterRect.corner.topRight / orgRads.topRight;
        //m_cornerRatios.bottomLeft = m_shapeOuterRect.corner.bottomLeft / orgRads.bottomLeft;
        //m_cornerRatios.bottomRight = m_shapeOuterRect.corner.bottomRight / orgRads.bottomRight;
	}

	// ShapeInnerRect を確定する
	{
        m_shapeInnerRect = m_shapeInnerGuide;
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
		Vertex v(Vector3::transformCoord(Vector3(pt.pos + g_finalOffset, 0), m_baseStyle.transform), Vector3::UnitZ, Vector2::Zero, pt.color);
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
#ifdef LN_COORD_RH
        if (path.winding == PathWinding::CW)
        {
            m_indexCache.add(outlineIndex(ib + i0));
            m_indexCache.add(outlineIndex(ib + i2));
            m_indexCache.add(outlineIndex(ib + i1));
        }
        else
        {
            m_indexCache.add(outlineIndex(ib + i0));
            m_indexCache.add(outlineIndex(ib + i1));
            m_indexCache.add(outlineIndex(ib + i2));
        }
#else
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
#endif

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
#ifdef LN_COORD_RH
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
#else
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
#endif
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
#ifdef LN_COORD_RH
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
#else
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
#endif
	}
}
#endif


//==============================================================================
// BoxElementShapeBuilder3

BoxElementShapeBuilder3::BoxElementShapeBuilder3()
{}

void BoxElementShapeBuilder3::init()
{
    reset();
}

void BoxElementShapeBuilder3::reset()
{
    resetComon();
}

void BoxElementShapeBuilder3::build()
{
    setupGuideline();

    if (outerShadowEnabled()) {
        if (m_shapeOuterGuide.corner.topLeft == 0.0f)
            m_shadowtess[TopLeft] = (int)Math::clamp(m_outerShadowFarGuide.corner.topLeft / 2, 1, 8);
        else
            m_shadowtess[TopLeft] = (int)Math::clamp(m_shapeOuterGuide.corner.topLeft * 2, 1, 8);

        if (m_shapeOuterGuide.corner.topRight == 0.0f)
            m_shadowtess[TopRight] = (int)Math::clamp(m_outerShadowFarGuide.corner.topRight / 2, 1, 8);
        else
            m_shadowtess[TopRight] = (int)Math::clamp(m_shapeOuterGuide.corner.topRight * 2, 1, 8);

        if (m_shapeOuterGuide.corner.bottomRight == 0.0f)
            m_shadowtess[BottomRight] = (int)Math::clamp(m_outerShadowFarGuide.corner.bottomRight / 2, 1, 8);
        else
            m_shadowtess[BottomRight] = (int)Math::clamp(m_shapeOuterGuide.corner.bottomRight * 2, 1, 8);

        if (m_shapeOuterGuide.corner.bottomLeft == 0.0f)
            m_shadowtess[BottomLeft] = (int)Math::clamp(m_outerShadowFarGuide.corner.bottomLeft / 2, 1, 8);
        else
            m_shadowtess[BottomLeft] = (int)Math::clamp(m_shapeOuterGuide.corner.bottomLeft * 2, 1, 8);
    }
    else if (insetShadowEnabled()) {
        m_shadowtess[TopLeft] = (int)Math::clamp(m_innerShadowNearGuide.corner.topLeft / 2, 1, 8);
        m_shadowtess[TopRight] = (int)Math::clamp(m_innerShadowNearGuide.corner.topRight / 2, 1, 8);
        m_shadowtess[BottomRight] = (int)Math::clamp(m_innerShadowNearGuide.corner.bottomRight / 2, 1, 8);
        m_shadowtess[BottomLeft] = (int)Math::clamp(m_innerShadowNearGuide.corner.bottomLeft / 2, 1, 8);
    }

    // Outer shadow
    if (outerShadowEnabled()) {

        PointIdRange shaprRange;
        makeShadowoutlinePoints(m_shapeOuterGuide, 1.0f, &shaprRange);
        applyColors(shaprRange, m_shadowStyle.shadowColor);

        PointIdRange nearRange;
        makeShadowoutlinePoints(m_outerShadowNearGuide, 1.0f, &nearRange);


#if 1
        applyColors(nearRange, m_shadowStyle.shadowColor);

        const float hb = (m_shadowStyle.shadowBlur / 2);
        for (int i = 0; i < shaprRange.countId; i++) {
            auto& vPt = outlinePoint(shaprRange.startId + i);
            auto& nPt = outlinePoint(nearRange.startId + i);

            //Plane plane(Vector3(nPt))

#if 1
            //float dx = std::abs(nPt.pos.x - vPt.pos.x);
            //float dy = std::abs(nPt.pos.y - vPt.pos.y);



            Vector2 org = nPt.pos;

            Vector2 dir;
            Vector2 ratio = Vector2::Ones;
            bool test = false;

            if (nPt.cornerGroup == TopLeft || nPt.cornerGroup == BottomLeft) {
                if (nPt.pos.x > vPt.pos.x) {
                    dir.x = nPt.pos.x - vPt.pos.x;
                    nPt.pos.x = vPt.pos.x;
                    ratio.x = nPt.antiAliasDir[0].x;
                    test = true;
                }
            }
            if (nPt.cornerGroup == TopRight || nPt.cornerGroup == BottomRight) {
                if (nPt.pos.x < vPt.pos.x) {
                    dir.x = nPt.pos.x - vPt.pos.x;
                    nPt.pos.x = vPt.pos.x;
                    ratio.x = nPt.antiAliasDir[0].x;
                    test = true;
                }
            }
            if (nPt.cornerGroup == TopLeft || nPt.cornerGroup == TopRight) {
                if (nPt.pos.y > vPt.pos.y) {
                    dir.y = nPt.pos.y - vPt.pos.y;
                    nPt.pos.y = vPt.pos.y;
                    ratio.y = nPt.antiAliasDir[0].y;
                    test = true;
                }
            }
            if (nPt.cornerGroup == BottomLeft || nPt.cornerGroup == BottomRight) {
                if (nPt.pos.y < vPt.pos.y) {
                    dir.y = nPt.pos.y - vPt.pos.y;
                    nPt.pos.y = vPt.pos.y;
                    ratio.y = nPt.antiAliasDir[0].y;
                    test = true;
                }
            }

            if (dir != Vector2::Zero) {
                float d = dir.length();
                //float a = Math::clamp01(1.0f - (hb + d / hb));
                float a = Math::clamp01(1.0f - d / m_shadowStyle.shadowBlur);
                nPt.color.a *= a;
                //nPt.color = Color::Blue;

                // 次のオフセット調整で、shape の内側に入り込んだ near が外に出ることに備えて、shape の色を合わせておく
                // こうしないと、はみ出たときに境界付近だけ色が濃くなる。
                vPt.color = nPt.color;
            }
            //if (ratio != Vector2::Ones) {
            //    float dx = std::abs(nPt.pos.x - org.x);
            //    float dy = std::abs(nPt.pos.y - org.y);

            //    float ax = Math::clamp01(1.0f - dx / hb);
            //    float ay = Math::clamp01(1.0f - dy / hb);

            //    float a = (ax * std::abs(ratio.x)) + (ay * std::abs(ratio.y));
            //    nPt.color.a *= a;
            //    //nPt.color = Color::Blue;
            //}

            //if (test) {
            //    float e = std::abs(Vector2::dot(Vector2::normalize(m_shadowStyle.shadowOffset), vPt.antiAliasDir[0]));//1.0f;//
            //    Plane plane(Vector3(vPt.pos, 0), Vector3(vPt.antiAliasDir[0], 0));
            //    float d = std::abs(plane.getDistanceToPoint(Vector3(org, 0)));  // 内側に入っているはずなのでマイナスを消す
            //    float a = Math::clamp01(1.0f - d / m_shadowStyle.shadowBlur);

            //    nPt.color.a = Math::lerp(nPt.color.a, a, e);
            //    //nPt.color.a = a * e;
            //}
#endif
        }

        //applyColorsAndOffset(nearRange, m_shadowStyle.shadowColor, m_shadowStyle.shadowOffset);


        if (m_shadowStyle.shadowOffset != Vector2::Zero) {
            for (int i = 0; i < shaprRange.countId; i++) {
                const auto& vPt = outlinePoint(shaprRange.startId + i);
                auto& nPt = outlinePoint(nearRange.startId + i);

                nPt.pos += m_shadowStyle.shadowOffset;


                float e = 1.0f;// std::abs(Vector2::dot(Vector2::normalize(m_shadowStyle.shadowOffset), vPt.antiAliasDir[0]));//1.0f;//
                Plane plane(Vector3(vPt.pos, 0), Vector3(vPt.antiAliasDir[0], 0));
                float d2 = plane.getDistanceToPoint(Vector3(nPt.pos, 0));
                if (d2 < 0.0f) {

                    float d = std::abs(d2);  // 内側に入っているはずなのでマイナスを消す
                    float a = Math::clamp01(1.0f - d / m_shadowStyle.shadowBlur);

                    //nPt.color.a = Math::lerp(nPt.color.a, a, e);
                    nPt.color.a *= a;
                    nPt.pos = vPt.pos;
                }


            }
        }
#else

        const float hb = (m_shadowStyle.shadowBlur / 2);
        for (int i = 0; i < shaprRange.countId; i++) {
            const auto& vPt = outlinePoint(shaprRange.startId + i);
            auto& nPt = outlinePoint(nearRange.startId + i);

            //Plane plane(Vector3(nPt))

#if 1
            //float dx = std::abs(nPt.pos.x - vPt.pos.x);
            //float dy = std::abs(nPt.pos.y - vPt.pos.y);

            Vector2 org = nPt.pos;

            //Vector2 dir;
            Vector2 ratio = Vector2::Ones;
            bool test = false;

            if (nPt.cornerGroup == TopLeft || nPt.cornerGroup == BottomLeft) {
                if (nPt.pos.x > vPt.pos.x) {
                    //dir.x = nPt.pos.x - vPt.pos.x;
                    nPt.pos.x = vPt.pos.x;
                    ratio.x = nPt.antiAliasDir[0].x;
                    test = true;
                }
            }
            if (nPt.cornerGroup == TopRight || nPt.cornerGroup == BottomRight) {
                if (nPt.pos.x < vPt.pos.x) {
                    //dir.x = nPt.pos.x - vPt.pos.x;
                    nPt.pos.x = vPt.pos.x;
                    ratio.x = nPt.antiAliasDir[0].x;
                    test = true;
                }
            }
            if (nPt.cornerGroup == TopLeft || nPt.cornerGroup == TopRight) {
                if (nPt.pos.y > vPt.pos.y) {
                    //dir.y = nPt.pos.y - vPt.pos.y;
                    nPt.pos.y = vPt.pos.y;
                    ratio.y = nPt.antiAliasDir[0].y;
                    test = true;
                }
            }
            if (nPt.cornerGroup == BottomLeft || nPt.cornerGroup == BottomRight) {
                if (nPt.pos.y < vPt.pos.y) {
                    //dir.y = nPt.pos.y - vPt.pos.y;
                    nPt.pos.y = vPt.pos.y;
                    ratio.y = nPt.antiAliasDir[0].y;
                    test = true;
                }
            }

            //if (dir != Vector2::Zero) {
            //    float d = dir.length();
            //    float a = Math::clamp01(1.0f - (hb + d / hb));
            //    nPt.color.a *= a;
            //    //nPt.color = Color::Blue;
            //}
            //if (ratio != Vector2::Ones) {
            //    float dx = std::abs(nPt.pos.x - org.x);
            //    float dy = std::abs(nPt.pos.y - org.y);

            //    float ax = Math::clamp01(1.0f - dx / hb);
            //    float ay = Math::clamp01(1.0f - dy / hb);

            //    float a = (ax * std::abs(ratio.x)) + (ay * std::abs(ratio.y));
            //    nPt.color.a *= a;
            //    //nPt.color = Color::Blue;
            //}

            if (test) {
                float e =  std::abs(Vector2::dot(Vector2::normalize(m_shadowStyle.shadowOffset), vPt.antiAliasDir[0]));//1.0f;//
                Plane plane(Vector3(vPt.pos, 0), Vector3(vPt.antiAliasDir[0], 0));
                float d = std::abs(plane.getDistanceToPoint(Vector3(org, 0)));  // 内側に入っているはずなのでマイナスを消す
                float a = Math::clamp01(1.0f - d / m_shadowStyle.shadowBlur);

                nPt.color.a = Math::lerp(nPt.color.a, a, e);
                //nPt.color.a = a * e;
            }
#endif
        }
#endif

        makeStripePath(nearRange, shaprRange);

        if (m_shadowStyle.shadowBlur > 0.0f) {
            PointIdRange farRange;
            makeShadowoutlinePoints(m_outerShadowFarGuide, 1.0f, &farRange);
            applyColorsAndOffset(farRange, m_shadowStyle.shadowColor.withAlpha(0.0f), m_shadowStyle.shadowOffset);
            makeStripePath(farRange, nearRange);
        }
    }



    if (m_backgroundEnabled)
    {
        ComponentSet backgroundComponentSet;
        if (m_borderEnabled) {
            makeBaseOuterPointsAndBorderComponent(m_shapeInnerGuide, 1.0f, &backgroundComponentSet);
        }
        else {
            makeBaseOuterPointsAndBorderComponent(m_shapeOuterGuide, 1.0f, &backgroundComponentSet);
        }


        auto* path = beginOutlinePath(OutlinePathType::Convex);
        for (int id = backgroundComponentSet.beginIds(); id < backgroundComponentSet.endIds(); id++) {
            addOutlineIndex(id);
            outlinePoint(id).color = m_backgroundStyle.color;
        }
        endOutlinePath(path);


        if (m_shapeAAEnabled) {
            // Border が無ければ、Background の色を使って外周に AA を作る
            if (!m_borderEnabled) {
               // makeOutlineAntiAlias(path);
            }
        }
    }

    if (m_borderEnabled) {

        ComponentSet shapeOuterComponentSet;
        makeBaseOuterPointsAndBorderComponent(m_shapeOuterGuide, 1.0f, &shapeOuterComponentSet);

        ComponentSet shapeInnerComponentSet;
        makeBaseOuterPointsAndBorderComponent(m_shapeInnerGuide, -1.0f, &shapeInnerComponentSet);

        for (int iCmp = 0; iCmp < 4; iCmp++) {
            if (m_commonComponents[iCmp].width > 0.0f) {
                const auto& outerComponent = shapeOuterComponentSet.components[iCmp];
                const auto& innerComponent = shapeInnerComponentSet.components[iCmp];

                //assert(outerComponent.pointIdCount == innerComponent.pointIdCount);

                auto* path = beginOutlinePath(OutlinePathType::Convex, PathWinding::CW);

                for (int id = outerComponent.beginIds(); id < outerComponent.endIds(); id++) {
                    addOutlineIndex(id);
                    outlinePoint(id).color = m_commonComponents[iCmp].color;
                }
                for (int id = innerComponent.endIds() - 1; id >= innerComponent.beginIds(); id--) {
                    addOutlineIndex(id);
                    outlinePoint(id).color = m_commonComponents[iCmp].color;
                }

                //for (int i = 0; i < outerComponent.pointIdCount; i++) {
                //    int oId = outerComponent.pointIdStart + i;
                //    int iId = innerComponent.pointIdStart + i;
                //    addOutlineIndex(oId);
                //    addOutlineIndex(iId);
                //    outlinePoint(oId).color = m_commonComponents[iCmp].color;
                //    outlinePoint(iId).color = m_commonComponents[iCmp].color;
                //}

                endOutlinePath(path);

                if (m_shapeAAEnabled) {
                    makeOutlineAntiAlias(path);
                }
            }
        }
    }





    // Inner shadow
    // Note: inner shadow の offset はサポートしない。（alpha 計算のいい方法が見つかったら）
    if (insetShadowEnabled()) {
        PointIdRange shaprRange;
        makeShadowoutlinePoints(m_shapeInnerGuide, 1.0f, &shaprRange);
        applyColors(shaprRange, m_shadowStyle.shadowColor);

        PointIdRange nearRange;
        makeShadowoutlinePoints(m_innerShadowNearGuide, 1.0f, &nearRange);
        applyColors(nearRange, m_shadowStyle.shadowColor);

        for (int i = 0; i < shaprRange.countId; i++) {
            const auto& vPt = outlinePoint(shaprRange.startId + i);
            auto& nPt = outlinePoint(nearRange.startId + i);

            Vector2 dir;

            if (nPt.cornerGroup == TopLeft || nPt.cornerGroup == BottomLeft) {
                if (nPt.pos.x < vPt.pos.x) {
                    dir.x = nPt.pos.x - vPt.pos.x;
                    nPt.pos.x = vPt.pos.x;
                }
            }
            if (nPt.cornerGroup == TopRight || nPt.cornerGroup == BottomRight) {
                if (nPt.pos.x > vPt.pos.x) {
                    dir.x = nPt.pos.x - vPt.pos.x;
                    nPt.pos.x = vPt.pos.x;
                }
            }
            if (nPt.cornerGroup == TopLeft || nPt.cornerGroup == TopRight) {
                if (nPt.pos.y < vPt.pos.y) {
                    dir.y = nPt.pos.y - vPt.pos.y;
                    nPt.pos.y = vPt.pos.y;
                }
            }
            if (nPt.cornerGroup == BottomLeft || nPt.cornerGroup == BottomRight) {
                if (nPt.pos.y > vPt.pos.y) {
                    dir.y = nPt.pos.y - vPt.pos.y;
                    nPt.pos.y = vPt.pos.y;
                }
            }

            if (dir != Vector2::Zero) {
                float d = dir.length();
                float a = Math::clamp01(1.0f - (d / m_shadowStyle.shadowBlur));
                nPt.color.a *= a;
            }
        }

        makeStripePath(shaprRange, nearRange);

        if (m_shadowStyle.shadowBlur > 0.0f) {
            PointIdRange farRange;
            makeShadowoutlinePoints(m_innerShadowFarGuide, 1.0f, &farRange);
            applyColors(farRange, m_shadowStyle.shadowColor.withAlpha(0.0f));
            makeStripePath(nearRange, farRange);
        }
    }


    expandPathes();
}

// Border 用。Shadow 用とは別にする。
void BoxElementShapeBuilder3::makeBaseOuterPointsAndBorderComponent(const GuideArea& baseRect, float dirSign, ComponentSet* outSet)
{
    outSet->startId = m_outlinePointBuffer.getCount();
    Component* components = outSet->components;

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

    auto addPointCallback_TL = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::TopLeft); };
    auto addPointCallback_TR = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::TopRight); };
    auto addPointCallback_BR = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::BottomRight); };
    auto addPointCallback_BL = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::BottomLeft); };

    // top-side component
    {
        components[Top].pointIdStart = m_outlinePointBuffer.getCount();
        components[Top].cornerStart1 = m_outlinePointBuffer.getCount();
        // top-left
        if (tlRad <= 0.0f)
            addOutlinePoint(lt[0], Vector2(0, -1), 0.0f, Corner::TopLeft);
        else
            Utils_plotCornerPointsBezier(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.5f, 1.0f, lt[1], addPointCallback_TL);
        components[Top].cornerStart2 = m_outlinePointBuffer.getCount();
        // top-right
        if (trRad <= 0.0f)
            addOutlinePoint(rt[0], Vector2(0, -1), 1.0f, Corner::TopRight);
        else
            Utils_plotCornerPointsBezier(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.0f, 0.5f, rt[1], addPointCallback_TR);
        components[Top].pointIdCount = m_outlinePointBuffer.getCount() - components[Top].pointIdStart;
    }

    // right-side component
    {
        components[Right].pointIdStart = m_outlinePointBuffer.getCount();
        components[Right].cornerStart1 = m_outlinePointBuffer.getCount();
        // top-right
        if (trRad <= 0.0f)
            addOutlinePoint(rt[0], Vector2(1, 0), 0.0f, Corner::TopRight);
        else
            Utils_plotCornerPointsBezier(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.5f, 1.0f, rt[1], addPointCallback_TR);
        components[Right].cornerStart2 = m_outlinePointBuffer.getCount();
        // bottom-right
        if (brRad <= 0.0f)
            addOutlinePoint(rb[0], Vector2(1, 0), 1.0f, Corner::BottomRight);
        else
            Utils_plotCornerPointsBezier(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.0f, 0.5f, rb[1], addPointCallback_BR);
        components[Right].pointIdCount = m_outlinePointBuffer.getCount() - components[Right].pointIdStart;
    }

    // bottom-side component
    {
        components[Bottom].pointIdStart = m_outlinePointBuffer.getCount();
        components[Bottom].cornerStart1 = m_outlinePointBuffer.getCount();
        // bottom-right
        if (brRad <= 0.0f)
            addOutlinePoint(rb[0], Vector2(0, 1), 0.0f, Corner::BottomRight);
        else
            Utils_plotCornerPointsBezier(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.5f, 1.0f, rb[1], addPointCallback_BR);
        components[Bottom].cornerStart2 = m_outlinePointBuffer.getCount();
        // bottom-left
        if (blRad <= 0.0f)
            addOutlinePoint(lb[0], Vector2(0, 1), 1.0f, Corner::BottomLeft);
        else
            Utils_plotCornerPointsBezier(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.0f, 0.5f, lb[1], addPointCallback_BL);
        components[Bottom].pointIdCount = m_outlinePointBuffer.getCount() - components[Bottom].pointIdStart;
    }

    // left-side component
    {
        components[Left].pointIdStart = m_outlinePointBuffer.getCount();
        components[Left].cornerStart1 = m_outlinePointBuffer.getCount();
        // bottom-left
        if (blRad <= 0.0f)
            addOutlinePoint(lb[0], Vector2(-1, 0), 0.0f, Corner::BottomLeft);
        else
            Utils_plotCornerPointsBezier(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.5f, 1.0f, lb[1], addPointCallback_BL);
        components[Left].cornerStart2 = m_outlinePointBuffer.getCount();
        // top-left
        if (tlRad <= 0.0f)
            addOutlinePoint(lt[0], Vector2(-1, 0), 1.0f, Corner::TopLeft);
        else
            Utils_plotCornerPointsBezier(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.0f, 0.5f, lt[1], addPointCallback_TL);
        components[Left].pointIdCount = m_outlinePointBuffer.getCount() - components[Left].pointIdStart;
    }

    outSet->countId = m_outlinePointBuffer.getCount() - outSet->startId;
}

void BoxElementShapeBuilder3::makeShadowoutlinePoints(const GuideArea& baseRect, float dirSign, PointIdRange* outRange)
{
    outRange->startId = m_outlinePointBuffer.getCount();

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

    auto addPointCallback_TL = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::TopLeft); };
    auto addPointCallback_TR = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::TopRight); };
    auto addPointCallback_BR = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::BottomRight); };
    auto addPointCallback_BL = [this, dirSign](const Vector2& pos, const Vector2& infrateDir, float t) { addOutlinePoint(pos, dirSign * infrateDir, t, Corner::BottomLeft); };

    //const float MagicOfs = 0.1;

    // TopLeft
    {
        if (tlRad <= 0.0f) {
            //const Vector2 dir = Vector2::normalize(Vector2(-1, -1));
            //int ps = m_outlinePointBuffer.getCount();
            const float step = Math::PIDiv2 / m_shadowtess[TopLeft];
            for (int i = 0; i < m_shadowtess[TopLeft] + 1; i++) {   // 必ず要求されている数だけ作る. +1 は Utils_plotCornerPointsBezier2 と生成数を合わせるための調整
                float r = Math::PI + (step * i);
                float s, c;
                Math::sinCos(r, &s, &c);
                addOutlinePoint(lt[0], Vector2(c, s), 0.0f, Corner::TopLeft);
            }
            //int pe = m_outlinePointBuffer.getCount();
            //outlinePoint(ps).pos.x -= MagicOfs;
            //outlinePoint(pe).pos.y -= MagicOfs;
        }
        else {
            Utils_plotCornerPointsBezier2(Vector2(lt[0].x, lt[1].y), Vector2(0, -1), Vector2(lt[1].x, lt[0].y), Vector2(-1, 0), 0.0f, 1.0f, lt[1], m_shadowtess[TopLeft], addPointCallback_TL);
        }
    }

    // TopRight
    {
        if (trRad <= 0.0f) {
            //const Vector2 dir = Vector2::normalize(Vector2(1, -1));
            //int ps = m_outlinePointBuffer.getCount();
            const float step = Math::PIDiv2 / m_shadowtess[TopRight];
            for (int i = 0; i < m_shadowtess[TopRight] + 1; i++) {
                float r = (Math::PI + Math::PIDiv2) + (step * i);
                float s, c;
                Math::sinCos(r, &s, &c);
                addOutlinePoint(rt[0], Vector2(c, s), 0.0f, Corner::TopRight);
            }
            //int pe = m_outlinePointBuffer.getCount();
            //outlinePoint(ps).pos.y -= MagicOfs;
            //outlinePoint(pe).pos.x += MagicOfs;
        }
        else {
            Utils_plotCornerPointsBezier2(Vector2(rt[1].x, rt[0].y), Vector2(1, 0), Vector2(rt[0].x, rt[1].y), Vector2(0, -1), 0.0f, 1.0f, rt[1], m_shadowtess[TopRight], addPointCallback_TR);
        }
    }

    // BottomRight
    {
        if (brRad <= 0.0f) {
            //const Vector2 dir = Vector2::normalize(Vector2(1, 1));
            //int ps = m_outlinePointBuffer.getCount();
            const float step = Math::PIDiv2 / m_shadowtess[BottomRight];
            for (int i = 0; i < m_shadowtess[BottomRight] + 1; i++) {
                float r = (step * i);
                float s, c;
                Math::sinCos(r, &s, &c);
                addOutlinePoint(rb[0], Vector2(c, s), 0.0f, Corner::BottomRight);
            }
            //int pe = m_outlinePointBuffer.getCount();
            //outlinePoint(ps).pos.x += MagicOfs;
            //outlinePoint(pe).pos.y += MagicOfs;
        }
        else {
            Utils_plotCornerPointsBezier2(Vector2(rb[0].x, rb[1].y), Vector2(0, 1), Vector2(rb[1].x, rb[0].y), Vector2(1, 0), 0.0f, 1.0f, rb[1], m_shadowtess[BottomRight], addPointCallback_BR);
        }
    }

    // BottomLeft
    {
        if (brRad <= 0.0f) {
            //const Vector2 dir = Vector2::normalize(Vector2(-1, 1));
            //int ps = m_outlinePointBuffer.getCount();
            const float step = Math::PIDiv2 / m_shadowtess[BottomLeft];
            for (int i = 0; i < m_shadowtess[BottomLeft] + 1; i++) {
                float r = Math::PIDiv2 + (step * i);
                float s, c;
                Math::sinCos(r, &s, &c);
                addOutlinePoint(lb[0], Vector2(c, s), 0.0f, Corner::BottomLeft);
            }
            //int pe = m_outlinePointBuffer.getCount();
            //outlinePoint(ps).pos.y += MagicOfs;
            //outlinePoint(pe).pos.x -= MagicOfs;
        }
        else {
            Utils_plotCornerPointsBezier2(Vector2(lb[1].x, lb[0].y), Vector2(-1, 0), Vector2(lb[0].x, lb[1].y), Vector2(0, 1), 0.0f, 1.0f, rb[1], m_shadowtess[BottomLeft], addPointCallback_BL);
        }
    }

    outRange->countId = m_outlinePointBuffer.getCount() - outRange->startId;
}

void BoxElementShapeBuilder3::makeStripePath(const PointIdRange& outer, const PointIdRange& inner)
{
    assert(outer.countId == inner.countId);
    auto* path = beginOutlinePath(OutlinePathType::Stripe, PathWinding::CW);
    path->stripeClosing = true;
    for (int i = 0; i < outer.countId; i++) {
        addOutlineIndex(outer.startId + i);
        addOutlineIndex(inner.startId + i);
    }
    endOutlinePath(path);
}

void BoxElementShapeBuilder3::applyColors(const PointIdRange& range, const Color& color)
{
    for (int id = range.beginIds(); id < range.endIds(); id++) {
        auto& pt = outlinePoint(id);
        pt.color = color;
    }
}

void BoxElementShapeBuilder3::applyColorsAndOffset(const PointIdRange& range, const Color& color, const Vector2& offset)
{
    for (int id = range.beginIds(); id < range.endIds(); id++) {
        auto& pt = outlinePoint(id);
        pt.color = color;
        pt.pos += offset;
    }
}

//==============================================================================
// ShapesRenderFeature2

ShapesRenderFeature2::ShapesRenderFeature2()
	: m_manager(nullptr)
	//, m_batchData()
{
}

void ShapesRenderFeature2::init(RenderingManager* manager)
{
	m_manager = manager;
	//m_batchData.indexOffset = 0;
	//m_batchData.indexCount = 0;

	m_vertexLayout = m_manager->standardVertexDeclaration();
	m_vertexBuffer = makeObject<VertexBuffer>(4096 * sizeof(Vertex), GraphicsResourceUsage::Dynamic);
	m_indexBuffer = makeObject<IndexBuffer>(4096, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
}

RequestBatchResult ShapesRenderFeature2::requestDrawCommandList(
    RenderFeatureBatchList* batchList,
    const RLIBatchState& batchState,
    ShapesRendererCommandList* commandList)
{
    Batch* batch = acquireBatch(batchList, batchState);

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


			auto vb = static_cast<Vertex*>(m_vertexBuffer->writableData());
			auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));
			m_shapeBuilder.writeToBuffer(vb + m_vertexUsedCount, ib + m_indexUsedCount, m_indexUsedCount);


			m_vertexUsedCount += m_shapeBuilder.vertexCount();
			m_indexUsedCount += m_shapeBuilder.indexCount();

            batch->data.indexCount += m_shapeBuilder.indexCount();

		}
		node = node->next;
	}




	return RequestBatchResult::Staging;
}

RequestBatchResult ShapesRenderFeature2::requestDrawCommandList(
    RenderFeatureBatchList* batchList,
    const RLIBatchState& batchState,
    BoxElementShapeCommandList* commandList)
{
    Batch* batch = acquireBatch(batchList, batchState);

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


                auto vb = static_cast<Vertex*>(m_vertexBuffer->writableData());
                auto ib = static_cast<uint16_t*>(m_indexBuffer->map(MapMode::Write));
                m_shapeBuilder.writeToBuffer(vb + m_vertexUsedCount, ib + m_indexUsedCount, m_vertexUsedCount);


                m_vertexUsedCount += m_shapeBuilder.vertexCount();
                m_indexUsedCount += m_shapeBuilder.indexCount();

                batch->data.indexCount += m_shapeBuilder.indexCount();
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
	//m_batchData.indexOffset = 0;
	//m_batchData.indexCount = 0;
	m_vertexUsedCount = 0;
	m_indexUsedCount = 0;
}

void ShapesRenderFeature2::submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList)
{
#ifdef LN_RLI_BATCH
    LN_UNREACHABLE();
#else
	//if (m_mappedVertices) {
	//	// TODO: unmap (今は自動だけど、明示した方が安心かも)
	//}

	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.indexOffset = m_batchData.indexOffset + m_batchData.indexCount;
	m_batchData.indexCount = 0;
#endif
}

void ShapesRenderFeature2::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);



	context->setVertexLayout(m_vertexLayout);
	context->setVertexBuffer(0, m_vertexBuffer);
	context->setIndexBuffer(m_indexBuffer);
	context->drawPrimitiveIndexed(localBatch->data.indexOffset, localBatch->data.indexCount / 3);
}

ShapesRenderFeature2::Batch* ShapesRenderFeature2::acquireBatch(RenderFeatureBatchList* batchList, const RLIBatchState& batchState)
{
    // TODO: Batching
    Batch* batch = batchList->addNewBatch<Batch>(this, batchState);
    batch->data.indexOffset = m_indexUsedCount;
    batch->data.indexCount = 0;
    return batch;
}

} // namespace detail
} // namespace ln

