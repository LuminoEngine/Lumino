
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIScrollView.hpp>
using namespace ln;

class UISandboxApp : public Application
{
public:
    virtual void onCreate()
    {
        //m_button1 = makeObject<UIButton>();
        //m_button1->setWidth(200);
        //m_button1->setHeight(100);
        //m_button1->setText(u"Lumino");

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

        auto scrollbar = makeObject<UIScrollBar>();
        scrollbar->setOrientation(Orientation::Vertical);
        scrollbar->setWidth(20);
        scrollbar->setHeight(100);
        scrollbar->setMaximum(50);
        scrollbar->setViewportSize(10);
        Engine::mainUIRoot()->addElement(scrollbar);
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




