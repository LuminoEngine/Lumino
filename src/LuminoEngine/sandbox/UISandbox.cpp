
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>
#include <LuminoEngine/UI/Controls/UICheckBox.hpp>
#include <LuminoEngine/UI/Controls/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/Controls/UIListBox.hpp>
#include <LuminoEngine/UI/Controls/UITreeView.hpp>
#include <LuminoEngine/UI/Controls/UISplitter.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>
#include <LuminoEngine/UI/UITabBar.hpp>
#include <LuminoEngine/UI/Controls/UIPropertyFields.hpp>
#include <LuminoEngine/UI/Controls/UIComboBox.hpp>
using namespace ln;



// https://www.infragistics.com/community/blogs/b/blagunas/posts/free-metro-light-and-dark-themes-for-wpf-and-silverlight-microsoft-controls
class UISandboxApp : public Application
{
public:
	void onInit() override
    {
		//auto vm = makeObject<UIFileSystemCollectionModel>();
		//auto root = vm->setRootPath(u"C:/Proj/LN/Lumino");
		//int count = vm->getRowCount(root);
		//for (int i = 0; i < count; i++) {
		//	auto index = vm->getIndex(i, 0, root);
		//	auto data = vm->getData(index, u"");
		//	std::cout << data << std::endl;

		//	int count2 = vm->getRowCount(index);
		//	for (int i2 = 0; i2 < count2; i2++) {
		//		auto index2 = vm->getIndex(i2, 0, index);
		//		auto data2 = vm->getData(index2, u"");
		//		std::cout << "  " << data2 << std::endl;
		//	}
		//}

		Engine::renderView()->setBackgroundColor(UI::mainTheme()->color(UIThemeConstantPalette::DefaultBackgroundColor));

		auto mainLauout = makeObject<UIGridLayout>();
		mainLauout->setColumnCount(4);
		Engine::ui()->addChild(mainLauout);

		int margin = 8;

#if 1
		// Button
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UIText>(u"Button"));

			auto button1 = makeObject<UIButton>();
			button1->setText(u"Button");
			layout1->addChild(button1);


			auto button2 = makeObject<UIButton>();
			button2->setText(u"IconButton");
			layout1->addChild(button2);
			auto icon2 = ln::makeObject<ln::UIIcon>();
			icon2->setIconName(u"file");
			//icon->setMargin(ln::Thickness(0, 0, 0, 4));
			button2->addInlineVisual(icon2, ln::UIInlinePlacement::Top);


