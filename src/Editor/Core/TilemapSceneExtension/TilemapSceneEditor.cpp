/*

[2019/8/13] Panes
----------

### モードPane
UE4の mode pane と同じイメージ。
ただ、プレハブのブラウザも兼ねる。

モードはいまのところ
・タイルマップ編集モード
・オブジェクトモード

タイルマップ編集モードでは、ここにタイルセットが表示される。

オブジェクトモードでは、各種オブジェクトやプレハブを選択し、シーンに配置できる。D&Dでいいかな。まぁ、細かいことはあとで。


### レイヤーPane
Sceneのレイヤーを表す。TkoolXPの時のような、タイルマップのレイヤーではない。
実際にはレイヤーの間にオブジェクト挟んだり、タイルの単なる3次元配列のエディタにはしないほうがいいと思う。
→ でも謎解きぷらぐいんとか作ったときはこの仕組みがあったほうがよかったんだよな・・。
→ よく考えたらやっぱり必要かも。特にタイルを「装飾」するために別のタイルを置きたい場合。
    ただ、シーンのレイヤーではなく tilemap の内部レイヤーという特殊な位置づけで。

レイヤー自体はシーンのルートオブジェクト。
Tilemap レイヤーが選択されたら、そのまますぐ Tilemap の編集モードに入る。
Tilemap のオブジェクトモードに戻ったりすると、Tilemap 自体のTransform とかいじれる。




[2019/8/13] レイヤーについて
----------
そもそもレイヤーというデータ構造は必要？何がしたいの？ゲームエンジンにおいてレイヤーはグルーピングの意味の方が強い。
ペイントソフトでは可視設定したり下層のイメージにフィルタ付けたりに使うけど？
・編集範囲の制御・・・タイルマップレイヤー:Tilemapだけ変種するモード   オブジェクトレイヤー:オブジェクトの配置をするモード
・前後関係の制御・・・描画順ではなく前後関係。なので、UE4 や Office のグループ化の意味が強い。

タイルマップの下に子オブジェクトを置いたりしたくない？
…したいかも。

そうすると、アウトライナで選んだオブジェクトによって編集モード変えるほうがいい？
・・・いや、Tilemap 自体にも、オブジェクトモードと編集モードの任意切り替えが欲しい・・・。

### 具体的にどんなストーリーで編集したいの？
Tiled かな。
そうなると、Tilemap ごとに編集モード切替、というより、シーン全体としての、2D編集モードがやっぱりほしい。

そして、複数層のTilemapを扱うことは日常なので、編集対象となるオブジェクトの選択としてグループ化を使いたい。それにレイヤー的な考えを入れたい。
→ UE4 では、この役目は「レイヤー」https://api.unrealengine.com/JPN/Engine/UI/LevelEditor/Layers/index.html


### Sceneエディタの基本機能として、排他的な編集ロックがあるといいのかな？
あるオブジェクトの子ツリーだけ編集できるような仕組み。
当初想定していた編集レイヤーの選択とはつまり、この機能で目的のツリー以外の編集ロックをかけるということ。
それに合わせて、ロックされたオブジェクトは半透明とかにしてもいいかも。

###「アウトラインレイヤー」みたいな名前にしておこうか
これの実態はヒエラルキー最上位のノードである。

### UE4 のモード切替のイメージが近い？
でも、複数ランドスケープを扱うとき、レイヤーみたいに 2D 的な前後関係をアウトライナで調整する必要があるので、
当初イメージしていたレイヤー的な使い方とはちょっと違う…。

### アプローチの乖離がひどいので見送り…のまえに、3Dエンジン側からのアプローチをちゃんと整理してみよう
・・・と言っても、UE4 と同じようになるかな。
でも、２層のtilemapの間に挟まれたオブジェクトを選択したいときに、シーンビュー上からは選択できないのがかなりストレスかも。（透明部分はmouseHitTestしない、でもいいけど）

やはり、シーンの構造は共通だけどアプローチの異なる2つのシーンエディタはあったほうがいい気がする。


[2019/8/13] タイルマップ編集モードと、イベント(オブジェクト) 編集モードを分ける？
----------
分けたほうがいいと思う。

PGMMV では統合されてたけど、大きなオブジェクトがあるときに、その後ろのタイルを選択できない。
※後で見直してみたら、編集モードがものすごく分かりづらいだけだった…。




*/

