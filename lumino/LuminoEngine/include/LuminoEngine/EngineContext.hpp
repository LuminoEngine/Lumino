#pragma once
#include "Engine/Property.hpp"

namespace ln {
namespace detail {
class RuntimeManager;
class EngineManager;
}

class EngineContext : public RefObject
{
public:
	static EngineContext* current();

	// EngineManager の初期化が行われる直前に呼び出される。
	// Binding で、ラインタイム情報をもとに EngineSetting を最終調整するために使用する。
	std::function<void()> engineManagerPreInit;

	// EngineManager の初期化直後に呼び出される。
	std::function<void()> engineManagerPostInit;

    EngineContext();
	virtual ~EngineContext();
	void internalInit();

    // TODO: 外部用。
    template<class T>
    void registerType() {
        T::_lnref_registerTypeInfo();
    }



    // TODO: 内部用。コールバックから呼び出す
	template<class TClassType>
	TypeInfo* registerType(const char* className, TypeInfo* baseType, std::initializer_list<Ref<PropertyInfo>> propInfos)//(std::initializer_list<Ref<PropertyAccessor>> accessors)
	{
		auto localName = String::fromCString(className);
		//TypeInfo* typeInfo = TypeInfo::getTypeInfo<TClassType>();

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
		if (itr == m_typeInfoSet.end())
		{
			auto typeInfo = makeRef<TypeInfo>(String::fromCString(className), baseType);

            typeInfo->m_id = static_cast<int>(m_typeInfos.size());
            m_typeInfos.push_back(typeInfo);

			typeInfo->m_factory = [](const TypeInfo*) { return detail::makeObjectHelper<TClassType>(); };

			m_typeInfoSet.insert({ typeInfo->name(), typeInfo });

			for (auto& p : propInfos) {
				typeInfo->registerProperty(p);
				//typeInfo->registerProperty(makeRef<PropertyInfo>(a));
			}


			return typeInfo;
		}
		else {
			return itr->second;
		}
	}

	TypeInfo* registerType(const String& className, TypeInfo* baseType, TypeInfoClass typeClass)
	{
		auto typeInfo = makeRef<TypeInfo>(className, baseType, typeClass);
		typeInfo->m_id = static_cast<int>(m_typeInfos.size());
		m_typeInfos.push_back(typeInfo);
		m_typeInfoSet.insert({ typeInfo->name(), typeInfo });
		return typeInfo;
	}

	TypeInfo* findTypeInfo(const StringRef& name) const
	{
		auto itr = m_typeInfoSet.find(name);
		if (itr != m_typeInfoSet.end()) {
			return itr->second;
		}
		else {
			return nullptr;
		}
	}

    TypeInfo* findTypeInfo(int id) const
    {
        if (0 <= id && id < static_cast<int>(m_typeInfos.size())) {
            return m_typeInfos[id];
        }
        else {
            return nullptr;
        }
    }

    TypeInfo* acquireTypeInfo(const StringRef& name);

	TypeInfo* objectTypeInfo() const { return m_objectTypeInfo; }

	const Ref<detail::RuntimeManager>& runtimeManager() const { return m_runtimeManager; }
	const Ref<detail::EngineManager>& engineManager() const { return m_engineManager; }

// TODO: internal
	void initializeRuntimeManager();
	void initializeEngineManager();
	void disposeRuntimeManager();
	void disposeEngineManager();

private:
	Ref<detail::RuntimeManager> m_runtimeManager;
	Ref<detail::EngineManager> m_engineManager;

	//std::unordered_set<TypeInfo*> m_typeInfoSet;
	std::unordered_map<String, TypeInfo*> m_typeInfoSet;
    std::vector<Ref<TypeInfo>> m_typeInfos;
	//List<TypeInfo*> m_typeInfos;
	TypeInfo* m_objectTypeInfo;
	bool m_init = false;
};

} // namespace ln

