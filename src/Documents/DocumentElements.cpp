
#include "Internal.h"
#include <Lumino/Graphics/Text/GlyphRun.h>
#include "DocumentsManager.h"
#include "DocumentElements.h"

LN_NAMESPACE_BEGIN
namespace detail {

//==============================================================================
// Document
//==============================================================================

//------------------------------------------------------------------------------
Document::Document()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
Document::~Document()
{
}

//------------------------------------------------------------------------------
void Document::Initialize(DocumentsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
}


//==============================================================================
// TextElement
//==============================================================================

//------------------------------------------------------------------------------
TextElement::TextElement()
	: m_manager(nullptr)
	, m_fontDataModified(false)
	, m_position()
	, m_size()
	, m_margin()
	, m_padding()
	, m_anchor(AlignmentAnchor::None)
	, m_horizontalAlignment(HAlignment::Left)
	, m_verticalAlignment(VAlignment::Top)
	, m_parent(nullptr)
	, m_desiredSize()
	, m_finalLocalRect()
{
}

//------------------------------------------------------------------------------
TextElement::~TextElement()
{
}

//------------------------------------------------------------------------------
void TextElement::Initialize(DocumentsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_fontData.Family = String::GetEmpty();
	m_fontData.Size = 20;
	m_fontData.IsBold = false;
	m_fontData.IsItalic = false;
	m_fontData.IsAntiAlias = true;
	m_fontDataModified = true;
}

//------------------------------------------------------------------------------
void TextElement::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
void TextElement::Render(const Matrix& transform, IDocumentsRenderer* renderer)
{
}

//------------------------------------------------------------------------------
Size TextElement::MeasureOverride(const Size& constraint)
{
	if (m_fontDataModified)
	{
		OnFontDataChanged(m_fontData);
		m_fontDataModified = false;
	}
	return ILayoutElement::MeasureOverride(constraint);
}

//------------------------------------------------------------------------------
const PointF& TextElement::GetLayoutPosition() const { return m_position; }
const Size& TextElement::GetLayoutSize() const { return m_size; }
const ThicknessF& TextElement::GetLayoutMargin() const { return m_margin; }
const ThicknessF& TextElement::GetLayoutPadding() const { return m_padding; }
AlignmentAnchor TextElement::GetLayoutAnchor() const { return m_anchor; }
HAlignment TextElement::GetLayoutHAlignment() const { return m_horizontalAlignment; }
VAlignment TextElement::GetLayoutVAlignment() const { return m_verticalAlignment; }
ILayoutElement* TextElement::GetLayoutParent() const { return m_parent; }
const HAlignment* TextElement::GetLayoutContentHAlignment() { return nullptr; }
const VAlignment* TextElement::GetLayoutContentVAlignment() { return nullptr; }
const Size& TextElement::GetLayoutDesiredSize() const { return m_desiredSize; }
void TextElement::SetLayoutDesiredSize(const Size& size) { m_desiredSize = size; }
void TextElement::SetLayoutFinalLocalRect(const RectF& rect) { m_finalLocalRect = rect; }
int TextElement::GetLayoutColumn() const { return m_gridLayoutInfo.layoutColumn; }
int TextElement::GetLayoutRow() const { return m_gridLayoutInfo.layoutRow; }
int TextElement::GetLayoutColumnSpan() const { return m_gridLayoutInfo.layoutColumnSpan; }
int TextElement::GetLayoutRowSpan() const { return m_gridLayoutInfo.layoutRowSpan; }


//==============================================================================
// Block
//==============================================================================

//------------------------------------------------------------------------------
Block::Block()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Block::~Block()
{
}

//------------------------------------------------------------------------------
void Block::Initialize(DocumentsManager* manager)
{
	TextElement::Initialize(manager);
}

//------------------------------------------------------------------------------
void Block::AddChildElement(TextElement* inl)
{
	LN_CHECK_ARG(inl != nullptr);
	LN_CHECK_ARG(inl->GetParent() == nullptr);
	m_childElements.Add(inl);
	inl->SetParent(this);
}

//------------------------------------------------------------------------------
void Block::ClearChildElements()
{
	for (TextElement* child : m_childElements) child->SetParent(nullptr);
	m_childElements.Clear();
}

//------------------------------------------------------------------------------
void Block::Render(const Matrix& transform, IDocumentsRenderer* renderer)
{
	for (TextElement* child : m_childElements) child->Render(transform, renderer);
}

//------------------------------------------------------------------------------
Size Block::MeasureOverride(const Size& constraint)
{
	Size childDesirdSize;
	for (TextElement* child : m_childElements)
	{
		// TODO: とりあえず 左から右へのフロー
		Size size = child->MeasureOverride(constraint);
		childDesirdSize.width += size.width;
		childDesirdSize.height = std::max(childDesirdSize.height, size.height);
	}

	Size desirdSize = TextElement::MeasureOverride(constraint);
	return Size::Max(desirdSize, childDesirdSize);
}

//------------------------------------------------------------------------------
Size Block::ArrangeOverride(const Size& finalSize)
{
	float prevChildSize = 0;
	RectF childRect;
	for (TextElement* child : m_childElements)
	{
		// TODO: とりあえず 左から右へのフロー
		Size childDesiredSize = child->GetDesiredSize();
		childRect.x += prevChildSize;
		prevChildSize = childDesiredSize.width;
		childRect.width = prevChildSize;
		childRect.height = finalSize.height;
		child->ArrangeLayout(childRect);
	}

	return finalSize;
}

//==============================================================================
// Paragraph
//==============================================================================

//------------------------------------------------------------------------------
Paragraph::Paragraph()
{
}

//------------------------------------------------------------------------------
Paragraph::~Paragraph()
{
}

//------------------------------------------------------------------------------
void Paragraph::Initialize(DocumentsManager* manager)
{
	TextElement::Initialize(manager);
}


//==============================================================================
// Inline
//==============================================================================

//------------------------------------------------------------------------------
Inline::Inline()
	: TextElement()
{
}

//------------------------------------------------------------------------------
Inline::~Inline()
{
}

//------------------------------------------------------------------------------
void Inline::Initialize(DocumentsManager* manager)
{
	TextElement::Initialize(manager);
}


//==============================================================================
// Run
//==============================================================================

//------------------------------------------------------------------------------
Run::Run()
	: Inline()
{
}

//------------------------------------------------------------------------------
Run::~Run()
{
}

//------------------------------------------------------------------------------
void Run::Initialize(DocumentsManager* manager)
{
	Inline::Initialize(manager);

	// TODO: 本当に画面に表示されている分だけ作ればいろいろ節約できそう
	m_glyphRun = RefPtr<GlyphRun>::MakeRef();
	m_glyphRun->Initialize(manager->GetGraphicsManager());
}

//------------------------------------------------------------------------------
void Run::SetText(const StringRef& text)
{
	//m_text.Clear();
	//m_text.Append(GetManager()->GetTCharToUTF32Converter()->Convert(text.GetBegin(), text.GetLength()));
	m_glyphRun->SetText(text);
}

//------------------------------------------------------------------------------
void Run::OnFontDataChanged(const FontData& newData)
{
}

//------------------------------------------------------------------------------
Size Run::MeasureOverride(const Size& constraint)
{
	Size size = Inline::MeasureOverride(constraint);
	const SizeI& runSize = m_glyphRun->GetRenderSize();

	size.width = std::max(size.width, (float)runSize.width);
	size.height = std::max(size.height, (float)runSize.height);
	return size;
}

//------------------------------------------------------------------------------
void Run::Render(const Matrix& transform, IDocumentsRenderer* renderer)
{
	renderer->OnDrawGlyphRun(transform, m_glyphRun, PointI::Zero);
}

} // namespace detail
LN_NAMESPACE_END
