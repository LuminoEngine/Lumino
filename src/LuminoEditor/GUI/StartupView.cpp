
#include "Application.hpp"
#include "StartupView.hpp"

StartupView::StartupView()
{
}

ln::Result StartupView::init()
{
    UIControl::init();
    //setLayoutPanel(ln::makeObject<ln::UIVBoxLayout>());

    m_newProjectButton = ln::makeObject<ln::UIButton>();
    m_newProjectButton->setText(u"New project...");
    m_newProjectButton->setCommand(EditorApplication::NewCommand);
    addElement(m_newProjectButton);

    m_openProjectButton = ln::makeObject<ln::UIButton>();
    m_openProjectButton->setText(u"Open project...");
    m_openProjectButton->setCommand(EditorApplication::OpenCommand);
    addElement(m_openProjectButton);



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
