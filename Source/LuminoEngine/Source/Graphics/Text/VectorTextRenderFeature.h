
#pragma once
#include <Lumino/Graphics/Bitmap.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "../RenderingCommand.h"
#include "TextLayoutEngine.h"

LN_NAMESPACE_BEGIN
struct Vertex;

namespace detail {


struct VectorGlyphData
{
	int		cacheGlyphInfoHandle;
	Matrix	transform;
	Point	origin;		// baseline origin
};

class VectorTextRendererCore
	: public RefObject
{
public:
	VectorTextRendererCore();
	virtual ~VectorTextRendererCore();
	void initialize(GraphicsManager* manager);

	void requestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex);
	void render(const VectorGlyphData* dataList, int dataCount, VectorFontGlyphCache* cache, Brush* fillBrush);

private:
	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	CacheBuffer<Vertex>		m_vertexCache;
	CacheBuffer<uint16_t>	m_indexCache;
};


class VectorTextRenderer
	: public RefObject
	, public detail::IRenderFeature
{
public:
	VectorTextRenderer();
	virtual ~VectorTextRenderer();
	void initialize(GraphicsManager* manager);

	void drawString(const Matrix& transform, const UTF32* str, int length, const Rect& rect, TextLayoutOptions options);
	void drawChar(const Matrix& transform, UTF32 ch, const Rect& rect, TextLayoutOptions options);

	virtual bool isStandaloneShader() const { return false; }
	virtual void flush() override;
	virtual void onActivated() override {}
	virtual void onDeactivated() override { flush(); }
	virtual void onSetState(const RenderStage* state) override;

	GraphicsManager* getManager() const { return m_manager; }

private:
	void drawInternal(const Matrix& transform);

LN_INTERNAL_ACCESS:
	GraphicsManager*				m_manager;
	Ref<VectorTextRendererCore>	m_core;
	Ref<RawFont>					m_currentFont;
	Ref<Brush>					m_fillBrush;
	List<VectorGlyphData>			m_bufferingCache;
	TextLayoutEngine2::ResultData	m_layoutResult;
};

} // namespace detail

#if 0
/// テキストの配置方法
enum TextAlignment
{
	TextAlignment_Left = 0,			///< 左揃え
	TextAlignment_Center,			///< 中央揃え
	TextAlignment_Right,			///< 右揃え
	TextAlignment_Justify,			///< 両端揃え
};

/// 描画領域にテキストが収まりきらない場合に、テキストを切り取る方法
enum TextTrimming
{
	TextTrimming_None,				///< 切り取りを行わない。
	TextTrimming_CharacterEllipsis,	///< 略記号(...) を描画する。
};

/// テキストの描画方向
enum FlowDirection
{
	FlowDirection_LeftToRight,		///< テキストを左から右に描画する。
	FlowDirection_RightToLeft,		///< テキストを左から右に描画する。
};

class TextRenderer
{
public:
	TextRenderer(Bitmap* targetBitmap, RawFont* font);
	~TextRenderer();

	void SetForeColor(Graphics::Color color) { m_foreColor = color; }
	void SetStrokeColor(Graphics::Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void SetTextTrimming(TextTrimming triming) { m_textTrimming = triming; }
	void SetFlowDirection(FlowDirection dir) { m_flowDirection = dir; }
	void SetAreaBox(const RectI& area) { m_areaBox = area; }

	/// (連続で呼び出せば、前回の描画情報を引き継ぐ。例えばカーニングを考慮しつつテキストの色を分けたい場合)
	void DrawText(const char* text, int length);
	void DrawText(const wchar_t* text, int length);

	void ResetKerning() { m_prevGlyphData = NULL; }

private:
	void DrawTextHorizontal(const UTF32* text, int length);
	void DrawLineHorizontal(const UTF32* text, int length, const RectI& lineArea);

private:
	Bitmap*			m_targetBitmap;
	RawFont*		m_font;
	Graphics::Color	m_foreColor;
	Graphics::Color	m_strokeColor;
	int				m_strokeSize;
	TextAlignment	m_textAlignment;
	TextTrimming	m_textTrimming;
	FlowDirection	m_flowDirection;
	RectI			m_areaBox;
	ByteBuffer		m_utf32Buffer;		///< UTF32 文字列への一時変換先 (頻繁にメモリ確保しないように、一度使ったメモリは使いまわしたい)
	FontGlyphData*	m_prevGlyphData;


};
#endif

LN_NAMESPACE_END
