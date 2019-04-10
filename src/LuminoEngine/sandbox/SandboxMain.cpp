
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "../src/Audio/AudioDecoder.hpp"
#include "Common.hpp"
#include "../src/Engine/EngineDomain.hpp"
#include "../src/Engine/EngineManager.hpp"
#include "../src/Rendering/RenderingManager.hpp"
#include "../src/Rendering/SpriteRenderFeature.hpp"
#include "../src/Rendering/DrawElementListBuilder.hpp"
#include "../src/Rendering/UnLigitingSceneRenderer.hpp"
#include "../src/Rendering/ClusteredShadingSceneRenderer.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Visual/MeshPrimitiveComponent.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Visual/SkinnedMeshComponent.hpp>
#include <LuminoEngine/Scene/SkinnedMesh.hpp>
#include <LuminoEngine/Scene/OffscreenWorldRenderView.hpp>
#include <LuminoEngine/Animation/Animator.hpp>
#include "../src/Mesh/MqoImporter.hpp"
#include "../src/Font/FontManager.hpp"
#include "../src/Font/FontCore.hpp"
#include "../src/Asset/AssetArchive.hpp"
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
using namespace ln;

class TestProcessorNode : public AudioProcessorNode
{
public:
    double step = 0.0;
    float frequency = 440;
    int counter = 0;

    void init()
    {
        AudioProcessorNode::init(0, 2);
    }

    virtual void onAudioProcess(AudioBus* input, AudioBus* output) override
    {
        // TODO: 隠蔽したいところ・・・
        output->setSampleRate(context()->sampleRate());

        AudioChannel* ch1 = output->channel(0);
        AudioChannel* ch2 = output->channel(1);
        float* data1 = ch1->mutableData();
        float* data2 = ch2->mutableData();

        for (int i = 0; i < output->length(); i++) {
            float v = std::sin(2.0 * Math::PI * step);
            data1[i] = data2[i] = v;
            step += frequency / output->sampleRate();//4096;
        }

        //step = 0;
        counter++;
        std::cout << counter << std::endl;
    }
};


class TestRecoderNode : public AudioProcessorNode
{
public:
    std::vector<float> data;
    std::atomic<bool> end;

    void init()
    {
        AudioProcessorNode::init(2, 2);
		end = false;
    }

    virtual void onAudioProcess(AudioBus* input, AudioBus* output) override
    {
        output->copyFrom(input);

        if (data.size() < context()->sampleRate())
        {
            size_t begin = data.size();
            size_t size = input->length() * input->channelCount();
            data.resize(data.size() + size);
            input->mergeToChannelBuffers(data.data() + begin, size);
            printf("put\n");
        }
        else
        {
            end = true;
        }
    }
};

