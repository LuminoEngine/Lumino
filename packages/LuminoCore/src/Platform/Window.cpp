#include <LuminoCore/Platform/Window.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/CoreInstance.hpp>

#if defined(__EMSCRIPTEN__)
    #include <emscripten/html5.h>
    #include <string>
#elif defined(LN_NX)
    #include <nn/nn_Common.h>
    #include <nn/os.h>
    #include <nn/vi.h>
#else
    #define GLFW_INCLUDE_NONE
    #include <GLFW/glfw3.h>
#endif

namespace ln::platform {

// ─── Impl ───
struct PlatformWindow::Impl {
#if defined(__EMSCRIPTEN__)
    // Web: 紐づく canvas の CSS セレクタ (例: "#my_canvas") だけを保持する。
    // GraphicsContext は Phase 2 で WebGPU rhi 側から作られる予定。
    std::string canvasSelector;
#else
    GLFWwindow* window = nullptr;
#endif
    Ref<ln::GraphicsContext> graphicsContext;
};

PlatformWindow::~PlatformWindow() {
    if (m_impl) {
        // GraphicsContext はネイティブウィンドウより先に破棄しなければならない
        m_impl->graphicsContext.reset();
#if !defined(__EMSCRIPTEN__)
        if (m_impl->window) glfwDestroyWindow(m_impl->window);
#endif
        delete m_impl;
    }
}

#if defined(__EMSCRIPTEN__)

Result<Ref<PlatformWindow>> PlatformWindow::create(
    GraphicsModule* module,
    const WindowDesc& desc,
    const ln::GraphicsContextDesc& gfxDesc) {

    // Web では canvas は HTML 側で用意済み。selector を保持するだけ。
    auto win = Ref<PlatformWindow>::adopt(new PlatformWindow());
    win->m_impl = new Impl();
    win->m_impl->canvasSelector = desc.canvasSelector;

    // GraphicsContext を canvas から作成する。
    auto ctxResult = ln::GraphicsContext::createForCanvas(
        module, desc.canvasSelector, desc.width, desc.height, gfxDesc);
    if (!ctxResult) return LN_FORWARD_ERROR(ctxResult);
    win->m_impl->graphicsContext = std::move(*ctxResult);

    return win;
}

bool PlatformWindow::processEvents() {
    // ブラウザ側で requestAnimationFrame が回るため no-op。終了は JS 側で表現する。
    return true;
}

NativeWindowHandle PlatformWindow::nativeHandle() const {
    NativeWindowHandle h{};
    h.canvasSelector = m_impl ? m_impl->canvasSelector.c_str() : nullptr;
    return h;
}

void PlatformWindow::framebufferSize(uint32_t& width, uint32_t& height) const {
    width  = 0;
    height = 0;
    if (!m_impl || m_impl->canvasSelector.empty()) return;
    int w = 0, h = 0;
    emscripten_get_canvas_element_size(m_impl->canvasSelector.c_str(), &w, &h);
    width  = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
}

#else

Result<Ref<PlatformWindow>> PlatformWindow::create(
    GraphicsModule* module,
    const WindowDesc& desc,
    const ln::GraphicsContextDesc& gfxDesc) {


    auto win = Ref<PlatformWindow>::adopt(new PlatformWindow());
    win->m_impl = new Impl();

    // GLFW は CoreInstance で初期化済み。
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, desc.resizable ? GLFW_TRUE : GLFW_FALSE);
    win->m_impl->window = glfwCreateWindow(
        static_cast<int>(desc.width),
        static_cast<int>(desc.height),
        desc.title.c_str(),
        nullptr,
        nullptr);
    if (!win->m_impl->window) {
        return LN_MAKE_ERROR("glfwCreateWindow failed");
    }

    //auto winResult = PlatformWindow::create(desc);
    //if (!winResult) return tl::make_unexpected(winResult.error());
    //auto win = std::move(*winResult);

    auto ctxResult = ln::GraphicsContext::createForWindow(module, win.get(), gfxDesc);
    if (!ctxResult) return LN_FORWARD_ERROR(ctxResult);
    win->m_impl->graphicsContext = std::move(*ctxResult);
    return win;
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

void PlatformWindow::framebufferSize(uint32_t& width, uint32_t& height) const {
    int w = 0, h = 0;
    glfwGetFramebufferSize(m_impl->window, &w, &h);
    width = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
}

#endif // __EMSCRIPTEN__

ln::GraphicsContext* PlatformWindow::graphicsContext() const {
    return m_impl ? m_impl->graphicsContext.get() : nullptr;
}

} // namespace ln::platform
