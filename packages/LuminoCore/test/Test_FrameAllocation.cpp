/**
 * フレームループ内のヒープ確保回数を数える計測テスト。
 *
 * docs/plans/per-frame-allocation-2026-08.md 第 6 節「数値の確認」に対応する。
 *
 * LuminoCore / LuminoBase は STATIC ライブラリとしてこの実行ファイルに直接リンク
 * されているため、グローバル operator new を置き換えると Lumino 側の確保をここで
 * 数えられる。Vulkan ローダやドライバ DLL の内部確保は別モジュールの operator new /
 * malloc を通るので数に入らない。計りたいのは Lumino 自身の確保なので、これは狙い通り。
 */
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/ForwardRenderer.hpp>
#include <LuminoCore/Graphics/Camera.hpp>
#include <LuminoCore/Graphics/Texture2D.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>
#include <LuminoCore/Graphics/Material.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <new>

using namespace ln;
using namespace ln::rhi;
using namespace ln::platform;

//------------------------------------------------------------------------------
// アロケーションカウンタ
//------------------------------------------------------------------------------
namespace {

std::atomic<uint64_t> g_allocCount{0};
std::atomic<uint64_t> g_allocBytes{0};

uint64_t allocCount() { return g_allocCount.load(std::memory_order_relaxed); }
uint64_t allocBytes() { return g_allocBytes.load(std::memory_order_relaxed); }

} // namespace

void* operator new(size_t size) {
    g_allocCount.fetch_add(1, std::memory_order_relaxed);
    g_allocBytes.fetch_add(size, std::memory_order_relaxed);
    void* p = std::malloc(size != 0 ? size : 1);
    if (!p) throw std::bad_alloc();
    return p;
}

void* operator new[](size_t size) { return operator new(size); }

void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, size_t) noexcept { std::free(p); }
void operator delete[](void* p) noexcept { std::free(p); }
void operator delete[](void* p, size_t) noexcept { std::free(p); }

// アライメント指定版 (C++17)。置き換えないと過小カウントになるので合わせて数える。
void* operator new(size_t size, std::align_val_t align) {
    g_allocCount.fetch_add(1, std::memory_order_relaxed);
    g_allocBytes.fetch_add(size, std::memory_order_relaxed);
#ifdef _MSC_VER
    void* p = _aligned_malloc(size != 0 ? size : 1, static_cast<size_t>(align));
#else
    void* p = std::aligned_alloc(static_cast<size_t>(align), size != 0 ? size : 1);
#endif
    if (!p) throw std::bad_alloc();
    return p;
}

void* operator new[](size_t size, std::align_val_t align) { return operator new(size, align); }

void operator delete(void* p, std::align_val_t) noexcept {
#ifdef _MSC_VER
    _aligned_free(p);
#else
    std::free(p);
#endif
}
void operator delete(void* p, size_t, std::align_val_t a) noexcept { operator delete(p, a); }
void operator delete[](void* p, std::align_val_t a) noexcept { operator delete(p, a); }
void operator delete[](void* p, size_t, std::align_val_t a) noexcept { operator delete(p, a); }

//------------------------------------------------------------------------------
// 計測
//------------------------------------------------------------------------------
namespace {

constexpr int kFrames = 10;
// フレームスロットは 2 枚。パイプライン/BindGroup/スプライトプールのキャッシュが
// 全スロット分埋まるまでを助走とし、それ以降を定常状態とみなす。
constexpr int kWarmupFrames = 4;

struct FrameStat {
    uint64_t count = 0;
    uint64_t bytes = 0;
};

} // namespace

