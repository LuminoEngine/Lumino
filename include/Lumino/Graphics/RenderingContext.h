
#pragma once
#include "../Graphics/Font.h"
#include "Texture.h"
#include "GlyphRun.h"
#include "Painter.h"	// Brush

namespace Lumino
{

/**
	@brief	
*/
//class RenderingContext
//	: public RefObject
//{
//public:
//	void Begin();
//	void End();
//
//	void ResetState();
//
//	void SetProjection(const Size& viewSize, float nearZ = 0.0f, float farZ = 1.0f);
//	void SetProjection(const SizeF& viewSize, float nearZ = 0.0f, float farZ = 1.0f);
//
//	void SetTransform(const Matrix& matrix);
//	void SetBrush(Brush* brush);
//	void SetSolidColor(const ColorF& color);
//	void SetTexture(Texture* texture, const Rect& srcRect);	///< ユーティリティ。TextureBrush をセットする
//	void SetOpacity(float opacity);	// 0~1
//	void SetTone(const ToneF& tone);
//	void SetFont(Font* font);
//
//	//void PushState();
//	//void PopState();
//
//	void DrawRectangle(const RectF& rect);
//	void DrawRectangle(float x, float y, float width, float height) { DrawRectangle(RectF(x, y, width, height)); }
//
//
//
//	//void DrawRectangle(const RectF& rect);	// TODO: 変える
//	// TODO: DrawRectangle がこの機能を兼ねる。いらない。
//	void DrawFrameRectangle(const RectF& rect, float frameWidth);
//
//	/// セットされている Brush は影響しません
//	//void DrawTexture(const RectF& dstRect, Texture* texture, const Rect& srcRect);
//
//	void DrawGlyphRun(const Point& position, GlyphRun* glyphRun);
//	void DrawGlyphRun(const PointF& position, GlyphRun* glyphRun);	// SetFont 無視
//
//	void DrawString(const String& str, const PointF& position);
//	void DrawString(const TCHAR* str, int length, const PointF& position);
//	void DrawString(const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags);
//
//	void Flush();
//
//protected:
//	RenderingContext(GraphicsManager* manager);
//	virtual ~RenderingContext();
//
//private:
//	void DrawGlyphs(const PointF& position, const TextLayoutResult* result, Internal::FontGlyphTextureCache* cache);
//	void CheckUpdateState();
//
//	GraphicsManager*				m_manager;
//	PainterEngine*					m_internal;
//	Details::PainterState			m_currentState;
//	bool							m_stateModified;
//
//	ColorBrush				m_internalSolidColorBrush;
//	TextureBrush			m_internalTextureBrush;
//
//	ByteBuffer				m_tempBuffer;
//	//RefPtr<Font>			m_currentFont;
//};

} // namespace Lumino
