
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
				m_vertexBuffer->resize(vertexBufferRequestSize);
			}

			// IndexBuffer
			int indexBufferRequestCount = (m_indexUsedCount + m_indexCache.getCount());
			if (!m_indexBuffer) {
				m_indexBuffer = makeObject<IndexBuffer>(indexBufferRequestCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Dynamic);
			}
			else if (m_indexBuffer->size() < indexBufferRequestCount) {
				m_indexBuffer->resize(indexBufferRequestCount);
			}
        }


		// 面方向を反転
		for (int i = 0; i < m_indexCache.getCount(); i += 3)
		{
			std::swap(m_indexCache.getAt(i + 1), m_indexCache.getAt(i + 2));
		}


		{
			//TODO: context に setData つくる



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

} // namespace detail
} // namespace ln

