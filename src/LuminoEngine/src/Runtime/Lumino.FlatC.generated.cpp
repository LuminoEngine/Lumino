#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>

#include <LuminoEngine.hpp>
#include "BindingValidation.hpp"

class LNWS_ln_Object : public ln::Object
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnObject_OverridePrototypes
    {
        LnObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnObject_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LnObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnObject_SubclassRegistrationInfo* subclassInfo() { static LnObject_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Object()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Object()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnObject_OnSerialize_OverrideCallback s_LnObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnObject_OnSerialize_OverrideCallback) s_LnObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Object::onSerialize(ar);
    }

    static LnObject_OnSerialize2_OverrideCallback s_LnObject_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnObject_OnSerialize2_OverrideCallback) s_LnObject_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Object::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnObject_OnSerialize_OverrideCallback LNWS_ln_Object::s_LnObject_OnSerialize_OverrideCallback = nullptr;
LnObject_OnSerialize2_OverrideCallback LNWS_ln_Object::s_LnObject_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_EventConnection : public ln::EventConnection
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnEventConnection_OverridePrototypes
    {
        LnEventConnection_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnEventConnection_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnEventConnection_OverridePrototypes> m_overridePrototypes = nullptr;
    LnEventConnection_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnEventConnection_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnEventConnection_SubclassRegistrationInfo* subclassInfo() { static LnEventConnection_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_EventConnection()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_EventConnection()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnEventConnection_OnSerialize_OverrideCallback s_LnEventConnection_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnEventConnection_OnSerialize_OverrideCallback) s_LnEventConnection_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::EventConnection::onSerialize(ar);
    }

    static LnEventConnection_OnSerialize2_OverrideCallback s_LnEventConnection_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnEventConnection_OnSerialize2_OverrideCallback) s_LnEventConnection_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::EventConnection::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnEventConnection_OnSerialize_OverrideCallback LNWS_ln_EventConnection::s_LnEventConnection_OnSerialize_OverrideCallback = nullptr;
LnEventConnection_OnSerialize2_OverrideCallback LNWS_ln_EventConnection::s_LnEventConnection_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_PromiseFailureDelegate : public ln::PromiseFailureDelegate
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnPromiseFailureDelegate_OverridePrototypes
    {

    };
    std::unique_ptr<LnPromiseFailureDelegate_OverridePrototypes> m_overridePrototypes = nullptr;
    LnPromiseFailureDelegate_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnPromiseFailureDelegate_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnPromiseFailureDelegate_SubclassRegistrationInfo* subclassInfo() { static LnPromiseFailureDelegate_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnPromiseFailureDelegateCallback m_callback;

    LNWS_ln_PromiseFailureDelegate()
      : ln::PromiseFailureDelegate([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_SUCCESS) { LN_ERROR("LnPromiseFailureDelegateCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_PromiseFailureDelegate()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnPromiseFailureDelegateCallback callback)
    {
        if (!ln::PromiseFailureDelegate::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnPromiseFailureDelegate_Create(LnPromiseFailureDelegateCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_PromiseFailureDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_ZVTestDelegate1 : public ln::ZVTestDelegate1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestDelegate1_OverridePrototypes
    {

    };
    std::unique_ptr<LnZVTestDelegate1_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestDelegate1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestDelegate1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestDelegate1_SubclassRegistrationInfo* subclassInfo() { static LnZVTestDelegate1_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnZVTestDelegate1Callback m_callback;

    LNWS_ln_ZVTestDelegate1()
      : ln::ZVTestDelegate1([this](int p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1);
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestDelegate1Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestDelegate1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnZVTestDelegate1Callback callback)
    {
        if (!ln::ZVTestDelegate1::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnZVTestDelegate1_Create(LnZVTestDelegate1Callback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate1, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_ZVTestDelegate2 : public ln::ZVTestDelegate2
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestDelegate2_OverridePrototypes
    {

    };
    std::unique_ptr<LnZVTestDelegate2_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestDelegate2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestDelegate2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestDelegate2_SubclassRegistrationInfo* subclassInfo() { static LnZVTestDelegate2_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnZVTestDelegate2Callback m_callback;

    LNWS_ln_ZVTestDelegate2()
      : ln::ZVTestDelegate2([this](int p1, int p2) -> int
    {
        int ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1, p2, &ret);
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestDelegate2Callback"); }
        return ret;
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestDelegate2()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnZVTestDelegate2Callback callback)
    {
        if (!ln::ZVTestDelegate2::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnZVTestDelegate2_Create(LnZVTestDelegate2Callback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate2, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_ZVTestDelegate3 : public ln::ZVTestDelegate3
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestDelegate3_OverridePrototypes
    {

    };
    std::unique_ptr<LnZVTestDelegate3_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestDelegate3_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestDelegate3_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestDelegate3_SubclassRegistrationInfo* subclassInfo() { static LnZVTestDelegate3_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnZVTestDelegate3Callback m_callback;

    LNWS_ln_ZVTestDelegate3()
      : ln::ZVTestDelegate3([this](ln::ZVTestClass1* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(p1));
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestDelegate3Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestDelegate3()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnZVTestDelegate3Callback callback)
    {
        if (!ln::ZVTestDelegate3::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnZVTestDelegate3_Create(LnZVTestDelegate3Callback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate3, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_ZVTestEventHandler1 : public ln::ZVTestEventHandler1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestEventHandler1_OverridePrototypes
    {

    };
    std::unique_ptr<LnZVTestEventHandler1_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestEventHandler1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestEventHandler1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestEventHandler1_SubclassRegistrationInfo* subclassInfo() { static LnZVTestEventHandler1_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnZVTestEventHandler1Callback m_callback;

    LNWS_ln_ZVTestEventHandler1()
      : ln::ZVTestEventHandler1([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestEventHandler1Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestEventHandler1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnZVTestEventHandler1Callback callback)
    {
        if (!ln::ZVTestEventHandler1::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnZVTestEventHandler1_Create(LnZVTestEventHandler1Callback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestEventHandler1, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_ZVTestEventHandler2 : public ln::ZVTestEventHandler2
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestEventHandler2_OverridePrototypes
    {

    };
    std::unique_ptr<LnZVTestEventHandler2_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestEventHandler2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestEventHandler2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestEventHandler2_SubclassRegistrationInfo* subclassInfo() { static LnZVTestEventHandler2_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnZVTestEventHandler2Callback m_callback;

    LNWS_ln_ZVTestEventHandler2()
      : ln::ZVTestEventHandler2([this](ln::ZVTestEventArgs1* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1));
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestEventHandler2Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestEventHandler2()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnZVTestEventHandler2Callback callback)
    {
        if (!ln::ZVTestEventHandler2::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnZVTestEventHandler2_Create(LnZVTestEventHandler2Callback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestEventHandler2, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_ZVTestPromise1 : public ln::ZVTestPromise1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestPromise1_OverridePrototypes
    {

    };
    std::unique_ptr<LnZVTestPromise1_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestPromise1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestPromise1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestPromise1_SubclassRegistrationInfo* subclassInfo() { static LnZVTestPromise1_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_ZVTestPromise1()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestPromise1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


class LNWS_ln_ZVTestPromise2 : public ln::ZVTestPromise2
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestPromise2_OverridePrototypes
    {

    };
    std::unique_ptr<LnZVTestPromise2_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestPromise2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestPromise2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestPromise2_SubclassRegistrationInfo* subclassInfo() { static LnZVTestPromise2_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_ZVTestPromise2()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestPromise2()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


class LNWS_ln_ZVTestClass1 : public ln::ZVTestClass1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestClass1_OverridePrototypes
    {
        LnZVTestClass1_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnZVTestClass1_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnZVTestClass1_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestClass1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestClass1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestClass1_SubclassRegistrationInfo* subclassInfo() { static LnZVTestClass1_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_ZVTestClass1()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestClass1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnZVTestClass1_OnSerialize_OverrideCallback s_LnZVTestClass1_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnZVTestClass1_OnSerialize_OverrideCallback) s_LnZVTestClass1_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::ZVTestClass1::onSerialize(ar);
    }

    static LnZVTestClass1_OnSerialize2_OverrideCallback s_LnZVTestClass1_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnZVTestClass1_OnSerialize2_OverrideCallback) s_LnZVTestClass1_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::ZVTestClass1::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnZVTestClass1_OnSerialize_OverrideCallback LNWS_ln_ZVTestClass1::s_LnZVTestClass1_OnSerialize_OverrideCallback = nullptr;
LnZVTestClass1_OnSerialize2_OverrideCallback LNWS_ln_ZVTestClass1::s_LnZVTestClass1_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_ZVTestEventArgs1 : public ln::ZVTestEventArgs1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnZVTestEventArgs1_OverridePrototypes
    {
        LnZVTestEventArgs1_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnZVTestEventArgs1_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnZVTestEventArgs1_OverridePrototypes> m_overridePrototypes = nullptr;
    LnZVTestEventArgs1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnZVTestEventArgs1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnZVTestEventArgs1_SubclassRegistrationInfo* subclassInfo() { static LnZVTestEventArgs1_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_ZVTestEventArgs1()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestEventArgs1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnZVTestEventArgs1_OnSerialize_OverrideCallback s_LnZVTestEventArgs1_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnZVTestEventArgs1_OnSerialize_OverrideCallback) s_LnZVTestEventArgs1_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::ZVTestEventArgs1::onSerialize(ar);
    }

    static LnZVTestEventArgs1_OnSerialize2_OverrideCallback s_LnZVTestEventArgs1_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnZVTestEventArgs1_OnSerialize2_OverrideCallback) s_LnZVTestEventArgs1_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::ZVTestEventArgs1::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnZVTestEventArgs1_OnSerialize_OverrideCallback LNWS_ln_ZVTestEventArgs1::s_LnZVTestEventArgs1_OnSerialize_OverrideCallback = nullptr;
LnZVTestEventArgs1_OnSerialize2_OverrideCallback LNWS_ln_ZVTestEventArgs1::s_LnZVTestEventArgs1_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_Serializer : public ln::Serializer
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnSerializer_OverridePrototypes
    {
        LnSerializer_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnSerializer_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnSerializer_OverridePrototypes> m_overridePrototypes = nullptr;
    LnSerializer_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnSerializer_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnSerializer_SubclassRegistrationInfo* subclassInfo() { static LnSerializer_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Serializer()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Serializer()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnSerializer_OnSerialize_OverrideCallback s_LnSerializer_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSerializer_OnSerialize_OverrideCallback) s_LnSerializer_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Serializer::onSerialize(ar);
    }

    static LnSerializer_OnSerialize2_OverrideCallback s_LnSerializer_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSerializer_OnSerialize2_OverrideCallback) s_LnSerializer_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Serializer::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnSerializer_OnSerialize_OverrideCallback LNWS_ln_Serializer::s_LnSerializer_OnSerialize_OverrideCallback = nullptr;
LnSerializer_OnSerialize2_OverrideCallback LNWS_ln_Serializer::s_LnSerializer_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_Serializer2 : public ln::Serializer2
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnSerializer2_OverridePrototypes
    {
        LnSerializer2_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnSerializer2_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnSerializer2_OverridePrototypes> m_overridePrototypes = nullptr;
    LnSerializer2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnSerializer2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnSerializer2_SubclassRegistrationInfo* subclassInfo() { static LnSerializer2_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Serializer2()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Serializer2()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnSerializer2_OnSerialize_OverrideCallback s_LnSerializer2_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSerializer2_OnSerialize_OverrideCallback) s_LnSerializer2_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Serializer2::onSerialize(ar);
    }

    static LnSerializer2_OnSerialize2_OverrideCallback s_LnSerializer2_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSerializer2_OnSerialize2_OverrideCallback) s_LnSerializer2_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Serializer2::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnSerializer2_OnSerialize_OverrideCallback LNWS_ln_Serializer2::s_LnSerializer2_OnSerialize_OverrideCallback = nullptr;
LnSerializer2_OnSerialize2_OverrideCallback LNWS_ln_Serializer2::s_LnSerializer2_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_AssetModel : public ln::AssetModel
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnAssetModel_OverridePrototypes
    {
        LnAssetModel_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnAssetModel_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnAssetModel_OverridePrototypes> m_overridePrototypes = nullptr;
    LnAssetModel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnAssetModel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnAssetModel_SubclassRegistrationInfo* subclassInfo() { static LnAssetModel_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_AssetModel()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AssetModel()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnAssetModel_OnSerialize_OverrideCallback s_LnAssetModel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnAssetModel_OnSerialize_OverrideCallback) s_LnAssetModel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::AssetModel::onSerialize(ar);
    }

    static LnAssetModel_OnSerialize2_OverrideCallback s_LnAssetModel_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnAssetModel_OnSerialize2_OverrideCallback) s_LnAssetModel_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::AssetModel::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnAssetModel_OnSerialize_OverrideCallback LNWS_ln_AssetModel::s_LnAssetModel_OnSerialize_OverrideCallback = nullptr;
LnAssetModel_OnSerialize2_OverrideCallback LNWS_ln_AssetModel::s_LnAssetModel_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_Assets : public ln::Assets
{
public:
};


class LNWS_ln_Texture2DDelegate : public ln::Texture2DDelegate
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnTexture2DDelegate_OverridePrototypes
    {

    };
    std::unique_ptr<LnTexture2DDelegate_OverridePrototypes> m_overridePrototypes = nullptr;
    LnTexture2DDelegate_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnTexture2DDelegate_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnTexture2DDelegate_SubclassRegistrationInfo* subclassInfo() { static LnTexture2DDelegate_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnTexture2DDelegateCallback m_callback;

    LNWS_ln_Texture2DDelegate()
      : ln::Texture2DDelegate([this](ln::Texture2D* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(p1));
        if (r != LN_SUCCESS) { LN_ERROR("LnTexture2DDelegateCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Texture2DDelegate()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnTexture2DDelegateCallback callback)
    {
        if (!ln::Texture2DDelegate::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnTexture2DDelegate_Create(LnTexture2DDelegateCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_Texture2DDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_Texture2DPromise : public ln::Texture2DPromise
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnTexture2DPromise_OverridePrototypes
    {

    };
    std::unique_ptr<LnTexture2DPromise_OverridePrototypes> m_overridePrototypes = nullptr;
    LnTexture2DPromise_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnTexture2DPromise_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnTexture2DPromise_SubclassRegistrationInfo* subclassInfo() { static LnTexture2DPromise_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Texture2DPromise()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Texture2DPromise()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


class LNWS_ln_Texture : public ln::Texture
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnTexture_OverridePrototypes
    {
        LnTexture_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnTexture_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnTexture_OverridePrototypes> m_overridePrototypes = nullptr;
    LnTexture_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnTexture_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnTexture_SubclassRegistrationInfo* subclassInfo() { static LnTexture_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Texture()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Texture()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnTexture_OnSerialize_OverrideCallback s_LnTexture_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnTexture_OnSerialize_OverrideCallback) s_LnTexture_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Texture::onSerialize(ar);
    }

    static LnTexture_OnSerialize2_OverrideCallback s_LnTexture_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnTexture_OnSerialize2_OverrideCallback) s_LnTexture_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Texture::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnTexture_OnSerialize_OverrideCallback LNWS_ln_Texture::s_LnTexture_OnSerialize_OverrideCallback = nullptr;
LnTexture_OnSerialize2_OverrideCallback LNWS_ln_Texture::s_LnTexture_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_Texture2D : public ln::Texture2D
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnTexture2D_OverridePrototypes
    {
        LnTexture2D_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnTexture2D_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnTexture2D_OverridePrototypes> m_overridePrototypes = nullptr;
    LnTexture2D_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnTexture2D_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnTexture2D_SubclassRegistrationInfo* subclassInfo() { static LnTexture2D_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Texture2D()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Texture2D()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnTexture2D_OnSerialize_OverrideCallback s_LnTexture2D_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnTexture2D_OnSerialize_OverrideCallback) s_LnTexture2D_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Texture2D::onSerialize(ar);
    }

    static LnTexture2D_OnSerialize2_OverrideCallback s_LnTexture2D_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnTexture2D_OnSerialize2_OverrideCallback) s_LnTexture2D_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Texture2D::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Texture>();
    }

};

LnTexture2D_OnSerialize_OverrideCallback LNWS_ln_Texture2D::s_LnTexture2D_OnSerialize_OverrideCallback = nullptr;
LnTexture2D_OnSerialize2_OverrideCallback LNWS_ln_Texture2D::s_LnTexture2D_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_RenderView : public ln::RenderView
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnRenderView_OverridePrototypes
    {
        LnRenderView_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnRenderView_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnRenderView_OverridePrototypes> m_overridePrototypes = nullptr;
    LnRenderView_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnRenderView_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnRenderView_SubclassRegistrationInfo* subclassInfo() { static LnRenderView_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_RenderView()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_RenderView()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnRenderView_OnSerialize_OverrideCallback s_LnRenderView_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnRenderView_OnSerialize_OverrideCallback) s_LnRenderView_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::RenderView::onSerialize(ar);
    }

    static LnRenderView_OnSerialize2_OverrideCallback s_LnRenderView_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnRenderView_OnSerialize2_OverrideCallback) s_LnRenderView_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::RenderView::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnRenderView_OnSerialize_OverrideCallback LNWS_ln_RenderView::s_LnRenderView_OnSerialize_OverrideCallback = nullptr;
LnRenderView_OnSerialize2_OverrideCallback LNWS_ln_RenderView::s_LnRenderView_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_Component : public ln::Component
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnComponent_OverridePrototypes
    {
        LnComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnComponent_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LnComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnComponent_SubclassRegistrationInfo* subclassInfo() { static LnComponent_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Component()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Component()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnComponent_OnSerialize_OverrideCallback s_LnComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnComponent_OnSerialize_OverrideCallback) s_LnComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Component::onSerialize(ar);
    }

    static LnComponent_OnSerialize2_OverrideCallback s_LnComponent_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnComponent_OnSerialize2_OverrideCallback) s_LnComponent_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Component::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnComponent_OnSerialize_OverrideCallback LNWS_ln_Component::s_LnComponent_OnSerialize_OverrideCallback = nullptr;
LnComponent_OnSerialize2_OverrideCallback LNWS_ln_Component::s_LnComponent_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_VisualComponent : public ln::VisualComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnVisualComponent_OverridePrototypes
    {
        LnVisualComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnVisualComponent_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnVisualComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LnVisualComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnVisualComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnVisualComponent_SubclassRegistrationInfo* subclassInfo() { static LnVisualComponent_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_VisualComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_VisualComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnVisualComponent_OnSerialize_OverrideCallback s_LnVisualComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnVisualComponent_OnSerialize_OverrideCallback) s_LnVisualComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::VisualComponent::onSerialize(ar);
    }

    static LnVisualComponent_OnSerialize2_OverrideCallback s_LnVisualComponent_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnVisualComponent_OnSerialize2_OverrideCallback) s_LnVisualComponent_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::VisualComponent::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Component>();
    }

};

LnVisualComponent_OnSerialize_OverrideCallback LNWS_ln_VisualComponent::s_LnVisualComponent_OnSerialize_OverrideCallback = nullptr;
LnVisualComponent_OnSerialize2_OverrideCallback LNWS_ln_VisualComponent::s_LnVisualComponent_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_SpriteComponent : public ln::SpriteComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnSpriteComponent_OverridePrototypes
    {
        LnSpriteComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnSpriteComponent_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnSpriteComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LnSpriteComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnSpriteComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnSpriteComponent_SubclassRegistrationInfo* subclassInfo() { static LnSpriteComponent_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_SpriteComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_SpriteComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnSpriteComponent_OnSerialize_OverrideCallback s_LnSpriteComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSpriteComponent_OnSerialize_OverrideCallback) s_LnSpriteComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::SpriteComponent::onSerialize(ar);
    }

    static LnSpriteComponent_OnSerialize2_OverrideCallback s_LnSpriteComponent_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSpriteComponent_OnSerialize2_OverrideCallback) s_LnSpriteComponent_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::SpriteComponent::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<VisualComponent>();
    }

};

LnSpriteComponent_OnSerialize_OverrideCallback LNWS_ln_SpriteComponent::s_LnSpriteComponent_OnSerialize_OverrideCallback = nullptr;
LnSpriteComponent_OnSerialize2_OverrideCallback LNWS_ln_SpriteComponent::s_LnSpriteComponent_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_World : public ln::World
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnWorld_OverridePrototypes
    {
        LnWorld_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnWorld_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnWorld_OverridePrototypes> m_overridePrototypes = nullptr;
    LnWorld_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnWorld_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnWorld_SubclassRegistrationInfo* subclassInfo() { static LnWorld_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_World()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_World()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnWorld_OnSerialize_OverrideCallback s_LnWorld_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnWorld_OnSerialize_OverrideCallback) s_LnWorld_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::World::onSerialize(ar);
    }

    static LnWorld_OnSerialize2_OverrideCallback s_LnWorld_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnWorld_OnSerialize2_OverrideCallback) s_LnWorld_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::World::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnWorld_OnSerialize_OverrideCallback LNWS_ln_World::s_LnWorld_OnSerialize_OverrideCallback = nullptr;
LnWorld_OnSerialize2_OverrideCallback LNWS_ln_World::s_LnWorld_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_ComponentList : public ln::ComponentList
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnComponentList_OverridePrototypes
    {
        LnComponentList_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnComponentList_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnComponentList_OverridePrototypes> m_overridePrototypes = nullptr;
    LnComponentList_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnComponentList_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnComponentList_SubclassRegistrationInfo* subclassInfo() { static LnComponentList_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_ComponentList()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ComponentList()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnComponentList_OnSerialize_OverrideCallback s_LnComponentList_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnComponentList_OnSerialize_OverrideCallback) s_LnComponentList_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::ComponentList::onSerialize(ar);
    }

    static LnComponentList_OnSerialize2_OverrideCallback s_LnComponentList_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnComponentList_OnSerialize2_OverrideCallback) s_LnComponentList_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::ComponentList::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnComponentList_OnSerialize_OverrideCallback LNWS_ln_ComponentList::s_LnComponentList_OnSerialize_OverrideCallback = nullptr;
LnComponentList_OnSerialize2_OverrideCallback LNWS_ln_ComponentList::s_LnComponentList_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_WorldObject : public ln::WorldObject
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnWorldObject_OverridePrototypes
    {
        LnWorldObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnWorldObject_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnWorldObject_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnWorldObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LnWorldObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnWorldObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnWorldObject_SubclassRegistrationInfo* subclassInfo() { static LnWorldObject_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_WorldObject()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_WorldObject()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnWorldObject_OnSerialize_OverrideCallback s_LnWorldObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnWorldObject_OnSerialize_OverrideCallback) s_LnWorldObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::WorldObject::onSerialize(ar);
    }

    static LnWorldObject_OnSerialize2_OverrideCallback s_LnWorldObject_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnWorldObject_OnSerialize2_OverrideCallback) s_LnWorldObject_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::WorldObject::onSerialize2(ar);
    }

    static LnWorldObject_OnUpdate_OverrideCallback s_LnWorldObject_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnWorldObject_OnUpdate_OverrideCallback) s_LnWorldObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::WorldObject::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnWorldObject_OnSerialize_OverrideCallback LNWS_ln_WorldObject::s_LnWorldObject_OnSerialize_OverrideCallback = nullptr;
LnWorldObject_OnSerialize2_OverrideCallback LNWS_ln_WorldObject::s_LnWorldObject_OnSerialize2_OverrideCallback = nullptr;
LnWorldObject_OnUpdate_OverrideCallback LNWS_ln_WorldObject::s_LnWorldObject_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_VisualObject : public ln::VisualObject
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnVisualObject_OverridePrototypes
    {
        LnVisualObject_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnVisualObject_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnVisualObject_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnVisualObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LnVisualObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnVisualObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnVisualObject_SubclassRegistrationInfo* subclassInfo() { static LnVisualObject_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_VisualObject()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_VisualObject()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnVisualObject_OnSerialize_OverrideCallback s_LnVisualObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnVisualObject_OnSerialize_OverrideCallback) s_LnVisualObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::VisualObject::onSerialize(ar);
    }

    static LnVisualObject_OnSerialize2_OverrideCallback s_LnVisualObject_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnVisualObject_OnSerialize2_OverrideCallback) s_LnVisualObject_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::VisualObject::onSerialize2(ar);
    }

    static LnVisualObject_OnUpdate_OverrideCallback s_LnVisualObject_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnVisualObject_OnUpdate_OverrideCallback) s_LnVisualObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::VisualObject::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<WorldObject>();
    }

};

LnVisualObject_OnSerialize_OverrideCallback LNWS_ln_VisualObject::s_LnVisualObject_OnSerialize_OverrideCallback = nullptr;
LnVisualObject_OnSerialize2_OverrideCallback LNWS_ln_VisualObject::s_LnVisualObject_OnSerialize2_OverrideCallback = nullptr;
LnVisualObject_OnUpdate_OverrideCallback LNWS_ln_VisualObject::s_LnVisualObject_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_Camera : public ln::Camera
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnCamera_OverridePrototypes
    {
        LnCamera_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnCamera_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnCamera_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnCamera_OverridePrototypes> m_overridePrototypes = nullptr;
    LnCamera_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnCamera_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnCamera_SubclassRegistrationInfo* subclassInfo() { static LnCamera_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Camera()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Camera()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnCamera_OnSerialize_OverrideCallback s_LnCamera_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnCamera_OnSerialize_OverrideCallback) s_LnCamera_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Camera::onSerialize(ar);
    }

    static LnCamera_OnSerialize2_OverrideCallback s_LnCamera_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnCamera_OnSerialize2_OverrideCallback) s_LnCamera_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Camera::onSerialize2(ar);
    }

    static LnCamera_OnUpdate_OverrideCallback s_LnCamera_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnCamera_OnUpdate_OverrideCallback) s_LnCamera_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::Camera::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<WorldObject>();
    }

};

LnCamera_OnSerialize_OverrideCallback LNWS_ln_Camera::s_LnCamera_OnSerialize_OverrideCallback = nullptr;
LnCamera_OnSerialize2_OverrideCallback LNWS_ln_Camera::s_LnCamera_OnSerialize2_OverrideCallback = nullptr;
LnCamera_OnUpdate_OverrideCallback LNWS_ln_Camera::s_LnCamera_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_DirectionalLight : public ln::DirectionalLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnDirectionalLight_OverridePrototypes
    {
        LnDirectionalLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnDirectionalLight_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnDirectionalLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnDirectionalLight_OverridePrototypes> m_overridePrototypes = nullptr;
    LnDirectionalLight_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnDirectionalLight_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnDirectionalLight_SubclassRegistrationInfo* subclassInfo() { static LnDirectionalLight_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_DirectionalLight()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_DirectionalLight()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnDirectionalLight_OnSerialize_OverrideCallback s_LnDirectionalLight_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnDirectionalLight_OnSerialize_OverrideCallback) s_LnDirectionalLight_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::DirectionalLight::onSerialize(ar);
    }

    static LnDirectionalLight_OnSerialize2_OverrideCallback s_LnDirectionalLight_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnDirectionalLight_OnSerialize2_OverrideCallback) s_LnDirectionalLight_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::DirectionalLight::onSerialize2(ar);
    }

    static LnDirectionalLight_OnUpdate_OverrideCallback s_LnDirectionalLight_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnDirectionalLight_OnUpdate_OverrideCallback) s_LnDirectionalLight_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::DirectionalLight::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<WorldObject>();
    }

};

