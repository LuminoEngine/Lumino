
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "Common.hpp"
#include "../src/Engine/EngineDomain.hpp"
#include "../src/Engine/EngineManager.hpp"
#include "../src/Rendering/RenderingManager.hpp"
#include "../src/Rendering/DrawElementListBuilder.hpp"
#include "../src/Rendering/UnLigitingSceneRenderer.hpp"
#include "../src/Rendering/ClusteredShadingSceneRenderer.hpp"
#include <LuminoEngine/Engine/Property.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Animation/Animator.hpp>
#include "../src/Mesh/MeshImporters/MqoImporter.hpp"
#include "../src/Font/FontManager.hpp"
#include "../src/Font/FontCore.hpp"
#include "../src/Asset/AssetArchive.hpp"
#include <LuminoEngine/Physics/PhysicsWorld2D.hpp>
#include <LuminoEngine/UI/Controls/UIButton.hpp>
#include <LuminoEngine/UI/UIFocusNavigator.hpp>
#include <LuminoEngine/UI/Controls/UIPropertyFields.hpp>
#include <LuminoEngine/Tilemap/Voxel.hpp>
#include <LuminoEngine/Scene/TransformControls.hpp>
#include <LuminoEngine/PostEffect/LightShaftPostEffect.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>
#include <LuminoEngine/Visual/EmojiComponent.hpp>
using namespace ln;

//#include <fbxsdk.h>
//
//#pragma comment(lib, "D:/Program Files/Autodesk/FBX/FBX SDK/2020.1/lib/vs2017/x64/debug/libfbxsdk-mt.lib")
//#pragma comment(lib, "D:/Program Files/Autodesk/FBX/FBX SDK/2020.1/lib/vs2017/x64/debug/libxml2-mt.lib")

#if 0
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
#endif











//#define URI_STATIC_BUILD
//#include "D:/Tech/Cpp/uriparser/include/uriparser/Uri.h"
//#pragma comment(lib, "D:/Tech/Cpp/uriparser/_build/Debug/uriparser.lib")



//#define CURL_STATICLIB
//#include <curl/curl.h>
//#pragma comment(lib, "D:/Tech/Cpp/curl/_build/lib/Debug/libcurl-d.lib")
//#pragma comment(lib, "ws2_32.lib")



#if 0
class ObservablePropertyBase;

// ObservableProperty 自体は new しなくても使えるようにしたいが、
// そうすると他で Ref で参照を持つことはできなくなるので、弱参照の仕組みを使う。
class ObservablePropertyRef
{
public:
	ObservablePropertyRef()
		: m_prop()
	{}

	ObservablePropertyRef(ObservablePropertyBase* prop)
		: m_prop(prop)
	{}

	bool isAlive() const
	{
		return m_prop.isAlive();
	}

	Ref<ObservablePropertyBase> resolve() const
	{
		auto ptr = m_prop.resolve();
		if (ptr != nullptr) {
			return ptr;
		}
		else {
			return nullptr;
		}
	}

private:
	WeakRefPtr<ObservablePropertyBase> m_prop;
};

class ObservablePropertyBase : public Object
{
public:
	ObservablePropertyBase()
		: m_bindingSource()
		, m_syncing(false)
	{}

	~ObservablePropertyBase() {
		if (auto s = m_bindingSource.resolve()) {
			s->m_changed = nullptr;
		}
	}

	virtual const Variant* getValue() const = 0;
	virtual void setValue(const Variant* value) = 0;
	
	void bind(ObservablePropertyBase* bindingSource)
	{
		m_bindingSource = bindingSource;
		bindingSource->m_changed = ln::bind(this, &ObservablePropertyBase::handleSourceChanged);
	}

protected:
	void notifyChanged(const Variant* value)
	{
		if (m_syncing) return;	// TODO: setValue で封印した方がいいかも
		m_syncing = true;
		if (m_changed) {
			m_changed(value);
		}
		syncToSource(value);
		m_syncing = false;
	}

	void syncToSource(const Variant* value)
	{
		if (auto s = m_bindingSource.resolve()) {
			if (!s->m_syncing) {
				s->setValue(value);
			}
		}
	}

	void handleSourceChanged(const Variant* value)
	{
		setValue(value);
	}

private:
	ObservablePropertyRef m_bindingSource;
	std::function<void(const Variant*)> m_changed;	// TODO: event の方がいいか
	bool m_syncing;
};


template<class TValue>
class ObservableProperty : public ObservablePropertyBase
{
public:
	ObservableProperty()
		: m_value()
	{}

	ObservableProperty(const TValue& value)
		: m_value(makeVariant(value))
	{}

