#pragma once

#include <LuminoBase/Types.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Object.hpp>
#include <functional>
#include <string>

namespace ln {
class GraphicsModule;
class GraphicsContext;
struct GraphicsContextDesc;
} // namespace ln

namespace ln::platform {

/** Window creation descriptor. */
struct WindowDesc {
    std::string title = "Lumino";
    u32 width = 1280;
    u32 height = 720;
    bool resizable = false;
};

/** Platform-specific window handle. */
struct NativeWindowHandle {
    void* glfwWindow = nullptr;
};

/** Platform window abstraction. */
class PlatformWindow : public ln::Object {
public:
    ~PlatformWindow() override;

    PlatformWindow(const PlatformWindow&) = delete;
    PlatformWindow& operator=(const PlatformWindow&) = delete;

    /** Create a platform window with an attached graphics context. */
    static Result<Ref<PlatformWindow>> create(GraphicsModule* module, const WindowDesc& desc, const GraphicsContextDesc& gfxDesc);

    /** Poll events and return false if the window should close. */
    bool processEvents();

    /** Get the native window handle. */
    NativeWindowHandle nativeHandle() const;

    /** Get framebuffer size in pixels. */
    void framebufferSize(u32& width, u32& height) const;

    /** Get the graphics context attached to this window. May be nullptr. */
    GraphicsContext* graphicsContext() const;

private:
    PlatformWindow() = default;

    struct Impl;
    Impl* m_impl = nullptr;
};

} // namespace ln::platform
