
#pragma once
#include "Common.h"
#include "ContextInterface.h"
#include "Brush.h"

LN_NAMESPACE_BEGIN
class SpriteRenderer;
namespace detail { class TextRenderer; }
namespace detail { class PrimitiveRenderer; }
namespace detail { class GeometryRenderer; }

class GraphicsContext
	: public RefObject
	, public detail::IContext
{
public:	// TODO
	//RenderingPass*				Pass;
	Details::Renderer*			Renderer;	// TODO 内部クラス
	//Camera*						CurrentCamera;
	//MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

	
	/**
		@brief	
	*/
	static GraphicsContext* GetContext();

public:

	void Set2DRenderingMode(float minZ = 0.0f, float maxZ = 1.0f);

	void SetOpacity(float opacity);	// 0~1
	void SetBrush(Brush* brush);
	void SetFont(Font* font);

	void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	void MoveTo(const Vector3& point, const ColorF& color);
	void LineTo(const Vector3& point, const ColorF& color);
	void BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const ColorF& color);
	void ClosePath();

	void DrawPoint(const Vector3& point, const ColorF& color);
	void DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color);
	void DrawRectangle(const RectF& rect, const Color& color);
	void DrawEllipse(const Vector3& center, const Vector2& radius);
	void DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color);
	

	//// プリミティブ 
	//void DrawPrimitiveLine(
	//	const Vector3& from, const ColorF& fromColor,
	//	const Vector3& to, const ColorF& toColor);

	//// 左回り
	//void DrawSquare(
	//	float x1, float y1, float z1, float u1, float v1, const ColorF& c1,
	//	float x2, float y2, float z2, float u2, float v2, const ColorF& c2,
	//	float x3, float y3, float z3, float u3, float v3, const ColorF& c3,
	//	float x4, float y4, float z4, float u4, float v4, const ColorF& c4);


	void DrawText(const StringRef& text, const PointF& position);
	void DrawText(const StringRef& text, const RectF& rect, StringFormatFlags flags);

	//void Flush();






	//detail::GeometryRenderer* BeginDrawingContext();
	SpriteRenderer* BeginSpriteRendering();


public:
	//enum class RendererType
	//{
	//	None,
	//	GeometryRenderer,
	//	SpriteRenderer,
	//	TextRenderer,
	//	PrimitiveRenderer,
	//};

	friend class GraphicsManager;
	GraphicsContext();
	virtual ~GraphicsContext();
	void Initialize(GraphicsManager* manager);


	//RendererType				m_currentRenderer;
	detail::GeometryRenderer*	m_geometryRenderer;
	SpriteRenderer*				m_spriteRenderer;
	detail::TextRenderer*		m_textRenderer;

	// このクラスが管理するステート。VertexBuffer などは描画時に各種 RendererCore 内で変更されるからちょっと考える必要がありそう。
	//Shader*						m_shader;

protected:
	// detail::IContext interface
	virtual void OnStateFlushRequested() override;

private:
	//void TryChangeRenderingClass(RendererType dc);
};

LN_NAMESPACE_END