LnDirectionalLight_OnSerialize_OverrideCallback LNWS_ln_DirectionalLight::s_LnDirectionalLight_OnSerialize_OverrideCallback = nullptr;
LnDirectionalLight_OnSerialize2_OverrideCallback LNWS_ln_DirectionalLight::s_LnDirectionalLight_OnSerialize2_OverrideCallback = nullptr;
LnDirectionalLight_OnUpdate_OverrideCallback LNWS_ln_DirectionalLight::s_LnDirectionalLight_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_PointLight : public ln::PointLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnPointLight_OverridePrototypes
    {
        LnPointLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnPointLight_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnPointLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnPointLight_OverridePrototypes> m_overridePrototypes = nullptr;
    LnPointLight_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnPointLight_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnPointLight_SubclassRegistrationInfo* subclassInfo() { static LnPointLight_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_PointLight()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_PointLight()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnPointLight_OnSerialize_OverrideCallback s_LnPointLight_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnPointLight_OnSerialize_OverrideCallback) s_LnPointLight_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::PointLight::onSerialize(ar);
    }

    static LnPointLight_OnSerialize2_OverrideCallback s_LnPointLight_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnPointLight_OnSerialize2_OverrideCallback) s_LnPointLight_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::PointLight::onSerialize2(ar);
    }

    static LnPointLight_OnUpdate_OverrideCallback s_LnPointLight_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnPointLight_OnUpdate_OverrideCallback) s_LnPointLight_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::PointLight::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<WorldObject>();
    }

};

