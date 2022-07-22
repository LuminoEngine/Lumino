#pragma once

/*
Property を使うときは以下のように実装しなければならない。

```
// .hpp
class ObjectA : public Object {
    private:
    Property<int> m_prop1;

    public:
    LN_PROPERTY_DECLARE(int, "Prop1")

    LN_PROPERTY()
    void setProp1(int value) { m_prop1 = value; }   // 何か別の処理をしてはならない
    LN_PROPERTY()
    int prop1() const { return m_prop1; }  // 何か別の処理をしてはならない

    private:
    void onProp1Changed();
};
```

```
// .cpp

LN_PROPERTY_IMPLEMENT(ObjectA, m_prop1, metadata);

ObjectA::init()
{
    Object::init();   // 必須
}

void ObjectA::onProp1Changed()
{
     何かする。ひとまずは WPF のように oldValue や newValue は作らない。（Variant 経由するオーバーヘッド短縮) = m_prop1;
}

```

*/

#include "Variant.hpp"
#include "TypeInfo.hpp"

namespace ln {
class PropertyAccessor;
class PropertyInfo;
class PropertyBase;
template<typename T>
class Property;
using GetPropertyCallback = PropertyBase* (*)(Object* obj);
using StaticPropertyChangedCallback = void (*)(Object* obj);

// TODO: detail
enum class PropertySetSource {
    Default = 0,
    ByInherit = 1,
    ByLocal = 2,
    ByAnimation = 3,
};

#define LN_PROPERTY_DECLARE(valueType, propertyName) \
    static const ::ln::Ref<::ln::PropertyInfo> propertyName##PropertyId;
// static const ::ln::PropertyInfo* propertyName##Id;

#define LN_PROPERTY_IMPLEMENT(classType, propertyName, member, metadata)                                         \
    const ::ln::Ref<::ln::PropertyInfo> classType::propertyName##PropertyId = ::ln::makeRef<::ln::PropertyInfo>( \
        ::ln::TypeInfo::getTypeInfo<classType>(),                                                                \
        [](Object* obj) -> PropertyBase* { return &static_cast<classType*>(obj)->member; },                      \
        metadata);
//static ::ln::PropertyInfo _##ownerClass##_##propertyName##Id(metadata); \
    //const ::ln::PropertyInfo* ownerClass::propertyName##Id = &_##ownerClass##_##propertyName##Id;

class PropertyRef {
public:
    PropertyRef()
        : m_propOwner()
        , m_accessor(nullptr) {}

    PropertyRef(Object* propOwner, PropertyAccessor* accessor)
        : m_propOwner(propOwner)
        , m_accessor(accessor) {}

    bool isNull() const {
        return m_accessor == nullptr;
    }

    std::pair<Ref<Object>, PropertyAccessor*> resolve() const {
        auto ptr = m_propOwner.resolve();
        if (ptr != nullptr) {
            return { ptr, m_accessor };
        }
        else {
            return { nullptr, nullptr };
        }
    }

private:
    WeakRefPtr<Object> m_propOwner;
    PropertyAccessor* m_accessor;
};

class PropertyMetadata {
public:
    PropertyMetadata()
        : m_staticPropertyChangedCallback(nullptr) {
    }

    PropertyMetadata(StaticPropertyChangedCallback callback)
        : m_staticPropertyChangedCallback(callback) {
    }

    StaticPropertyChangedCallback staticPropertyChangedCallback() const { return m_staticPropertyChangedCallback; }

private:
    StaticPropertyChangedCallback m_staticPropertyChangedCallback;
};

class PropertyInfo
    : public RefObject {
public:
    PropertyInfo(TypeInfo* ownerClassType, GetPropertyCallback getPropertyCallback, const PropertyMetadata& metadata)
        : m_getPropertyCallback(getPropertyCallback)
        , m_staticPropertyChangedCallback(metadata.staticPropertyChangedCallback())
        , m_registerd(false) {
        ownerClassType->registerProperty(this);
    }

    PropertyInfo(const char* name, const Ref<PropertyAccessor>& accessor)
        : m_name(String::fromCString(name))
        , m_accessor(accessor)
        , m_registerd(false) {
        m_getPropertyCallback = nullptr;
        m_staticPropertyChangedCallback = nullptr;
    }

    const String& name() const { return m_name; }
    const Ref<PropertyAccessor>& accessor() const { return m_accessor; }

    template<typename TValue>
    static void setTypedValue(Object* obj, PropertyInfo* propertyInfo, TValue&& value) {
        PropertyBase* prop = propertyInfo->m_getPropertyCallback(obj);
        static_cast<Property<TValue>*>(prop)->set(std::forward(value));
    }

    static PropertyRef getPropertyRef(Object* obj, PropertyInfo* propertyInfo);

    // TODO: Helper でいい
    static void notifyPropertyChanged(Object* ownerObject, PropertyBase* target, const PropertyInfo* prop, PropertySetSource source);

    // template<class TClass>
    // bool verifyTypeInfo() const
    //{
    //     auto ptr = dynamic_cast<PropertyAccessorImpl<TClass>>
    // }

private:
    GetPropertyCallback m_getPropertyCallback;
    StaticPropertyChangedCallback m_staticPropertyChangedCallback;
    bool m_registerd;

    // new model
    String m_name;
    Ref<PropertyAccessor> m_accessor;

    friend class TypeInfo;
};

