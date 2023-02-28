#pragma once

class TestEnv {
public:
    static void setup();
    static void teardown();
    static void initializeRendering();

    static bool isCI;
};

