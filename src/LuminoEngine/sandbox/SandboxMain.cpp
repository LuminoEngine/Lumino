
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "Common.hpp"
#include "../src/Engine/EngineDomain.hpp"
#include "../src/Rendering/RenderingManager.hpp"
#include "../src/Rendering/SpriteRenderFeature.hpp"
#include "../src/Rendering/DrawElementListBuilder.hpp"
#include "../src/Rendering/UnLigitingSceneRenderer.hpp"
#include <Lumino/Rendering/Material.hpp>
#include <Lumino/Rendering/RenderingContext.hpp>
using namespace ln;

class TestRenderView
	: public RenderView
{
public:
	void drawScene();
	void render();

	FrameBuffer frameBuffer;
};

int main(int argc, char** argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif



	GlobalLogger::addStdErrAdapter();
	//EngineSettings::setMainWindowSize(80, 60);
	//EngineSettings::setMainBackBufferSize(80, 60);
	Engine::initialize();

	{
		class TestRenderView : public RenderView
		{
		public:
			// 本番では、World が持っていたりする。
			Ref<detail::WorldSceneGraphRenderingContext> m_context;

			Ref<detail::DrawElementListManager> m_elementListManager;

			Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;

			TestRenderView()
			{
				m_elementListManager = makeRef<detail::DrawElementListManager>();
				m_context = makeRef<detail::WorldSceneGraphRenderingContext>();
				m_elementListManager->addDrawElementList(detail::RendringPhase::Default, m_context->m_elementList);
				addDrawElementListManager(m_elementListManager);

				m_sceneRenderer = makeRef<detail::UnLigitingSceneRenderer>();
				m_sceneRenderer->initialize(detail::EngineDomain::renderingManager());
			}

			void render()
			{

				auto swapChain = Engine::mainWindow()->swapChain();
				FrameBuffer fb;
				fb.renderTarget[0] = swapChain->colorBuffer();
				fb.depthBuffer = swapChain->depthBuffer();

				RenderView::render(Engine::graphicsContext(), fb, m_sceneRenderer);
			}
		};

		auto renderView = newObject<TestRenderView>();

		auto tex1 = newObject<Texture2D>(2, 2);
		auto bmp1 = tex1->map(MapMode::Write);
		bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
		bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
		bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
		bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));
		auto material = Material::create();
		material->setMainTexture(tex1);

		while (Engine::update())
		{
			renderView->m_context->reset();

			renderView->m_context->drawSprite(
				Matrix(), Size(1, 1), Vector2(0, 0), Rect(0, 0, 1, 1), Color::Blue,
				SpriteBaseDirection::ZMinus, BillboardType::None, material);

			renderView->render();

			if (::GetKeyState('Z') < 0)
			{
				break;
			}
		}
