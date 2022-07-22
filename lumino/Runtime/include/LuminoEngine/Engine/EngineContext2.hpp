#pragma once
#include <LuminoCore/Runtime/TypeInfo.hpp>
#include <LuminoCore/Runtime/Property.hpp>
#include "Common2.hpp"
#include "../RuntimeModule.hpp"

namespace ln {
namespace detail {
class RuntimeManager;
class AssetManager;
} // namespace detail

class EngineContext2 {
    /*
     * [2021/9/10] なぜモジュールの細分化を行うのか？
     * ----------
     * - 移植やテストを段階的に進められるようにするため。
     *   - 全部まとめて Web に移植！とかだとつらい。
     *     動作確認大変な他、全部モジュールのビルドを通さないと動かすこともできない。
     * - 依存関係の違反を検出しやすくするため。
     *     - 一部だけモジュールを使いたいというときに対応できなくなる。
     * - 実験的な機能を分離しやすくするため。
     *
     * ユーザーのためというより、検証をやりやすくするための対策。
     */
public:
    static EngineContext2* instance() { return s_instance.get(); };

    /** Initialize context. */
    static bool initialize(const RuntimeModuleSettings& settings, EngineContext2* sharedContext = nullptr);

    /** Terminate context. */
    static void terminate();

    /** Register module to this context. */
    void registerModule(Module* mod);

    /** Unregister module from this context. */
    void unregisterModule(Module* mod);

    const Ref<Dispatcher>& mainThreadTaskDispatcher() const { return m_mainThreadTaskDispatcher; }

    const Ref<DiagnosticsManager>& activeDiagnostics() const { return m_activeDiagnostics; }



    // TODO:
    RefObject* platformManager = nullptr;
    RefObject* runtimeManager = nullptr;
    RefObject* shaderManager = nullptr;
    RefObject* fontManager = nullptr;
    RefObject* graphicsManager = nullptr;

    const URef<detail::AssetManager>& assetManager() const { return m_assetManager; }

    ~EngineContext2();

private:
    EngineContext2();
    bool init(const RuntimeModuleSettings& settings);
    void dispose();

    static std::unique_ptr<EngineContext2> s_instance;

    List<Ref<Module>> m_modules;
    Ref<Dispatcher> m_mainThreadTaskDispatcher;
    Ref<DiagnosticsManager> m_activeDiagnostics;

    Ref<detail::RuntimeManager> m_runtimeManager;
    URef<detail::AssetManager> m_assetManager;
};

} // namespace ln
