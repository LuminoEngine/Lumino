// Phase 0 smoke-test implementation.
// Intentionally self-contained: does not depend on LuminoBase / LuminoCore so
// that it can be compiled as the sole source file for the Emscripten build
// before the rest of the engine is ported to the web.

#include <cstdio>
#include <LuminoC/lumino.h>

extern "C" LUMINO_API int32_t LNHelloTest(int32_t value) {
    std::printf("Lumino HelloTest: %d\n", value);
    return value;
}
