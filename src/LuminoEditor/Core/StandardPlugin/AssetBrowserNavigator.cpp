
#include <Workspace.hpp>
#include <Project.hpp>
#include <AssetDatabase.hpp>
#include "../App/Application.hpp"
#include "AssetBrowserNavigator.hpp"

//==============================================================================
// AssetBrowserTreeView

void AssetBrowserTreeView::init()
{
    UITreeView::init();


    m_model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    m_model->setExcludeFilters(ln::makeList<ln::String>({u"*.lnasset"}));
    setViewModel(m_model);
}

void AssetBrowserTreeView::setPath(const ln::Path & path)
{
    m_model->setRootPath(path);
}

Ref<ln::UITreeItem> AssetBrowserTreeView::onRenderItem(ln::UICollectionItemModel* viewModel)
{
    auto item = UITreeView::onRenderItem(viewModel);

    auto project = lna::Workspace::instance()->project();
    if (project->assetDatabase()->isImportedAssetFile(m_model->filePath(viewModel))) {

    }
    else {
        item->setTextColor(ln::Color::Gray);
    }

    return item;
}

void AssetBrowserTreeView::onItemClick(ln::UITreeItem* item, ln::UIMouseEventArgs* e)
{
    UITreeView::onItemClick(item, e);

#if 0
    if (e->getClickCount() == 2) {

        auto path = m_model->filePath(static_cast<ln::UICollectionItemModel*>(item->m_viewModel.get()));
        auto project = lna::Workspace::instance()->project();
        if (project->assetDatabase()->isImportedAssetFile(path)) {
            EditorApplication::instance()->openAssetFile(path);
        }
        else {
            EditorApplication::instance()->importFile(path);
        }
    }
#endif
}

//==============================================================================
// AssetBrowserListView

void AssetBrowserListView::init()
{
    UIListView::init();

    auto project = lna::Workspace::instance()->project();

    m_model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    setViewModel(m_model);
}

void AssetBrowserListView::setPath(const ln::Path& path)
{
    m_model->setRootPath(path);
}

//==============================================================================
// AssetBrowserNavigatorExtension

void AssetBrowserNavigatorExtension::init()
{
    Object::init();
    m_navbarItemContent = ln::makeObject<ln::NavigationMenuItem>();
    m_navbarItemContent->setIconName(u"file");

    m_splitter = ln::makeObject<ln::UISplitter>();
    m_splitter->setOrientation(ln::Orientation::Vertical);
    m_splitter->setCellDefinition(0, ln::UILayoutLengthType::Ratio, 1);
    m_splitter->setCellDefinition(1, ln::UILayoutLengthType::Ratio, 1);

	m_treeView = ln::makeObject<AssetBrowserTreeView>();
	m_treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
	m_treeView->getGridLayoutInfo()->layoutRow = 0;
    m_splitter->addElement(m_treeView);

    m_listView = ln::makeObject<AssetBrowserListView>();
    m_listView->getGridLayoutInfo()->layoutRow = 1;
    m_listView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 3));
    m_splitter->addElement(m_listView);


    auto project = lna::Workspace::instance()->project();
    m_treeView->setPath(project->assetsDir());
    m_listView->setPath(project->assetsDir());
}

void AssetBrowserNavigatorExtension::onAttached()
{
}

void AssetBrowserNavigatorExtension::onDetached()
{
}

ln::NavigationMenuItem* AssetBrowserNavigatorExtension::getNavigationMenuItem()
{
    return m_navbarItemContent;
}

ln::UIElement* AssetBrowserNavigatorExtension::getNavigationPane()
{
	return m_splitter;
}

//
////==============================================================================
//// AssetBrowserNavigator
//
//ln::UIElement* AssetBrowserNavigator::createNavigationBarItem()
//{
//    m_navbarItem = ln::makeObject<ln::UIIcon>();
//    m_navbarItem->setIconName(u"file");
//    return m_navbarItem;
//}
//
//ln::UIElement* AssetBrowserNavigator::createView()
//{
//
//
//    m_treeView = ln::makeObject<AssetBrowserTreeView>();
//    m_treeView->setWidth(200);
//    m_treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
//    m_treeView->getGridLayoutInfo()->layoutRow = 0;
//
//    return m_treeView;
//}
