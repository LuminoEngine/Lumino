﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/VertexLayout.hpp>
#include <LuminoEngine/Graphics/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/IndexBuffer.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/Graphics/SwapChain.hpp>
#include <LuminoEngine/Shader/ShaderDescriptor.hpp>
#include "../../Graphics/GraphicsManager.hpp"
#include "../RenderingManager.hpp"
#include "PathRenderFeature.hpp"

// [2020/11/17] Shader を使うことになるので、Descripter の対応 (Material ごとに UniformBuffer を持てるようにする) が済んでからにする。
#if 1

typedef unsigned int GLenum;
typedef unsigned int GLuint;

#include <nanovg/nanovg.h>

/**

最も基本的な API の呼び出し↓
```
nvgBeginFrame(...);
nvgBeginPath(vg);
nvgRect(vg, 100,100, 120,30);
nvgFillColor(vg, nvgRGBA(255,192,0,255));
nvgFill(vg);
nvgEndFrame(...);
```

1. nvgFill, nvgStroke, nvgText　のタイミングで、それぞれ renderFill, renderStroke, renderTriangles コールバックが呼び出される。
   ここで "Call" というデータを 1 つ作る。
   （"Call" は nanovg の内部データではなく、ドライバレイヤーで利用するデータ構造。ドローコールひとつに相当）
   まだ GraphicsAPI へドローコールは出さない。
2. glnvg__renderFlush() で、溜まった Call を GraphicsAPI へ送り出す。
   ここから Call の性質に応じて
   glnvg__fill, glnvg__stroke, glnvg__triangles が呼び出される。
	

UniformBuffer の使われ方
----------
- 実際に作られる OpenGL の UBO はひとつだけ。
- render* の時に realloc で、ドローコールの数分の GLNVGfragUniforms が入るバッファを Host 側に確保する。
- flush の時に glBufferData() でこのバッファを丸ごとひとつの UBO へ転送し、glDraw** するときに glBindBufferRange で参照位置を指定している。

 */


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

struct GLNVGcontext {
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

	ln::detail::PathRenderFeature* renderFeature;
	ln::Ref<ln::VertexLayout> vertexLayout;
	ln::Ref<ln::VertexBuffer> vertexBuffer;
	ln::Shader* shader;
	ln::GraphicsContext* g = nullptr;
	ln::detail::GraphicsCommandList* commandList() const { return g->commandList(); }
};
typedef struct GLNVGcontext GLNVGcontext;

static int glnvg__maxi(int a, int b) { return a > b ? a : b; }

static void glnvg__blendFuncSeparate(GLNVGcontext* gl, const GLNVGblend* blend)
{
	if ((gl->blendFunc.srcRGB != blend->srcRGB) ||
		(gl->blendFunc.dstRGB != blend->dstRGB) ||
		(gl->blendFunc.srcAlpha != blend->srcAlpha) ||
		(gl->blendFunc.dstAlpha != blend->dstAlpha)) {

		gl->blendFunc = *blend;

		ln::BlendStateDesc desc;
		desc.independentBlendEnable = false;
		desc.renderTargets[0].blendEnable = true;
		desc.renderTargets[0].sourceBlend = blend->srcRGB;
		desc.renderTargets[0].destinationBlend = blend->dstRGB;
		desc.renderTargets[0].blendOp = ln::BlendOp::Add;
		desc.renderTargets[0].sourceBlendAlpha = blend->srcAlpha;
		desc.renderTargets[0].destinationBlendAlpha = blend->dstAlpha;
		desc.renderTargets[0].blendOpAlpha = ln::BlendOp::Add;
		gl->g->setBlendState(desc);
	}
}

static GLNVGtexture* glnvg__findTexture(GLNVGcontext* gl, int id)
{
	int i;
	for (i = 0; i < gl->ntextures; i++)
		if (gl->textures[i].id == id)
			return &gl->textures[i];
	return NULL;
}

