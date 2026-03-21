#include <lumino_core/platform/Window.hpp>

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
};

PlatformWindow::~PlatformWindow() {
    if (impl_) {
        if (impl_->window) glfwDestroyWindow(impl_->window);
        delete impl_;
    }
}

PlatformWindow::PlatformWindow(PlatformWindow&& o) noexcept : impl_(o.impl_) {
    o.impl_ = nullptr;
}

PlatformWindow& PlatformWindow::operator=(PlatformWindow&& o) noexcept {
    if (this != &o) {
        delete impl_;
        impl_ = o.impl_;
        o.impl_ = nullptr;
    }
    return *this;
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
