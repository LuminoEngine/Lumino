
#include "Internal.hpp"
#include <LuminoEngine/GraphicsRHI/GraphicsDeviceContext.hpp>
#include <LuminoEngine/Graphics/detail/GraphicsManager.hpp>
#include <LuminoEngine/GPU/VertexLayout.hpp>
#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/GPU/IndexBuffer.hpp>
#include <LuminoEngine/GPU/RenderPass.hpp>
#include <LuminoEngine/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/GPU/ShaderDescriptor.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "PathRenderFeature.hpp"

// [2020/11/17] Shader を使うことになるので、Descripter の対応 (Material ごとに UniformBuffer を持てるようにする) が済んでからにする。
#if 1

#include <LuminoEngine/Rendering/detail/NanoVGHelper.hpp>

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


struct GLNVGcontext : public GLNVGcontextBase {
	ln::detail::PathRenderFeature* renderFeature;
	ln::Ref<ln::VertexLayout> vertexLayout;
	ln::Ref<ln::VertexBuffer> vertexBuffer;
	ln::Shader* shader;
	ln::GraphicsCommandList* g = nullptr;
	ln::GraphicsCommandList* commandList() const { return g; }	// TODO: 不要
};


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

// params.renderCreate
static int glnvg__renderCreate(void* uptr)
{
	// NanoVG の内部初期化後に呼び出される処理。
	// OpenGL サンプルでは OpenGL リソースの初期化を行っている。

	GLNVGcontext* gl = (GLNVGcontext*)uptr;
	gl->shader = gl->renderFeature->manager()->builtinShader(ln::detail::BuiltinShader::NanoVG);

	gl->vertexLayout = ln::makeObject_deprecated<ln::VertexLayout>();
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
	// RHIResource(Texture) → NanoVGImage の対応付けは lnnvg__AddImageTexture で行っている。
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

static void glnvg__setUniforms(GLNVGcontext* gl, int uniformOffset, int image)
{
	ln::GraphicsCommandList* commandList = gl->commandList();
	ln::detail::ShaderSecondaryDescriptor* descriptor = commandList->acquireShaderDescriptor(gl->shader);

	// ConstantBuffer
	ln::detail::ConstantBufferView view = commandList->allocateUniformBuffer(sizeof(GLNVGfragUniforms));
	view.setData(gl->uniforms + uniformOffset, sizeof(GLNVGfragUniforms));
	descriptor->setUniformBuffer(0, view);

	// Texture
	GLNVGtexture* tex = NULL;
	if (image != 0) {
		tex = GLNVGcontextBase::findTexture(gl, image);
	}
	// If no image is set, use empty texture
	if (tex == NULL) {
		tex = GLNVGcontextBase::findTexture(gl, gl->dummyTex);
	}
	if (tex) {
		descriptor->setTexture(0, tex->tex);
	}

	gl->g->setShaderDescriptor_deprecated(descriptor);
}

// 頂点バッファは呼び出し側で bind 済み
static void glnvg__fill(GLNVGcontext* gl, GLNVGcall* call)
{
	ln::GraphicsCommandList* g = gl->g;
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
				gl->vertexBuffer = ln::makeObject_deprecated<ln::VertexBuffer>(size, ln::GraphicsResourceUsage::Dynamic);
			}
			void* data = gl->vertexBuffer->writableData();
			memcpy(data, gl->verts, gl->nverts * sizeof(NVGvertex));
			//gl->vertexBuffer->unmap();
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
		gl->g->setShaderDescriptor_deprecated(nullptr);
		gl->g->setShaderPass(nullptr);
	}

	// Reset calls
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
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

	gl->dispose();

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

	gl->makeNVGparams(&params);
	params.renderCreate = glnvg__renderCreate;
	params.renderCreateTexture = glnvg__renderCreateTexture;
	params.renderDeleteTexture = glnvg__renderDeleteTexture;
	params.renderUpdateTexture = glnvg__renderUpdateTexture;
	params.renderGetTextureSize = glnvg__renderGetTextureSize;
	params.renderFlush = glnvg__renderFlush;
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

RequestBatchResult PathRenderFeature::draw(RenderFeatureBatchList* batchList, const RLIBatchState& batchState, GraphicsCommandList* context)
{
	LN_NOTIMPLEMENTED();
	//submitBatch(context, batchList);
	//return RequestBatchResult::Submitted;
	return RequestBatchResult::Staging;
}

void PathRenderFeature::submitBatch(GraphicsCommandList* context, RenderFeatureBatchList* batchList)
{
#ifdef LN_RLI_BATCH
	LN_UNREACHABLE();
#else
	auto batch = batchList->addNewBatch<Batch>(this);
	batch->data = m_batchData;

	m_batchData.offset = 0;
#endif
}

void PathRenderFeature::renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch)
{
	auto localBatch = static_cast<Batch*>(batch);

	RenderPass* pass = context->renderPass();

	m_glnvgContext->g = context;
	nvgBeginFrame(m_nvgContext, pass->width(), pass->height(), 1.0);	// TODO: DPI


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

