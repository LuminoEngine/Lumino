
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIFlexMessageTextArea.hpp>
using namespace ln;

class App_Example_MessageWindow : public Application
{
    Ref<UIButton> m_button1;
    Ref<UIMessageTextArea> m_message1;
    Ref<UIMessageTextArea> m_message2;
    int m_step = 0;

    virtual void onInit() override
    {
        Engine::mainCamera()->setBackgroundColor(Color::Gray);

        auto windowSkin = Assets::loadTexture(u"Window1");

        auto window1 = UIWindow::create();
        window1->setPosition(10, 10);
        window1->setWidth(200);
        window1->setHeight(100);
        window1->setBackgroundImage(windowSkin);
        window1->setBackgroundImageRect(Rect(0, 0, 48, 48));
        window1->setBackgroundImageBorder(Thickness(8));
        window1->setBackgroundDrawMode(BrushImageDrawMode::BoxFrame);
        //Engine::mainUIView()->addElement(window1);

        m_message1 = UIMessageTextArea::create();
        m_message1->setMargin(16);
        m_message1->setText(u"Hello, Lumino!");
        window1->addElement(m_message1);

        auto window2 = UIWindow::create();
        window2->setPosition(10, 10);
        window2->setHorizontalAlignment(HAlignment::Left);
        window2->setVerticalAlignment(VAlignment::Top);
        window2->setWidth(200);
        window2->setHeight(100);
        window2->setBackgroundImage(windowSkin);
        window2->setBackgroundImageRect(Rect(0, 0, 48, 48));
        window2->setBackgroundImageBorder(Thickness(8));
        window2->setBackgroundDrawMode(BrushImageDrawMode::BoxFrame);
        Engine::mainUIView()->addElement(window2);

        m_message2 = UIMessageTextArea::create();
        m_message2->setMargin(16);
        m_message2->setText(
            u"window2->setPosition(10, 10);\n"
            u"window2->setHorizontalAlignment(HAlignment::Left);\n"
            u"window2->setVerticalAlignment(VAlignment::Top);\n"
            u"window2->setWidth(200);\n"
            u"window2->setHeight(100);\n"
            u"window2->setBackgroundImage(windowSkin);\n"
            u"window2->setBackgroundImageRect(Rect(0, 0, 48, 48));\n"
            u"window2->setBackgroundImageBorder(Thickness(8));\n"
            u"window2->setBackgroundDrawMode(BrushImageDrawMode::BoxFrame);");
        m_message2->setViewportLineCount(2);
        window2->addElement(m_message2);
    }

    virtual void onUpdate() override
    {
        if (Input::isTriggered(InputButtons::Submit)) {

            switch (m_step)
            {
            case 0:
                m_message1->setText(u"All in the golden afternoon");
                break;
            case 1:
                m_message1->setText(u"Full leisurely we glide;");
                break;
            case 2:
                m_message1->setText(u"For both our oars, with little skill,");
                break;
            case 3:
                m_message1->setText(u"By little arms are plied,");
                break;
            case 4:
                m_message1->setText(u"While little hands make vain pretence");
                break;
            case 5:
                m_message1->setText(u"Our wanderings to guide.");
                break;
            default:
                break;
            }

            m_step++;
        }
    }
};

void Example_MessageWindow()
{
    App_Example_MessageWindow app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
}




