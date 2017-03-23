
#include "../Internal.h"
#include <ft2build.h>
#include FT_IMAGE_H
#include FT_OUTLINE_H
#include "FreeTypeFontTessellator.h"

LN_NAMESPACE_BEGIN
namespace detail {



	Vector3 Filled::FTVectorToLNVector(const FT_Vector* ftVec)
	{
		return Vector3(
			(double)(ftVec->x / 64) + (double)(ftVec->x % 64) / 64.,
			(double)(ftVec->y / 64) + (double)(ftVec->y % 64) / 64.,
			0);
	}

	void Filled::Initialize()
	{
		m_ftOutlineFuncs.move_to = (FT_Outline_MoveTo_Func)ftMoveToCallback;
		m_ftOutlineFuncs.line_to = (FT_Outline_LineTo_Func)ftLineToCallback;
		m_ftOutlineFuncs.conic_to = (FT_Outline_ConicTo_Func)ftConicToCallback;
		m_ftOutlineFuncs.cubic_to = (FT_Outline_CubicTo_Func)ftCubicToCallback;
		m_ftOutlineFuncs.shift = 0;
		m_ftOutlineFuncs.delta = 0;

		m_gluTesselator = gluNewTess();

		gluTessCallback(m_gluTesselator, GLU_TESS_BEGIN_DATA, (GLUTessCallback)tessBeginCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_END_DATA, (GLUTessCallback)tessEndCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_VERTEX_DATA, (GLUTessCallback)vertexDataCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_COMBINE_DATA, (GLUTessCallback)combineCallback);
		gluTessCallback(m_gluTesselator, GLU_TESS_ERROR, (GLUTessCallback)errorCallback);
	}

	Filled::~Filled(void)
	{
		gluDeleteTess(m_gluTesselator);
	}


void Filled::DecomposeOutlineVertices(FreeTypeFont* font, UTF32 utf32code)
{
	font->UpdateFont();
	FT_Face face = font->GetFTFace();

	//FT_OutlineGlyph g;
	//FT_Error error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&g);

	FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		font->GetManager()->GetFTCacheMapCache(),
		font->GetFTCFaceId(),
		font->GetFTCacheMapIndex(),
		utf32code);

	//FT_Glyph glyph;
	FT_OutlineGlyph g;
	FTC_ImageTypeRec imageType = {};
	imageType.face_id = font->GetFTCFaceId();
	imageType.height = 12;
	imageType.width = 0;
	imageType.flags = 0;
	FT_Error error = FTC_ImageCache_Lookup(
		font->GetManager()->GetFTCImageCache(),
		&imageType,
		glyphIndex,
		(FT_Glyph*)&g,
		NULL);


	m_vectorScale = 0.5f;//(m_pointSize * m_resolution) / (72. * face->units_per_EM);



	error = FT_Outline_Decompose(&g->outline, &m_ftOutlineFuncs, this);

	//FT_Done_Glyph((FT_Glyph)g);


	// 1つ前の Outline があれば、頂点数を確定させる
	if (!m_contourOutlineList.IsEmpty())
	{
		ContourOutline& outline = m_contourOutlineList.GetLast();
		outline.indexCount = m_vertexList.GetCount() - outline.startIndex;
	}
}


// ポイントを移動するだけ。まだ点は置かない。
int Filled::ftMoveToCallback(FT_Vector* to, Filled* thisData)
{
	// 1つ前の Outline があれば、頂点数を確定させる
	if (!thisData->m_contourOutlineList.IsEmpty())
	{
		ContourOutline& outline = thisData->m_contourOutlineList.GetLast();
		outline.indexCount = thisData->m_vertexList.GetCount() - outline.startIndex;
	}

	// 新しい Contour を開始する
	ContourOutline contour;
	contour.startIndex = thisData->m_vertexList.GetCount();
	thisData->m_contourOutlineList.Add(contour);

	thisData->m_lastVertex = FTVectorToLNVector(to);
	//printf("moveto ");
	//thisData->m_lastVertex.Print();
	return 0;
}

