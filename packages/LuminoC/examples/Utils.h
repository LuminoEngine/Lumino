#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <LuminoC/lumino.h>

#define WINDOW_W 1280
#define WINDOW_H 720

void InitializeInstance() {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    LNInstanceInitializeSettings settings = {};
    settings.preferredBackend = LN_GRAPHICS_BACKEND_VULKAN;
    settings.enableValidation = LN_TRUE;
    LNInstance_Initialize(&settings);
}

void printGraphicsProfilering(LNHandle graphicsContext) {
    LNGraphicsProfiler profilering = {};
    LNDebug_GetGraphicsProfiler(graphicsContext, &profilering);
    LNDebug_Print(
        graphicsContext,
        (std::string("FPS: ") + std::to_string(profilering.fps)).c_str());
    LNDebug_Print(
        graphicsContext,
        (std::string("FrameTime(ms): ") +
         std::to_string(profilering.lastFrameTimeMs))
            .c_str());
    LNDebug_Print(
        graphicsContext,
        (std::string("DrawCall: ") + std::to_string(profilering.drawCallCount))
            .c_str());
}

std::vector<unsigned char> loadFile(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open: %s\n", path);
        return {};
    }
    fseek(fp, 0, SEEK_END);
    long sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    std::vector<unsigned char> buf(sz);
    fread(buf.data(), 1, sz, fp);
    fclose(fp);
    return buf;
}