			auto button3 = makeObject<UIButton>();
			button3->setText(u"Disabled");
			button3->setEnabled(false);
			layout1->addChild(button3);
			auto icon3 = ln::makeObject<ln::UIIcon>();
			icon3->setIconName(u"file");
			//icon->setMargin(ln::Thickness(0, 0, 0, 4));
			button3->addInlineVisual(icon3, ln::UIInlinePlacement::Top);
		}
		//// RadioButton
		//{
		//	auto layout1 = makeObject<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject<UIText>(u"RadioButton"));

		//	layout1->addChild(makeObject<UIButton>(u"dummy"));
		//}
		// CheckBox
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UIText>(u"CheckBox"));

			auto checkbox1 = makeObject<UICheckBox>();
			checkbox1->addChild(u"CheckBox1");
			layout1->addChild(checkbox1);

			auto checkbox2 = makeObject<UICheckBox>();
			checkbox2->addChild(u"CheckBox2");
			layout1->addChild(checkbox2);

			auto checkbox3 = makeObject<UICheckBox>();
			checkbox3->addChild(u"CheckBox3");
			layout1->addChild(checkbox3);
		}
		//// ComboBox
		//{
		//	auto layout1 = makeObject<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject<UIText>(u"ComboBox"));

		//	layout1->addChild(makeObject<UIButton>(u"dummy"));
		//}
		// ListBox (Direct)
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UIText>(u"ListBox (Direct)"));

			auto listbox1 = UIListBox::create();
			listbox1->addChild(u"item1");
			listbox1->addChild(u"item2");
			listbox1->addChild(u"item3");
			listbox1->addChild(u"item4");
			//listbox1->addChild(u"item4");
			//listbox1->addChild(u"item4");
			//listbox1->addChild(u"item4");
			//listbox1->addChild(u"item4");
			//listbox1->addChild(u"item4");
			//listbox1->setHeight(200);
			layout1->addChild(listbox1);
		}

		// TreeView (Model)
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UIText>(u"TreeView (Model)"));

			auto model1 = ln::makeObject<ln::UIFileSystemCollectionModel>();
			model1->setRootPath(LN_LOCALFILE("Assets"));

			auto treeview1 = makeObject<UITreeView2>();
			treeview1->setHeight(200);
			//treeview1->setBackgroundColor(Color::Red);
			treeview1->setViewModel(model1);
			treeview1->connectOnChecked([model1](UIEventArgs* e) {
				auto* item = static_cast<UITreeItem2*>(e->sender());
				auto path = model1->filePath(static_pointer_cast<UICollectionItemViewModel>(item->m_viewModel));
				Debug::print(u"Item clicked. " + path);
			});
			layout1->addChild(treeview1);
		}

		// Splitter
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UIText>(u"Splitter"));

			auto splitter1 = makeObject<UISplitter>();
			splitter1->setOrientation(UILayoutOrientation::Horizontal);

			auto e1 = makeObject<UIElement>();
			e1->setHeight(100);
			e1->setBackgroundColor(UIColors::get(UIColorHues::Red, 3));
			splitter1->addChild(e1);

			auto e2 = makeObject<UIElement>();
			e2->setHeight(100);
			e2->setBackgroundColor(UIColors::get(UIColorHues::Green, 3));
			splitter1->addChild(e2);

			auto e3 = makeObject<UIElement>();
			e3->setHeight(100);
			e3->setBackgroundColor(UIColors::get(UIColorHues::Blue, 3));
			splitter1->addChild(e3);

			layout1->addChild(splitter1);
		}
		// Icon
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UIText>(u"Font icon"));

			{
				auto layout2 = makeObject<UIStackLayout>();
				layout2->setOrientation(UILayoutOrientation::Horizontal);
				layout2->setHAlignment(UIHAlignment::Center);

				auto icon1 = makeObject<UIIcon>();
				icon1->setIconName(u"file");
				icon1->setFontSize(30);
				layout2->addChild(icon1);

				auto icon2 = makeObject<UIIcon>();
				icon2->setIconName(u"file");
				icon2->setFontSize(20);
				layout2->addChild(icon2);

				auto icon3 = makeObject<UIIcon>();
				icon3->setIconName(u"file");
				icon3->setFontSize(10);
				layout2->addChild(icon3);

				//icon1->setBorderThickness(1);
				//icon2->setBorderThickness(1);
				//icon3->setBorderThickness(1);

				layout1->addChild(layout2);
			}

			{
				auto layout2 = makeObject<UIStackLayout>();
				layout2->setOrientation(UILayoutOrientation::Horizontal);
				layout2->setHAlignment(UIHAlignment::Center);

				layout2->addChild(UIIcon::loadFontIcon(u"align-justify", 20));
				layout2->addChild(UIIcon::loadFontIcon(u"arrow-alt-circle-up", 20));
				layout2->addChild(UIIcon::loadFontIcon(u"book-medical", 20));
				layout2->addChild(UIIcon::loadFontIcon(u"check-circle", 20, UIColors::get(UIColorHues::Green)));
				layout2->addChild(UIIcon::loadFontIcon(u"times-circle", 20, UIColors::get(UIColorHues::Red)));

				layout1->addChild(layout2);
			}
		}


		// TabBar
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);
			layout1->addChild(makeObject<UIText>(u"TabBar"));


			auto tabbar1 = makeObject<UITabBar2>();
			layout1->addChild(tabbar1);

			auto tab1 = makeObject<UITabBarItem2>();
			tab1->addChild(u"Tab1");
			tabbar1->addChild(tab1);

			auto tab2 = makeObject<UITabBarItem2>();
			tab2->addChild(u"Tab2");
			tabbar1->addChild(tab2);

			auto switch1 = makeObject<UISwitchLayout>();
			layout1->addChild(switch1);

			auto e1 = makeObject<UIElement>();
			e1->setHeight(100);
			e1->setBackgroundColor(UIColors::get(UIColorHues::Red, 3));
			switch1->addChild(e1);

			auto e2 = makeObject<UIElement>();
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
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);
			layout1->addChild(makeObject<UIText>(u"PropertyFields"));

			auto field1 = makeObject<UISliderField>();
			field1->setChanged([](float v) { ln::Debug::print(String::fromNumber(v)); });
			layout1->addChild(field1);

			auto field2 = makeObject<UIColorField>();
			layout1->addChild(field2);
			
		}
#endif

		// ComboBox
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);
			layout1->addChild(makeObject<UIText>(u"ComboBox"));

			auto comboBox1 = makeObject<UIComboBox>();
			comboBox1->setHAlignment(UIHAlignment::Left);
			comboBox1->addChild(u"CItem1");
			comboBox1->addChild(u"CItem2");
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