#if 0
		class TestRenderView : public RenderView
		{
		public:
			// 本番では、World が持っていたりする。
			Ref<detail::DrawElementListManager> m_elementListManager;
			Ref<detail::DrawElementList> m_elementList;

			Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;

			TestRenderView()
			{
				m_elementListManager = makeRef<detail::DrawElementListManager>();
				m_elementList = makeRef<detail::DrawElementList>(detail::EngineDomain::renderingManager());
				m_elementListManager->addDrawElementList(detail::RendringPhase::Default, m_elementList);
				addDrawElementListManager(m_elementListManager);

				m_sceneRenderer = makeRef<detail::UnLigitingSceneRenderer>();
				m_sceneRenderer->initialize(detail::EngineDomain::renderingManager());
			}

			void render()
			{
				auto swapChain = Engine::mainWindow()->swapChain();
				FrameBuffer fb;
				fb.renderTarget[0] = swapChain->colorBuffer();
				fb.depthBuffer = swapChain->depthBuffer();

				RenderView::render(Engine::graphicsContext(), fb, m_sceneRenderer);
			}
		};

		auto renderView = newObject<TestRenderView>();

		auto builder = detail::EngineDomain::renderingManager()->renderStageListBuilder();
		builder->setTargetList(renderView->m_elementList);

		auto spriteRender = detail::EngineDomain::renderingManager()->spriteRenderFeature();



		auto tex1 = newObject<Texture2D>(2, 2);
		auto bmp1 = tex1->map(MapMode::Write);
		bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
		bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
		bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
		bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));
		auto material = Material::create();
		material->setMainTexture(tex1);

		while (Engine::update())
		{
			renderView->m_elementList->clear();
			builder->reset();

			class DrawSprite : public detail::RenderDrawElement
			{
			public:
				virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
				{
					static_cast<detail::SpriteRenderFeature*>(renderFeatures)->drawRequest(
						Matrix(), Vector2(1, 1), Vector2(0, 0), Rect(0, 0, 1, 1), Color::Blue, SpriteBaseDirection::ZMinus, BillboardType::None);
				}
			};

			// drawSprite
			{
				builder->setMaterial(material);
				auto* element = builder->addNewDrawElement<DrawSprite>(spriteRender, builder->spriteRenderFeatureStageParameters());
			}

			renderView->render();

			if (::GetKeyState('Z') < 0)
			{
				break;
			}
		}
#endif
#if 0
		auto shader = Shader::create(LN_LOCALFILE("Assets/SpriteTest.hlsl"));

		//auto shader = Shader::create(
		//	LN_LOCALFILE("Assets/simple.vert"),
		//	LN_LOCALFILE("Assets/simple.frag"));
		//shader->setVector("g_color", Vector4(0, 1, 0, 1));

		//Engine::graphicsContext()->setShaderPass(shader->techniques()[0]->passes()[0]);

		//auto tex1 = newObject<Texture2D>(LN_LOCALFILE("Assets/Sprite1.png"));
		auto tex1 = newObject<Texture2D>(2, 2);
		auto bmp1 = tex1->map(MapMode::Write);
		bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
		bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
		bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
		bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));

		ShaderParameter* param = shader->findParameter("g_texture1");
		param->setTexture(tex1);

		auto ctx = Engine::graphicsContext();
		//ctx->setColorBuffer(0, Engine::mainWindow()->swapChain()->colorBuffer());

		auto sr = detail::EngineDomain::renderingManager()->spriteRenderFeature();

		auto list = makeRef<detail::DrawElementList>(detail::EngineDomain::renderingManager());
		auto builder = detail::EngineDomain::renderingManager()->renderStageListBuilder();
		builder->setTargetList(list);

		int loop = 0;
		while (Engine::update())
		{
			ctx->setShaderPass(shader->techniques()[0]->passes()[0]);

			list->clear();
			builder->reset();
			class DrawSprite : public detail::RenderDrawElement
			{
			public:
				virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures) override
				{
					static_cast<detail::SpriteRenderFeature*>(renderFeatures)->drawRequest(
						Matrix(), Vector2(1, 1), Vector2(0, 0), Rect(0, 0, 1, 1), Color::Blue, SpriteBaseDirection::ZMinus, BillboardType::None);
				}
			};

			auto* element = builder->addNewDrawElement<DrawSprite>(sr, builder->spriteRenderFeatureStageParameters());

			auto* ie = list->headElement();
			while (ie)
			{
				auto* stage = ie->stage();
				// TODO: applystate
				ie->onDraw(ctx, stage->renderFeature);
				stage->flush();
				ie = ie->next();
			}

			//ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

			//sr->drawRequest(Matrix(), Vector2(1, 1), Vector2(0, 0), Rect(0, 0, 1, 1), Color::Blue, SpriteBaseDirection::ZMinus, BillboardType::None);
			//sr->flush();

			if (::GetKeyState('Z') < 0)
			{
				break;
			}
		}
#endif
	}

	Engine::terminate();

	return 0;
}

