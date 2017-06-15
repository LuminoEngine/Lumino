
#pragma once
#include <glues/source/glues.h>
#include "FreeTypeFont.h"

LN_NAMESPACE_BEGIN
namespace detail {




class FontOutlineTessellator
{
public:
	FontOutlineTessellator();
	~FontOutlineTessellator();

	void tessellate(RawFont::VectorGlyphInfo* info);

private:
	typedef void (APIENTRYP GLUTessCallback)();

	struct Contour
	{
		// GLenum (GL_TRIANGLE_FAN, GL_TRIANGLE_STRIP, or GL_TRIANGLES)
		int	primitiveType;

		// TriangleFan と、TriangleStrip の後続頂点のインデックス。三角形を構成する1つめの頂点番号。
		int	intermediateVertexIndex1;

		// TriangleFan と、TriangleStrip の後続頂点のインデックス。三角形を構成する2つめの頂点番号。
		int	intermediateVertexIndex2;

		// TriangleStrip の面張り時に面の正面方向を決めるために使用する
		int faceCount;
	};

	struct TessellatingState
	{
		FontOutlineTessellator*		thisPtr;
		RawFont::VectorGlyphInfo*	glyphInfo;
		List<Contour>				contourList;
	};

	static void APIENTRY beginCallback(GLenum primitiveType, TessellatingState* state);
	static void APIENTRY endCallback(TessellatingState* state);
	static void APIENTRY vertexDataCallback(void* vertexData, TessellatingState* state);
	static void APIENTRY combineCallback(GLfloat coords[3], void* vertex_data[4], GLfloat weight[4], void** out_data, TessellatingState* state);
	static void APIENTRY errorCallback(GLenum error_code);

	GLUtesselator*		m_gluTesselator;
};

class FontOutlineStroker
{
public:

	void makeStroke(RawFont::VectorGlyphInfo* info);

private:
	void calculateExtrusion();
	void makeAntiAliasStroke();
	
	RawFont::VectorGlyphInfo* m_info;
};

} // namespace detail
LN_NAMESPACE_END