	TValue get() const
	{
		return m_value->get<TValue>();
	}

	void set(const TValue& value)
	{
		m_value = makeVariant(value);	// TODO: assign でやりたい
		notifyChanged(m_value);
	}

	virtual const Variant* getValue() const override
	{
		return m_value;
	}

	virtual void setValue(const Variant* value) override
	{
		m_value = makeVariant(*value);	// TODO: convert
		notifyChanged(m_value);
	}

protected:

private:
	Ref<Variant> m_value;
};

#endif



void Sandbox_EmptyApp();
void Example_GameAudio();
void Example_glTFSampleViewer();
void Example_MainLoop();
void Example_MeshViewer();
void Example_MessageWindow();
void Example_Navigator();
void Example_Shader();
void Example_SoundControl();
void Example_Sprite();
void Example_UIControls();
void Example_Tilemap();
void Experiment_CharacterController();
void Experiment_Editor();
void Experiment_MeshCollider();
void Experiment_Reflector();
void Experiment_Scripting();
void Experiment_SelectObject();
void Experiment_SerializeLevel();
void Experiment_Shadow();
void Experiment_SSR();
void Tutorial_Sandbox();
void Sandbox_Builder();
void Sandbox_GridListBox();
void Sandbox_MeshTilemap();
void Sandbox_Physics();
void Sandbox_Physics2();
void Sandbox_Particle();
void Sandbox_PathRendering();
void Sandbox_PathShape();
void Sandbox_PostEffect();
void Sandbox_Sky();
void Sandbox_Voxel();
void Sandbox_UIShader();
void UISandboxMain();

#if 0
void DisplayMesh(FbxNode* node, const std::string& indent)
{
    FbxMesh* mesh = (FbxMesh*)node->GetNodeAttribute();

    std::string name = mesh->GetName();


    if (!name.size())
    {
        // FIXME:FbxMeshに名前がついていない場合は親のノードの名前を拝借
        //       １つのノードが複数のメッシュを含む場合に適していない
        name = mesh->GetNode()->GetName();
    }
    std::cout << indent << "Mesh Name: " << name << std::endl;


    //DisplayIndex(mesh);
    //DisplayPosition(mesh);

}
void DisplayContent(FbxNode* node, const std::string& indent)
{
    FbxNodeAttribute::EType lAttributeType;

    if (node->GetNodeAttribute() == NULL)
    {
        std::cout << indent << "NULL Node Attribute\n\n";
    }
    else
    {
        lAttributeType = (node->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
        case FbxNodeAttribute::eUnknown:
            std::cout << indent << "eUnknown" << std::endl;
            break;
        case FbxNodeAttribute::eNull:
            std::cout << indent << "eNull" << std::endl;
            break;
        case FbxNodeAttribute::eMarker:
            std::cout << indent << "eMarker" << std::endl;
            break;
        case FbxNodeAttribute::eSkeleton:
            std::cout << indent << "eSkeleton" << std::endl;
            break;
        case FbxNodeAttribute::eMesh:
            std::cout << indent << "eMesh" << std::endl;
            DisplayMesh(node, indent);
            break;
        case FbxNodeAttribute::eNurbs:
            std::cout << indent << "eNurbs" << std::endl;
            break;
        case FbxNodeAttribute::ePatch:
            std::cout << indent << "ePatch" << std::endl;
            break;
        case FbxNodeAttribute::eCamera:
            std::cout << indent << "eCamera" << std::endl;
            break;
        case FbxNodeAttribute::eCameraStereo:
            std::cout << indent << "eCameraStereo" << std::endl;
            break;
        case FbxNodeAttribute::eCameraSwitcher:
            std::cout << indent << "eCameraSwitcher" << std::endl;
            break;
        case FbxNodeAttribute::eLight:
            std::cout << indent << "eLight" << std::endl;
            break;
        case FbxNodeAttribute::eOpticalReference:
            std::cout << indent << "eOpticalReference" << std::endl;
            break;
        case FbxNodeAttribute::eOpticalMarker:
            std::cout << indent << "eOpticalMarker" << std::endl;
            break;
        case FbxNodeAttribute::eNurbsCurve:
            std::cout << indent << "eNurbsCurve" << std::endl;
            break;
        case FbxNodeAttribute::eTrimNurbsSurface:
            std::cout << indent << "eTrimNurbsSurface" << std::endl;
            break;
        case FbxNodeAttribute::eBoundary:
            std::cout << indent << "eBoundary" << std::endl;
            break;
        case FbxNodeAttribute::eNurbsSurface:
            std::cout << indent << "eNurbsSurface" << std::endl;
            break;
        case FbxNodeAttribute::eShape:
            std::cout << indent << "eShape" << std::endl;
            break;
        case FbxNodeAttribute::eLODGroup:
            std::cout << indent << "eLODGroup" << std::endl;
            break;
        case FbxNodeAttribute::eSubDiv:
            std::cout << indent << "eSubDiv" << std::endl;
            break;
        case FbxNodeAttribute::eCachedEffect:
            std::cout << indent << "eCachedEffect" << std::endl;
            break;
        case FbxNodeAttribute::eLine:
            std::cout << indent << "eLine" << std::endl;
            break;
        default:
            std::cout << indent << "Unknown FbxNodeAttribute" << std::endl;
            break;
        }
    }


    for (int i = 0; i < node->GetChildCount(); i++)
    {
        DisplayContent(node->GetChild(i), indent + "  ");
    }
}

