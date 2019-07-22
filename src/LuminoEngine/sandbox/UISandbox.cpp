
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
using namespace ln;




class UISandboxApp : public Application
{
public:
    virtual void onCreate()
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
		
		//{
		//	auto button1 = makeObject<UIButton>();
		//	button1->setWidth(200);
		//	button1->setHeight(32);
		//	button1->setText(u"Push button");
		//	button1->setHorizontalAlignment(HAlignment::Right);
		//	Engine::mainUIRoot()->addElement(button1);
		//}

		{
			auto button1 = makeObject<UIToggleButton>();
			button1->setWidth(200);
			button1->setHeight(32);
			button1->setText(u"Toggle button");
			button1->setHorizontalAlignment(HAlignment::Right);
			button1->setVerticalAlignment(VAlignment::Center);
			Engine::mainUIRoot()->addElement(button1);
		}


		//auto thumb1 = makeObject<UIThumb>();
		//thumb1->setWidth(20);
		//thumb1->setHeight(20);
		
		//auto track = makeObject<UITrack>();
		//track->setOrientation(Orientation::Vertical);
		//track->setWidth(20);
		//track->setHeight(100);
		//track->setMaximum(50);
		//track->setViewportSize(10);
		//Engine::mainUIRoot()->addElement(track);

        //auto scrollbar = makeObject<UIScrollBar>();
        //scrollbar->setOrientation(Orientation::Vertical);
        //scrollbar->setWidth(20);
        //scrollbar->setHeight(100);
        //scrollbar->setMaximum(50);
        //scrollbar->setViewportSize(10);
        //Engine::mainUIRoot()->addElement(scrollbar);

        //auto scrollview = makeObject<UIScrollViewer>();
        //scrollview->setWidth(200);
        //scrollview->setHeight(300);
        //Engine::mainUIRoot()->addElement(scrollview);


        //m_button1 = makeObject<UIButton>();
        //m_button1->setWidth(300);
        //m_button1->setHeight(400);
        //m_button1->setText(u"Lumino");
        //scrollview->addElement(m_button1);


    }

private:
    Ref<UIButton> m_button1;

};

int UISandboxMain()
{
	UISandboxApp app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
	return 0;
}




