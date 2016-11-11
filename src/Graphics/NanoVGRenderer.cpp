
#include "Internal.h"
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

struct LNNVGcontext
{
	int	flags = 0;
	float view[2];

	GraphicsManager*			manager;
	RefPtr<Driver::IShader>		shader;
	Driver::IShaderPass*		shaderPass;
	Driver::IShaderVariable*	varViewSize;
	Driver::IShaderVariable*	varFrag;


	//IndexStack			textureIndexStack;
	//List<TexturePtr>	textureList;
	List<Driver::ITexture*>	textureList;
};

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

	ShaderCompileResult result;
	lnc->shader.Attach(device->CreateShader(codeData, codeLen, &result), false);
	LN_THROW(result.Level != ShaderCompileResultLevel_Error, CompilationException, result);
	lnc->shaderPass = lnc->shader->GetTechnique(0)->GetPass(0);
	lnc->varViewSize = lnc->shader->GetVariableByName(_T("viewSize"));
	lnc->varFrag = lnc->shader->GetVariableByName(_T("frag"));

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
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
}

//------------------------------------------------------------------------------
static void lnnvg__renderFill(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
	const float* bounds, const NVGpath* paths, int npaths)
{
}

//------------------------------------------------------------------------------
static void lnnvg__renderStroke(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
	float strokeWidth, const NVGpath* paths, int npaths)
{
}

//------------------------------------------------------------------------------
static void lnnvg__renderTriangles(void* uptr, NVGpaint* paint, NVGscissor* scissor,
	const NVGvertex* verts, int nverts)
{
}

//------------------------------------------------------------------------------
static void lnnvg__renderDelete(void* uptr)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
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
RefPtr<NanoVGCommandList> NanoVGRenderer::TakeCommandList()
{
	MutexScopedLock lock(m_commandListCacheMutex);

	if (m_commandListCache.IsEmpty())
	{
		return RefPtr<NanoVGCommandList>::MakeRef();
	}
	else
	{
		auto ptr = m_commandListCache.GetLast();
		m_commandListCache.RemoveLast();
		return ptr;
	}
}

//------------------------------------------------------------------------------
void NanoVGRenderer::ExecuteCommand(NanoVGCommandList* commandList)
{
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
	NanoVGCommandHelper::ExpandState(state->GetBrush(), state->GetPen(), &m_state);
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

	MutexScopedLock lock(m_commandListCacheMutex);
	m_commandListCache.Add(commandList);
}

