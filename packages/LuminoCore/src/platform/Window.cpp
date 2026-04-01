#include <LuminoCore/platform/Window.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>

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
    if (m_impl) {
        // Graphics context must be torn down before the GLFW window
        m_impl->graphicsContext.reset();
        if (m_impl->window) glfwDestroyWindow(m_impl->window);
        delete m_impl;
    }
}

Result<Ref<PlatformWindow>> PlatformWindow::create(const WindowDesc& desc) {
    auto win = Ref<PlatformWindow>::adopt(new PlatformWindow());
    win->m_impl = new Impl();

    ensureGlfw();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
    win->m_impl->window = glfwCreateWindow(
        static_cast<int>(desc.width),
        static_cast<int>(desc.height),
        desc.title.c_str(),
        nullptr,
        nullptr);
    if (!win->m_impl->window) {
        return tl::make_unexpected(ln::Error{ln::ErrorCode::RuntimeError, "glfwCreateWindow failed"});
    }
    return win;
}

Result<Ref<PlatformWindow>> PlatformWindow::create(const WindowDesc& desc, const ln::GraphicsContextDesc& gfxDesc) {
    auto winResult = PlatformWindow::create(desc);
    if (!winResult) return tl::make_unexpected(winResult.error());
    auto win = std::move(*winResult);

    auto ctxResult = ln::GraphicsContext::createForWindow(win.get(), gfxDesc);
    if (!ctxResult) return tl::make_unexpected(ctxResult.error());
    win->m_impl->graphicsContext = std::move(*ctxResult);
    return win;
}

ln::GraphicsContext* PlatformWindow::graphicsContext() const {
    return m_impl ? m_impl->graphicsContext.get() : nullptr;
}

bool PlatformWindow::processEvents() {
    glfwPollEvents();
    return !glfwWindowShouldClose(m_impl->window);
}

NativeWindowHandle PlatformWindow::nativeHandle() const {
    NativeWindowHandle h{};
    h.glfwWindow = m_impl->window;
    return h;
}

void PlatformWindow::framebufferSize(u32& width, u32& height) const {
    int w = 0, h = 0;
    glfwGetFramebufferSize(m_impl->window, &w, &h);
    width = static_cast<u32>(w);
    height = static_cast<u32>(h);
}

} // namespace ln::platform