int Filled::ftLineToCallback(FT_Vector* to, Filled* thisData)
{
	Vector3 v = FTVectorToLNVector(to);

	thisData->m_vertexList.Add(v * thisData->m_vectorScale);

	thisData->m_lastVertex = v;
	//thisData->m_lastVertex.Print();
	return 0;
}

int Filled::ftConicToCallback(FT_Vector* control, FT_Vector* to, Filled* thisData)
{
	Vector3 to_vertex = FTVectorToLNVector(to);
	Vector3 control_vertex = FTVectorToLNVector(control);

	double b[2], c[2], d[2], f[2], df[2], d2f[2];

	b[X] = thisData->m_lastVertex.x - 2 * control_vertex.x +
		to_vertex.x;
	b[Y] = thisData->m_lastVertex.y - 2 * control_vertex.y +
		to_vertex.y;

	c[X] = -2 * thisData->m_lastVertex.x + 2 * control_vertex.x;
	c[Y] = -2 * thisData->m_lastVertex.y + 2 * control_vertex.y;

	d[X] = thisData->m_lastVertex.x;
	d[Y] = thisData->m_lastVertex.y;

	f[X] = d[X];
	f[Y] = d[Y];
	df[X] = c[X] * thisData->m_delta1 + b[X] * thisData->m_delta2;
	df[Y] = c[Y] * thisData->m_delta1 + b[Y] * thisData->m_delta2;
	d2f[X] = 2 * b[X] * thisData->m_delta2;
	d2f[Y] = 2 * b[Y] * thisData->m_delta2;

	for (unsigned int i = 0; i < thisData->m_tessellationStep - 1; i++)
	{
		f[X] += df[X];
		f[Y] += df[Y];

		Vector3 v(f[0], f[1], 0);
		v *= thisData->m_vectorScale;
		thisData->m_vertexList.Add(v);

		df[X] += d2f[X];
		df[Y] += d2f[Y];
	}

	Vector3 v = FTVectorToLNVector(to);
	v *= thisData->m_vectorScale;
	thisData->m_vertexList.Add(v);

	thisData->m_lastVertex = to_vertex;

	return 0;
}

int Filled::ftCubicToCallback(FT_Vector* control1, FT_Vector* control2, FT_Vector* to, Filled* thisData)
{
	Vector3 to_vertex = FTVectorToLNVector(to);
	Vector3 control1_vertex = FTVectorToLNVector(control1);
	Vector3 control2_vertex = FTVectorToLNVector(control2);

	double a[2], b[2], c[2], d[2], f[2], df[2], d2f[2], d3f[2];

	a[X] = -thisData->m_lastVertex.x + 3 * control1_vertex.x
		- 3 * control2_vertex.x + to_vertex.x;
	a[Y] = -thisData->m_lastVertex.y + 3 * control1_vertex.y
		- 3 * control2_vertex.y + to_vertex.y;

	b[X] = 3 * thisData->m_lastVertex.x - 6 * control1_vertex.x +
		3 * control2_vertex.x;
	b[Y] = 3 * thisData->m_lastVertex.y - 6 * control1_vertex.y +
		3 * control2_vertex.y;

	c[X] = -3 * thisData->m_lastVertex.x + 3 * control1_vertex.x;
	c[Y] = -3 * thisData->m_lastVertex.y + 3 * control1_vertex.y;

	d[X] = thisData->m_lastVertex.x;
	d[Y] = thisData->m_lastVertex.y;

	f[X] = d[X];
	f[Y] = d[Y];
	df[X] = c[X] * thisData->m_delta1 + b[X] * thisData->m_delta2
		+ a[X] * thisData->m_delta3;
	df[Y] = c[Y] * thisData->m_delta1 + b[Y] * thisData->m_delta2
		+ a[Y] * thisData->m_delta3;
	d2f[X] = 2 * b[X] * thisData->m_delta2 + 6 * a[X] * thisData->m_delta3;
	d2f[Y] = 2 * b[Y] * thisData->m_delta2 + 6 * a[Y] * thisData->m_delta3;
	d3f[X] = 6 * a[X] * thisData->m_delta3;
	d3f[Y] = 6 * a[Y] * thisData->m_delta3;

	for (unsigned int i = 0; i < thisData->m_tessellationStep - 1; i++)
	{
		f[X] += df[X];
		f[Y] += df[Y];

		Vector3 v(f[0], f[1], 0);
		v *= thisData->m_vectorScale;
		thisData->m_vertexList.Add(v);

		df[X] += d2f[X];
		df[Y] += d2f[Y];
		d2f[X] += d3f[X];
		d2f[Y] += d3f[Y];
	}

	Vector3 v = FTVectorToLNVector(to);
	v *= thisData->m_vectorScale;
	thisData->m_vertexList.Add(v);

	thisData->m_lastVertex = to_vertex;

	return 0;
}


