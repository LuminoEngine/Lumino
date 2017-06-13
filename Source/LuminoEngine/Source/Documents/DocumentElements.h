
#pragma once
#include <Lumino/Documents/Common.h>
#include <Lumino/UI/LayoutElement.h>
#include "../Graphics/GraphicsManager.h"

LN_NAMESPACE_BEGIN
namespace detail {
class DocumentsManager;
class Inline;
class Block;

enum class InternalTextElementType
{
	Common,
	LineBreak,
};

class DocumentSelection
{
public:
	int	start;		///< text.GetLength() は有効値。Select(text.GetLength(), 0) でコンテンツの末尾にキャレットを移動する
	int length;

public:
	DocumentSelection()
		: start(0)
		, length(0)
	{}
};

/**
	@brief
*/
class Document
	: public Object
{
public:
	Document();
	virtual ~Document();
	void initialize();

	// 現在の内容をすべて破棄して、新しい1つの Paragraph を作る
	void setText(const StringRef& text);

LN_INTERNAL_ACCESS:
	void replace(int offset, int length, const StringRef& text);

private:
	void replace(int offset, int length, const UTF32* text, int len);

	DocumentsManager*		m_manager;
	List<RefPtr<Block>>		m_blockList;
};

/**
	@brief
*/
class TextElement
	: public Object
	, public ILayoutElement
{
public:

	/** フォントファミリ名を設定します。*/
	void setFontFamily(const StringRef& value) { m_fontData.Family = value; m_fontDataModified = true; }

	/** フォントファミリ名を取得します。*/
	const String& getFontFamily() const { return m_fontData.Family; }

	/** フォントサイズを設定します。*/
	void setFontSize(int value) { m_fontData.Size = value; m_fontDataModified = true; }

	/** フォントサイズを取得します。*/
	int getFontSize() const { return m_fontData.Size; }

	/** フォントの太字有無を設定します。*/
	void setFontBold(bool value) { m_fontData.IsBold = value; m_fontDataModified = true; }

	/** フォントの太字有無を取得します。*/
	bool isFontBold() const { return m_fontData.IsBold; }

	/** フォントのイタリック体有無を設定します。*/
	void setFontItalic(bool value) { m_fontData.IsItalic = value; m_fontDataModified = true; }

	/** フォントのイタリック体有無を取得します。*/
	bool isFontItalic() const { return m_fontData.IsItalic; }

	/** フォントのアンチエイリアス有無を設定します。*/
	void setFontAntiAlias(bool value) { m_fontData.IsAntiAlias = value; m_fontDataModified = true; }

	/** フォントのアンチエイリアス有無を取得します。*/
	bool isFontAntiAlias() const { return m_fontData.IsAntiAlias; }

	Brush* getForeground() const;


	virtual void render(const Matrix& transform, IDocumentsRenderer* renderer);

LN_PROTECTED_INTERNAL_ACCESS:
	virtual void onFontDataChanged(const FontData& newData);

	// ILayoutElement interface
	virtual const PointF& getLayoutPosition() const override;
	virtual Size getLayoutSize() const override;
	virtual const ThicknessF& getLayoutMargin() const override;
	virtual const ThicknessF& getLayoutPadding() const override;
	virtual AlignmentAnchor getLayoutAnchor() const override;
	virtual HAlignment getLayoutHAlignment() const override;
	virtual VAlignment getLayoutVAlignment() const override;
	virtual void getLayoutMinMaxInfo(Size* outMin, Size* outMax) const override;
	virtual ILayoutElement* getLayoutParent() const override;
	virtual const HAlignment* getLayoutContentHAlignment() override;
	virtual const VAlignment* getLayoutContentVAlignment() override;
	virtual const Size& getLayoutDesiredSize() const override;
	virtual void setLayoutDesiredSize(const Size& size) override;
	virtual void setLayoutFinalLocalRect(const Rect& rect) override;
	virtual const Rect& getLayoutFinalLocalRect() const override;
	virtual void setLayoutFinalGlobalRect(const Rect& rect) override;
	virtual int getVisualChildrenCount() const override;
	virtual ILayoutElement* getVisualChild(int index) const override;
	virtual int getLayoutColumn() const override;
	virtual int getLayoutRow() const override;
	virtual int getLayoutColumnSpan() const override;
	virtual int getLayoutRowSpan() const override;

	// ILayoutElement interface
	virtual Size measureOverride(const Size& constraint) override;

LN_CONSTRUCT_ACCESS:
	TextElement();
	virtual ~TextElement();
	void initialize();

LN_INTERNAL_ACCESS:
	DocumentsManager* getManager() const { return m_manager; }
	virtual InternalTextElementType getInternalTextElementType() const;
	void setParent(TextElement* parent) { m_parent = parent; }
	TextElement* getParent() const { return m_parent; }
	const Size& getDesiredSize() const { return m_desiredSize; }
	const Size& getRenderSize() const { return m_finalLocalRect.getSize(); }

private:
	DocumentsManager*		m_manager;
	FontData				m_fontData;
	RefPtr<Brush>			m_foreground;
	bool					m_fontDataModified;

	// layout data
	PointF					m_position;
	Size					m_size;
	ThicknessF				m_margin;
	ThicknessF				m_padding;
	AlignmentAnchor			m_anchor;
	HAlignment				m_horizontalAlignment;
	VAlignment				m_verticalAlignment;
	TextElement*			m_parent;
	Size					m_desiredSize;
	Rect					m_finalLocalRect;
	Rect					m_finalGlobalRect;
	detail::GridLayoutInfo	m_gridLayoutInfo;
};

/**
	@brief	コンテンツをグループ化して段落にするために使用される。
*/
class Block
	: public TextElement
{
public:
	Block();
	virtual ~Block();
	void initialize();

	void addInline(Inline* inl);
	void clearInlines();

	virtual void render(const Matrix& transform, IDocumentsRenderer* renderer);

protected:
	virtual Size measureOverride(const Size& constraint) override;
	virtual Size arrangeOverride(const Size& finalSize) override;

LN_INTERNAL_ACCESS:
	void insertInlines(int index, const List<RefPtr<Inline>>& inlines);
	const List<RefPtr<Inline>>& getInlines() const { return m_inlines; }

private:
	List<RefPtr<Inline>>	m_inlines;
};

/**
	@brief	コンテンツをグループ化して段落にするために使用される。
*/
class Paragraph
	: public Block
{
public:
	Paragraph();
	virtual ~Paragraph();
	void initialize();


private:
};

/**
	@brief
*/
class Inline
	: public TextElement
{
public:
	Inline();
	virtual ~Inline();
	void initialize();

private:
};

/**
	@brief
*/
class run
	: public Inline
{
public:
	run();
	virtual ~run();
	void initialize();
	void initialize(const UTF32* str, int len);

	void setText(const StringRef& text);

protected:
	// TextElement interface
	virtual void onFontDataChanged(const FontData& newData) override;
	virtual void render(const Matrix& transform, IDocumentsRenderer* renderer) override;

	// ILayoutElement interface
	virtual Size measureOverride(const Size& constraint);

private:
	GenericStringBuilderCore<UTF32>	m_text;
	RefPtr<GlyphRun>				m_glyphRun;
};

/**
	@brief
*/
class Span
	: public Inline
{
public:
	Span();
	virtual ~Span();
	void initialize();

private:
	// Inline List
};


/**
	@brief
*/
class LineBreak
	: public Inline
{
public:
	LineBreak();
	virtual ~LineBreak();
	void initialize();

private:
	virtual InternalTextElementType getInternalTextElementType() const;
};




class VisualTextElement
	: public Object
{
public:
	//TextElement*	m_element;
	int				m_documentLength;	// Document 上での TextLength

};



// 物理行
class VisualLine
	: public Object
{
public:

private:
public:	// TODO:
	List<RefPtr<VisualTextElement>>	m_visualTextElementList;
};


/**
	@brief
	@note	今は float(ブロックの横並び) などを考えない。とすると、Paragraph 単位でデータモデル組んだほうがわかりやすい。
*/
class VisualBlock
	: public Object
{
public:
	void setParagraph(Paragraph* paragraph);

private:
	void rebuildVisualLineList();

	RefPtr<Paragraph>			m_paragraph;
	List<RefPtr<VisualLine>>	m_visualLineList;
};

//
class DocumentView
	: public Object
{
public:

private:
	void initialize(Document* document);

	Document*	m_document;
};







} // namespace detail
LN_NAMESPACE_END
