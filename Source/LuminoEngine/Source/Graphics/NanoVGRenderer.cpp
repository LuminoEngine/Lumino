
#include "Internal.h"
#include <math.h>
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Graphics/GraphicsException.h>
#include "Device/GraphicsDriverInterface.h"
#include "GraphicsManager.h"
#include "RenderingCommand.h"
#include "NanoVGRenderer.h"

LN_NAMESPACE_BEGIN
namespace detail
{

class IndexStack
{
public:
	IndexStack()
		: m_indexStack()
		, m_capacity()
	{
	}

	void Reserved(int size)
	{
		m_indexStack.Clear();
		m_capacity = 0;
		Grow(size);
	}

	int Pop()
	{
		int top;
		m_indexStack.Pop(&top);
		return top;
	}

	bool IsEmpty() const
	{
		return m_indexStack.IsEmpty();
	}

	void Grow(int size)
	{
		for (int i = 0; i < size; i++)
		{
			m_indexStack.Push(m_capacity + i);
		}
		m_capacity += size;
	}

private:
	ln::Stack<int>	m_indexStack;
	int				m_capacity;
};

enum NVGcreateFlags
{
	// Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
	NVG_ANTIALIAS = 1 << 0,
	// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
	// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
	NVG_STENCIL_STROKES = 1 << 1,
	// Flag indicating that additional debug checks are done.
	NVG_DEBUG = 1 << 2,
};

Driver::ITexture* LNNVGcontext_GetTexture(LNNVGcontext* ctx, int imageId);

//==============================================================================
//
//==============================================================================
enum GLNVGshaderType {
	NSVG_SHADER_FILLGRAD,
	NSVG_SHADER_FILLIMG,
	NSVG_SHADER_SIMPLE,
	NSVG_SHADER_IMG
};

enum GLNVGcallType {
	GLNVG_NONE = 0,
	GLNVG_FILL,
	GLNVG_CONVEXFILL,
	GLNVG_STROKE,
	GLNVG_TRIANGLES,
};

struct GLNVGcall {
	int type;
	int image;
	int pathOffset;
	int pathCount;
	int triangleOffset;
	int triangleCount;
	int uniformOffset;		// このドローコールで使う UniformBuffer の場所
};
typedef struct GLNVGcall GLNVGcall;

struct GLNVGpath {
	int fillOffset;
	int fillCount;
	int strokeOffset;
	int strokeCount;
};
typedef struct GLNVGpath GLNVGpath;


struct GLNVGcontext
{
	float view[2];
	//int ntextures;
	//int ctextures;
	//int textureId;
	//GLuint vertBuf;
	int fragSize;	// アライメント済みの GLNVGfragUniforms サイズ
	int flags;

	GLNVGcall* calls;
	int ccalls;		// メモリ確保済み Call 数
	int ncalls;		// 使用中 Call 数

	GLNVGpath* paths;
	int cpaths;		// メモリ確保済み Path 数
	int npaths;		// 使用中 Paths 数