// params.renderCreate
static int glnvg__renderCreate(void* uptr)
{
	// NanoVG の内部初期化後に呼び出される処理。
	// OpenGL サンプルでは OpenGL リソースの初期化を行っている。

	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	gl->shader = gl->renderFeature->manager()->builtinShader(ln::detail::BuiltinShader::NanoVG);

	gl->vertexLayout = ln::makeObject<ln::VertexLayout>();
	gl->vertexLayout->addElement(0, ln::VertexElementType::Float2, ln::VertexElementUsage::Position, 0);
	gl->vertexLayout->addElement(0, ln::VertexElementType::Float2, ln::VertexElementUsage::TexCoord, 0);

	size_t align = gl->renderFeature->manager()->graphicsManager()->deviceContext()->caps().uniformBufferOffsetAlignment;
	gl->fragSize = sizeof(GLNVGfragUniforms) + align - sizeof(GLNVGfragUniforms) % align;

	static_assert(192 == sizeof(GLNVGfragUniforms), "Invalid sizeof(GLNVGfragUniforms)");
	static_assert(96 == offsetof(GLNVGfragUniforms, viewSize), "Invalid offsetof(GLNVGfragUniforms, viewSize)");
	static_assert(184 == offsetof(GLNVGfragUniforms, texType), "Invalid offsetof(GLNVGfragUniforms, texType)");
	static_assert(188 == offsetof(GLNVGfragUniforms, type), "Invalid offsetof(GLNVGfragUniforms, type)");

	return 1;
}

// params.renderCreateTexture
static int glnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
{
	// ITexture → NanoVGImage の対応付けは lnnvg__AddImageTexture で行っている。
	// このドライバ関数が呼び出されるのは、NanoVG のテキスト描画機能からのみで、
	// いまは NanoVG 初期化時にしか使われない。ここで作られたイメージも使用されることはない。
	return INT_MAX;
}

// params.renderDeleteTexture
static int glnvg__renderDeleteTexture(void* uptr, int image)
{
	// 削除はここでする必要はない
	return 1;	// 成功扱い
}

// params.renderUpdateTexture
static int glnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
{
	// テキスト描画機能を使うときに必要になる
	LN_UNREACHABLE();	// unused
	return 0;
}

// params.renderGetTextureSize
static int glnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h)
{
	// テキスト描画機能を使うときに必要になる
	LN_UNREACHABLE();	// unused
	return 0;
}

static void glnvg__xformToMat3x4(float* m3, float* t)
{
	m3[0] = t[0];
	m3[1] = t[1];
	m3[2] = 0.0f;
	m3[3] = 0.0f;
	m3[4] = t[2];
	m3[5] = t[3];
	m3[6] = 0.0f;
	m3[7] = 0.0f;
	m3[8] = t[4];
	m3[9] = t[5];
	m3[10] = 1.0f;
	m3[11] = 0.0f;
}

static NVGcolor glnvg__premulColor(NVGcolor c)
{
	c.r *= c.a;
	c.g *= c.a;
	c.b *= c.a;
	return c;
}

static int glnvg__convertPaint(GLNVGcontext* gl, GLNVGfragUniforms* frag, NVGpaint* paint,
	NVGscissor* scissor, float width, float fringe, float strokeThr)
{
	GLNVGtexture* tex = NULL;
	float invxform[6];

	memset(frag, 0, sizeof(*frag));

	frag->viewSize.x = gl->view[0];
	frag->viewSize.y = gl->view[1];
	frag->innerCol = glnvg__premulColor(paint->innerColor);
	frag->outerCol = glnvg__premulColor(paint->outerColor);

	if (scissor->extent[0] < -0.5f || scissor->extent[1] < -0.5f) {
		memset(frag->scissorMat, 0, sizeof(frag->scissorMat));
		frag->scissorExt[0] = 1.0f;
		frag->scissorExt[1] = 1.0f;
		frag->scissorScale[0] = 1.0f;
		frag->scissorScale[1] = 1.0f;
	}
	else {
		nvgTransformInverse(invxform, scissor->xform);
		glnvg__xformToMat3x4(frag->scissorMat, invxform);
		frag->scissorExt[0] = scissor->extent[0];
		frag->scissorExt[1] = scissor->extent[1];
		frag->scissorScale[0] = sqrtf(scissor->xform[0] * scissor->xform[0] + scissor->xform[2] * scissor->xform[2]) / fringe;
		frag->scissorScale[1] = sqrtf(scissor->xform[1] * scissor->xform[1] + scissor->xform[3] * scissor->xform[3]) / fringe;
	}

	memcpy(frag->extent, paint->extent, sizeof(frag->extent));
	frag->strokeMult = (width * 0.5f + fringe * 0.5f) / fringe;
	frag->strokeThr = strokeThr;

	if (paint->image != 0) {
		tex = glnvg__findTexture(gl, paint->image);
		if (tex == NULL) return 0;
		if ((tex->flags & NVG_IMAGE_FLIPY) != 0) {
			float m1[6], m2[6];
			nvgTransformTranslate(m1, 0.0f, frag->extent[1] * 0.5f);
			nvgTransformMultiply(m1, paint->xform);
			nvgTransformScale(m2, 1.0f, -1.0f);
			nvgTransformMultiply(m2, m1);
			nvgTransformTranslate(m1, 0.0f, -frag->extent[1] * 0.5f);
			nvgTransformMultiply(m1, m2);
			nvgTransformInverse(invxform, m1);
		}
		else {
			nvgTransformInverse(invxform, paint->xform);
		}
		frag->type = NSVG_SHADER_FILLIMG;

#if NANOVG_GL_USE_UNIFORMBUFFER
		if (tex->type == NVG_TEXTURE_RGBA)
			frag->texType = (tex->flags & NVG_IMAGE_PREMULTIPLIED) ? 0 : 1;
		else
			frag->texType = 2;
#else
		if (tex->type == NVG_TEXTURE_RGBA)
			frag->texType = (tex->flags & NVG_IMAGE_PREMULTIPLIED) ? 0.0f : 1.0f;
		else
			frag->texType = 2.0f;
#endif
		//		printf("frag->texType = %d\n", frag->texType);
	}
	else {
		frag->type = NSVG_SHADER_FILLGRAD;
		frag->radius = paint->radius;
		frag->feather = paint->feather;
		nvgTransformInverse(invxform, paint->xform);
	}

	glnvg__xformToMat3x4(frag->paintMat, invxform);

	return 1;
}

