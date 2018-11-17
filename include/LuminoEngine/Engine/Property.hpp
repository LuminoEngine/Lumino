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

ObjectA::initialize()
{
    Object::initialize();   // 必須
}

void ObjectA::onProp1Changed()
{
     何かする。ひとまずは WPF のように oldValue や newValue は作らない。（Variant 経由するオーバーヘッド短縮) = m_prop1;
}

```

*/

namespace ln {

class PropertyInfo;
class PropertyBase;
template<typename T> class Property;
using GetPropertyCallback = PropertyBase*(*)(Object* obj);
using StaticPropertyChangedCallback = void(*)(Object* obj);

// TODO: detail
enum class PropertySetSource
{
    Default = 0,
    ByInherit = 1,
    ByLocal = 2,
    ByAnimation = 3,
};

#define LN_OBJECT2 \
    friend class ::ln::TypeInfo; \
    static TypeInfo* _lnref_getTypeInfo(); \
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override;

#define LN_OBJECT2_IMPLEMENT(classType, baseclassType) \
    TypeInfo* classType::_lnref_getTypeInfo() \
    { \
        static TypeInfo typeInfo(#classType, ::ln::TypeInfo::getTypeInfo<baseclassType>()); \
        return &typeInfo; \
    } \
    ::ln::TypeInfo* classType::_lnref_getThisTypeInfo() const { return _lnref_getTypeInfo(); }

#define LN_PROPERTY_DECLARE(valueType, propertyName) \
    static const ::ln::Ref<::ln::PropertyInfo> propertyName##PropertyId;
    //static const ::ln::PropertyInfo* propertyName##Id;

#define LN_PROPERTY_IMPLEMENT(classType, propertyName, member, metadata) \
    const ::ln::Ref<::ln::PropertyInfo> classType::propertyName##PropertyId = ::ln::makeRef<::ln::PropertyInfo>( \
        ::ln::TypeInfo::getTypeInfo<classType>(), \
        [](Object* obj) -> PropertyBase* { return &static_cast<classType*>(obj)->member; }, \
        metadata);
    //static ::ln::PropertyInfo _##ownerClass##_##propertyName##Id(metadata); \
    //const ::ln::PropertyInfo* ownerClass::propertyName##Id = &_##ownerClass##_##propertyName##Id;




class PropertyRef
{
public:
    PropertyRef(Object* propOwner, PropertyBase* prop)
        : m_propOwner(propOwner)
        , m_prop(prop)
    {
    }

    std::pair<Ref<Object>, PropertyBase*> resolve()
    {
        auto ptr = m_propOwner.resolve();
        if (ptr != nullptr) {
            return { ptr, m_prop };
        }
        else {
            return std::pair<Ref<Object>, PropertyBase*>();
        }
    }

    template<typename TValue>
    void setTypedValue(const TValue& value)
    {
        auto ptr = m_propOwner.resolve();
        if (ptr != nullptr) {
            static_cast<Property<TValue>*>(m_prop)->set(value);
        }
    }

    template<typename TValue>
    const TValue& getTypedValue() const
    {
        auto ptr = m_propOwner.resolve();
        if (ptr != nullptr) {
            return static_cast<Property<TValue>*>(m_prop)->get();
        }
        LN_ERROR();
        return TValue();
    }

    void clearValue();

    Ref<Object> owenr();

    //const PropertyInfo* propertyInfo() const
    //{
    //    return m_prop.getPropertyInfo();
    //}

    template<typename TValue>
    Property<TValue>* getTypedProperty()
    {
        return static_cast<Property<TValue>*>(m_prop);
    }

private:
    WeakRefPtr<Object>	m_propOwner;
    PropertyBase* m_prop;
};





class TypeInfo
    : public RefObject
{
public:
    TypeInfo(const char* className, TypeInfo* baseType)
        : m_name(className)
        , m_baseType(baseType)
    {}

    /** クラス名を取得します。 */
    const String& name() const { return m_name; }

    /** ベースクラスの型情報を取得します。 */
    TypeInfo* baseType() const { return m_baseType; }

    void registerProperty(PropertyInfo* prop);
    const List<PropertyInfo*>& properties() const { return m_properties; }

    /** 型引数に指定したクラス型の型情報を取得します。 */
    template<class T>
    static TypeInfo* getTypeInfo()
    {
        return T::_lnref_getTypeInfo();
    }

    static TypeInfo* getTypeInfo(const Object* obj)
    {
        return obj->_lnref_getThisTypeInfo();
    }

    static void initializeObjectProperties(Object* obj);

private:
    String m_name;
    TypeInfo* m_baseType;
    List<PropertyInfo*> m_properties;
};

class PropertyMetadata
{
public:
    PropertyMetadata()
        : m_staticPropertyChangedCallback(nullptr)
    {
    }

    PropertyMetadata(StaticPropertyChangedCallback callback)
        : m_staticPropertyChangedCallback(callback)
    {
    }

    StaticPropertyChangedCallback staticPropertyChangedCallback() const { return m_staticPropertyChangedCallback; }

private:

    StaticPropertyChangedCallback m_staticPropertyChangedCallback;
};

class PropertyInfo
    : public RefObject
{
public:
    PropertyInfo(TypeInfo* ownerClassType, GetPropertyCallback getPropertyCallback, const PropertyMetadata& metadata)
        : m_getPropertyCallback(getPropertyCallback)
        , m_staticPropertyChangedCallback(metadata.staticPropertyChangedCallback())
        , m_registerd(false)
    {
        ownerClassType->registerProperty(this);
    }

    template<typename TValue>
    static void setTypedValue(Object* obj, PropertyInfo* propertyInfo, TValue&& value)
    {
        PropertyBase* prop = propertyInfo->m_getPropertyCallback();
        static_cast<Property<TValue>*>(prop)->set(std::forward(value));
    }
    
    static PropertyRef getPropertyRef(Object* obj, PropertyInfo* propertyInfo);

    // TODO: Helper でいい
    static void notifyPropertyChanged(Object* ownerObject, PropertyBase* target, const PropertyInfo* prop, PropertySetSource source);

private:
    GetPropertyCallback m_getPropertyCallback;
    StaticPropertyChangedCallback m_staticPropertyChangedCallback;
    bool m_registerd;
    friend class TypeInfo;
};

//namespace detail {
//
//class PropertyInitializer
//{
//public:
//    PropertyInitializer(const PropertyMetadata& metadata)
//    {
//    }
//};
//
//} // namespace detail

class PropertyBase
{
public:
    virtual void setValue(const Variant& value) = 0;
    virtual Variant getValue() const = 0;
    virtual void clearValue() = 0;

protected:
    PropertyBase()
        : m_owner(nullptr)
        , m_propertyInfo(nullptr)
    {
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
    : public PropertyBase
{
public:
    Property()
        : PropertyBase()
        , m_defaultValue()
        , m_value()
        , m_valueSource(PropertySetSource::Default)
    {
    }

    Property(const TValue& value)
        : PropertyBase(/*owner*/)
        , m_defaultValue(value)
        , m_value(value)
        , m_valueSource(PropertySetSource::Default)
    {}

    ~Property()
    {}

    virtual void setValue(const Variant& value) override
    {
        set(value.get<TValue>());
    }

    virtual Variant getValue() const override
    {
        return Variant(get());
    }

    /** プロパティのローカル値を設定します。*/
    void set(const TValue& value)
    {
        setInternal(value, PropertySetSource::ByLocal);
    }

    /** プロパティのローカル値を取得します。*/
    const TValue& get() const { return m_value; }

    /** プロパティのローカル値を消去します。*/
    virtual void clearValue() override
    {
        if (m_value != m_defaultValue || m_valueSource != PropertySetSource::Default)
        {
            m_value = m_defaultValue;
            m_valueSource = PropertySetSource::Default;
            PropertyInfo::notifyPropertyChanged(ownerObject(), this, propertyInfo(), m_valueSource);
        }
    }

    Property& operator = (const TValue& value) { set(value); return *this; }
    operator const TValue&() const { return m_value; }

    //const PropertyInfo* getPropertyInfo() const { return m_propId; }

LN_INTERNAL_ACCESS:
    //uint32_t getHashCode() const
    //{
    //    return
    //        Hash::calcHash(reinterpret_cast<const char*>(&m_value), sizeof(m_value)) +
    //        Hash::calcHash(reinterpret_cast<const char*>(&m_valueSource), sizeof(m_valueSource));
    //}

    bool inherit(const Property& parent)
    {
        if (m_valueSource <= PropertySetSource::ByInherit)
        {
            bool changed = false;
            if (m_value != parent.m_value)
            {
                m_value = parent.m_value;
                changed = true;
            }
            m_valueSource = PropertySetSource::ByInherit;
            return changed;
        }
        return false;
    }

LN_INTERNAL_ACCESS:
    void setValueSource(PropertySetSource source)
    {
        m_valueSource = source;
    }

private:
    LN_DISALLOW_COPY_AND_ASSIGN(Property);

    void setInternal(const TValue& value, PropertySetSource source)
    {
        if (m_value != value || m_valueSource != source)
        {
            m_value = value;
            m_valueSource = source;
            PropertyInfo::notifyPropertyChanged(ownerObject(), this, propertyInfo(), m_valueSource);
        }
    }

    TValue				m_defaultValue;
    TValue				m_value;
    PropertySetSource	m_valueSource;

    friend class PropertyHelper;
    friend class PropertyInfo;
};

template<typename TValue>
inline bool operator == (const Property<TValue>& lhs, const TValue& rhs)
{
    return lhs.get() == rhs;
}
template<typename TValue>
inline bool operator == (const TValue& lhs, const Property<TValue>& rhs)
{
    return lhs == rhs.get();
}
template<typename TValue>
inline bool operator != (const Property<TValue>& lhs, const TValue& rhs)
{
    return lhs.get() != rhs;
}
template<typename TValue>
inline bool operator != (const TValue& lhs, const Property<TValue>& rhs)
{
    return lhs != rhs.get();
}




} // namespace ln

