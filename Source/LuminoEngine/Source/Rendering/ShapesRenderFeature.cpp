
#include "../Internal.h"
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Rendering/Rendering.h>
#include "../Graphics/Device/GraphicsDriverInterface.h"
#include "../Graphics/GraphicsManager.h"
#include "../Graphics/RenderingCommand.h"
#include "ShapesRenderFeature.h"

LN_NAMESPACE_BEGIN
namespace detail {

static const int g_finalOffset = 0.0;

//==============================================================================
// ShapesRendererCommandList
//==============================================================================

//------------------------------------------------------------------------------
void ShapesRendererCommandList::AddDrawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius)
{
	float cmd[] =
	{
		(float)Cmd_DrawBoxBackground,
		// [1]
		rect.x, rect.y, rect.width, rect.height,
		// [5]
		cornerRadius.topLeft, cornerRadius.topRight, cornerRadius.bottomLeft, cornerRadius.bottomRight,
	};
	allocData(sizeof(cmd), cmd);
}

//------------------------------------------------------------------------------
void ShapesRendererCommandList::AddDrawBoxBorder(
	float x, float y, float w, float h, float l, float t, float r, float b,
	const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor,
	float ltRad, float rtRad, float lbRad, float rbRad,
	const Color& shadowColor, float shadowBlur, float shadowWidth, bool shadowInset, bool borderInset)
{
	float cmd[] =
	{
		(float)Cmd_DrawBoxBorder, x, y, w, h, l, t, r, b,
		// [9]
		leftColor.r, leftColor.g, leftColor.b, leftColor.a,
		topColor.r, topColor.g, topColor.b, topColor.a,
		rightColor.r, rightColor.g, rightColor.b, rightColor.a,
		bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a,
		// [25]
		ltRad, rtRad, lbRad, rbRad,
		// [29]
		shadowColor.r, shadowColor.g, shadowColor.b, shadowColor.a,
		shadowBlur, shadowWidth, (shadowInset) ? 1.0f : 0.0f, (borderInset) ? 1.0f : 0.0f,
	};
	allocData(sizeof(cmd), cmd);
}

//------------------------------------------------------------------------------s
void ShapesRendererCommandList::AddDrawBoxBorder2(const Rect& rect, const ThicknessF& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset)
{
	float cmd[] =
	{
		(float)Cmd_DrawBoxBorder2,
		// [1]
		rect.x, rect.y, rect.width, rect.height,
		// [5]
		thickness.Left, thickness.Top, thickness.Right, thickness.Bottom,
		// [9]
		leftColor.r, leftColor.g, leftColor.b, leftColor.a,
		topColor.r, topColor.g, topColor.b, topColor.a,
		rightColor.r, rightColor.g, rightColor.b, rightColor.a,
		bottomColor.r, bottomColor.g, bottomColor.b, bottomColor.a,
		// [25]
		cornerRadius.topLeft, cornerRadius.topRight, cornerRadius.bottomLeft, cornerRadius.bottomRight,
		(borderInset) ? 1.0f : 0.0f,
	};
	allocData(sizeof(cmd), cmd);
}

//------------------------------------------------------------------------------
void ShapesRendererCommandList::AddDrawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Color& color, float blur, float width, bool inset)
{
	float cmd[] =
	{
		(float)Cmd_DrawBoxShadow,
		// [1]
		rect.x, rect.y, rect.width, rect.height,
		// [5]
		cornerRadius.topLeft, cornerRadius.topRight, cornerRadius.bottomLeft, cornerRadius.bottomRight,
		// [9]
		color.r, color.g, color.b, color.a,
		// [13]
		blur, width, (inset) ? 1.0f : 0.0f
	};
	allocData(sizeof(cmd), cmd);
}

//==============================================================================
// ShapesRendererCommandListCache
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<ShapesRendererCommandList> ShapesRendererCommandListCache::createObject()
{
	return RefPtr<ShapesRendererCommandList>::makeRef();
}

//==============================================================================
// ShapesRendererCore
//==============================================================================
//------------------------------------------------------------------------------
ShapesRendererCore::ShapesRendererCore()
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
ShapesRendererCore::~ShapesRendererCore()
{
	LN_SAFE_RELEASE(m_vertexBuffer);
	LN_SAFE_RELEASE(m_indexBuffer);
}

//------------------------------------------------------------------------------
void ShapesRendererCore::initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_basePoints.reserve(4096);
	m_outlinePoints.reserve(4096);
	m_vertexCache.reserve(4096);
	m_indexCache.reserve(4096);
}

