
#include "Application.hpp"
#include "NavigatorManager.hpp"
#include "DocumentManager.hpp"
#include "ToolPanesArea.hpp"
#include "OutputPane.hpp"
#include "ProblemsPane.hpp"
#include "InspectorPane.hpp"
#include "MainWindow.hpp"

MainWindow::MainWindow()
{
	m_updateMode = ln::UIFrameWindowUpdateMode::EventDispatches;
}

void MainWindow::init()
{
	UIMainWindow::init();
}

void MainWindow::onLoaded()
{
	UIMainWindow::onLoaded();
	renderView()->setBackgroundColor(ln::Color::White);
	renderView()->setClearMode(ln::RenderViewClearMode::ColorAndDepth);

	auto layout1 = ln::makeObject<ln::UIStackLayout2>();
	layout1->setOrientation(ln::Orientation::Horizontal);
    addElement(layout1);


    m_mainHSplitter = ln::makeObject<ln::UISplitter>();
    m_mainHSplitter->setCellDefinition(0, ln::UILayoutLengthType::Auto);
    m_mainHSplitter->setCellDefinition(1);
    m_mainHSplitter->setCellDefinition(2, ln::UILayoutLengthType::Direct, 100);
    //addElement(m_mainHSplitter);
    layout1->addChild(m_mainHSplitter, ln::UILayoutLengthType::Ratio);

    {

        m_navigatorManager = ln::makeObject<NavigatorManager>();
        m_navigatorManager->navigationViewOpen = ln::bind(this, &MainWindow::onNavigationViewOpen);
        m_navigatorManager->navigationViewClose = ln::bind(this, &MainWindow::onNavigationViewClose);
        m_mainHSplitter->addElement(m_navigatorManager);

        //--------

		auto splitter2 = ln::makeObject<ln::UISplitter>();
		splitter2->setOrientation(ln::Orientation::Vertical);
		splitter2->setCellDefinition(0);
		splitter2->setCellDefinition(1, ln::UILayoutLengthType::Direct, 200);
        m_mainHSplitter->addElement(splitter2);

		{
            m_documentManager = ln::makeObject<DocumentManager>();
            splitter2->addElement(m_documentManager);

			//--------

			m_toolPanesArea = ln::makeObject<ToolPanesArea>();
			splitter2->addElement(m_toolPanesArea);

			m_outputPane = ln::makeObject<OutputPane>();
			m_toolPanesArea->addPane(m_outputPane);
		}

		//--------

		m_inspectorPanesArea = ln::makeObject<ToolPanesArea>();
        m_inspectorPanesArea->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 1));
        m_mainHSplitter->addElement(m_inspectorPanesArea);
    }


    //// test
    //EditorApplication::instance()->workspace()->openProject2(u"D:/Proj/TH-10/TH-10.lnproj");
    //navigatorManager()->resetNavigators();
}

void MainWindow::onNavigationViewOpen()
{
    m_mainHSplitter->setCellDefinition(0, ln::UILayoutLengthType::Direct, 200 + NavigationBar::ItemSize);
}

void MainWindow::onNavigationViewClose()
{
}
