#pragma once

namespace ln {

class ObjectRegistry;

// Library root manager クラス。シングルトンで、ライブラリ全体の初期化や終了処理を管理する。
class CoreInstance final {
public:
    struct Settings {};
    static CoreInstance* instance() { return s_instance.get(); };
    static VoidResult initialize(const Settings& settings);
    static void terminate();

    ObjectRegistry* objectRegistry() { return m_objectRegistry.get(); }

private:
    VoidResult init(const Settings& settings);
    void dispose();
    static std::unique_ptr<CoreInstance> s_instance;
    Settings m_settings;
    std::unique_ptr<ObjectRegistry> m_objectRegistry;
};

} // namespace ln