//------------------------------------------------------------------------------
void ShapesRendererCore::RequestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex)
{
	////assert(vb != nullptr);
	////assert(ib != nullptr);
	////*outBeginVertexIndex = m_vertexCache.GetCount();
	////*vb = m_vertexCache.request(vertexCount);
	////*ib = m_indexCache.request(indexCount);
}

//------------------------------------------------------------------------------
void ShapesRendererCore::RenderCommandList(ShapesRendererCommandList* commandList, detail::BrushRawData* fillBrush)
{
	ExtractBasePoints(commandList);
	CalcExtrudedDirection();


	for (int iPath = 0; iPath < m_pathes.getCount(); iPath++)
	{
		Path& path = m_pathes[iPath];

		if (path.attribute == PathAttribute::Background)
		{
			path.color *= fillBrush->color;
		}

		switch (path.type)
		{
		case PathType::Convex:
			ExpandFill(path);
			break;
		case PathType::Strip2Point:
			ExpandStrip2PointStroke(path);
			break;
		case PathType::Strip3Point:
			ExpandStrip3PointStroke(path);
			break;
		}
	}

	//for (int i = 0; i < dataCount; i++)
	//{
	//	Vertex* vb;
	//	uint16_t* ib;
	//	uint16_t beginVertexIndex;
	//	RequestBuffers(
	//		cache->GetVertexCount(dataList[i].cacheGlyphInfoHandle),
	//		cache->getIndexCount(dataList[i].cacheGlyphInfoHandle),
	//		&vb, &ib, &beginVertexIndex);
	//	cache->GenerateMesh(
	//		dataList[i].cacheGlyphInfoHandle, Vector3(dataList[i].origin.x, dataList[i].origin.y, 0), dataList[i].transform,
	//		vb, ib, beginVertexIndex);
	//}

	// TODO: このへん PrimitiveRenderFeature と同じ。共通にできないか？
	{
		Driver::IRenderer* renderer = m_manager->getGraphicsDevice()->getRenderer();

		// サイズが足りなければ再作成
		auto* device = m_manager->getGraphicsDevice();
		if (m_vertexBuffer == nullptr || m_vertexBuffer->getByteCount() < m_vertexCache.getBufferUsedByteCount())
		{
			LN_SAFE_RELEASE(m_vertexBuffer);
			m_vertexBuffer = device->CreateVertexBuffer(m_vertexCache.getBufferUsedByteCount(), nullptr, ResourceUsage::Dynamic);
		}
		if (m_indexBuffer == nullptr || m_indexBuffer->getByteCount() < m_indexCache.getBufferUsedByteCount())
		{
			LN_SAFE_RELEASE(m_indexBuffer);
			m_indexBuffer = device->CreateIndexBuffer(m_indexCache.getBufferUsedByteCount(), nullptr, IndexBufferFormat_UInt16, ResourceUsage::Dynamic);
		}

		// 描画する
		m_vertexBuffer->setSubData(0, m_vertexCache.getBuffer(), m_vertexCache.getBufferUsedByteCount());
		m_indexBuffer->setSubData(0, m_indexCache.getBuffer(), m_indexCache.getBufferUsedByteCount());

		{
			renderer->SetVertexDeclaration(m_manager->getDefaultVertexDeclaration()->getDeviceObject());
			renderer->SetVertexBuffer(0, m_vertexBuffer);
			renderer->SetIndexBuffer(m_indexBuffer);
			renderer->drawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.getCount() / 3);
		}
	}

	// キャッシュクリア
	m_vertexCache.clear();
	m_indexCache.clear();
	m_basePoints.clear();
	m_outlinePoints.clear();
	m_pathes.clear();
}

//------------------------------------------------------------------------------
void ShapesRendererCore::releaseCommandList(ShapesRendererCommandList* commandList)
{
	commandList->clear();
	m_manager->getShapesRendererCommandListCache()->releaseCommandList(commandList);
}

//------------------------------------------------------------------------------
ShapesRendererCore::Path* ShapesRendererCore::AddPath(PathType type, const Color& color, PathWinding winding, PathAttribute attribute)
{
	m_pathes.add(Path{ type, m_outlinePoints.getCount(), 0, color, winding, attribute });
	return &m_pathes.getLast();
}

//------------------------------------------------------------------------------
void ShapesRendererCore::EndPath(Path* path)
{
	path->pointCount = m_outlinePoints.getCount() - path->pointStart;
}

