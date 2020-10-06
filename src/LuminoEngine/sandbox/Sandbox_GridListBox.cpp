
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIIcon.hpp>
using namespace ln;

class App_Sandbox_GridListBox : public Application
{

    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::renderView()->setPhysicsDebugDrawEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);
		Scene::setClearMode(SceneClearMode::SkyDome);

		auto panel = makeObject<UIGridLayout>();
		panel->setColumnCount(3);

		auto listbox1 = UIListBox::create();
		listbox1->setItemsLayoutPanel(panel);
		//listbox1->setSize(300, 400);
		

		{
			auto icon = makeObject<UIIcon>();
			icon->setIconName(u"file");
			icon->setFontSize(30);

			auto text = makeObject<UITextBlock>(u"Item1");

			auto item = makeObject<UIListBoxItem>();
			item->addChild(icon);
			item->addChild(text);
			item->getGridLayoutInfo()->actualLayoutRow = 0;
			item->getGridLayoutInfo()->actualLayoutColumn = 0;

			listbox1->addItem(item);
		}

		auto item2 = listbox1->addItem(u"item2");
		item2->getGridLayoutInfo()->actualLayoutRow = 0;
		item2->getGridLayoutInfo()->actualLayoutColumn = 1;
		auto item3 = listbox1->addItem(u"item3");
		item3->getGridLayoutInfo()->actualLayoutRow = 0;
		item3->getGridLayoutInfo()->actualLayoutColumn = 2;
		auto item4 = listbox1->addItem(u"item4");
		item4->getGridLayoutInfo()->actualLayoutRow = 1;
		item4->getGridLayoutInfo()->actualLayoutColumn = 0;

		auto window = UIWindow::create();
		window->setSize(400, 300);
		window->addChild(listbox1);

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




