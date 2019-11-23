﻿
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/UIButton.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIFocusNavigator.hpp>
#include <LuminoEngine/UI/UIListBox.hpp>
using namespace ln;

class App_Example_Navigator : public Application
{
    Ref<UIFocusNavigator> m_navigator;
    Ref<UIListBox> m_listbox1;
    Ref<UIWindow> m_window1;
    Ref<UIWindow> m_window2;

    virtual void onInit() override
    {
        Engine::mainCamera()->setBackgroundColor(Color::Gray);

        auto windowSkin = Assets::loadTexture(u"Window1");

        m_navigator = makeObject<UIFocusNavigator>();
        m_navigator->setBackgroundColor(Color(1., 1., 1., 0.5));    //Color::Green);
        Engine::mainUIView()->addElement(m_navigator);

        m_listbox1 = UIListBox::create();
        m_listbox1->setHorizontalAlignment(HAlignment::Left);
        m_listbox1->setVerticalAlignment(VAlignment::Top);
        m_listbox1->setWidth(100);
        m_listbox1->setHeight(300);
        m_listbox1->addChild(u"item1");
        m_listbox1->addChild(u"item2");
        m_listbox1->addChild(u"item3");
        m_listbox1->addChild(u"item4");
        m_navigator->addElement(m_listbox1);


        m_window1 = UIWindow::create();
        m_window1->setPosition(10, 10);
        m_window1->setWidth(50);
        m_window1->setHeight(30);
        m_window1->setBackgroundColor(Color::Red);
        m_navigator->addElement(m_window1);

        m_window2 = UIWindow::create();
        m_window2->setPosition(20, 20);
        m_window2->setWidth(50);
        m_window2->setHeight(30);
        m_window2->setBackgroundColor(Color::Blue);
        m_navigator->addElement(m_window2);

        m_navigator->pushFocus(m_window2);
    }

    virtual void onUpdate() override
    {
        if (Input::isTriggered(InputButtons::Submit)) {
            m_navigator->pushFocus(m_window1);
        }
        if (Input::isTriggered(InputButtons::Cancel)) {
            m_navigator->popFocus();
        }
    }
};

void Example_Navigator()
{
    App_Example_Navigator app;
	detail::ApplicationHelper::init(&app);
	detail::ApplicationHelper::run(&app);
}