int main(int argc, char** argv)
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	GlobalLogger::addStdErrAdapter();
    EngineSettings::setEngineFeatures(EngineFeature::Experimental);
	EngineSettings::addAssetDirectory(LN_LOCALFILE("Assets"));
	detail::EngineDomain::engineManager()->settings().standaloneFpsControl = true;


    GlobalLogger::addStdErrAdapter();
    EngineSettings::setMainWindowSize(800, 600);
    EngineSettings::setMainBackBufferSize(800, 600);
    EngineSettings::setGraphicsAPI(GraphicsAPI::OpenGL);//GraphicsAPI::Vulkan);//
    EngineSettings::setEngineFeatures(EngineFeature::Experimental);// EngineFeature::Public);// 


	Engine::initialize();



	auto text1 = UITextBlock::create();
	text1->setText("B");
	//text1->setFontSize(22);
	{

		while (Engine::update()) {
		}

	}


	Engine::finalize();
	return 0;


	//{

	//	auto p2world = newObject<PhysicsWorld2D>();
    auto p2world = Engine::mainWorld()->physicsWorld2D();
	auto shape1 = newObject<BoxCollisionShape2D>(Size(10, 2));
	auto body1 = newObject<RigidBody2D>();
	body1->addCollisionShape(shape1);
	p2world->addPhysicsObject(body1);

	auto shape2 = newObject<BoxCollisionShape2D>(Size(1, 1));
	auto body2 = newObject<RigidBody2D>();
	body2->addCollisionShape(shape2);
	body2->setPosition(Vector2(0, 15));
	body2->setMass(1);
	p2world->addPhysicsObject(body2);

	//	for (int i = 0; i < 60; i++) {
	//		p2world->stepSimulation(1.0 / 60);

	//		printf("%4.2f %4.2f\n", body2->position().x, body2->position().y);
	//	}
	//}


    auto ctl = newObject<CameraOrbitControlComponent>();
    Engine::mainCamera()->addComponent(ctl);
    Engine::mainCamera()->setPosition(0, 5, -10);
    Engine::mainCamera()->setBackgroundColor(Color::Gray);


    struct PosColor
    {
        ln::Vector4 pos;
        ln::Vector2 uv;
    };
    PosColor v1[] = {
        { { -1, 1, 0, 1 }, { 0, 0 } },
        { { 1, 1, 0, 1 }, { 0, 1 } },
        { { -1, -1, 0, 1 }, { 1, 0 } },
        { { 1, -1, 0, 1 }, { 1, 1 } },
    };
    auto m_vertexBuffer = ln::newObject<ln::VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
    
	//detail::EngineDomain::fontManager()->registerFontFromFile();
    Font::registerFontFromFile(LN_LOCALFILE("../../../tools/VLGothic/VL-PGothic-Regular.ttf"));

	//detail::FontDesc desc;
	//desc.Family = "VL PGothic";
	//desc.isBold = true;
	//auto font1 = detail::EngineDomain::fontManager()->lookupFontCore(desc);
	//auto font2 = detail::EngineDomain::fontManager()->lookupFontCore(desc);

	//detail::FontGlobalMetrics gm;
	//font2->getGlobalMetrics(&gm);

	//detail::BitmapGlyphInfo bmpInfo;
	////auto bmp = newObject<Bitmap2D>();
	////bmpInfo.glyphBitmap = bmp;
	//bmpInfo.glyphBitmap = nullptr;
	//font2->lookupGlyphBitmap('A', &bmpInfo);
	//bmpInfo.glyphBitmap->transcodeTo(PixelFormat::RGBA32)->save("C:/LocalProj/tmp/test.png");
	//font2->lookupGlyphBitmap('A', &bmpInfo);

	//auto ker = font2->getKerning('i', 'j');

	//printf("");

#if 1
    //auto font = Font::create(u"VL PGothic", 20);

	//auto light1 = AmbientLight::create();
 //   auto light2 = DirectionalLight::create();

    auto tex = newObject<Texture2D>(u"D:/tmp/110220c_as019.png");
    //auto tex = Assets::loadTexture(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/test/Assets/Sprite1.png");
    //tex->drawText(u"Hello!", Rect(0, 0, 100, 100), font, Color::White);
    //auto tex = newObject<Texture2D>(2, 2);
    //auto bmp1 = tex->map(MapMode::Write);
    //bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
    //bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
    //bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
    //bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));

    //auto sprite = newObject<UISprite>();
    //sprite->setTexture(tex);
    //sprite->setPosition(20, 10);
    //auto imageEffect = newObject<ScreenBlurImageEffect>();
    //imageEffect->setAmount(0.7);
    //imageEffect->setRadialScale(1.05);
    //Engine::mainViewport()->addImageEffect(imageEffect);

    //auto sprite = Sprite::create(3, 3, tex);

    //auto tilemap = newObject<Tilemap>();
    //tilemap->setShadingModel(ShadingModel::UnLighting);



    //auto ToneLayer = ToneImageEffect::create();
    //Engine::mainViewport()->addImageEffect(ToneLayer);

    //auto ToneLayer2 = ToneImageEffect::create();
    //Engine::mainViewport()->addImageEffect(ToneLayer2);

    //auto BlurLayer = ScreenBlurImageEffect::create();
    //Engine::mainViewport()->addImageEffect(BlurLayer);

    //auto texture2 = Texture2D::create(100, 100);
    //texture2->clear(Color::Gray);
    ////font->setAntiAlias(false);
    //texture2->drawText(u"Left", Rect(0, 0, 0, 0), font, Color::Black);
    //auto sprite2 = ln::UISprite::create(texture2);
    ////sprite2->setPosition(200, 100);
    //sprite2->setCenterPoint(50, 50);

    //auto mesh1 = newObject<StaticMesh>(u"D:/tmp/cube.obj");
    //auto mesh1 = newObject<StaticMesh>(u"D:/Proj/Volkoff/Engine/Lumino/build/ExternalSource/tinyobjloader/models/cornell_box.obj");
    //auto mesh1 = newObject<StaticMesh>(u"D:/Proj/Volkoff/Engine/Lumino/build/ExternalSource/tinyobjloader/models/usemtl-issue-68.obj");
    //mesh1->setPosition(0, -1, 0);
    //

    //auto mesh2 = newObject<StaticMesh>(u"D:/Proj/Volkoff/Engine/Lumino/build/ExternalSource/tinyobjloader/models/cornell_box.obj");
    //mesh2->setPosition(2, 0, 0);

    //auto mesh3 = newObject<StaticMesh>(u"D:/Proj/TH-10/Assets/Graphics/test/sphere4.obj", 2);
    //mesh3->setVisible(false);

    //auto clip1 = VmdAnimationClip::create(u"D:/MMD/Materials/モーション/Love&Joy/love&joyお面無しver.vmd");

    //auto smesh1 = SkinnedMesh::create(u"D:/MMD/Materials/モデル/Appearance Miku/Appearance Miku.pmx");
    ////smesh1->setBlendMode(BlendMode::Alpha);
    ////smesh1->setEulerAngles(0, Math::PI, 0);
    ////smesh1->setShadingModel(ShadingModel::UnLighting);

    //smesh1->skinnedMeshComponent()->model()->animationController()->addClip(u"anim1", clip1);
    //smesh1->skinnedMeshComponent()->model()->animationController()->play(u"anim1");


    //auto sprite = UISprite::create(Assets::loadTexture(u"D:/MMD/Materials/モデル/Appearance Miku/A4.bmp"));

    //Engine::update();
    //Engine::update();
    //{
    //    auto camera = newObject<Camera>();
    //    camera->setPosition(0, 0, -20);
    //    auto ofs = newObject<OffscreenWorldRenderView>();
    //    auto rt1 = newObject<RenderTargetTexture>(640, 480, TextureFormat::RGBA32, false);
    //    ofs->setRenderTarget(rt1);
    //    ofs->setTargetWorld(Engine::mainWorld());
    //    ofs->setCamera(camera);
    //    ofs->render();
    //    auto bitmap = rt1->readData();
    //    bitmap->save(u"test1.png");
    //}


