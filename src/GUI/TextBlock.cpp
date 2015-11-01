/*
	・RichTextBox
	・アニメーション (スプライトテキスト)
	この2つは、表示するグリフは同じ。書き方が違うだけ。

*/
#include "Internal.h"
#include "UIManagerImpl.h"
#include <Lumino/GUI/Control.h>
#include <Lumino/GUI/TextBlock.h>
#include <Lumino/Documents/DocumentsRenderer.h>
#include <Lumino/Documents/Run.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GUI_BEGIN

//=============================================================================
// TextBlock
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextBlock, UITextElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(TextBlock);

LN_PROPERTY_IMPLEMENT(TextBlock, String, TextProperty, "Text", m_text, PropertyMetadata(String::GetEmpty(), &TextBlock::OnTextPropertyChanged));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock* TextBlock::Create(UIElement* parent, const String& text)
{
	auto* obj = internalCreateInstance(GetUIManager());
	AutoReleasePool::AddObjectToCurrent(obj);
	if (parent != nullptr) {
		parent->AddChild(obj);
	}
	obj->SetText(text);
	return obj;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock::TextBlock(GUIManagerImpl* manager)
	: UITextElement(manager)
{
	m_paragraph = RefPtr<Documents::Paragraph>::Create(m_manager->GetDocumentsManager());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock::~TextBlock()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void TextBlock::SetText(const String& text)
//{
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF TextBlock::MeasureOverride(const SizeF& availableSize)
{
	SizeF size = UITextElement::MeasureOverride(availableSize);
	Size parSize = m_paragraph->Measure();

	size.Width = (float)std::max(parSize.Width, parSize.Width);
	size.Height = (float)std::max(parSize.Height, parSize.Height);
	return size;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF TextBlock::ArrangeOverride(const SizeF& finalSize)
{
	return UITextElement::ArrangeOverride(finalSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBlock::OnRender(RenderingContext* painter)
{
	Brush* brush = GetForeground();
	painter->SetBrush(brush);
	//m_document->Render(painter);

	UITextElement::OnRender(painter);
	Documents::RenderTargetDocumentsRenderer r(painter);
	m_paragraph->Render(&r);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBlock::OnUpdateFont(const GraphicsManager::FontData& fontData)
{
	if (m_paragraph != NULL)
	{
		if (HasLocalPropertyValue(FontFamilyProperty)) { m_paragraph->SetFontFamily(fontData.Family); }
		if (HasLocalPropertyValue(FontSizeProperty)) { m_paragraph->SetFontSize(fontData.Size); }
		if (HasLocalPropertyValue(IsFontBoldProperty)) { m_paragraph->SetFontBold(fontData.IsBold); }
		if (HasLocalPropertyValue(IsFontItalicProperty)) { m_paragraph->SetFontItalic(fontData.IsItalic); }
		if (HasLocalPropertyValue(IsFontAntiAliasProperty)) { m_paragraph->SetFontAntiAlias(fontData.IsAntiAlias); }
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextBlock::OnTextPropertyChanged(PropertyChangedEventArgs* e)
{
	m_paragraph->GetInlines()->Clear();
	RefPtr<Documents::Run> run(LN_NEW Documents::Run(m_text, m_manager->GetDocumentsManager()), false);
	m_paragraph->GetInlines()->Add(run);
}

LN_NAMESPACE_GUI_END
LN_NAMESPACE_END

