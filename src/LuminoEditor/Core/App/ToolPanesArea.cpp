
#include "Application.hpp"
#include "ToolPanesArea.hpp"

namespace ln {
//==============================================================================
// EditorPane

EditorPane::EditorPane()
{
}

} // namespace ln

//==============================================================================
// ToolPanesArea

ToolPanesArea::ToolPanesArea()
{
}

void ToolPanesArea::init()
{
    UIControl::init();
    //setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 1));
}

void ToolPanesArea::addPane(ln::EditorPane* pane)
{
    addElement(pane);
}

