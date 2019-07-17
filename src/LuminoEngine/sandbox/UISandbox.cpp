
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

		auto thumb1 = makeObject<UIThumb>();
		thumb1->setWidth(20);
		thumb1->setHeight(20);
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




