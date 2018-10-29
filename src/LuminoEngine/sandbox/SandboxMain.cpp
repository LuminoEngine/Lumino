
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "../src/Audio/Mp3AudioDecoder.hpp"
#include "Common.hpp"
#include "../src/Engine/EngineDomain.hpp"
#include "../src/Rendering/RenderingManager.hpp"
#include "../src/Rendering/SpriteRenderFeature.hpp"
#include "../src/Rendering/DrawElementListBuilder.hpp"
#include "../src/Rendering/UnLigitingSceneRenderer.hpp"
#include "../src/Rendering/ClusteredShadingSceneRenderer.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include "../src/Mesh/MqoImporter.hpp"
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
	Engine::initialize();

	auto light1 = AmbientLight::create();

    auto tex = newObject<Texture2D>(u"D:/tmp/110220c_as019.png");
    //auto tex = newObject<Texture2D>(2, 2);
    //auto bmp1 = tex->map(MapMode::Write);
    //bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
    //bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
    //bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
    //bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));
    auto sprite = newObject<Sprite>();
    sprite->setTexture(tex);


    while (Engine::update())
    {
    }


#if 0
	{
		auto ss = FileStream::create(u"D:\\tmp\\light_song_instrumental_0.mp3");
		detail::Mp3AudioDecoder dec;
		auto diag = newObject<DiagnosticsManager>();
		dec.initialize(ss, diag);
	}

	
#if 0
	auto source = newObject<AudioSourceNode>(u"D:\\tmp\\8_MapBGM2.wav");
	auto panner = newObject<AudioPannerNode>();
	AudioNode::connect(source, panner);
	AudioNode::connect(panner, AudioContext::primary()->destination());
	source->setPlaybackRate(1.2);
	source->start();
#else
	auto source = newObject<AudioSourceNode>(u"D:\\tmp\\3_EventScene_variation2.wav");
	//auto source = newObject<AudioSourceNode>(u"D:\\tmp\\8_MapBGM2.wav");
	AudioNode::connect(source, AudioContext::primary()->destination());
	//source->setPlaybackRate(1.2);
	source->start();
#endif

	auto shader = Shader::create(
		LN_LOCALFILE("Assets/simple.vert"),
		LN_LOCALFILE("Assets/simple.frag"));
	//shader->setVector("g_color", Vector4(0, 1, 0, 1));

	//Engine::graphicsContext()->setShaderPass(shader->techniques()[0]->passes()[0]);

	struct Vertex
	{
		Vector4 pos;
	};
	Vertex v[] = {
		Vector4(0, 1, 0, 1),
		Vector4(-1, 1, 0, 1),
		Vector4(0, -1, 0, 1),
	};

	auto vb = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto decl = newObject<VertexDeclaration>();
	decl->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

	//auto renderTarget = newObject<RenderTargetTexture>(32, 32, TextureFormat::RGBX32, false);




	while (Engine::update())
	{
		class TestRenderView : public RenderView
		{
		public:
			// 本番では、World が持っていたりする。
			Ref<detail::WorldSceneGraphRenderingContext> m_context;

			Ref<detail::DrawElementListCollector> m_elementListManager;

			//Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;
			Ref<detail::ClusteredShadingSceneRenderer> m_sceneRenderer;
			
			TestRenderView()
			{
				m_elementListManager = makeRef<detail::DrawElementListCollector>();
				m_context = makeRef<detail::WorldSceneGraphRenderingContext>();
				m_elementListManager->addDrawElementList(detail::RendringPhase::Default, m_context->m_elementList);
				addDrawElementListManager(m_elementListManager);

				//m_sceneRenderer = makeRef<detail::UnLigitingSceneRenderer>();
				//m_sceneRenderer->initialize(detail::EngineDomain::renderingManager());
				m_sceneRenderer = makeRef<detail::ClusteredShadingSceneRenderer>();
				m_sceneRenderer->initialize(detail::EngineDomain::renderingManager());
			}

			void render()
			{

				auto swapChain = Engine::mainWindow()->swapChain();
				FrameBuffer fb;
				fb.renderTarget[0] = swapChain->colorBuffer();
				fb.depthBuffer = swapChain->depthBuffer();

				{
					Size size(fb.renderTarget[0]->width(), fb.renderTarget[0]->height());
					Vector3 pos = Vector3(5, 5, -5);
					mainCameraInfo.makePerspective(pos, Vector3::normalize(Vector3::Zero - pos), Math::PI / 3.0f, size, 0.1f, 100.0f);

				}

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

#if 1
		auto diag = newObject<DiagnosticsManager>();
		detail::MqoImporter importer;
		auto meshModel = importer.import(detail::EngineDomain::meshManager(),
			u"D:\\Documents\\LuminoProjects\\TestModels\\grass-1.mqo", diag);
		auto meshContainer = meshModel->meshContainers()[0];

		//meshModel->materials()[0]->setMainTexture(tex1);

#else
		auto meshRes = newObject<MeshResource>();
		meshRes->resizeVertexBuffer(4);
		meshRes->resizeIndexBuffer(6);
		meshRes->resizeSections(1);
		meshRes->setVertex(0, Vertex{ Vector3(-3, 1, 0), -Vector3::UnitZ, Vector2(0, 0), Color::White });
		meshRes->setVertex(1, Vertex{ Vector3(3, 1, 0), -Vector3::UnitZ, Vector2(1, 0), Color::White });
		meshRes->setVertex(2, Vertex{ Vector3(-3, -1, 0), -Vector3::UnitZ, Vector2(0, 1), Color::White });
		meshRes->setVertex(3, Vertex{ Vector3(3, -1, 0), -Vector3::UnitZ, Vector2(1, 1), Color::White });
		meshRes->setIndex(0, 0);
		meshRes->setIndex(1, 1);
		meshRes->setIndex(2, 2);
		meshRes->setIndex(3, 2);
		meshRes->setIndex(4, 1);
		meshRes->setIndex(5, 3);
		meshRes->setSection(0, 0, 2, 0);

		auto meshContainer = newObject<MeshContainer>();
		meshContainer->setMeshResource(meshRes);

		auto meshModel = newObject<MeshModel>();
		meshModel->addMeshContainer(meshContainer);
		meshModel->addMaterial(material);
#endif

		while (Engine::update())
		{
			renderView->m_context->reset();

			renderView->m_context->setCullingMode(CullMode::None);

			renderView->m_context->drawMesh(meshContainer, 0);

			renderView->m_context->addPointLight(Color::White, 1.0, Vector3(0, 1, -1), 10.0, 1.0);

			renderView->render();
		}
#endif
#if 0
		class TestRenderView : public RenderView
		{
		public:
			// 本番では、World が持っていたりする。
			Ref<detail::WorldSceneGraphRenderingContext> m_context;

			Ref<detail::DrawElementListCollector> m_elementListManager;

			Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;

			TestRenderView()
			{
				m_elementListManager = makeRef<detail::DrawElementListCollector>();
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
#endif
#if 0
		class TestRenderView : public RenderView
		{
		public:
			// 本番では、World が持っていたりする。
			Ref<detail::DrawElementListCollector> m_elementListManager;
			Ref<detail::DrawElementList> m_elementList;

			Ref<detail::UnLigitingSceneRenderer> m_sceneRenderer;

			TestRenderView()
			{
				m_elementListManager = makeRef<detail::DrawElementListCollector>();
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
	}
#endif

	Engine::terminate();

	return 0;
}

