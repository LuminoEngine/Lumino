/*
	・RichTextBox
	・アニメーション (スプライトテキスト)
	この2つは、表示するグリフは同じ。書き方が違うだけ。

*/
#include "../Internal.h"
#include <Lumino/GUI/GUIManager.h>
#include <Lumino/GUI/TextBlock.h>
#include <Lumino/Documents/Run.h>

namespace Lumino
{
namespace GUI
{

//=============================================================================
// TextBlock
//=============================================================================
LN_CORE_OBJECT_TYPE_INFO_IMPL(TextBlock, UIElement);
LN_UI_ELEMENT_SUBCLASS_IMPL(TextBlock);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextBlock::TextBlock(GUIManager* manager)
	: UIElement(manager)
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
void TextBlock::SetText(const String& text)
{
	m_paragraph->GetInlines()->Clear();

	auto run = RefPtr<Documents::Run>::Create(text, m_manager->GetDocumentsManager());
	m_paragraph->GetInlines()->Add(run);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF TextBlock::MeasureOverride(const SizeF& availableSize)
{
	Size s = m_paragraph->Measure();
	return UIElement::MeasureOverride(availableSize);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
SizeF TextBlock::ArrangeOverride(const SizeF& finalSize)
{
	return UIElement::ArrangeOverride(finalSize);
}

} // namespace GUI
} // namespace Lumino