void DisplayContent(FbxScene* scene)
{
    FbxNode* node = scene->GetRootNode();

    if (node)
    {
        for (int i = 0; i < node->GetChildCount(); i++)
        {
            DisplayContent(node->GetChild(i), "");
        }
    }
}
#endif

int main(int argc, char** argv)
{
	setlocale(LC_ALL, "");

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	Logger::addStdErrAdapter();
    Logger::setLevel(LogLevel::Debug);
	EngineSettings::setEngineFeatures(EngineFeature::Experimental);// EngineFeature::Public);// 
	EngineSettings::setGraphicsAPI(GraphicsAPI::Vulkan);//GraphicsAPI::DirectX12);//(GraphicsAPI::OpenGL);//
    EngineSettings::setPriorityGPUName(u"Microsoft Basic Render Driver");
	EngineSettings::addAssetDirectory(LN_LOCALFILE("Assets"));
	//EngineSettings::setUITheme(u"Chocotelier");
    EngineSettings::setGraphicsDebugEnabled(true);
    EngineSettings::setDebugToolEnabled(true);
	detail::EngineManager::s_settings.standaloneFpsControl = true;
	//detail::EngineDomain::engineManager()->settings().createMainLights = true;

#if 0
    FbxManager* manager = FbxManager::Create();
    FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);
    FbxScene* scene = FbxScene::Create(manager, "");

    {
        FbxImporter* importer = FbxImporter::Create(manager, "");
        importer->Initialize("D:/Documents/UnityProjects/New Unity Project/Assets/UnityChan/SD_Kohaku_chanz/Models/Misaki_sum_humanoid.fbx", -1, manager->GetIOSettings());
        importer->Import(scene);
        importer->Destroy();

        // 三角形化
        FbxGeometryConverter geometryConverter(manager);
        geometryConverter.Triangulate(scene, true);

        DisplayContent(scene);
    }


    manager->Destroy();
    return 0;
#endif


    if (1) {
		//Sandbox_EmptyApp();
		//Example_GameAudio();
        //Example_glTFSampleViewer();
        //Example_MainLoop();
		//Example_MeshViewer();
        //Example_MessageWindow();
        //Example_Navigator();
        //Example_Shader();
		//Example_SoundControl();
        //Example_Sprite();
        //Example_UIControls();
		//Example_Tilemap();
        //Experiment_CharacterController();
        //Experiment_Editor();
        //Experiment_MeshCollider();
        //Experiment_Reflector();
        //Experiment_Scripting();
        //Experiment_SelectObject();
        //Experiment_SerializeLevel();
        //Experiment_Shadow();
        //Experiment_SSR();
        //Sandbox_Builder();
        //Sandbox_GridListBox();
		//Sandbox_MeshTilemap();
        //Sandbox_Physics();
        //Sandbox_Physics2();
        //Sandbox_Particle();
        //Sandbox_PathRendering();
        //Sandbox_PathShape();
        //Sandbox_PostEffect();
        //Sandbox_Sky();
        //Sandbox_UIShader();
		//Sandbox_Voxel();
		Tutorial_Sandbox();
		//UISandboxMain();
        return 0;
    }



#if 0
	{
		ObservableProperty<int> v1(100);


		ObservableProperty<int> v2(100);
		v2.bind(&v1);
		v2.set(200);


		int a = v1.get();
		// 200

		v1.set(300);
		int b = v2.get();


		printf("");
	}
