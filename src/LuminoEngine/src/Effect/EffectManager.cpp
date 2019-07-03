
#include "Internal.hpp"
#include <LuminoEngine/Effect/EffectContext.hpp>
#include "EffectManager.hpp"

#include <Effekseer.h>
#include <EffekseerRendererGL.h>
#include <EffekseerRenderer/EffekseerRendererDX12.Renderer.h>
#include <EffekseerRendererLLGI.Renderer.h>
//#include <EffekseerSoundAL.h>
#include <LLGI.Platform.h>
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "C:/VulkanSDK/1.1.101.0/Lib32/vulkan-1.lib")

namespace ln {
namespace detail {

static int g_window_width = 800;
static int g_window_height = 600;
static ::Effekseer::Manager*			g_manager = NULL;
static ::EffekseerRenderer::Renderer*	g_renderer = NULL;
//static ::EffekseerSound::Sound*			g_sound = NULL;
static ::Effekseer::Effect*				g_effect = NULL;
static ::Effekseer::Handle				g_handle = -1;
static ::Effekseer::Vector3D			g_position;
static LLGI::Platform* g_platform = nullptr;
static LLGI::Graphics* g_graphics = nullptr;

//==============================================================================
// EffectManager

EffectManager::EffectManager()
{
}

void EffectManager::init(const Settings& settings)
{
	//return;
    LN_LOG_DEBUG << "EffectManager Initialization started.";

    g_platform = LLGI::CreatePlatform(LLGI::DeviceType::Vulkan);
    g_graphics = g_platform->CreateGraphics();


    ::EffekseerRendererLLGI::FixedShader fixedShaders;
	::EffekseerRendererLLGI::Renderer::CreateFixedShaderForVulkan(&fixedShaders);

    // 描画用インスタンスの生成
    //g_renderer = ::EffekseerRendererGL::Renderer::Create(2000);
    g_renderer = ::EffekseerRendererLLGI::Renderer::Create(g_graphics, &fixedShaders, false, 1000);

    // エフェクト管理用インスタンスの生成
    g_manager = ::Effekseer::Manager::Create(2000);

    // 描画用インスタンスから描画機能を設定
    g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
    g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
    g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
    g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());

    // 描画用インスタンスからテクスチャの読込機能を設定
    // 独自拡張可能、現在はファイルから読み込んでいる。
    g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
    g_manager->SetModelLoader(g_renderer->CreateModelLoader());

    // 音再生用インスタンスの生成
    //g_sound = EffekseerSound::Sound::Create(32);

    // 音再生用インスタンスから再生機能を指定
    //g_manager->SetSoundPlayer(g_sound->CreateSoundPlayer());

    // 音再生用インスタンスからサウンドデータの読込機能を設定
    // 独自拡張可能、現在はファイルから読み込んでいる。
    //g_manager->SetSoundLoader(g_sound->CreateSoundLoader());

    // 視点位置を確定
    g_position = ::Effekseer::Vector3D(10.0f, 5.0f, 20.0f);

    // 投影行列を設定
    g_renderer->SetProjectionMatrix(
        ::Effekseer::Matrix44().PerspectiveFovRH_OpenGL(90.0f / 180.0f * 3.14f, (float)g_window_width / (float)g_window_height, 1.0f, 50.0f));

    // カメラ行列を設定
    g_renderer->SetCameraMatrix(
        ::Effekseer::Matrix44().LookAtRH(g_position, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

    // エフェクトの読込
    g_effect = Effekseer::Effect::Create(g_manager, (const EFK_CHAR*)L"D:/LocalProj/Effekseer/EffekseerRuntime143b/RuntimeSample/release/test.efk");

    // エフェクトの再生
    g_handle = g_manager->Play(g_effect, 0, 0, 0);



    LN_LOG_DEBUG << "EffectManager Initialization ended.";
}

void EffectManager::dispose()
{
	//return;
    // エフェクトの停止
    g_manager->StopEffect(g_handle);

    // エフェクトの破棄
    ES_SAFE_RELEASE(g_effect);

    // 先にエフェクト管理用インスタンスを破棄
    g_manager->Destroy();

    // 次に音再生用インスタンスを破棄
    //g_sound->Destroy();

    // 次に描画用インスタンスを破棄
    g_renderer->Destroy();
}

void EffectManager::testDraw()
{
	//return;

    if (1) {

        if (!g_platform->NewFrame())
            return;

        g_graphics->NewFrame();

    }


    // エフェクトの移動処理を行う
    g_manager->AddLocation(g_handle, ::Effekseer::Vector3D(0.2f, 0.0f, 0.0f));

    // エフェクトの更新処理を行う
    g_manager->Update();



    // エフェクトの描画開始処理を行う。
    g_renderer->BeginRendering();

    // エフェクトの描画を行う。
    g_manager->Draw();

    // エフェクトの描画終了処理を行う。
    g_renderer->EndRendering();


    if (1) {
        g_platform->Present();
    }

}

} // namespace detail
} // namespace ln