//------------------------------------------------------------------------------
void ShapesRendererCore::ExtractBasePoints(ShapesRendererCommandList* commandList)
{
	int count = commandList->getDataCount();
	for (int i = 0; i < count; i++)
	{
		float* cmd = (float*)commandList->getDataByIndex(i);
		switch ((int)cmd[0])
		{
			//------------------------------------------------------------------
			case ShapesRendererCommandList::Cmd_DrawBoxBackground:
			{
				BorderComponent components[4];
				MakeBasePointsAndBorderComponent(
					Rect(cmd[1], cmd[2], cmd[3], cmd[4]),
					ThicknessF(0, 0, 0, 0),
					CornerRadius(cmd[5], cmd[6], cmd[7], cmd[8]),
					components);

				// center box
				{
					auto* path = AddPath(PathType::Convex, Color::White, PathWinding::CCW, PathAttribute::Background);
					for (int iComp = 0; iComp < 4; iComp++)
					{
						for (int i = components[iComp].firstPoint; i < components[iComp].lastPoint; i++)	// 終点は次の Componet の開始点と一致するので必要ない
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ pt.pos, GetAAExtDir(pt), 1.0f });
						}
					}
					EndPath(path);
				}
				break;
			}
			//------------------------------------------------------------------
			case ShapesRendererCommandList::Cmd_DrawBoxBorder2:
			{
				BorderComponent components[4];
				MakeBasePointsAndBorderComponent(
					Rect(cmd[1], cmd[2], cmd[3], cmd[4]),
					ThicknessF(cmd[5], cmd[6], cmd[7], cmd[8]),
					CornerRadius(cmd[25], cmd[26], cmd[27], cmd[28]),
					components);

				bool borderInset = (cmd[29] != 0.0f);
				struct BorderInfo
				{
					int		componentIndex;
					Color	color;
					float	width;
				};
				BorderInfo intos[4] =
				{
					{ 0, Color(cmd[9], cmd[10], cmd[11], cmd[12]), cmd[5] },	// left border
					{ 3, Color(cmd[21], cmd[22], cmd[23], cmd[24]), cmd[8] },	// bottom border
					{ 2, Color(cmd[17], cmd[18], cmd[19], cmd[20]), cmd[7] },	// right border
					{ 1, Color(cmd[13], cmd[14], cmd[15], cmd[16]), cmd[6] },	// top border
				};

				for (int iInfo = 0; iInfo < 4; iInfo++)
				{
					const BorderComponent& component = components[intos[iInfo].componentIndex];
					auto* path = AddPath(PathType::Convex, intos[iInfo].color, PathWinding::CCW);

					if (borderInset)
					{
						// right-dir
						for (int i = component.firstPoint; i <= component.lastPoint; i++)
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ pt.pos, GetAAExtDir(pt), 1.0f });
						}
						// left-dir
						for (int i = component.lastPoint; i >= component.firstPoint; i--)
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ GetExtPos(pt, -1.0f, intos[iInfo].width), -GetAAExtDir(pt), 1.0f });
						}
					}
					else
					{
						// right-dir
						for (int i = component.firstPoint; i <= component.lastPoint; i++)
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ GetExtPos(pt, 1.0f, intos[iInfo].width), GetAAExtDir(pt), 1.0f });
						}
						// left-dir
						for (int i = component.lastPoint; i >= component.firstPoint; i--)
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ pt.pos, -GetAAExtDir(pt), 1.0f });
						}
					}
					EndPath(path);
				}
				break;
			}

			case ShapesRendererCommandList::Cmd_DrawBoxShadow:
			{
				float ltRad = cmd[5];
				float rtRad = cmd[6];
				float lbRad = cmd[7];
				float rbRad = cmd[8];

				Color shadowColor(cmd[9], cmd[10], cmd[11], cmd[12]);
				float shadowBlur = cmd[13];
				float shadowWidth = cmd[14];
				bool shadowInset = (cmd[15] != 0.0f);

				float shadowFill = shadowBlur * 2/*(shadowWidth - shadowBlur)*/;
				float shadowBlurWidth = (shadowWidth - shadowBlur) + shadowBlur * 2;	// base からシャドウのもっとも外側まで

				BorderComponent components[4];
					//// [1]
					//rect.x, rect.y, rect.width, rect.height,
					//// [5]
					//cornerRadius.topLeft, cornerRadius.topRight, cornerRadius.bottomLeft, cornerRadius.bottomRight,
					//// [9]
					//color.r, color.g, color.b, color.a,
					//// [13]
					//blur, width, (inset) ? 1.0f : 0.0f
				Vector2 lt[4];
				Vector2 rt[4];
				Vector2 lb[4];
				Vector2 rb[4];
				// basis
				lt[1] = Vector2(cmd[1], cmd[2]);
				rt[1] = Vector2(cmd[1] + cmd[3], cmd[2]);
				lb[1] = Vector2(cmd[1], cmd[2] + cmd[4]);
				rb[1] = Vector2(cmd[1] + cmd[3], cmd[2] + cmd[4]);
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
				PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[3].y), Vector2(-1, 0), Vector2(lt[3].x, lt[2].y), Vector2(0, -1), 0.5, 1.0, lt[2]);
				// left-bottom
				PlotCornerBasePointsBezier(Vector2(lb[3].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[3].y), Vector2(-1, 0), 0.0, 0.5, lb[2]);
				components[0].lastPoint = m_basePoints.getCount() - 1;

				// bottom-side component
				components[1].firstPoint = m_basePoints.getCount();
				// left-bottom
				PlotCornerBasePointsBezier(Vector2(lb[3].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[3].y), Vector2(-1, 0), 0.5, 1.0, lb[2]);
				// right-bottom
				PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[3].y), Vector2(1, 0), Vector2(rb[3].x, rb[2].y), Vector2(0, 1), 0.0, 0.5, rb[2]);
				components[1].lastPoint = m_basePoints.getCount() - 1;

				// right-side component
				components[2].firstPoint = m_basePoints.getCount();
				// right-bottom
				PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[3].y), Vector2(1, 0), Vector2(rb[3].x, rb[2].y), Vector2(0, 1), 0.5, 1.0, rb[2]);
				// right-top
				PlotCornerBasePointsBezier(Vector2(rt[3].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[3].y), Vector2(1, 0), 0.0, 0.5, rt[2]);
				components[2].lastPoint = m_basePoints.getCount() - 1;

				// top-side component
				components[3].firstPoint = m_basePoints.getCount();
				// right-top
				PlotCornerBasePointsBezier(Vector2(rt[3].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[3].y), Vector2(1, 0), 0.5, 1.0, rt[2]);
				// left-top
				PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[3].y), Vector2(-1, 0), Vector2(lt[3].x, lt[2].y), Vector2(0, -1), 0.0, 0.5, lt[2]);
				components[3].lastPoint = m_basePoints.getCount() - 1;



				if (shadowInset)
				{
					for (int iComp = 0; iComp < 4; iComp++)
					{
						auto* path = AddPath(PathType::Strip3Point, shadowColor);
						for (int i = components[iComp].firstPoint; i <= components[iComp].lastPoint; i++)
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth - pt.exDir * shadowBlurWidth, GetAAExtDir(pt), 0.0f });
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth - pt.exDir * (shadowBlurWidth - shadowFill), GetAAExtDir(pt), 1.0f });
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth, GetAAExtDir(pt), 1.0f });

						}
						EndPath(path);
					}
				}
				else
				{
					for (int iComp = 0; iComp < 4; iComp++)
					{
						auto* path = AddPath(PathType::Strip3Point, shadowColor);
						for (int i = components[iComp].firstPoint; i <= components[iComp].lastPoint; i++)
						{
							BasePoint& pt = m_basePoints.getAt(i);

							// left-dir
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth, GetAAExtDir(pt), 1.0f });
							// right-dir
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowFill, GetAAExtDir(pt), 1.0f });
							// right-dir
							m_outlinePoints.add({ pt.pos, GetAAExtDir(pt), 0.0f });

						}
						EndPath(path);
					}
				}

				break;
			}

			case ShapesRendererCommandList::Cmd_DrawBoxBorder:
			{
				const float root2 = 1.414213562373095f;

				// Component の始点と終点は、前後の Component のそれと重なっている
				struct BaseComponent
				{
					int	firstPoint;
					int lastPoint;
				};
				BaseComponent baseComponents[4];
				BaseComponent shadowComponents[4];


				float ltRad = cmd[25];
				float rtRad = cmd[26];
				float lbRad = cmd[27];
				float rbRad = cmd[28];

				Color shadowColor(cmd[29], cmd[30], cmd[31], cmd[32]);
				float shadowBlur = cmd[33];
				float shadowWidth = cmd[34];
				bool shadowInset = (cmd[35] != 0.0f);
				bool borderInset = (cmd[36] != 0.0f);

				float borderExtSign = 1.0f;
				PathWinding borderWinding = PathWinding::CCW;
				if (borderInset)
				{
					borderExtSign = -1.0f;
					borderWinding = PathWinding::CW;
				}

				float shadowFill = shadowBlur * 2/*(shadowWidth - shadowBlur)*/;
				float shadowBlurWidth = (shadowWidth - shadowBlur) + shadowBlur * 2;	// base からシャドウのもっとも外側まで

				Vector2 lt[4];
				Vector2 rt[4];
				Vector2 lb[4];
				Vector2 rb[4];
				// basis
				lt[1] = Vector2(cmd[1], cmd[2]);
				rt[1] = Vector2(cmd[1] + cmd[3], cmd[2]);
				lb[1] = Vector2(cmd[1], cmd[2] + cmd[4]);
				rb[1] = Vector2(cmd[1] + cmd[3], cmd[2] + cmd[4]);
				// outer
				lt[0] = Vector2(lt[1].x - cmd[5], lt[1].y - cmd[6]);
				rt[0] = Vector2(rt[1].x + cmd[7], rt[1].y - cmd[6]);
				lb[0] = Vector2(lb[1].x - cmd[5], lb[1].y + cmd[8]);
				rb[0] = Vector2(rb[1].x + cmd[7], rb[1].y + cmd[8]);
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
				baseComponents[0].firstPoint = m_basePoints.getCount();
				// left-top
				if (ltRad == 0.0f)
					m_basePoints.add({ lt[1], lt[0] - lt[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[1].y), Vector2(-1, 0), Vector2(lt[1].x, lt[2].y), Vector2(0, -1), 0.5, 1.0, lt[2]);
				// left-bottom
				if (lbRad == 0.0f)
					m_basePoints.add({ lb[1], lb[0] - lb[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(lb[1].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[1].y), Vector2(-1, 0), 0.0, 0.5, lb[2]);
				baseComponents[0].lastPoint = m_basePoints.getCount() - 1;

				// bottom-side component
				baseComponents[1].firstPoint = m_basePoints.getCount();
				// left-bottom
				if (lbRad == 0.0f)
					m_basePoints.add({ lb[1], lb[0] - lb[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(lb[1].x, lb[2].y), Vector2(0,  1), Vector2(lb[2].x, lb[1].y), Vector2(-1,  0), 0.5, 1.0, lb[2]);
				// right-bottom
				if (rbRad == 0.0f)
					m_basePoints.add({ rb[1], rb[0] - rb[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[1].y), Vector2(1,  0), Vector2(rb[1].x, rb[2].y), Vector2(0, 1), 0.0, 0.5, rb[2]);
				baseComponents[1].lastPoint = m_basePoints.getCount() - 1;

				// right-side component
				baseComponents[2].firstPoint = m_basePoints.getCount();
				// right-bottom
				if (rbRad == 0.0f)
					m_basePoints.add({ rb[1], rb[0] - rb[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[1].y), Vector2(1,  0), Vector2(rb[1].x, rb[2].y), Vector2(0, 1), 0.5, 1.0, rb[2]);
				// right-top
				if (rtRad == 0.0f)
					m_basePoints.add({ rt[1], rt[0] - rt[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(rt[1].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[1].y), Vector2(1, 0), 0.0, 0.5, rt[2]);
				baseComponents[2].lastPoint = m_basePoints.getCount() - 1;

				// top-side component
				baseComponents[3].firstPoint = m_basePoints.getCount();
				// right-top
				if (rtRad == 0.0f)
					m_basePoints.add({ rt[1], rt[0] - rt[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(rt[1].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[1].y), Vector2(1, 0), 0.5, 1.0, rt[2]);
				// left-top
				if (ltRad == 0.0f)
					m_basePoints.add({ lt[1], lt[0] - lt[1], false, true });
				else
					PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[1].y), Vector2(-1, 0), Vector2(lt[1].x, lt[2].y), Vector2(0, -1), 0.0, 0.5, lt[2]);
				baseComponents[3].lastPoint = m_basePoints.getCount() - 1;


				// left-side component
				shadowComponents[0].firstPoint = m_basePoints.getCount();
				// left-top
				PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[3].y), Vector2(-1, 0), Vector2(lt[3].x, lt[2].y), Vector2(0, -1), 0.5, 1.0, lt[2]);
				// left-bottom
				PlotCornerBasePointsBezier(Vector2(lb[3].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[3].y), Vector2(-1, 0), 0.0, 0.5, lb[2]);
				shadowComponents[0].lastPoint = m_basePoints.getCount() - 1;

				// bottom-side component
				shadowComponents[1].firstPoint = m_basePoints.getCount();
				// left-bottom
				PlotCornerBasePointsBezier(Vector2(lb[3].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[3].y), Vector2(-1, 0), 0.5, 1.0, lb[2]);
				// right-bottom
				PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[3].y), Vector2(1, 0), Vector2(rb[3].x, rb[2].y), Vector2(0, 1), 0.0, 0.5, rb[2]);
				shadowComponents[1].lastPoint = m_basePoints.getCount() - 1;

				// right-side component
				shadowComponents[2].firstPoint = m_basePoints.getCount();
				// right-bottom
				PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[3].y), Vector2(1, 0), Vector2(rb[3].x, rb[2].y), Vector2(0, 1), 0.5, 1.0, rb[2]);
				// right-top
				PlotCornerBasePointsBezier(Vector2(rt[3].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[3].y), Vector2(1, 0), 0.0, 0.5, rt[2]);
				shadowComponents[2].lastPoint = m_basePoints.getCount() - 1;

				// top-side component
				shadowComponents[3].firstPoint = m_basePoints.getCount();
				// right-top
				PlotCornerBasePointsBezier(Vector2(rt[3].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[3].y), Vector2(1, 0), 0.5, 1.0, rt[2]);
				// left-top
				PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[3].y), Vector2(-1, 0), Vector2(lt[3].x, lt[2].y), Vector2(0, -1), 0.0, 0.5, lt[2]);
				shadowComponents[3].lastPoint = m_basePoints.getCount() - 1;

				// shadows
				if (!shadowInset)
				{
					for (int iComp = 0; iComp < 4; iComp++)
					{
						auto* path = AddPath(PathType::Strip3Point, shadowColor);
						for (int i = shadowComponents[iComp].firstPoint; i <= shadowComponents[iComp].lastPoint; i++)
						{
							BasePoint& pt = m_basePoints.getAt(i);
							
							// left-dir
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth, GetAAExtDir(pt), 1.0f });
							// right-dir
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowFill, GetAAExtDir(pt), 1.0f });
							// right-dir
							m_outlinePoints.add({ pt.pos, GetAAExtDir(pt), 0.0f });
							
						}
						EndPath(path);
					}
				}

				// center box
				{
					auto* path = AddPath(PathType::Convex, Color::White);
					for (int iComp = 0; iComp < 4; iComp++)
					{
						for (int i = baseComponents[iComp].firstPoint; i < baseComponents[iComp].lastPoint; i++)	// 終点は次の Componet の開始点と一致するので必要ない
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ pt.pos, GetAAExtDir(pt), 1.0f });
						}
					}
					EndPath(path);
				}

				// ※右下のコーナーが小さいとか、ちょっとゆがんで見えるのは DX9 シェーダで 0.5px オフセットが考慮されていないことが原因。





				if (shadowInset)
				{
					// shadows
					for (int iComp = 0; iComp < 4; iComp++)
					{
						auto* path = AddPath(PathType::Strip3Point, shadowColor);
						for (int i = shadowComponents[iComp].firstPoint; i <= shadowComponents[iComp].lastPoint; i++)
						{
							BasePoint& pt = m_basePoints.getAt(i);
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth - pt.exDir * shadowBlurWidth, GetAAExtDir(pt), 0.0f });
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth - pt.exDir * (shadowBlurWidth - shadowFill), GetAAExtDir(pt), 1.0f });
							m_outlinePoints.add({ pt.pos - pt.exDir * shadowBlurWidth, GetAAExtDir(pt), 1.0f });

						}
						EndPath(path);
					}
				}



				// left border
				{
					auto* path = AddPath(PathType::Convex, Color(cmd[9], cmd[10], cmd[11], cmd[12]), borderWinding);

					for (int i = baseComponents[0].firstPoint; i <= baseComponents[0].lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// right-dir
						m_outlinePoints.add({ GetExtPos(pt, borderExtSign, cmd[5]), borderExtSign * GetAAExtDir(pt), 1.0f });
					}
					for (int i = baseComponents[0].lastPoint; i >= baseComponents[0].firstPoint; i--)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// left-dir
						m_outlinePoints.add({ pt.pos, borderExtSign * -GetAAExtDir(pt), 1.0f });
					}
					EndPath(path);
				}
				// bottom border
				{
					auto* path = AddPath(PathType::Convex, Color(cmd[21], cmd[22], cmd[23], cmd[24]), borderWinding);
					for (int i = baseComponents[1].firstPoint; i <= baseComponents[1].lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// right-dir
						m_outlinePoints.add({ GetExtPos(pt, borderExtSign, cmd[8]), borderExtSign * GetAAExtDir(pt), 1.0f });
					}
					for (int i = baseComponents[1].lastPoint; i >= baseComponents[1].firstPoint; i--)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// left-dir
						m_outlinePoints.add({ pt.pos, borderExtSign * -GetAAExtDir(pt), 1.0f });
					}
					EndPath(path);
				}
				// right border
				{
					auto* path = AddPath(PathType::Convex, Color(cmd[17], cmd[18], cmd[19], cmd[20]), borderWinding);
					for (int i = baseComponents[2].firstPoint; i <= baseComponents[2].lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// right-dir
						m_outlinePoints.add({ GetExtPos(pt, borderExtSign, cmd[7]), borderExtSign * GetAAExtDir(pt), 1.0f });
					}
					for (int i = baseComponents[2].lastPoint; i >= baseComponents[2].firstPoint; i--)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// left-dir
						m_outlinePoints.add({ pt.pos, borderExtSign * -GetAAExtDir(pt), 1.0f });
					}
					EndPath(path);
				}
				// top border
				{
					auto* path = AddPath(PathType::Convex, Color(cmd[13], cmd[14], cmd[15], cmd[16]), borderWinding);
					for (int i = baseComponents[3].firstPoint; i <= baseComponents[3].lastPoint; i++)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// right-dir
						m_outlinePoints.add({ GetExtPos(pt, borderExtSign, cmd[6]), borderExtSign * GetAAExtDir(pt), 1.0f });
					}
					for (int i = baseComponents[3].lastPoint; i >= baseComponents[3].firstPoint; i--)
					{
						BasePoint& pt = m_basePoints.getAt(i);
						// left-dir
						m_outlinePoints.add({ pt.pos, borderExtSign * -GetAAExtDir(pt), 1.0f });
					}
					EndPath(path);
				}
				break;
			}
			default:
				LN_UNREACHABLE();
				break;
		}
	}
}

