#pragma once

class TestEnv {
public:
    static void initialize();
    static void terminate();
    static void present();
    static ln::ByteBuffer compileShader(const ln::Path& filePath);
    static ln::Path getTestDataPath(const ln::Path& localPath);
    static ln::Path getTempPath(const ln::Path& localPath);
    static bool checkScreenShot(const ln::Path& filePath, int passRate = 95, bool save = false);

    static ln::Ref<ln::PlatformWindow> mainWindow;
    static LNHandle graphicsContext;
    static LNHandle viewPoint;
};

#define ASSERT_SCREENSHOT(...) ASSERT_TRUE(TestEnv::checkScreenShot(__VA_ARGS__))