#endif

#if 0
    //auto shape1 = BoxCollisionShape::create(5, 1, 5);
    //auto body1 = newObject<RigidBody>();
    //body1->addCollisionShape(shape1);
    //body1->setTransform(Matrix::makeTranslation(0, -3, 0));
    //Engine::mainPhysicsWorld()->addPhysicsObject(body1);


    //auto shape2 = CapsuleCollisionShape::create(0.5, 3);
    //auto body2 = newObject<RigidBody>();
    //body2->addCollisionShape(shape2);
    //body2->setTransform(Matrix::makeTranslation(0, -2, 0));
    //body2->setMass(1.0f);
    //body2->setKinematic(true);
    //Engine::mainPhysicsWorld()->addPhysicsObject(body2);

    //auto body3 = newObject<SoftBody>();
    //body3->createFromMesh(mesh3->staticMeshComponent()->model()->meshContainers().front()->meshResource(), Engine::mainPhysicsWorld());

    auto tex2 = Assets::loadTexture(u"D:/Documents/Modeling/grid_uv_2.png");
    List<Ref<WorldObject>> spheres;
    for (int y = 0; y < 5; y++)
    {
        for (int i = 0; i < 5; i++)
        {
            auto obj2 = newObject<WorldObject>();
            auto cmp2 = newObject<SphereComponent>();
            auto mat2 = Material::create();
            mat2->setMetallic(static_cast<float>(i) / 5);
            mat2->setRoughness(std::max(static_cast<float>(y) / 5, 0.001f));
            //mat2->setMainTexture(tex2);
            obj2->addComponent(cmp2);
            obj2->setPosition(i, -y, 0);
            cmp2->setMaterial(mat2);
            spheres.add(obj2);
        }
    }

    auto plane1 = newObject<WorldObject>();
    auto planecmp2 = newObject<PlaneComponent>();
    auto planemat2 = Material::create();
    //planemat2->setMetallic(0.1);
    //planemat2->setRoughness(0.1);
    plane1->addComponent(planecmp2);
    planecmp2->setMaterial(planemat2);

    auto light1 = PointLight::create();
    light1->setPosition(-3, 0.1, -2);

    auto light2 = SpotLight::create();
    light2->setPosition(3, 0.1, -2);
#endif

