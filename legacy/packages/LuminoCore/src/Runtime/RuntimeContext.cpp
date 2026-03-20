#include <LuminoCore/Runtime/TypeInfo.hpp>
#include <LuminoCore/Runtime/Object.hpp>
#include <LuminoCore/Runtime/RuntimeContext.hpp>

namespace ln {

//==============================================================================
// RuntimeContext
	
RuntimeContext RuntimeContext::s_default;

RuntimeContext* RuntimeContext::current() {
    return &s_default;
}

RuntimeContext::RuntimeContext() 
	: m_typeInfos()
    , m_typeInfoSet()
    , m_objectTypeInfo(nullptr) {
	
    m_typeInfos.push_back(nullptr); // [0] is dummy

    PredefinedTypes::Char = registerType(_TT("Char"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int8 = registerType(_TT("Int8"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int16 = registerType(_TT("Int16"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int32 = registerType(_TT("Int32"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Int64 = registerType(_TT("Int64"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt8 = registerType(_TT("UInt8"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt16 = registerType(_TT("UInt16"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt32 = registerType(_TT("UInt32"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::UInt64 = registerType(_TT("UInt64"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Float = registerType(_TT("Float"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Double = registerType(_TT("Double"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::String = registerType(_TT("String"), nullptr, TypeInfoClass::Primitive);
    PredefinedTypes::Object = registerType<Object>("Object", nullptr, {});
    PredefinedTypes::List = registerType<Object>("List", nullptr, {});

    m_objectTypeInfo = PredefinedTypes::Object;
}

void RuntimeContext::clear() {
    m_typeInfos.clear();
    m_typeInfoSet.clear();
}

TypeInfo* RuntimeContext::registerType(const String& className, TypeInfo* baseType, TypeInfoClass typeClass) {
    auto typeInfo = makeRef<TypeInfo>(className, baseType, typeClass);
    typeInfo->m_id = static_cast<int>(m_typeInfos.size());
    m_typeInfos.push_back(typeInfo);
    m_typeInfoSet.insert({ typeInfo->name(), typeInfo });
    return typeInfo;
}

TypeInfo* RuntimeContext::findTypeInfo(const StringView& name) const {
    auto itr = m_typeInfoSet.find(name);
    if (itr != m_typeInfoSet.end()) {
        return itr->second;
    }
    else {
        return nullptr;
    }
}

TypeInfo* RuntimeContext::findTypeInfo(int id) const {
    if (0 <= id && id < static_cast<int>(m_typeInfos.size())) {
        return m_typeInfos[id];
    }
    else {
        return nullptr;
    }
}

TypeInfo* RuntimeContext::acquireTypeInfo(const StringView& name) {
    auto* r = findTypeInfo(name);
    if (r) {
        return r;
    }
    else {
        auto typeInfo = makeRef<TypeInfo>(name);
        typeInfo->m_id = static_cast<int>(m_typeInfos.size());
        m_typeInfos.push_back(typeInfo);
        m_typeInfoSet.insert({ typeInfo->name(), typeInfo });
        return typeInfo;
    }
}

} // namespace ln
