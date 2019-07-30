
#include "Application.hpp"
#include "ToolPanesArea.hpp"

//==============================================================================
// ToolPane

ToolPane::ToolPane()
{
}

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

void ToolPanesArea::addPane(ToolPane* pane)
{
    addElement(pane);
}

