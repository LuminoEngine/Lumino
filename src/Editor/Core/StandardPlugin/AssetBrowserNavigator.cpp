
#include <LuminoPlatform/PlatformWindow.hpp>
#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "../App/DocumentManager.hpp"
#include "AssetBrowserNavigator.hpp"
#include "LevelEditor.hpp"  // TODO:

namespace lna {

//==============================================================================
// AssetBrowserTreeViewModel

bool AssetBrowserTreeViewModel::onTestFilter(const ln::Path& path)
{
    return ln::FileSystem::existsDirectory(path);
}

#if 0
//==============================================================================
// AssetBrowserTreeView

void AssetBrowserTreeView::init(AssetBrowserNavigatorExtension* owner)
{
    UITreeView::init();
    m_owner = owner;


    m_model = ln::makeObject_deprecated<AssetBrowserTreeViewModel>();
    //m_model->setExcludeFilters(ln::makeList<ln::String>({_TT("*.lnasset"}));
    setViewModel(m_model);
}

void AssetBrowserTreeView::setPath(const ln::Path & path)
{
    m_model->setRootPath(path);
}

Ref<ln::UITreeItem> AssetBrowserTreeView::onRenderItem(ln::UICollectionItemViewModel* viewModel)
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
    auto path = m_model->filePath(static_cast<ln::UICollectionItemViewModel*>(item->m_viewModel.get()));
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

    m_model = ln::makeObject_deprecated<AssetBrowserListViewModel>();
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
    m_navbarItemContent = ln::makeObject_deprecated<ln::NavigationMenuItem>();
    m_navbarItemContent->setIconName(_TT("file");

    m_splitter = ln::makeObject_deprecated<ln::UISplitter>();
    m_splitter->setOrientation(ln::UILayoutOrientation::Vertical);
    m_splitter->setCellDefinition(0, ln::UILayoutLengthType::Ratio, 1);
    m_splitter->setCellDefinition(1, ln::UILayoutLengthType::Ratio, 1);

	m_treeView = ln::makeObject_deprecated<AssetBrowserTreeView>(this);
	m_treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
	m_treeView->getGridLayoutInfo()->layoutRow = 0;
    m_splitter->addElement(m_treeView);

    m_layout2 = ln::makeObject_deprecated<ln::UIVBoxLayout2>();
    m_layout2->getGridLayoutInfo()->layoutRow = 1;
    m_splitter->addElement(m_layout2);

    m_importButton = ln::makeObject_deprecated<ln::UIButton>();
    m_importButton->setText(_TT("Import");
    m_importButton->connectOnClicked(ln::bind(this, &AssetBrowserNavigatorExtension::onImport));
    m_layout2->addChild(m_importButton);

    m_listView = ln::makeObject_deprecated<AssetBrowserListView>(this);
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

void AssetBrowserNavigatorExtension::onImport()
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
//    m_navbarItem = ln::makeObject_deprecated<ln::UIIcon>();
//    m_navbarItem->setIconName(_TT("file");
//    return m_navbarItem;
//}
//
//ln::UIElement* AssetBrowserNavigator::createView()
//{
//
//
//    m_treeView = ln::makeObject_deprecated<AssetBrowserTreeView>();
//    m_treeView->setWidth(200);
//    m_treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
//    m_treeView->getGridLayoutInfo()->layoutRow = 0;
//
//    return m_treeView;
//}

#endif


//==============================================================================
// AssetBrowserPane

bool AssetBrowserPane::init(lna::EditorContext* context)
{
    if (!NavigatorContentPane::init()) return false;
    Project* project = context->mainProject();
    DocumentManager* documentManager = context->documentManager();

    auto mainLauout = ln::makeObject_deprecated<ln::UIGridLayout>();
    mainLauout->setColumnCount(4);
    addChild(mainLauout);

    auto model1 = ln::makeObject_deprecated<ln::UIFileSystemCollectionModel>();
    model1->setRootPath(project->assetsDir());

    auto treeview1 = ln::makeObject_deprecated<ln::UITreeView2>();
    treeview1->connectOnChecked([model1](ln::UIEventArgs* e) {
        auto* item = static_cast<ln::UITreeItem2*>(e->sender());
        auto path = model1->filePath(ln::static_pointer_cast<ln::UICollectionItemViewModel>(item->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    });
    treeview1->setGenerateTreeItemHandler([documentManager, model1](ln::UITreeItem2* item) {

        // TODO: とりいそぎ LevelEditor に追加したい臨時ボタン
        auto button = ln::UIButton::create(_TT(">"));
        button->setAlignments(ln::UIAlignment::Right);
        button->setMargin(1);
        item->addChild(button);

        button->connectOnClicked([documentManager, model1, item]() {
            if (auto d = dynamic_cast<LevelEditor*>(documentManager->activeDocument()->editor().get())) {
                auto path = model1->filePath(ln::static_pointer_cast<ln::UICollectionItemViewModel>(item->m_viewModel));
                d->tryInstantiateObjectFromAnyFile(path);
            }
        });
    });

    treeview1->setViewModel(model1);
    mainLauout->addChild(treeview1);

    return true;
}

//==============================================================================
// AssetBrowserNavigator

bool AssetBrowserNavigator::init(lna::EditorContext* context)
{
    if (!Navigator::init()) return false;

    m_navigationItem = ln::makeObject_deprecated<ln::UIIcon>();
    m_navigationItem->setIconName(_TT("file"));
    m_navigationItem->setAlignments(ln::UIAlignment::Center);
    m_navigationItem->setFontSize(24);

    m_mainPane = ln::makeObject_deprecated<AssetBrowserPane>(context);
    //m_mainPane->setBackgroundColor(ln::Color::Red);

    return true;
}

ln::UIElement* AssetBrowserNavigator::getNavigationMenuItem()
{
    return m_navigationItem;
}

ln::UIElement* AssetBrowserNavigator::getNavigationPane()
{
    return m_mainPane;
}

} // namespace lna
