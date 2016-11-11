
#pragma once
#include <Lumino/Graphics/Common.h>
#include "../../external/nanovg/nanovg.h"
#include <Lumino/Base/GeometryStructs.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Math/Matrix.h>
#include <Lumino/Graphics/Brush.h>

LN_NAMESPACE_BEGIN
namespace detail
{
struct LNNVGcontext;

class NanoVGCommandList
	: public CommandDataCache
{
};

enum class NanoVGBrushType
{
	SolidColor,
	LinearGradient,
	BoxGradient,
	RadialGradient,
	ImagePattern,
};

struct NanoVGBrush
{
	NanoVGBrushType	type;

	union
	{
		struct
		{
			NVGcolor color;
		} SolidColorInfo;

		struct
		{
			float sx;
			float sy;
			float ex;
			float ey;
			NVGcolor icol;
			NVGcolor ocol;
		} LinearGradientInfo;

		struct
		{
			float x;
			float y;
			float w;
			float h;
			float r;
			float f;
			NVGcolor icol;
			NVGcolor ocol;
		} BoxGradientInfo;

		struct
		{
			float cx;
			float cy;
			float inr;
			float outr;
			NVGcolor icol;
			NVGcolor ocol;
		} RadialGradientInfo;

		struct
		{
			float ox;
			float oy;
			float ex;
			float ey;
			float angle;
			float alpha;
		} ImagePatternInfo;
	};

	RefPtr<Driver::ITexture>	imagePatternTexture;
};

struct NanoVGState
{
	NanoVGBrush	fillBrush;
	NanoVGBrush	strokeBrush;
	float		strokeWidth;
	float		miterLimit;
	int			lineCap;
	int			lineJoin;
	float		globalAlpha;
};

class NanoVGCommandHelper
{
public:
	enum Command
	{
		Cmd_nvgMoveTo,
		Cmd_nvgLineTo,
		Cmd_nvgBezierTo,
		Cmd_nvgQuadTo,
		Cmd_nvgArcTo,
		Cmd_nvgClosePath,
		Cmd_nvgPathWinding,
		Cmd_nvgArc,
		Cmd_nvgRect,
		Cmd_nvgRoundedRect,
		Cmd_nvgRoundedRectVarying,
		Cmd_nvgEllipse,
		Cmd_nvgCircle,
		Cmd_nvgFill,
		Cmd_nvgStroke,
	};

	static void nvgMoveTo(NanoVGCommandList* ctx, float x, float y);
	static void nvgLineTo(NanoVGCommandList* ctx, float x, float y);
	static void nvgBezierTo(NanoVGCommandList* ctx, float c1x, float c1y, float c2x, float c2y, float x, float y);
	static void nvgQuadTo(NanoVGCommandList* ctx, float cx, float cy, float x, float y);
	static void nvgArcTo(NanoVGCommandList* ctx, float x1, float y1, float x2, float y2, float radius);
	static void nvgClosePath(NanoVGCommandList* ctx);
	static void nvgPathWinding(NanoVGCommandList* ctx, int dir);
	static void nvgArc(NanoVGCommandList* ctx, float cx, float cy, float r, float a0, float a1, int dir);
	static void nvgRect(NanoVGCommandList* ctx, float x, float y, float w, float h);
	static void nvgRoundedRect(NanoVGCommandList* ctx, float x, float y, float w, float h, float r);
	static void nvgRoundedRectVarying(NanoVGCommandList* ctx, float x, float y, float w, float h, float radTopLeft, float radTopRight, float radBottomRight, float radBottomLeft);
	static void nvgEllipse(NanoVGCommandList* ctx, float cx, float cy, float rx, float ry);
	static void nvgCircle(NanoVGCommandList* ctx, float cx, float cy, float r);
	static void nvgFill(NanoVGCommandList* ctx);
	static void nvgStroke(NanoVGCommandList* ctx);
	static void ExecuteCommand(NanoVGCommandList* commandList, NVGcontext* ctx);
	static void ExpandState(Brush* brush, Pen* pen, NanoVGState* outState);
	static void ExpandBrushState(Brush* brush, NanoVGBrush* outBrush);
	static void ApplyState(NVGcontext* ctx, const NanoVGState* state);
	static NVGpaint GetNVGpaint(NVGcontext* ctx, const NanoVGBrush& brush);
};

class NanoVGRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	NanoVGRenderer();
	virtual ~NanoVGRenderer();
	void Initialize(GraphicsManager* manager);

	RefPtr<NanoVGCommandList> TakeCommandList();
	void ExecuteCommand(NanoVGCommandList* commandList);

protected:
	virtual bool IsStandaloneShader() const { return true; }
	virtual void Flush() override {}
	virtual void OnActivated() override {}
	virtual void OnDeactivated() override { Flush(); }
	virtual void OnSetState(const DrawElementBatch* state);

private:
	void ExecuteCommandInternal(const NanoVGState& state, NanoVGCommandList* commandList);
	void PushCommandList(NanoVGCommandList* commandList);

	GraphicsManager*				m_manager;
	NVGcontext*						m_nvgContext;
	NanoVGState						m_state;
	List<RefPtr<NanoVGCommandList>>	m_commandListCache;
	Mutex							m_commandListCacheMutex;
};

} // namespace detail
LN_NAMESPACE_END
