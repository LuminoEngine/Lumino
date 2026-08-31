#include <LuminoC/lumino.h>
#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>

/**
 * 使用するバックエンド。コマンドライン引数 --backend=<vulkan|webgpu|webgl2> で選ぶ。
 * 同じビジュアルテストをバックエンドごとに走らせるための入口で、ctest 側では
 * "webgl2." 接頭辞つきのテストとして登録している。
 */
static LNGraphicsBackend s_backend = LN_GRAPHICS_BACKEND_DEFAULT;

class LuminoApiEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        LNInstanceInitializeSettings settings = {};
        settings.enableValidation = LN_TRUE;
        settings.preferredBackend = s_backend;
        ASSERT_EQ(LN_OK, LNInstance_Initialize(&settings));
    }

    void TearDown() override {
        LNInstance_Terminate();
    }
};

int main(int argc, char** argv) {
#if 0 // Debug Utility.
        if (argc == 1) {
            char* testArgs[] = {
                argv[0],
                (char*)"--gtest_break_on_failure",
                (char*)"--gtest_filter=Test_Graphics.MaterialDepthWriteEnabled",
            };
            argc = sizeof(testArgs) / sizeof(char*);
            argv = testArgs;
        }
#endif
    ::testing::InitGoogleTest(&argc, argv);

    // gtest が解釈しなかった引数から --backend= を読む。
    const char* kBackendOpt = "--backend=";
    const size_t kBackendOptLen = std::strlen(kBackendOpt);
    for (int i = 1; i < argc; i++) {
        if (std::strncmp(argv[i], kBackendOpt, kBackendOptLen) != 0) continue;
        const char* name = argv[i] + kBackendOptLen;
        if (std::strcmp(name, "vulkan") == 0) {
            s_backend = LN_GRAPHICS_BACKEND_VULKAN;
        } else if (std::strcmp(name, "webgpu") == 0) {
            s_backend = LN_GRAPHICS_BACKEND_WEBGPU;
        } else if (std::strcmp(name, "webgl2") == 0) {
            s_backend = LN_GRAPHICS_BACKEND_WEBGL2;
        } else {
            std::fprintf(stderr, "Unknown backend: %s\n", name);
            return 1;
        }
    }

    ::testing::AddGlobalTestEnvironment(new LuminoApiEnvironment());
    return RUN_ALL_TESTS();
}
