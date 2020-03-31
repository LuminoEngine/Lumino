
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIListBox.hpp>
#include <LuminoEngine/UI/UITreeView.hpp>
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

		Engine::renderView()->setBackgroundColor(Color::Gray);

		auto mainLauout = makeObject<UIGridLayout>();
		mainLauout->setColumnCount(4);
		Engine::ui()->addChild(mainLauout);

		int margin = 8;

		// Button
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UITextBlock>(u"Button"));

			auto button1 = makeObject<UIButton>();
			button1->setText(u"Button");
			layout1->addChild(button1);
		}
		//// RadioButton
		//{
		//	auto layout1 = makeObject<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject<UITextBlock>(u"RadioButton"));

		//	layout1->addChild(makeObject<UIButton>(u"dummy"));
		//}
		//// CheckBox
		//{
		//	auto layout1 = makeObject<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject<UITextBlock>(u"CheckBox"));

		//	layout1->addChild(makeObject<UIButton>(u"dummy"));
		//}
		//// ComboBox
		//{
		//	auto layout1 = makeObject<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject<UITextBlock>(u"ComboBox"));

		//	layout1->addChild(makeObject<UIButton>(u"dummy"));
		//}
		// ListBox (Direct)
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UITextBlock>(u"ListBox (Direct)"));

			auto listbox1 = UIListBox::create();
			listbox1->addChild(u"item1");
			listbox1->addChild(u"item2");
			listbox1->addChild(u"item3");
			listbox1->addChild(u"item4");
			layout1->addChild(listbox1);
		}
		//// TreeView (Model)
		//{
		//	auto layout1 = makeObject<UIStackLayout>();
		//	layout1->setMargin(margin);
		//	mainLauout->addChild(layout1);

		//	layout1->addChild(makeObject<UITextBlock>(u"TreeView (Model)"));

		//	auto model1 = ln::makeObject<ln::UIFileSystemCollectionModel>();
		//	model1->setRootPath(LN_LOCALFILE("Assets"));

		//	auto treeview1 = makeObject<UITreeView>();
		//	treeview1->setViewModel(model1);
		//	layout1->addChild(treeview1);
		//}

		// TreeView (Model)
		{
			auto layout1 = makeObject<UIStackLayout>();
			layout1->setMargin(margin);
			mainLauout->addChild(layout1);

			layout1->addChild(makeObject<UITextBlock>(u"TreeView (Model)"));

			auto model1 = ln::makeObject<ln::UIFileSystemCollectionModel>();
			model1->setRootPath(LN_LOCALFILE("Assets"));

			auto treeview1 = makeObject<UITreeView2>();
			treeview1->setHeight(200);
			//treeview1->setBackgroundColor(Color::Red);
			treeview1->setViewModel(model1);
			layout1->addChild(treeview1);
		}


		//{
		//	auto button1 = makeObject<UIToggleButton>();
		//	button1->setWidth(200);
		//	button1->setHeight(32);
		//	button1->setText(u"Toggle button");
		//	button1->setHAlignment(HAlignment::Right);
		//	button1->setVAlignment(VAlignment::Center);
		//}


		//auto thumb1 = makeObject<UIThumb>();
		//thumb1->setWidth(20);
		//thumb1->setHeight(20);
		
		//auto track = makeObject<UITrack>();
		//track->setOrientation(Orientation::Vertical);
		//track->setWidth(20);
		//track->setHeight(100);
		//track->setMaximum(50);
		//track->setViewportSize(10);
		//Engine::mainUIView()->addElement(track);

        //auto scrollbar = makeObject<UIScrollBar>();
        //scrollbar->setOrientation(Orientation::Vertical);
        //scrollbar->setWidth(20);
        //scrollbar->setHeight(100);
        //scrollbar->setMaximum(50);
        //scrollbar->setViewportSize(10);
        //Engine::mainUIView()->addElement(scrollbar);

        //auto scrollview = makeObject<UIScrollViewer>();
        //scrollview->setWidth(200);
        //scrollview->setHeight(300);
        //Engine::mainUIView()->addElement(scrollview);


        //m_button1 = makeObject<UIButton>();
        //m_button1->setWidth(300);
        //m_button1->setHeight(400);
        //m_button1->setText(u"Lumino");
        //scrollview->addElement(m_button1);


    }

private:
    Ref<UIButton> m_button1;

};

void UISandboxMain()
{
	UISandboxApp app;
	EngineSettings::setMainWindowSize(1000, 600);
	detail::ApplicationHelper::run(&app);
}




