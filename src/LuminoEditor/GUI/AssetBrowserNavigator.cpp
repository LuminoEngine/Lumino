
#include <Workspace.hpp>
#include <Project.hpp>
#include "AssetBrowserNavigator.hpp"

//==============================================================================
// AssetBrowserTreeView

void AssetBrowserTreeView::onItemClick(ln::UITreeItem* item, ln::UIMouseEventArgs* e)
{
    UITreeView::onItemClick(item, e);
    if (e->getClickCount() == 2) {
    }
}

//==============================================================================
// AssetBrowserNavigator

ln::UIElement* AssetBrowserNavigator::createNavigationBarItem()
{
    m_navbarItem = ln::makeObject<ln::UIIcon>();
    m_navbarItem->setIconName(u"file");
    return m_navbarItem;
}

ln::UIElement* AssetBrowserNavigator::createView()
{
	auto project = lna::Workspace::instance()->project();

    auto model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    //model->setRootPath(project->assetsDir());
	model->setRootPath(u"C:/Proj/LN/Lumino");

    m_treeView = ln::makeObject<AssetBrowserTreeView>();
    m_treeView->setViewModel(model);
    m_treeView->setWidth(200);
    m_treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
    m_treeView->getGridLayoutInfo()->layoutRow = 0;

    return m_treeView;
}
