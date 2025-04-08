#pragma once
#define LN_TEST_EXTERNAL_OPENGL_CONTEXT 0

class TestEnv {
public:
    static void initialize();
    static void terminate();
    static void present();
    static ln::ByteBuffer compileShader(const ln::Path& filePath);
    static ln::Path getTestDataPath(const ln::Path& localPath);
    static ln::Path getTempPath(const ln::Path& localPath);
    static bool checkScreenShot(const ln::Path& filePath, int passRate = 95, bool save = false);

#if LN_TEST_EXTERNAL_OPENGL_CONTEXT
    static ln::Ref<ln::PlatformWindow> mainWindow;
#endif
    static LNHandle mainWindow;
    static LNHandle surfaceContext;
    static LNHandle viewPoint;
};

#define ASSERT_SCREENSHOT(...) ASSERT_TRUE(TestEnv::checkScreenShot(__VA_ARGS__))