// namespace detail {
//
// class PropertyInitializer
//{
// public:
//     PropertyInitializer(const PropertyMetadata& metadata)
//     {
//     }
// };
//
// } // namespace detail

class PropertyBase {
public:
    virtual void setValue(const Ref<Variant>& value) = 0;
    virtual Ref<Variant> getValue() const = 0;
    virtual void clearValue() = 0;

protected:
    PropertyBase()
        : m_owner(nullptr)
        , m_propertyInfo(nullptr) {
    }

public: // TODO: Helper
    Object* ownerObject() const { return m_owner; }
    PropertyInfo* propertyInfo() const { return m_propertyInfo; }

private:
    Object* m_owner;
    PropertyInfo* m_propertyInfo;
    bool m_registerd;
    friend class TypeInfo;
};

template<typename TValue>
class Property
    : public PropertyBase {
public:
    Property()
        : PropertyBase()
        , m_defaultValue()
        , m_value()
        , m_valueSource(PropertySetSource::Default) {
    }

    Property(const TValue& value)
        : PropertyBase(/*owner*/)
        , m_defaultValue(value)
        , m_value(value)
        , m_valueSource(PropertySetSource::Default) {}

    ~Property() {}

    virtual void setValue(const Ref<Variant>& value) override {
        set(value->get<TValue>());
    }

    virtual Ref<Variant> getValue() const override {
        return makeVariant(get()); // TODO: Variant Pool
    }

    /** プロパティのローカル値を設定します。*/
    void set(const TValue& value) {
        setInternal(value, PropertySetSource::ByLocal);
    }

    /** プロパティのローカル値を取得します。*/
    const TValue& get() const { return m_value; }

    /** プロパティのローカル値を消去します。*/
    virtual void clearValue() override {
        if (m_value != m_defaultValue || m_valueSource != PropertySetSource::Default) {
            m_value = m_defaultValue;
            m_valueSource = PropertySetSource::Default;
            PropertyInfo::notifyPropertyChanged(ownerObject(), this, propertyInfo(), m_valueSource);
        }
    }

    Property& operator=(const TValue& value) {
        set(value);
        return *this;
    }
    operator const TValue&() const { return m_value; }

    // const PropertyInfo* getPropertyInfo() const { return m_propId; }

    LN_INTERNAL_ACCESS :
        // uint32_t getHashCode() const
        //{
        //     return
        //         Hash::calcHash(reinterpret_cast<const char*>(&m_value), sizeof(m_value)) +
        //         Hash::calcHash(reinterpret_cast<const char*>(&m_valueSource), sizeof(m_valueSource));
        // }

        bool
        inherit(const Property& parent) {
        if (m_valueSource <= PropertySetSource::ByInherit) {
            bool changed = false;
            if (m_value != parent.m_value) {
                m_value = parent.m_value;
                changed = true;
            }
            m_valueSource = PropertySetSource::ByInherit;
            return changed;
        }
        return false;
    }

    LN_INTERNAL_ACCESS : void setValueSource(PropertySetSource source) {
        m_valueSource = source;
    }

private:
    LN_DISALLOW_COPY_AND_ASSIGN(Property);

    void setInternal(const TValue& value, PropertySetSource source) {
        if (m_value != value || m_valueSource != source) {
            m_value = value;
            m_valueSource = source;
            PropertyInfo::notifyPropertyChanged(ownerObject(), this, propertyInfo(), m_valueSource);
        }
    }

    TValue m_defaultValue;
    TValue m_value;
    PropertySetSource m_valueSource;

    friend class PropertyHelper;
    friend class PropertyInfo;
};

