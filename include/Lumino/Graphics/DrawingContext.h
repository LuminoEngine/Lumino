
#pragma once
#include "Common.h"
#include "Painter.h"	// for Brush

LN_NAMESPACE_BEGIN
class DrawingContextImpl;
class SpriteRenderer;

/**
	@brief	図形の枠線の描画方法を表します。
*/
class Pen
	: public RefObject
{
public:
	Pen()
		: m_thickness(0)
	{}

	void SetBrush(Brush* brush) { m_brush = brush; }
	Brush* GetBrush() const { return m_brush; }

	void SetThickness(float thickness) { m_thickness = thickness; }
	float GetThickness() const { return m_thickness; }

private:
	RefPtr<Brush>	m_brush;
	float			m_thickness;
};

namespace detail
{
enum class DrawingClass : uint32_t
{
	Unknown,
	PointList,
	LineList,
	TriangleList,
	GryphRun,		// TODO: いらないかも。専用クラスに任せたい
};


struct DrawingState
{
	Matrix			transform;
	RefPtr<Brush>	brush;
	RefPtr<Pen>		pen;
	RefPtr<Font>	font;
	float			opacity = 1.0f;
	ToneF			tone;
};

}


/**
	@brief	図形や画像、テキストを描画するための機能を提供します。
*/
class DrawingContext
	: public RefObject
{
public:

	void SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize);

	void SetTransform(const Matrix& matrix);
	void SetBrush(Brush* brush);
	void SetPen(Pen* pen);
	void SetOpacity(float opacity);	// 0~1
	void SetTone(const ToneF& tone);
	void SetFont(Font* font);

	void MoveTo(const Vector3& point, const ColorF& color);
	void LineTo(const Vector3& point, const ColorF& color);

	void DrawPoint(const Vector3& point, const ColorF& color);

	/**
		@brief		直線を描画します。
	*/
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& color);

	void DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color);

	/**
		@brief		矩形を描画します。
	*/
	void DrawRectangle(const RectF& rect, const ColorF& color);

	void DrawEllipse(const Vector3& center, const Vector2& radius);

	void DrawTexture(const RectF& rect, Texture* texture, const Rect& secRect, const ColorF& color);

	void Flush();

LN_INTERNAL_ACCESS:
	DrawingContext();
	virtual ~DrawingContext();
	void Initialize(GraphicsManager* manager);

private:
	void AddCommand(const void* command, size_t size);
	void FlushInternal();
	void SetDrawingClassInternal(detail::DrawingClass dc);
	//void CheckFlush();

	GraphicsManager*		m_manager;
	DrawingContextImpl*		m_internal;
	ByteBuffer				m_commandsBuffer;
	size_t					m_commandsUsingByte;
	detail::DrawingClass	m_currentDrawingClass;
	detail::DrawingState	m_currentState;
	
	Vector2					m_uvParPixel;
	Matrix					m_invViewProj;
	Vector2					m_viewOnePixelOffset;

	bool					m_flushRequested;
	//bool					m_stateModified;

	TextureBrush*			m_internalTextureBrush;
};


class GraphicsContext
	: public RefObject
{
public:	// TODO
	//RenderingPass*				Pass;
	Details::Renderer*			Renderer;	// TODO 内部クラス
	GeometryRenderer*			GeometryRenderer;
	//Camera*						CurrentCamera;
	//MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

	
	/**
		@brief	
	*/
	static GraphicsContext* GetContext();

public:

	void Set2DRenderingMode(float minZ = 0.0f, float maxZ = 1.0f);

	void SetBrush(Brush* brush);
	void SetOpacity(float opacity);	// 0~1
	void DrawPoint(const Vector3& point, const ColorF& color);
	void DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color);
	void DrawRectangle(const RectF& rect, const ColorF& color);
	void DrawEllipse(const Vector3& center, const Vector2& radius);
	void DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color);

	void Flush();






	DrawingContext* BeginDrawingContext();
	SpriteRenderer* BeginSpriteRendering();


protected:
	enum class RendererType
	{
		None,
		DrawingContext,
		SpriteRenderer,
	};

	friend class GraphicsManager;
	GraphicsContext(GraphicsManager* manager);
	virtual ~GraphicsContext();


	RendererType	m_currentRenderer;
	DrawingContext	m_drawingContext;
	SpriteRenderer*	m_spriteRenderer;

private:
	void TryChangeRenderingClass(RendererType dc);
};

LN_NAMESPACE_END