static void glnvg__setUniforms(GLNVGcontext* gl, int uniformOffset, int image)
{
	ln::detail::GraphicsCommandList* commandList = gl->commandList();
	ln::ShaderDescriptor* descriptor = commandList->acquireShaderDescriptor(gl->shader);

	// ConstantBuffer
	ln::detail::ConstantBufferView view = commandList->allocateUniformBuffer(sizeof(GLNVGfragUniforms));
	view.setData(gl->uniforms + uniformOffset, sizeof(GLNVGfragUniforms));
	descriptor->setUniformBuffer(0, view);

	// Texture
	GLNVGtexture* tex = NULL;
	if (image != 0) {
		tex = glnvg__findTexture(gl, image);
	}
	// If no image is set, use empty texture
	if (tex == NULL) {
		tex = glnvg__findTexture(gl, gl->dummyTex);
	}
	if (tex) {
		descriptor->setTexture(0, tex->tex);
	}

	gl->g->setShaderDescriptor(descriptor);
}

// params.renderViewport
static void glnvg__renderViewport(void* uptr, float width, float height, float devicePixelRatio)
{
	NVG_NOTUSED(devicePixelRatio);
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	gl->view[0] = width;
	gl->view[1] = height;
}

// 頂点バッファは呼び出し側で bind 済み
static void glnvg__fill(GLNVGcontext* gl, GLNVGcall* call)
{
	ln::GraphicsContext* g = gl->g;
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

	// Draw shapes
#if 0	// TODO:
#if 1
	glnvg__setUniforms(gl, call->uniformOffset, 0);

	ln::DepthStencilStateDesc desc0 = g->depthStencilState();
	ln::DepthStencilStateDesc desc1;
	desc1.stencilEnabled = true;
	//desc1.stencilReferenceValue = 0;
	desc1.frontFace.stencilFailOp = ln::StencilOp::Keep;
	desc1.frontFace.stencilDepthFailOp = ln::StencilOp::Keep;
	desc1.frontFace.stencilPassOp = ln::StencilOp::Replace;
	desc1.frontFace.stencilFunc = ln::ComparisonFunc::Always;
	desc1.backFace.stencilFailOp = ln::StencilOp::Keep;
	desc1.backFace.stencilDepthFailOp = ln::StencilOp::Keep;
	desc1.backFace.stencilPassOp = ln::StencilOp::Replace;
	desc1.backFace.stencilFunc = ln::ComparisonFunc::Always;
	g->setDepthStencilState(desc1);

	ln::RasterizerStateDesc rdesc0 = g->rasterizerState();
	ln::RasterizerStateDesc rdesc1;
	rdesc1.cullMode = ln::CullMode::None;
	g->setRasterizerState(rdesc1);


	for (i = 0; i < npaths; i++) {
		gl->g->setPrimitiveTopology(ln::PrimitiveTopology::TriangleFan);
		gl->g->drawPrimitive(paths[i].fillOffset, (paths[i].fillCount - 2));
	}

	g->setDepthStencilState(desc0);
	g->setRasterizerState(rdesc0);
#else
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	// set bindpoint for solid loc
	glnvg__setUniforms(gl, call->uniformOffset, 0);
	glnvg__checkError(gl, "fill simple");

	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
	glDisable(GL_CULL_FACE);
	for (i = 0; i < npaths; i++)
		glDrawArrays(GL_TRIANGLE_FAN, paths[i].fillOffset, paths[i].fillCount);
	glEnable(GL_CULL_FACE);

	// Draw anti-aliased pixels
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE)
#endif
#endif

	glnvg__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);

	if (gl->flags & NVG_ANTIALIAS) {
#if 0	// TODO:
		glnvg__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
#endif
		// Draw fringes
		for (i = 0; i < npaths; i++) {
			gl->g->setPrimitiveTopology(ln::PrimitiveTopology::TriangleStrip);
			gl->g->drawPrimitive(paths[i].strokeOffset, (paths[i].strokeCount - 2));
		}
	}

	// Draw fill
