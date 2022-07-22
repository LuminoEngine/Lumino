#pragma once
#include "Common.hpp"
#include <LuminoCore/Runtime/TypeInfo.hpp>
#include <LuminoCore/Runtime/Object.hpp>
#include <LuminoCore/Runtime/Property.hpp>

namespace ln {
namespace detail {
class IObjectEventListener {
public:
    virtual void onDestructObject(Object* obj) = 0;
    virtual void onRetainedObject(Object* obj) = 0;
    virtual void onReleasedObject(Object* obj) = 0;
};
} // namespace detail

class RuntimeContext final {
public:
	static RuntimeContext* current();

    TypeInfo* objectTypeInfo() const { return m_objectTypeInfo; }

    detail::IObjectEventListener* objectEventListener = nullptr;

    TypeInfo* registerType(const String& className, TypeInfo* baseType, TypeInfoClass typeClass);

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

    // TODO: 外部用。
    template<class T>
    void registerType() {
        T::_lnref_registerTypeInfo(this);
    }

    TypeInfo* findTypeInfo(const StringView& name) const;

    TypeInfo* findTypeInfo(int id) const;

    TypeInfo* acquireTypeInfo(const StringView& name);
	
    
	
	void clear();

private:
    RuntimeContext();

    std::vector<Ref<TypeInfo>> m_typeInfos;
    std::unordered_map<String, TypeInfo*> m_typeInfoSet; // for finding
    TypeInfo* m_objectTypeInfo;

	static RuntimeContext s_default;
};

} // namespace ln
