
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>
using namespace ln;

class App_Sandbox_GridListBox : public Application
{

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setPhysicsDebugDrawEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);
		Scene::setClearMode(SceneClearMode::SkyDome);

		auto panel = makeObject<UIGridLayout>();
		panel->setColumnCount(3);

		auto listbox1 = UIListBox::create();
		listbox1->setItemsLayoutPanel(panel);
		//listbox1->setAlignments(UIHAlignment::Stretch, UIVAlignment::Stretch);
		//listbox1->setSize(300, 400);
		

		{
			auto icon = makeObject<UIIcon>();
			icon->setIconName(_TT("file"));
			icon->setFontSize(40);
			//icon->setAlignments(UIHAlignment::Center, UIVAlignment::Top);

			auto text = makeObject<UIText>(_TT("Item1"));
			//text->setAlignments(UIHAlignment::Center, UIVAlignment::Bottom);

			auto item = makeObject<UIListBoxItem>();
			item->addChild(icon);
			item->addChild(text);
			item->setPadding(20);
			item->setMargin(10);
			item->setData(makeVariant(100));
			item->connectOnSubmit([](const UIEventArgs* e) {
				std::cout << "submit: " << static_cast<UIControl*>(e->sender())->dataAs<int>() << std::endl;
			});
			UIGridLayout::setPlacement(item, 0, 0);

			listbox1->addItem(item);
		}

		auto item2 = listbox1->addItem(_TT("item2"));
		UIGridLayout::setPlacement(item2, 0, 1);
		auto item3 = listbox1->addItem(_TT("item3"));
		UIGridLayout::setPlacement(item3, 0, 2);
		auto item4 = listbox1->addItem(_TT("item4"));
		UIGridLayout::setPlacement(item4, 1, 0);

		auto text = makeObject<UIText>(_TT("Inventory"));
		//text->setAlignments(UIHAlignment::Left, UIVAlignment::Top);

		auto layout = makeObject<UIBoxLayout>();
		layout->setOrientation(UILayoutOrientation::Vertical);
		layout->addChild(text);
		layout->addChild(listbox1);

		auto window = UIWindow::create();
		window->setSize(400, 300);
		window->addChild(layout);

		UI::add(window);
    }

    void onUpdate() override
    {
    }
};

void Sandbox_GridListBox()
{
    App_Sandbox_GridListBox app;
	detail::ApplicationHelper::run(&app);
}




