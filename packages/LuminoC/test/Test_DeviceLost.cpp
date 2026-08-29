#include "pch.hpp"
#include <LuminoC/lumino.h>

//------------------------------------------------------------------------------
// デバイスロスト対応 (docs/plans/device-lost-design.md) の契約テスト。
//
// LNDebug_SimulateDeviceLost はプロセス内のデバイスをロスト状態にする。
// 以後の BeginFrame が自動復旧ステートマシンをポンプし、復旧が完了すると
// BeginFrame は再び LN_OK を返す (クライアントはフレームループを回すだけでよい)。
//
// gtest_discover_tests により各テストは個別プロセスで実行されるため、
// 他のテストへは影響しない。ただし順序依存を避けるため、ロストから復旧までの
// 検証は 1 つの TEST_F にまとめている。
//------------------------------------------------------------------------------

#define TEST_W 320
#define TEST_H 240

class Test_DeviceLost : public ::testing::Test {
protected:
    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;

    void SetUp() override {
        ASSERT_EQ(LN_OK, LNWindow_Create("DeviceLostTest", TEST_W, TEST_H, &window));
        ASSERT_EQ(LN_OK, LNWindow_GetGraphicsContext(window, &graphicsContext));
    }

    void TearDown() override {
        if (window != LN_NULL_HANDLE) LNObject_Release(window);
    }

    /** バックバッファを青 (0,0,1,1) でクリアする 1 フレームを実行する。 */
    void renderClearFrame(bool capture) {
        LNHandle renderer, colorBuffer, depthBuffer;
        ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(
            graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, LN_NULL_HANDLE));
        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
        if (capture) {
            ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
        }
        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));
    }
};