LnPointLight_OnSerialize_OverrideCallback LNWS_ln_PointLight::s_LnPointLight_OnSerialize_OverrideCallback = nullptr;
LnPointLight_OnSerialize2_OverrideCallback LNWS_ln_PointLight::s_LnPointLight_OnSerialize2_OverrideCallback = nullptr;
LnPointLight_OnUpdate_OverrideCallback LNWS_ln_PointLight::s_LnPointLight_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_SpotLight : public ln::SpotLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnSpotLight_OverridePrototypes
    {
        LnSpotLight_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnSpotLight_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnSpotLight_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnSpotLight_OverridePrototypes> m_overridePrototypes = nullptr;
    LnSpotLight_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnSpotLight_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnSpotLight_SubclassRegistrationInfo* subclassInfo() { static LnSpotLight_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_SpotLight()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_SpotLight()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnSpotLight_OnSerialize_OverrideCallback s_LnSpotLight_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSpotLight_OnSerialize_OverrideCallback) s_LnSpotLight_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::SpotLight::onSerialize(ar);
    }

    static LnSpotLight_OnSerialize2_OverrideCallback s_LnSpotLight_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSpotLight_OnSerialize2_OverrideCallback) s_LnSpotLight_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::SpotLight::onSerialize2(ar);
    }

    static LnSpotLight_OnUpdate_OverrideCallback s_LnSpotLight_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnSpotLight_OnUpdate_OverrideCallback) s_LnSpotLight_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::SpotLight::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<WorldObject>();
    }

};

LnSpotLight_OnSerialize_OverrideCallback LNWS_ln_SpotLight::s_LnSpotLight_OnSerialize_OverrideCallback = nullptr;
LnSpotLight_OnSerialize2_OverrideCallback LNWS_ln_SpotLight::s_LnSpotLight_OnSerialize2_OverrideCallback = nullptr;
LnSpotLight_OnUpdate_OverrideCallback LNWS_ln_SpotLight::s_LnSpotLight_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_TestDelegate : public ln::TestDelegate
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnTestDelegate_OverridePrototypes
    {

    };
    std::unique_ptr<LnTestDelegate_OverridePrototypes> m_overridePrototypes = nullptr;
    LnTestDelegate_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnTestDelegate_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnTestDelegate_SubclassRegistrationInfo* subclassInfo() { static LnTestDelegate_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnTestDelegateCallback m_callback;

    LNWS_ln_TestDelegate()
      : ln::TestDelegate([this](int p1) -> int
    {
        int ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1, &ret);
        if (r != LN_SUCCESS) { LN_ERROR("LnTestDelegateCallback"); }
        return ret;
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_TestDelegate()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnTestDelegateCallback callback)
    {
        if (!ln::TestDelegate::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnTestDelegate_Create(LnTestDelegateCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_TestDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_Sprite : public ln::Sprite
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnSprite_OverridePrototypes
    {
        LnSprite_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnSprite_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnSprite_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnSprite_OverridePrototypes> m_overridePrototypes = nullptr;
    LnSprite_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnSprite_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnSprite_SubclassRegistrationInfo* subclassInfo() { static LnSprite_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Sprite()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Sprite()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnSprite_OnSerialize_OverrideCallback s_LnSprite_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSprite_OnSerialize_OverrideCallback) s_LnSprite_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Sprite::onSerialize(ar);
    }

    static LnSprite_OnSerialize2_OverrideCallback s_LnSprite_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnSprite_OnSerialize2_OverrideCallback) s_LnSprite_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Sprite::onSerialize2(ar);
    }

    static LnSprite_OnUpdate_OverrideCallback s_LnSprite_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnSprite_OnUpdate_OverrideCallback) s_LnSprite_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::Sprite::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<VisualObject>();
    }

};

LnSprite_OnSerialize_OverrideCallback LNWS_ln_Sprite::s_LnSprite_OnSerialize_OverrideCallback = nullptr;
LnSprite_OnSerialize2_OverrideCallback LNWS_ln_Sprite::s_LnSprite_OnSerialize2_OverrideCallback = nullptr;
LnSprite_OnUpdate_OverrideCallback LNWS_ln_Sprite::s_LnSprite_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_CameraOrbitControlComponent : public ln::CameraOrbitControlComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnCameraOrbitControlComponent_OverridePrototypes
    {
        LnCameraOrbitControlComponent_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnCameraOrbitControlComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LnCameraOrbitControlComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnCameraOrbitControlComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnCameraOrbitControlComponent_SubclassRegistrationInfo* subclassInfo() { static LnCameraOrbitControlComponent_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_CameraOrbitControlComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_CameraOrbitControlComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnCameraOrbitControlComponent_OnSerialize_OverrideCallback s_LnCameraOrbitControlComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnCameraOrbitControlComponent_OnSerialize_OverrideCallback) s_LnCameraOrbitControlComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::CameraOrbitControlComponent::onSerialize(ar);
    }

    static LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback s_LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback) s_LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::CameraOrbitControlComponent::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Component>();
    }

};

LnCameraOrbitControlComponent_OnSerialize_OverrideCallback LNWS_ln_CameraOrbitControlComponent::s_LnCameraOrbitControlComponent_OnSerialize_OverrideCallback = nullptr;
LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback LNWS_ln_CameraOrbitControlComponent::s_LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_Raycaster : public ln::Raycaster
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnRaycaster_OverridePrototypes
    {
        LnRaycaster_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnRaycaster_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnRaycaster_OverridePrototypes> m_overridePrototypes = nullptr;
    LnRaycaster_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnRaycaster_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnRaycaster_SubclassRegistrationInfo* subclassInfo() { static LnRaycaster_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Raycaster()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Raycaster()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnRaycaster_OnSerialize_OverrideCallback s_LnRaycaster_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnRaycaster_OnSerialize_OverrideCallback) s_LnRaycaster_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Raycaster::onSerialize(ar);
    }

    static LnRaycaster_OnSerialize2_OverrideCallback s_LnRaycaster_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnRaycaster_OnSerialize2_OverrideCallback) s_LnRaycaster_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Raycaster::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnRaycaster_OnSerialize_OverrideCallback LNWS_ln_Raycaster::s_LnRaycaster_OnSerialize_OverrideCallback = nullptr;
LnRaycaster_OnSerialize2_OverrideCallback LNWS_ln_Raycaster::s_LnRaycaster_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_RaycastResult : public ln::RaycastResult
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnRaycastResult_OverridePrototypes
    {
        LnRaycastResult_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnRaycastResult_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnRaycastResult_OverridePrototypes> m_overridePrototypes = nullptr;
    LnRaycastResult_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnRaycastResult_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnRaycastResult_SubclassRegistrationInfo* subclassInfo() { static LnRaycastResult_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_RaycastResult()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_RaycastResult()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnRaycastResult_OnSerialize_OverrideCallback s_LnRaycastResult_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnRaycastResult_OnSerialize_OverrideCallback) s_LnRaycastResult_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::RaycastResult::onSerialize(ar);
    }

    static LnRaycastResult_OnSerialize2_OverrideCallback s_LnRaycastResult_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnRaycastResult_OnSerialize2_OverrideCallback) s_LnRaycastResult_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::RaycastResult::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnRaycastResult_OnSerialize_OverrideCallback LNWS_ln_RaycastResult::s_LnRaycastResult_OnSerialize_OverrideCallback = nullptr;
LnRaycastResult_OnSerialize2_OverrideCallback LNWS_ln_RaycastResult::s_LnRaycastResult_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_WorldRenderView : public ln::WorldRenderView
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnWorldRenderView_OverridePrototypes
    {
        LnWorldRenderView_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnWorldRenderView_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnWorldRenderView_OverridePrototypes> m_overridePrototypes = nullptr;
    LnWorldRenderView_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnWorldRenderView_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnWorldRenderView_SubclassRegistrationInfo* subclassInfo() { static LnWorldRenderView_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_WorldRenderView()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_WorldRenderView()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnWorldRenderView_OnSerialize_OverrideCallback s_LnWorldRenderView_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnWorldRenderView_OnSerialize_OverrideCallback) s_LnWorldRenderView_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::WorldRenderView::onSerialize(ar);
    }

    static LnWorldRenderView_OnSerialize2_OverrideCallback s_LnWorldRenderView_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnWorldRenderView_OnSerialize2_OverrideCallback) s_LnWorldRenderView_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::WorldRenderView::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<RenderView>();
    }

};

LnWorldRenderView_OnSerialize_OverrideCallback LNWS_ln_WorldRenderView::s_LnWorldRenderView_OnSerialize_OverrideCallback = nullptr;
LnWorldRenderView_OnSerialize2_OverrideCallback LNWS_ln_WorldRenderView::s_LnWorldRenderView_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_BoxMesh : public ln::BoxMesh
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnBoxMesh_OverridePrototypes
    {
        LnBoxMesh_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnBoxMesh_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnBoxMesh_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnBoxMesh_OverridePrototypes> m_overridePrototypes = nullptr;
    LnBoxMesh_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnBoxMesh_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnBoxMesh_SubclassRegistrationInfo* subclassInfo() { static LnBoxMesh_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_BoxMesh()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_BoxMesh()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnBoxMesh_OnSerialize_OverrideCallback s_LnBoxMesh_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnBoxMesh_OnSerialize_OverrideCallback) s_LnBoxMesh_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::BoxMesh::onSerialize(ar);
    }

    static LnBoxMesh_OnSerialize2_OverrideCallback s_LnBoxMesh_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnBoxMesh_OnSerialize2_OverrideCallback) s_LnBoxMesh_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::BoxMesh::onSerialize2(ar);
    }

    static LnBoxMesh_OnUpdate_OverrideCallback s_LnBoxMesh_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnBoxMesh_OnUpdate_OverrideCallback) s_LnBoxMesh_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::BoxMesh::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<VisualObject>();
    }

};

LnBoxMesh_OnSerialize_OverrideCallback LNWS_ln_BoxMesh::s_LnBoxMesh_OnSerialize_OverrideCallback = nullptr;
LnBoxMesh_OnSerialize2_OverrideCallback LNWS_ln_BoxMesh::s_LnBoxMesh_OnSerialize2_OverrideCallback = nullptr;
LnBoxMesh_OnUpdate_OverrideCallback LNWS_ln_BoxMesh::s_LnBoxMesh_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_PlaneMesh : public ln::PlaneMesh
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnPlaneMesh_OverridePrototypes
    {
        LnPlaneMesh_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnPlaneMesh_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnPlaneMesh_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnPlaneMesh_OverridePrototypes> m_overridePrototypes = nullptr;
    LnPlaneMesh_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnPlaneMesh_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnPlaneMesh_SubclassRegistrationInfo* subclassInfo() { static LnPlaneMesh_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_PlaneMesh()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_PlaneMesh()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnPlaneMesh_OnSerialize_OverrideCallback s_LnPlaneMesh_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnPlaneMesh_OnSerialize_OverrideCallback) s_LnPlaneMesh_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::PlaneMesh::onSerialize(ar);
    }

    static LnPlaneMesh_OnSerialize2_OverrideCallback s_LnPlaneMesh_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnPlaneMesh_OnSerialize2_OverrideCallback) s_LnPlaneMesh_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::PlaneMesh::onSerialize2(ar);
    }

    static LnPlaneMesh_OnUpdate_OverrideCallback s_LnPlaneMesh_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
                return;
            }
        }
        if (s_LnPlaneMesh_OnUpdate_OverrideCallback) s_LnPlaneMesh_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::PlaneMesh::onUpdate(elapsedSeconds);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<VisualObject>();
    }

};

LnPlaneMesh_OnSerialize_OverrideCallback LNWS_ln_PlaneMesh::s_LnPlaneMesh_OnSerialize_OverrideCallback = nullptr;
LnPlaneMesh_OnSerialize2_OverrideCallback LNWS_ln_PlaneMesh::s_LnPlaneMesh_OnSerialize2_OverrideCallback = nullptr;
LnPlaneMesh_OnUpdate_OverrideCallback LNWS_ln_PlaneMesh::s_LnPlaneMesh_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_UIEventArgs : public ln::UIEventArgs
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnUIEventArgs_OverridePrototypes
    {
        LnUIEventArgs_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnUIEventArgs_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnUIEventArgs_OverridePrototypes> m_overridePrototypes = nullptr;
    LnUIEventArgs_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnUIEventArgs_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnUIEventArgs_SubclassRegistrationInfo* subclassInfo() { static LnUIEventArgs_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_UIEventArgs()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIEventArgs()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnUIEventArgs_OnSerialize_OverrideCallback s_LnUIEventArgs_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUIEventArgs_OnSerialize_OverrideCallback) s_LnUIEventArgs_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UIEventArgs::onSerialize(ar);
    }

    static LnUIEventArgs_OnSerialize2_OverrideCallback s_LnUIEventArgs_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUIEventArgs_OnSerialize2_OverrideCallback) s_LnUIEventArgs_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::UIEventArgs::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnUIEventArgs_OnSerialize_OverrideCallback LNWS_ln_UIEventArgs::s_LnUIEventArgs_OnSerialize_OverrideCallback = nullptr;
