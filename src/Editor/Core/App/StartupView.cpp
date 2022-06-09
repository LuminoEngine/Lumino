
#include "Application.hpp"
#include "StartupView.hpp"
#include "../Controls/AssetPicker.hpp"  // TODO: Test
#include "AppData.hpp"

//==============================================================================
// RecentProjectListView

void RecentProjectListView::init()
{
    UIListBox::init();
    ////ln::_TT("test"
    //auto t = ln::UIText::create(_TT("test");
    //addElement(t);
}

//==============================================================================
// UIListBoxItem

StartupView::StartupView()
{
}

ln::Result StartupView::init()
{
    UIControl::init();
    //setLayoutPanel(ln::makeObject<ln::UIVBoxLayout>());

    auto layout = ln::makeObject<ln::UIVBoxLayout2>();

    //layout->setVAlignment(ln::UIVAlignment::Top);
    addElement(layout);

    m_newProjectButton = ln::makeObject<ln::UIButton>();
    m_newProjectButton->setWidth(200);
    m_newProjectButton->setText(_TT("New project..."));
    m_newProjectButton->setCommand(EditorApplication::NewCommand);
#if LN_USE_YOGA
    LN_NOTIMPLEMENTED();
#else
    m_newProjectButton->setHorizontalContentAlignment(ln::UIHAlignment::Center);
    m_newProjectButton->setVerticalContentAlignment(ln::UIVAlignment::Center);
#endif
    layout->addChild(m_newProjectButton);

    m_openProjectButton = ln::makeObject<ln::UIButton>();
    m_openProjectButton->setWidth(200);
    m_openProjectButton->setText(_TT("Open project..."));
    m_openProjectButton->setCommand(EditorApplication::OpenCommand);
#if LN_USE_YOGA
    LN_NOTIMPLEMENTED();
#else
    m_openProjectButton->setHorizontalContentAlignment(ln::UIHAlignment::Center);
    m_openProjectButton->setVerticalContentAlignment(ln::UIVAlignment::Center);
#endif
    layout->addChild(m_openProjectButton);

    m_recentProjectListView = ln::makeObject<RecentProjectListView>();
    m_recentProjectListView->setWidth(200);
    layout->addChild(m_recentProjectListView);






    auto icon = ln::makeObject<ln::UIIcon>();
    icon->setIconName(_TT("file"));
#if LN_USE_YOGA
    LN_NOTIMPLEMENTED();
#else
    icon->setHAlignment(ln::UIHAlignment::Center);
#endif
    icon->setMargin(ln::Thickness(0, 0, 0, 4));
    m_openProjectButton->addInlineVisual(icon, ln::UIInlinePlacement::Top);





    auto popupContent = ln::makeObject<ln::UIText>();
    popupContent->setText(_TT("POP"));
    auto popup = ln::makeObject<ln::UIDialog>();
    popup->addElement(popupContent);
    //popup->setPlacementTarget(m_openProjectButton);
	popup->setBackgroundColor(ln::Color::Green);
    layout->addChild(popup);


    auto popupButton = ln::makeObject<ln::UIButton>();
    popupButton->setWidth(200);
    popupButton->setText(_TT("Popup"));
#if LN_USE_YOGA
    LN_NOTIMPLEMENTED();
#else
    popupButton->setHorizontalContentAlignment(ln::UIHAlignment::Center);
    popupButton->setVerticalContentAlignment(ln::UIVAlignment::Center);
#endif
    popupButton->connectOnClicked([popup]() { if (!popup->isOpend()) popup->open(); else popup->close(); });
    layout->addChild(popupButton);


	auto text = ln::makeObject<ln::UITextField>();
	text->setWidth(200);
	text->setHeight(30);
	text->setText(_TT("Hello\nLumino"));
	layout->addChild(text);

    auto ap = ln::makeObject<ln::AssetPicker>();
    layout->addChild(ap);



	//auto m_mainViewport = ln::makeObject<ln::UIViewport>();
	//addElement(m_mainViewport);

	//auto m_mainWorld = ln::makeObject<ln::World>();
	//auto m_mainCamera = ln::makeObject<ln::Camera>();
	//auto m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
	//m_mainWorldRenderView->setTargetWorld(m_mainWorld);
	//m_mainWorldRenderView->setCamera(m_mainCamera);
	//m_mainWorldRenderView->setClearMode(ln::SceneClearMode::ColorAndDepth);
	//m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
	//m_mainViewport->addRenderView(m_mainWorldRenderView);

	//m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());

 //   auto sprite = ln::Sprite::create(ln::Texture2D::create(_TT("D:/Documents/LuminoProjects/RinoTutorial/Assets/player.png"), 4,4);
 //   sprite->setSourceRect(0, 0, 16, 16);
 //   sprite->setPosition(0, 2, 0);
 //   m_mainWorld->addObject(sprite);

    for (auto& file : lna::AppData::current()->recentProjectFiles) {
        auto link = ln::makeObject<ln::UILinkLabel>();
        link->setText(file.fileName());
        link->setUrl(file);
        layout->addChild(link);
    }
    

    return ln::ok();
}