	struct NVGvertex* verts;
	int cverts;		// メモリ確保済み vertex 数
	int nverts;		// 使用中 vertex 数

//	// Per frame buffers
//	
	unsigned char* uniforms;
	int cuniforms;
	int nuniforms;
//
//	// cached state
//#if NANOVG_GL_USE_STATE_FILTER
//	GLuint boundTexture;
//	GLuint stencilMask;
//	GLenum stencilFunc;
//	GLint stencilFuncRef;
//	GLuint stencilFuncMask;
//#endif
};

struct GLNVGfragUniforms
{
	// note: after modifying layout or size of uniform array,
	// don't forget to also update the fragment shader source!
#define NANOVG_GL_UNIFORMARRAY_SIZE 11
	union
	{
		struct
		{
			float scissorMat[12]; // matrices are actually 3 vec4s
			float paintMat[12];
			struct NVGcolor innerCol;
			struct NVGcolor outerCol;
			float scissorExt[2];
			float scissorScale[2];
			float extent[2];
			float radius;
			float feather;
			float strokeMult;
			float strokeThr;
			float texType;
			float type;
		};
		float uniformArray[NANOVG_GL_UNIFORMARRAY_SIZE][4];
	};
};

static void glnvg__setUniforms(GLNVGcontext* gl, int uniformOffset, int image);

static int glnvg__maxi(int a, int b)
{
	return a > b ? a : b;
}

static void glnvg__checkError(GLNVGcontext* gl, const char* str)
{
	LN_LOG_WARNING(str);
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

static int glnvg__maxVertCount(const NVGpath* paths, int npaths)
{
	int i, count = 0;
	for (i = 0; i < npaths; i++) {
		count += paths[i].nfill;
		count += paths[i].nstroke;
	}
	return count;
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

static NVGcolor glnvg__premulColor(NVGcolor c)
{
	c.r *= c.a;
	c.g *= c.a;
	c.b *= c.a;
	return c;
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

static int glnvg__convertPaint(GLNVGcontext* gl, GLNVGfragUniforms* frag, NVGpaint* paint,
	NVGscissor* scissor, float width, float fringe, float strokeThr)
{
	float invxform[6];

	memset(frag, 0, sizeof(*frag));

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
	frag->strokeMult = (width*0.5f + fringe*0.5f) / fringe;
	frag->strokeThr = strokeThr;

	if (paint->image != 0)
	{
		// NVG_IMAGE_FLIPY などのフラグは無し。フォーマットも RGBA 固定。 
		frag->type = NSVG_SHADER_FILLIMG;
		frag->texType = 0;
		nvgTransformInverse(invxform, paint->xform);
	}
	else
	{
		frag->type = NSVG_SHADER_FILLGRAD;
		frag->radius = paint->radius;
		frag->feather = paint->feather;
		nvgTransformInverse(invxform, paint->xform);
	}

	glnvg__xformToMat3x4(frag->paintMat, invxform);

	return 1;
}


//==============================================================================
//
//==============================================================================
struct LNNVGcontext
	: public GLNVGcontext
{
	//int	flags = 0;
	//float view[2];

	GraphicsManager*			manager;
	RefPtr<Driver::IShader>		shader;
	Driver::IShaderPass*		shaderPass;
	Driver::IShaderVariable*	varViewSize;
	Driver::IShaderVariable*	varFrag;
	Driver::IShaderVariable*	varTex;

	


	//IndexStack			textureIndexStack;
	//List<TexturePtr>	textureList;
	List<Driver::ITexture*>	textureList;

	void Initialize()
	{
		GLNVGcontext* base = this;
		memset(base, 0, sizeof(GLNVGcontext));


		Driver::IGraphicsDevice* device = manager->GetGraphicsDevice();
		
		VertexElement elements[] =
		{
			{ 0, VertexElementType_Float2, VertexElementUsage_Position, 0 },
			{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
		};
		m_vertexDeclaration.Attach(device->CreateVertexDeclaration(elements, 2), false);
	}

	Driver::IRenderer* GetRenderer()
	{
		return manager->GetGraphicsDevice()->GetRenderer();
	}

	void ApplyVertexBuffer()
	{
		Driver::IGraphicsDevice* device = manager->GetGraphicsDevice();
		size_t bufferSize = nverts * sizeof(NVGvertex);
		if (m_vertexBuffer == nullptr || bufferSize > m_vertexBuffer->GetByteCount())
		{
			m_vertexBuffer.Attach(device->CreateVertexBuffer(bufferSize, verts, ResourceUsage::Dynamic), false);
		}
		else
		{
			m_vertexBuffer->SetSubData(0, verts, bufferSize);
		}

		Driver::IRenderer* renderer = device->GetRenderer();
		renderer->SetVertexDeclaration(m_vertexDeclaration);
		renderer->SetVertexBuffer(0, m_vertexBuffer);
	}

private:
	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclaration;
	RefPtr<Driver::IVertexBuffer>		m_vertexBuffer;
};

Driver::ITexture* LNNVGcontext_GetTexture(LNNVGcontext* ctx, int imageId)
{
	return ctx->textureList[imageId - 1];
}

//------------------------------------------------------------------------------
static void glnvg__setUniforms(GLNVGcontext* gl, int uniformOffset, int image)
{
	LNNVGcontext* lnc = (LNNVGcontext*)gl;
#if NANOVG_GL_USE_UNIFORMBUFFER
	glBindBufferRange(GL_UNIFORM_BUFFER, GLNVG_FRAG_BINDING, gl->fragBuf, uniformOffset, sizeof(GLNVGfragUniforms));
#else
	GLNVGfragUniforms* frag = nvg__fragUniformPtr(gl, uniformOffset);
	lnc->varFrag->SetVectorArray((Vector4*)&(frag->uniformArray[0][0]), NANOVG_GL_UNIFORMARRAY_SIZE);

	//glUniform4fv(gl->shader.loc[GLNVG_LOC_FRAG], NANOVG_GL_UNIFORMARRAY_SIZE, &(frag->uniformArray[0][0]));
#endif

	lnc->varViewSize->SetVector(Vector4(lnc->view[0], lnc->view[1], 0, 0));
	if (image != 0)
		lnc->varTex->SetTexture(LNNVGcontext_GetTexture(lnc, image));
	else
		lnc->varTex->SetTexture(nullptr);
}

static void glnvg__fill(LNNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

#if 0
	{
		// Draw shapes
		glEnable(GL_STENCIL_TEST);
		glnvg__stencilMask(gl, 0xff);
		glnvg__stencilFunc(gl, GL_ALWAYS, 0, 0xff);
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
	}
#endif

	// Draw anti-aliased pixels
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glnvg__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);
	glnvg__checkError(gl, "fill fill");

	Driver::IRenderer* renderer = gl->GetRenderer();
	renderer->SetShaderPass(gl->shaderPass);

	if (gl->flags & NVG_ANTIALIAS) {
		//glnvg__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		// Draw fringes
		for (i = 0; i < npaths; i++)
			renderer->DrawPrimitive(PrimitiveType_TriangleStrip, paths[i].strokeOffset, paths[i].strokeCount - 2);
	}

	// Draw fill
	//glnvg__stencilFunc(gl, GL_NOTEQUAL, 0x0, 0xff);	// ステンシルへの書き込み禁止
	//glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
	renderer->DrawPrimitive(PrimitiveType_TriangleList, call->triangleOffset, call->triangleCount / 3);

	//glDisable(GL_STENCIL_TEST);
}

static void glnvg__convexFill(LNNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int i, npaths = call->pathCount;

	glnvg__setUniforms(gl, call->uniformOffset, call->image);
	glnvg__checkError(gl, "convex fill");

	Driver::IRenderer* renderer = gl->GetRenderer();
	renderer->SetShaderPass(gl->shaderPass);

	for (i = 0; i < npaths; i++)
		renderer->DrawPrimitive(PrimitiveType_TriangleFan, paths[i].fillOffset, paths[i].fillCount - 2);

	if (gl->flags & NVG_ANTIALIAS) {
		// Draw fringes
		for (i = 0; i < npaths; i++)
			renderer->DrawPrimitive(PrimitiveType_TriangleStrip, paths[i].strokeOffset, paths[i].strokeCount - 2);
	}
}

static void glnvg__stroke(LNNVGcontext* gl, GLNVGcall* call)
{
	GLNVGpath* paths = &gl->paths[call->pathOffset];
	int npaths = call->pathCount, i;

	Driver::IRenderer* renderer = gl->GetRenderer();
	renderer->SetShaderPass(gl->shaderPass);

	if (gl->flags & NVG_STENCIL_STROKES) {
#if 1
		assert(0);
#else
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
#endif
	}
	else {
		glnvg__setUniforms(gl, call->uniformOffset, call->image);
		glnvg__checkError(gl, "stroke fill");
		// Draw Strokes
		for (i = 0; i < npaths; i++)
			renderer->DrawPrimitive(PrimitiveType_TriangleStrip, paths[i].strokeOffset, paths[i].strokeCount - 2);
	}
}

//------------------------------------------------------------------------------
static int lnnvg__renderCreate(void* uptr)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
	Driver::IGraphicsDevice* device = lnc->manager->GetGraphicsDevice();

	// シェーダ
	static const byte_t codeData[] =
	{
#include "Resource/NanoVGShader.fx.h"
	};
	static const size_t codeLen = LN_ARRAY_SIZE_OF(codeData);
	StringA code = lnc->manager->GetCommonShaderHeader();
	code.Append((const char*)codeData, codeLen);

	ShaderCompileResult result;
	lnc->shader.Attach(device->CreateShader(code.c_str(), code.GetLength(), &result), false);
	LN_THROW(result.Level != ShaderCompileResultLevel_Error, CompilationException, result);
	lnc->shaderPass = lnc->shader->GetTechnique(0)->GetPass(0);
	lnc->varViewSize = lnc->shader->GetVariableByName(_T("viewSize"));
	lnc->varFrag = lnc->shader->GetVariableByName(_T("frag"));
	lnc->varTex = lnc->shader->GetVariableByName(_T("tex"));

	int align = 4;
	lnc->fragSize = sizeof(GLNVGfragUniforms) + align - sizeof(GLNVGfragUniforms) % align;

	return 1;
}

//------------------------------------------------------------------------------
static int lnnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
{
	// ITexture → NanoVGImage の対応付けは lnnvg__AddImageTexture で行っている。
	// このドライバ関数が呼び出されるのは、NanoVG のテキスト描画機能からのみで、
	// いまは NanoVG 初期化時にしか使われない。ここで作られたイメージも使用されることはない。
	return INT_MAX;
}

//------------------------------------------------------------------------------
static int lnnvg__renderDeleteTexture(void* uptr, int image)
{
	// 削除はここでする必要はない
	return 1;	// 成功扱い
}

//------------------------------------------------------------------------------
static int lnnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
{
	// テキスト描画機能を使うときに必要になる
	assert(0);
	return 0;
}

//------------------------------------------------------------------------------
static int lnnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h)
{
	// テキスト描画機能を使うときに必要になる
	assert(0);
	return 0;
}

//------------------------------------------------------------------------------
// レンダリング開始時に呼び出される
static void lnnvg__renderViewport(void* uptr, int width, int height, float devicePixelRatio)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
	lnc->view[0] = (float)width;
	lnc->view[1] = (float)height;
}

//------------------------------------------------------------------------------
static void lnnvg__renderCancel(void* uptr)
{
	// nvgCancelFrame を使う予定はない
	assert(0);
}

//------------------------------------------------------------------------------
static void lnnvg__renderFlush(void* uptr, NVGcompositeOperationState compositeOperation)
{
	LNNVGcontext* gl = (LNNVGcontext*)uptr;
	int i;

	if (gl->ncalls > 0) {
#if 0
		// Setup require GL state.
		glUseProgram(gl->shader.prog);

		glnvg__blendCompositeOperation(compositeOperation);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_SCISSOR_TEST);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilMask(0xffffffff);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
#if NANOVG_GL_USE_STATE_FILTER
		gl->boundTexture = 0;
		gl->stencilMask = 0xffffffff;
		gl->stencilFunc = GL_ALWAYS;
		gl->stencilFuncRef = 0;
		gl->stencilFuncMask = 0xffffffff;
#endif

#if NANOVG_GL_USE_UNIFORMBUFFER
		// Upload ubo for frag shaders
		glBindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
		glBufferData(GL_UNIFORM_BUFFER, gl->nuniforms * gl->fragSize, gl->uniforms, GL_STREAM_DRAW);
#endif

		// Upload vertex data
#if defined NANOVG_GL3
		glBindVertexArray(gl->vertArr);
#endif
		glBindBuffer(GL_ARRAY_BUFFER, gl->vertBuf);
		glBufferData(GL_ARRAY_BUFFER, gl->nverts * sizeof(NVGvertex), gl->verts, GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(NVGvertex), (const GLvoid*)(size_t)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(NVGvertex), (const GLvoid*)(0 + 2 * sizeof(float)));

		// Set view and texture just once per frame.
		glUniform1i(gl->shader.loc[GLNVG_LOC_TEX], 0);
		glUniform2fv(gl->shader.loc[GLNVG_LOC_VIEWSIZE], 1, gl->view);

#if NANOVG_GL_USE_UNIFORMBUFFER
		glBindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
#endif
#endif
		gl->ApplyVertexBuffer();

		for (i = 0; i < gl->ncalls; i++) {
			GLNVGcall* call = &gl->calls[i];
			if (call->type == GLNVG_FILL)
				glnvg__fill(gl, call);
			else if (call->type == GLNVG_CONVEXFILL)
				glnvg__convexFill(gl, call);
			else if (call->type == GLNVG_STROKE)
				glnvg__stroke(gl, call);
			else if (call->type == GLNVG_TRIANGLES)
			{
				// 今は使っていない
				assert(0);
				//glnvg__triangles(gl, call);
			}
		}

#if 0
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
#if defined NANOVG_GL3
		glBindVertexArray(0);
#endif
		glDisable(GL_CULL_FACE);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(0);
		glnvg__bindTexture(gl, 0);
#endif
	}

