
#pragma once
#include "../UI/LayoutElement.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace detail { class DocumentsManager; }
namespace tr {
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
	void Initialize();

	// 現在の内容をすべて破棄して、新しい1つの Paragraph を作る
	void SetText(const StringRef& text);

LN_INTERNAL_ACCESS:
	void Replace(int offset, int length, const StringRef& text);

private:
	void Replace(int offset, int length, const UTF32* text, int len);

	ln::detail::DocumentsManager*		m_manager;
	List<RefPtr<Block>>				m_blockList;
};

/**
	@brief
*/
class TextElement
	: public Object
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
	void Initialize();

LN_INTERNAL_ACCESS:
	ln::detail::DocumentsManager* GetManager() const { return m_manager; }
	virtual InternalTextElementType GetInternalTextElementType() const;
	void SetParent(TextElement* parent) { m_parent = parent; }
	TextElement* GetParent() const { return m_parent; }

private:
	ln::detail::DocumentsManager*		m_manager;
	TextElement*						m_parent;
	ln::detail::FontData				m_fontData;
	RefPtr<Brush>			m_foreground;
	bool					m_fontDataModified;

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
	void Initialize();

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
	void Initialize();


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
	void Initialize();

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
	void Initialize();
	void Initialize(const UTF32* str, int len);

	void SetText(const StringRef& text);

protected:
	// TextElement interface
	virtual void OnFontDataChanged(const ln::detail::FontData& newData) override;

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
	void Initialize();

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
	void Initialize();

private:
	virtual InternalTextElementType GetInternalTextElementType() const;
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
	void SetParagraph(Paragraph* paragraph);

private:
	void RebuildVisualLineList();

	RefPtr<Paragraph>			m_paragraph;
	List<RefPtr<VisualLine>>	m_visualLineList;
};

//
class DocumentView
	: public Object
{
public:

private:
	void Initialize(Document* document);

	Document*	m_document;
};



} // namespace tr
LN_NAMESPACE_END
