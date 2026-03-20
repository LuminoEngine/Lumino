
#include <LuminoFramework.hpp>
using namespace ln;

class App_Example_Shader : public Application
{
    virtual void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());

        auto plane = PlaneMesh::create();
        plane->setScale(5);

        auto shader = Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/sandbox/Assets/Shader/Ring.fx"));
        auto mat = Material::create();
        mat->setShader(shader);
        plane->planeMeshComponent()->setMaterial(mat);
    }

    virtual void onUpdate() override
    {
    }
};

void Example_Shader()
{
    App_Example_Shader app;
	detail::ApplicationHelper::run(&app);
}




