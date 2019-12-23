
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "AssetBrowserNavigator.hpp"

//==============================================================================
// AssetBrowserTreeViewModel

bool AssetBrowserTreeViewModel::onTestFilter(const ln::Path& path)
{
    return ln::FileSystem::existsDirectory(path);
}

    
//==============================================================================
// AssetBrowserTreeView

void AssetBrowserTreeView::init(AssetBrowserNavigatorExtension* owner)
{
    UITreeView::init();
    m_owner = owner;


    m_model = ln::makeObject<AssetBrowserTreeViewModel>();
    //m_model->setExcludeFilters(ln::makeList<ln::String>({u"*.lnasset"}));
    setViewModel(m_model);
}

void AssetBrowserTreeView::setPath(const ln::Path & path)
{
    m_model->setRootPath(path);
}

Ref<ln::UITreeItem> AssetBrowserTreeView::onRenderItem(ln::UICollectionItemModel* viewModel)
{
    return UITreeView::onRenderItem(viewModel);
    //auto item = UITreeView::onRenderItem(viewModel);

    //auto project = lna::Workspace::instance()->project();
    //if (project->assetDatabase()->isImportedAssetFile(m_model->filePath(viewModel))) {

    //}
    //else {
    //    item->setTextColor(ln::Color::Gray);
    //}

    //return item;
}

void AssetBrowserTreeView::onItemClick(ln::UITreeItem* item, ln::UIMouseEventArgs* e)
{
    UITreeView::onItemClick(item, e);
    auto path = m_model->filePath(static_cast<ln::UICollectionItemModel*>(item->m_viewModel.get()));
    m_owner->setAssetListPathFromTreeClick(path);

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
// AssetBrowserListViewModel

bool AssetBrowserListViewModel::onTestFilter(const ln::Path& path)
{
    // ignore folder and other files
    return lna::AssetDatabase::isAssetFile(path);
}

//==============================================================================
// AssetBrowserListView

void AssetBrowserListView::init(AssetBrowserNavigatorExtension* owner)
{
    UIListView::init();
    m_owner = owner;

    auto project = lna::Workspace::instance()->mainProject();

    m_model = ln::makeObject<AssetBrowserListViewModel>();
    setViewModel(m_model);
}

void AssetBrowserListView::setPath(const ln::Path& path)
{
    m_model->setRootPath(path);
    m_path = path;
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

	m_treeView = ln::makeObject<AssetBrowserTreeView>(this);
	m_treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
	m_treeView->getGridLayoutInfo()->layoutRow = 0;
    m_splitter->addElement(m_treeView);

    m_layout2 = ln::makeObject<ln::UIVBoxLayout2>();
    m_layout2->getGridLayoutInfo()->layoutRow = 1;
    m_splitter->addElement(m_layout2);

    m_importButton = ln::makeObject<ln::UIButton>();
    m_importButton->setText(u"Import");
    m_importButton->connectOnClicked(ln::bind(this, &AssetBrowserNavigatorExtension::onImport));
    m_layout2->addChild(m_importButton);

    m_listView = ln::makeObject<AssetBrowserListView>(this);
    m_listView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 3));
    m_treeView->getGridLayoutInfo()->layoutWeight = 1;
    m_layout2->addChild(m_listView);


    auto project = lna::Workspace::instance()->mainProject();
    m_treeView->setPath(project->assetsDir());
    m_listView->setPath(project->assetsDir());
}

void AssetBrowserNavigatorExtension::setAssetListPathFromTreeClick(const ln::Path& path)
{
    m_listView->setPath(path);
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

void AssetBrowserNavigatorExtension::onImport(ln::UIEventArgs* e)
{
    auto dlg = ln::PlatformOpenFileDialog::create();
    if (dlg->showDialog(EditorApplication::instance()->mainWindow()->platformWindow())) {
        auto src = dlg->getPath();
        auto dst = ln::Path(m_listView->path(), src.fileName());
        EditorApplication::instance()->workspace()->mainAssetDatabase()->importAsset(src, dst);
        m_listView->model()->refresh();
    }
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