template<typename TValue>
inline bool operator==(const Property<TValue>& lhs, const TValue& rhs) {
    return lhs.get() == rhs;
}
template<typename TValue>
inline bool operator==(const TValue& lhs, const Property<TValue>& rhs) {
    return lhs == rhs.get();
}
template<typename TValue>
inline bool operator!=(const Property<TValue>& lhs, const TValue& rhs) {
    return lhs.get() != rhs;
}
template<typename TValue>
inline bool operator!=(const TValue& lhs, const Property<TValue>& rhs) {
    return lhs != rhs.get();
}

//------------------------------------------------------------------------------

class PropertyAccessor : public RefObject {
public:
    virtual void getValue(const Object* obj, Ref<Variant>* value) const = 0;
    virtual void setValue(Object* obj, const Ref<Variant>& value) = 0;
    virtual void serializeMember(Object* obj, Archive& ar, const ln::String& name) = 0;
};

// 呼び出し側が型を知っている場合、PropertyAccessor からキャストすることで Variant を介すことなく直接値を操作できるようにするための中間クラス
template<class TValue>
class TypedPropertyAccessor : public PropertyAccessor {
public:
    virtual void getValueDirect(const Object* obj, TValue* value) const = 0;
    virtual void setValueDirect(Object* obj, const TValue& value) = 0;
};

template<class TClassType, class TValue, class TGetFunction, class TSetFunction>
class PropertyAccessorImpl : public TypedPropertyAccessor<TValue> {
public:
    PropertyAccessorImpl(TGetFunction getFunction, TSetFunction setFunction)
        : m_getFunction(getFunction)
        , m_setFunction(setFunction) {}

    void getValue(const Object* obj, Ref<Variant>* value) const override {
        LN_DCHECK(obj);
        LN_DCHECK(value);
        const auto classPtr = static_cast<const TClassType*>(obj);
        TValue t;
        m_getFunction(classPtr, &t);
        *value = makeVariant(t); // DOTO: Variant Pool
    }

    void setValue(Object* obj, const Ref<Variant>& value) override {
        LN_DCHECK(obj);
        auto classPtr = static_cast<TClassType*>(obj);
        m_setFunction(classPtr, value->get<TValue>());
    }

    virtual void getValueDirect(const Object* obj, TValue* value) const override {
        LN_CHECK(obj);
        const auto classPtr = static_cast<const TClassType*>(obj);
        m_getFunction(classPtr, value);
    }

    virtual void setValueDirect(Object* obj, const TValue& value) override {
        LN_DCHECK(obj);
        if (auto classPtr = dynamic_cast<TClassType*>(obj)) {
            m_setFunction(classPtr, value);
        }
        // auto classPtr = static_cast<TClassType*>(obj);
        // m_setFunction(classPtr, value);
    }

    virtual void serializeMember(Object* obj, Archive& ar, const ln::String& name) {
        LN_CHECK(obj);
        if (ar.isSaving()) {
            TValue value;
            const auto classPtr = static_cast<const TClassType*>(obj);
            m_getFunction(classPtr, &value);
            ar& ln::makeNVP(name, value);
        }
        else {
            TValue value;
            auto classPtr = static_cast<TClassType*>(obj);
            ar& ln::makeNVP(name, value);
            m_setFunction(classPtr, value);
        }
    }

private:
    TGetFunction m_getFunction;
    TSetFunction m_setFunction;
};

template<class TClassType, class TValue, class TGetFunction, class TSetFunction>
Ref<PropertyAccessor> makePropertyAccessor(TGetFunction getFunction, TSetFunction setFunction) {
    return Ref<PropertyAccessor>(LN_NEW PropertyAccessorImpl<TClassType, TValue, TGetFunction, TSetFunction>(getFunction, setFunction), false);
}

class PropertyPath
    : public RefObject {
public:
    static PropertyRef findProperty(Object* root, const PropertyPath* path);

    std::vector<String> m_objectNames;

    String m_propertyName;

    // ローカル部 (リフレクション管理外の、構造体メンバなどを示す。Transform なら "position.x" -> ["position", "x"] など)
    std::vector<String> m_local;
};

class ReflectionObjectVisitor {
public:
    // bool visitObject(Object* obj);
    virtual bool visitProperty(Object* obj, PropertyInfo* prop);
};

#define LN_MAKE_GET_SET_PROPERTY_ACCESSOR(className, typeName, getFunction, setFunction) \
    makePropertyAccessor<className, typeName>(                                           \
        [](const className* self, typeName* value) { *value = self->getFunction(); },    \
        [](className* self, const typeName& value) { self->setFunction(value); }) // TOO: typeName と、OwnerClass を間違えてしまったときの対策。(setValueDirect とかで stack 破壊する)

} // namespace ln
