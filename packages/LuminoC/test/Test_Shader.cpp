#include "pch.hpp"
#include <LuminoC/lumino.h>
#include <cstdio>
#include <vector>

#define TEST_W 320
#define TEST_H 240

// lumino.slang (import lumino;) を含むディレクトリ。
#define LUMINO_SHADER_DIR LN_REPO_ROOT_DIR "/packages/LuminoShader/shaders"

class Test_Shader : public ::testing::Test {
protected:
    LNHandle window = LN_NULL_HANDLE;
    LNHandle graphicsContext = LN_NULL_HANDLE;

    void SetUp() override {
        ASSERT_EQ(LN_OK, LNWindow_Create("ShaderTest", TEST_W, TEST_H, &window));
        ASSERT_EQ(LN_OK, LNWindow_GetGraphicsContext(window, &graphicsContext));
    }

    void TearDown() override {
        if (window != LN_NULL_HANDLE) LNObject_Release(window);
    }

    /** ファイル全体をバイト列として読み込む。失敗時は空の vector を返す。 */
    static std::vector<unsigned char> loadFile(const char* path) {
        std::vector<unsigned char> data;
        FILE* fp = fopen(path, "rb");
        if (!fp) return data;
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (size > 0) {
            data.resize(static_cast<size_t>(size));
            if (fread(data.data(), 1, data.size(), fp) != data.size()) data.clear();
        }
        fclose(fp);
        return data;
    }

    /** 現在生存しているシェーダパス数 (= シェーダモジュール数 / 2 = パイプラインレイアウト数)。 */
    int32_t shaderPassCount() {
        LNGraphicsProfiler profiler = {};
        EXPECT_EQ(LN_OK, LNDebug_GetGraphicsProfiler(graphicsContext, &profiler));
        return profiler.shaderPassCount;
    }
};

// マテリアル定数バッファ ($Material CB) を持たないシェーダ
// (フルスクリーン blit: テクスチャ + サンプラのみ) を読み込めることを検証する。
//
// 以前は ShaderPass::buildFromUnifiedShader が $Material 内に定数バッファが
// 無いと "No constant buffer found in '$Material'" エラーで失敗していた。
TEST_F(Test_Shader, CreateFromShaderSourceWithoutConstantBuffer) {
    LNHandle material = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromShaderSourceFile(
        graphicsContext,
        TEST_DATA_DIR "/Data/Composite.slang",
        LUMINO_SHADER_DIR,
        &material));
    ASSERT_NE(LN_NULL_HANDLE, material);

    LNObject_Release(material);
}

//------------------------------------------------------------------------------
// Shader を共有して Material を複数作ると、GPU シェーダモジュールと
// パイプラインレイアウトが共有されることを検証する。
//
// 計測は LNGraphicsProfiler::shaderPassCount (生存 ShaderPass 数) で行う。
// 1 ShaderPass = シェーダモジュール 2 個 + パイプラインレイアウト 1 個なので、
// この値が増えなければ GPU 側のシェーダ資源が増えていないと言える。
TEST_F(Test_Shader, MaterialsFromOneShaderShareShaderPasses) {
    auto shaderData = loadFile(LN_TEST_UNLIT_LCSH);
    ASSERT_FALSE(shaderData.empty());

    const int32_t baseCount = shaderPassCount();

    // Shader の作成でパスが作られる (Unlit.lcsh はパス 1 つ)。
    LNHandle shader = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNShader_CreateFromCompiledShader(
        graphicsContext, shaderData.data(), (uint32_t)shaderData.size(), &shader));
    ASSERT_NE(LN_NULL_HANDLE, shader);

    const int32_t afterShader = shaderPassCount();
    const int32_t passesPerShader = afterShader - baseCount;
    EXPECT_GT(passesPerShader, 0) << "Shader の作成でシェーダパスが作られていません";

    // 同じ Shader から Material を 4 つ作る。共有されていればパス数は増えない。
    LNHandle materials[4] = {};
    for (auto& mat : materials) {
        ASSERT_EQ(LN_OK, LNMaterial_CreateFromShader(shader, &mat));
        ASSERT_NE(LN_NULL_HANDLE, mat);
    }
    EXPECT_EQ(afterShader, shaderPassCount())
        << "Material ごとにシェーダモジュール / パイプラインレイアウトが再生成されています";

    for (auto mat : materials) LNObject_Release(mat);
    LNObject_Release(shader);

    // 比較: LNMaterial_CreateFromCompiledShader は呼び出しごとに新規生成する。
    LNHandle standalone[4] = {};
    for (auto& mat : standalone) {
        ASSERT_EQ(LN_OK, LNMaterial_CreateFromCompiledShader(
            graphicsContext, shaderData.data(), (uint32_t)shaderData.size(), &mat));
    }
    EXPECT_EQ(baseCount + passesPerShader * 4, shaderPassCount())
        << "旧経路 (マテリアルごとの生成) の想定が変わっています";

    for (auto mat : standalone) LNObject_Release(mat);

    // 全て解放すればカウンタは元に戻る (リーク検出)。
    EXPECT_EQ(baseCount, shaderPassCount());
}

