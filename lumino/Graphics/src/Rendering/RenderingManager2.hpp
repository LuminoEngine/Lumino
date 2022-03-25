#pragma once
#include <LuminoGraphics/Rendering/Common.hpp>

namespace ln {
namespace detail {

class RenderingManager2 : public URefObject {
public:
    struct Settings {
    };

    RenderingManager2();
    Result init(const Settings& settings);
    void dispose();

    const Ref<VertexLayout>& standardVertexLayout() const { return m_standardVertexLayout; }

private:
    Ref<VertexLayout> m_standardVertexLayout;
};

} // namespace detail
} // namespace ln
