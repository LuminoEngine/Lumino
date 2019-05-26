
#include "MainWindow.h"
#include "SceneContentsView.h"

//==============================================================================
// SceneContentsViewProvider

SceneContentsViewProvider::SceneContentsViewProvider()
    : m_treeView(nullptr)
{

}

QWidget* SceneContentsViewProvider::createView()
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}