#include <Workspace.hpp>
#include "../Project/Project.hpp"
#include "../Project/AssetDatabase.hpp"
#include "../Project/PluginManager.hpp"
#include "../UIExtension.hpp"
#include "../App/Application.hpp"
#include "../App/MainWindow.hpp"
#include "TilemapSceneNavigator.hpp"
#include "TilemapSceneEditorModel.hpp"
#include "TilemapSceneEditor.hpp"
#include "TilemapSceneModePane.hpp"


namespace lna {
   
//==============================================================================
// TilemapSceneEditor

ln::Result TilemapSceneEditor::init()
{
    AssetEditorModel::init();
	m_model = ln::makeObject<TilemapSceneEditorModel>();

    m_modePane = ln::makeObject<TilemapSceneModePane>(m_model);
    m_inspectorPane = ln::makeObject<ln::EditorPane>();
    m_inspectorPane->setBackgroundColor(ln::Color::LightGray);

    m_modePanes = ln::makeList<Ref<ln::EditorPane>>({ ln::static_pointer_cast<ln::EditorPane>(m_modePane) });
    m_inspectorPanes = ln::makeList<Ref<ln::EditorPane>>({ m_inspectorPane });
    m_toolPanes = ln::makeList<Ref<ln::EditorPane>>();

    return true;
}

void TilemapSceneEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
    m_assetModel = asset;
    m_mainLayout = ln::makeObject<ln::UIVBoxLayout3>();
    m_mainLayout->setName(u"---");
    frame->addElement(m_mainLayout);
    {
        auto t = ln::UITextBlock::create(u"test");
		t->getGridLayoutInfo()->layoutWeight = 0;
		//t->setVAlignment(ln::VAlignment::Center);
        m_mainLayout->addChild(t);
    }


	m_mainViewport = ln::makeObject<ln::UIViewport>();
    //m_mainViewport->setVAlignment(ln::VAlignment::Stretch);
	m_mainViewport->getGridLayoutInfo()->layoutWeight = 1;
    m_mainLayout->addChild(m_mainViewport);
    //frame->addElement(m_mainViewport);
    //m_mainViewport->setBackgroundColor(ln::Color::Blue);// ln::Color(ln::Random::randFloat(), ln::Random::randFloat(), ln::Random::randFloat(), 1));


    //auto m_mainWorld = dynamic_cast<ln::World*>(asset->target());//ln::makeObject<ln::World>(); //
    //if (LN_REQUIRE(m_mainWorld)) return;
    //auto m_mainWorldAsset = ln::AssetModel::create(m_mainWorld);

    // create world and view
    {
        m_mainWorld = ln::makeObject<ln::World>();
        m_mainCamera = ln::makeObject<ln::Camera>();
        m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
        m_mainWorldRenderView->setTargetWorld(m_mainWorld);
        m_mainWorldRenderView->setCamera(m_mainCamera);
        //m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::Sky);
        m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
        m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
        m_mainWorldRenderView->connectOnUIEvent(ln::bind(this, &TilemapSceneEditor::WorldRenderView_OnUIEvent));
		m_mainWorldRenderView->setPhysicsDebugDrawEnabled(true);
        m_mainWorldRenderView->setGizmoEnabled(true);
        m_mainViewport->addRenderView(m_mainWorldRenderView);

        m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());
    }

    // attach as main scene
    auto scene = dynamic_cast<ln::Level*>(asset->target());
    m_mainWorld->addScene(scene);
   
    m_tilemap = static_cast<ln::Tilemap*>(m_mainWorld->findObjectByComponentType(ln::TypeInfo::getTypeInfo<ln::TilemapComponent>()));
    m_tilemap->setBlendMode(ln::BlendMode::Alpha);
    m_modePane->setTileset(m_tilemap->tilemapComponent()->tilemapModel()->tileset());
    m_currentLayer = m_tilemap->tilemapComponent()->tilemapModel()->layer(0);
    //m_tilemap->tilemapComponent();

    //editorContext()->mainProject()->assetDatabase()->createAsset(m_mainWorld, u"D:/Proj/LN/PrivateProjects/HC0/Assets/Scenes/test.lnasset");
 
    //editorContext()->mainProject()->assetDatabase()->saveAsset(m_assetModel);

	m_timer = ln::makeObject<ln::UIActiveTimer>();
	m_timer->connectOnTick(ln::bind(this, &TilemapSceneEditor::handleTickEvent));
	m_mainViewport->registerActiveTimer(m_timer);


    frame->addAction(ln::makeObject<ln::UIAction>(EditorApplication::SaveCommand, [this](ln::UICommandEventArgs* x) {
        printf("Save");
        this->editorContext()->assetDatabase()->saveAsset(this->m_assetModel);
    }));

}

