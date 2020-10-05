
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
		listbox1->setSize(300, 400);
		

		auto icon1 = makeObject<UIIcon>();
		icon1->setIconName(u"file");
		icon1->setFontSize(30);

		auto item1 = listbox1->addItem(icon1);
		item1->getGridLayoutInfo()->actualLayoutRow = 0;
		item1->getGridLayoutInfo()->actualLayoutColumn = 0;
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


		//listbox1->addChild(u"item4");
		//listbox1->addChild(u"item4");
		//listbox1->addChild(u"item4");
		//listbox1->addChild(u"item4");
		//listbox1->addChild(u"item4");
		//listbox1->setHeight(200);
		//layout1->addChild(listbox1);
		Engine::ui()->addChild(listbox1);
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




