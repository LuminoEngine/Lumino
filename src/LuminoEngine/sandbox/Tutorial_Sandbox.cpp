
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIComboBox.hpp>
using namespace ln;

#include <Lumino.hpp>

class App : public Application
{
	Ref<BoxMesh> box;    // (1)

	void onInit() override
	{
		box = BoxMesh::create();

		auto camera = Engine::camera();
		camera->setPosition(5, 5, -5);
		camera->lookAt(0, 0, 0);
	}

	void onUpdate() override
	{
		auto raycaster = Raycaster::fromScreen(Mouse::position());  // (2)
		if (auto result = raycaster->intersectPlane(0, 1, 0)) {     // (3)
			box->setPosition(result->point());                      // (4)
		}
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
//		s->setShadingModel(ShadingModel::Unlit);
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




