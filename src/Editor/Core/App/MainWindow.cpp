
#include "Application.hpp"
#include "NavigatorManager.hpp"
#include "DocumentManager.hpp"
#include "ToolPanesArea.hpp"
#include "OutputPane.hpp"
#include "ProblemsPane.hpp"
#include "InspectorPane.hpp"
#include "MainWindow.hpp"

MainWindow::MainWindow()
	: m_navigationViewExpandingSize(300)
{
	m_updateMode = ln::UIFrameWindowUpdateMode::EventDispatches;
}

void MainWindow::init()
{
	UIMainWindow::init(false);
    m_realtimeRenderingEnabled = false;
    setAllowDragDrop(true);
}

void MainWindow::onLoaded()
{
	UIMainWindow::onLoaded();
	renderView()->setBackgroundColor(ln::Color::White);
	renderView()->setClearMode(ln::SceneClearMode::ColorAndDepth);

    m_documentManager = ln::makeObject<DocumentManager>();

	auto layout1 = ln::makeObject<ln::UIStackLayout2_Obsolete>();
	layout1->setOrientation(ln::UILayoutOrientation::Horizontal);
    addElement(layout1);


    m_mainHSplitter = ln::makeObject<ln::UISplitter>();
    m_mainHSplitter->setCellDefinition(0, ln::UILayoutLengthType::Auto);
    m_mainHSplitter->setCellDefinition(1);
    m_mainHSplitter->setCellDefinition(2, ln::UILayoutLengthType::Direct, 300);
    //addElement(m_mainHSplitter);
    layout1->addChild(m_mainHSplitter, ln::UILayoutLengthType::Ratio);

    {

        m_navigatorManager = ln::makeObject<NavigatorManager>();
        m_navigatorManager->navigationViewOpen = ln::bind(this, &MainWindow::onNavigationViewOpen);
        m_navigatorManager->navigationViewClose = ln::bind(this, &MainWindow::onNavigationViewClose);
        m_mainHSplitter->addElement(m_navigatorManager);


        m_mainVSplitter = ln::makeObject<ln::UISplitter>();
        m_mainVSplitter->setOrientation(ln::UILayoutOrientation::Vertical);
        m_mainVSplitter->setCellDefinition(0);
        m_mainVSplitter->setCellDefinition(1, ln::UILayoutLengthType::Direct, 200);
        m_mainHSplitter->addElement(m_mainVSplitter);

        {
            {
                auto documentHSplitter = ln::makeObject<ln::UISplitter>();
                documentHSplitter->setName(_TT("documentHSplitter"));
                documentHSplitter->setOrientation(ln::UILayoutOrientation::Horizontal);
                documentHSplitter->setCellDefinition(0, ln::UILayoutLengthType::Direct, 300);
                
                documentHSplitter->addElement(m_documentManager->modePanesArea());
                //--------
                documentHSplitter->addElement(m_documentManager);

                m_mainVSplitter->addElement(documentHSplitter);
            }

            //--------

            m_mainVSplitter->addElement(m_documentManager->toolPanesArea());

            m_outputPane = ln::makeObject<OutputPane>();
            m_documentManager->toolPanesArea()->addPane(m_outputPane);
        }

        //--------

        m_mainHSplitter->addElement(m_documentManager->inspectorPanesArea());



    }
}

void MainWindow::onNavigationViewOpen()
{
    m_mainHSplitter->setCellDefinition(0, ln::UILayoutLengthType::Direct, m_navigationViewExpandingSize);
	m_mainHSplitter->resetCellSizes();
	m_mainVSplitter->resetCellSizes();
}

void MainWindow::onNavigationViewClose()
{
}
