
#pragma once
#include <stack>
#include "Common.h"
#include "ContextInterface.h"
#include "Brush.h"

#pragma push_macro("DrawText")
#undef DrawText

LN_NAMESPACE_BEGIN
class RenderingContext;
namespace detail { class GeometryRenderer; }
namespace detail { class TextRenderer; }
namespace detail { class FrameRectRenderer; }

class DrawingContext
	: public detail::ContextInterface
{
public:

	void Set2DRenderingMode(float minZ = 0.0f, float maxZ = 1.0f);
	void SetViewProjectionTransform(const Matrix& view, const Matrix& proj);

	void SetRenderTarget(int index, Texture* texture);
	void SetDepthBuffer(Texture* depthBuffer);

	void SetBlendMode(BlendMode mode);

	void SetTransform(const Matrix& matrix);
	void SetOpacity(float opacity);	// 0~1
	void SetBrush(Brush* brush);
	void SetFont(Font* font);




	void PushState();
	void PopState();

	/** @} */

	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);


	void MoveTo(const Vector3& point, const Color& color);
	void LineTo(const Vector3& point, const Color& color);
	void BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const Color& color);
	void ClosePath();

	//void DrawPoint(const Vector3& point, const ColorF& color);
	void DrawTriangle(const Vector3& p1, const Color& p1Color, const Vector3& p2, const Color& p2Color, const Vector3& p3, const Color& p3Color);
	void DrawRectangle(const RectF& rect);
	void DrawEllipse(const Vector3& center, const Vector2& radius);
	void DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const Color& color);


	void DrawText(const StringRef& text, const PointF& position);
	void DrawText(const StringRef& text, const RectF& rect, StringFormatFlags flags);



	void Flush();



LN_INTERNAL_ACCESS:
	DrawingContext();
	virtual ~DrawingContext();
	void Initialize(GraphicsManager* manager);
	void InitializeFrame(RenderTarget* renderTarget);
	void InheritStatus(RenderingContext* parent);

protected:
	virtual bool OnCheckStateChanged() override;
	virtual void OnStateFlush(detail::IRendererPloxy* activeRenderer) override;
	virtual void OnPrimitiveFlush() override;
	virtual void OnShaderVariableModified(ShaderVariable* var) override;

private:

	struct ContextState : public detail::BasicContextState
	{
		Matrix			worldTransform;
		Matrix			viewTransform;
		Matrix			projectionTransform;
		float			opacity = 1.0f;
		RefPtr<Brush>	fillBrush;
		RefPtr<Font>	font;

		bool Equals(const ContextState& s) const;
	};

	detail::GeometryRenderer*	m_geometryRenderer;
	detail::TextRenderer*		m_textRenderer;
	detail::FrameRectRenderer*	m_frameRectRenderer;

	ContextState				m_state;
	ContextState				m_backendState;
	std::stack<ContextState>	m_stateStack;

public:
	void LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const PointF& position);
	void LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const RectF& rect, StringFormatFlags flags);
};

LN_NAMESPACE_END

#pragma pop_macro("DrawText")
