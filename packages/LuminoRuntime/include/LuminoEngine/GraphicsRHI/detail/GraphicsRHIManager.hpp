#pragma once

namespace ln {
namespace detail {

class GraphicsRHIManager : public RefObject {
public:
    struct Settings {
        Settings() {}
    };

    static GraphicsRHIManager* initialize(const Settings& settings);
    static void terminate();
    static inline GraphicsRHIManager* instance() { return s_instance; }

private:
    GraphicsRHIManager();
    virtual ~GraphicsRHIManager() = default;
    Result<> init(const Settings& settings);
    void dispose();

    static GraphicsRHIManager* s_instance;
};

} // namespace detail
} // namespace ln
