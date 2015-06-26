/**
	@file	TextRenderer.h
*/
#pragma once
#include "Common.h"
#include "Color.h"
#include "../Imaging/Font.h"

namespace Lumino
{
namespace Graphics
{
class TextRendererImplemented;

/**
	@file	文字列の描画を行うクラスです。
*/
class TextRenderer
	: public RefObject
{
public:

	/**
		@brief		TextRenderer のインスタンスを作成します。
	*/
	static TextRenderer* Create(GraphicsManager* manager);

public:
	void SetTransform(const Matrix& matrix);
	void SetViewProjection(const Matrix& view, const Matrix& proj, const Size& viewPixelSize);


	void SetFont(Imaging::Font* font);
	void SetForeColor(Color color) { m_foreColor = color; }
	void SetStrokeColor(Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void SetTextTrimming(TextTrimming triming) { m_textTrimming = triming; }
	void SetFlowDirection(FlowDirection dir) { m_flowDirection = dir; }
	void SetDrawingArea(const Rect& area) { m_drawingArea = area; }	// いらないかも

	/**
		@brief		
	*/
	void DrawText(const char* text, int length);
	void DrawText(const wchar_t* text, int length);

	/**
		@brief		要求されているスプライトを全て描画します。
	*/
	void Flash();

private:
	TextRenderer(GraphicsManager* manager);
	virtual ~TextRenderer();
	GraphicsManager*			m_manager;
	Imaging::Font*		m_font;
	Color				m_foreColor;
	Color				m_strokeColor;
	int					m_strokeSize;
	TextAlignment		m_textAlignment;
	TextTrimming		m_textTrimming;
	FlowDirection		m_flowDirection;
	Rect				m_drawingArea;
};

} // namespace Graphics
} // namespace Lumino