#if 0	// TODO:
	//glnvg__stencilFunc(gl, GL_NOTEQUAL, 0x0, 0xff);
	//glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
#endif
	gl->g->setPrimitiveTopology(ln::PrimitiveTopology::TriangleStrip);
	gl->g->drawPrimitive(call->triangleOffset, (call->triangleCount - 2));

#if 0	// TODO:
	glDisable(GL_STENCIL_TEST);
#endif
}

static void glnvg__convexFill(GLNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

	glnvg__setUniforms(gl, call->uniformOffset, call->image);

	for (i = 0; i < npaths; i++) {
		gl->g->setPrimitiveTopology(ln::PrimitiveTopology::TriangleFan);
		gl->g->drawPrimitive(paths[i].fillOffset, (paths[i].fillCount - 2));
		// Draw fringes
		if (paths[i].strokeCount > 0) {
			gl->g->setPrimitiveTopology(ln::PrimitiveTopology::TriangleStrip);
			gl->g->drawPrimitive(paths[i].strokeOffset, (paths[i].strokeCount - 2));
		}
	}
}

static void glnvg__stroke(GLNVGcontext* gl, GLNVGcall* call)
{
#if 1	// TODO:
	LN_NOTIMPLEMENTED();
#else
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int npaths = call->pathCount, i;

	if (gl->flags & NVG_STENCIL_STROKES) {

		glEnable(GL_STENCIL_TEST);
		glnvg__stencilMask(gl, 0xff);

		// Fill the stroke base without overlap
		glnvg__stencilFunc(gl, GL_EQUAL, 0x0, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
		glnvg__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);
		glnvg__checkError(gl, "stroke fill 0");
		for (i = 0; i < npaths; i++)
			glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);

		// Draw anti-aliased pixels.
		glnvg__setUniforms(gl, call->uniformOffset, call->image);
		glnvg__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		for (i = 0; i < npaths; i++)
			glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);

		// Clear stencil buffer.
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glnvg__stencilFunc(gl, GL_ALWAYS, 0x0, 0xff);
		glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
		glnvg__checkError(gl, "stroke fill 1");
		for (i = 0; i < npaths; i++)
			glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		glDisable(GL_STENCIL_TEST);

		//		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

	}
	else {
		glnvg__setUniforms(gl, call->uniformOffset, call->image);
		glnvg__checkError(gl, "stroke fill");
		// Draw Strokes
		for (i = 0; i < npaths; i++)
			glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
	}
#endif
}

static void glnvg__triangles(GLNVGcontext* gl, GLNVGcall* call)
{
	// テキスト描画機能を使うときに必要になる
	LN_UNREACHABLE();	// unused
}

