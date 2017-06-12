
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


	void SetThisRevision(int rev) { m_thisRevision = rev; }
	void SetChildrenRevision(int rev) { m_childrenRevision = rev; }

LN_INTERNAL_ACCESS:
	void SetParentContent(DocumentContentElement* parent) { m_parent = parent; IncreaseRevision(); }
	int GetThisRevision() const { return m_thisRevision; }
	int GetChildrenRevision() const { return m_childrenRevision; }
	void IncreaseRevision() { m_thisRevision++; if (m_parent != nullptr) { m_parent->m_childrenRevision++; } }

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
	void SetText(const StringRef& text);

LN_INTERNAL_ACCESS:
	void Replace(int offset, int length, const StringRef& text);
	const List<RefPtr<Block>>& GetBlocks() const { return m_blockList; }

private:
	void ReplaceInternal(int offset, int length, const UTF32* text, int len);

	ln::detail::DocumentsManager*	m_manager;
	List<RefPtr<Block>>				m_blockList;
};

/**
	@brief
*/
class TextElement
	: public DocumentContentElement
{
public:

	/** フォントファミリ名を設定します。*/
	void SetFontFamily(const StringRef& value) { m_fontData.Family = value; m_fontDataModified = true; }

	/** フォントファミリ名を取得します。*/
	const String& GetFontFamily() const { return m_fontData.Family; }

	/** フォントサイズを設定します。*/
	void SetFontSize(int value) { m_fontData.Size = value; m_fontDataModified = true; }

	/** フォントサイズを取得します。*/
	int GetFontSize() const { return m_fontData.Size; }

	/** フォントの太字有無を設定します。*/
	void SetFontBold(bool value) { m_fontData.IsBold = value; m_fontDataModified = true; }

	/** フォントの太字有無を取得します。*/
	bool IsFontBold() const { return m_fontData.IsBold; }

	/** フォントのイタリック体有無を設定します。*/
	void SetFontItalic(bool value) { m_fontData.IsItalic = value; m_fontDataModified = true; }

	/** フォントのイタリック体有無を取得します。*/
	bool IsFontItalic() const { return m_fontData.IsItalic; }

	/** フォントのアンチエイリアス有無を設定します。*/
	void SetFontAntiAlias(bool value) { m_fontData.IsAntiAlias = value; m_fontDataModified = true; }

	/** フォントのアンチエイリアス有無を取得します。*/
	bool IsFontAntiAlias() const { return m_fontData.IsAntiAlias; }

	Brush* GetForeground() const;

LN_PROTECTED_INTERNAL_ACCESS:
	virtual void OnFontDataChanged(const ln::detail::FontData& newData);


LN_CONSTRUCT_ACCESS:
	TextElement();
	virtual ~TextElement();
	void initialize();

LN_INTERNAL_ACCESS:
	ln::detail::DocumentsManager* GetManager() const { return m_manager; }
	virtual InternalTextElementType GetInternalTextElementType() const;
	//void SetParent(TextElement* parent) { m_parent = parent; }
	//TextElement* GetParent() const { return m_parent; }
	//int GetThisRevision() const { return m_thisRevision; }
	//int GetChildrenRevision() const { return m_childrenRevision; }
	bool IsDeleted() const { return m_deleted; }

private:
	ln::detail::DocumentsManager*		m_manager;
	//TextElement*						m_parent;
	ln::detail::FontData				m_fontData;
	RefPtr<Brush>			m_foreground;
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

	void AddInline(Inline* inl);
	void ClearInlines();

protected:

LN_INTERNAL_ACCESS:
	void InsertInlines(int index, const List<RefPtr<Inline>>& inlines);
	const List<RefPtr<Inline>>& GetInlines() const { return m_inlines; }

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
class Run
	: public Inline
{
public:
	Run();
	virtual ~Run();
	void initialize();
	void initialize(const UTF32* str, int len);

	void SetText(const StringRef& text);

protected:
	// TextElement interface
	virtual void OnFontDataChanged(const ln::detail::FontData& newData) override;
	virtual InternalTextElementType GetInternalTextElementType() const;

LN_INTERNAL_ACCESS:
	const UTF32* GetText() const { return m_text.c_str(); }
	int GetLength() const { return m_text.GetLength(); }

private:
	GenericStringBuilderCore<UTF32>	m_text;
	//RefPtr<GlyphRun>				m_glyphRun;
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
	virtual InternalTextElementType GetInternalTextElementType() const;
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
//	//void MeasureLayout(const Size& availableSize);
//	//void ArrangeLayout(const Rect& finalLocalRect);
//	//const Size& GetDesiredSize() const { return m_desiredSize; }
//
//	void Render(DrawList* renderer);
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
	RefPtr<GlyphRun>	m_glyphRun;

LN_CONSTRUCT_ACCESS:
	VisualTextFragment();
	virtual ~VisualTextFragment();
	void initialize();

LN_INTERNAL_ACCESS:
	//void MeasureLayout(const Size& availableSize);
	//void ArrangeLayout(const Rect& finalLocalRect);
	//const Size& GetDesiredSize() const { return m_desiredSize; }

	void Render(DrawList* renderer);

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
	void MeasureLayout(const Size& availableSize, VisualBlock* rootBlock);
	//void ArrangeLayout(const Rect& finalLocalRect);
	void Render(const Matrix& transform, ln::detail::IDocumentsRenderer* renderer);

private:
	RefPtr<Inline>	m_inline;

	//TextElement*	m_element;
	//int				m_documentLength;	// Document 上での TextLength


};


// Block の Visual 要素
class VisualBlock
	: public VisualTextElement
{
public:
	void SetBlock(Block* block);

	bool IsModelDeleted() const;

LN_CONSTRUCT_ACCESS:
	VisualBlock();
	virtual ~VisualBlock();
	void initialize(Block* block);

LN_INTERNAL_ACCESS:
	void MeasureLayout(const Size& availableSize);
	void ArrangeLayout(const Rect& finalLocalRect);
	void Render(DrawList* renderer);
	void AddVisualFragment(VisualTextFragment* glyph) { m_visualFragments.Add(glyph); }

private:
	void RebuildVisualLineList();

	RefPtr<Block>						m_block;
	List<RefPtr<VisualInline>>			m_visualInlines;
	List<RefPtr<VisualTextFragment>>	m_visualFragments;
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
	void MeasureLayout(const Size& availableSize);
	void ArrangeLayout(const Rect& finalLocalRect);
	void Render(DrawList* renderer);

private:
	Document*					m_document;
	//int							m_thisRevision;
	//int							m_childrenRevision;
	List<RefPtr<VisualBlock>>	m_visualBlocks;
};



} // namespace tr
LN_NAMESPACE_END
