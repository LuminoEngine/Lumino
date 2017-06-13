
#include "../Internal.h"
#include <ft2build.h>
#include FT_IMAGE_H
#include FT_OUTLINE_H
#include "FontOutlineTessellator.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// FontOutlineTessellator
//==============================================================================

//------------------------------------------------------------------------------
FontOutlineTessellator::FontOutlineTessellator()
{
	m_gluTesselator = gluNewTess();

	// デフォルトは GLU_TESS_WINDING_ODD.
	// ただ、これだと凸面が重なるようなグリフで想定外の穴抜けが発生する。(メイリオの"驚" がわかりやすい)
	// とりあえず GLU_TESS_WINDING_NONZERO で全部埋めるようにする。
	gluTessProperty(m_gluTesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

	gluTessCallback(m_gluTesselator, GLU_TESS_BEGIN_DATA, (GLUTessCallback)BeginCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_END_DATA, (GLUTessCallback)EndCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_VERTEX_DATA, (GLUTessCallback)VertexDataCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_COMBINE_DATA, (GLUTessCallback)CombineCallback);
	gluTessCallback(m_gluTesselator, GLU_TESS_ERROR, (GLUTessCallback)ErrorCallback);
}

//------------------------------------------------------------------------------
FontOutlineTessellator::~FontOutlineTessellator()
{
	gluDeleteTess(m_gluTesselator);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::Tessellate(RawFont::VectorGlyphInfo* info)
{
	TessellatingState state;
	state.thisPtr = this;
	state.glyphInfo = info;

	::gluTessBeginPolygon(m_gluTesselator, &state);

	for (auto& outline : info->outlines)
	{
		::gluTessBeginContour(m_gluTesselator);

		for (int i = 0; i < outline.vertexCount; i++)
		{
			int vertexIndex = outline.startIndex + i;
			const Vector2& v = info->vertices[vertexIndex].pos;
			GLfloat coords[3] = { v.x, v.y, 0 };
			::gluTessVertex(m_gluTesselator, coords, reinterpret_cast<void*>(vertexIndex));
		}

		::gluTessEndContour(m_gluTesselator);
	}

	::gluTessEndPolygon(m_gluTesselator);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::BeginCallback(GLenum primitiveType, TessellatingState* state)
{
	Contour c;
	c.primitiveType = primitiveType;
	c.intermediateVertexIndex1 = -1;
	c.intermediateVertexIndex2 = -1;
	c.faceCount = 0;
	state->contourList.add(c);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::EndCallback(TessellatingState* state)
{

}

//------------------------------------------------------------------------------
// vertex : gluTessVertex() の data
// 点は時計回りで送られてくるようだ。
void FontOutlineTessellator::VertexDataCallback(void* vertexData, TessellatingState* state)
{
	Contour* contour = &state->contourList.getLast();
	int vertexIndex = reinterpret_cast<int>(vertexData);

	switch (contour->primitiveType)
	{
		// 通常の三角形リスト。3頂点が順に送られてくる。
		case GL_TRIANGLES:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				// 1つめの点
				contour->intermediateVertexIndex1 = vertexIndex;
				contour->intermediateVertexIndex2 = -1;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				// 2つめの点
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				// 3つの点ができあがった
				state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex1);
				state->glyphInfo->triangleIndices.add(vertexIndex);
				state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex2);
				contour->intermediateVertexIndex1 = -1;
			}
			break;
		}
		// TriangleStrip は最後の2点を再利用する。
		case GL_TRIANGLE_STRIP:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				contour->intermediateVertexIndex1 = vertexIndex;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				if (contour->faceCount & 1)	// 奇数回
				{
					state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex1);
					state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex2);
					state->glyphInfo->triangleIndices.add(vertexIndex);
				}
				else	// 偶数回 (初回含む)
				{
					state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex1);
					state->glyphInfo->triangleIndices.add(vertexIndex);
					state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex2);
				}

				contour->intermediateVertexIndex1 = contour->intermediateVertexIndex2;
				contour->intermediateVertexIndex2 = vertexIndex;
				contour->faceCount++;
			}
			break;
		}
		// TriangleFan は、最初の点をずっと再利用する。
		case GL_TRIANGLE_FAN:
		{
			if (contour->intermediateVertexIndex1 == -1)
			{
				contour->intermediateVertexIndex1 = vertexIndex;
			}
			else if (contour->intermediateVertexIndex2 == -1)
			{
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			else
			{
				state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex1);
				state->glyphInfo->triangleIndices.add(vertexIndex);
				state->glyphInfo->triangleIndices.add(contour->intermediateVertexIndex2);
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			break;
		}
	}
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::CombineCallback(GLfloat coords[3], void* vertex_data[4], GLfloat weight[4], void** out_data, TessellatingState* state)
{
	RawFont::FontOutlineVertex v(Vector2(coords[0], coords[1]));
	state->glyphInfo->vertices.add(v);
	*out_data = reinterpret_cast<void*>(state->glyphInfo->vertices.getCount() - 1);
}

//------------------------------------------------------------------------------
void FontOutlineTessellator::ErrorCallback(GLenum error_code)
{
	LN_ASSERT(0);	// TODO:
}