void Filled::Tessellate()
{
	::gluTessBeginPolygon(m_gluTesselator, this);

	for (const ContourOutline& outline : m_contourOutlineList)
	{
		::gluTessBeginContour(m_gluTesselator);

		for (int i = 0; i < outline.indexCount; i++)
		{
			int vertexIndex = outline.startIndex + i;
			const Vector3& v = m_vertexList[vertexIndex];
			GLfloat coords[3] = {v.x, v.y, 0};
			::gluTessVertex(m_gluTesselator, coords, reinterpret_cast<void*>(vertexIndex));
		}

		::gluTessEndContour(m_gluTesselator);
	}

	::gluTessEndPolygon(m_gluTesselator);
}


void Filled::tessBeginCallback(GLenum primitiveType, Filled* thisData)
{
	Contour c;
	c.primitiveType = primitiveType;
	c.intermediateVertexIndex1 = -1;
	c.intermediateVertexIndex2 = -1;
	thisData->m_contourList.Add(c);
}

void Filled::tessEndCallback(Filled* thisData)
{
}

// vertex : gluTessVertex() の data
// 点は時計回りで送られてくるようだ。
void Filled::vertexDataCallback(void* vertexData, Filled* thisData)
{
	Contour* contour = &thisData->m_contourList.GetLast();
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
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
				thisData->m_triangleIndexList.Add(vertexIndex);
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
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
#if 1

				if (thisData->m_triangleIndexList.GetCount() & 1)	// 奇数回
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
					thisData->m_triangleIndexList.Add(vertexIndex);
				}
				else	// 偶数回 (初回含む)
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(vertexIndex);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
				}
				contour->intermediateVertexIndex1 = contour->intermediateVertexIndex2;
				contour->intermediateVertexIndex2 = vertexIndex;
#else
				if (thisData->m_triangleIndexList.GetCount() & 1)	// 奇数回
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(vertexIndex);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
				}
				else	// 偶数回 (初回含む)
				{
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
					thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
					thisData->m_triangleIndexList.Add(vertexIndex);


					thisData->m_vertexList[contour->intermediateVertexIndex1].Print();
					thisData->m_vertexList[contour->intermediateVertexIndex2].Print();
					thisData->m_vertexList[vertexIndex].Print();

				}
				contour->intermediateVertexIndex1 = contour->intermediateVertexIndex2;
				contour->intermediateVertexIndex2 = vertexIndex;
#endif
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
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex1);
				thisData->m_triangleIndexList.Add(vertexIndex);
				thisData->m_triangleIndexList.Add(contour->intermediateVertexIndex2);
				contour->intermediateVertexIndex2 = vertexIndex;
			}
			break;
		}
	}
}

void Filled::combineCallback(GLdouble coords[3], void* vertex_data[4], GLfloat weight[4], void** out_data, Filled* thisData)
{
	(void)vertex_data;
	(void)weight;
	//    std::cerr << "called combine" << std::endl;
	//VertexInfo* vertex = new VertexInfo(coords);
	//*out_data = vertex;
	//filled->extraVertices().push_back(vertex);
	LN_ASSERT(0);
}

void Filled::errorCallback(GLenum error_code)
{
	//std::cerr << "hmm. error during tessellation?:" << gluErrorString(error_code) << std::endl;
}






