#pragma once
#include "../Base/Delegate.hpp"

namespace ln {
class PropertyInfo;
class ViewPropertyInfo;
namespace detail {
struct TypeInfoInternal;
}

/**
 * ObjectCreationHandler
 */
LN_DELEGATE()
using ObjectCreationHandler = Delegate<Ref<Object>()>;

template<class T>
class TypeInfoTraits;

class PredefinedTypes {
public:
    static TypeInfo* Bool;
    static TypeInfo* Char;

    static TypeInfo* Int8;
    static TypeInfo* Int16;
    static TypeInfo* Int32;
    static TypeInfo* Int64;

    static TypeInfo* UInt8;
    static TypeInfo* UInt16;
    static TypeInfo* UInt32;
    static TypeInfo* UInt64;

    static TypeInfo* Float;
    static TypeInfo* Double;

    static TypeInfo* String;
    static TypeInfo* Object;
    static TypeInfo* List;

    // template<class T>
    // static TypeInfo* getType() { return nullptr; }
};

enum class TypeInfoClass {
    Primitive,
    /*Struct,*/
    Object,
};

template<class T>
class TypeInfoTraits {
public:
    static TypeInfo* typeInfo() noexcept { return T::_lnref_getTypeInfo(); }
};

template<>
class TypeInfoTraits<bool> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Bool; }
};

template<>
class TypeInfoTraits<ln::Char> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Char; }
};

template<>
class TypeInfoTraits<int8_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int8; }
};

template<>
class TypeInfoTraits<int16_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int16; }
};

template<>
class TypeInfoTraits<int32_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int32; }
};

template<>
class TypeInfoTraits<int64_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Int64; }
};

template<>
class TypeInfoTraits<uint8_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt8; }
};

template<>
class TypeInfoTraits<uint16_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt16; }
};

template<>
class TypeInfoTraits<uint32_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt32; }
};

template<>
class TypeInfoTraits<uint64_t> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::UInt64; }
};

template<>
class TypeInfoTraits<float> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Float; }
};

template<>
class TypeInfoTraits<double> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Double; }
};

template<>
class TypeInfoTraits<ln::String> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::String; }
};

template<>
class TypeInfoTraits<ln::Object> {
public:
    static TypeInfo* typeInfo() noexcept { return PredefinedTypes::Object; }
};

class TypeInfo
    : public RefObject {
public:
    static TypeInfo* registerType(const String& typeName, TypeInfo* baseClass, Ref<ObjectCreationHandler> factoryCallback);

public:
    TypeInfo(const String& className, TypeInfo* baseType, TypeInfoClass typeClass = TypeInfoClass::Object);

    TypeInfo(const String& className);

    virtual ~TypeInfo();

    /** クラス名を取得します。 */
    const String& name() const { return m_name; }

    /** ベースクラスの型情報を取得します。 */
    TypeInfo* baseType() const { return m_baseType; }

    TypeInfoClass typeClass() const { return m_typeClass; }

    // 0 is invalid
    int id() const { return m_id; }

    void registerProperty(PropertyInfo* prop);
    const List<Ref<PropertyInfo>>& properties() const { return m_properties; }

    void registerViewProperty(ViewPropertyInfo* prop);
    const List<Ref<ViewPropertyInfo>>& viewProperties() const { return m_viewProperties; }
    ViewPropertyInfo* findViewProperty(const StringView& name) const;

    Ref<Object> createInstance() const;
    static Ref<Object> createInstance(const String& typeName); // TODO: EngineContext へ

    /** 型引数に指定したクラス型の型情報を取得します。型情報が定義されていない場合、nullptr を返します。 */
    template<class T>
    static TypeInfo* getTypeInfo() {
        TypeInfo* typeInfo = TypeInfoTraits<T>::typeInfo();
        return typeInfo;
    }

    // ネストされた型の関数特殊化は不可能
    // template<> template<class TValue> static TypeInfo* getTypeInfo<typename Ref<TValue>>() { return getTypeInfo<TValue>(); }

    static TypeInfo* getTypeInfo(const Object* obj);

    static void initializeObjectProperties(Object* obj);

    // TODO: internal
    std::function<Ref<Object>(const TypeInfo* typeinfo)> m_factory;
    TypeInfo* m_baseType;
    int m_id = -1;

private:
    String m_name;
    TypeInfoClass m_typeClass;
    List<Ref<PropertyInfo>> m_properties; // obsolete
    List<Ref<ViewPropertyInfo>> m_viewProperties;
    int64_t m_managedTypeInfoId;

    friend struct detail::TypeInfoInternal;
};