	// Reset calls
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

//------------------------------------------------------------------------------
// bounds	: bounds[4]. 
static void lnnvg__renderFill(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
	const float* bounds, const NVGpath* paths, int npaths)
{
	LNNVGcontext* gl = (LNNVGcontext*)uptr;
	GLNVGcall* call = glnvg__allocCall(gl);
	NVGvertex* quad;
	GLNVGfragUniforms* frag;
	int i, maxverts, offset;

	if (call == NULL) return;

	call->type = GLNVG_FILL;
	call->pathOffset = glnvg__allocPaths(gl, npaths);
	if (call->pathOffset == -1) goto error;
	call->pathCount = npaths;
	call->image = paint->image;

	if (npaths == 1 && paths[0].convex)
		call->type = GLNVG_CONVEXFILL;

	// Allocate vertices for all the paths.
	maxverts = glnvg__maxVertCount(paths, npaths) + 6;
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

	// Quad
	call->triangleOffset = offset;
	call->triangleCount = 6;
	quad = &gl->verts[call->triangleOffset];
	glnvg__vset(&quad[0], bounds[0], bounds[3], 0.5f, 1.0f);
	glnvg__vset(&quad[1], bounds[2], bounds[3], 0.5f, 1.0f);
	glnvg__vset(&quad[2], bounds[2], bounds[1], 0.5f, 1.0f);

	glnvg__vset(&quad[3], bounds[0], bounds[3], 0.5f, 1.0f);
	glnvg__vset(&quad[4], bounds[2], bounds[1], 0.5f, 1.0f);
	glnvg__vset(&quad[5], bounds[0], bounds[1], 0.5f, 1.0f);

	// Setup uniforms for draw calls
	if (call->type == GLNVG_FILL) {
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
	
	// NanoVG 用のシェーダを使いつつ、テクスチャ転送元の左上が、ジオメトリの境界矩形の左上になるようにしたい。
	// サンプルのままだと絶対座標での左上 (0, 0) がテクスチャ転送元の原点になってしまう。
	GLNVGfragUniforms* flags = nvg__fragUniformPtr(gl, call->uniformOffset);
	flags->paintMat[8] = -bounds[0];
	flags->paintMat[9] = -bounds[1];

	return;

error:
	// We get here if call alloc was ok, but something else is not.
	// Roll back the last call to prevent drawing it.
	if (gl->ncalls > 0) gl->ncalls--;
}

//------------------------------------------------------------------------------
static void lnnvg__renderStroke(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
	float strokeWidth, const NVGpath* paths, int npaths)
{
	assert(0);
}

//------------------------------------------------------------------------------
static void lnnvg__renderTriangles(void* uptr, NVGpaint* paint, NVGscissor* scissor,
	const NVGvertex* verts, int nverts)
{
	// テキスト描画機能を使うときに必要になる
	assert(0);
}

//------------------------------------------------------------------------------
static void lnnvg__renderDelete(void* uptr)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;

	free(lnc->paths);
	free(lnc->verts);
	free(lnc->uniforms);
	free(lnc->calls);

	LN_SAFE_DELETE(lnc);
}

//------------------------------------------------------------------------------
static int lnnvg__AddImageTexture(NVGcontext* ctx, Driver::ITexture* texture)
{
	LNNVGcontext* lnc = (LNNVGcontext*)nvgInternalParams(ctx)->userPtr;
	lnc->textureList.Add(texture);
	return lnc->textureList.GetCount();
}

//------------------------------------------------------------------------------
NVGcontext* nvgCreateLNContext(GraphicsManager* manager, int flags)
{
	LNNVGcontext* lnc = LN_NEW LNNVGcontext();
	lnc->manager = manager;
	lnc->Initialize();

	NVGparams params;
	memset(&params, 0, sizeof(params));
	params.renderCreate = lnnvg__renderCreate;
	params.renderCreateTexture = lnnvg__renderCreateTexture;
	params.renderDeleteTexture = lnnvg__renderDeleteTexture;
	params.renderUpdateTexture = lnnvg__renderUpdateTexture;
	params.renderGetTextureSize = lnnvg__renderGetTextureSize;
	params.renderViewport = lnnvg__renderViewport;
	params.renderCancel = lnnvg__renderCancel; 
	params.renderFlush = lnnvg__renderFlush;
	params.renderFill = lnnvg__renderFill;
	params.renderStroke = lnnvg__renderStroke;
	params.renderTriangles = lnnvg__renderTriangles;
	params.renderDelete = lnnvg__renderDelete;
	params.userPtr = lnc;
	params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

	lnc->flags = flags;

	NVGcontext* ctx = nvgCreateInternal(&params);
	if (ctx == NULL) goto error;

	return ctx;

error:
	if (ctx != NULL) nvgDeleteInternal(ctx);
	return NULL;
}

//------------------------------------------------------------------------------
void nvgDeleteLNContext(NVGcontext* ctx)
{
	nvgDeleteInternal(ctx);
}

//==============================================================================
// NanoVGCommandListCache
//==============================================================================

//------------------------------------------------------------------------------
RefPtr<NanoVGCommandList> NanoVGCommandListCache::CreateObject()
{
	return RefPtr<NanoVGCommandList>::MakeRef();
}


//==============================================================================
// NanoVGRenderer
//==============================================================================
//------------------------------------------------------------------------------
NanoVGRenderer::NanoVGRenderer()
	: m_manager(nullptr)
	, m_nvgContext(nullptr)
{
}

//------------------------------------------------------------------------------
NanoVGRenderer::~NanoVGRenderer()
{
	nvgDeleteLNContext(m_nvgContext);
}

//------------------------------------------------------------------------------
void NanoVGRenderer::Initialize(GraphicsManager* manager)
{
	m_manager = manager;
	m_nvgContext = nvgCreateLNContext(m_manager, NVG_ANTIALIAS/* | NVG_STENCIL_STROKES*/);
}

//------------------------------------------------------------------------------
void NanoVGRenderer::ExecuteCommand(NanoVGCommandList* commandList)
{
	LN_CHECK_ARG(commandList != nullptr);

	NanoVGRenderer* _this = this;
	LN_ENQUEUE_RENDER_COMMAND_3(
		ExecuteCommand, m_manager,
		NanoVGRenderer*, _this,
		NanoVGState, m_state,
		RefPtr<NanoVGCommandList>, commandList,
		{
			_this->ExecuteCommandInternal(m_state, commandList);
		});
}
//------------------------------------------------------------------------------
void NanoVGRenderer::OnSetState(const DrawElementBatch* state)
{
	NanoVGCommandHelper::ExpandState(state->GetTransfrom(), state->state.GetBrush(), state->state.GetPen(), &m_state);
}

//------------------------------------------------------------------------------
void NanoVGRenderer::ExecuteCommandInternal(const NanoVGState& state, NanoVGCommandList* commandList)
{
	Driver::IRenderer* renderer = m_manager->GetGraphicsDevice()->GetRenderer();
	const SizeI& size = renderer->GetRenderTarget(0)->GetSize();

	// 描画開始前にテクスチャテーブルをクリア
	LNNVGcontext* lnc = (LNNVGcontext*)nvgInternalParams(m_nvgContext)->userPtr;
	lnc->textureList.Clear();

	// 描画実行
	float pxRatio = 1.0f;
	nvgBeginFrame(m_nvgContext, size.width, size.height, pxRatio);
	NanoVGCommandHelper::ApplyState(m_nvgContext, &state);
	NanoVGCommandHelper::ExecuteCommand(commandList, m_nvgContext);
	nvgEndFrame(m_nvgContext);

	// コマンドリストを使い終わった。返却する
	PushCommandList(commandList);
}

//------------------------------------------------------------------------------
void NanoVGRenderer::PushCommandList(NanoVGCommandList* commandList)
{
	commandList->Clear();

	m_manager->GetNanoVGCommandListCache()->ReleaseCommandList(commandList);
}

//==============================================================================
// NanoVGCommandHelper
//==============================================================================
void NanoVGCommandHelper::nvgBeginPath(NanoVGCommandList* ctx)
{
	float cmd[] = { (float)Cmd_nvgBeginPath };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgMoveTo(NanoVGCommandList* ctx, float x, float y)
{
	float cmd[] = { (float)Cmd_nvgMoveTo, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgLineTo(NanoVGCommandList* ctx, float x, float y)
{
	float cmd[] = { (float)Cmd_nvgLineTo, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgBezierTo(NanoVGCommandList* ctx, float c1x, float c1y, float c2x, float c2y, float x, float y)
{
	float cmd[] = { (float)Cmd_nvgBezierTo, c1x, c1y, c2x, c2y, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgQuadTo(NanoVGCommandList* ctx, float cx, float cy, float x, float y)
{
	float cmd[] = { (float)Cmd_nvgQuadTo, cx, cy, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgArcTo(NanoVGCommandList* ctx, float x1, float y1, float x2, float y2, float radius)
{
	float cmd[] = { (float)Cmd_nvgArcTo, x1, y1, x2, y2, radius };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgClosePath(NanoVGCommandList* ctx)
{
	float cmd[] = { (float)Cmd_nvgClosePath };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgPathWinding(NanoVGCommandList* ctx, int dir)
{
	float cmd[] = { (float)Cmd_nvgPathWinding,(float)dir };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgArc(NanoVGCommandList* ctx, float cx, float cy, float r, float a0, float a1, int dir)
{
	float cmd[] = { (float)Cmd_nvgArc, cx, cy, r, a0, a1, (float)dir };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgRect(NanoVGCommandList* ctx, float x, float y, float w, float h)
{
	float cmd[] = { (float)Cmd_nvgRect, x, y, w, h };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgRoundedRect(NanoVGCommandList* ctx, float x, float y, float w, float h, float r)
{
	float cmd[] = { (float)Cmd_nvgRoundedRect, x, y, w, h, r };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgRoundedRectVarying(NanoVGCommandList* ctx, float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft)
{
	float cmd[] = { (float)Cmd_nvgRoundedRectVarying, x, y, w, h, radTopLeft, radTopRight, radBottomRight, radBottomLeft };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgEllipse(NanoVGCommandList* ctx, float cx, float cy, float rx, float ry)
{
	float cmd[] = { (float)Cmd_nvgEllipse, cx, cy, rx, ry };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgCircle(NanoVGCommandList* ctx, float cx, float cy, float r)
{
	float cmd[] = { (float)Cmd_nvgCircle, cx, cy, r };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgFill(NanoVGCommandList* ctx)
{
	float cmd[] = { (float)Cmd_nvgFill };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgStroke(NanoVGCommandList* ctx)
{
	float cmd[] = { (float)Cmd_nvgStroke };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::ExecuteCommand(NanoVGCommandList* commandList, NVGcontext* ctx)
{
	NVGparams* param = nvgInternalParams(ctx);
	param->edgeAntiAlias = 0;

	int count = commandList->GetDataCount();
	for (int i = 0; i < count; i++)
	{
		float* cmd = (float*)commandList->GetDataByIndex(i);
		switch ((int)cmd[0])
		{
			case Cmd_nvgBeginPath:
				::nvgBeginPath(ctx);
				break;
			case Cmd_nvgMoveTo:
				param->edgeAntiAlias = 1;
				::nvgMoveTo(ctx, cmd[1], cmd[2]);
				break;
			case Cmd_nvgLineTo:
				param->edgeAntiAlias = 1;
				::nvgLineTo(ctx, cmd[1], cmd[2]);
				break;
			case Cmd_nvgBezierTo:
				param->edgeAntiAlias = 1;
				::nvgBezierTo(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6]);
				break;
			case Cmd_nvgQuadTo:
				param->edgeAntiAlias = 1;
				::nvgQuadTo(ctx, cmd[1], cmd[2], cmd[3], cmd[4]);
				break;
			case Cmd_nvgArcTo:
				param->edgeAntiAlias = 1;
				::nvgArcTo(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
				break;
			case Cmd_nvgClosePath:
				::nvgClosePath(ctx);
				break;
			case Cmd_nvgPathWinding:
				::nvgPathWinding(ctx, cmd[1]);
				break;
			case Cmd_nvgArc:
				param->edgeAntiAlias = 1;
				::nvgArc(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6]);
				break;
			case Cmd_nvgRect:
				::nvgRect(ctx, cmd[1], cmd[2], cmd[3], cmd[4]);
				break;
			case Cmd_nvgRoundedRect:
				param->edgeAntiAlias = 1;
				::nvgRoundedRect(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
				break;
			case Cmd_nvgRoundedRectVarying:
				param->edgeAntiAlias = 1;
				::nvgRoundedRectVarying(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6], cmd[7], cmd[8]);
				break;
			case Cmd_nvgEllipse:
				param->edgeAntiAlias = 1;
				::nvgEllipse(ctx, cmd[1], cmd[2], cmd[3], cmd[4]);
				break;
			case Cmd_nvgCircle:
				param->edgeAntiAlias = 1;
				::nvgCircle(ctx, cmd[1], cmd[2], cmd[3]);
				break;
			case Cmd_nvgFill:
				::nvgFill(ctx);
				break;
			case Cmd_nvgStroke:
				::nvgStroke(ctx);
				break;
			default:
				assert(0);
				break;
		}
	}
}
void NanoVGCommandHelper::ExpandState(const Matrix& transform, Brush* brush, Pen* pen, NanoVGState* outState)
{
	outState->transform = transform;
	ExpandBrushState(brush, &outState->fillBrush);
	if (pen != nullptr)
	{
		ExpandBrushState(pen->GetBrush(), &outState->strokeBrush);
		outState->strokeWidth = pen->GetThickness();
		// TODO:
		outState->miterLimit = 10.0f;
		outState->lineCap = NVG_BUTT;
		outState->lineJoin = NVG_MITER;
		outState->globalAlpha = 1.0f;
	}
	else
	{
		ExpandBrushState(nullptr, &outState->strokeBrush);
		outState->strokeWidth = 0;
		outState->miterLimit = 10.0f;
		outState->lineCap = NVG_BUTT;
		outState->lineJoin = NVG_MITER;
		outState->globalAlpha = 1.0f;
	}
}
void NanoVGCommandHelper::ExpandBrushState(Brush* brush, NanoVGBrush* outBrush)
{
	if (brush != nullptr)
	{
		if (brush->IsSolidColor())
		{
			Brush* b = brush;
			outBrush->type = NanoVGBrushType::SolidColor;
			outBrush->SolidColorInfo.color.r = brush->GetColor().r;
			outBrush->SolidColorInfo.color.g = brush->GetColor().g;
			outBrush->SolidColorInfo.color.b = brush->GetColor().b;
			outBrush->SolidColorInfo.color.a = brush->GetColor().a;
		}
		else
		{
			RectF rc = static_cast<TextureBrush*>(brush)->GetActualSourceRect();
			outBrush->type = NanoVGBrushType::ImagePattern;
			outBrush->ImagePatternInfo.ox = rc.x;
			outBrush->ImagePatternInfo.oy = rc.y;
			outBrush->ImagePatternInfo.ex = rc.width;
			outBrush->ImagePatternInfo.ey = rc.height;
			outBrush->ImagePatternInfo.angle = 0;
			outBrush->ImagePatternInfo.alpha = 1.0f;
			outBrush->imagePatternTexture = brush->GetTexture()->ResolveDeviceObject();
		}
	}
	else
	{
		outBrush->type = NanoVGBrushType::SolidColor;
		outBrush->SolidColorInfo.color.r = 0;
		outBrush->SolidColorInfo.color.g = 0;
		outBrush->SolidColorInfo.color.b = 0;
		outBrush->SolidColorInfo.color.a = 1;
	}
}
void NanoVGCommandHelper::ApplyState(NVGcontext* ctx, const NanoVGState* state)
{
	// fill
	if (state->fillBrush.type == NanoVGBrushType::SolidColor)
		nvgFillColor(ctx, state->fillBrush.SolidColorInfo.color);
	else
		nvgFillPaint(ctx, GetNVGpaint(ctx, state->fillBrush));

	// stroke
	if (state->strokeBrush.type == NanoVGBrushType::SolidColor)
		nvgStrokeColor(ctx, state->strokeBrush.SolidColorInfo.color);
	else
		nvgStrokePaint(ctx, GetNVGpaint(ctx, state->strokeBrush));

	// stroke
	nvgStrokeWidth(ctx, state->strokeWidth);
	nvgMiterLimit(ctx, state->miterLimit);
	nvgLineCap(ctx, state->lineCap);
	nvgLineJoin(ctx, state->lineJoin);
	nvgGlobalAlpha(ctx, state->globalAlpha);

	// transform
	const Matrix& m = state->transform;
	nvgTransform(ctx, m.m11, m.m21, m.m12, m.m22, m.m41, m.m42);
}
NVGpaint NanoVGCommandHelper::GetNVGpaint(NVGcontext* ctx, const NanoVGBrush& brush)
{
	switch (brush.type)
	{
		case NanoVGBrushType::LinearGradient:
		{
			auto& info = brush.LinearGradientInfo;
			return nvgLinearGradient(ctx, info.sx, info.sy, info.ex, info.ey, info.icol, info.ocol);
		}
		case NanoVGBrushType::BoxGradient:
		{
			auto& info = brush.BoxGradientInfo;
			return nvgBoxGradient(ctx, info.x, info.y, info.w, info.h, info.r, info.f, info.icol, info.ocol);
		}
		case NanoVGBrushType::RadialGradient:
		{
			auto& info = brush.RadialGradientInfo;
			return nvgRadialGradient(ctx, info.cx, info.cy, info.inr, info.outr, info.icol, info.ocol);
		}
		case NanoVGBrushType::ImagePattern:
		{
			auto& info = brush.ImagePatternInfo;
			return nvgImagePattern(ctx, info.ox, info.oy, info.ex, info.ey, 0.0f, lnnvg__AddImageTexture(ctx, brush.imagePatternTexture), info.alpha);
		}
	}
	assert(0);
	return NVGpaint();
}

} // namespace detail
LN_NAMESPACE_END