void TilemapSceneEditor::onClosed()
{
}

Ref<ln::List<Ref<ln::EditorPane>>> TilemapSceneEditor::getEditorPanes(lna::EditorPaneKind kind)
{
    switch (kind)
    {
    case lna::EditorPaneKind::Mode:
        return m_modePanes;
    case lna::EditorPaneKind::Inspector:
        return m_inspectorPanes;
    case lna::EditorPaneKind::Tool:
        return m_toolPanes;
    }
    return nullptr;
}

void TilemapSceneEditor::clearSelectedObjects()
{
    m_selectedObjects.clear();
}

void TilemapSceneEditor::addSelectedObject(ln::WorldObject* obj)
{
    m_selectedObjects.add(obj);
}

void TilemapSceneEditor::WorldRenderView_OnUIEvent(ln::UIEventArgs* e)
{
#if 1   // TODO: ui layout リファクタリング中
    if (e->type() == ln::UIEvents::MouseDownEvent ||
        e->type() == ln::UIEvents::MouseUpEvent ||
        e->type() == ln::UIEvents::MouseMoveEvent) {
        auto me = static_cast<ln::UIMouseEventArgs*>(e);
        auto pt = me->getPosition(m_mainViewport);
        auto ray = m_mainCamera->screenToWorldRay(pt);
        ln::PointI tilePt;
        bool result = targetTilemapComponent()->intersectTile(ray, &tilePt);
        if (result/* && targetTilemapComponent()->tilemapModel()->getValidFrontLayer(tilePt.x, tilePt.y) != nullptr*/) {


            if (me->getMouseButtons() == ln::MouseButtons::Left) {
                m_currentLayer->setTileId(tilePt.x, tilePt.y, m_model->tilemapBrush()->tiles()[0]);
                m_mainViewport->invalidateVisual();
            }



            e->handled = true;
            return;
        }


    }
#endif
	//else if (e->type() == ln::UIEvents::MouseMoveEvent) {
	//	auto me = static_cast<ln::UIMouseEventArgs*>(e);
 //       if (me->getMouseButtons() == ln::MouseButtons::Left) {
 //           auto pt = me->getPosition(m_mainWorldRenderView);
 //           auto ray = m_mainCamera->screenToWorldRay(pt);
 //           ln::PointI tilePt;
 //           if (targetTilemapComponent()->intersectTile(ray, &tilePt)) {
 //               m_currentLayer->setTileId(tilePt.x, tilePt.y, m_model->tilemapBrush()->tiles()[0]);
 //               m_mainViewport->invalidateVisual();
 //               e->handled = true;
 //               return;
 //           }
 //       }
	//}
}

void TilemapSceneEditor::handleTickEvent(ln::UITimerEventArgs* e)
{
	m_mainWorld->updateFrame(e->elapsedSeconds());
}

} // namespace lna


#if 0

//==============================================================================
// TilemapSceneListPane

void TilemapSceneListPane::init()
{
    UIControl::init();

    auto layout1 = ln::makeObject<ln::UIBoxLayout3>();
    layout1->setOrientation(ln::Orientation::Vertical);
    addElement(layout1);

    auto layout2 = ln::makeObject<ln::UIHBoxLayout2>();
    layout1->addChild(layout2);
    {
        auto caption = ln::UITextBlock::create(u"Scene");
        caption->setMargin(ln::Thickness(8, 0));    // TODO: theme からとりたい
        caption->setVAlignment(ln::VAlignment::Center);
        layout2->addChild(caption);

        auto addButton = ln::UIButton::create(u"Add");
        addButton->connectOnClicked(ln::bind(this, &TilemapSceneListPane::addButton_onClick));
        layout2->addChild(addButton);

        auto deleteButton = ln::UIButton::create(u"Delete");
        layout2->addChild(deleteButton);
    }

    m_listview = ln::makeObject<ln::UIListView>();
    m_listview->getGridLayoutInfo()->layoutWeight = 1;
    m_listview->connectOnItemClick(ln::bind(this, &TilemapSceneListPane::listView_onItemClick));
    layout1->addChild(m_listview);


    auto project = lna::Workspace::instance()->project();
    m_assetRootDir = ln::Path(project->assetsDir(), u"Scenes");

    m_model = ln::makeObject<ln::UIFileSystemCollectionModel>();
    m_model->setRootPath(m_assetRootDir);
    m_listview->setViewModel(m_model);
}

