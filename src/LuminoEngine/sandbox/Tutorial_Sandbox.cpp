
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

class App : public Application
{
	virtual void onUpdate() override
	{
		if (Input::pressed(u"left")) Debug::print(0, u"left");
		if (Input::pressed(u"right")) Debug::print(0, u"right");
		if (Input::pressed(u"up")) Debug::print(0, u"up");
		if (Input::pressed(u"down")) Debug::print(0, u"down");
		if (Input::pressed(u"submit")) Debug::print(0, u"submit");
		if (Input::pressed(u"cancel")) Debug::print(0, u"cancel");
		if (Input::pressed(u"menu")) Debug::print(0, u"menu");
		if (Input::pressed(u"shift")) Debug::print(0, u"shift");
		if (Input::pressed(u"pageup")) Debug::print(0, u"pageup");
		if (Input::pressed(u"pagedown")) Debug::print(0, u"pagedown");
		if (Input::pressed(u"any")) Debug::print(0, u"any");
	}
};

//class App : public Application
//{
//	Ref<SphereMesh> s;
//
//	virtual void onInit() override
//	{
//		//Engine::renderView()->setGuideGridEnabled(true);
//		Engine::camera()->addComponent(CameraOrbitControlComponent::create());
//
//		//auto texture = Texture2D::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/picture1.jpg");
//		//auto sprite = Sprite::create(texture, -1, 6);
//		auto sprite = Sprite::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/picture1.jpg");
//		sprite->setSize(Size(-1, 2));
//
//		s = SphereMesh::create();
//		s->setShadingModel(ShadingModel::UnLighting);
//		s->setColorScale(Color::Yellow);
//	}
//
//	virtual void onUpdate() override
//	{
//		auto light = Engine::light();
//
//		
//		s->setPosition(-light->transform()->getFront() * 5);;
//
//		//float x, y;
//		//Math::sinCos(Engine::time(), &x, &y);
//		//light->lookAt(x, 0, y);
//
//		//s->setPosition(-Vector3(x, 0, y) * 5);
//	}
//};

void Tutorial_Sandbox()
{
    App app;
    EngineSettings::setDebugToolEnabled(false);
    detail::ApplicationHelper::run(&app);
}




