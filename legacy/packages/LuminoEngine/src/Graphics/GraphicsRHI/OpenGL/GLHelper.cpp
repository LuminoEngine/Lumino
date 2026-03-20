#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/detail/PlatformManager.hpp>
#include <LuminoEngine/Platform/detail/OpenGLContext.hpp>
#include "OpenGLDeviceContext.hpp"

#include "GLFWSwapChain.hpp"
#include "GLHelper.hpp"

namespace ln {
namespace detail {

bool OpenGLHelper::checkOpenGLES() {
    // see: grad.c find_coreGL()

    const char* version;
    const char* prefixes[] = {
        "OpenGL ES-CM ",
        "OpenGL ES-CL ",
        "OpenGL ES ",
        NULL
    };

    version = (const char*)glGetString(GL_VERSION);
    if (!version) return false;

    for (int i = 0; prefixes[i]; i++) {
        const size_t length = strlen(prefixes[i]);
        if (strncmp(version, prefixes[i], length) == 0) {
            return true;
        }
    }

    return false;
}

} // namespace detail
} // namespace ln

