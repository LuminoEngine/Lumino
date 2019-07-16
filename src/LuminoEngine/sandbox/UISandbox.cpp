
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
using namespace ln;

class UISandboxApp : public Application
{
public:
    virtual void onCreate()
    {
        m_button1 = makeObject<UIButton>();
        m_button1->setWidth(200);
        m_button1->setHeight(100);
        m_button1->setText(u"Lumino");
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




