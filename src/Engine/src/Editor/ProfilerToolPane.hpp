#pragma once
#include <LuminoEngine/UI/UIFrameWindow.hpp>
#include <LuminoEngine/UI/ImGuiIntegration.hpp>

namespace ln {
namespace detail {

class ProfilerToolPane
    : public ImGuiDockPane
{
public:

protected:
    void onGui() override;

private:
};

} // namespace detail
} // namespace ln