// シミュレート後、GPU 依存 API が LN_ERROR_DEVICE_LOST を返し、
// フレームループを回すだけで自動復旧して描画を再開できることを確認する。
TEST_F(Test_DeviceLost, SimulateThenAutoRecover) {
    // 正常な 1 フレームを描画する (デバイスが正しく動作していることの前提確認)
    renderClearFrame(false);

    // ロスト前に GPU リソースを作成しておく (復旧後の stale 検証用)
    LNHandle staleMaterial = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(
        graphicsContext, LN_BUILTIN_SHADER_UNLIT, &staleMaterial));
    LNHandle staleRT = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNTexture2D_CreateRenderTargetEx(
        graphicsContext, 64, 64, LN_TEXTURE_FORMAT_RGBA8_UNORM, &staleRT));
    LNHandle staleMesh = LN_NULL_HANDLE;
    {
        LNVertex vertices[3] = {};
        uint32_t indices[3] = { 0, 1, 2 };
        LNSubMesh sub = { 0, 3, 0 };
        ASSERT_EQ(LN_OK, LNMesh_Create(
            graphicsContext, vertices, 3, indices, 3, &sub, 1, &staleMesh));
    }

    // デバイスロストをシミュレートする
    ASSERT_EQ(LN_OK, LNDebug_SimulateDeviceLost(LN_FALSE));

    // ロスト中 (最初の BeginFrame を呼ぶ前) は GPU 依存 API が
    // LN_ERROR_DEVICE_LOST を返し、RHI には触れない
    EXPECT_EQ(LN_ERROR_DEVICE_LOST, LNGraphicsContext_EndFrame(graphicsContext));
    {
        LNHandle handle = LN_NULL_HANDLE;
        EXPECT_EQ(LN_ERROR_DEVICE_LOST, LNTexture2D_CreateRenderTargetEx(
            graphicsContext, 64, 64, LN_TEXTURE_FORMAT_RGBA8_UNORM, &handle));
        EXPECT_EQ(LN_ERROR_DEVICE_LOST, LNMaterial_CreateFromBuiltinShader(
            graphicsContext, LN_BUILTIN_SHADER_UNLIT, &handle));
    }

    // GPU 非依存の API は引き続き機能する
    {
        LNHandle camera = LN_NULL_HANDLE;
        EXPECT_EQ(LN_OK, LNCamera_Create(&camera));
        EXPECT_EQ(LN_OK, LNObject_Release(camera));
    }

    // フレームループを回し続けると自動復旧して BeginFrame が LN_OK に戻る。
    // デスクトップ (Vulkan) ではデバイス再作成が同期的なため最初の
    // BeginFrame で復旧が完了するが、契約としては「有限回で LN_OK に戻る」
    // のみを仮定する (Web では非同期のため数フレームかかる)。
    LNHandle renderer = LN_NULL_HANDLE;
    LNHandle colorBuffer = LN_NULL_HANDLE;
    LNHandle depthBuffer = LN_NULL_HANDLE;
    bool recovered = false;
    for (int i = 0; i < 300; i++) {
        LNResult r = LNGraphicsContext_BeginFrame(
            graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer);
        if (r == LN_OK) {
            recovered = true;
            break;
        }
        ASSERT_EQ(LN_ERROR_DEVICE_LOST, r) << "frame=" << i;
    }
    ASSERT_TRUE(recovered) << "device did not recover within 300 frames";
    EXPECT_NE(LN_NULL_HANDLE, renderer);
    EXPECT_NE(LN_NULL_HANDLE, colorBuffer);
    EXPECT_NE(LN_NULL_HANDLE, depthBuffer);

    // 復旧後のフレーム内で stale リソースの契約を検証する
    {
        // stale レンダーターゲットをアタッチメントに使うのはハードエラー
        LNRenderPassDesc rtDesc;
        LNRenderPassDesc_Init(&rtDesc);
        rtDesc.colorAttachmentCount = 1;
        rtDesc.colorAttachments[0].renderTarget = staleRT;
        EXPECT_EQ(LN_ERROR_INVALID_HANDLE,
            LNRenderer_BeginRenderPass(renderer, graphicsContext, &rtDesc, LN_NULL_HANDLE));

        // 通常のパスを開始し、stale リソースの描画がスキップされること
        // (クラッシュやエラーにならないこと) を確認する
        LNRenderPassDesc rpDesc;
        LNRenderPassDesc_Init(&rpDesc);
        rpDesc.colorAttachments[0].clearColor[2] = 1.0f;
        rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
        ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, LN_NULL_HANDLE));

        LNTransform identity = { 0,0,0,  0,0,0,1,  1,1,1 };
        EXPECT_EQ(LN_OK, LNRenderer_DrawMesh(renderer, staleMesh, &identity, 0));

        // stale マテリアルへの設定系 API は LN_OK を返して無視される
        EXPECT_EQ(LN_OK, LNMaterial_SetColor(staleMaterial, 1, 0, 0, 1));

        ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
        ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
        ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));
    }

    // キャプチャ結果がクリアカラー (青) であること = 復旧後の描画が機能していること
    {
        const uint8_t* data = nullptr;
        int32_t w = 0, h = 0;
        ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
        ASSERT_NE(nullptr, data);
        ASSERT_EQ(TEST_W, w);
        ASSERT_EQ(TEST_H, h);
        // RGBA: (0, 0, 255, 255) 付近であること (中央ピクセルを確認)
        const size_t center = (static_cast<size_t>(h / 2) * w + (w / 2)) * 4;
        EXPECT_LE(data[center + 0], 8);
        EXPECT_LE(data[center + 1], 8);
        EXPECT_GE(data[center + 2], 247);
    }

    // 復旧後に新しいリソースを作成して描画できること
    {
        LNHandle newMaterial = LN_NULL_HANDLE;
        ASSERT_EQ(LN_OK, LNMaterial_CreateFromBuiltinShader(
            graphicsContext, LN_BUILTIN_SHADER_UNLIT, &newMaterial));
        EXPECT_EQ(LN_OK, LNMaterial_SetColor(newMaterial, 1, 1, 1, 1));
        EXPECT_EQ(LN_OK, LNObject_Release(newMaterial));
    }

    // stale リソースの Release は常に成功する (ハンドル契約)
    EXPECT_EQ(LN_OK, LNObject_Release(staleMesh));
    EXPECT_EQ(LN_OK, LNObject_Release(staleRT));
    EXPECT_EQ(LN_OK, LNObject_Release(staleMaterial));

    // 復旧後、通常のフレームが継続して描画できる
    renderClearFrame(false);
}
