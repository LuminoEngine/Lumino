#pragma once
#include "Common2.hpp"
#include "../Runtime/Runtime.hpp"
#include "../Reflection/TypeInfo.hpp"
#include "../Reflection/Property.hpp"

namespace ln {
namespace detail {
class RuntimeManager;
class AssetManager;
class IObjectEventListener {
public:
    virtual void onDestructObject(Object* obj) = 0;
    virtual void onRetainedObject(Object* obj) = 0;
    virtual void onReleasedObject(Object* obj) = 0;
};
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

    // TODO: 外部用。
    template<class T>
    void registerType() {
        T::_lnref_registerTypeInfo(this);
    }

    // TODO: 内部用。コールバックから呼び出す
    template<class TClassType>
    TypeInfo* registerType(const char* className, TypeInfo* baseType, std::initializer_list<Ref<PropertyInfo>> propInfos) //(std::initializer_list<Ref<PropertyAccessor>> accessors)
    {
        auto localName = String::fromCString(className);
        // TypeInfo* typeInfo = TypeInfo::getTypeInfo<TClassType>();

        // Promise や Delegate のテンプレートインスタンスは alias で定義したいが、そうすると型名を簡単に指定する方法が無い。
        // ただこれらはシリアライズのように型名からインスタンスを作るようなことは無く、Binding の Managed 側のオブジェクトを new するときなど
        // Managed 側の TypeInfo とマッピングさせるために異なるインスタンスを生成する必要がある。
        if (localName == _TT("__Promise")) {
            auto typeInfo = makeRef<TypeInfo>(String::fromCString(className), baseType);
            typeInfo->m_id = static_cast<int>(m_typeInfos.size());
            m_typeInfos.push_back(typeInfo);
            return typeInfo;
        }

        auto itr = m_typeInfoSet.find(localName);
        if (itr == m_typeInfoSet.end()) {
            auto typeInfo = makeRef<TypeInfo>(String::fromCString(className), baseType);

            typeInfo->m_id = static_cast<int>(m_typeInfos.size());
            m_typeInfos.push_back(typeInfo);

            typeInfo->m_factory = [](const TypeInfo*) { return detail::makeObjectHelper<TClassType>(); };

            m_typeInfoSet.insert({ typeInfo->name(), typeInfo });

            for (auto& p : propInfos) {
                typeInfo->registerProperty(p);
                // typeInfo->registerProperty(makeRef<PropertyInfo>(a));
            }

            return typeInfo;
        }
        else {
            return itr->second;
        }
    }

    TypeInfo* registerType(const String& className, TypeInfo* baseType, TypeInfoClass typeClass) {
        auto typeInfo = makeRef<TypeInfo>(className, baseType, typeClass);
        typeInfo->m_id = static_cast<int>(m_typeInfos.size());
        m_typeInfos.push_back(typeInfo);
        m_typeInfoSet.insert({ typeInfo->name(), typeInfo });
        return typeInfo;
    }

    TypeInfo* findTypeInfo(const StringView& name) const {
        auto itr = m_typeInfoSet.find(name);
        if (itr != m_typeInfoSet.end()) {
            return itr->second;
        }
        else {
            return nullptr;
        }
    }

    TypeInfo* findTypeInfo(int id) const {
        if (0 <= id && id < static_cast<int>(m_typeInfos.size())) {
            return m_typeInfos[id];
        }
        else {
            return nullptr;
        }
    }

    TypeInfo* acquireTypeInfo(const StringView& name);

    TypeInfo* objectTypeInfo() const { return m_objectTypeInfo; }

    // TODO:
    RefObject* platformManager = nullptr;
    RefObject* runtimeManager = nullptr;
    RefObject* shaderManager = nullptr;
    RefObject* fontManager = nullptr;
    RefObject* graphicsManager = nullptr;
    detail::IObjectEventListener* objectEventListener = nullptr;

    const URef<detail::AssetManager>& assetManager() const { return m_assetManager; }

private:
    EngineContext2();
    bool init(const RuntimeModuleSettings& settings);
    void dispose();

    static std::unique_ptr<EngineContext2> s_instance;

    List<Ref<Module>> m_modules;
    Ref<Dispatcher> m_mainThreadTaskDispatcher;
    Ref<DiagnosticsManager> m_activeDiagnostics;

    std::unordered_map<String, TypeInfo*> m_typeInfoSet;
    std::vector<Ref<TypeInfo>> m_typeInfos;
    TypeInfo* m_objectTypeInfo;

    URef<detail::RuntimeManager> m_runtimeManager;
    URef<detail::AssetManager> m_assetManager;
};

} // namespace ln
