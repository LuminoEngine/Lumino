
#pragma once
#include "../UI/LayoutElement.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace detail { class DocumentsManager; }
class DrawList;

namespace tr {
class Inline;
class Block;

enum class InternalTextElementType
{
	Common,
	TextRun,
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


class DocumentContentElement
	: public Object
{
protected:
	DocumentContentElement()
		: m_parent(nullptr)
		, m_thisRevision(0)
		, m_childrenRevision(0)
	{}


	void setThisRevision(int rev) { m_thisRevision = rev; }
	void setChildrenRevision(int rev) { m_childrenRevision = rev; }

LN_INTERNAL_ACCESS:
	void setParentContent(DocumentContentElement* parent) { m_parent = parent; increaseRevision(); }
	int getThisRevision() const { return m_thisRevision; }
	int getChildrenRevision() const { return m_childrenRevision; }
	void increaseRevision() { m_thisRevision++; if (m_parent != nullptr) { m_parent->m_childrenRevision++; } }

private:
	DocumentContentElement*		m_parent;
	int							m_thisRevision;
	int							m_childrenRevision;
};



/**
	@brief
*/
class Document
	: public DocumentContentElement
{
public:
	Document();
	virtual ~Document();
	void initialize();

	// 現在の内容をすべて破棄して、新しい1つの Paragraph を作る
	void setText(const StringRef& text);

LN_INTERNAL_ACCESS:
	void replace(int offset, int length, const StringRef& text);
	const List<Ref<Block>>& getBlocks() const { return m_blockList; }

private:
	void replaceInternal(int offset, int length, const UTF32* text, int len);

	ln::detail::DocumentsManager*	m_manager;
	List<Ref<Block>>				m_blockList;
};

/**
	@brief
*/
class TextElement
	: public DocumentContentElement
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
	void setFontBold(bool value) { m_fontData.isBold = value; m_fontDataModified = true; }

	/** フォントの太字有無を取得します。*/
	bool isFontBold() const { return m_fontData.isBold; }

	/** フォントのイタリック体有無を設定します。*/
	void setFontItalic(bool value) { m_fontData.isItalic = value; m_fontDataModified = true; }

	/** フォントのイタリック体有無を取得します。*/
	bool isFontItalic() const { return m_fontData.isItalic; }

	/** フォントのアンチエイリアス有無を設定します。*/
	void setFontAntiAlias(bool value) { m_fontData.isAntiAlias = value; m_fontDataModified = true; }

	/** フォントのアンチエイリアス有無を取得します。*/
	bool isFontAntiAlias() const { return m_fontData.isAntiAlias; }

	Brush* getForeground() const;

LN_PROTECTED_INTERNAL_ACCESS:
	virtual void onFontDataChanged(const ln::detail::FontData& newData);


LN_CONSTRUCT_ACCESS:
	TextElement();
	virtual ~TextElement();
	void initialize();

LN_INTERNAL_ACCESS:
	ln::detail::DocumentsManager* getManager() const { return m_manager; }
	virtual InternalTextElementType getInternalTextElementType() const;
	//void setParent(TextElement* parent) { m_parent = parent; }
	//TextElement* GetParent() const { return m_parent; }
	//int getThisRevision() const { return m_thisRevision; }
	//int getChildrenRevision() const { return m_childrenRevision; }
	bool isDeleted() const { return m_deleted; }

private:
	ln::detail::DocumentsManager*		m_manager;
	//TextElement*						m_parent;
	ln::detail::FontData				m_fontData;
	Ref<Brush>			m_foreground;
	bool					m_fontDataModified;


	//int								m_thisRevision;
	//int								m_childrenRevision;