// フレームループが定常状態でヒープ確保を行わないことを確認する。
// 失敗した場合は各フレームの確保回数がそのまま計測結果になる。
static void measureFrameAllocations(Backend backend, const char* backendName) {
    CoreInstance::Settings coreSettings;
    coreSettings.preferredBackend = backend;
    if (!CoreInstance::initialize(coreSettings)) {
        GTEST_SKIP() << backendName << " バックエンドが利用できない環境です。";
    }

    FrameStat mesh[kFrames] = {};
    FrameStat sprite[kFrames] = {};
    {
        WindowDesc winDesc;
        winDesc.title = "Test_FrameAllocation";
        winDesc.width = 320;
        winDesc.height = 240;
        GraphicsContextDesc gfxDesc;

        auto windowResult = PlatformWindow::create(
            CoreInstance::instance()->graphicsModule(), winDesc, gfxDesc);
        ASSERT_TRUE(windowResult);
        auto window = *windowResult;
        auto* ctx = window->graphicsContext();

        auto rendererResult = ForwardRenderer::create(ctx);
        ASSERT_TRUE(rendererResult);
        auto renderer = *rendererResult;

        auto matResult = MaterialFactory::createUnlit(ctx);
        ASSERT_TRUE(matResult);
        auto material = *matResult;

        Vertex v0{};
        v0.position = {0.0f, 0.5f, 0.0f};
        v0.color = {1.0f, 1.0f, 1.0f, 1.0f};
        Vertex v1{};
        v1.position = {-0.5f, -0.5f, 0.0f};
        v1.color = {1.0f, 1.0f, 1.0f, 1.0f};
        Vertex v2{};
        v2.position = {0.5f, -0.5f, 0.0f};
        v2.color = {1.0f, 1.0f, 1.0f, 1.0f};
        std::vector<Vertex> vertices = {v0, v1, v2};
        std::vector<uint32_t> indices = {0, 1, 2};
        SubMesh sub;
        sub.indexOffset = 0;
        sub.indexCount = 3;
        sub.materialIndex = 0;
        auto meshResult = Mesh::create(ctx->device(), vertices, indices, {sub});
        ASSERT_TRUE(meshResult);
        auto triangle = *meshResult;
        triangle->materials() = {material};

        Camera camera;
        camera.setPerspective(60.0f * 3.14159f / 180.0f,
                              static_cast<float>(ctx->width()) / static_cast<float>(ctx->height()),
                              0.1f, 100.0f);
        camera.setLookAt({0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f});

        Camera ortho;
        ortho.setOrthographic(static_cast<float>(ctx->width()), static_cast<float>(ctx->height()),
                              -1000.0f, 1000.0f);
        ortho.setLookAt({0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f});

        RenderObject obj;
        obj.mesh = triangle;

        // ---- シナリオ 1: メッシュ 1 個 + 毎フレームのマテリアルパラメータ変更 ----
        // B-1 (パラメータ変更で BindGroup を作り直さない) と A-1 / D-1 が通る経路。
        for (int i = 0; i < kFrames; ++i) {
            window->processEvents();
            const float t = static_cast<float>(i) / kFrames;
            material->setColor(Color{t, 1.0f - t, 0.5f, 1.0f});

            const uint64_t c0 = allocCount();
            const uint64_t b0 = allocBytes();

            const FramebufferInfo* fb = *ctx->beginFrame(ctx->width(), ctx->height());
            auto r = renderer->renderFrame(
                fb->colorTexture->rhiTextureView(),
                fb->depthTexture->rhiTextureView(),
                camera, &obj, 1, Color{0.1f, 0.1f, 0.15f, 1.0f});
            ctx->endFrame();

            mesh[i].count = allocCount() - c0;
            mesh[i].bytes = allocBytes() - b0;
            ASSERT_TRUE(r);
        }

        // ---- シナリオ 2: スプライト 64 枚のバッチ描画 ----
        // DrawCommandBuffer / BatchProcessor / SpriteMeshPool が通る経路。
        for (int i = 0; i < kFrames; ++i) {
            window->processEvents();

            const uint64_t c0 = allocCount();
            const uint64_t b0 = allocBytes();

            const FramebufferInfo* fb = *ctx->beginFrame(ctx->width(), ctx->height());
            renderer->beginFrame();
            renderer->beginRenderPass(
                fb->colorTexture->rhiTextureView(),
                fb->depthTexture->rhiTextureView(),
                ortho, Color{0.0f, 0.0f, 0.0f, 1.0f});
            for (int s = 0; s < 64; ++s) {
                renderer->renderer()->drawSprite(
                    material.get(), 0,
                    Matrix4x4::translate(Vector3{static_cast<float>(s) * 2.0f, 0.0f, 0.0f}),
                    Vector2{0.0f, 0.0f}, Vector2{8.0f, 8.0f}, Vector2{0.5f, 0.5f},
                    Vector2{0.0f, 0.0f}, Vector2{1.0f, 1.0f},
                    Color{1.0f, 1.0f, 1.0f, 1.0f});
            }
            renderer->endRenderPass();
            renderer->endFrame();
            ctx->endFrame();

            sprite[i].count = allocCount() - c0;
            sprite[i].bytes = allocBytes() - b0;
        }

        ctx->waitIdle();
    }
    CoreInstance::terminate();

    // 計測結果の出力 (測定区間の外で行う)。
    std::cout << "--- per-frame heap allocations [" << backendName << "] (count / bytes) ---\n";
    for (int i = 0; i < kFrames; ++i) {
        std::cout << "  frame " << i
                  << " : mesh " << mesh[i].count << " / " << mesh[i].bytes
                  << " , sprite " << sprite[i].count << " / " << sprite[i].bytes << "\n";
    }
    std::cout << std::flush;

    for (int i = kWarmupFrames; i < kFrames; ++i) {
        EXPECT_EQ(mesh[i].count, 0u)
            << backendName << ": mesh シナリオのフレーム " << i << " でヒープ確保が発生しています。";
        EXPECT_EQ(sprite[i].count, 0u)
            << backendName << ": sprite シナリオのフレーム " << i << " でヒープ確保が発生しています。";
    }
}

TEST(Test_FrameAllocation, PerFrameHeapAllocationVulkan) {
    measureFrameAllocations(Backend::Vulkan, "Vulkan");
}

TEST(Test_FrameAllocation, PerFrameHeapAllocationWebGPU) {
    measureFrameAllocations(Backend::WebGPU, "WebGPU");
}
