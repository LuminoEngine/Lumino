
#define LN_MSVC_DISABLE_LIBRARY_LINK
#include <LuminoEngine.hpp>
#include <LuminoCore/Testing/TestHelper.hpp>
#include "../src/Engine/EngineManager.hpp"
using namespace ln;


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
void Sandbox_PostEffect();
void Sandbox_Sky();
void Sandbox_Voxel();
void Sandbox_UIShader();
void UISandboxMain();

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



    Engine::initialize();

    while (Engine::update()) {

    }

    Engine::terminate();

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
        //Sandbox_PostEffect();
        //Sandbox_Sky();
        //Sandbox_UIShader();
		//Sandbox_Voxel();
		Tutorial_Sandbox();
		//UISandboxMain();
        return 0;
    }

	return 0;
}

