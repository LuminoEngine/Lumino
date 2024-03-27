
#include <LuminoFramework.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoFramework/UI/Controls/UIButton.hpp>
#include <LuminoFramework/UI/Controls/UICheckBox.hpp>
#include <LuminoFramework/UI/Controls/UIScrollView.hpp>
#include <LuminoFramework/UI/UIItemsModel.hpp>
#include <LuminoFramework/UI/UIItemsElement.hpp>
#include <LuminoFramework/UI/UIStyle.hpp>
#include <LuminoFramework/UI/Controls/UIListBox.hpp>
#include <LuminoFramework/UI/Controls/UITreeView.hpp>
#include <LuminoFramework/UI/Controls/UISplitter.hpp>
#include <LuminoFramework/UI/UIIcon.hpp>
#include <LuminoFramework/UI/UITabBar.hpp>
#include <LuminoFramework/UI/Controls/UIPropertyFields.hpp>
#include <LuminoFramework/UI/Controls/UIComboBox.hpp>
using namespace ln;



// https://www.infragistics.com/community/blogs/b/blagunas/posts/free-metro-light-and-dark-themes-for-wpf-and-silverlight-microsoft-controls
class UISandboxApp : public Application
{
public:
	void onInit() override
    {
		//auto vm = makeObject_deprecated<UIFileSystemCollectionModel>();
		//auto root = vm->setRootPath(_TT("C:/Proj/LN/Lumino");
		//int count = vm->getRowCount(root);
		//for (int i = 0; i < count; i++) {
		//	auto index = vm->getIndex(i, 0, root);
		//	auto data = vm->getData(index, _TT("");
		//	std::cout << data << std::endl;

		//	int count2 = vm->getRowCount(index);
		//	for (int i2 = 0; i2 < count2; i2++) {
		//		auto index2 = vm->getIndex(i2, 0, index);
		//		auto data2 = vm->getData(index2, _TT("");
		//		std::cout << "  " << data2 << std::endl;
		//	}
		//}

		Engine::renderView()->setBackgroundColor(UI::mainTheme()->color(UIThemeConstantPalette::DefaultBackgroundColor));

		auto mainLauout = makeObject_deprecated<UIGridLayout>();
		mainLauout->setColumnCount(4);
		Engine::ui()->addChild(mainLauout);

		int margin = 8;

#if 1
		// Button
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject_deprecated<UIText>(_TT("Button")));

			auto button1 = makeObject_deprecated<UIButton>();
			button1->setText(_TT("Button"));
			layout1->addChild(button1);


			auto button2 = makeObject_deprecated<UIButton>();
			button2->setText(_TT("IconButton"));
			layout1->addChild(button2);
			auto icon2 = ln::makeObject_deprecated<ln::UIIcon>();
			icon2->setIconName(_TT("file"));
			//icon->setMargin(ln::Thickness(0, 0, 0, 4));
			button2->addInlineVisual(icon2, ln::UIInlinePlacement::Top);


