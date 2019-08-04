
#include <Workspace.hpp>
#include <Project.hpp>
#include <AssetDatabase.hpp>
#include "Application.hpp"
#include "AssetBrowserNavigator.hpp"

//==============================================================================
// AssetBrowserTreeView

void AssetBrowserTreeView::init()
{
    UITreeView::init();

    auto project = lna::Workspace::instance()->project();

    m_model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    m_model->setExcludeFilters(ln::makeList<ln::String>({u"*.lnasset"}));
    m_model->setRootPath(project->assetsDir());
    //model->setRootPath(u"D:/Proj/LN/Lumino");
    setViewModel(m_model);
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


    m_treeView = ln::makeObject<AssetBrowserTreeView>();
    m_treeView->setWidth(200);
    m_treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
    m_treeView->getGridLayoutInfo()->layoutRow = 0;

    return m_treeView;
}