namespace detail {
struct TypeInfoInternal {
    static void setManagedTypeInfoId(TypeInfo* typeInfo, int64_t id) { typeInfo->m_managedTypeInfoId = id; }
    static int64_t getManagedTypeInfoId(const TypeInfo* typeInfo) { return typeInfo->m_managedTypeInfoId; }
};
} // namespace detail

// class Serializer
//{
// public:
//	template<class T>
//	static String serialize(const Ref<T>&& value)
//	{
//		JsonTextOutputArchive ar;
//		ar.save(*value);
//		return ar.toString(JsonFormatting::None);
//	}
//
//	template<class T>
//	static Ref<T> deserialize(const StringView& jsonText)
//	{
//		Ref<T> value = makeObject<T>();
//		JsonTextInputArchive ar(jsonText);
//		ar.load(*value);
//		return value;
//	}
// };

// template<
//	typename TValue,
//	typename std::enable_if<std::is_abstract<TValue>::value_type, std::nullptr_t>::type = nullptr>
//	void serialize(Archive& ar, Ref<TValue>& value)
//{
//	bool isNull = (value == nullptr);
//	ar.makeSmartPtrTag(&isNull);
//
//	ln::String typeName;
//	if (ar.isSaving()) {
//		typeName = TypeInfo::getTypeInfo(value)->name();
//	}
//	ar.makeTypeInfo(&typeName);
//	//TODO: ここで makeTypeInfo すると、その中では setNextName しているため、現在の nextName をうわがいてしまう
//
//	if (ar.isSaving()) {
//		if (!isNull) {
//			ar.process(*value.get());
//		}
//	}
//	else {
//		if (!isNull) {
//			if (!typeName.isEmpty()) {
//				auto obj = TypeInfo::createInstance(typeName);
//				value = dynamic_pointer_cast<TValue>(obj);
//			}
//
//			//// TODO: TypeName が登録されていない場合はベースのを作るか、エラーにするかオプションで決められるようにしたい。
//			//if (!value) {
//			//	value = makeObject<TValue>();
//			//}
//
//			ar.process(*value.get());
//		}
//		else {
//			value = nullptr;
//		}
//	}
// }

namespace detail {

template<
    typename T,
    typename std::enable_if<std::is_abstract<T>::value, std::nullptr_t>::type = nullptr>
Ref<T> makeObjectHelper() {
    return nullptr;
}

template<
    typename T,
    typename std::enable_if<!std::is_abstract<T>::value, std::nullptr_t>::type = nullptr>
Ref<T> makeObjectHelper() {
    return makeObject<T>();
}

} // namespace detail

#if LN_USE_DEPRECATED_ARCHIVE

template<
    typename TValue,
    typename std::enable_if<detail::is_lumino_engine_object<TValue>::value, std::nullptr_t>::type = nullptr>
void serialize(Archive& ar, Ref<TValue>& value) {
    bool isNull = (value == nullptr);
    ar.makeSmartPtrTag(&isNull);

    ln::String typeName;
    if (ar.isSaving()) {
        typeName = TypeInfo::getTypeInfo(value)->name();
    }
    ar.makeTypeInfo(&typeName);
    // TODO: ここで makeTypeInfo すると、その中では setNextName しているため、現在の nextName をうわがいてしまう

    if (ar.isSaving()) {
        if (!isNull) {
            ar.process(*value.get());
        }
    }
    else {
        if (!isNull) {
            if (!typeName.isEmpty()) {
                auto obj = TypeInfo::createInstance(typeName);
                if (obj) {
                    value = dynamic_pointer_cast<TValue>(obj);
                }
            }

            // TODO: TypeName が登録されていない場合はベースのを作るか、エラーにするかオプションで決められるようにしたい。
            // TODO: Abstract 対策のためいったん無効化。多分有効化する必要はない気がするけど…。
            if (!value) {
                value = detail::makeObjectHelper<TValue>();
            }
            if (value) {

                printf("[Engine] start ar.process [%p]\n", value.get());
                ar.process(*value.get());
                printf("[Engine] end ar.process [%p]\n", value.get());
            }
        }
        else {
            value = nullptr;
        }
    }
}
#endif // LN_USE_DEPRECATED_ARCHIVE

} // namespace ln
