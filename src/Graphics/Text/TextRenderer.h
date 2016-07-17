
#pragma once
#include <Lumino/Graphics/Bitmap.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "../PainterEngine.h"
#include "../RenderingCommand.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

namespace detail
{

class TextRendererCore
	: public RefObject
{
public:
	struct GlyphRunData
	{
		PointF		Position;
		//RectF		SrcPixelRect;

		// CacheGlyphInfo から取りだすデータ
		int			outlineOffset;
		Rect		srcRect;
		//RenderingCommandList::DataHandle	fillBitmapData;	// 無ければ 0
	};

	TextRendererCore();
	~TextRendererCore();
	void Initialize(GraphicsManager* manager);

	void SetState(const Matrix& world, const Matrix& viewProj, const Size& viewPixelSize);
	void DrawGlyphRun(const PointF& position, const GlyphRunData* dataList, int dataCount, Internal::FontGlyphTextureCache* cache, RenderingCommandList* cmdList/*Driver::ITexture* glyphsTexture, Driver::ITexture* strokesTexture*//*, const ColorF& foreColor, const ColorF& strokeColor*/);
	void Flush(Internal::FontGlyphTextureCache* cache);

private:

	struct Vertex
	{
	public:
		Vector3	position;
		Vector4	color;
		Vector2	uv;

		// 頂点レイアウト
		static VertexElement* Elements()
		{
			static VertexElement elements[] =
			{
				{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
				{ 0, VertexElementType_Float4, VertexElementUsage_Color, 0 },
				{ 0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0 },
			};
			return elements;
		}
		static const int ElementCount = 3;
	};

	void InternalDrawRectangle(const RectF& rect, const RectF& srcUVRect);

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	CacheBuffer<Vertex>		m_vertexCache;
	CacheBuffer<uint16_t>	m_indexCache;

	Color					m_foreColor;
	ToneF					m_tone;
	//Driver::ITexture*		m_foreTexture;
	//Driver::ITexture*		m_glyphsMaskTexture;

	Bitmap					m_tmpBitmap;

	struct
	{
		Driver::IShader*			shader;
		Driver::IShaderTechnique*	technique;
		Driver::IShaderPass*		pass;
		Driver::IShaderVariable*	varWorldMatrix;
		Driver::IShaderVariable*	varViewProjMatrix;
		Driver::IShaderVariable*	varTone;
		Driver::IShaderVariable*	varTexture;
		//Driver::IShaderVariable*	varGlyphMaskSampler;
		Driver::IShaderVariable*	varPixelStep;

	} m_shader;

	TextRenderer*			m_activeRenderer;
};

class TextRenderer
	: public RefObject
	, public IRendererPloxy
{
public:
	TextRenderer();
	~TextRenderer();
	void Initialize(GraphicsManager* manager);

	void SetTransform(const Matrix& matrix);
	void SetViewProjMatrix(const Matrix& matrix);
	void SetViewPixelSize(const Size& size);
	void SetFont(Font* font);

	void DrawGlyphRun(const Point& position, GlyphRun* glyphRun);
	void DrawGlyphRun(const PointF& position, GlyphRun* glyphRun);	// SetFont 無視

	void DrawString(const String& str, const PointF& position);
	void DrawString(const TCHAR* str, int length, const PointF& position);
	void DrawString(const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags);

	virtual void Flush() override;
	virtual void OnActivated() { m_stateModified = true; }
	virtual void OnDeactivated() { Flush(); }

public:
	// TODO: ↓いまは Flush でやるようなことをしている。後で変更したい。
	void DrawGlyphsInternal(const PointF& position, const Array<TextLayoutResultItem>& layoutItems, Internal::FontGlyphTextureCache* cache);
	void CheckUpdateState();

private:
	GraphicsManager*	m_manager;
	TextRendererCore*	m_core;
	ByteBuffer			m_tempBuffer;	// TODO: 複数 Renderer が作られたときに備えて、メモリ効率のため core に移動してしまうのも手

	Matrix				m_transform;
	Matrix				m_viewProj;
	Size				m_viewPixelSize;
	Font*				m_font;
	bool				m_stateModified;
	bool				m_flushRequested;
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
	TextRenderer(Bitmap* targetBitmap, Font* font);
	~TextRenderer();

	void SetForeColor(Graphics::Color color) { m_foreColor = color; }
	void SetStrokeColor(Graphics::Color color) { m_strokeColor = color; }
	void SetStrokeSize(int size) { m_strokeSize = size; }
	void SetTextAlignment(TextAlignment align) { m_textAlignment = align; }
	void SetTextTrimming(TextTrimming triming) { m_textTrimming = triming; }
	void SetFlowDirection(FlowDirection dir) { m_flowDirection = dir; }
	void SetAreaBox(const Rect& area) { m_areaBox = area; }

	/// (連続で呼び出せば、前回の描画情報を引き継ぐ。例えばカーニングを考慮しつつテキストの色を分けたい場合)
	void DrawText(const char* text, int length);
	void DrawText(const wchar_t* text, int length);

	void ResetKerning() { m_prevGlyphData = NULL; }

private:
	void DrawTextHorizontal(const UTF32* text, int length);
	void DrawLineHorizontal(const UTF32* text, int length, const Rect& lineArea);

private:
	Bitmap*			m_targetBitmap;
	Font*			m_font;
	Graphics::Color	m_foreColor;
	Graphics::Color	m_strokeColor;
	int				m_strokeSize;
	TextAlignment	m_textAlignment;
	TextTrimming	m_textTrimming;
	FlowDirection	m_flowDirection;
	Rect			m_areaBox;
	ByteBuffer		m_utf32Buffer;		///< UTF32 文字列への一時変換先 (頻繁にメモリ確保しないように、一度使ったメモリは使いまわしたい)
	FontGlyphData*	m_prevGlyphData;


};
#endif

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