#if 0
	void Filled::renderGlyph(FreeTypeFont* font, UTF32 utf32code)
	{
		//FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);

		//if (error != 0)
		//	return;

		//FT_OutlineGlyph g;

		//error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&g);

		//if (error != 0)
		//	return;



		//FT_UInt glyphIndex = FTC_CMapCache_Lookup(
		//	font->GetManager()->GetFTCacheMapCache(),
		//	m_ftFaceID,
		//	m_ftCacheMapIndex,
		//	utf32code);

		//FT_Glyph glyph;
		//FT_Error err = FTC_ImageCache_Lookup(
		//	m_manager->GetFTCImageCache(),
		//	&m_ftImageType,
		//	glyphIndex,
		//	&glyph,
		//	NULL);

		font->UpdateFont();
		FT_Face face = font->GetFTFace();

		//FT_OutlineGlyph g;
		//FT_Error error = FT_Get_Glyph(face->glyph, (FT_Glyph*)&g);
		
		FT_UInt glyphIndex = FTC_CMapCache_Lookup(
			font->GetManager()->GetFTCacheMapCache(),
			font->GetFTCFaceId(),
			font->GetFTCacheMapIndex(),
			utf32code);

		//FT_Glyph glyph;
		FT_OutlineGlyph g;
		FTC_ImageTypeRec imageType = {};
		imageType.face_id = font->GetFTCFaceId();
		imageType.height = 12;
		imageType.width = 0;
		imageType.flags = 0;
		FT_Error error = FTC_ImageCache_Lookup(
			font->GetManager()->GetFTCImageCache(),
			&imageType,
			glyphIndex,
			(FT_Glyph*)&g,
			NULL);


		//FT_OutlineGlyph g;
		//FT_Error error = FT_Get_Glyph(glyph, (FT_Glyph*)&g);







		m_vectorScale = (m_pointSize * m_resolution) / (72. * face->units_per_EM);

		//if (character_rotation_.active_) {
		//	glPushMatrix();
		//	glTranslatef((face->glyph->metrics.width / 2. +
		//		face->glyph->metrics.horiBearingX) / 64.
		//		* m_vectorScale,
		//		rotation_offset_y_,
		//		0.);

		//	if (character_rotation_.x_ != 0.)
		//		glRotatef(character_rotation_.x_, 1., 0., 0.);

		//	if (character_rotation_.y_ != 0.)
		//		glRotatef(character_rotation_.y_, 0., 1., 0.);

		//	if (character_rotation_.z_ != 0.)
		//		glRotatef(character_rotation_.z_, 0., 0., 1.);

		//	glTranslatef(-(face->glyph->metrics.width / 2. +
		//		face->glyph->metrics.horiBearingX) / 64.
		//		* m_vectorScale,
		//		-rotation_offset_y_,
		//		0.);
		//}

		//if (depth_offset_ != 0.) {
		//	glPushMatrix();
		//	glTranslatef(0., 0., depth_offset_);
		//	glNormal3f(0., 0., 1.);
		//}
		//else {
		//	glNormal3f(0., 0., -1.);
		//}

		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		contour_open_ = false;

		gluTessBeginPolygon(m_gluTesselator, this);

		// The Big Kahuna: the FreeType glyph decomposition routine traverses
		// the outlines of the font by calling the various routines stored in
		// m_ftOutlineFuncs. These routines in turn call the GLU tessellation routines
		// to create OGL polygons.

		error = FT_Outline_Decompose(&g->outline, &m_ftOutlineFuncs, this);

		FT_Done_Glyph((FT_Glyph)g);

		// Some glyphs may be empty (the 'blank' for instance!)

		if (contour_open_)
			gluTessEndContour(m_gluTesselator);

		gluTessEndPolygon(m_gluTesselator);

		//if (depth_offset_ != 0.) {
		//	glPopMatrix();
		//}
		//if (character_rotation_.active_) {
		//	glPopMatrix();
		//}

		// Drawing a character always advances the MODELVIEW.

		//glTranslatef(face->glyph->advance.x / 64 * m_vectorScale,
		//	face->glyph->advance.y / 64 * m_vectorScale,
		//	0.);

		//for (VILI vili = extra_vertices_.begin(); vili != extra_vertices_.end(); vili++)
		//	delete *vili;

		//extra_vertices_.clear();

		//for (VILI vili = vertices_.begin(); vili != vertices_.end(); vili++)
		//	delete *vili;

		//vertices_.clear();
	}

	// ポイントを移動するだけ。まだ点は置かない。
	int Filled::ftMoveToCallback(FT_Vector* to, Filled* filled)
	{
		if (filled->contour_open_) {
			gluTessEndContour(filled->m_gluTesselator);
		}

		filled->m_lastVertex = FTVectorToLNVector(to);

		gluTessBeginContour(filled->m_gluTesselator);

		filled->contour_open_ = true;

		return 0;
	}

	int Filled::ftLineToCallback(FT_Vector* to, Filled* filled)
	{
		filled->m_lastVertex = FTVectorToLNVector(to);

		Vector3 vertex = filled->m_lastVertex;
		vertex *= filled->m_vectorScale;

		//VertexInfo* vertex = new VertexInfo(to, filled->colorTess(), filled->textureTess());

		////float y = 
		//vertex->v_[X] *= filled->m_vectorScale;
		//vertex->v_[Y] *= filled->m_vectorScale;

		gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, (void*)1);//nullptr/*vertex*/);

		//filled->vertices_.push_back(vertex);

		return 0;
	}

	int Filled::ftConicToCallback(FT_Vector* control, FT_Vector* to, Filled* filled)
	{
		// This is crude: Step off conics with a fixed number of increments

		//VertexInfo to_vertex(to, filled->colorTess(), filled->textureTess());
		//VertexInfo control_vertex(control, filled->colorTess(), filled->textureTess());
		Vector3 to_vertex = FTVectorToLNVector(to);
		Vector3 control_vertex = FTVectorToLNVector(control);

		double b[2], c[2], d[2], f[2], df[2], d2f[2];

		b[X] = filled->m_lastVertex.x - 2 * control_vertex.x +
			to_vertex.x;
		b[Y] = filled->m_lastVertex.y - 2 * control_vertex.y +
			to_vertex.y;

		c[X] = -2 * filled->m_lastVertex.x + 2 * control_vertex.x;
		c[Y] = -2 * filled->m_lastVertex.y + 2 * control_vertex.y;

		d[X] = filled->m_lastVertex.x;
		d[Y] = filled->m_lastVertex.y;

		f[X] = d[X];
		f[Y] = d[Y];
		df[X] = c[X] * filled->m_delta1 + b[X] * filled->m_delta2;
		df[Y] = c[Y] * filled->m_delta1 + b[Y] * filled->m_delta2;
		d2f[X] = 2 * b[X] * filled->m_delta2;
		d2f[Y] = 2 * b[Y] * filled->m_delta2;

		for (unsigned int i = 0; i < filled->m_tessellationStep - 1; i++) {

			f[X] += df[X];
			f[Y] += df[Y];

			//VertexInfo* vertex = new VertexInfo(f, filled->colorTess(), filled->textureTess());

			//vertex->v_[X] *= filled->m_vectorScale;
			//vertex->v_[Y] *= filled->m_vectorScale;

			////filled->vertices_.push_back(vertex);

			//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);


			Vector3 vertex(f[0], f[1], 0);
			vertex *= filled->m_vectorScale;
			gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);


			df[X] += d2f[X];
			df[Y] += d2f[Y];
		}

		//VertexInfo* vertex = new VertexInfo(to, filled->colorTess(), filled->textureTess());

		//vertex->v_[X] *= filled->m_vectorScale;
		//vertex->v_[Y] *= filled->m_vectorScale;

		////filled->vertices_.push_back(vertex);

		//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);
		Vector3 vertex = FTVectorToLNVector(to);
		vertex *= filled->m_vectorScale;
		gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);

		filled->m_lastVertex = to_vertex;

		return 0;
	}

	int Filled::ftCubicToCallback(FT_Vector* control1, FT_Vector* control2,
		FT_Vector* to, Filled* filled)
	{
		// This is crude: Step off cubics with a fixed number of increments

		//VertexInfo to_vertex(to, filled->colorTess(), filled->textureTess());
		//VertexInfo control1_vertex(control1, filled->colorTess(), filled->textureTess());
		//VertexInfo control2_vertex(control2, filled->colorTess(), filled->textureTess());

		Vector3 to_vertex = FTVectorToLNVector(to);
		Vector3 control1_vertex = FTVectorToLNVector(control1);
		Vector3 control2_vertex = FTVectorToLNVector(control2);

		double a[2], b[2], c[2], d[2], f[2], df[2], d2f[2], d3f[2];

		a[X] = -filled->m_lastVertex.x + 3 * control1_vertex.x
			- 3 * control2_vertex.x + to_vertex.x;
		a[Y] = -filled->m_lastVertex.y + 3 * control1_vertex.y
			- 3 * control2_vertex.y + to_vertex.y;

		b[X] = 3 * filled->m_lastVertex.x - 6 * control1_vertex.x +
			3 * control2_vertex.x;
		b[Y] = 3 * filled->m_lastVertex.y - 6 * control1_vertex.y +
			3 * control2_vertex.y;

		c[X] = -3 * filled->m_lastVertex.x + 3 * control1_vertex.x;
		c[Y] = -3 * filled->m_lastVertex.y + 3 * control1_vertex.y;

		d[X] = filled->m_lastVertex.x;
		d[Y] = filled->m_lastVertex.y;

		f[X] = d[X];
		f[Y] = d[Y];
		df[X] = c[X] * filled->m_delta1 + b[X] * filled->m_delta2
			+ a[X] * filled->m_delta3;
		df[Y] = c[Y] * filled->m_delta1 + b[Y] * filled->m_delta2
			+ a[Y] * filled->m_delta3;
		d2f[X] = 2 * b[X] * filled->m_delta2 + 6 * a[X] * filled->m_delta3;
		d2f[Y] = 2 * b[Y] * filled->m_delta2 + 6 * a[Y] * filled->m_delta3;
		d3f[X] = 6 * a[X] * filled->m_delta3;
		d3f[Y] = 6 * a[Y] * filled->m_delta3;

		for (unsigned int i = 0; i < filled->m_tessellationStep - 1; i++) {

			f[X] += df[X];
			f[Y] += df[Y];

			//VertexInfo* vertex = new VertexInfo(f, filled->colorTess(), filled->textureTess());

			//vertex->v_[X] *= filled->m_vectorScale;
			//vertex->v_[Y] *= filled->m_vectorScale;

			////filled->vertices_.push_back(vertex);

			//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);
			Vector3 vertex(f[0], f[1], 0);
			vertex *= filled->m_vectorScale;
			gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);

			df[X] += d2f[X];
			df[Y] += d2f[Y];
			d2f[X] += d3f[X];
			d2f[Y] += d3f[Y];
		}

		//VertexInfo* vertex = new VertexInfo(to, filled->colorTess(), filled->textureTess());

		//vertex->v_[X] *= filled->m_vectorScale;
		//vertex->v_[Y] *= filled->m_vectorScale;

		////filled->vertices_.push_back(vertex);

		//gluTessVertex(filled->m_gluTesselator, vertex->v_, vertex);
		Vector3 vertex = FTVectorToLNVector(to);
		vertex *= filled->m_vectorScale;
		gluTessVertex(filled->m_gluTesselator, (GLfloat*)&vertex, nullptr/*vertex*/);

		filled->m_lastVertex = to_vertex;

		return 0;
	}

	void Filled::beginCallback(GLenum primitiveType)
	{
		printf("");
		//GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, or GL_TRIANGLES
		
	}

	void Filled::endCallback()
	{

	}

	// vertex : gluTessVertex() の data
	void Filled::vertexDataCallback(VertexInfo* vertex, void* userData)
	{
		//glVertex3dv(vertex->v_);
		vertex->Print();
	}

	void Filled::combineCallback(GLdouble coords[3], void* vertex_data[4],
		GLfloat weight[4], void** out_data,
		Filled* filled)
	{
		(void)vertex_data;
		(void)weight;
		//    std::cerr << "called combine" << std::endl;
		//VertexInfo* vertex = new VertexInfo(coords);
		//*out_data = vertex;
		//filled->extraVertices().push_back(vertex);
	}

	void Filled::errorCallback(GLenum error_code)
	{
		//std::cerr << "hmm. error during tessellation?:" << gluErrorString(error_code) << std::endl;
	}
#endif

} // namespace detail
LN_NAMESPACE_END