//------------------------------------------------------------------------------
void ShapesRendererCore::MakeBasePointsAndBorderComponent(const Rect& rect, const ThicknessF& thickness, const CornerRadius& cornerRadius, BorderComponent components[4])
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
	lt[0] = Vector2(lt[1].x - thickness.Left, lt[1].y - thickness.Top);
	rt[0] = Vector2(rt[1].x + thickness.Right, rt[1].y - thickness.Top);
	lb[0] = Vector2(lb[1].x - thickness.Left, lb[1].y + thickness.Bottom);
	rb[0] = Vector2(rb[1].x + thickness.Right, rb[1].y + thickness.Bottom);
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
		PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[1].y), Vector2(-1, 0), Vector2(lt[1].x, lt[2].y), Vector2(0, -1), 0.5, 1.0, lt[2]);
	// left-bottom
	if (blRad == 0.0f)
		m_basePoints.add({ lb[1], lb[0] - lb[1], false, true });
	else
		PlotCornerBasePointsBezier(Vector2(lb[1].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[1].y), Vector2(-1, 0), 0.0, 0.5, lb[2]);
	components[0].lastPoint = m_basePoints.getCount() - 1;

	// bottom-side component
	components[1].firstPoint = m_basePoints.getCount();
	// left-bottom
	if (blRad == 0.0f)
		m_basePoints.add({ lb[1], lb[0] - lb[1], false, true });
	else
		PlotCornerBasePointsBezier(Vector2(lb[1].x, lb[2].y), Vector2(0, 1), Vector2(lb[2].x, lb[1].y), Vector2(-1, 0), 0.5, 1.0, lb[2]);
	// right-bottom
	if (brRad == 0.0f)
		m_basePoints.add({ rb[1], rb[0] - rb[1], false, true });
	else
		PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[1].y), Vector2(1, 0), Vector2(rb[1].x, rb[2].y), Vector2(0, 1), 0.0, 0.5, rb[2]);
	components[1].lastPoint = m_basePoints.getCount() - 1;

	// right-side component
	components[2].firstPoint = m_basePoints.getCount();
	// right-bottom
	if (brRad == 0.0f)
		m_basePoints.add({ rb[1], rb[0] - rb[1], false, true });
	else
		PlotCornerBasePointsBezier(Vector2(rb[2].x, rb[1].y), Vector2(1, 0), Vector2(rb[1].x, rb[2].y), Vector2(0, 1), 0.5, 1.0, rb[2]);
	// right-top
	if (trRad == 0.0f)
		m_basePoints.add({ rt[1], rt[0] - rt[1], false, true });
	else
		PlotCornerBasePointsBezier(Vector2(rt[1].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[1].y), Vector2(1, 0), 0.0, 0.5, rt[2]);
	components[2].lastPoint = m_basePoints.getCount() - 1;

	// top-side component
	components[3].firstPoint = m_basePoints.getCount();
	// right-top
	if (trRad == 0.0f)
		m_basePoints.add({ rt[1], rt[0] - rt[1], false, true });
	else
		PlotCornerBasePointsBezier(Vector2(rt[1].x, rt[2].y), Vector2(0, -1), Vector2(rt[2].x, rt[1].y), Vector2(1, 0), 0.5, 1.0, rt[2]);
	// left-top
	if (tlRad == 0.0f)
		m_basePoints.add({ lt[1], lt[0] - lt[1], false, true });
	else
		PlotCornerBasePointsBezier(Vector2(lt[2].x, lt[1].y), Vector2(-1, 0), Vector2(lt[1].x, lt[2].y), Vector2(0, -1), 0.0, 0.5, lt[2]);
	components[3].lastPoint = m_basePoints.getCount() - 1;
}