//==============================================================================
// NanoVGCommandHelper
//==============================================================================
void NanoVGCommandHelper::nvgMoveTo(NanoVGCommandList* ctx, float x, float y)
{
	float cmd[] = { Cmd_nvgMoveTo, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgLineTo(NanoVGCommandList* ctx, float x, float y)
{
	float cmd[] = { Cmd_nvgLineTo, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgBezierTo(NanoVGCommandList* ctx, float c1x, float c1y, float c2x, float c2y, float x, float y)
{
	float cmd[] = { Cmd_nvgBezierTo, c1x, c1y, c2x, c2y, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgQuadTo(NanoVGCommandList* ctx, float cx, float cy, float x, float y)
{
	float cmd[] = { Cmd_nvgQuadTo, cx, cy, x, y };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgArcTo(NanoVGCommandList* ctx, float x1, float y1, float x2, float y2, float radius)
{
	float cmd[] = { Cmd_nvgArcTo, x1, y1, x2, y2, radius };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgClosePath(NanoVGCommandList* ctx)
{
	float cmd[] = { Cmd_nvgClosePath };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgPathWinding(NanoVGCommandList* ctx, int dir)
{
	float cmd[] = { Cmd_nvgPathWinding, dir };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgArc(NanoVGCommandList* ctx, float cx, float cy, float r, float a0, float a1, int dir)
{
	float cmd[] = { Cmd_nvgArc, cx, cy, r, a0, a1, dir };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgRect(NanoVGCommandList* ctx, float x, float y, float w, float h)
{
	float cmd[] = { Cmd_nvgRect, x, y, w, h };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgRoundedRect(NanoVGCommandList* ctx, float x, float y, float w, float h, float r)
{
	float cmd[] = { Cmd_nvgRoundedRect, x, y, w, h, r };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgRoundedRectVarying(NanoVGCommandList* ctx, float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft)
{
	float cmd[] = { Cmd_nvgRoundedRectVarying, x, y, w, h, radTopLeft, radTopRight, radBottomRight, radBottomLeft };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgEllipse(NanoVGCommandList* ctx, float cx, float cy, float rx, float ry)
{
	float cmd[] = { Cmd_nvgEllipse, cx, cy, rx, ry };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgCircle(NanoVGCommandList* ctx, float cx, float cy, float r)
{
	float cmd[] = { Cmd_nvgCircle, cx, cy, r };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgFill(NanoVGCommandList* ctx)
{
	float cmd[] = { Cmd_nvgFill };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::nvgStroke(NanoVGCommandList* ctx)
{
	float cmd[] = { Cmd_nvgStroke };
	ctx->AllocData(sizeof(cmd), cmd);
}
void NanoVGCommandHelper::ExecuteCommand(NanoVGCommandList* commandList, NVGcontext* ctx)
{
	int count = commandList->GetDataCount();
	for (int i = 0; i < count; i++)
	{
		float* cmd = (float*)commandList->GetDataByIndex(i);
		switch ((int)cmd[0])
		{
			case Cmd_nvgMoveTo:
				::nvgMoveTo(ctx, cmd[1], cmd[2]);
				break;
			case Cmd_nvgLineTo:
				::nvgLineTo(ctx, cmd[1], cmd[2]);
				break;
			case Cmd_nvgBezierTo:
				::nvgBezierTo(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6]);
				break;
			case Cmd_nvgQuadTo:
				::nvgQuadTo(ctx, cmd[1], cmd[2], cmd[3], cmd[4]);
				break;
			case Cmd_nvgArcTo:
				::nvgArcTo(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
				break;
			case Cmd_nvgClosePath:
				::nvgClosePath(ctx);
				break;
			case Cmd_nvgPathWinding:
				::nvgPathWinding(ctx, cmd[1]);
				break;
			case Cmd_nvgArc:
				::nvgArc(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6]);
				break;
			case Cmd_nvgRect:
				::nvgRect(ctx, cmd[1], cmd[2], cmd[3], cmd[4]);
				break;
			case Cmd_nvgRoundedRect:
				::nvgRoundedRect(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
				break;
			case Cmd_nvgRoundedRectVarying:
				::nvgRoundedRectVarying(ctx, cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6], cmd[7], cmd[8]);
				break;
			case Cmd_nvgEllipse:
				::nvgEllipse(ctx, cmd[1], cmd[2], cmd[3], cmd[4]);
				break;
			case Cmd_nvgCircle:
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
void NanoVGCommandHelper::ExpandState(Brush* brush, Pen* pen, NanoVGState* outState)
{
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
		switch (brush->GetType())
		{
			case BrushType_SolidColor:
			{
				ColorBrush* b = static_cast<ColorBrush*>(brush);
				outBrush->type = NanoVGBrushType::SolidColor;
				outBrush->SolidColorInfo.color.r = b->GetColor().r;
				outBrush->SolidColorInfo.color.g = b->GetColor().g;
				outBrush->SolidColorInfo.color.b = b->GetColor().b;
				outBrush->SolidColorInfo.color.a = b->GetColor().a;
				break;
			}
			case BrushType_Texture:
			{
				TextureBrush* b = static_cast<TextureBrush*>(brush);
				outBrush->type = NanoVGBrushType::ImagePattern;
				outBrush->ImagePatternInfo.ox = b->GetSourceRect().x;
				outBrush->ImagePatternInfo.oy = b->GetSourceRect().y;
				outBrush->ImagePatternInfo.ex = b->GetSourceRect().width;
				outBrush->ImagePatternInfo.ey = b->GetSourceRect().height;
				outBrush->ImagePatternInfo.angle = 0;
				outBrush->ImagePatternInfo.alpha = 1.0f;
				outBrush->imagePatternTexture = b->GetTexture()->ResolveDeviceObject();
				break;
			}
			default:
				assert(0);
				break;
		}
	}
	else
	{
		ColorBrush* b = static_cast<ColorBrush*>(brush);
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
			return nvgImagePattern(ctx, info.ox, info.oy, info.ex, info.ey, info.alpha, lnnvg__AddImageTexture(ctx, brush.imagePatternTexture), info.alpha);
		}
	}
	assert(0);
	return NVGpaint();
}

} // namespace detail
LN_NAMESPACE_END

