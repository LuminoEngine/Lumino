
#include "LuminoWidget.h"
#include "SpriteFramesetEditorDocumentView.h"

//==============================================================================
// SpriteFramesetEditorDocument

SpriteFramesetEditorDocument::SpriteFramesetEditorDocument(QObject* parent)
    : EditorDocument(parent)
{


}




//==============================================================================
// SpriteFramesetEditorView

SpriteFramesetEditorView::SpriteFramesetEditorView(QWidget* parent)
    : QWidget(parent)
{
    m_layout1 = new QVBoxLayout();
    m_luminoWidget = new LuminoWidget();
    m_layout1->addWidget(m_luminoWidget);

    setLayout(m_layout1);
}