//------------------------------------------------------------------------------
void ShapesRendererCore::CalcExtrudedDirection()
{

}

//------------------------------------------------------------------------------
void ShapesRendererCore::ExpandVertices(const Path& path)
{
	for (int i = 0; i < path.pointCount; i++)
	{
		const OutlinePoint& pt = m_outlinePoints.getAt(path.pointStart + i);
		Vertex v;
		v.position = Vector3(pt.pos + g_finalOffset, 0);
		v.color = path.color;
		v.color.a *= pt.alpha;
		m_vertexCache.add(v);
	}
}

//------------------------------------------------------------------------------
void ShapesRendererCore::ExpandFill(const Path& path)
{
	int startIndex = m_vertexCache.getCount();

	ExpandVertices(path);

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

	ExpandAntiAliasStroke(path, startIndex);
}

//------------------------------------------------------------------------------
void ShapesRendererCore::ExpandStrip2PointStroke(const Path& path)
{
	/*
		0-2-4
		|/|/|	-> front
		1-3-5
	*/

	int startIndex = m_vertexCache.getCount();

	ExpandVertices(path);

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
void ShapesRendererCore::ExpandStrip3PointStroke(const Path& path)
{
	/*
		0-3-6
		|/|/|
		1-4-7	-> front
		|/|/|
		2-5-8
	*/
	int startIndex = m_vertexCache.getCount();

	ExpandVertices(path);

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
void ShapesRendererCore::ExpandAntiAliasStroke(const Path& path, int startIndex)
{
	const float ext = 0.25f;
	const float extAA = 0.5f;

	// 凸面周囲を右回りする。右に押し出す。
	int startAA = m_vertexCache.getCount();



	for (int i = 0; i < path.pointCount / 2; i++)
	{
		auto& p1 = m_vertexCache.getAt(startIndex + i);
		auto& p2 = m_vertexCache.getAt(startIndex + (path.pointCount / 2) + i);
		float len = (p1.position - p2.position).getLength() - (ext * 2);
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

		m_vertexCache.getAt(startIndex + i).position = Vector3(pt.pos + g_finalOffset, 0);

		Vertex v;
		v.position = Vector3(pt.pos + extDir * extAA + g_finalOffset, 0);
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

//------------------------------------------------------------------------------
void ShapesRendererCore::PlotCornerBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center)
{
	LN_ASSERT(firstT < lastT);
	const int tess = 8;
	const float rtir = 0.55228f;	// https://cat-in-136.github.io/2014/03/bezier-1-kappa.html
	Vector2 d(std::abs(last.x - first.x), std::abs(last.y - first.y));
	Vector2 cp2 = (first) + (d * firstCpDir) * rtir;
	Vector2 cp3 = (last) + (d * lastCpDir) * rtir;
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

//==============================================================================
// ShapesRenderFeature
//==============================================================================
//------------------------------------------------------------------------------
ShapesRenderFeature::ShapesRenderFeature()
	: m_manager(nullptr)
	, m_core(nullptr)
	, m_fillBrush()
{
}

//------------------------------------------------------------------------------
ShapesRenderFeature::~ShapesRenderFeature()
{
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_core = RefPtr<ShapesRendererCore>::makeRef();
	m_core->initialize(m_manager);
}

//------------------------------------------------------------------------------
void ShapesRenderFeature::ExecuteCommand(ShapesRendererCommandList* commandList)
{
	if (LN_CHECK_ARG(commandList != nullptr)) return;

	LN_ENQUEUE_RENDER_COMMAND_3(
		ExecuteCommand, m_manager,
		RefPtr<ShapesRendererCore>, m_core,
		RefPtr<ShapesRendererCommandList>, commandList,
		detail::BrushRawData, m_fillBrush,
		{
			m_core->RenderCommandList(commandList, &m_fillBrush);
		});
}
//------------------------------------------------------------------------------
void ShapesRenderFeature::onSetState(const DrawElementBatch* state)
{
	if (state->state.getBrush() != nullptr)
	{
		state->state.getBrush()->getRawData(&m_fillBrush);
	}
}

} // namespace detail
LN_NAMESPACE_END
