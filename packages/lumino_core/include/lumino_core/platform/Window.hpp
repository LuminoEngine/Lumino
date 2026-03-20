#pragma once

#include <lumino_base/Types.hpp>
#include <functional>
#include <string>

namespace lumino::platform {

/// Window creation descriptor.
struct WindowDesc {
    std::string title = "Lumino";
    u32 width = 1280;
    u32 height = 720;
    bool resizable = false;
};

/// Platform-specific window handle.
struct NativeWindowHandle {
    void* glfwWindow = nullptr;
};

/// Platform window abstraction.
class PlatformWindow {
public:
    ~PlatformWindow();

    PlatformWindow(const PlatformWindow&) = delete;
    PlatformWindow& operator=(const PlatformWindow&) = delete;
    PlatformWindow(PlatformWindow&&) noexcept;
    PlatformWindow& operator=(PlatformWindow&&) noexcept;

    /// Create a platform window. Returns nullptr on failure.
    static PlatformWindow* create(const WindowDesc& desc);

    /// Poll events and return false if the window should close.
    bool processEvents();

    /// Get the native window handle.
    NativeWindowHandle nativeHandle() const;

    /// Get framebuffer size in pixels.
    void framebufferSize(u32& width, u32& height) const;

private:
    PlatformWindow() = default;

    struct Impl;
    Impl* impl_ = nullptr;
};

} // namespace lumino::platform
