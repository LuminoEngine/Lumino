
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

#include <Lumino.hpp>


class App : public Application
{
	Ref<SphereMesh> s;

	virtual void onInit() override
	{
		auto texture = Texture2D::load(u"picture1.jpg");
		auto sprite = UISprite::create(texture);
	}

	void onUpdate() override
	{
	}

	//virtual void onUpdate() override
	//{
	//	auto light = Engine::light();

	//	
	//	s->setPosition(-light->transform()->getFront() * 5);;

	//	//float x, y;
	//	//Math::sinCos(Engine::time(), &x, &y);
	//	//light->lookAt(x, 0, y);

	//	//s->setPosition(-Vector3(x, 0, y) * 5);
	//}
};

void Tutorial_Sandbox()
{
    App app;
	EngineSettings::setGraphicsDebugEnabled(true);
    EngineSettings::setDebugToolEnabled(false);
	EngineSettings::setMainWindowSize(1600, 800);
    detail::ApplicationHelper::run(&app);
}




