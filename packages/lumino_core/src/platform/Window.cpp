#include <lumino_core/platform/Window.hpp>
#include <lumino_core/graphics/GraphicsContext.hpp>

#ifdef LN_NX
    #include <nn/nn_Common.h>
    #include <nn/os.h>
    #include <nn/vi.h>
#else
    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
#endif

namespace ln::platform {

// ─── GLFW global init ───
#ifndef LN_NX
namespace {
struct GlfwGuard {
    GlfwGuard() { glfwInit(); }
    ~GlfwGuard() { glfwTerminate(); }
};
static GlfwGuard& ensureGlfw() {
    static GlfwGuard guard;
    return guard;
}
} // namespace
#endif

// ─── Impl ───
struct PlatformWindow::Impl {
    GLFWwindow* window = nullptr;
    Ref<ln::GraphicsContext> graphicsContext;
};

PlatformWindow::~PlatformWindow() {
    if (impl_) {
        // Graphics context must be torn down before the GLFW window
        impl_->graphicsContext.reset();
        if (impl_->window) glfwDestroyWindow(impl_->window);
        delete impl_;
    }
}

PlatformWindow* PlatformWindow::create(const WindowDesc& desc) {
    auto win = new PlatformWindow();
    win->impl_ = new Impl();

    ensureGlfw();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
    win->impl_->window = glfwCreateWindow(
        static_cast<int>(desc.width),
        static_cast<int>(desc.height),
        desc.title.c_str(),
        nullptr,
        nullptr);
    if (!win->impl_->window) {
        delete win;
        return nullptr;
    }
    return win;
}

PlatformWindow* PlatformWindow::create(const WindowDesc& desc, const ln::GraphicsContextDesc& gfxDesc) {
    auto* win = PlatformWindow::create(desc);
    if (!win) return nullptr;

    auto ctxResult = ln::GraphicsContext::createForWindow(win, gfxDesc);
    if (!ctxResult) {
        delete win;
        return nullptr;
    }
    win->impl_->graphicsContext = std::move(*ctxResult);
    return win;
}

ln::GraphicsContext* PlatformWindow::graphicsContext() const {
    return impl_ ? impl_->graphicsContext.get() : nullptr;
}

bool PlatformWindow::processEvents() {
    glfwPollEvents();
    return !glfwWindowShouldClose(impl_->window);
}

NativeWindowHandle PlatformWindow::nativeHandle() const {
    NativeWindowHandle h{};
    h.glfwWindow = impl_->window;
    return h;
}

void PlatformWindow::framebufferSize(u32& width, u32& height) const {
    int w = 0, h = 0;
    glfwGetFramebufferSize(impl_->window, &w, &h);
    width = static_cast<u32>(w);
    height = static_cast<u32>(h);
}

} // namespace ln::platform