#endif

	//UriUriA uri;
	////const char * const uriString = "file:///home/user/song.mp3";
	//const char * const uriString = "lnasset://assets/user/song.mp3";
	//const char * errorPos;
	//if (uriParseSingleUriA(&uri, uriString, &errorPos) != URI_SUCCESS) {

	//		return 1;
	//}
	///* Success */
	//uriFreeUriMembersA(&uri);

	//{
	//	CURLUcode rc;
	//	CURLU *url = curl_url();
	//	rc = curl_url_set(url, CURLUPART_URL, "file://assets1/Tilesets/Tileset-1.lnasset", CURLU_NON_SUPPORT_SCHEME);
	//	if (!rc) {
	//		char *scheme, *path;
	//		rc = curl_url_get(url, CURLUPART_SCHEME, &scheme, 0);
	//		rc = curl_url_get(url, CURLUPART_PATH, &path, 0);
	//		if (!rc) {
	//			printf("the scheme is %s\n", scheme);
	//			curl_free(scheme);
	//		}
	//		curl_url_cleanup(url);
	//	}
	//}


	//return UISandboxMain();


    //Logger::addStdErrAdapter();
	//Logger::setLevel(LogLevel::Verbose);
	int div = 2;
    EngineSettings::setMainWindowSize(640 / div, 480 / div);
    //EngineSettings::setMainWorldViewSize(640 / div, 480 / div);


	Engine::initialize();
	//Engine::update();
	//Engine::finalize();
	//return 0;

    Engine::renderView()->setGuideGridEnabled(true);

    //LnLog_PrintA(LN_LOG_LEVEL_INFO, "RubyRuntime", "Usesr type registering. (class: 20, typeInfoId: 16, baseTypeInfoId: 10)");

    
    //GameAudio::playBGM(u"D:/Music/momentum/02 - momentum.wav");

	Camera* camera = Engine::mainCamera();
	camera->addComponent(makeObject<CameraOrbitControlComponent>());
    //Engine::mainCamera()->setPosition(0, 0, 25);
	camera->setBackgroundColor(Color::Gray);
	//Engine::mainCamera()->setPosition(0, 1, -5);
	//camera->setProjectionMode(ProjectionMode::Orthographic);
	//camera->setOrthographicSize(16, 12);

    auto ft = Texture2D::create(512, 256);
    //Font::registerFontFromFile(u"meiryo.ttc");
    //auto font1 = Font::create(u"Meiryo", 20);
    Font::registerFontFromFile(u"C:/Windows/Fonts/Arial.ttf");
    auto font1 = Font::create(u"Arial", 20);
    //Font::registerFontFromFile(u"times.ttf");
    //auto font1 = Font::create(u"Times New Roman", 20);
    //auto font1 = Font::create();
    //font1->setSize(26);
    //Font::registerFontFromFile(u"mplus-1c-regular.ttf");
    //auto font1 = Font::create(u"M+ 1c", 20);
    //Font::registerFontFromFile(u"mplus-1m-regular.ttf");
    //auto font1 = Font::create(u"M+ 1m", 20);


    
    //ft->clear(Color::White);
    //ft->drawText(u"ABCDEFGHIJKabcdefghijk", Rect(0, 0, 512, 256), font1, Color::Black);
    //ft->map(MapMode::Read)->save(u"test.png");

    //auto sprite1 = Sprite::create(Texture2D::load(u"Sprite1"), 3, 3);

	//auto m_rigidBody = ln::RigidBody2DComponent::create();
	//m_rigidBody->addCollisionShape(ln::BoxCollisionShape2D::create(5, 2));
	//m_rigidBody->setMass(0);
	//m_rigidBody->setFixedRotation(true);
	//m_rigidBody->setCollisionGroup(0x8000);
	//m_rigidBody->setCollisionGroupMask(0x0FFF);
	//sprite1->addComponent(m_rigidBody);
	//Engine::mainRenderView()->setPhysicsDebugDrawEnabled(true);

	//auto sprite2 = UISprite::create(Texture2D::load(u"Sprite1"));
	//Engine::mainUIView()->addElement(sprite2);

 //   auto message1 = UIMessageTextArea::create();
 //   Engine::mainUIView()->addElement(message1);



	//auto navi1 = makeObject<UIFocusNavigator>();
	//Engine::mainUIView()->addElement(navi1);

	//auto window1 = UIWindow::create();
	//window1->setPosition(10, 10);
	//window1->setWidth(50);
	//window1->setHeight(30);
	//window1->setBackgroundColor(Color::Red);
	//navi1->addElement(window1);

	//auto window2 = UIWindow::create();
	//window2->setPosition(20, 20);
	//window2->setWidth(50);
	//window2->setHeight(30);
	//window2->setBackgroundColor(Color::Blue);
	//navi1->addElement(window2);


    //Effect::emit(u"D:/LocalProj/Effekseer/EffekseerRuntime143b/RuntimeSample/release/test.efk", Matrix::makeTranslation(Vector3(1, 0, 0)));


	auto mesh1 = StaticMesh::load(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/Box/glTF/Box.gltf");
    //auto mesh1 = Mesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/2CylinderEngine/glTF/2CylinderEngine.gltf");
    //auto mesh1 = Mesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/AlphaBlendModeTest/glTF/AlphaBlendModeTest.gltf");
    //auto mesh1 = Mesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/AntiqueCamera/glTF/AntiqueCamera.gltf");
    //auto mesh1 = Mesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/BrainStem/glTF/BrainStem.gltf");
	//auto mesh1 = Mesh::create(u"D:/Tech/Graphics/three.js/examples/models/gltf/PrimaryIonDrive.glb");

 //   auto mesh1 = Mesh::create(u"D:/Programs/MagicaVoxel-0.99.4.2-alpha-win64/export/monu10.glb");
	//mesh1->setEulerAngles(Math::PI / 2, 0, 0);
 //   mesh1->setScale(10);
	mesh1->setPosition(0, 5, 0);
    //Engine::world()->add(mesh1);
    
	//auto skymesh1 = Mesh::create(u"D:/Materials/UE4_Marketplace/GoodSky/SM_GoodSky_Hemisphere.glb");
 //   skymesh1->setBlendMode(BlendMode::Add);
 //   skymesh1->setScale(100);
 //   skymesh1->setShadingModel(ShadingModel::Unlit);
 //   skymesh1->setColorScale(Color(0.5, 0.5, 0.5));
 //   Engine::world()->add(skymesh1);

    //Engine::mainRenderView()->setClearMode(SceneClearMode::Sky);

    //auto vvv = Vector3(5.804542996261093E-6, 1.3562911419845635E-5, 3.0265902468824876E-5);
    //auto vvv2 = Vector3(0.000005804542996261093f, 0.000013562911419845635, 0.000030265902468824876);


    //auto mainAmbientLight = makeObject<AmbientLight>();
    //mainAmbientLight->setColor(Color::Purple);
    //Engine::world()->add(mainAmbientLight);
    //Engine::world()->mainAmbientLight()->setColor(Color::Purple);
    //Engine::world()->mainAmbientLight()->setIntensity(1);
    //Engine::world()->mainDirectionalLight()->lookAt(Vector3(1, -0.25, -1));
    //Engine::world()->mainDirectionalLight()->setIntensity(5);

    //auto mainDirectionalLight = makeObject<DirectionalLight>();
    //Engine::world()->add(mainDirectionalLight);
    
    // MeshContainer 複数
    //auto mesh1 = Mesh::create(u"D:/Tech/Graphics/glTF-Sample-Models/2.0/Lantern/glTF/Lantern.gltf");

    //auto voxelmap1 = makeObject<VisualObject>();
    //auto voxelmapComponent1 = makeObject<VoxelmapComponent>();
    //voxelmap1->addComponent(voxelmapComponent1);

	//auto tc1 = makeObject<TransformControls>();

    //Engine::mainRenderView()->transformControls()->setTarget(mesh1);
	//Engine::mainRenderView()->transformControls()->setTarget(Engine::world()->mainDirectionalLight());

	auto field1 = makeObject<UISliderField>();
	field1->setName(u"test_field");
	field1->setWidth(200);
	field1->setHeight(30);
	//Engine::mainUIView()->addElement(field1);


    //auto ToneLayer = TonePostEffect::create();
    //ToneLayer->play(ColorTone(0.7, 0.5, 0.2, 1.0), 1);
    //Engine::mainViewport()->addPostEffect(ToneLayer);

	//auto bloomEffect = BloomPostEffect ::create();
 //   Engine::mainViewport()->addPostEffect(bloomEffect);

	//auto lightShaft = LightShaftPostEffect::create();
	//Engine::mainRenderView()->addPostEffect(lightShaft);

#if 1
	{
		Engine::mainCamera()->setBackgroundColor(Color::White);

		//auto s = u'🐈';
		//auto t = Texture2D::loadEmoji(U'🐈');
		auto t = Texture2D::loadEmoji(u"🌱");

		auto s2 = SamplerState::create(TextureFilterMode::Linear);
		t->setSamplerState(s2);

		auto s = Sprite::create(t, 1, 1);
		s->setPosition(0, 1.2, 0);
		s->setScale(2);
		//s->setAnchorPoint(Vector2(0.5, 0));
		//s->setBlendMode(BlendMode::Normal);
		//s->setShadingModel(ShadingModel::Unlit);
		//Engine::world()->add(s);

		//auto c = makeObject<EmojiComponent>();
		//auto obj = makeObject<WorldObject>();
		//obj->addComponent(c);
		//c->setShadingModel(ShadingModel::Unlit);
		//Engine::world()->add(obj);

		auto text1 = makeObject<Text>();
		text1->setText(u"Hello, Lumino!");
		text1->setColor(Color::Gray);
		//text1->setPosition(0, -0.5, 0);
		text1->setAnchorPoint(Vector2(0.5, 1));
		text1->setFontSize(18);
		Engine::world()->add(text1);

		auto text2 = makeObject<UIText>();
		
		//field1->setWidth(200);
		//field1->setHeight(30);
		//text1->setText(u"Hello, Lumino!");
		text2->setText(u"Hello");
		text2->setPosition(0, 50, 0);
		text2->setHAlignment(UIHAlignment::Center);
		text2->setVAlignment(UIVAlignment::Center);
		text2->setFontSize(20);
		text2->setTextColor(Color::DimGray);
		//Engine::mainUIView()->addElement(text2);
	}
#endif

#if 0
    auto window1 = UIWindow::create();
    window1->setPosition(Vector3(8, 8, 0));
    window1->setWidth(200);
    window1->setHeight(100);
    window1->setBackgroundColor(Color::White);
    window1->setBackgroundImage(Assets::loadTexture(u"window"));
    window1->setBackgroundImageRect(Rect(0, 0, 48, 48));
    window1->setBackgroundDrawMode(BrushImageDrawMode::BoxFrame);
    window1->setBackgroundImageBorder(Thickness(16));
    window1->setPadding(Thickness(16));

	auto tp1 = UITypographyArea::create();
	tp1->setBackgroundColor(Color::Red);
	window1->addElement(tp1);
#endif

 //   window1->setLayoutPanel(UIStackLayout_Obsolete::create());

 //   auto text1 = UIText::create();
 //   text1->setText(u"ABCDEFGabcdefg");
 //   //text1->setFontFamily(u"M+ 1c");
 //   //text1->setFontSize(20);
 //   //text1->setTextColor(Color::White);
 //   //window1->addElement(text1);
	//Engine::mainWindow()->addElement(text1);

 //   auto text2 = UIText::create();
 //   text2->setText(u"sc");
 //   //text2->setTextColor(Color::White);
 //   window1->addElement(text2);

	////auto text2 = UIText::create();
	////text2->setText(u"Test");
	////text2->setTextColor(Color::Gray);

	////auto text3 = UIText::create();
	////text3->setText(u"Test3");
	////text3->setTextColor(Color::Gray);
	////text3->setPosition(Vector3(0, 400, 0));



#if 0
    auto material = PhongMaterial::create();
    material->setMainTexture(Assets::loadTexture("D:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Effect/Particle1.png"));

    auto m1 = SpriteParticleModel::create();
    m1->setMaterial(material);
    m1->m_maxParticles = 30;
    m1->setSpawnRate(6);
    m1->setLifeTime(3.0);
    m1->setAutoFadeTime(0.3, 0.1);
    m1->m_loop = false;

    m1->setSize(1, 1);

    //m1->m_trailType = ParticlTrailType::Point;
    //m1->m_trailTime = 0.1;

    m1->m_movementType = ParticleMovementType::Radial;
    m1->m_axis.minValue.set(0, 0, 1);
    m1->m_axis.maxValue.set(0, 0, 1);
    m1->m_angle.minValue = 0;
    m1->m_angle.maxValue = Math::PI * 2;
    m1->m_forwardVelocity.minValue = 0.25 * 60;
    m1->m_forwardVelocity.maxValue = 0.025 * 60;
    //m1->m_forwardPosition.minValue = 5;
    //m1->m_forwardPosition.maxValue = 10;

    m1->m_sizeRandomSource = ParticleRandomSource::ByBaseValue;	// サイズが小さいものほど、
    m1->m_forwardVelocity.randomSource = ParticleRandomSource::ByBaseValueInverse;	// 速度が大きい

    Effect::emit(m1, Vector3::Zero);

    //auto cmp1 = makeObject<ParticleEmitterComponent>(m1);
    //cmp1->setCullMode(CullMode::None);
    //cmp1->setBlendMode(BlendMode::Add);

    //auto obj1 = makeObject<WorldObject>();
    //obj1->addComponent(cmp1);
    //obj1->setPosition(5, 0, 0);

#endif



	{
		int x = 0;
		while (Engine::update()) {
			//sprite2->setPosition(x, 0, 0);
			++x;
			//sprite1->setEulerAngles(0, Engine::totalTime(), 0);

            //skymesh1->setPosition(Engine::mainCamera()->position());


			//printf("--------------------------\n");
			//float t = Engine::totalTime() / 2;
			//float s = (sin(t) * (Math::PI / 2));
			//std::cout << s << std::endl;
			//Engine::mainCamera()->setEulerAngles(s, 0, 0);

			//Engine::mainCamera()->setEulerAngles(Math::PI / 8, 0, 0);
			//Engine::mainCamera()->setEulerAngles(Math::PI / 4, 0, 0);
            //obj1->setPosition(cos(Engine::totalTime()), 0, sin(Engine::totalTime()));

			//Engine::mainWindow()->swapChain()->currentBackbuffer
		}

	}


	Engine::terminate();
	return 0;


	//{

	//	auto p2world = makeObject<PhysicsWorld2D>();
    auto p2world = Engine::world()->physicsWorld2D();
	auto shape1 = makeObject<BoxCollisionShape2D>(Size(10, 2));
	auto body1 = makeObject<RigidBody2D>();
	body1->addCollisionShape(shape1);
	p2world->addPhysicsObject(body1);

	auto shape2 = makeObject<BoxCollisionShape2D>(Size(1, 1));
	auto body2 = makeObject<RigidBody2D>();
	body2->addCollisionShape(shape2);
	body2->setPosition(Vector2(0, 15));
	body2->setMass(1);
	p2world->addPhysicsObject(body2);

	//	for (int i = 0; i < 60; i++) {
	//		p2world->stepSimulation(1.0 / 60);

	//		printf("%4.2f %4.2f\n", body2->position().x, body2->position().y);
	//	}
	//}


    auto ctl = makeObject<CameraOrbitControlComponent>();
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
    auto m_vertexBuffer = ln::makeObject<ln::VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
    
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
	////auto bmp = makeObject<Bitmap2D>();
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

    //auto tex = makeObject<Texture2D>(u"D:/tmp/110220c_as019.png");
    //auto tex = Assets::loadTexture(u"D:/Proj/Volkoff/Engine/Lumino/src/LuminoEngine/test/Assets/Sprite1.png");
    //tex->drawText(u"Hello!", Rect(0, 0, 100, 100), font, Color::White);
    //auto tex = makeObject<Texture2D>(2, 2);
    //auto bmp1 = tex->map(MapMode::Write);
    //bmp1->setPixel32(0, 0, ColorI(255, 0, 0, 255));
    //bmp1->setPixel32(1, 0, ColorI(255, 0, 255, 255));
    //bmp1->setPixel32(0, 1, ColorI(0, 255, 0, 255));
    //bmp1->setPixel32(1, 1, ColorI(0, 0, 255, 255));

    //auto sprite = makeObject<UISprite>();
    //sprite->setTexture(tex);
    //sprite->setPosition(20, 10);
    //auto imageEffect = makeObject<ScreenBlurPostEffect>();
    //imageEffect->setAmount(0.7);
    //imageEffect->setRadialScale(1.05);
    //Engine::mainViewport()->addPostEffect(imageEffect);

    //auto sprite = Sprite::create(3, 3, tex);

    //auto tilemap = makeObject<Tilemap>();
    //tilemap->setShadingModel(ShadingModel::Unlit);



    //auto ToneLayer = TonePostEffect::create();
    //Engine::mainViewport()->addPostEffect(ToneLayer);

    //auto ToneLayer2 = TonePostEffect::create();
    //Engine::mainViewport()->addPostEffect(ToneLayer2);

    //auto BlurLayer = ScreenBlurPostEffect::create();
    //Engine::mainViewport()->addPostEffect(BlurLayer);

    //auto texture2 = Texture2D::create(100, 100);
    //texture2->clear(Color::Gray);
    ////font->setAntiAlias(false);
    //texture2->drawText(u"Left", Rect(0, 0, 0, 0), font, Color::Black);
    //auto sprite2 = ln::UISprite::create(texture2);
    ////sprite2->setPosition(200, 100);
    //sprite2->setCenterPoint(50, 50);

    //auto mesh1 = makeObject<Mesh>(u"D:/tmp/cube.obj");
    //auto mesh1 = makeObject<Mesh>(u"D:/Proj/Volkoff/Engine/Lumino/build/ExternalSource/tinyobjloader/models/cornell_box.obj");
    //auto mesh1 = makeObject<Mesh>(u"D:/Proj/Volkoff/Engine/Lumino/build/ExternalSource/tinyobjloader/models/usemtl-issue-68.obj");
    //mesh1->setPosition(0, -1, 0);
    //

    //auto mesh2 = makeObject<Mesh>(u"D:/Proj/Volkoff/Engine/Lumino/build/ExternalSource/tinyobjloader/models/cornell_box.obj");
    //mesh2->setPosition(2, 0, 0);

    //auto mesh3 = makeObject<Mesh>(u"D:/Proj/TH-10/Assets/Graphics/test/sphere4.obj", 2);
    //mesh3->setVisible(false);

    //auto clip1 = VmdAnimationClip::create(u"D:/MMD/Materials/モーション/Love&Joy/love&joyお面無しver.vmd");

    //auto smesh1 = SkinnedMesh::create(u"D:/MMD/Materials/モデル/Appearance Miku/Appearance Miku.pmx");
    ////smesh1->setBlendMode(BlendMode::Alpha);
    ////smesh1->setEulerAngles(0, Math::PI, 0);
    ////smesh1->setShadingModel(ShadingModel::Unlit);

    //smesh1->skinnedMeshComponent()->model()->animationController()->addClip(u"anim1", clip1);
    //smesh1->skinnedMeshComponent()->model()->animationController()->play(u"anim1");


    //auto sprite = UISprite::create(Assets::loadTexture(u"D:/MMD/Materials/モデル/Appearance Miku/A4.bmp"));

    //Engine::update();
    //Engine::update();
    //{
    //    auto camera = makeObject<Camera>();
    //    camera->setPosition(0, 0, -20);
    //    auto ofs = makeObject<OffscreenWorldRenderView>();
    //    auto rt1 = makeObject<RenderTargetTexture>(640, 480, TextureFormat::RGBA32, false);
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
    //auto body1 = makeObject<RigidBody>();
    //body1->addCollisionShape(shape1);
    //body1->setTransform(Matrix::makeTranslation(0, -3, 0));
    //Engine::mainPhysicsWorld()->addPhysicsObject(body1);


    //auto shape2 = CapsuleCollisionShape::create(0.5, 3);
    //auto body2 = makeObject<RigidBody>();
    //body2->addCollisionShape(shape2);
    //body2->setTransform(Matrix::makeTranslation(0, -2, 0));
    //body2->setMass(1.0f);
    //body2->setKinematic(true);
    //Engine::mainPhysicsWorld()->addPhysicsObject(body2);

    //auto body3 = makeObject<SoftBody>();
    //body3->createFromMesh(mesh3->staticMeshComponent()->model()->meshContainers().front()->meshResource(), Engine::mainPhysicsWorld());

    auto tex2 = Assets::loadTexture(u"D:/Documents/Modeling/grid_uv_2.png");
    List<Ref<WorldObject>> spheres;
    for (int y = 0; y < 5; y++)
    {
        for (int i = 0; i < 5; i++)
        {
            auto obj2 = makeObject<WorldObject>();
            auto cmp2 = makeObject<SphereMeshComponent>();
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

    auto plane1 = makeObject<WorldObject>();
    auto planecmp2 = makeObject<PlaneMeshComponent>();
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

    auto meshRes = makeObject<MeshResource>();
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

    auto meshContainer = makeObject<MeshContainer>();
    meshContainer->setMeshResource(meshRes);

    auto meshModel = makeObject<MeshModel>();
    meshModel->addMeshContainer(meshContainer);
    meshModel->addMaterial(meshMaterial);

    auto mesh1 = makeObject<Mesh>();
    mesh1->staticMeshComponent()->setModel(meshModel);
#endif

#if 0
    //auto sound = makeObject<Sound>(u"D:\\tmp\\4_Battle_win.wav");
    auto sound = makeObject<Sound>(u"D:/Music/momentum/02 - momentum.wav");
    //auto sound = makeObject<Sound>(u"D:/Proj/Volkoff/Assets/Data/Sound/BGM/monochrome.ogg");
    sound->play();
    //sound->setPitch(1.2);

    //GameAudio::playBGM(u"D:/Proj/Volkoff/Assets/Data/Sound/BGM/monochrome.ogg");
#endif
#if 0
    //auto source = makeObject<AudioSourceNode>(u"D:/Tech/Audio/WebAudioTest1/2018-11-29T13_00_15.686Z.wav");
    auto source = makeObject<AudioSourceNode>(u"D:/Music/momentum/02 - momentum.wav");
    //auto source = makeObject<TestProcessorNode>();
    //AudioNode::connect(source, AudioContext::primary()->destination());
    
    //auto recoder = makeObject<TestRecoderNode>();
    //AudioNode::connect(source, recoder);
    //AudioNode::connect(recoder, AudioContext::primary()->destination());

    auto gain = makeObject<AudioGainNode>();
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

    auto uitext = UIText::create();
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
        if (Mouse::pressed(MouseButtons::Left))
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


	Engine::terminate();

	return 0;
}