void TilemapSceneListPane::addButton_onClick(ln::UIEventArgs* e)
{
    auto Level = ln::makeObject<ln::Level>();
    auto asset = ln::makeObject<ln::AssetModel>(Level);

    auto project = lna::Workspace::instance()->project();

    auto path = ln::Path::getUniqueFilePathInDirectory(m_assetRootDir, u"Scene-", ln::AssetModel::AssetFileExtension.c_str());

    asset->saveInternal(path);

    m_model->refresh();
}

void TilemapSceneListPane::listView_onItemClick(ln::UIClickEventArgs* e)
{
    if (e->clickCount() == 2) {
        auto path = m_model->filePath(ln::static_pointer_cast<ln::UICollectionItemModel>(e->sender()->m_viewModel));
        EditorApplication::instance()->openAssetFile(path);
    }
}

//==============================================================================
// SceneNavigatorExtension

void SceneNavigatorExtension::onAttached()
{
    m_item = ln::makeObject<ln::NavigationMenuItem>();
    m_item->setIconName(u"globe");  // map, image

    m_list = ln::makeObject<TilemapSceneListPane>();
}

ln::NavigationMenuItem* SceneNavigatorExtension::getNavigationMenuItem()
{
    return m_item;
}

ln::UIElement* SceneNavigatorExtension::getNavigationPane()
{
    return m_list;
}

//==============================================================================
// TilemapSceneEditor

ln::Result TilemapSceneEditor::init()
{
    AssetEditor::init();
    m_modePane = ln::makeObject<ln::EditorPane>();
    m_modePane->setBackgroundColor(ln::Color::Red);
    m_inspectorPane = ln::makeObject<ln::EditorPane>();
    m_inspectorPane->setBackgroundColor(ln::Color::Green);

    m_modePanes = ln::makeList<Ref<ln::EditorPane>>({ m_modePane });
    m_inspectorPanes = ln::makeList<Ref<ln::EditorPane>>({ m_inspectorPane });
    m_toolPanes = ln::makeList<Ref<ln::EditorPane>>();
    return true;
}

void TilemapSceneEditor::onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame)
{
    auto m_mainViewport = ln::makeObject<ln::UIViewport>();
    frame->addElement(m_mainViewport);
    //m_mainViewport->setBackgroundColor(ln::Color::Blue);// ln::Color(ln::Random::randFloat(), ln::Random::randFloat(), ln::Random::randFloat(), 1));


    auto m_mainWorld = ln::makeObject<ln::World>();
    auto m_mainCamera = ln::makeObject<ln::Camera>();
    auto m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
    m_mainWorldRenderView->setTargetWorld(m_mainWorld);
    m_mainWorldRenderView->setCamera(m_mainCamera);
    m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
    m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
    m_mainViewport->addRenderView(m_mainWorldRenderView);

    m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());

    auto sprite = ln::Sprite::create(ln::Texture2D::create(u"D:/Documents/LuminoProjects/RinoTutorial/Assets/player.png"), 4,4);
    sprite->setSourceRect(0, 0, 16, 16);
    sprite->setPosition(0, 2, 0);
    m_mainWorld->addObject(sprite);
}

void TilemapSceneEditor::onClosed()
{
}

Ref<ln::List<Ref<ln::EditorPane>>> TilemapSceneEditor::getEditorPanes(ln::EditorPaneKind kind)
{
    switch (kind)
    {
    case ln::EditorPaneKind::Mode:
        return m_modePanes;
    case ln::EditorPaneKind::Inspector:
        return m_inspectorPanes;
    case ln::EditorPaneKind::Tool:
        return m_toolPanes;
    }
    return nullptr;
}

//==============================================================================
// TilemapSceneEditorExtension

TilemapSceneEditorExtension::TilemapSceneEditorExtension()
{

}

ln::Ref<ln::AssetEditor> TilemapSceneEditorExtension::createEditor()
{
    return ln::makeObject<TilemapSceneEditor>();
}

#endif