LnUIEventArgs_OnSerialize2_OverrideCallback LNWS_ln_UIEventArgs::s_LnUIEventArgs_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_UIGeneralEventHandler : public ln::UIGeneralEventHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnUIGeneralEventHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LnUIGeneralEventHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LnUIGeneralEventHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnUIGeneralEventHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnUIGeneralEventHandler_SubclassRegistrationInfo* subclassInfo() { static LnUIGeneralEventHandler_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnUIGeneralEventHandlerCallback m_callback;

    LNWS_ln_UIGeneralEventHandler()
      : ln::UIGeneralEventHandler([this](ln::UIEventArgs* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1));
        if (r != LN_SUCCESS) { LN_ERROR("LnUIGeneralEventHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIGeneralEventHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnUIGeneralEventHandlerCallback callback)
    {
        if (!ln::UIGeneralEventHandler::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnUIGeneralEventHandler_Create(LnUIGeneralEventHandlerCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_UIGeneralEventHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_UIEventHandler : public ln::UIEventHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnUIEventHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LnUIEventHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LnUIEventHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnUIEventHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnUIEventHandler_SubclassRegistrationInfo* subclassInfo() { static LnUIEventHandler_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LnUIEventHandlerCallback m_callback;

    LNWS_ln_UIEventHandler()
      : ln::UIEventHandler([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_SUCCESS) { LN_ERROR("LnUIEventHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIEventHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LnUIEventHandlerCallback callback)
    {
        if (!ln::UIEventHandler::init()) return false;
        m_callback = callback;
        return true;
    }

    // Overrides
    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};


LN_FLAT_API LnResult LnUIEventHandler_Create(LnUIEventHandlerCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_UIEventHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_UILayoutElement : public ln::UILayoutElement
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnUILayoutElement_OverridePrototypes
    {
        LnUILayoutElement_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnUILayoutElement_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnUILayoutElement_OverridePrototypes> m_overridePrototypes = nullptr;
    LnUILayoutElement_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnUILayoutElement_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnUILayoutElement_SubclassRegistrationInfo* subclassInfo() { static LnUILayoutElement_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_UILayoutElement()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UILayoutElement()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnUILayoutElement_OnSerialize_OverrideCallback s_LnUILayoutElement_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUILayoutElement_OnSerialize_OverrideCallback) s_LnUILayoutElement_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UILayoutElement::onSerialize(ar);
    }

    static LnUILayoutElement_OnSerialize2_OverrideCallback s_LnUILayoutElement_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUILayoutElement_OnSerialize2_OverrideCallback) s_LnUILayoutElement_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::UILayoutElement::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnUILayoutElement_OnSerialize_OverrideCallback LNWS_ln_UILayoutElement::s_LnUILayoutElement_OnSerialize_OverrideCallback = nullptr;
LnUILayoutElement_OnSerialize2_OverrideCallback LNWS_ln_UILayoutElement::s_LnUILayoutElement_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_UIElement : public ln::UIElement
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnUIElement_OverridePrototypes
    {
        LnUIElement_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnUIElement_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnUIElement_OverridePrototypes> m_overridePrototypes = nullptr;
    LnUIElement_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnUIElement_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnUIElement_SubclassRegistrationInfo* subclassInfo() { static LnUIElement_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_UIElement()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIElement()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnUIElement_OnSerialize_OverrideCallback s_LnUIElement_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUIElement_OnSerialize_OverrideCallback) s_LnUIElement_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UIElement::onSerialize(ar);
    }

    static LnUIElement_OnSerialize2_OverrideCallback s_LnUIElement_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUIElement_OnSerialize2_OverrideCallback) s_LnUIElement_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::UIElement::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<UILayoutElement>();
    }

};

LnUIElement_OnSerialize_OverrideCallback LNWS_ln_UIElement::s_LnUIElement_OnSerialize_OverrideCallback = nullptr;
LnUIElement_OnSerialize2_OverrideCallback LNWS_ln_UIElement::s_LnUIElement_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_UITextBlock : public ln::UITextBlock
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnUITextBlock_OverridePrototypes
    {
        LnUITextBlock_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnUITextBlock_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnUITextBlock_OverridePrototypes> m_overridePrototypes = nullptr;
    LnUITextBlock_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnUITextBlock_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnUITextBlock_SubclassRegistrationInfo* subclassInfo() { static LnUITextBlock_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_UITextBlock()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UITextBlock()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnUITextBlock_OnSerialize_OverrideCallback s_LnUITextBlock_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUITextBlock_OnSerialize_OverrideCallback) s_LnUITextBlock_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UITextBlock::onSerialize(ar);
    }

    static LnUITextBlock_OnSerialize2_OverrideCallback s_LnUITextBlock_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUITextBlock_OnSerialize2_OverrideCallback) s_LnUITextBlock_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::UITextBlock::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<UIElement>();
    }

};

LnUITextBlock_OnSerialize_OverrideCallback LNWS_ln_UITextBlock::s_LnUITextBlock_OnSerialize_OverrideCallback = nullptr;
LnUITextBlock_OnSerialize2_OverrideCallback LNWS_ln_UITextBlock::s_LnUITextBlock_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_UISprite : public ln::UISprite
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnUISprite_OverridePrototypes
    {
        LnUISprite_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnUISprite_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;

    };
    std::unique_ptr<LnUISprite_OverridePrototypes> m_overridePrototypes = nullptr;
    LnUISprite_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnUISprite_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnUISprite_SubclassRegistrationInfo* subclassInfo() { static LnUISprite_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_UISprite()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UISprite()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnUISprite_OnSerialize_OverrideCallback s_LnUISprite_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUISprite_OnSerialize_OverrideCallback) s_LnUISprite_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UISprite::onSerialize(ar);
    }

    static LnUISprite_OnSerialize2_OverrideCallback s_LnUISprite_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnUISprite_OnSerialize2_OverrideCallback) s_LnUISprite_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::UISprite::onSerialize2(ar);
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<UIElement>();
    }

};

LnUISprite_OnSerialize_OverrideCallback LNWS_ln_UISprite::s_LnUISprite_OnSerialize_OverrideCallback = nullptr;
LnUISprite_OnSerialize2_OverrideCallback LNWS_ln_UISprite::s_LnUISprite_OnSerialize2_OverrideCallback = nullptr;

class LNWS_ln_Input : public ln::Input
{
public:
};


class LNWS_ln_Mouse : public ln::Mouse
{
public:
};


class LNWS_ln_EngineSettings : public ln::EngineSettings
{
public:
};


class LNWS_ln_Engine : public ln::Engine
{
public:
};


class LNWS_ln_Application : public ln::Application
{
public:
    // Override functions per instance for FlatAPI User.
    struct LnApplication_OverridePrototypes
    {
        LnApplication_OnSerialize_OverrideCallback OnSerialize_OverrideFunc;
        LnApplication_OnSerialize2_OverrideCallback OnSerialize2_OverrideFunc;
        LnApplication_OnInit_OverrideCallback OnInit_OverrideFunc;
        LnApplication_OnUpdate_OverrideCallback OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LnApplication_OverridePrototypes> m_overridePrototypes = nullptr;
    LnApplication_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LnApplication_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LnApplication_SubclassRegistrationInfo* subclassInfo() { static LnApplication_SubclassRegistrationInfo info; return &info; }
    LnSubinstanceId m_subinstance = 0;

    LNWS_ln_Application()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Application()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LnApplication_OnSerialize_OverrideCallback s_LnApplication_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnApplication_OnSerialize_OverrideCallback) s_LnApplication_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Application::onSerialize(ar);
    }

    static LnApplication_OnSerialize2_OverrideCallback s_LnApplication_OnSerialize2_OverrideCallback; // deprecated
    virtual void onSerialize2(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize2_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
                return;
            }
        }
        if (s_LnApplication_OnSerialize2_OverrideCallback) s_LnApplication_OnSerialize2_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }
    void onSerialize2_CallBase(ln::Serializer2* ar)
    {
        ln::Application::onSerialize2(ar);
    }

    static LnApplication_OnInit_OverrideCallback s_LnApplication_OnInit_OverrideCallback; // deprecated
    virtual void onInit() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnInit_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this));
                return;
            }
        }
        if (s_LnApplication_OnInit_OverrideCallback) s_LnApplication_OnInit_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
    }
    void onInit_CallBase()
    {
        ln::Application::onInit();
    }

    static LnApplication_OnUpdate_OverrideCallback s_LnApplication_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func(LNI_OBJECT_TO_HANDLE(this));
                return;
            }
        }
        if (s_LnApplication_OnUpdate_OverrideCallback) s_LnApplication_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
    }
    void onUpdate_CallBase()
    {
        ln::Application::onUpdate();
    }

    // TypeInfo
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LnApplication_OnSerialize_OverrideCallback LNWS_ln_Application::s_LnApplication_OnSerialize_OverrideCallback = nullptr;
LnApplication_OnSerialize2_OverrideCallback LNWS_ln_Application::s_LnApplication_OnSerialize2_OverrideCallback = nullptr;
LnApplication_OnInit_OverrideCallback LNWS_ln_Application::s_LnApplication_OnInit_OverrideCallback = nullptr;
LnApplication_OnUpdate_OverrideCallback LNWS_ln_Application::s_LnApplication_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_Debug : public ln::Debug
{
public:
};




extern "C"
{

LN_FLAT_API LnResult LnVector3_SetZeros(LnVector3* vector3)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector3*>(vector3)) ln::Vector3();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector3_Set(LnVector3* vector3, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector3*>(vector3)) ln::Vector3(x, y, z);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector3_Length(const LnVector3* vector3, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (reinterpret_cast<const ln::Vector3*>(vector3)->length());
    }
    else {
        (reinterpret_cast<const ln::Vector3*>(vector3)->length());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector3_LengthSquared(const LnVector3* vector3, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (reinterpret_cast<const ln::Vector3*>(vector3)->lengthSquared());
    }
    else {
        (reinterpret_cast<const ln::Vector3*>(vector3)->lengthSquared());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector3_MutatingNormalize(LnVector3* vector3)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::Vector3*>(vector3)->mutatingNormalize());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector3_NormalizeXYZ(float x, float y, float z, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(ln::Vector3::normalize(x, y, z));
    }
    else {
        (ln::Vector3::normalize(x, y, z));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector3_Normalize(const LnVector3* vec, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(ln::Vector3::normalize(*reinterpret_cast<const ln::Vector3*>(vec)));
    }
    else {
        (ln::Vector3::normalize(*reinterpret_cast<const ln::Vector3*>(vec)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector4_SetZeros(LnVector4* vector4)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector4*>(vector4)) ln::Vector4();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVector4_Set(LnVector4* vector4, float x, float y, float z, float w)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector4*>(vector4)) ln::Vector4(x, y, z, w);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnQuaternion_SetZeros(LnQuaternion* quaternion)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Quaternion*>(quaternion)) ln::Quaternion();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnQuaternion_Set(LnQuaternion* quaternion, float x, float y, float z, float w)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Quaternion*>(quaternion)) ln::Quaternion(x, y, z, w);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnQuaternion_SetFromAxis(LnQuaternion* quaternion, const LnVector3* axis, float r)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Quaternion*>(quaternion)) ln::Quaternion(*reinterpret_cast<const ln::Vector3*>(axis), r);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnMatrix_SetZeros(LnMatrix* matrix)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Matrix*>(matrix)) ln::Matrix();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnMatrix_Set(LnMatrix* matrix, float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Matrix*>(matrix)) ln::Matrix(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnColor_SetZeros(LnColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Color*>(color)) ln::Color();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnColor_Set(LnColor* color, float r_, float g_, float b_, float a_)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Color*>(color)) ln::Color(r_, g_, b_, a_);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPoint_SetZeros(LnPoint* point)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Point*>(point)) ln::Point();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPoint_Set(LnPoint* point, float x_, float y_)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Point*>(point)) ln::Point(x_, y_);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSize_SetZeros(LnSize* size)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Size*>(size)) ln::Size();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSize_Set(LnSize* size, float w, float h)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Size*>(size)) ln::Size(w, h);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRect_SetZeros(LnRect* rect)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Rect*>(rect)) ln::Rect();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRect_Set(LnRect* rect, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Rect*>(rect)) ln::Rect(x, y, width, height);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRect_GetLeft(const LnRect* rect, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (reinterpret_cast<const ln::Rect*>(rect)->getLeft());
    }
    else {
        (reinterpret_cast<const ln::Rect*>(rect)->getLeft());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRect_SetSize(LnRect* rect, const LnSize* size)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::Rect*>(rect)->setSize(*reinterpret_cast<const ln::Size*>(size)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRect_GetSize(const LnRect* rect, LnSize* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnSize>(reinterpret_cast<const ln::Rect*>(rect)->getSize());
    }
    else {
        (reinterpret_cast<const ln::Rect*>(rect)->getSize());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnThickness_SetZeros(LnThickness* thickness)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Thickness*>(thickness)) ln::Thickness();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnThickness_Set(LnThickness* thickness, float left_, float top_, float right_, float bottom_)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Thickness*>(thickness)) ln::Thickness(left_, top_, right_, bottom_);
    LNI_FUNC_TRY_END_RETURN;
}




LN_FLAT_API LnResult LnObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnObject_OnSerialize_SetOverrideCallback(LnObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Object::s_LnObject_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnObject_SetPrototype_OnSerialize(LnHandle object, LnObject_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnObject_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnObject_OnSerialize2_SetOverrideCallback(LnObject_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Object::s_LnObject_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnObject_SetPrototype_OnSerialize2(LnHandle object, LnObject_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Object>()->id();
}

LN_FLAT_API void LnObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Object>(), id);
}

void LnObject_RegisterSubclassTypeInfo(const LnObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Object>(), info->subclassId);
        *LNWS_ln_Object::subclassInfo() = *info;
    }
}

LnSubinstanceId LnObject_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnEventConnection_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnEventConnection_OnSerialize_SetOverrideCallback(LnEventConnection_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_EventConnection::s_LnEventConnection_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnEventConnection_SetPrototype_OnSerialize(LnHandle object, LnEventConnection_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnEventConnection_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnEventConnection_OnSerialize2_SetOverrideCallback(LnEventConnection_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_EventConnection::s_LnEventConnection_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnEventConnection_SetPrototype_OnSerialize2(LnHandle object, LnEventConnection_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnEventConnection_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::EventConnection>()->id();
}

LN_FLAT_API void LnEventConnection_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EventConnection>(), id);
}

void LnEventConnection_RegisterSubclassTypeInfo(const LnEventConnection_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EventConnection>(), info->subclassId);
        *LNWS_ln_EventConnection::subclassInfo() = *info;
    }
}

LnSubinstanceId LnEventConnection_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnPromiseFailureDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PromiseFailureDelegate>(), id);
}

void LnPromiseFailureDelegate_RegisterSubclassTypeInfo(const LnPromiseFailureDelegate_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PromiseFailureDelegate>(), info->subclassId);
        *LNWS_ln_PromiseFailureDelegate::subclassInfo() = *info;
    }
}

LnSubinstanceId LnPromiseFailureDelegate_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_PromiseFailureDelegate, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnZVTestDelegate1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate1>(), id);
}

void LnZVTestDelegate1_RegisterSubclassTypeInfo(const LnZVTestDelegate1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate1>(), info->subclassId);
        *LNWS_ln_ZVTestDelegate1::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestDelegate1_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestDelegate1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnZVTestDelegate2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate2>(), id);
}

void LnZVTestDelegate2_RegisterSubclassTypeInfo(const LnZVTestDelegate2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate2>(), info->subclassId);
        *LNWS_ln_ZVTestDelegate2::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestDelegate2_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestDelegate2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnZVTestDelegate3_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate3>(), id);
}

void LnZVTestDelegate3_RegisterSubclassTypeInfo(const LnZVTestDelegate3_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate3>(), info->subclassId);
        *LNWS_ln_ZVTestDelegate3::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestDelegate3_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestDelegate3, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnZVTestEventHandler1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler1>(), id);
}

void LnZVTestEventHandler1_RegisterSubclassTypeInfo(const LnZVTestEventHandler1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler1>(), info->subclassId);
        *LNWS_ln_ZVTestEventHandler1::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestEventHandler1_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventHandler1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnZVTestEventHandler2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler2>(), id);
}

void LnZVTestEventHandler2_RegisterSubclassTypeInfo(const LnZVTestEventHandler2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler2>(), info->subclassId);
        *LNWS_ln_ZVTestEventHandler2::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestEventHandler2_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventHandler2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnZVTestPromise1_ThenWith(LnHandle zvtestpromise1, LnHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise1, zvtestpromise1)->thenWith(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate3, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestPromise1_CatchWith(LnHandle zvtestpromise1, LnHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise1, zvtestpromise1)->catchWith(LNI_HANDLE_TO_OBJECT(ln::PromiseFailureDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


extern LN_FLAT_API int LnZVTestPromise1_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestPromise1>()->id();
}

LN_FLAT_API void LnZVTestPromise1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise1>(), id);
}