// glnvg__renderCancel
static void glnvg__renderCancel(void* uptr) {
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

static ln::BlendFactor glnvg_convertBlendFuncFactor(int factor)
{
	if (factor == NVG_ZERO)
		return ln::BlendFactor::Zero;
	if (factor == NVG_ONE)
		return ln::BlendFactor::One;
	if (factor == NVG_SRC_COLOR)
		return ln::BlendFactor::SourceColor;
	if (factor == NVG_ONE_MINUS_SRC_COLOR)
		return ln::BlendFactor::InverseSourceColor;
	if (factor == NVG_DST_COLOR)
		return ln::BlendFactor::DestinationColor;
	if (factor == NVG_ONE_MINUS_DST_COLOR)
		return ln::BlendFactor::InverseDestinationColor;
	if (factor == NVG_SRC_ALPHA)
		return ln::BlendFactor::SourceAlpha;
	if (factor == NVG_ONE_MINUS_SRC_ALPHA)
		return ln::BlendFactor::InverseSourceAlpha;
	if (factor == NVG_DST_ALPHA)
		return ln::BlendFactor::DestinationAlpha;
	if (factor == NVG_ONE_MINUS_DST_ALPHA)
		return ln::BlendFactor::InverseDestinationAlpha;
	//if (factor == NVG_SRC_ALPHA_SATURATE)
	//	return GL_SRC_ALPHA_SATURATE;
	LN_NOTIMPLEMENTED();
	return ln::BlendFactor::SourceColor;
}

static GLNVGblend glnvg__blendCompositeOperation(NVGcompositeOperationState op)
{
	GLNVGblend blend;
	blend.srcRGB = glnvg_convertBlendFuncFactor(op.srcRGB);
	blend.dstRGB = glnvg_convertBlendFuncFactor(op.dstRGB);
	blend.srcAlpha = glnvg_convertBlendFuncFactor(op.srcAlpha);
	blend.dstAlpha = glnvg_convertBlendFuncFactor(op.dstAlpha);
	//if (blend.srcRGB == GL_INVALID_ENUM || blend.dstRGB == GL_INVALID_ENUM || blend.srcAlpha == GL_INVALID_ENUM || blend.dstAlpha == GL_INVALID_ENUM)
	//{
	//	blend.srcRGB = GL_ONE;
	//	blend.dstRGB = GL_ONE_MINUS_SRC_ALPHA;
	//	blend.srcAlpha = GL_ONE;
	//	blend.dstAlpha = GL_ONE_MINUS_SRC_ALPHA;
	//}
	return blend;
}

static void glnvg__renderFlush(void* uptr)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	int i;

	if (gl->ncalls > 0) {
	//if (0) {

		// Setup require GL state.
		gl->g->setShaderPass(gl->shader->techniques()[0]->passes()[0]);

		ln::BlendStateDesc desc1;
		desc1.independentBlendEnable = false;
		desc1.renderTargets[0].blendEnable = true;
		gl->g->setBlendState(desc1);

		ln::DepthStencilStateDesc desc2;
		desc2.depthTestFunc = ln::ComparisonFunc::Always;	// Z-Test 無効
		desc2.stencilEnabled = false;	// Stencil 無効
		gl->g->setDepthStencilState(desc2);

		ln::RasterizerStateDesc desc3;
		desc3.fillMode = ln::FillMode::Solid;
		desc3.cullMode = ln::CullMode::Back;	// TODO: 要チェック
		desc3.cullMode = ln::CullMode::None;
		gl->g->setRasterizerState(desc3);

		//gl->boundTexture = 0;
		//gl->stencilMask = 0xffffffff;
		//gl->stencilFunc = GL_ALWAYS;
		//gl->stencilFuncRef = 0;
		//gl->stencilFuncMask = 0xffffffff;
		gl->blendFunc.srcRGB = desc1.renderTargets[0].sourceBlend;
		gl->blendFunc.srcAlpha = desc1.renderTargets[0].sourceBlendAlpha;
		gl->blendFunc.dstRGB = desc1.renderTargets[0].destinationBlend;
		gl->blendFunc.dstAlpha = desc1.renderTargets[0].destinationBlendAlpha;

		//glDisable(GL_SCISSOR_TEST);
		//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		// Prepare VertexBuffer
		{
			size_t size = gl->nverts * sizeof(NVGvertex);
			if (!gl->vertexBuffer || gl->vertexBuffer->size() < size) {
				gl->vertexBuffer = ln::makeObject<ln::VertexBuffer>(size, ln::GraphicsResourceUsage::Dynamic);
			}
			void* data = gl->vertexBuffer->map(ln::MapMode::Write);
			memcpy(data, gl->verts, gl->nverts * sizeof(NVGvertex));
			gl->vertexBuffer->unmap();
		}

		// Bind VertexBuffer
		gl->g->setVertexBuffer(0, gl->vertexBuffer);
		gl->g->setVertexLayout(gl->vertexLayout);


		// Set view and texture just once per frame.
		//glUniform2fv(gl->shader.loc[GLNVG_LOC_VIEWSIZE], 1, gl->view);

		for (i = 0; i < gl->ncalls; i++) {
			GLNVGcall* call = &gl->calls[i];
			glnvg__blendFuncSeparate(gl, &call->blendFunc);
			if (call->type == GLNVG_FILL)
				glnvg__fill(gl, call);
			else if (call->type == GLNVG_CONVEXFILL)
				glnvg__convexFill(gl, call);
			else if (call->type == GLNVG_STROKE)
				glnvg__stroke(gl, call);
			else if (call->type == GLNVG_TRIANGLES)
				glnvg__triangles(gl, call);
		}

		// Cleanup
		gl->g->setVertexBuffer(0, nullptr);
		gl->g->setVertexLayout(nullptr);
		gl->g->setShaderDescriptor(nullptr);
		gl->g->setShaderPass(nullptr);
	}

	// Reset calls
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

static int glnvg__maxVertCount(const NVGpath* paths, int npaths)
{
	int i, count = 0;
	for (i = 0; i < npaths; i++) {
		count += paths[i].nfill;
		count += paths[i].nstroke;
	}
	return count;
}

static GLNVGcall* glnvg__allocCall(GLNVGcontext* gl)
{
	GLNVGcall* ret = NULL;
	if (gl->ncalls + 1 > gl->ccalls) {
		GLNVGcall* calls;
		int ccalls = glnvg__maxi(gl->ncalls + 1, 128) + gl->ccalls / 2; // 1.5x Overallocate
		calls = (GLNVGcall*)realloc(gl->calls, sizeof(GLNVGcall) * ccalls);
		if (calls == NULL) return NULL;
		gl->calls = calls;
		gl->ccalls = ccalls;
	}
	ret = &gl->calls[gl->ncalls++];
	memset(ret, 0, sizeof(GLNVGcall));
	return ret;
}

static int glnvg__allocPaths(GLNVGcontext* gl, int n)
{
	int ret = 0;
	if (gl->npaths + n > gl->cpaths) {
		GLNVGpath* paths;
		int cpaths = glnvg__maxi(gl->npaths + n, 128) + gl->cpaths / 2; // 1.5x Overallocate
		paths = (GLNVGpath*)realloc(gl->paths, sizeof(GLNVGpath) * cpaths);
		if (paths == NULL) return -1;
		gl->paths = paths;
		gl->cpaths = cpaths;
	}
	ret = gl->npaths;
	gl->npaths += n;
	return ret;
}

static int glnvg__allocVerts(GLNVGcontext* gl, int n)
{
	int ret = 0;
	if (gl->nverts + n > gl->cverts) {
		NVGvertex* verts;
		int cverts = glnvg__maxi(gl->nverts + n, 4096) + gl->cverts / 2; // 1.5x Overallocate
		verts = (NVGvertex*)realloc(gl->verts, sizeof(NVGvertex) * cverts);
		if (verts == NULL) return -1;
		gl->verts = verts;
		gl->cverts = cverts;
	}
	ret = gl->nverts;
	gl->nverts += n;
	return ret;
}

static int glnvg__allocFragUniforms(GLNVGcontext* gl, int n)
{
	int ret = 0, structSize = gl->fragSize;
	if (gl->nuniforms + n > gl->cuniforms) {
		unsigned char* uniforms;
		int cuniforms = glnvg__maxi(gl->nuniforms + n, 128) + gl->cuniforms / 2; // 1.5x Overallocate
		uniforms = (unsigned char*)realloc(gl->uniforms, structSize * cuniforms);
		if (uniforms == NULL) return -1;
		gl->uniforms = uniforms;
		gl->cuniforms = cuniforms;
	}
	ret = gl->nuniforms * structSize;
	gl->nuniforms += n;
	return ret;
}

static GLNVGfragUniforms* nvg__fragUniformPtr(GLNVGcontext* gl, int i)
{
	return (GLNVGfragUniforms*)&gl->uniforms[i];
}

static void glnvg__vset(NVGvertex* vtx, float x, float y, float u, float v)
{
	vtx->x = x;
	vtx->y = y;
	vtx->u = u;
	vtx->v = v;
}

static void glnvg__renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
	const float* bounds, const NVGpath* paths, int npaths)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	NVGvertex* quad;
	GLNVGfragUniforms* frag;
	int i, maxverts, offset;

	if (call == NULL) return;

	call->type = GLNVG_FILL;
	call->triangleCount = 4;
	call->pathOffset = glnvg__allocPaths(gl, npaths);
	if (call->pathOffset == -1) goto error;
	call->pathCount = npaths;
	call->image = paint->image;
	call->blendFunc = glnvg__blendCompositeOperation(compositeOperation);

	if (npaths == 1 && paths[0].convex)
	{
		call->type = GLNVG_CONVEXFILL;
		call->triangleCount = 0;	// Bounding box fill quad not needed for convex fill
	}

	// Allocate vertices for all the paths.
	maxverts = glnvg__maxVertCount(paths, npaths) + call->triangleCount;
	offset = glnvg__allocVerts(gl, maxverts);
	if (offset == -1) goto error;

	for (i = 0; i < npaths; i++) {
		GLNVGpath* copy = &gl->paths[call->pathOffset + i];
		const NVGpath* path = &paths[i];
		memset(copy, 0, sizeof(GLNVGpath));
		if (path->nfill > 0) {
			copy->fillOffset = offset;
			copy->fillCount = path->nfill;
			memcpy(&gl->verts[offset], path->fill, sizeof(NVGvertex) * path->nfill);
			offset += path->nfill;
		}
		if (path->nstroke > 0) {
			copy->strokeOffset = offset;
			copy->strokeCount = path->nstroke;
			memcpy(&gl->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
			offset += path->nstroke;
		}
	}

	// Setup uniforms for draw calls
	if (call->type == GLNVG_FILL) {
		// Quad
		call->triangleOffset = offset;
		quad = &gl->verts[call->triangleOffset];
		glnvg__vset(&quad[0], bounds[2], bounds[3], 0.5f, 1.0f);
		glnvg__vset(&quad[1], bounds[2], bounds[1], 0.5f, 1.0f);
		glnvg__vset(&quad[2], bounds[0], bounds[3], 0.5f, 1.0f);
		glnvg__vset(&quad[3], bounds[0], bounds[1], 0.5f, 1.0f);

		call->uniformOffset = glnvg__allocFragUniforms(gl, 2);
		if (call->uniformOffset == -1) goto error;
		// Simple shader for stencil
		frag = nvg__fragUniformPtr(gl, call->uniformOffset);
		memset(frag, 0, sizeof(*frag));
		frag->strokeThr = -1.0f;
		frag->type = NSVG_SHADER_SIMPLE;
		// Fill shader
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, fringe, fringe, -1.0f);
	}
	else {
		call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
		if (call->uniformOffset == -1) goto error;
		// Fill shader
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, fringe, fringe, -1.0f);
	}

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderStroke(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
	float strokeWidth, const NVGpath* paths, int npaths)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	int i, maxverts, offset;

	if (call == NULL) return;

	call->type = GLNVG_STROKE;
	call->pathOffset = glnvg__allocPaths(gl, npaths);
	if (call->pathOffset == -1) goto error;
	call->pathCount = npaths;
	call->image = paint->image;
	call->blendFunc = glnvg__blendCompositeOperation(compositeOperation);

	// Allocate vertices for all the paths.
	maxverts = glnvg__maxVertCount(paths, npaths);
	offset = glnvg__allocVerts(gl, maxverts);
	if (offset == -1) goto error;

	for (i = 0; i < npaths; i++) {
		GLNVGpath* copy = &gl->paths[call->pathOffset + i];
		const NVGpath* path = &paths[i];
		memset(copy, 0, sizeof(GLNVGpath));
		if (path->nstroke) {
			copy->strokeOffset = offset;
			copy->strokeCount = path->nstroke;
			memcpy(&gl->verts[offset], path->stroke, sizeof(NVGvertex) * path->nstroke);
			offset += path->nstroke;
		}
	}

	if (gl->flags & NVG_STENCIL_STROKES) {
		// Fill shader
		call->uniformOffset = glnvg__allocFragUniforms(gl, 2);
		if (call->uniformOffset == -1) goto error;

		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f / 255.0f);

	}
	else {
		// Fill shader
		call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
		if (call->uniformOffset == -1) goto error;
		glnvg__convertPaint(gl, nvg__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
	}

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderTriangles(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor,
	const NVGvertex* verts, int nverts, float fringe)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	GLNVGfragUniforms* frag;

	if (call == NULL) return;

	call->type = GLNVG_TRIANGLES;
	call->image = paint->image;
	call->blendFunc = glnvg__blendCompositeOperation(compositeOperation);

	// Allocate vertices for all the paths.
	call->triangleOffset = glnvg__allocVerts(gl, nverts);
	if (call->triangleOffset == -1) goto error;
	call->triangleCount = nverts;

	memcpy(&gl->verts[call->triangleOffset], verts, sizeof(NVGvertex) * nverts);

	// Fill shader
	call->uniformOffset = glnvg__allocFragUniforms(gl, 1);
	if (call->uniformOffset == -1) goto error;
	frag = nvg__fragUniformPtr(gl, call->uniformOffset);
	glnvg__convertPaint(gl, frag, paint, scissor, 1.0f, fringe, -1.0f);
	frag->type = NSVG_SHADER_IMG;

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

static void glnvg__renderDelete(void* uptr)
{
	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	int i;
	if (gl == NULL) return;

	gl->renderFeature = nullptr;
	gl->vertexLayout = nullptr;
	gl->vertexBuffer = nullptr;
	gl->shader = nullptr;
	gl->g = nullptr;

	//for (i = 0; i < gl->ntextures; i++) {
	//	if (gl->textures[i].tex != 0 && (gl->textures[i].flags & NVG_IMAGE_NODELETE) == 0)
	//		glDeleteTextures(1, &gl->textures[i].tex);
	//}
	free(gl->textures);

	free(gl->paths);
	free(gl->verts);
	free(gl->uniforms);
	free(gl->calls);

	free(gl);
}

NVGcontext* nvgCreateGL(int flags, ln::detail::PathRenderFeature* renderFeature, GLNVGcontext** glCtx)
{
	NVGparams params;
	NVGcontext* ctx = NULL;
	GLNVGcontext* gl = (GLNVGcontext*)malloc(sizeof(GLNVGcontext));
	if (gl == NULL) goto error;
	memset(gl, 0, sizeof(GLNVGcontext));
	gl->renderFeature = renderFeature;

	memset(&params, 0, sizeof(params));
	params.renderCreate = glnvg__renderCreate;
	params.renderCreateTexture = glnvg__renderCreateTexture;
	params.renderDeleteTexture = glnvg__renderDeleteTexture;
	params.renderUpdateTexture = glnvg__renderUpdateTexture;
	params.renderGetTextureSize = glnvg__renderGetTextureSize;
	params.renderViewport = glnvg__renderViewport;
	params.renderCancel = glnvg__renderCancel;
	params.renderFlush = glnvg__renderFlush;
	params.renderFill = glnvg__renderFill;
	params.renderStroke = glnvg__renderStroke;
	params.renderTriangles = glnvg__renderTriangles;
	params.renderDelete = glnvg__renderDelete;
	params.userPtr = gl;
	params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

	gl->flags = flags;

	ctx = nvgCreateInternal(&params);
	if (ctx == NULL) goto error;

	*glCtx = gl;

	return ctx;

error:
	// 'gl' is freed by nvgDeleteInternal.
	if (ctx != NULL) nvgDeleteInternal(ctx);
	return NULL;
}

void nvgDeleteGL(NVGcontext* ctx)
{
	nvgDeleteInternal(ctx);
}
#endif

namespace ln {
namespace detail {

//==============================================================================
// PathRenderFeature

PathRenderFeature::PathRenderFeature()
	: m_nvgContext(nullptr)
{
}

void PathRenderFeature::init(RenderingManager* manager)
{
	RenderFeature::init();
	m_manager = manager;
	m_nvgContext = nvgCreateGL(0/*NVG_ANTIALIAS*/, this, &m_glnvgContext);
}

void PathRenderFeature::onDispose(bool explicitDisposing)
{
	if (m_nvgContext) {
		nvgDeleteGL(m_nvgContext);
		m_nvgContext = nullptr;
	}
	RenderFeature::onDispose(explicitDisposing);
}

void PathRenderFeature::beginRendering()
{
	m_batchData.offset = 0;
}

RequestBatchResult PathRenderFeature::draw(detail::RenderFeatureBatchList* batchList, GraphicsContext* context)
{
	//submitBatch(context, batchList);
	//return RequestBatchResult::Submitted;
	return RequestBatchResult::Staging;
}

void PathRenderFeature::submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList)
{
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.offset = 0;
}

void PathRenderFeature::renderBatch(GraphicsContext* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);

	RenderPass* pass = context->renderPass();
	RenderTargetTexture* target = pass->renderTarget(0);

	m_glnvgContext->g = context;
	nvgBeginFrame(m_nvgContext, target->width(), target->height(), 1.0);	// TODO: DPI


	// Drop shadow
	float x = 100;
	float y = 50;
	float w = 300;
	float h = 200;
	NVGcontext* vg = m_nvgContext;
	float cornerRadius = 3.0f;
	NVGpaint shadowPaint;
	shadowPaint = nvgBoxGradient(vg, x, y + 2, w, h, cornerRadius * 2, 10, nvgRGBA(0, 0, 0, 128), nvgRGBA(0, 0, 0, 0));
	nvgBeginPath(vg);
	nvgRect(vg, x - 10, y - 10, w + 20, h + 30);
	nvgRoundedRect(vg, x, y, w, h, cornerRadius);
	nvgPathWinding(vg, NVG_HOLE);
	nvgFillPaint(vg, shadowPaint);
	nvgFill(vg);

	//nvgBeginPath(m_nvgContext);
	//nvgRect(m_nvgContext, 100, 100, 120, 30);
	//nvgFillColor(m_nvgContext, nvgRGBA(255, 192, 0, 255));
	//nvgFill(m_nvgContext);

	nvgEndFrame(m_nvgContext);

}

} // namespace detail
} // namespace ln

