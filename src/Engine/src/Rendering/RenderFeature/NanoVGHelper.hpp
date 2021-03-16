#pragma once

typedef unsigned int GLenum;
typedef unsigned int GLuint;

#include <nanovg/nanovg.h>

#include <LuminoEngine/Graphics/RenderState.hpp>

//==============================================================================
// see: build/ExternalSource/nanovg/src/nanovg_gl.h
// ★ NanoVG はまだ 1.0.0 未リリースで、API やドライバが変更されることがある。
//    そのためマージをするときの負担を減らすため、識別子の名前等を nanovg_gl.h そのまま使用している。
//    ところどころ "GL" という名前が出てくるが、OpenGL 専用ではない点に注意。

#define NANOVG_GL_USE_UNIFORMBUFFER 1

enum NVGcreateFlags {
	// Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
	NVG_ANTIALIAS = 1 << 0,
	// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
	// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
	NVG_STENCIL_STROKES = 1 << 1,
	// Flag indicating that additional debug checks are done.
	NVG_DEBUG = 1 << 2,
};

//enum GLNVGuniformLoc {
//	GLNVG_LOC_VIEWSIZE,
//	GLNVG_LOC_TEX,
//	GLNVG_LOC_FRAG,
//	GLNVG_MAX_LOCS
//};

enum GLNVGshaderType {
	NSVG_SHADER_FILLGRAD,
	NSVG_SHADER_FILLIMG,
	NSVG_SHADER_SIMPLE,
	NSVG_SHADER_IMG
};

//#if NANOVG_GL_USE_UNIFORMBUFFER
//enum GLNVGuniformBindings {
//	GLNVG_FRAG_BINDING = 0,
//};
//#endif

//struct GLNVGshader {
//	GLuint prog;
//	GLuint frag;
//	GLuint vert;
//	GLint loc[GLNVG_MAX_LOCS];
//};
//typedef struct GLNVGshader GLNVGshader;

struct GLNVGtexture {
	int id;
	ln::Ref<ln::Texture> tex;
	int width, height;
	int type;
	int flags;
};
typedef struct GLNVGtexture GLNVGtexture;

struct GLNVGblend
{
	ln::BlendFactor srcRGB;
	ln::BlendFactor dstRGB;
	ln::BlendFactor srcAlpha;
	ln::BlendFactor dstAlpha;
};
typedef struct GLNVGblend GLNVGblend;

enum GLNVGcallType {
	GLNVG_NONE = 0,
	GLNVG_FILL,
	GLNVG_CONVEXFILL,
	GLNVG_STROKE,
	GLNVG_TRIANGLES,
};

struct GLNVGcall {
	GLNVGcallType type;
	int image;
	int pathOffset;			// Call に含まれている最初の Path までの Offset
	int pathCount;
	int triangleOffset;
	int triangleCount;
	int uniformOffset;		// このドローコールで使う UniformBuffer の場所。データ位置までのバイトオフセット。
	GLNVGblend blendFunc;
};
typedef struct GLNVGcall GLNVGcall;

struct GLNVGpath {
	int fillOffset;
	int fillCount;
	int strokeOffset;
	int strokeCount;
};
typedef struct GLNVGpath GLNVGpath;

struct GLNVGfragUniforms {
	float scissorMat[12]; // matrices are actually 3 vec4s
	float paintMat[12];
	ln::Vector4 viewSize;
	struct NVGcolor innerCol;
	struct NVGcolor outerCol;
	float scissorExt[2];
	float scissorScale[2];
	float extent[2];
	float radius;
	float feather;
	float strokeMult;
	float strokeThr;
	int texType;
	int type;		// GLNVGshaderType
};
typedef struct GLNVGfragUniforms GLNVGfragUniforms;

struct GLNVGcontextBase {
	//GLNVGshader shader;
	GLNVGtexture* textures;
	float view[2];
	int ntextures;
	int ctextures;
	int textureId;
	//GLuint vertBuf;
#if defined NANOVG_GL3
	GLuint vertArr;
#endif
	//ln::detail::ConstantBufferView fragBuf;

	int fragSize;
	int flags;

	// Per frame buffers
	GLNVGcall* calls;			// Call 実データ (必要に応じて realloc で伸長)
	int ccalls;					// メモリ確保済み Call 数
	int ncalls;					// 使用中 Call 数
	GLNVGpath* paths;			// Path 実データ (必要に応じて realloc で伸長)
	int cpaths;					// メモリ確保済み Path 数
	int npaths;					// 使用中 Paths 数
	struct NVGvertex* verts;	// Vertex 実データ (必要に応じて realloc で伸長)
	int cverts;					// メモリ確保済み vertex 数
	int nverts;					// 使用中 vertex 数
	unsigned char* uniforms;	// Uniform 実データ (必要に応じて realloc で伸長)
	int cuniforms;				// メモリ確保済み Uniform 数
	int nuniforms;				// 使用中 Uniform 数

	// cached state
#if NANOVG_GL_USE_STATE_FILTER
	GLuint boundTexture;
	GLuint stencilMask;
	GLenum stencilFunc;
	GLint stencilFuncRef;
	GLuint stencilFuncMask;
#endif
	GLNVGblend blendFunc;

	int dummyTex;

	void clear();
	void makeNVGparams(NVGparams* params);
	static GLNVGtexture* findTexture(GLNVGcontextBase* gl, int id);
	void dispose();
};
//typedef struct GLNVGcontext GLNVGcontext;