void LnZVTestPromise1_RegisterSubclassTypeInfo(const LnZVTestPromise1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise1>(), info->subclassId);
        *LNWS_ln_ZVTestPromise1::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestPromise1_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnZVTestPromise2_ThenWith(LnHandle zvtestpromise2, LnHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise2, zvtestpromise2)->thenWith(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate1, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestPromise2_CatchWith(LnHandle zvtestpromise2, LnHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise2, zvtestpromise2)->catchWith(LNI_HANDLE_TO_OBJECT(ln::PromiseFailureDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


extern LN_FLAT_API int LnZVTestPromise2_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestPromise2>()->id();
}

LN_FLAT_API void LnZVTestPromise2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise2>(), id);
}

void LnZVTestPromise2_RegisterSubclassTypeInfo(const LnZVTestPromise2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise2>(), info->subclassId);
        *LNWS_ln_ZVTestPromise2::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestPromise2_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnZVTestClass1_SetTestDelegate1(LnHandle zvtestclass1, LnHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->setTestDelegate1(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate1, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_SetTestDelegate2(LnHandle zvtestclass1, LnHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->setTestDelegate2(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate2, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_SetTestDelegate3(LnHandle zvtestclass1, LnHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->setTestDelegate3(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate3, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_CallTestDelegate1(LnHandle zvtestclass1, int a)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->callTestDelegate1(a));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_CallTestDelegate2(LnHandle zvtestclass1, int a, int b, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->callTestDelegate2(a, b));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->callTestDelegate2(a, b));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_CallTestDelegate3(LnHandle zvtestclass1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->callTestDelegate3());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_LoadAsync(const LnChar* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::ZVTestClass1::loadAsync(filePath));
    }
    else {
        (ln::ZVTestClass1::loadAsync(filePath));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_LoadAsyncA(const char* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::ZVTestClass1::loadAsync(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::ZVTestClass1::loadAsync(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_ExecuteAsync(LnHandle zvtestclass1, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->executeAsync());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->executeAsync());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_GetFilePath(LnHandle zvtestclass1, const LnChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->filePath());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->filePath());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_GetFilePathA(LnHandle zvtestclass1, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF8(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->filePath());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->filePath());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_ConnectOnEvent1(LnHandle zvtestclass1, LnHandle handler, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->connectOnEvent1(LNI_HANDLE_TO_OBJECT(ln::ZVTestEventHandler1, handler)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->connectOnEvent1(LNI_HANDLE_TO_OBJECT(ln::ZVTestEventHandler1, handler)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_RaiseEvent1(LnHandle zvtestclass1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->raiseEvent1());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_ConnectOnEvent2(LnHandle zvtestclass1, LnHandle handler, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->connectOnEvent2(LNI_HANDLE_TO_OBJECT(ln::ZVTestEventHandler2, handler)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->connectOnEvent2(LNI_HANDLE_TO_OBJECT(ln::ZVTestEventHandler2, handler)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_RaiseEvent2(LnHandle zvtestclass1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->raiseEvent2());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_Create(LnHandle* outZVTestClass1)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outZVTestClass1, LNWS_ln_ZVTestClass1, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnZVTestClass1_OnSerialize_SetOverrideCallback(LnZVTestClass1_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ZVTestClass1::s_LnZVTestClass1_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnZVTestClass1_SetPrototype_OnSerialize(LnHandle object, LnZVTestClass1_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnZVTestClass1_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnZVTestClass1_OnSerialize2_SetOverrideCallback(LnZVTestClass1_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_ZVTestClass1::s_LnZVTestClass1_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnZVTestClass1_SetPrototype_OnSerialize2(LnHandle object, LnZVTestClass1_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnZVTestClass1_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>()->id();
}

LN_FLAT_API void LnZVTestClass1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>(), id);
}

void LnZVTestClass1_RegisterSubclassTypeInfo(const LnZVTestClass1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>(), info->subclassId);
        *LNWS_ln_ZVTestClass1::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestClass1_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnZVTestEventArgs1_GetValue(LnHandle zvtesteventargs1, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, zvtesteventargs1)->value());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, zvtesteventargs1)->value());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestEventArgs1_Create(LnHandle* outZVTestEventArgs1)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outZVTestEventArgs1, LNWS_ln_ZVTestEventArgs1, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestEventArgs1_CreateWithValue(int v, LnHandle* outZVTestEventArgs1)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outZVTestEventArgs1, LNWS_ln_ZVTestEventArgs1, init, v);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestEventArgs1_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnZVTestEventArgs1_OnSerialize_SetOverrideCallback(LnZVTestEventArgs1_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ZVTestEventArgs1::s_LnZVTestEventArgs1_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnZVTestEventArgs1_SetPrototype_OnSerialize(LnHandle object, LnZVTestEventArgs1_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnZVTestEventArgs1_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnZVTestEventArgs1_OnSerialize2_SetOverrideCallback(LnZVTestEventArgs1_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_ZVTestEventArgs1::s_LnZVTestEventArgs1_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnZVTestEventArgs1_SetPrototype_OnSerialize2(LnHandle object, LnZVTestEventArgs1_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnZVTestEventArgs1_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestEventArgs1>()->id();
}

LN_FLAT_API void LnZVTestEventArgs1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventArgs1>(), id);
}

void LnZVTestEventArgs1_RegisterSubclassTypeInfo(const LnZVTestEventArgs1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventArgs1>(), info->subclassId);
        *LNWS_ln_ZVTestEventArgs1::subclassInfo() = *info;
    }
}

LnSubinstanceId LnZVTestEventArgs1_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnSerializer_WriteBool(LnHandle serializer, const LnChar* name, LnBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeBool(name, LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteBoolA(LnHandle serializer, const char* name, LnBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeBool(LNI_UTF8STRPTR_TO_STRING(name), LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteInt(LnHandle serializer, const LnChar* name, int value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeInt(name, value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteIntA(LnHandle serializer, const char* name, int value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeInt(LNI_UTF8STRPTR_TO_STRING(name), value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteFloat(LnHandle serializer, const LnChar* name, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeFloat(name, value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteFloatA(LnHandle serializer, const char* name, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeFloat(LNI_UTF8STRPTR_TO_STRING(name), value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteString(LnHandle serializer, const LnChar* name, const LnChar* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeString(name, value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteStringA(LnHandle serializer, const char* name, const char* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeString(LNI_UTF8STRPTR_TO_STRING(name), LNI_UTF8STRPTR_TO_STRING(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteObject(LnHandle serializer, const LnChar* name, LnHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeObject(name, LNI_HANDLE_TO_OBJECT(ln::Object, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_WriteObjectA(LnHandle serializer, const char* name, LnHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->writeObject(LNI_UTF8STRPTR_TO_STRING(name), LNI_HANDLE_TO_OBJECT(ln::Object, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadBool(LnHandle serializer, const LnChar* name, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readBool(name));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readBool(name));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadBoolA(LnHandle serializer, const char* name, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readBool(LNI_UTF8STRPTR_TO_STRING(name)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readBool(LNI_UTF8STRPTR_TO_STRING(name)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadInt(LnHandle serializer, const LnChar* name, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readInt(name));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readInt(name));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadIntA(LnHandle serializer, const char* name, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readInt(LNI_UTF8STRPTR_TO_STRING(name)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readInt(LNI_UTF8STRPTR_TO_STRING(name)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadFloat(LnHandle serializer, const LnChar* name, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readFloat(name));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readFloat(name));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadFloatA(LnHandle serializer, const char* name, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readFloat(LNI_UTF8STRPTR_TO_STRING(name)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readFloat(LNI_UTF8STRPTR_TO_STRING(name)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadString(LnHandle serializer, const LnChar* name, const LnChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readString(name));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readString(name));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadStringA(LnHandle serializer, const char* name, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF8(LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readString(LNI_UTF8STRPTR_TO_STRING(name)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readString(LNI_UTF8STRPTR_TO_STRING(name)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadObject(LnHandle serializer, const LnChar* name, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readObject(name));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readObject(name));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_ReadObjectA(LnHandle serializer, const char* name, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readObject(LNI_UTF8STRPTR_TO_STRING(name)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, serializer)->readObject(LNI_UTF8STRPTR_TO_STRING(name)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_Serialize(LnHandle value, const LnChar* basePath, const LnChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(ln::Serializer::serialize(LNI_HANDLE_TO_OBJECT(ln::Object, value), basePath));
    }
    else {
        (ln::Serializer::serialize(LNI_HANDLE_TO_OBJECT(ln::Object, value), basePath));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_SerializeA(LnHandle value, const char* basePath, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF8(ln::Serializer::serialize(LNI_HANDLE_TO_OBJECT(ln::Object, value), LNI_UTF8STRPTR_TO_STRING(basePath)));
    }
    else {
        (ln::Serializer::serialize(LNI_HANDLE_TO_OBJECT(ln::Object, value), LNI_UTF8STRPTR_TO_STRING(basePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_Deserialize(const LnChar* str, const LnChar* basePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Serializer::deserialize(str, basePath));
    }
    else {
        (ln::Serializer::deserialize(str, basePath));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_DeserializeA(const char* str, const char* basePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Serializer::deserialize(LNI_UTF8STRPTR_TO_STRING(str), LNI_UTF8STRPTR_TO_STRING(basePath)));
    }
    else {
        (ln::Serializer::deserialize(LNI_UTF8STRPTR_TO_STRING(str), LNI_UTF8STRPTR_TO_STRING(basePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSerializer_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSerializer_OnSerialize_SetOverrideCallback(LnSerializer_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Serializer::s_LnSerializer_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSerializer_SetPrototype_OnSerialize(LnHandle object, LnSerializer_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnSerializer_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSerializer_OnSerialize2_SetOverrideCallback(LnSerializer_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Serializer::s_LnSerializer_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSerializer_SetPrototype_OnSerialize2(LnHandle object, LnSerializer_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnSerializer_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Serializer>()->id();
}

LN_FLAT_API void LnSerializer_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Serializer>(), id);
}

void LnSerializer_RegisterSubclassTypeInfo(const LnSerializer_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Serializer>(), info->subclassId);
        *LNWS_ln_Serializer::subclassInfo() = *info;
    }
}

LnSubinstanceId LnSerializer_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnSerializer2_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSerializer2_OnSerialize_SetOverrideCallback(LnSerializer2_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Serializer2::s_LnSerializer2_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSerializer2_SetPrototype_OnSerialize(LnHandle object, LnSerializer2_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnSerializer2_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSerializer2_OnSerialize2_SetOverrideCallback(LnSerializer2_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Serializer2::s_LnSerializer2_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSerializer2_SetPrototype_OnSerialize2(LnHandle object, LnSerializer2_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnSerializer2_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Serializer2>()->id();
}

LN_FLAT_API void LnSerializer2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Serializer2>(), id);
}

void LnSerializer2_RegisterSubclassTypeInfo(const LnSerializer2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Serializer2>(), info->subclassId);
        *LNWS_ln_Serializer2::subclassInfo() = *info;
    }
}

LnSubinstanceId LnSerializer2_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnAssetModel_Target(LnHandle assetmodel, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, assetmodel)->target());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, assetmodel)->target());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssetModel_Create(LnHandle target, LnHandle* outAssetModel)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outAssetModel, LNWS_ln_AssetModel, init, LNI_HANDLE_TO_OBJECT(ln::Object, target));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssetModel_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnAssetModel_OnSerialize_SetOverrideCallback(LnAssetModel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AssetModel::s_LnAssetModel_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnAssetModel_SetPrototype_OnSerialize(LnHandle object, LnAssetModel_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnAssetModel_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnAssetModel_OnSerialize2_SetOverrideCallback(LnAssetModel_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_AssetModel::s_LnAssetModel_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnAssetModel_SetPrototype_OnSerialize2(LnHandle object, LnAssetModel_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnAssetModel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AssetModel>()->id();
}

LN_FLAT_API void LnAssetModel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetModel>(), id);
}

void LnAssetModel_RegisterSubclassTypeInfo(const LnAssetModel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetModel>(), info->subclassId);
        *LNWS_ln_AssetModel::subclassInfo() = *info;
    }
}

LnSubinstanceId LnAssetModel_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnAssets_SaveAssetToLocalFile(LnHandle asset, const LnChar* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::saveAssetToLocalFile(LNI_HANDLE_TO_OBJECT(ln::AssetModel, asset), filePath));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssets_SaveAssetToLocalFileA(LnHandle asset, const char* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::saveAssetToLocalFile(LNI_HANDLE_TO_OBJECT(ln::AssetModel, asset), LNI_UTF8STRPTR_TO_STRING(filePath)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssets_LoadAssetFromLocalFile(const LnChar* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAssetFromLocalFile(filePath));
    }
    else {
        (ln::Assets::loadAssetFromLocalFile(filePath));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssets_LoadAssetFromLocalFileA(const char* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAssetFromLocalFile(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Assets::loadAssetFromLocalFile(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssets_LoadAsset(const LnChar* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAsset(filePath));
    }
    else {
        (ln::Assets::loadAsset(filePath));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssets_LoadAssetA(const char* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAsset(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Assets::loadAsset(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssets_ReloadAsset(const LnChar* filePath, LnHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::reloadAsset(filePath, LNI_HANDLE_TO_OBJECT(ln::Object, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnAssets_ReloadAssetA(const char* filePath, LnHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::reloadAsset(LNI_UTF8STRPTR_TO_STRING(filePath), LNI_HANDLE_TO_OBJECT(ln::Object, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API void LnTexture2DDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DDelegate>(), id);
}

void LnTexture2DDelegate_RegisterSubclassTypeInfo(const LnTexture2DDelegate_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DDelegate>(), info->subclassId);
        *LNWS_ln_Texture2DDelegate::subclassInfo() = *info;
    }
}

LnSubinstanceId LnTexture2DDelegate_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DDelegate, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnTexture2DPromise_ThenWith(LnHandle texture2dpromise, LnHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DPromise, texture2dpromise)->thenWith(LNI_HANDLE_TO_OBJECT(ln::Texture2DDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2DPromise_CatchWith(LnHandle texture2dpromise, LnHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DPromise, texture2dpromise)->catchWith(LNI_HANDLE_TO_OBJECT(ln::PromiseFailureDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


extern LN_FLAT_API int LnTexture2DPromise_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture2DPromise>()->id();
}

LN_FLAT_API void LnTexture2DPromise_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DPromise>(), id);
}

void LnTexture2DPromise_RegisterSubclassTypeInfo(const LnTexture2DPromise_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DPromise>(), info->subclassId);
        *LNWS_ln_Texture2DPromise::subclassInfo() = *info;
    }
}

LnSubinstanceId LnTexture2DPromise_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DPromise, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnTexture_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnTexture_OnSerialize_SetOverrideCallback(LnTexture_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Texture::s_LnTexture_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnTexture_SetPrototype_OnSerialize(LnHandle object, LnTexture_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnTexture_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnTexture_OnSerialize2_SetOverrideCallback(LnTexture_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Texture::s_LnTexture_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnTexture_SetPrototype_OnSerialize2(LnHandle object, LnTexture_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnTexture_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture>()->id();
}

LN_FLAT_API void LnTexture_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture>(), id);
}

void LnTexture_RegisterSubclassTypeInfo(const LnTexture_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture>(), info->subclassId);
        *LNWS_ln_Texture::subclassInfo() = *info;
    }
}

LnSubinstanceId LnTexture_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnTexture2D_Load(const LnChar* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::load(filePath));
    }
    else {
        (ln::Texture2D::load(filePath));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2D_LoadA(const char* filePath, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::load(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Texture2D::load(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2D_LoadEmoji(const LnChar* code, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::loadEmoji(code));
    }
    else {
        (ln::Texture2D::loadEmoji(code));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2D_LoadEmojiA(const char* code, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::loadEmoji(LNI_UTF8STRPTR_TO_STRING(code)));
    }
    else {
        (ln::Texture2D::loadEmoji(LNI_UTF8STRPTR_TO_STRING(code)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, width, height);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, width, height, static_cast<ln::TextureFormat>(format));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2D_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnTexture2D_OnSerialize_SetOverrideCallback(LnTexture2D_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Texture2D::s_LnTexture2D_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnTexture2D_SetPrototype_OnSerialize(LnHandle object, LnTexture2D_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnTexture2D_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnTexture2D_OnSerialize2_SetOverrideCallback(LnTexture2D_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Texture2D::s_LnTexture2D_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnTexture2D_SetPrototype_OnSerialize2(LnHandle object, LnTexture2D_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnTexture2D_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture2D>()->id();
}

LN_FLAT_API void LnTexture2D_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2D>(), id);
}

void LnTexture2D_RegisterSubclassTypeInfo(const LnTexture2D_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2D>(), info->subclassId);
        *LNWS_ln_Texture2D::subclassInfo() = *info;
    }
}

LnSubinstanceId LnTexture2D_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnRenderView_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnRenderView_OnSerialize_SetOverrideCallback(LnRenderView_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_RenderView::s_LnRenderView_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnRenderView_SetPrototype_OnSerialize(LnHandle object, LnRenderView_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnRenderView_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnRenderView_OnSerialize2_SetOverrideCallback(LnRenderView_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_RenderView::s_LnRenderView_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnRenderView_SetPrototype_OnSerialize2(LnHandle object, LnRenderView_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnRenderView_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::RenderView>()->id();
}

LN_FLAT_API void LnRenderView_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RenderView>(), id);
}

void LnRenderView_RegisterSubclassTypeInfo(const LnRenderView_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RenderView>(), info->subclassId);
        *LNWS_ln_RenderView::subclassInfo() = *info;
    }
}

LnSubinstanceId LnRenderView_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnComponent_OnSerialize_SetOverrideCallback(LnComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Component::s_LnComponent_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnComponent_SetPrototype_OnSerialize(LnHandle object, LnComponent_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnComponent_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnComponent_OnSerialize2_SetOverrideCallback(LnComponent_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Component::s_LnComponent_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnComponent_SetPrototype_OnSerialize2(LnHandle object, LnComponent_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Component>()->id();
}

LN_FLAT_API void LnComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Component>(), id);
}

void LnComponent_RegisterSubclassTypeInfo(const LnComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Component>(), info->subclassId);
        *LNWS_ln_Component::subclassInfo() = *info;
    }
}

LnSubinstanceId LnComponent_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnVisualComponent_SetVisible(LnHandle visualcomponent, LnBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, visualcomponent)->setVisible(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVisualComponent_IsVisible(LnHandle visualcomponent, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, visualcomponent)->isVisible());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, visualcomponent)->isVisible());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVisualComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnVisualComponent_OnSerialize_SetOverrideCallback(LnVisualComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_VisualComponent::s_LnVisualComponent_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnVisualComponent_SetPrototype_OnSerialize(LnHandle object, LnVisualComponent_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnVisualComponent_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnVisualComponent_OnSerialize2_SetOverrideCallback(LnVisualComponent_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_VisualComponent::s_LnVisualComponent_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnVisualComponent_SetPrototype_OnSerialize2(LnHandle object, LnVisualComponent_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnVisualComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::VisualComponent>()->id();
}

LN_FLAT_API void LnVisualComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualComponent>(), id);
}

void LnVisualComponent_RegisterSubclassTypeInfo(const LnVisualComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualComponent>(), info->subclassId);
        *LNWS_ln_VisualComponent::subclassInfo() = *info;
    }
}

LnSubinstanceId LnVisualComponent_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnSpriteComponent_SetTexture(LnHandle spritecomponent, LnHandle texture)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, spritecomponent)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, texture)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpriteComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSpriteComponent_OnSerialize_SetOverrideCallback(LnSpriteComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_SpriteComponent::s_LnSpriteComponent_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSpriteComponent_SetPrototype_OnSerialize(LnHandle object, LnSpriteComponent_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnSpriteComponent_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSpriteComponent_OnSerialize2_SetOverrideCallback(LnSpriteComponent_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_SpriteComponent::s_LnSpriteComponent_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSpriteComponent_SetPrototype_OnSerialize2(LnHandle object, LnSpriteComponent_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnSpriteComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::SpriteComponent>()->id();
}

LN_FLAT_API void LnSpriteComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpriteComponent>(), id);
}

void LnSpriteComponent_RegisterSubclassTypeInfo(const LnSpriteComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpriteComponent>(), info->subclassId);
        *LNWS_ln_SpriteComponent::subclassInfo() = *info;
    }
}

LnSubinstanceId LnSpriteComponent_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnWorld_Add(LnHandle world, LnHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_World, world)->add(LNI_HANDLE_TO_OBJECT(ln::WorldObject, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorld_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_World, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnWorld_OnSerialize_SetOverrideCallback(LnWorld_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_World::s_LnWorld_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnWorld_SetPrototype_OnSerialize(LnHandle object, LnWorld_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_World, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnWorld_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_World, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnWorld_OnSerialize2_SetOverrideCallback(LnWorld_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_World::s_LnWorld_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnWorld_SetPrototype_OnSerialize2(LnHandle object, LnWorld_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_World, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnWorld_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::World>()->id();
}

LN_FLAT_API void LnWorld_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::World>(), id);
}

void LnWorld_RegisterSubclassTypeInfo(const LnWorld_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::World>(), info->subclassId);
        *LNWS_ln_World::subclassInfo() = *info;
    }
}

LnSubinstanceId LnWorld_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_World, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnComponentList_GetLength(LnHandle componentlist, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->getLength());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->getLength());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnComponentList_GetItem(LnHandle componentlist, int index, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->getItem(index));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->getItem(index));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnComponentList_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnComponentList_OnSerialize_SetOverrideCallback(LnComponentList_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ComponentList::s_LnComponentList_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnComponentList_SetPrototype_OnSerialize(LnHandle object, LnComponentList_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnComponentList_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnComponentList_OnSerialize2_SetOverrideCallback(LnComponentList_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_ComponentList::s_LnComponentList_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnComponentList_SetPrototype_OnSerialize2(LnHandle object, LnComponentList_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnComponentList_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ComponentList>()->id();
}

LN_FLAT_API void LnComponentList_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ComponentList>(), id);
}

void LnComponentList_RegisterSubclassTypeInfo(const LnComponentList_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ComponentList>(), info->subclassId);
        *LNWS_ln_ComponentList::subclassInfo() = *info;
    }
}

LnSubinstanceId LnComponentList_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnWorldObject_SetPosition(LnHandle worldobject, const LnVector3* pos)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setPosition(*reinterpret_cast<const ln::Vector3*>(pos)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetPositionXYZ(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setPosition(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_GetPosition(LnHandle worldobject, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->position());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->position());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetRotationQuaternion(LnHandle worldobject, const LnQuaternion* rot)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setRotation(*reinterpret_cast<const ln::Quaternion*>(rot)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetRotation(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setRotation(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_GetRotation(LnHandle worldobject, LnQuaternion* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnQuaternion>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->rotation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->rotation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetScale(LnHandle worldobject, const LnVector3* scale)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setScale(*reinterpret_cast<const ln::Vector3*>(scale)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetScaleS(LnHandle worldobject, float xyz)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setScale(xyz));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetScaleXYZ(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setScale(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_GetScale(LnHandle worldobject, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->scale());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->scale());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetCenterPoint(LnHandle worldobject, const LnVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setCenterPoint(*reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_SetCenterPointXYZ(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setCenterPoint(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_GetCenterPoint(LnHandle worldobject, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->centerPoint());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->centerPoint());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_LookAt(LnHandle worldobject, const LnVector3* target)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->lookAt(*reinterpret_cast<const ln::Vector3*>(target)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_LookAtXYZ(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->lookAt(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_AddComponent(LnHandle worldobject, LnHandle component)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->addComponent(LNI_HANDLE_TO_OBJECT(ln::Component, component)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_GetComponents(LnHandle worldobject, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->components());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->components());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnWorldObject_OnSerialize_SetOverrideCallback(LnWorldObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_WorldObject::s_LnWorldObject_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnWorldObject_SetPrototype_OnSerialize(LnHandle object, LnWorldObject_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnWorldObject_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnWorldObject_OnSerialize2_SetOverrideCallback(LnWorldObject_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_WorldObject::s_LnWorldObject_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnWorldObject_SetPrototype_OnSerialize2(LnHandle object, LnWorldObject_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnWorldObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnWorldObject_OnUpdate_SetOverrideCallback(LnWorldObject_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_WorldObject::s_LnWorldObject_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnWorldObject_SetPrototype_OnUpdate(LnHandle worldobject, LnWorldObject_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnWorldObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::WorldObject>()->id();
}

LN_FLAT_API void LnWorldObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldObject>(), id);
}

void LnWorldObject_RegisterSubclassTypeInfo(const LnWorldObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldObject>(), info->subclassId);
        *LNWS_ln_WorldObject::subclassInfo() = *info;
    }
}

LnSubinstanceId LnWorldObject_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnVisualObject_SetVisible(LnHandle visualobject, LnBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->setVisible(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVisualObject_IsVisible(LnHandle visualobject, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->isVisible());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->isVisible());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnVisualObject_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnVisualObject_OnSerialize_SetOverrideCallback(LnVisualObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_VisualObject::s_LnVisualObject_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnVisualObject_SetPrototype_OnSerialize(LnHandle object, LnVisualObject_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnVisualObject_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnVisualObject_OnSerialize2_SetOverrideCallback(LnVisualObject_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_VisualObject::s_LnVisualObject_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnVisualObject_SetPrototype_OnSerialize2(LnHandle object, LnVisualObject_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnVisualObject_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnVisualObject_OnUpdate_SetOverrideCallback(LnVisualObject_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_VisualObject::s_LnVisualObject_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnVisualObject_SetPrototype_OnUpdate(LnHandle worldobject, LnVisualObject_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnVisualObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::VisualObject>()->id();
}

LN_FLAT_API void LnVisualObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualObject>(), id);
}

void LnVisualObject_RegisterSubclassTypeInfo(const LnVisualObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualObject>(), info->subclassId);
        *LNWS_ln_VisualObject::subclassInfo() = *info;
    }
}

LnSubinstanceId LnVisualObject_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnCamera_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnCamera_OnSerialize_SetOverrideCallback(LnCamera_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Camera::s_LnCamera_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnCamera_SetPrototype_OnSerialize(LnHandle object, LnCamera_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnCamera_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnCamera_OnSerialize2_SetOverrideCallback(LnCamera_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Camera::s_LnCamera_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnCamera_SetPrototype_OnSerialize2(LnHandle object, LnCamera_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnCamera_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnCamera_OnUpdate_SetOverrideCallback(LnCamera_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_Camera::s_LnCamera_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnCamera_SetPrototype_OnUpdate(LnHandle worldobject, LnCamera_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnCamera_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Camera>()->id();
}

LN_FLAT_API void LnCamera_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Camera>(), id);
}

void LnCamera_RegisterSubclassTypeInfo(const LnCamera_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Camera>(), info->subclassId);
        *LNWS_ln_Camera::subclassInfo() = *info;
    }
}

LnSubinstanceId LnCamera_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnDirectionalLight_SetEnabled(LnHandle directionallight, LnBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_IsEnabled(LnHandle directionallight, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->isEnabled());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->isEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_SetColor(LnHandle directionallight, const LnColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setColor(*reinterpret_cast<const ln::Color*>(color)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_GetColor(LnHandle directionallight, LnColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->getColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->getColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_SetIntensity(LnHandle directionallight, float intensity)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setIntensity(intensity));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_GetIntensity(LnHandle directionallight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->getIntensity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->getIntensity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_SetShadowEffectiveDistance(LnHandle directionallight, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setShadowEffectiveDistance(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_GetShadowEffectiveDistance(LnHandle directionallight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->shadowEffectiveDistance());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->shadowEffectiveDistance());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_SetShadowEffectiveDepth(LnHandle directionallight, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setShadowEffectiveDepth(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_GetShadowEffectiveDepth(LnHandle directionallight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->shadowEffectiveDepth());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->shadowEffectiveDepth());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_Create(LnHandle* outDirectionalLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDirectionalLight, LNWS_ln_DirectionalLight, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_CreateWithColor(const LnColor* color, LnHandle* outDirectionalLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDirectionalLight, LNWS_ln_DirectionalLight, init, *reinterpret_cast<const ln::Color*>(color));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDirectionalLight_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnDirectionalLight_OnSerialize_SetOverrideCallback(LnDirectionalLight_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_DirectionalLight::s_LnDirectionalLight_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnDirectionalLight_SetPrototype_OnSerialize(LnHandle object, LnDirectionalLight_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnDirectionalLight_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnDirectionalLight_OnSerialize2_SetOverrideCallback(LnDirectionalLight_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_DirectionalLight::s_LnDirectionalLight_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnDirectionalLight_SetPrototype_OnSerialize2(LnHandle object, LnDirectionalLight_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnDirectionalLight_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnDirectionalLight_OnUpdate_SetOverrideCallback(LnDirectionalLight_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_DirectionalLight::s_LnDirectionalLight_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnDirectionalLight_SetPrototype_OnUpdate(LnHandle worldobject, LnDirectionalLight_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnDirectionalLight_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::DirectionalLight>()->id();
}

LN_FLAT_API void LnDirectionalLight_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::DirectionalLight>(), id);
}

void LnDirectionalLight_RegisterSubclassTypeInfo(const LnDirectionalLight_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::DirectionalLight>(), info->subclassId);
        *LNWS_ln_DirectionalLight::subclassInfo() = *info;
    }
}

LnSubinstanceId LnDirectionalLight_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnPointLight_SetEnabled(LnHandle pointlight, LnBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_IsEnabled(LnHandle pointlight, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->isEnabled());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->isEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_SetColor(LnHandle pointlight, const LnColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setColor(*reinterpret_cast<const ln::Color*>(color)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_GetColor(LnHandle pointlight, LnColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_SetIntensity(LnHandle pointlight, float intensity)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setIntensity(intensity));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_GetIntensity(LnHandle pointlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getIntensity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getIntensity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_SetRange(LnHandle pointlight, float range)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setRange(range));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_GetRange(LnHandle pointlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getRange());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getRange());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_SetAttenuation(LnHandle pointlight, float attenuation)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setAttenuation(attenuation));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_GetAttenuation(LnHandle pointlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getAttenuation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getAttenuation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_Create(LnHandle* outPointLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outPointLight, LNWS_ln_PointLight, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_CreateWithColorAndRange(const LnColor* color, float range, LnHandle* outPointLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outPointLight, LNWS_ln_PointLight, init, *reinterpret_cast<const ln::Color*>(color), range);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPointLight_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnPointLight_OnSerialize_SetOverrideCallback(LnPointLight_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_PointLight::s_LnPointLight_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnPointLight_SetPrototype_OnSerialize(LnHandle object, LnPointLight_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnPointLight_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnPointLight_OnSerialize2_SetOverrideCallback(LnPointLight_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_PointLight::s_LnPointLight_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnPointLight_SetPrototype_OnSerialize2(LnHandle object, LnPointLight_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnPointLight_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnPointLight_OnUpdate_SetOverrideCallback(LnPointLight_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_PointLight::s_LnPointLight_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnPointLight_SetPrototype_OnUpdate(LnHandle worldobject, LnPointLight_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnPointLight_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::PointLight>()->id();
}

LN_FLAT_API void LnPointLight_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PointLight>(), id);
}

void LnPointLight_RegisterSubclassTypeInfo(const LnPointLight_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PointLight>(), info->subclassId);
        *LNWS_ln_PointLight::subclassInfo() = *info;
    }
}

LnSubinstanceId LnPointLight_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnSpotLight_SetEnabled(LnHandle spotlight, LnBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_IsEnabled(LnHandle spotlight, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->isEnabled());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->isEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_SetColor(LnHandle spotlight, const LnColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setColor(*reinterpret_cast<const ln::Color*>(color)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_GetColor(LnHandle spotlight, LnColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_SetIntensity(LnHandle spotlight, float intensity)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setIntensity(intensity));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_GetIntensity(LnHandle spotlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getIntensity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getIntensity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_SetRange(LnHandle spotlight, float range)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setRange(range));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_GetRange(LnHandle spotlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getRange());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getRange());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_SetAttenuation(LnHandle spotlight, float attenuation)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setAttenuation(attenuation));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_GetAttenuation(LnHandle spotlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getAttenuation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getAttenuation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_SetAngle(LnHandle spotlight, float angle)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setAngle(angle));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_GetAngle(LnHandle spotlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getAngle());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getAngle());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_SetPenumbra(LnHandle spotlight, float penumbra)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setPenumbra(penumbra));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_GetPenumbra(LnHandle spotlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getPenumbra());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getPenumbra());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_Create(LnHandle* outSpotLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSpotLight, LNWS_ln_SpotLight, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_CreateWithColorAndRange(const LnColor* color, float range, float angle, LnHandle* outSpotLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSpotLight, LNWS_ln_SpotLight, init, *reinterpret_cast<const ln::Color*>(color), range, angle);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSpotLight_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSpotLight_OnSerialize_SetOverrideCallback(LnSpotLight_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_SpotLight::s_LnSpotLight_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSpotLight_SetPrototype_OnSerialize(LnHandle object, LnSpotLight_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnSpotLight_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSpotLight_OnSerialize2_SetOverrideCallback(LnSpotLight_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_SpotLight::s_LnSpotLight_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSpotLight_SetPrototype_OnSerialize2(LnHandle object, LnSpotLight_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnSpotLight_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSpotLight_OnUpdate_SetOverrideCallback(LnSpotLight_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_SpotLight::s_LnSpotLight_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSpotLight_SetPrototype_OnUpdate(LnHandle worldobject, LnSpotLight_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnSpotLight_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::SpotLight>()->id();
}

LN_FLAT_API void LnSpotLight_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpotLight>(), id);
}

void LnSpotLight_RegisterSubclassTypeInfo(const LnSpotLight_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpotLight>(), info->subclassId);
        *LNWS_ln_SpotLight::subclassInfo() = *info;
    }
}

LnSubinstanceId LnSpotLight_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnTestDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::TestDelegate>(), id);
}

void LnTestDelegate_RegisterSubclassTypeInfo(const LnTestDelegate_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::TestDelegate>(), info->subclassId);
        *LNWS_ln_TestDelegate::subclassInfo() = *info;
    }
}

LnSubinstanceId LnTestDelegate_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_TestDelegate, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_SetSize(LnHandle sprite, const LnSize* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setSize(*reinterpret_cast<const ln::Size*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_SetSizeWH(LnHandle sprite, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setSize(width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_SetSourceRectXYWH(LnHandle sprite, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setSourceRect(x, y, width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_SetCallerTest(LnHandle sprite, LnHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setCallerTest(LNI_HANDLE_TO_OBJECT(ln::TestDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_Create(LnHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_CreateWithTexture(LnHandle texture, LnHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_CreateWithTextureAndSize(LnHandle texture, float width, float height, LnHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture), width, height);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnSprite_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSprite_OnSerialize_SetOverrideCallback(LnSprite_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Sprite::s_LnSprite_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSprite_SetPrototype_OnSerialize(LnHandle object, LnSprite_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnSprite_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSprite_OnSerialize2_SetOverrideCallback(LnSprite_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Sprite::s_LnSprite_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSprite_SetPrototype_OnSerialize2(LnHandle object, LnSprite_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnSprite_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnSprite_OnUpdate_SetOverrideCallback(LnSprite_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_Sprite::s_LnSprite_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnSprite_SetPrototype_OnUpdate(LnHandle worldobject, LnSprite_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnSprite_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Sprite>()->id();
}

LN_FLAT_API void LnSprite_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sprite>(), id);
}

void LnSprite_RegisterSubclassTypeInfo(const LnSprite_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sprite>(), info->subclassId);
        *LNWS_ln_Sprite::subclassInfo() = *info;
    }
}

LnSubinstanceId LnSprite_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnCameraOrbitControlComponent_Create(LnHandle* outCameraOrbitControlComponent)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outCameraOrbitControlComponent, LNWS_ln_CameraOrbitControlComponent, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnCameraOrbitControlComponent_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnCameraOrbitControlComponent_OnSerialize_SetOverrideCallback(LnCameraOrbitControlComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_CameraOrbitControlComponent::s_LnCameraOrbitControlComponent_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnCameraOrbitControlComponent_SetPrototype_OnSerialize(LnHandle object, LnCameraOrbitControlComponent_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnCameraOrbitControlComponent_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnCameraOrbitControlComponent_OnSerialize2_SetOverrideCallback(LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_CameraOrbitControlComponent::s_LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnCameraOrbitControlComponent_SetPrototype_OnSerialize2(LnHandle object, LnCameraOrbitControlComponent_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnCameraOrbitControlComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::CameraOrbitControlComponent>()->id();
}

LN_FLAT_API void LnCameraOrbitControlComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CameraOrbitControlComponent>(), id);
}

void LnCameraOrbitControlComponent_RegisterSubclassTypeInfo(const LnCameraOrbitControlComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CameraOrbitControlComponent>(), info->subclassId);
        *LNWS_ln_CameraOrbitControlComponent::subclassInfo() = *info;
    }
}

LnSubinstanceId LnCameraOrbitControlComponent_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnRaycaster_FromScreen(const LnPoint* point, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Raycaster::fromScreen(*reinterpret_cast<const ln::Point*>(point)));
    }
    else {
        (ln::Raycaster::fromScreen(*reinterpret_cast<const ln::Point*>(point)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRaycaster_IntersectPlane(LnHandle raycaster, float normalX, float normalY, float normalZ, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, raycaster)->intersectPlane(normalX, normalY, normalZ));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, raycaster)->intersectPlane(normalX, normalY, normalZ));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRaycaster_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnRaycaster_OnSerialize_SetOverrideCallback(LnRaycaster_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Raycaster::s_LnRaycaster_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnRaycaster_SetPrototype_OnSerialize(LnHandle object, LnRaycaster_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnRaycaster_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnRaycaster_OnSerialize2_SetOverrideCallback(LnRaycaster_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Raycaster::s_LnRaycaster_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnRaycaster_SetPrototype_OnSerialize2(LnHandle object, LnRaycaster_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnRaycaster_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Raycaster>()->id();
}

LN_FLAT_API void LnRaycaster_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Raycaster>(), id);
}

void LnRaycaster_RegisterSubclassTypeInfo(const LnRaycaster_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Raycaster>(), info->subclassId);
        *LNWS_ln_Raycaster::subclassInfo() = *info;
    }
}

LnSubinstanceId LnRaycaster_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnRaycastResult_GetPoint(LnHandle raycastresult, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, raycastresult)->point());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, raycastresult)->point());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnRaycastResult_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnRaycastResult_OnSerialize_SetOverrideCallback(LnRaycastResult_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_RaycastResult::s_LnRaycastResult_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnRaycastResult_SetPrototype_OnSerialize(LnHandle object, LnRaycastResult_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnRaycastResult_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnRaycastResult_OnSerialize2_SetOverrideCallback(LnRaycastResult_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_RaycastResult::s_LnRaycastResult_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnRaycastResult_SetPrototype_OnSerialize2(LnHandle object, LnRaycastResult_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnRaycastResult_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::RaycastResult>()->id();
}

LN_FLAT_API void LnRaycastResult_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RaycastResult>(), id);
}

void LnRaycastResult_RegisterSubclassTypeInfo(const LnRaycastResult_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RaycastResult>(), info->subclassId);
        *LNWS_ln_RaycastResult::subclassInfo() = *info;
    }
}

LnSubinstanceId LnRaycastResult_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnWorldRenderView_SetGuideGridEnabled(LnHandle worldrenderview, LnBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, worldrenderview)->setGuideGridEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldRenderView_GetGuideGridEnabled(LnHandle worldrenderview, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, worldrenderview)->guideGridEnabled());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, worldrenderview)->guideGridEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnWorldRenderView_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnWorldRenderView_OnSerialize_SetOverrideCallback(LnWorldRenderView_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_WorldRenderView::s_LnWorldRenderView_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnWorldRenderView_SetPrototype_OnSerialize(LnHandle object, LnWorldRenderView_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnWorldRenderView_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnWorldRenderView_OnSerialize2_SetOverrideCallback(LnWorldRenderView_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_WorldRenderView::s_LnWorldRenderView_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnWorldRenderView_SetPrototype_OnSerialize2(LnHandle object, LnWorldRenderView_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnWorldRenderView_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::WorldRenderView>()->id();
}

LN_FLAT_API void LnWorldRenderView_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldRenderView>(), id);
}

void LnWorldRenderView_RegisterSubclassTypeInfo(const LnWorldRenderView_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldRenderView>(), info->subclassId);
        *LNWS_ln_WorldRenderView::subclassInfo() = *info;
    }
}

LnSubinstanceId LnWorldRenderView_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnBoxMesh_Create(LnHandle* outBoxMesh)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outBoxMesh, LNWS_ln_BoxMesh, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnBoxMesh_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnBoxMesh_OnSerialize_SetOverrideCallback(LnBoxMesh_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_BoxMesh::s_LnBoxMesh_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnBoxMesh_SetPrototype_OnSerialize(LnHandle object, LnBoxMesh_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnBoxMesh_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnBoxMesh_OnSerialize2_SetOverrideCallback(LnBoxMesh_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_BoxMesh::s_LnBoxMesh_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnBoxMesh_SetPrototype_OnSerialize2(LnHandle object, LnBoxMesh_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnBoxMesh_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnBoxMesh_OnUpdate_SetOverrideCallback(LnBoxMesh_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_BoxMesh::s_LnBoxMesh_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnBoxMesh_SetPrototype_OnUpdate(LnHandle worldobject, LnBoxMesh_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnBoxMesh_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::BoxMesh>()->id();
}

LN_FLAT_API void LnBoxMesh_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::BoxMesh>(), id);
}

void LnBoxMesh_RegisterSubclassTypeInfo(const LnBoxMesh_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::BoxMesh>(), info->subclassId);
        *LNWS_ln_BoxMesh::subclassInfo() = *info;
    }
}

LnSubinstanceId LnBoxMesh_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnPlaneMesh_Create(LnHandle* outPlaneMesh)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outPlaneMesh, LNWS_ln_PlaneMesh, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnPlaneMesh_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnPlaneMesh_OnSerialize_SetOverrideCallback(LnPlaneMesh_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_PlaneMesh::s_LnPlaneMesh_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnPlaneMesh_SetPrototype_OnSerialize(LnHandle object, LnPlaneMesh_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnPlaneMesh_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnPlaneMesh_OnSerialize2_SetOverrideCallback(LnPlaneMesh_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_PlaneMesh::s_LnPlaneMesh_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnPlaneMesh_SetPrototype_OnSerialize2(LnHandle object, LnPlaneMesh_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnPlaneMesh_OnUpdate_CallOverrideBase(LnHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnPlaneMesh_OnUpdate_SetOverrideCallback(LnPlaneMesh_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_PlaneMesh::s_LnPlaneMesh_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnPlaneMesh_SetPrototype_OnUpdate(LnHandle worldobject, LnPlaneMesh_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnPlaneMesh_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::PlaneMesh>()->id();
}

LN_FLAT_API void LnPlaneMesh_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PlaneMesh>(), id);
}

void LnPlaneMesh_RegisterSubclassTypeInfo(const LnPlaneMesh_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PlaneMesh>(), info->subclassId);
        *LNWS_ln_PlaneMesh::subclassInfo() = *info;
    }
}

LnSubinstanceId LnPlaneMesh_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnUIEventArgs_Sender(LnHandle uieventargs, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, uieventargs)->sender());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, uieventargs)->sender());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIEventArgs_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUIEventArgs_OnSerialize_SetOverrideCallback(LnUIEventArgs_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIEventArgs::s_LnUIEventArgs_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUIEventArgs_SetPrototype_OnSerialize(LnHandle object, LnUIEventArgs_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnUIEventArgs_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUIEventArgs_OnSerialize2_SetOverrideCallback(LnUIEventArgs_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_UIEventArgs::s_LnUIEventArgs_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUIEventArgs_SetPrototype_OnSerialize2(LnHandle object, LnUIEventArgs_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUIEventArgs_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIEventArgs>()->id();
}

LN_FLAT_API void LnUIEventArgs_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventArgs>(), id);
}

void LnUIEventArgs_RegisterSubclassTypeInfo(const LnUIEventArgs_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventArgs>(), info->subclassId);
        *LNWS_ln_UIEventArgs::subclassInfo() = *info;
    }
}

LnSubinstanceId LnUIEventArgs_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnUIGeneralEventHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIGeneralEventHandler>(), id);
}

void LnUIGeneralEventHandler_RegisterSubclassTypeInfo(const LnUIGeneralEventHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIGeneralEventHandler>(), info->subclassId);
        *LNWS_ln_UIGeneralEventHandler::subclassInfo() = *info;
    }
}

LnSubinstanceId LnUIGeneralEventHandler_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIGeneralEventHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LnUIEventHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventHandler>(), id);
}

void LnUIEventHandler_RegisterSubclassTypeInfo(const LnUIEventHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventHandler>(), info->subclassId);
        *LNWS_ln_UIEventHandler::subclassInfo() = *info;
    }
}

LnSubinstanceId LnUIEventHandler_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnUILayoutElement_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUILayoutElement_OnSerialize_SetOverrideCallback(LnUILayoutElement_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UILayoutElement::s_LnUILayoutElement_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUILayoutElement_SetPrototype_OnSerialize(LnHandle object, LnUILayoutElement_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnUILayoutElement_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUILayoutElement_OnSerialize2_SetOverrideCallback(LnUILayoutElement_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_UILayoutElement::s_LnUILayoutElement_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUILayoutElement_SetPrototype_OnSerialize2(LnHandle object, LnUILayoutElement_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUILayoutElement_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UILayoutElement>()->id();
}

LN_FLAT_API void LnUILayoutElement_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UILayoutElement>(), id);
}

void LnUILayoutElement_RegisterSubclassTypeInfo(const LnUILayoutElement_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UILayoutElement>(), info->subclassId);
        *LNWS_ln_UILayoutElement::subclassInfo() = *info;
    }
}

LnSubinstanceId LnUILayoutElement_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnUIElement_SetMargin(LnHandle uielement, const LnThickness* margin)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setMargin(*reinterpret_cast<const ln::Thickness*>(margin)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetMargin(LnHandle uielement, LnThickness* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnThickness>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->margin());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->margin());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetPadding(LnHandle uielement, const LnThickness* padding)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setPadding(*reinterpret_cast<const ln::Thickness*>(padding)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetPadding(LnHandle uielement, LnThickness* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnThickness>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->padding());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->padding());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetHAlignment(LnHandle uielement, LnHAlignment value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setHAlignment(static_cast<ln::HAlignment>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetHAlignment(LnHandle uielement, LnHAlignment* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LnHAlignment>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->hAlignment());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->hAlignment());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetVAlignment(LnHandle uielement, LnVAlignment value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setVAlignment(static_cast<ln::VAlignment>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetVAlignment(LnHandle uielement, LnVAlignment* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LnVAlignment>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->vAlignment());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->vAlignment());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetAlignments(LnHandle uielement, LnHAlignment halign, LnVAlignment valign)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setAlignments(static_cast<ln::HAlignment>(halign), static_cast<ln::VAlignment>(valign)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetPosition(LnHandle uielement, const LnVector3* pos)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setPosition(*reinterpret_cast<const ln::Vector3*>(pos)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetPositionXYZ(LnHandle uielement, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setPosition(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetPosition(LnHandle uielement, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->position());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->position());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetRotation(LnHandle uielement, const LnQuaternion* rot)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setRotation(*reinterpret_cast<const ln::Quaternion*>(rot)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetEulerAngles(LnHandle uielement, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setEulerAngles(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetRotation(LnHandle uielement, LnQuaternion* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnQuaternion>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->rotation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->rotation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetScale(LnHandle uielement, const LnVector3* scale)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setScale(*reinterpret_cast<const ln::Vector3*>(scale)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetScaleS(LnHandle uielement, float xyz)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setScale(xyz));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetScaleXY(LnHandle uielement, float x, float y)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setScale(x, y));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetScale(LnHandle uielement, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->scale());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->scale());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetCenterPoint(LnHandle uielement, const LnVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setCenterPoint(*reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_SetCenterPointXYZ(LnHandle uielement, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setCenterPoint(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_GetCenterPoint(LnHandle uielement, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->centerPoint());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->centerPoint());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_AddChild(LnHandle uielement, LnHandle child)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->addChild(LNI_HANDLE_TO_OBJECT(ln::UIElement, child)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIElement_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUIElement_OnSerialize_SetOverrideCallback(LnUIElement_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIElement::s_LnUIElement_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUIElement_SetPrototype_OnSerialize(LnHandle object, LnUIElement_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnUIElement_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUIElement_OnSerialize2_SetOverrideCallback(LnUIElement_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_UIElement::s_LnUIElement_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUIElement_SetPrototype_OnSerialize2(LnHandle object, LnUIElement_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUIElement_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIElement>()->id();
}

LN_FLAT_API void LnUIElement_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIElement>(), id);
}

void LnUIElement_RegisterSubclassTypeInfo(const LnUIElement_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIElement>(), info->subclassId);
        *LNWS_ln_UIElement::subclassInfo() = *info;
    }
}

LnSubinstanceId LnUIElement_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnUITextBlock_Create(LnHandle* outUITextBlock)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUITextBlock, LNWS_ln_UITextBlock, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUITextBlock_CreateWithText(const LnChar* text, LnHandle* outUITextBlock)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUITextBlock, LNWS_ln_UITextBlock, init, text);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUITextBlock_CreateWithTextA(const char* text, LnHandle* outUITextBlock)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUITextBlock, LNWS_ln_UITextBlock, init, LNI_UTF8STRPTR_TO_STRING(text));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUITextBlock_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUITextBlock_OnSerialize_SetOverrideCallback(LnUITextBlock_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UITextBlock::s_LnUITextBlock_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUITextBlock_SetPrototype_OnSerialize(LnHandle object, LnUITextBlock_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnUITextBlock_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUITextBlock_OnSerialize2_SetOverrideCallback(LnUITextBlock_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_UITextBlock::s_LnUITextBlock_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUITextBlock_SetPrototype_OnSerialize2(LnHandle object, LnUITextBlock_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUITextBlock_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UITextBlock>()->id();
}

LN_FLAT_API void LnUITextBlock_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UITextBlock>(), id);
}

void LnUITextBlock_RegisterSubclassTypeInfo(const LnUITextBlock_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UITextBlock>(), info->subclassId);
        *LNWS_ln_UITextBlock::subclassInfo() = *info;
    }
}

LnSubinstanceId LnUITextBlock_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnUISprite_SetTexture(LnHandle uisprite, LnHandle texture)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, texture)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUISprite_SetSourceRect(LnHandle uisprite, const LnRect* rect)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->setSourceRect(*reinterpret_cast<const ln::Rect*>(rect)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUISprite_SetSourceRectXYWH(LnHandle uisprite, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->setSourceRect(x, y, width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUISprite_GetSourceRect(LnHandle uisprite, LnRect* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnRect>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->sourceRect());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->sourceRect());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUISprite_Create(LnHandle* outUISprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUISprite, LNWS_ln_UISprite, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUISprite_CreateWithTexture(LnHandle texture, LnHandle* outUISprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUISprite, LNWS_ln_UISprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUISprite_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUISprite_OnSerialize_SetOverrideCallback(LnUISprite_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UISprite::s_LnUISprite_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUISprite_SetPrototype_OnSerialize(LnHandle object, LnUISprite_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnUISprite_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUISprite_OnSerialize2_SetOverrideCallback(LnUISprite_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_UISprite::s_LnUISprite_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnUISprite_SetPrototype_OnSerialize2(LnHandle object, LnUISprite_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUISprite_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UISprite>()->id();
}

LN_FLAT_API void LnUISprite_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UISprite>(), id);
}

void LnUISprite_RegisterSubclassTypeInfo(const LnUISprite_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UISprite>(), info->subclassId);
        *LNWS_ln_UISprite::subclassInfo() = *info;
    }
}

LnSubinstanceId LnUISprite_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnInput_Pressed(const LnChar* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::pressed(buttonName));
    }
    else {
        (ln::Input::pressed(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_PressedA(const char* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::pressed(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::pressed(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_Triggered(const LnChar* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::triggered(buttonName));
    }
    else {
        (ln::Input::triggered(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_TriggeredA(const char* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::triggered(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::triggered(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_TriggeredOff(const LnChar* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::triggeredOff(buttonName));
    }
    else {
        (ln::Input::triggeredOff(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_TriggeredOffA(const char* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::triggeredOff(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::triggeredOff(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_Repeated(const LnChar* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::repeated(buttonName));
    }
    else {
        (ln::Input::repeated(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_RepeatedA(const char* buttonName, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::repeated(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::repeated(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_GetAxisValue(const LnChar* buttonName, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (ln::Input::getAxisValue(buttonName));
    }
    else {
        (ln::Input::getAxisValue(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_GetAxisValueA(const char* buttonName, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (ln::Input::getAxisValue(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::getAxisValue(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnInput_ClearAllBindings()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Input::clearAllBindings());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnMouse_Pressed(LnMouseButtons button, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Mouse::pressed(static_cast<ln::MouseButtons>(button)));
    }
    else {
        (ln::Mouse::pressed(static_cast<ln::MouseButtons>(button)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnMouse_Triggered(LnMouseButtons button, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Mouse::triggered(static_cast<ln::MouseButtons>(button)));
    }
    else {
        (ln::Mouse::triggered(static_cast<ln::MouseButtons>(button)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnMouse_TriggeredOff(LnMouseButtons button, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Mouse::triggeredOff(static_cast<ln::MouseButtons>(button)));
    }
    else {
        (ln::Mouse::triggeredOff(static_cast<ln::MouseButtons>(button)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnMouse_Repeated(LnMouseButtons button, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Mouse::repeated(static_cast<ln::MouseButtons>(button)));
    }
    else {
        (ln::Mouse::repeated(static_cast<ln::MouseButtons>(button)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnMouse_Position(LnPoint* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LnPoint>(ln::Mouse::position());
    }
    else {
        (ln::Mouse::position());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetMainWindowSize(int width, int height)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowSize(width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetMainWorldViewSize(int width, int height)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWorldViewSize(width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetMainWindowTitle(const LnChar* title)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowTitle(title));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetMainWindowTitleA(const char* title)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowTitle(LNI_UTF8STRPTR_TO_STRING(title)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_AddAssetDirectory(const LnChar* path)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetDirectory(path));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_AddAssetDirectoryA(const char* path)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetDirectory(LNI_UTF8STRPTR_TO_STRING(path)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_AddAssetArchive(const LnChar* fileFullPath, const LnChar* password)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetArchive(fileFullPath, password));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_AddAssetArchiveA(const char* fileFullPath, const char* password)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetArchive(LNI_UTF8STRPTR_TO_STRING(fileFullPath), LNI_UTF8STRPTR_TO_STRING(password)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetFrameRate(int value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setFrameRate(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetDebugToolEnabled(LnBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setDebugToolEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetEngineLogEnabled(LnBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setEngineLogEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetEngineLogFilePath(const LnChar* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setEngineLogFilePath(filePath));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngineSettings_SetEngineLogFilePathA(const char* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setEngineLogFilePath(LNI_UTF8STRPTR_TO_STRING(filePath)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngine_Initialize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::initialize());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngine_Finalize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::finalize());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Engine::update());
    }
    else {
        (ln::Engine::update());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngine_Time(double* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (ln::Engine::time());
    }
    else {
        (ln::Engine::time());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngine_GetCamera(LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Engine::camera());
    }
    else {
        (ln::Engine::camera());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngine_GetLight(LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Engine::light());
    }
    else {
        (ln::Engine::light());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnEngine_GetRenderView(LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Engine::renderView());
    }
    else {
        (ln::Engine::renderView());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnApplication_OnInit(LnHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->LNWS_ln_Application::onInit_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnApplication_OnUpdate(LnHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->LNWS_ln_Application::onUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnApplication_World(LnHandle application, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->world());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->world());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnApplication_Create(LnHandle* outApplication)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outApplication, LNWS_ln_Application, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnApplication_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnApplication_OnSerialize_SetOverrideCallback(LnApplication_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Application::s_LnApplication_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnApplication_SetPrototype_OnSerialize(LnHandle object, LnApplication_OnSerialize_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnApplication_OnSerialize2_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, object)->onSerialize2_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnApplication_OnSerialize2_SetOverrideCallback(LnApplication_OnSerialize2_OverrideCallback callback)
{
    LNWS_ln_Application::s_LnApplication_OnSerialize2_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnApplication_SetPrototype_OnSerialize2(LnHandle object, LnApplication_OnSerialize2_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, object)->acquireOverridePrototypes()->OnSerialize2_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnApplication_OnInit_CallOverrideBase(LnHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->onInit_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnApplication_OnInit_SetOverrideCallback(LnApplication_OnInit_OverrideCallback callback)
{
    LNWS_ln_Application::s_LnApplication_OnInit_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnApplication_SetPrototype_OnInit(LnHandle application, LnApplication_OnInit_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnInit_OverrideFunc = func;
    return LN_SUCCESS;
}

LN_FLAT_API LnResult LnApplication_OnUpdate_CallOverrideBase(LnHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->onUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnApplication_OnUpdate_SetOverrideCallback(LnApplication_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_Application::s_LnApplication_OnUpdate_OverrideCallback = callback;
    return LN_SUCCESS;
}
LN_FLAT_API LnResult LnApplication_SetPrototype_OnUpdate(LnHandle application, LnApplication_OnUpdate_OverrideCallback func)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = func;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnApplication_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Application>()->id();
}

LN_FLAT_API void LnApplication_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Application>(), id);
}

void LnApplication_RegisterSubclassTypeInfo(const LnApplication_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Application>(), info->subclassId);
        *LNWS_ln_Application::subclassInfo() = *info;
    }
}

LnSubinstanceId LnApplication_GetSubinstanceId(LnHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LnResult LnDebug_Print(const LnChar* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(str));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDebug_PrintA(const char* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(LNI_UTF8STRPTR_TO_STRING(str)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDebug_PrintWithTime(float time, const LnChar* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, str));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDebug_PrintWithTimeA(float time, const char* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, LNI_UTF8STRPTR_TO_STRING(str)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDebug_PrintWithTimeAndColor(float time, const LnColor* color, const LnChar* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, *reinterpret_cast<const ln::Color*>(color), str));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnDebug_PrintWithTimeAndColorA(float time, const LnColor* color, const char* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, *reinterpret_cast<const ln::Color*>(color), LNI_UTF8STRPTR_TO_STRING(str)));
    LNI_FUNC_TRY_END_RETURN;
}



	
} // extern "C"
