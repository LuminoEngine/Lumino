#pragma once

class TestEnv {
public:
    static void initialize();
    static void terminate();
    static void present();
    static ln::Path getTestDataPath(ln::Path localPath);
    static bool checkScreenShot(const ln::Path& filePath, int passRate = 95, bool save = false);

    static LNHandle graphicsContext;
    static LNHandle viewPoint;
};

#define ASSERT_SCREENSHOT(...) ASSERT_TRUE(TestEnv::checkScreenShot(__VA_ARGS__))
