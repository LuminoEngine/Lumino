#pragma once
#include "PlatformWindow.hpp"

namespace ln {

/**
 * ウィンドウシステムをラップせず、外部からウィンドウイベントを受け取りリスナーへ通知するための仮想的な PlatformWindow。
 */
class ExternalProxyPlatformWindow : public PlatformWindow {
public:
    ExternalProxyPlatformWindow();
    Result init(const WindowCreationSettings& settings);
    void setWindowTitle(const String& title) override {}
    void getSize(SizeI* size) override { *size = m_size; }
    void setSize(const SizeI& size) override { LN_NOTIMPLEMENTED(); }
    void getFramebufferSize(int* width, int* height) override {
        *width = m_size.width;
        *height = m_size.height;
    }
    void setAllowDragDrop(bool value) override;
    bool isAllowDragDrop() const override;
    PointI pointFromScreen(const PointI& screenPoint) override { return screenPoint; }
    PointI pointToScreen(const PointI& clientPoint) override { return clientPoint; }
    void grabCursor() override {}
    void releaseCursor() override {}


    void injectSizeChanged(int width, int height);

private:
    SizeI m_size;
};

} // namespace ln

