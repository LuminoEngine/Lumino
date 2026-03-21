#pragma once

namespace ln {


class CoreInstance final {
public:
    struct Settings {};
    static CoreInstance* instance() { return s_instance.get(); };
    static VoidResult initialize(const Settings& settings);
    static void terminate();

private:
    VoidResult init(const Settings& settings);
    void dispose();
    static std::unique_ptr<CoreInstance> s_instance;
    Settings m_settings;
};

} // namespace ln
