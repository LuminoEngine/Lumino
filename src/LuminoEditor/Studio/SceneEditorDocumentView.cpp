
#include "LuminoWidget.h"
#include "SceneEditorDocumentView.h"

//==============================================================================
// SceneEditorDocument

SceneEditorDocument::SceneEditorDocument(QObject* parent)
    : EditorDocument(parent)
{


}


//==============================================================================
// SceneEditorView

SceneEditorView::SceneEditorView(QWidget* parent)
    : EditorView(parent)
{
    m_layout1 = new QVBoxLayout();
    m_luminoWidget = new LuminoWidget();
    m_layout1->addWidget(m_luminoWidget);

    setLayout(m_layout1);
}