	bool					m_deleted;
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

protected:

LN_INTERNAL_ACCESS:
	void insertInlines(int index, const List<Ref<Inline>>& inlines);
	const List<Ref<Inline>>& getInlines() const { return m_inlines; }

private:
	List<Ref<Inline>>	m_inlines;
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
class Run
	: public Inline
{
public:
	Run();
	virtual ~Run();
	void initialize();
	void initialize(const UTF32* str, int len);

	void setText(const StringRef& text);

protected:
	// TextElement interface
	virtual void onFontDataChanged(const ln::detail::FontData& newData) override;
	virtual InternalTextElementType getInternalTextElementType() const;

LN_INTERNAL_ACCESS:
	const UTF32* getText() const { return m_text.c_str(); }
	int getLength() const { return m_text.getLength(); }

private:
	ln::detail::SimpleStringBuilder<UTF32>	m_text;
	//Ref<GlyphRun>				m_glyphRun;
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








class VisualBlock;



// 1文字分 の Visual 要素
//class VisualGlyph
//	: public Object
//{
//public:
//
//	Rect	m_localRect;
//
//LN_CONSTRUCT_ACCESS:
//	VisualGlyph();
//	virtual ~VisualGlyph();
//	void initialize();
//
//LN_INTERNAL_ACCESS:
//	//void measureLayout(const Size& availableSize);
//	//void arrangeLayout(const Rect& finalLocalRect);
//	//const Size& getDesiredSize() const { return m_desiredSize; }
//
//	void render(DrawList* renderer);
//
//private:
//	//Size	m_desiredSize;
//};



// type : text, image...
class VisualTextFragment
	: public Object
{
public:

	Rect	m_localRect;
	Ref<GlyphRun>	m_glyphRun;

LN_CONSTRUCT_ACCESS:
	VisualTextFragment();
	virtual ~VisualTextFragment();
	void initialize();

LN_INTERNAL_ACCESS:
	//void measureLayout(const Size& availableSize);
	//void arrangeLayout(const Rect& finalLocalRect);
	//const Size& getDesiredSize() const { return m_desiredSize; }

	void render(DrawList* renderer);

private:
	//Size	m_desiredSize;
};


// Visual 要素
class VisualTextElement
	: public DocumentContentElement
{
public:

protected:
	VisualTextElement();
	virtual ~VisualTextElement();

	//int							m_thisRevision;
	//int							m_childrenRevision;

};

// Inline の Visual 要素
class VisualInline
	: public VisualTextElement
{
public:

LN_CONSTRUCT_ACCESS:
	VisualInline();
	virtual ~VisualInline();
	void initialize(Inline* inl);

LN_INTERNAL_ACCESS:
	void measureLayout(const Size& availableSize, VisualBlock* rootBlock);
	//void arrangeLayout(const Rect& finalLocalRect);
	void render(const Matrix& transform, ln::detail::IDocumentsRenderer* renderer);

private:
	Ref<Inline>	m_inline;

	//TextElement*	m_element;
	//int				m_documentLength;	// Document 上での TextLength


};


// Block の Visual 要素
class VisualBlock
	: public VisualTextElement
{
public:
	void setBlock(Block* block);

	bool isModelDeleted() const;

LN_CONSTRUCT_ACCESS:
	VisualBlock();
	virtual ~VisualBlock();
	void initialize(Block* block);

LN_INTERNAL_ACCESS:
	void measureLayout(const Size& availableSize);
	void arrangeLayout(const Rect& finalLocalRect);
	void render(DrawList* renderer);
	void addVisualFragment(VisualTextFragment* glyph) { m_visualFragments.add(glyph); }

private:
	void rebuildVisualLineList();

	Ref<Block>						m_block;
	List<Ref<VisualInline>>			m_visualInlines;
	List<Ref<VisualTextFragment>>	m_visualFragments;
};

//
class DocumentView
	: public DocumentContentElement
{
public:

LN_CONSTRUCT_ACCESS:
	DocumentView();
	virtual ~DocumentView();
	void initialize(Document* document);

LN_INTERNAL_ACCESS:
	void measureLayout(const Size& availableSize);
	void arrangeLayout(const Rect& finalLocalRect);
	void render(DrawList* renderer);

private:
	Document*					m_document;
	//int							m_thisRevision;
	//int							m_childrenRevision;
	List<Ref<VisualBlock>>	m_visualBlocks;
};



} // namespace tr
LN_NAMESPACE_END
