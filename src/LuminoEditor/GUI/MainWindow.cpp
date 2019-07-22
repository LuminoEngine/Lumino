
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include "MainWindow.hpp"

class NavigationBar : public ln::UIItemsControl
{
public:
	void addItem(const ln::String& text);

protected:
	//virtual UIControl* generateItem(UIElement* content) override;

private:
};

void NavigationBar::addItem(const ln::String& text)
{

	auto textblock = ln::makeObject<ln::UITextBlock>();
	textblock->setText(text);

	auto item = ln::makeObject<ln::UICollectionItem>();
	item->addElement(textblock);
	item->setBackgroundColor(ln::Color::Blue);

	addSelectionTarget(item);
	addElement(item);
}



MainWindow::MainWindow()
{
}

void MainWindow::onLoaded()
{
	UIMainWindow::onLoaded();

	auto layout1 = ln::makeObject<ln::UIStackLayout2>();
	layout1->setOrientation(ln::Orientation::Horizontal);
	setLayoutPanel(layout1);

	auto sidebar = ln::makeObject<NavigationBar>();
	sidebar->setWidth(40);
	sidebar->setBackgroundColor(ln::Color::Gray);
	sidebar->setHorizontalAlignment(ln::HAlignment::Stretch);
	sidebar->setVerticalAlignment(ln::VAlignment::Stretch);
	addElement(sidebar);


	sidebar->addItem(u"A");
}
