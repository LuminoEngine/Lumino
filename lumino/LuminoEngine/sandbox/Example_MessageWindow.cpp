
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>
#include <LuminoEngine/UI/Controls/UIScrollView.hpp>
#include <LuminoEngine/UI/UIItemsModel.hpp>
#include <LuminoEngine/UI/UIItemsElement.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIMessageTextArea.hpp>
using namespace ln;

class App_Example_MessageWindow : public Application
{
public:
    Ref<UIButton> m_button1;
    Ref<UIMessageTextWindow> m_window1;
    //Ref<UIMessageTextArea> m_message1;
    Ref<UIMessageTextArea> m_message2;
    int m_step = 0;

    App_Example_MessageWindow()
    {
        ln::EngineSettings::setFontFile(_TT("C:/Proj/LN/PrivateProjects/HC4/assets/Font/mplus-1c-regular.ttf"));
    }

    virtual void onInit() override
    {
        Engine::mainCamera()->setBackgroundColor(Color::Gray);

        auto windowSkin = Texture2D::load(_TT("Window1"));

        //m_window1 = UIMessageTextWindow::create();
        //m_window1->setHAlignment(UIHAlignment::Left);
        //m_window1->setVAlignment(UIVAlignment::Top);
        //m_window1->setPosition(10, 10);
        //m_window1->setWidth(200);
        //m_window1->setHeight(80);
        //m_window1->setBackgroundImage(windowSkin);
        //m_window1->setBackgroundImageRect(Rect(0, 0, 48, 48));
        //m_window1->setBackgroundImageBorder(Thickness(8));
        //m_window1->setBackgroundDrawMode(Sprite9DrawMode::StretchedBoxFrame);
        //m_window1->setBackgroundColor(Color::Transparency);
        //m_window1->setBorderThickness(0);
        ////Engine::mainUIView()->addElement(window1);
        //m_window1->setText(_TT("Hello, Lumino!"));

        //m_message1 = UIMessageTextArea::create();
        //m_message1->setMargin(16);
        //m_message1->setText(_TT("Hello, Lumino!"));
        //window1->addElement(m_message1);

        auto window2 = UIWindow::create();
        //window2->setPosition(10, 100);
        //window2->setHAlignment(UIHAlignment::Stretch);
        //window2->setVAlignment(UIVAlignment::Stretch);
        window2->setWidth(400);
        //window2->setHeight(100);
        //window2->setBackgroundColor(Color(0,0,0,0.5));
        window2->setBackgroundImage(windowSkin);
        window2->setBackgroundImageRect(Rect(0, 0, 48, 48));
        window2->setBackgroundImageBorder(Thickness(8));
        window2->setBackgroundDrawMode(Sprite9DrawMode::StretchedBoxFrame);
        UI::add(window2);

        m_message2 = UIMessageTextArea::create();
        m_message2->setMargin(16);
        //m_message2->setHeight(50);
        m_message2->setBackgroundColor(Color::DarkCyan);
        m_message2->setText(
            _TT("window2->setPosition(10, 10);\n")
            _TT("window2->setHAlignment(UIHAlignment::Left);\n")
            _TT("window2->setVAlignment(UIVAlignment::Top);\n")
            _TT("window2->setWidth(200);\n")
            _TT("window2->setHeight(100);\n")
            _TT("window2->setBackgroundImage(windowSkin);\n")
            _TT("window2->setBackgroundImageRect(Rect(0, 0, 48, 48));\n")
            _TT("window2->setBackgroundImageBorder(Thickness(8));\n")
            _TT("window2->setBackgroundDrawMode(BrushImageDrawMode::BoxFrame);"));
        //m_message2->setText(_TT("日本語表示\n\nbbbbb\nccccc");
        //m_message2->setViewportLineCount(2);
        //m_message2->setClipToBounds(true);
        window2->addChild(m_message2);
    }

    virtual void onUpdate() override
    {
        if (Input::isTriggered(InputButtons::Submit)) {

            switch (m_step)
            {
            case 0:
                m_window1->setText(_TT("All in the golden afternoon"));
                break;
            case 1:
                m_window1->setText(_TT("Full leisurely we glide;"));
                break;
            case 2:
                m_window1->setText(_TT("For both our oars, with little skill,"));
                break;
            case 3:
                m_window1->setText(_TT("By little arms are plied,"));
                break;
            case 4:
                m_window1->setText(_TT("While little hands make vain pretence"));
                break;
            case 5:
                m_window1->setText(_TT("Our wanderings to guide."));
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
	detail::ApplicationHelper::run(&app);
}




