#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	
*/
class Graphics
{
public:
	static const int MaxMultiRenderTargets = 4;

	//static void Set2DRenderingMode(float minZ = 0.0f, float maxZ = 1.0f);

	//static void setBrush(Brush* brush);
	//static void setOpacity(float opacity);	// 0~1
	//static void SetPrimitiveRenderingEnabled(bool enabled);

	//static void MoveTo(const Vector3& point, const Color& color);
	//static void LineTo(const Vector3& point, const Color& color);
	//static void BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const Color& color);
	//static void ClosePath();

	////static void DrawPoint(const Vector3& point, const ColorF& color);

	//static void DrawTriangle(const Vector3& p1, const Color& p1Color, const Vector3& p2, const Color& p2Color, const Vector3& p3, const Color& p3Color);

	//static void drawRectangle(const RectF& rect);

	//static void DrawEllipse(const Vector3& center, const Vector2& radius);

	//static void drawTexture(const RectF& rect, Texture* texture, const RectI& srcRect, const Color& color);

	//static void drawText(const StringRef& text, const PointF& position);

	//static void Flush();

	/**
		@brief		レンダリングを開始します。
		@details	Application::updateFrame() を使用する場合、この関数を呼び出す必要はありません。
	*/
	//void BeginRendering();
	
	/**
		@brief		レンダリングを終了します。
	*/
	//void endRendering();

	static void changeDirectX9Device(void* id3d9device);
};

//class GraphicsPlatformSupport
//{
//public:
//	// IDirect3DSurface9
//	static RenderTargetTexturePtr CreateRenderTargetFromD3D9Surface(void* surface);
//};

LN_NAMESPACE_END
