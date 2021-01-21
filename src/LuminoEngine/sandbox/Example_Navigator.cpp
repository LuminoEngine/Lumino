
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIFocusNavigator.hpp>
#include <LuminoEngine/UI/Controls/UIListBox.hpp>
#include <LuminoEngine/Engine/VMProperty.hpp>
using namespace ln;

class App_Example_Navigator : public Application
{
 //   Ref<UIFocusNavigator> m_navigator;
    Ref<UIListBox> m_listbox1;
	Ref<UIListBox> m_listbox2;
    Ref<UIWindow> m_window1;
    Ref<UIWindow> m_window2;

	////ModelProperty<String> m_actorName;
	//ModelProperty<int> m_itemId;
	//ModelProperty<Ref<Collection<Ref<ModelProperty<String>>>>> m_items;

    virtual void onInit() override
    {
  //      Engine::camera()->setBackgroundColor(Color::Gray);

  //      //auto windowSkin = Texture2D::load(u"Window1");

  //      m_navigator = makeObject<UIFocusNavigator>();
  //      m_navigator->setBackgroundColor(Color(1., 1., 1., 0.5));    //Color::Green);
  //      Engine::mainUIView()->addElement(m_navigator);

		//auto layout1 = makeObject<UIHBoxLayout3>();
		//m_navigator->addChild(layout1);


        m_window1 = UIWindow::create();
		m_window1->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
        m_window1->setPosition(10, 10);
        m_window1->setSize(200, 300);
		//m_window1->setMargin(8);
        //m_window1->setBackgroundColor(Color::Red);
		//layout1->addChild(m_window1);


		m_listbox1 = UIListBox::create();
		//m_listbox1->setHAlignment(UIHAlignment::Left);
		//m_listbox1->setVAlignment(UIVAlignment::Top);
		auto item1 = UIListBoxItem::create(u"Item");
		m_listbox1->addChild(item1);
		m_listbox1->addChild(u"Skill");
		m_listbox1->addChild(u"Status");
		m_listbox1->addChild(u"Save");
		m_listbox1->addChild(u"Load");
		//m_listbox1->addChild(u"item4");
		m_window1->addChild(m_listbox1);
		//// m_navigator->addElement(m_listbox1);


		static_cast<UIDomainProvidor*>(Engine::mainUIView())->focusNavigator()->pushFocus(m_window1);


        m_window2 = UIWindow::create();
		m_window2->setAlignments(UIHAlignment::Left, UIVAlignment::Top);
		m_window2->setPosition(220, 20);
		m_window2->setSize(200, 300);
		//m_window2->setMargin(8);
		//m_window2->setBackgroundDrawMode(BrushImageDrawMode::BoxFrame);
		//m_window2->setBackgroundImage(windowSkin);
		//m_window2->setBackgroundImageBorder(8);
		//layout1->addChild(m_window2);

		//auto items = makeCollection<Ref<ModelProperty<String>>>();
		//items->add(makeProperty<String>(u"item1"));
		//items->add(makeProperty<String>(u"item2"));
		//items->add(makeProperty<String>(u"item3"));
		//m_items.set(items);
		m_listbox2 = UIListBox::create();
		auto item2_1 = UIListBoxItem::create(u"item1");
		m_listbox2->addChild(item2_1);
		m_listbox2->addChild(u"item2");
		m_listbox2->addChild(u"item3");
		m_window2->addChild(m_listbox2);
		//// m_navigator->addElement(m_listbox1);




		item1->connectOnSubmit([this, item2_1](auto x) {
			static_cast<UIDomainProvidor*>(Engine::mainUIView())->focusNavigator()->pushFocus(m_window2);
			Debug::print(u"Item clicked. ");
			m_listbox2->selectItem(item2_1);
			item2_1->focus();
		});


		//auto text = UIText::create(u"Test");
		//auto viewProp = text->getViewProperty(u"text");
		////viewProp->bind(&m_actorName, ln::makeObject2<FunctionalPropertyValueConverter>([](Variant* v) { return u"aaa"; }));
		//viewProp->bind(&m_itemId, ln::makeObject2<FunctionalPropertyValueConverter>([](Variant* v) { return makeVariant(u"aaa"); }));
		//m_window2->addChild(text);

		////m_actorName.set(u"Lumino");
		//m_itemId.set(1);

  //      m_navigator->pushFocus(m_window1);
    }

    virtual void onUpdate() override
    {
        //if (Input::triggered(InputButtons::Submit)) {
        //    m_navigator->pushFocus(m_window1);
        //}
        //if (Input::triggered(InputButtons::Cancel)) {
        //    m_navigator->popFocus();
        //}
    }
};

void Example_Navigator()
{
    App_Example_Navigator app;
	detail::ApplicationHelper::run(&app);
}




