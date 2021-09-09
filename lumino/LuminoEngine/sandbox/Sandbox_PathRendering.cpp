
#include <LuminoEngine.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
using namespace ln;

class TestElement : public UIElement
{
public:
    void onRender(UIRenderingContext* context) override
    {
        //context->beginPath();
        //context->endPath();

        context->drawRegularPolygonPrimitive(3, 5, Color::Red, true);
    }

private:
};

class App_Sandbox_PathRendering : public Application
{

    void onInit() override
    {
        auto e = makeObject<TestElement>();
        UI::add(e);
    }

    void onUpdate() override
    {
    }
};

void Sandbox_PathRendering()
{
    App_Sandbox_PathRendering app;
	detail::ApplicationHelper::run(&app);
}




