
#include "Document.h"
#include "LuminoWidget.h"

//==============================================================================
// EditorDocument

EditorDocument::EditorDocument(QObject* parent)
	: QObject(parent)
{
}

//==============================================================================
// EditorView

EditorView::EditorView(QWidget* parent)
    : QFrame(parent)
{
    setStyleSheet("border: 1px solid red");
}

//==============================================================================
// AssetEditorDocument

AssetEditorDocument::AssetEditorDocument(ln::AssetModel* asset, ln::AssetEditorViewModel* editor, QObject* parent)
    : EditorDocument(parent)
    , m_asset(asset)
    , m_editor(editor)
{


}


//==============================================================================
// AssetEditorView

AssetEditorView::AssetEditorView(AssetEditorDocument* document, QWidget* parent)
    : EditorView(parent)
    , m_document(document)
{
    m_rootLayout = new QVBoxLayout();

    if (m_document->editor()->viewType() == ln::AssetEditorViewType::Scene) {
        m_luminoWidget = new LuminoWidget();
        m_rootLayout->addWidget(m_luminoWidget);
        m_document->editor()->onOpened(m_document->asset(), m_luminoWidget->frameWindow());
    }

    setLayout(m_rootLayout);
}