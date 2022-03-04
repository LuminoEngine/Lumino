#pragma once
#include "Common.hpp"

namespace ln {

class PlatformApplication
    : public RefObject {
public:
    PlatformApplication();
    int run();

protected:
    void onFrameBegin();

private:
    detail::PlatformManager* m_manager;
};

} // namespace ln
