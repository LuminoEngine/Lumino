
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
    //setLayoutPanel(ln::makeObject_deprecated<ln::UIVBoxLayout>());

    auto layout = ln::makeObject_deprecated<ln::UIVBoxLayout2>();

    //layout->setVAlignment(ln::UIVAlignment::Top);
    addElement(layout);

    m_newProjectButton = ln::makeObject_deprecated<ln::UIButton>();
    m_newProjectButton->setWidth(200);
    m_newProjectButton->setText(_TT("New project..."));
    m_newProjectButton->setCommand(EditorApplication::NewCommand);
    m_newProjectButton->setHorizontalContentAlignment(ln::UIHAlignment::Center);
    m_newProjectButton->setVerticalContentAlignment(ln::UIVAlignment::Center);
    layout->addChild(m_newProjectButton);

    m_openProjectButton = ln::makeObject_deprecated<ln::UIButton>();
    m_openProjectButton->setWidth(200);
    m_openProjectButton->setText(_TT("Open project..."));
    m_openProjectButton->setCommand(EditorApplication::OpenCommand);
    m_openProjectButton->setHorizontalContentAlignment(ln::UIHAlignment::Center);
    m_openProjectButton->setVerticalContentAlignment(ln::UIVAlignment::Center);
    layout->addChild(m_openProjectButton);

    m_recentProjectListView = ln::makeObject_deprecated<RecentProjectListView>();
    m_recentProjectListView->setWidth(200);
    layout->addChild(m_recentProjectListView);






    auto icon = ln::makeObject_deprecated<ln::UIIcon>();
    icon->setIconName(_TT("file"));
    icon->setAlignments(ln::UIAlignment::Center);
    icon->setMargin(ln::Thickness(0, 0, 0, 4));
    m_openProjectButton->addInlineVisual(icon, ln::UIInlinePlacement::Top);





    auto popupContent = ln::makeObject_deprecated<ln::UIText>();
    popupContent->setText(_TT("POP"));
    auto popup = ln::makeObject_deprecated<ln::UIDialog>();
    popup->addElement(popupContent);
    //popup->setPlacementTarget(m_openProjectButton);
	popup->setBackgroundColor(ln::Color::Green);
    layout->addChild(popup);


    auto popupButton = ln::makeObject_deprecated<ln::UIButton>();
    popupButton->setWidth(200);
    popupButton->setText(_TT("Popup"));
    popupButton->setHorizontalContentAlignment(ln::UIHAlignment::Center);
    popupButton->setVerticalContentAlignment(ln::UIVAlignment::Center);
    popupButton->connectOnClicked([popup]() { if (!popup->isOpend()) popup->open(); else popup->close(); });
    layout->addChild(popupButton);


	auto text = ln::makeObject_deprecated<ln::UITextField>();
	text->setWidth(200);
	text->setHeight(30);
	text->setText(_TT("Hello\nLumino"));
	layout->addChild(text);

    auto ap = ln::makeObject_deprecated<ln::AssetPicker>();
    layout->addChild(ap);



	//auto m_mainViewport = ln::makeObject_deprecated<ln::UIViewport>();
	//addElement(m_mainViewport);

	//auto m_mainWorld = ln::makeObject_deprecated<ln::World>();
	//auto m_mainCamera = ln::makeObject_deprecated<ln::Camera>();
	//auto m_mainWorldRenderView = ln::makeObject_deprecated<ln::WorldRenderView>();
	//m_mainWorldRenderView->setTargetWorld(m_mainWorld);
	//m_mainWorldRenderView->setCamera(m_mainCamera);
	//m_mainWorldRenderView->setClearMode(ln::SceneClearMode::ColorAndDepth);
	//m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
	//m_mainViewport->addRenderView(m_mainWorldRenderView);

	//m_mainCamera->addComponent(ln::makeObject_deprecated<ln::CameraOrbitControlComponent>());

 //   auto sprite = ln::Sprite::create(ln::Texture2D::create(_TT("D:/Documents/LuminoProjects/RinoTutorial/Assets/player.png"), 4,4);
 //   sprite->setSourceRect(0, 0, 16, 16);
 //   sprite->setPosition(0, 2, 0);
 //   m_mainWorld->addObject(sprite);

    for (auto& file : lna::AppData::current()->recentProjectFiles) {
        auto link = ln::makeObject_deprecated<ln::UILinkLabel>();
        link->setText(file.fileName());
        link->setUrl(file);
        layout->addChild(link);
    }
    

    return ln::ok();
}
