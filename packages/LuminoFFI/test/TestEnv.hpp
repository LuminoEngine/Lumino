#pragma once

class TestEnv {
public:
    static void initialize();
    static void terminate();
    static void present();

    static LNHandle graphicsContext;
    static LNHandle viewPoint;
};
