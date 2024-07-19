#pragma once

class TestEnv {
public:
    static void initialize();
    static void terminate();
    static void present();
    static ln::Path getTestDataPath(ln::Path localPath);

    static LNHandle graphicsContext;
    static LNHandle viewPoint;
};
