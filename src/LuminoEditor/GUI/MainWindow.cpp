

#include "NavigatorManager.hpp"
#include "DocumentManager.hpp"
#include "ToolPanesArea.hpp"
#include "OutputPane.hpp"
#include "ProblemsPane.hpp"
#include "InspectorPane.hpp"
#include "MainWindow.hpp"




class FileSystemTreeViewItem
	: public ln::UITreeItem
{
public:

	//void onViewModelChanged(ln::UIViewModel* newViewModel, ln::UIViewModel* oldViewModel)
	//{
	//	if (oldViewModel) {
	//		LN_NOTIMPLEMENTED();
	//	}

	//	
	//}
};


MainWindow::MainWindow()
{
	m_updateMode = ln::UIFrameWindowUpdateMode::EventDispatches;
}

void MainWindow::onLoaded()
{
	UIMainWindow::onLoaded();

	auto layout1 = ln::makeObject<ln::UIStackLayout>();
	layout1->setOrientation(ln::Orientation::Horizontal);
    layout1->lastStretch = true;
	setLayoutPanel(layout1);




    m_mainHSplitter = ln::makeObject<ln::UISplitter>();
    m_mainHSplitter->setCellDefinition(0, ln::UILayoutLengthType::Auto);
    m_mainHSplitter->setCellDefinition(1);
    m_mainHSplitter->setCellDefinition(2, ln::UILayoutLengthType::Direct, 100);
    addElement(m_mainHSplitter);

    {

        m_navigatorManager = ln::makeObject<NavigatorManager>();
        m_navigatorManager->navigationViewOpen = ln::bind(this, &MainWindow::onNavigationViewOpen);
        m_navigatorManager->navigationViewClose = ln::bind(this, &MainWindow::onNavigationViewClose);
        m_mainHSplitter->addElement(m_navigatorManager);

        //auto model = ln::makeObject<ln::UIFileSystemCollectionModel>();
        //model->setRootPath(u"D:/Proj/LN/Lumino");

  //      auto treeView = ln::makeObject<ln::UITreeView>();
  //      treeView->setViewModel(model);
  //      treeView->setWidth(200);
  //      treeView->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::Grey, 2));
  //      splitter->addElement(treeView);
		//treeView->getGridLayoutInfo()->layoutRow = 0;

        //--------

		auto splitter2 = ln::makeObject<ln::UISplitter>();
		splitter2->setOrientation(ln::Orientation::Vertical);
		splitter2->setCellDefinition(0);
		splitter2->setCellDefinition(1, ln::UILayoutLengthType::Direct, 200);
        m_mainHSplitter->addElement(splitter2);

		{
			//auto documentArea = ln::makeObject<ln::UIControl>();
			////documentArea->setHeight(300);
			//documentArea->setBackgroundColor(ln::Color::LightGray);
			//splitter2->addElement(documentArea);

			//auto documentTabs = ln::makeObject<ln::UITabBar>();
			//documentTabs->setVerticalAlignment(ln::VAlignment::Top);
			//documentTabs->setBackgroundColor(ln::Color::Azure);
			//documentTabs->setHeight(30);
			//documentArea->addElement(documentTabs);

			//// test
			//auto tab1 = ln::makeObject<ln::UITabItem>();
			//auto text1 = ln::makeObject<ln::UITextBlock>();
			//text1->setText(u"Tab1");
			//tab1->addElement(text1);
			//documentTabs->addTab(tab1);

			//auto tab2 = ln::makeObject<ln::UITabItem>();
			//auto text2 = ln::makeObject<ln::UITextBlock>();
			//text2->setText(u"Tab2");
			//tab2->addElement(text2);
			//documentTabs->addTab(tab2);

            m_documentManager = ln::makeObject<DocumentManager>();
            splitter2->addElement(m_documentManager);

			//--------

			m_toolPanesArea = ln::makeObject<ToolPanesArea>();
			m_toolPanesArea->setBackgroundColor(ln::Color::Green);
			splitter2->addElement(m_toolPanesArea);

			m_outputPane = ln::makeObject<OutputPane>();
			m_toolPanesArea->addPane(m_outputPane);
		}

		//--------

		m_inspectorPanesArea = ln::makeObject<ToolPanesArea>();
		m_inspectorPanesArea->setBackgroundColor(ln::UIColors::get(ln::UIColorHues::DeepOrange));
        m_mainHSplitter->addElement(m_inspectorPanesArea);
    }

	//auto d = ln::makeObject<ln::UIStyleDecorator>();
	//d->setIconName(u"file", 20);
	//treeView->m_localStyle->decorators.add(d);


	//auto icon = ln::makeObject<ln::UIIcon>();
	//icon->setIconName(u"file");
	//addElement(icon);



	//sidebar->addItem(u"A");
}

void MainWindow::onNavigationViewOpen()
{
    m_mainHSplitter->setCellDefinition(0, ln::UILayoutLengthType::Direct, 240);
}

void MainWindow::onNavigationViewClose()
{
}
