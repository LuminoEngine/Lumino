
#pragma once
#include <Lumino/Graphics/Bitmap.h>
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "../RenderingCommand.h"

LN_NAMESPACE_BEGIN
namespace detail {

class TextRendererCore
	: public RefObject
{
public:
	struct GlyphRunData
	{
		Matrix	transform;
		PointF	Position;

		// CacheGlyphInfo から取りだすデータ
		int			outlineOffset;
		RectI		srcRect;
		//RenderingCommandList::DataHandle	fillBitmapData;	// 無ければ 0
	};

	TextRendererCore();
	~TextRendererCore();
	void Initialize(GraphicsManager* manager);

	void SetState(const Matrix& world, const Matrix& viewProj, const SizeI& viewPixelSize);
	void Render(const GlyphRunData* dataList, int dataCount, FontGlyphTextureCache* cache, Brush* fillBrush);

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

	void InternalDrawRectangle(const Matrix& transform, const RectF& rect, const RectF& srcUVRect, const Color& color);
	void Flush(FontGlyphTextureCache* cache);

	GraphicsManager*		m_manager;
	Driver::IRenderer*		m_renderer;
	RefPtr<Driver::IVertexDeclaration>	m_vertexDeclaration;
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
	void SetViewInfo(const Matrix& viewProj, const SizeI& viewPixelSize);
	void SetState(Font* font, Brush* fillBrush);

	void DrawGlyphRun(const Matrix& transform, const PointI& position, GlyphRun* glyphRun);
	void DrawGlyphRun(const Matrix& transform, const PointF& position, GlyphRun* glyphRun);	// SetFont 無視

	void DrawString(const Matrix& transform, const TCHAR* str, int length, const PointF& position);
	void DrawString(const Matrix& transform, const TCHAR* str, int length, const RectF& rect, StringFormatFlags flags);

	virtual bool IsStandaloneShader() const { return true; }
	virtual void Flush() override;
	virtual void OnActivated() { m_stateModified = true; }
	virtual void OnDeactivated() { Flush(); }
	virtual void OnSetState(const DrawElementBatch* state);

public:
	// TODO: ↓いまは Flush でやるようなことをしている。後で変更したい。
	void DrawGlyphsInternal(const Matrix& transform, const PointF& position, const List<TextLayoutResultItem>& layoutItems, FontGlyphTextureCache* cache);
	void FlushInternal(FontGlyphTextureCache* cache);
	void CheckUpdateState();

private:
	GraphicsManager*	m_manager;
	TextRendererCore*	m_core;
	List<TextRendererCore::GlyphRunData>	m_glyphLayoutDataList;

	Matrix				m_transform;
	Matrix				m_viewProj;
	SizeI				m_viewPixelSize;
	RefPtr<RawFont>		m_font;
	RefPtr<Brush>		m_fillBrush;
	bool				m_stateModified;
	bool				m_flushRequested;
};






struct VectorGlyphData
{
	Matrix	transform;
	PointF	position;
};

class VectorTextRendererCore
	: public RefObject
{
public:
	VectorTextRendererCore();
	virtual ~VectorTextRendererCore();
	void Initialize(GraphicsManager* manager);
};

//
//class VectorTextRenderer
//	: public RefObject
//	, public detail::IRendererPloxy
//{
//public:
//	VectorTextRenderer();
//	virtual ~VectorTextRenderer();
//	void Initialize(GraphicsManager* manager);
//
//	void SetTransform(const Matrix& matrix);
//
//
//	/**
//		@brief		スプライトの描画を要求します。
//		@param[in]	position	: 
//		@param[in]	center		: 
//		@param[in]	size		: 
//		@param[in]	texture		: 
//		@param[in]	srcRect		: 
//		@param[in]	color		: 
//	*/
//	void DrawRequest2D(
//		const Vector3& position,
//		const Vector2& size,
//		const Vector2& anchorRatio,
//		Texture* texture,
//		const RectF& srcRect,
//		const Color& color);
//
//	void DrawRequest(
//		const Vector3& position,
//		const Vector2& anchorRatio,
//		const Vector2& size,
//		Texture* texture,
//		const RectF& srcRect,
//		const Color& color,
//		SpriteBaseDirection baseDirection);
//
//	virtual bool IsStandaloneShader() const { return false; }
//	virtual void Flush() override;
//	virtual void OnActivated() override {}
//	virtual void OnDeactivated() override { Flush(); }
//
//	GraphicsManager* GetManager() const { return m_manager; }
//
//	static void MakeBoundingSphere(const Vector2& size, SpriteBaseDirection baseDir, detail::Sphere* sphere);
//
//LN_INTERNAL_ACCESS:
//	GraphicsManager*	m_manager;
//	SpriteRendererImpl*	m_internal;
//	SpriteSortMode		m_spriteSortMode;
//};
//
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
