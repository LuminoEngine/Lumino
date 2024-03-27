
#include <LuminoFramework.hpp>
#include <LuminoFramework/Tilemap/Voxel.hpp>
using namespace ln;

namespace ln {
	namespace detail {
		extern RenderTargetTexture* g_m_shadowMap;
	}
}

class App_Experiment_Shadow : public Application
{
	Ref<Sprite> m_sprite;

    void onInit() override
    {
		Engine::renderView()->setGuideGridEnabled(true);
		Engine::mainCamera()->addComponent(CameraOrbitControlComponent::create());
		Engine::mainLight()->setShadowEffectiveDistance(50);
		Engine::mainLight()->setShadowEffectiveDepth(500);

		auto plane1 = PlaneMesh::create();

		auto box1 = BoxMesh::With().build();
		box1->setPosition(2, 2, 0);

		m_sprite = Sprite::create();
		m_sprite->setPosition(3, 3, 0);
		m_sprite->setTexture(Texture2D::load(_TT("Sprite1")));
	}

    void onUpdate() override
    {
		auto light = Engine::mainLight();
		//light->setPosition(50, 50, 50);
		//light->lookAt(0, 0, 0);


		if (ln::detail::g_m_shadowMap) {
			m_sprite->setTexture(ln::detail::g_m_shadowMap);
		}
    }
};

void Experiment_Shadow()
{
	App_Experiment_Shadow app;
	detail::ApplicationHelper::run(&app);
}