//------------------------------------------------------------------------------
// 共有した Shader から作った複数の Material が、それぞれ独立したパラメータで
// 正しく描画できることを検証する。
//
// ShaderPass を共有しても、マテリアル単位の定数バッファと BindGroup は
// Material ごとに作られる ((Material, ShaderPass) をキーにキャッシュされる) ため、
// 同一フレーム内で色の異なる 2 つの Material を描き分けられる。
TEST_F(Test_Shader, DrawMaterialsFromSharedShader) {
    auto shaderData = loadFile(LN_TEST_UNLIT_LCSH);
    ASSERT_FALSE(shaderData.empty());

    LNHandle shader = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNShader_CreateFromCompiledShader(
        graphicsContext, shaderData.data(), (uint32_t)shaderData.size(), &shader));

    // 同一シェーダから、色だけが違う 2 つの Material を作る。
    LNHandle redMat = LN_NULL_HANDLE;
    LNHandle greenMat = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromShader(shader, &redMat));
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromShader(shader, &greenMat));
    ASSERT_EQ(LN_OK, LNMaterial_SetColor(redMat, 1.0f, 0.0f, 0.0f, 1.0f));
    ASSERT_EQ(LN_OK, LNMaterial_SetColor(greenMat, 0.0f, 1.0f, 0.0f, 1.0f));

    // 左上原点の 2D カメラ。スプライトを画面の左半分と右半分に描く。
    LNHandle camera = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNCamera_Create(&camera));
    ASSERT_EQ(LN_OK, LNCamera_SetOrthographic2D(camera,
        (float)TEST_W, (float)TEST_H, -1000.0f, 1000.0f, 0.0f, 0.0f));

    LNHandle renderer, colorBuffer, depthBuffer;
    ASSERT_EQ(LN_OK, LNGraphicsContext_BeginFrame(graphicsContext, TEST_W, TEST_H, &renderer, &colorBuffer, &depthBuffer));
    LNRenderPassDesc rpDesc;
    LNRenderPassDesc_Init(&rpDesc);
    rpDesc.colorAttachments[0].clearColor[0] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[1] = 0.0f;
    rpDesc.colorAttachments[0].clearColor[2] = 1.0f; // 青クリア
    rpDesc.colorAttachments[0].clearColor[3] = 1.0f;
    ASSERT_EQ(LN_OK, LNRenderer_BeginRenderPass(renderer, graphicsContext, &rpDesc, camera));

    // 頂点カラーは白のまま。色の違いはマテリアルのパラメータだけで与える。
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, redMat, 0, nullptr,
        0.0f, 0.0f,                       // 左上
        TEST_W * 0.5f, (float)TEST_H,
        0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f));
    ASSERT_EQ(LN_OK, LNRenderer_DrawSprite(
        renderer, greenMat, 0, nullptr,
        TEST_W * 0.5f, 0.0f,              // 右半分
        TEST_W * 0.5f, (float)TEST_H,
        0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f));

    ASSERT_EQ(LN_OK, LNRenderer_EndRenderPass(renderer));
    ASSERT_EQ(LN_OK, LNGraphicsContext_RequestCaptureBackbuffer(graphicsContext));
    ASSERT_EQ(LN_OK, LNGraphicsContext_EndFrame(graphicsContext));

    const uint8_t* data = nullptr;
    int32_t w = 0, h = 0;
    ASSERT_EQ(LN_OK, LNGraphicsContext_CaptureBackbuffer(graphicsContext, &data, &w, &h));
    ASSERT_NE(nullptr, data);
    ASSERT_EQ(TEST_W, w);
    ASSERT_EQ(TEST_H, h);

    // 左 1/4 の位置は赤、右 3/4 の位置は緑になる。
    const uint8_t* left  = data + (static_cast<size_t>(h / 2) * w + w / 4) * 4;
    const uint8_t* right = data + (static_cast<size_t>(h / 2) * w + w * 3 / 4) * 4;

    EXPECT_GT(left[0], 192) << "左半分が赤になっていません";
    EXPECT_LT(left[1], 64);
    EXPECT_GT(right[1], 192) << "右半分が緑になっていません (マテリアル間でパラメータが混ざった可能性)";
    EXPECT_LT(right[0], 64);

    LNObject_Release(camera);
    LNObject_Release(greenMat);
    LNObject_Release(redMat);
    LNObject_Release(shader);
}

//------------------------------------------------------------------------------
// .slang ソースから作った Shader からも Material を作成できることを検証する。
TEST_F(Test_Shader, CreateShaderFromShaderSourceFile) {
    LNHandle shader = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNShader_CreateFromShaderSourceFile(
        graphicsContext,
        TEST_DATA_DIR "/Data/Composite.slang",
        LUMINO_SHADER_DIR,
        &shader));
    ASSERT_NE(LN_NULL_HANDLE, shader);

    const int32_t afterShader = shaderPassCount();

    LNHandle mat1 = LN_NULL_HANDLE;
    LNHandle mat2 = LN_NULL_HANDLE;
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromShader(shader, &mat1));
    ASSERT_EQ(LN_OK, LNMaterial_CreateFromShader(shader, &mat2));
    EXPECT_EQ(afterShader, shaderPassCount());

    LNObject_Release(mat2);
    LNObject_Release(mat1);
    LNObject_Release(shader);
}