			auto button3 = makeObject_deprecated<UIButton>();
			button3->setText(_TT("Disabled"));
			button3->setEnabled(false);
			layout1->addChild(button3);
			auto icon3 = ln::makeObject_deprecated<ln::UIIcon>();
			icon3->setIconName(_TT("file"));
			//icon->setMargin(ln::Thickness(0, 0, 0, 4));
			button3->addInlineVisual(icon3, ln::UIInlinePlacement::Top);
		}
		//// RadioButton
		//{
		//	auto layout1 = makeObject_deprecated<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject_deprecated<UIText>(_TT("RadioButton"));

		//	layout1->addChild(makeObject_deprecated<UIButton>(_TT("dummy"));
		//}
		// CheckBox
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject_deprecated<UIText>(_TT("CheckBox")));

			auto checkbox1 = makeObject_deprecated<UICheckBox>();
			checkbox1->addChild(_TT("CheckBox1"));
			layout1->addChild(checkbox1);

			auto checkbox2 = makeObject_deprecated<UICheckBox>();
			checkbox2->addChild(_TT("CheckBox2"));
			layout1->addChild(checkbox2);

			auto checkbox3 = makeObject_deprecated<UICheckBox>();
			checkbox3->addChild(_TT("CheckBox3"));
			layout1->addChild(checkbox3);
		}
		//// ComboBox
		//{
		//	auto layout1 = makeObject_deprecated<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject_deprecated<UIText>(_TT("ComboBox"));

		//	layout1->addChild(makeObject_deprecated<UIButton>(_TT("dummy"));
		//}
		// ListBox (Direct)
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject_deprecated<UIText>(_TT("ListBox (Direct)")));

			auto listbox1 = UIListBox::create();
			listbox1->addChild(_TT("item1"));
			listbox1->addChild(_TT("item2"));
			listbox1->addChild(_TT("item3"));
			listbox1->addChild(_TT("item4"));
			//listbox1->addChild(_TT("item4");
			//listbox1->addChild(_TT("item4");
			//listbox1->addChild(_TT("item4");
			//listbox1->addChild(_TT("item4");
			//listbox1->addChild(_TT("item4");
			//listbox1->setHeight(200);
			layout1->addChild(listbox1);
		}

		// TreeView (Model)
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject_deprecated<UIText>(_TT("TreeView (Model)")));

			auto model1 = ln::makeObject_deprecated<ln::UIFileSystemCollectionModel>();
			model1->setRootPath(LN_LOCALFILE("Assets"));

			auto treeview1 = makeObject_deprecated<UITreeView2>();
			treeview1->setHeight(200);
			//treeview1->setBackgroundColor(Color::Red);
			treeview1->setViewModel(model1);
			treeview1->connectOnChecked([model1](UIEventArgs* e) {
				auto* item = static_cast<UITreeItem2*>(e->sender());
				auto path = model1->filePath(static_pointer_cast<UICollectionItemViewModel>(item->m_viewModel));
				Debug::print(_TT("Item clicked. ") + path);
			});
			layout1->addChild(treeview1);
		}

		// Splitter
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject_deprecated<UIText>(_TT("Splitter")));

			auto splitter1 = makeObject_deprecated<UISplitter>();
			splitter1->setOrientation(UILayoutOrientation::Horizontal);

			auto e1 = makeObject_deprecated<UIElement>();
			e1->setHeight(100);
			e1->setBackgroundColor(UIColors::get(UIColorHues::Red, 3));
			splitter1->addChild(e1);

			auto e2 = makeObject_deprecated<UIElement>();
			e2->setHeight(100);
			e2->setBackgroundColor(UIColors::get(UIColorHues::Green, 3));
			splitter1->addChild(e2);

			auto e3 = makeObject_deprecated<UIElement>();
			e3->setHeight(100);
			e3->setBackgroundColor(UIColors::get(UIColorHues::Blue, 3));
			splitter1->addChild(e3);

			layout1->addChild(splitter1);
		}
		// Icon
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject_deprecated<UIText>(_TT("Font icon")));

			{
				auto layout2 = makeObject_deprecated<UIStackLayout>();
				layout2->setOrientation(UILayoutOrientation::Horizontal);
				layout2->setAlignments(UIAlignment::HorizontalStretch);

				auto icon1 = makeObject_deprecated<UIIcon>();
				icon1->setIconName(_TT("file"));
				icon1->setFontSize(30);
				layout2->addChild(icon1);

				auto icon2 = makeObject_deprecated<UIIcon>();
				icon2->setIconName(_TT("file"));
				icon2->setFontSize(20);
				layout2->addChild(icon2);

				auto icon3 = makeObject_deprecated<UIIcon>();
				icon3->setIconName(_TT("file"));
				icon3->setFontSize(10);
				layout2->addChild(icon3);

				//icon1->setBorderThickness(1);
				//icon2->setBorderThickness(1);
				//icon3->setBorderThickness(1);

				layout1->addChild(layout2);
			}

			{
				auto layout2 = makeObject_deprecated<UIStackLayout>();
				layout2->setOrientation(UILayoutOrientation::Horizontal);
				layout2->setAlignments(UIAlignment::HorizontalStretch);

				layout2->addChild(UIIcon::loadFontIcon(_TT("align-justify"), 20));
				layout2->addChild(UIIcon::loadFontIcon(_TT("arrow-alt-circle-up)"), 20));
				layout2->addChild(UIIcon::loadFontIcon(_TT("book-medical"), 20));
				layout2->addChild(UIIcon::loadFontIcon(_TT("check-circle"), 20, UIColors::get(UIColorHues::Green)));
				layout2->addChild(UIIcon::loadFontIcon(_TT("times-circle"), 20, UIColors::get(UIColorHues::Red)));

				layout1->addChild(layout2);
			}
		}


		// TabBar
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);
			layout1->addChild(makeObject_deprecated<UIText>(_TT("TabBar")));


			auto tabbar1 = makeObject_deprecated<UITabBar2>();
			layout1->addChild(tabbar1);

			auto tab1 = makeObject_deprecated<UITabBarItem2>();
			tab1->addChild(_TT("Tab1"));
			tabbar1->addChild(tab1);

			auto tab2 = makeObject_deprecated<UITabBarItem2>();
			tab2->addChild(_TT("Tab2"));
			tabbar1->addChild(tab2);

			auto switch1 = makeObject_deprecated<UISwitchLayout>();
			layout1->addChild(switch1);

			auto e1 = makeObject_deprecated<UIElement>();
			e1->setHeight(100);
			e1->setBackgroundColor(UIColors::get(UIColorHues::Red, 3));
			switch1->addChild(e1);

			auto e2 = makeObject_deprecated<UIElement>();
			e2->setHeight(100);
			e2->setBackgroundColor(UIColors::get(UIColorHues::Green, 3));
			switch1->addChild(e2);

			tab1->setData(makeVariant(e1));
			tab2->setData(makeVariant(e2));
			tabbar1->connectOnSelectedTabChanged([/*tabbar1, */switch1](UIEventArgs* e) {	// tabbar1 をキャプチャすると循環参照になるので注意
				auto tab = static_cast<UITabBar2*>(e->sender())->selectedTab()->data()->getAsObject<UIElement>();
				switch1->setActive(tab);
			});
		}


		// PropertyFields
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);
			layout1->addChild(makeObject_deprecated<UIText>(_TT("PropertyFields")));

			auto field1 = makeObject_deprecated<UISliderField>();
			field1->setChanged([](float v) { ln::Debug::print(String::fromNumber(v)); });
			layout1->addChild(field1);

			auto field2 = makeObject_deprecated<UIColorField>();
			layout1->addChild(field2);
			
		}
#endif

		// ComboBox
		{
			auto layout1 = makeObject_deprecated<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);
			layout1->addChild(makeObject_deprecated<UIText>(_TT("ComboBox")));

			auto comboBox1 = makeObject_deprecated<UIComboBox>();
			comboBox1->setAlignments(UIAlignment::Left);
			comboBox1->addChild(_TT("CItem1"));
			comboBox1->addChild(_TT("CItem2"));
			layout1->addChild(comboBox1);
		}
    }

private:
    Ref<UIButton> m_button1;

};

void UISandboxMain()
{
	UISandboxApp app;
	EngineSettings::setMainWindowSize(1000, 800);
	detail::ApplicationHelper::run(&app);
}