#if 0
    auto meshMaterial = Material::create();
    meshMaterial->setMainTexture(tex);

    auto meshRes = newObject<MeshResource>();
    meshRes->resizeVertexBuffer(4);
    meshRes->resizeIndexBuffer(6);
    meshRes->resizeSections(1);
    meshRes->setVertex(0, ln::Vertex{ Vector3(-3, 2, 0), -Vector3::UnitZ, Vector2(0, 0), Color::White });
    meshRes->setVertex(1, ln::Vertex{ Vector3(3, 1, 0), -Vector3::UnitZ, Vector2(1, 0), Color::White });
    meshRes->setVertex(2, ln::Vertex{ Vector3(-3, -1, 0), -Vector3::UnitZ, Vector2(0, 1), Color::White });
    meshRes->setVertex(3, ln::Vertex{ Vector3(3, -1, 0), -Vector3::UnitZ, Vector2(1, 1), Color::White });
    meshRes->setIndex(0, 0);
    meshRes->setIndex(1, 1);
    meshRes->setIndex(2, 2);
    meshRes->setIndex(3, 2);
    meshRes->setIndex(4, 1);
    meshRes->setIndex(5, 3);
    meshRes->setSection(0, 0, 2, 0);

    auto meshContainer = newObject<MeshContainer>();
    meshContainer->setMeshResource(meshRes);

    auto meshModel = newObject<StaticMeshModel>();
    meshModel->addMeshContainer(meshContainer);
    meshModel->addMaterial(meshMaterial);

    auto mesh1 = newObject<StaticMesh>();
    mesh1->staticMeshComponent()->setModel(meshModel);
#endif

#if 0
    //auto sound = newObject<Sound>(u"D:\\tmp\\4_Battle_win.wav");
    auto sound = newObject<Sound>(u"D:/Music/momentum/02 - momentum.wav");
    //auto sound = newObject<Sound>(u"D:/Proj/Volkoff/Assets/Data/Sound/BGM/monochrome.ogg");
    sound->play();
    //sound->setPitch(1.2);

    //GameAudio::playBGM(u"D:/Proj/Volkoff/Assets/Data/Sound/BGM/monochrome.ogg");
#endif
#if 0
    //auto source = newObject<AudioSourceNode>(u"D:/Tech/Audio/WebAudioTest1/2018-11-29T13_00_15.686Z.wav");
    auto source = newObject<AudioSourceNode>(u"D:/Music/momentum/02 - momentum.wav");
    //auto source = newObject<TestProcessorNode>();
    //AudioNode::connect(source, AudioContext::primary()->destination());
    
    //auto recoder = newObject<TestRecoderNode>();
    //AudioNode::connect(source, recoder);
    //AudioNode::connect(recoder, AudioContext::primary()->destination());

    auto gain = newObject<AudioGainNode>();
    AudioNode::connect(source, gain);
    AudioNode::connect(gain, AudioContext::primary()->destination());

    source->start();

    //auto filedata = d.readAllSamples();
#endif
    //GameAudio::playBGM(u"D:/Music/momentum/02 - momentum.wav");



    //auto curve1 = KeyFrameAnimationCurve::create();
    //curve1->addKeyFrame(0, 0.0f, TangentMode::Tangent);
    //curve1->addKeyFrame(2, 100.0f, TangentMode::Linear);
    //curve1->addKeyFrame(5, 100.0f, TangentMode::Tangent);
    //curve1->addKeyFrame(7, 200.0f, TangentMode::Constant);

    //auto track1 = ScalarAnimationTrack::create();
    //track1->setCurve(curve1);





    //Engine::update();

    //Engine::terminate();
    //return 0;

    auto uitext = UITextBlock::create();
    uitext->setText(u"text");


    float time = 0;
    int frameCount = 0;
    while (Engine::update())
    {
        if (Input::isTriggered(InputButtons::Submit)) {
            GameAudio::playSE(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/test/Assets/Audio/coin04_16bit_mono.wav");
            //GameAudio::playSE(u"D:/Proj/Volkoff/Assets/Data/Sound/SE/coin04.wav");
            //GameAudio::playSE(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/test/Assets/Audio/sin_440_3s_48000_2ch.wav");
            //GameAudio::playSE(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/test/Assets/Audio/coin04.wav");
        }

        //BlurLayer->play(0.7f, Vector2(0, 0.0), 1.05);

        //sprite->setPosition(0, track1->evaluate(time));
        //std::cout << track1->evaluate(time) << std::endl;
        time += 0.016;
#if 0
        if (Mouse::isPressed(MouseButtons::Left))
        {
            float pitch = (Mouse::position().x / 640)  + 0.5;
            float volume = (Mouse::position().y / 480);
            //std::cout << pitch << std::endl;
            sound->setPitch(pitch);
            sound->setVolume(volume);
        }
#endif
        //if (recoder->end)
        //{
        //    printf("a\n");
        //}
        frameCount++;
        
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

	auto decl = newObject<VertexLayout>();
	decl->addElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);

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

		auto meshModel = newObject<StaticMeshModel>();
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

	Engine::finalize();

	return 0;
}

