#include "Internal.hpp"
#include <LuminoEngine/Rendering/detail/NanoVGHelper.hpp>

static int glnvg__maxi(int a, int b) { return a > b ? a : b; }

static GLNVGcall* glnvg__allocCall(GLNVGcontextBase* gl)
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

static int glnvg__allocPaths(GLNVGcontextBase* gl, int n)
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

static int glnvg__maxVertCount(const NVGpath* paths, int npaths)
{
	int i, count = 0;
	for (i = 0; i < npaths; i++) {
		count += paths[i].nfill;
		count += paths[i].nstroke;
	}
	return count;
}

static int glnvg__allocVerts(GLNVGcontextBase* gl, int n)
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

static void glnvg__vset(NVGvertex* vtx, float x, float y, float u, float v)
{
	vtx->x = x;
	vtx->y = y;
	vtx->u = u;
	vtx->v = v;
}

static int glnvg__allocFragUniforms(GLNVGcontextBase* gl, int n)
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

static GLNVGfragUniforms* nvg__fragUniformPtr(GLNVGcontextBase* gl, int i)
{
	return (GLNVGfragUniforms*)&gl->uniforms[i];
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

static int glnvg__convertPaint(GLNVGcontextBase* gl, GLNVGfragUniforms* frag, NVGpaint* paint,
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
		tex = GLNVGcontextBase::findTexture(gl, paint->image);
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


// params.renderViewport
static void glnvg__renderViewport(void* uptr, float width, float height, float devicePixelRatio)
{
	NVG_NOTUSED(devicePixelRatio);
	GLNVGcontextBase* gl = (GLNVGcontextBase*)uptr;
	gl->view[0] = width;
	gl->view[1] = height;
}

// glnvg__renderCancel
static void glnvg__renderCancel(void* uptr) {
	GLNVGcontextBase* gl = (GLNVGcontextBase*)uptr;
	gl->nverts = 0;
	gl->npaths = 0;
	gl->ncalls = 0;
	gl->nuniforms = 0;
}

static void glnvg__renderFill(void* uptr, NVGpaint* paint, NVGcompositeOperationState compositeOperation, NVGscissor* scissor, float fringe,
	const float* bounds, const NVGpath* paths, int npaths)
{
	GLNVGcontextBase* gl = (GLNVGcontextBase*)uptr;
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
	GLNVGcontextBase* gl = (GLNVGcontextBase*)uptr;
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
	const NVGvertex* verts, int nverts/*, float fringe*/)
{
	const float fringe = 1.0f;
	GLNVGcontextBase* gl = (GLNVGcontextBase*)uptr;
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

void GLNVGcontextBase::clear()
{
	textures = nullptr;
	view[0] = 0.0f;
	view[1] = 0.0f;
	ntextures = 0;
	ctextures = 0;
	textureId = 0;
	fragSize = 0;
	flags = 0;
	calls = nullptr;
	ccalls = 0;
	ncalls = 0;
	paths = nullptr;
	cpaths = 0;
	npaths = 0;
	verts = nullptr;
	cverts = 0;
	nverts = 0;
	uniforms = nullptr;
	cuniforms = 0;
	nuniforms = 0;
	blendFunc = {};
	dummyTex = 0;
}

void GLNVGcontextBase::makeNVGparams(NVGparams* params)
{
	memset(params, 0, sizeof(params));
	params->renderViewport = glnvg__renderViewport;
	params->renderCancel = glnvg__renderCancel;
	params->renderFill = glnvg__renderFill;
	params->renderStroke = glnvg__renderStroke;
	params->renderTriangles = glnvg__renderTriangles;
}

void GLNVGcontextBase::dispose()
{
	free(paths);
	free(verts);
	free(uniforms);
	free(calls);
}

GLNVGtexture* GLNVGcontextBase::findTexture(GLNVGcontextBase* gl, int id)
{
	int i;
	for (i = 0; i < gl->ntextures; i++)
		if (gl->textures[i].id == id)
			return &gl->textures[i];
	return NULL;
}