//==============================================================================
// FontOutlineStroker
//==============================================================================
//------------------------------------------------------------------------------
void FontOutlineStroker::MakeStroke(RawFont::VectorGlyphInfo* info)
{
	m_info = info;
	CalculateExtrusion();
	MakeAntiAliasStroke();
}

//------------------------------------------------------------------------------
void FontOutlineStroker::CalculateExtrusion()
{
	for (auto& v : m_info->vertices)
	{
		v.pos += 0.5;
	}

	for (const auto& outline : m_info->outlines)
	{
		int end = outline.startIndex + outline.vertexCount;
		for (int i = outline.startIndex; i < end; i++)
		{
			int iPrev = i - 1;
			if (iPrev < outline.startIndex) iPrev = end - 1;
			int iNext = i + 1;
			if (iNext >= end) iNext = outline.startIndex;

			auto& cur = m_info->vertices[i];
			auto& prev = m_info->vertices[iPrev];
			auto& next = m_info->vertices[iNext];

			//cur.pos.x += 0.5;

			Vector2 d0 = Vector2::normalize(cur.pos - prev.pos);//cur.pos - prev.pos;//
			Vector2 d1 = Vector2::normalize(next.pos - cur.pos);//next.pos - cur.pos;//
			//Vector2 v = prev.pos.x * next.pos.y - next.pos.x * prev.pos.y;
			//Vector2::clo
			float dlx0, dly0, dlx1, dly1, dmr2, cross, limit;
			dlx0 = d0.y;//p0->dy;
			dly0 = -d0.x;
			dlx1 = d1.y;
			dly1 = -d1.x;
			// Calculate extrusions
			// 進行方向の左側をさす
			cur.extrusion.x = -(dlx0 + dlx1) * 0.5f;
			cur.extrusion.y = -(dly0 + dly1) * 0.5f;
			cur.extrusion.normalize();

			cur.extrusion2 = cur.extrusion;
			cur.extrusion2.y = 0;
			//cur.extrusion2.normalize();
			//cur.extrusion *= 0.5;
		}
	}
}

//------------------------------------------------------------------------------
void FontOutlineStroker::MakeAntiAliasStroke()
{
	for (const auto& outline : m_info->outlines)
	{
		int end = outline.startIndex + outline.vertexCount;

		float extRate = 1.0;//0.85;//0.075;0.5f;//
		float extRateIn = 0.3f;

		int i = outline.startIndex;
		auto& cur = m_info->vertices[i];

		int iNextExt = 0;
		int iStartExt = m_info->vertices.getCount();
		int iCurExt = m_info->vertices.getCount();




		//if (Math::NearEqual(cur.extrusion.x, 0.0f) || Math::NearEqual(cur.extrusion.y, 0.0f))
		//{
		//	m_info->vertices.Add(cur.pos);
		//	m_info->vertices.GetLast().alpha = 0.0f;
		//}
		//else
		{
			m_info->vertices.add(cur.pos + cur.extrusion2 * extRate);
			m_info->vertices.getLast().alpha = 0.0f;
		}


		m_info->vertices[i].pos -= m_info->vertices[i].extrusion2 * extRateIn;
		//{
		//	m_info->vertices[i].pos -= m_info->vertices[i].extrusion * extRate;
		//}

		for (int i = outline.startIndex; i < end; i++)
		{
			//int iPrev = i - 1;
			//if (iPrev < outline.startIndex) iPrev = end - 1;
			int iNext = i + 1;
			if (iNext >= end) iNext = outline.startIndex;



			auto& cur = m_info->vertices[i];

			auto& next = m_info->vertices[iNext];


			//float extRate = 0.8f * cur.extrusion.y;

			if (iNext != outline.startIndex)	// start は押し出し済み
			{
				// next のを押し出す
				iNextExt = m_info->vertices.getCount();

				//if (Math::NearEqual(next.pos.x - cur.pos.x, 0.0f) || Math::NearEqual(next.pos.y - cur.pos.y, 0.0f))
				//{
				//	m_info->vertices.Add(next.pos);
				//	m_info->vertices.GetLast().alpha = 0.0f;
				//}
				//else
				{
					m_info->vertices.add(next.pos + next.extrusion2 * extRate);
					m_info->vertices.getLast().alpha = 0.0f;
				}

				//m_info->vertices.Add(next.pos + next.extrusion * extRate);
				//m_info->vertices.GetLast().alpha = 0.0f;


				m_info->vertices[iNext].pos -= m_info->vertices[iNext].extrusion2 * extRateIn;

			}
			else
			{
				iNextExt = iStartExt;
			}



			int i0 = i;
			int i1 = iNext;
			int i2 = iCurExt;
			int i3 = iNextExt;

			m_info->triangleIndices.add(i0);
			m_info->triangleIndices.add(i1);
			m_info->triangleIndices.add(i2);

			m_info->triangleIndices.add(i2);
			m_info->triangleIndices.add(i1);
			m_info->triangleIndices.add(i3);
			//Vector2 p0 = cur.pos;
			//Vector2 p1 = next.pos;
			//Vector2 p2 = m_vertexList[iCurExt].pos;

			iCurExt = iNextExt;
		}


		//return;
	}
}

} // namespace detail
LN_NAMESPACE_END
