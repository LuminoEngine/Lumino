
#include "Application.hpp"
#include "StartupView.hpp"

StartupView::StartupView()
{
}

ln::Result StartupView::init()
{
    UIControl::init();
    //setLayoutPanel(ln::makeObject<ln::UIVBoxLayout>());

    auto layout = ln::makeObject<ln::UIVBoxLayout2>();

    //layout->setVerticalAlignment(ln::VAlignment::Top);
    addElement(layout);

    m_newProjectButton = ln::makeObject<ln::UIButton>();
    m_newProjectButton->setWidth(200);
    m_newProjectButton->setText(u"New project...");
    m_newProjectButton->setCommand(EditorApplication::NewCommand);
    m_newProjectButton->setHorizontalContentAlignment(ln::HAlignment::Center);
    m_newProjectButton->setVerticalContentAlignment(ln::VAlignment::Center);
    layout->addChild(m_newProjectButton);

    m_openProjectButton = ln::makeObject<ln::UIButton>();
    m_openProjectButton->setWidth(200);
    m_openProjectButton->setText(u"Open project...");
    m_openProjectButton->setCommand(EditorApplication::OpenCommand);
    m_openProjectButton->setHorizontalContentAlignment(ln::HAlignment::Center);
    m_openProjectButton->setVerticalContentAlignment(ln::VAlignment::Center);
    layout->addChild(m_openProjectButton);

    auto icon = ln::makeObject<ln::UIIcon>();
    icon->setIconName(u"file");
    icon->setHorizontalAlignment(ln::HAlignment::Center);
    icon->setMargin(ln::Thickness(0, 0, 0, 4));
    m_openProjectButton->addInlineElement(icon, ln::UIInlineLayout::Top);





    auto popupContent = ln::makeObject<ln::UITextBlock>();
    popupContent->setText(u"POP");
    auto popup = ln::makeObject<ln::UIPopup>();
    popup->setContent(popupContent);
    popup->setPlacementTarget(m_openProjectButton);
    layout->addChild(popup);


    auto popupButton = ln::makeObject<ln::UIButton>();
    popupButton->setWidth(200);
    popupButton->setText(u"Popup");
    popupButton->setHorizontalContentAlignment(ln::HAlignment::Center);
    popupButton->setVerticalContentAlignment(ln::VAlignment::Center);
    popupButton->connectOnClicked([popup](ln::UIEventArgs* e) { if (!popup->isOpend()) popup->open(); else popup->close(); });
    layout->addChild(popupButton);


	auto text = ln::makeObject<ln::UITextField>();
	text->setWidth(200);
	text->setHeight(30);
	text->setText(u"Hello\nLumino");
	layout->addChild(text);


	//auto m_mainViewport = ln::makeObject<ln::UIViewport>();
	//addElement(m_mainViewport);

	//auto m_mainWorld = ln::makeObject<ln::World>();
	//auto m_mainCamera = ln::makeObject<ln::Camera>();
	//auto m_mainWorldRenderView = ln::makeObject<ln::WorldRenderView>();
	//m_mainWorldRenderView->setTargetWorld(m_mainWorld);
	//m_mainWorldRenderView->setCamera(m_mainCamera);
	//m_mainWorldRenderView->setClearMode(ln::RenderViewClearMode::ColorAndDepth);
	//m_mainWorldRenderView->setBackgroundColor(ln::Color::Gray);
	//m_mainViewport->addRenderView(m_mainWorldRenderView);

	//m_mainCamera->addComponent(ln::makeObject<ln::CameraOrbitControlComponent>());

 //   auto sprite = ln::Sprite::create(ln::Texture2D::create(u"D:/Documents/LuminoProjects/RinoTutorial/Assets/player.png"), 4,4);
 //   sprite->setSourceRect(0, 0, 16, 16);
 //   sprite->setPosition(0, 2, 0);
 //   m_mainWorld->addObject(sprite);

    return true;
}
