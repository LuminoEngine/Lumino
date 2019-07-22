
#include "MainWindow.hpp"

MainWindow::MainWindow()
{
}

void MainWindow::onLoaded()
{
	UIMainWindow::onLoaded();

	auto layout1 = ln::makeObject<ln::UIStackLayout2>();
	layout1->setOrientation(ln::Orientation::Horizontal);
	setLayoutPanel(layout1);

	auto sidebar = ln::makeObject<ln::UIContainerElement>();
	sidebar->setWidth(40);
	sidebar->setBackgroundColor(ln::Color::Gray);
	sidebar->setHorizontalAlignment(ln::HAlignment::Stretch);
	sidebar->setVerticalAlignment(ln::VAlignment::Stretch);
	addElement(sidebar);
}
