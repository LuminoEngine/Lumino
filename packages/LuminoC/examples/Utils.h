
#pragma once
#include <stdio.h>
#include <string>
#include <LuminoC/lumino.h>

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
