
#pragma once
#include <Lumino/Documents/Common.h>
#include <Lumino/UI/LayoutElement.h>
#include "../Graphics/GraphicsManager.h"

LN_NAMESPACE_BEGIN
namespace detail {
class DocumentsManager;
class Inline;

/**
	@brief
*/
class Document
	: public Object
{
public:
	Document();
	virtual ~Document();
	void Initialize(DocumentsManager* manager);

private:
	DocumentsManager*	m_manager;
};

/**
	@brief
*/
class TextElement
	: public Object
	, public ILayoutElement
{
public:
	TextElement();
	virtual ~TextElement();
	void Initialize(DocumentsManager* manager);

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


	virtual void Render(const Matrix& transform, IDocumentsRenderer* renderer);

protected:
	virtual void OnFontDataChanged(const FontData& newData);

	// ILayoutElement interface
	virtual const PointF& GetLayoutPosition() const override;
	virtual const SizeF& GetLayoutSize() const override;
	virtual const ThicknessF& GetLayoutMargin() const override;
	virtual const ThicknessF& GetLayoutPadding() const override;
	virtual AlignmentAnchor GetLayoutAnchor() const override;
	virtual HAlignment GetLayoutHAlignment() const override;
	virtual VAlignment GetLayoutVAlignment() const override;
	virtual ILayoutElement* GetLayoutParent() const override;
	virtual HAlignment* GetLayoutContentHAlignment() override;
	virtual VAlignment* GetLayoutContentVAlignment() override;
	virtual const SizeF& GetLayoutDesiredSize() const override;
	virtual void SetLayoutDesiredSize(const SizeF& size) override;
	virtual void SetLayoutFinalLocalRect(const RectF& rect) override;
	virtual int GetLayoutColumn() const override;
	virtual int GetLayoutRow() const override;
	virtual int GetLayoutColumnSpan() const override;
	virtual int GetLayoutRowSpan() const override;

LN_PROTECTED_INTERNAL_ACCESS:
	// ILayoutElement interface
	virtual SizeF MeasureOverride(const SizeF& constraint);

LN_INTERNAL_ACCESS:
	DocumentsManager* GetManager() const { return m_manager; }
	void SetParent(TextElement* parent) { m_parent = parent; }
	TextElement* GetParent() const { return m_parent; }
	const SizeF& GetDesiredSize() const { return m_desiredSize; }

private:
	DocumentsManager*		m_manager;
	FontData				m_fontData;
	bool					m_fontDataModified;

	PointF					m_position;
	SizeF					m_size;
	ThicknessF				m_margin;
	ThicknessF				m_padding;
	AlignmentAnchor			m_anchor;
	HAlignment				m_horizontalAlignment;
	VAlignment				m_verticalAlignment;
	TextElement*			m_parent;
	SizeF					m_desiredSize;
	RectF					m_finalLocalRect;
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
	void Initialize(DocumentsManager* manager);

	void AddChildElement(TextElement* inl);
	void ClearChildElements();

	virtual void Render(const Matrix& transform, IDocumentsRenderer* renderer);

protected:
	virtual SizeF MeasureOverride(const SizeF& constraint);
	virtual SizeF ArrangeOverride(const SizeF& finalSize);

private:
	Array<RefPtr<TextElement>>	m_childElements;
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
	void Initialize(DocumentsManager* manager);


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
	void Initialize(DocumentsManager* manager);

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
	void Initialize(DocumentsManager* manager);

	void SetText(const StringRef& text);

protected:
	// TextElement interface
	virtual void OnFontDataChanged(const FontData& newData) override;
	virtual void Render(const Matrix& transform, IDocumentsRenderer* renderer) override;

	// ILayoutElement interface
	virtual SizeF MeasureOverride(const SizeF& constraint);

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
	void Initialize(DocumentsManager* manager);

private:
	// Inline List
};

} // namespace detail
LN_NAMESPACE_END
