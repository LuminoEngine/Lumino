#include <LuminoFFI/Runtime.hpp>
#include <LuminoFFI/Lumino.FlatC.generated.h>

#include <LuminoEngine.hpp>
#include "BindingValidation.hpp"

// Auto generated override handler
using ObjectSerializeHandler = ln::Delegate<void(ln::Object* self, ln::Serializer2* ar)>;

class LNWS_ObjectSerializeHandler : public ObjectSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNObjectSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNObjectSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNObjectSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNObjectSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNObjectSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNObjectSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNObjectSerializeHandlerCallback m_callback;

    LNWS_ObjectSerializeHandler()
      : ObjectSerializeHandler([this](ln::Object* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNObjectSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ObjectSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNObjectSerializeHandlerCallback callback)
    {
        if (!ObjectSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNObjectSerializeHandler_Create(LNObjectSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ObjectSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using EventConnectionSerializeHandler = ln::Delegate<void(ln::EventConnection* self, ln::Serializer2* ar)>;

class LNWS_EventConnectionSerializeHandler : public EventConnectionSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEventConnectionSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNEventConnectionSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEventConnectionSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEventConnectionSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEventConnectionSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNEventConnectionSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNEventConnectionSerializeHandlerCallback m_callback;

    LNWS_EventConnectionSerializeHandler()
      : EventConnectionSerializeHandler([this](ln::EventConnection* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNEventConnectionSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_EventConnectionSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNEventConnectionSerializeHandlerCallback callback)
    {
        if (!EventConnectionSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNEventConnectionSerializeHandler_Create(LNEventConnectionSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_EventConnectionSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using VariantSerializeHandler = ln::Delegate<void(ln::Variant* self, ln::Serializer2* ar)>;

class LNWS_VariantSerializeHandler : public VariantSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVariantSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNVariantSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVariantSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVariantSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVariantSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNVariantSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNVariantSerializeHandlerCallback m_callback;

    LNWS_VariantSerializeHandler()
      : VariantSerializeHandler([this](ln::Variant* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNVariantSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_VariantSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNVariantSerializeHandlerCallback callback)
    {
        if (!VariantSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNVariantSerializeHandler_Create(LNVariantSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_VariantSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ZVTestClass1SerializeHandler = ln::Delegate<void(ln::ZVTestClass1* self, ln::Serializer2* ar)>;

class LNWS_ZVTestClass1SerializeHandler : public ZVTestClass1SerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestClass1SerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestClass1SerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestClass1SerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestClass1SerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestClass1SerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNZVTestClass1SerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNZVTestClass1SerializeHandlerCallback m_callback;

    LNWS_ZVTestClass1SerializeHandler()
      : ZVTestClass1SerializeHandler([this](ln::ZVTestClass1* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNZVTestClass1SerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ZVTestClass1SerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNZVTestClass1SerializeHandlerCallback callback)
    {
        if (!ZVTestClass1SerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNZVTestClass1SerializeHandler_Create(LNZVTestClass1SerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ZVTestClass1SerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ZVTestEventArgs1SerializeHandler = ln::Delegate<void(ln::ZVTestEventArgs1* self, ln::Serializer2* ar)>;

class LNWS_ZVTestEventArgs1SerializeHandler : public ZVTestEventArgs1SerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestEventArgs1SerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestEventArgs1SerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestEventArgs1SerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestEventArgs1SerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNZVTestEventArgs1SerializeHandlerCallback m_callback;

    LNWS_ZVTestEventArgs1SerializeHandler()
      : ZVTestEventArgs1SerializeHandler([this](ln::ZVTestEventArgs1* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNZVTestEventArgs1SerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ZVTestEventArgs1SerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNZVTestEventArgs1SerializeHandlerCallback callback)
    {
        if (!ZVTestEventArgs1SerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNZVTestEventArgs1SerializeHandler_Create(LNZVTestEventArgs1SerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ZVTestEventArgs1SerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using Serializer2SerializeHandler = ln::Delegate<void(ln::Serializer2* self, ln::Serializer2* ar)>;

class LNWS_Serializer2SerializeHandler : public Serializer2SerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSerializer2SerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSerializer2SerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSerializer2SerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSerializer2SerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSerializer2SerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNSerializer2SerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSerializer2SerializeHandlerCallback m_callback;

    LNWS_Serializer2SerializeHandler()
      : Serializer2SerializeHandler([this](ln::Serializer2* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNSerializer2SerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_Serializer2SerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSerializer2SerializeHandlerCallback callback)
    {
        if (!Serializer2SerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNSerializer2SerializeHandler_Create(LNSerializer2SerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_Serializer2SerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using AssetObjectSerializeHandler = ln::Delegate<void(ln::AssetObject* self, ln::Serializer2* ar)>;

class LNWS_AssetObjectSerializeHandler : public AssetObjectSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAssetObjectSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNAssetObjectSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAssetObjectSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAssetObjectSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAssetObjectSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNAssetObjectSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNAssetObjectSerializeHandlerCallback m_callback;

    LNWS_AssetObjectSerializeHandler()
      : AssetObjectSerializeHandler([this](ln::AssetObject* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNAssetObjectSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_AssetObjectSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNAssetObjectSerializeHandlerCallback callback)
    {
        if (!AssetObjectSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNAssetObjectSerializeHandler_Create(LNAssetObjectSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_AssetObjectSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using AssetImportSettingsSerializeHandler = ln::Delegate<void(ln::AssetImportSettings* self, ln::Serializer2* ar)>;

class LNWS_AssetImportSettingsSerializeHandler : public AssetImportSettingsSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAssetImportSettingsSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNAssetImportSettingsSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAssetImportSettingsSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAssetImportSettingsSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAssetImportSettingsSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNAssetImportSettingsSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNAssetImportSettingsSerializeHandlerCallback m_callback;

    LNWS_AssetImportSettingsSerializeHandler()
      : AssetImportSettingsSerializeHandler([this](ln::AssetImportSettings* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNAssetImportSettingsSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_AssetImportSettingsSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNAssetImportSettingsSerializeHandlerCallback callback)
    {
        if (!AssetImportSettingsSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNAssetImportSettingsSerializeHandler_Create(LNAssetImportSettingsSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_AssetImportSettingsSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using AssetModelSerializeHandler = ln::Delegate<void(ln::AssetModel* self, ln::Serializer2* ar)>;

class LNWS_AssetModelSerializeHandler : public AssetModelSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAssetModelSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNAssetModelSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAssetModelSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAssetModelSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAssetModelSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNAssetModelSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNAssetModelSerializeHandlerCallback m_callback;

    LNWS_AssetModelSerializeHandler()
      : AssetModelSerializeHandler([this](ln::AssetModel* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNAssetModelSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_AssetModelSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNAssetModelSerializeHandlerCallback callback)
    {
        if (!AssetModelSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNAssetModelSerializeHandler_Create(LNAssetModelSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_AssetModelSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SoundSerializeHandler = ln::Delegate<void(ln::Sound* self, ln::Serializer2* ar)>;

class LNWS_SoundSerializeHandler : public SoundSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSoundSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSoundSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSoundSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSoundSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSoundSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNSoundSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSoundSerializeHandlerCallback m_callback;

    LNWS_SoundSerializeHandler()
      : SoundSerializeHandler([this](ln::Sound* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNSoundSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SoundSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSoundSerializeHandlerCallback callback)
    {
        if (!SoundSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNSoundSerializeHandler_Create(LNSoundSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SoundSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using TextureSerializeHandler = ln::Delegate<void(ln::Texture* self, ln::Serializer2* ar)>;

class LNWS_TextureSerializeHandler : public TextureSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTextureSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNTextureSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTextureSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTextureSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTextureSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNTextureSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNTextureSerializeHandlerCallback m_callback;

    LNWS_TextureSerializeHandler()
      : TextureSerializeHandler([this](ln::Texture* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNTextureSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_TextureSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNTextureSerializeHandlerCallback callback)
    {
        if (!TextureSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNTextureSerializeHandler_Create(LNTextureSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_TextureSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using Texture2DSerializeHandler = ln::Delegate<void(ln::Texture2D* self, ln::Serializer2* ar)>;

class LNWS_Texture2DSerializeHandler : public Texture2DSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTexture2DSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNTexture2DSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTexture2DSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTexture2DSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTexture2DSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNTexture2DSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNTexture2DSerializeHandlerCallback m_callback;

    LNWS_Texture2DSerializeHandler()
      : Texture2DSerializeHandler([this](ln::Texture2D* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNTexture2DSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_Texture2DSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNTexture2DSerializeHandlerCallback callback)
    {
        if (!Texture2DSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNTexture2DSerializeHandler_Create(LNTexture2DSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_Texture2DSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ShaderSerializeHandler = ln::Delegate<void(ln::Shader* self, ln::Serializer2* ar)>;

class LNWS_ShaderSerializeHandler : public ShaderSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNShaderSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNShaderSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNShaderSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNShaderSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNShaderSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNShaderSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNShaderSerializeHandlerCallback m_callback;

    LNWS_ShaderSerializeHandler()
      : ShaderSerializeHandler([this](ln::Shader* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNShaderSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ShaderSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNShaderSerializeHandlerCallback callback)
    {
        if (!ShaderSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNShaderSerializeHandler_Create(LNShaderSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ShaderSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using RenderViewSerializeHandler = ln::Delegate<void(ln::RenderView* self, ln::Serializer2* ar)>;

class LNWS_RenderViewSerializeHandler : public RenderViewSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNRenderViewSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNRenderViewSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNRenderViewSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNRenderViewSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNRenderViewSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNRenderViewSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNRenderViewSerializeHandlerCallback m_callback;

    LNWS_RenderViewSerializeHandler()
      : RenderViewSerializeHandler([this](ln::RenderView* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNRenderViewSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_RenderViewSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNRenderViewSerializeHandlerCallback callback)
    {
        if (!RenderViewSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNRenderViewSerializeHandler_Create(LNRenderViewSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_RenderViewSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using MaterialSerializeHandler = ln::Delegate<void(ln::Material* self, ln::Serializer2* ar)>;

class LNWS_MaterialSerializeHandler : public MaterialSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMaterialSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNMaterialSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMaterialSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMaterialSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMaterialSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNMaterialSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNMaterialSerializeHandlerCallback m_callback;

    LNWS_MaterialSerializeHandler()
      : MaterialSerializeHandler([this](ln::Material* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNMaterialSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_MaterialSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNMaterialSerializeHandlerCallback callback)
    {
        if (!MaterialSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNMaterialSerializeHandler_Create(LNMaterialSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_MaterialSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using MeshNodeSerializeHandler = ln::Delegate<void(ln::MeshNode* self, ln::Serializer2* ar)>;

class LNWS_MeshNodeSerializeHandler : public MeshNodeSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshNodeSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNMeshNodeSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshNodeSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshNodeSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshNodeSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNMeshNodeSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNMeshNodeSerializeHandlerCallback m_callback;

    LNWS_MeshNodeSerializeHandler()
      : MeshNodeSerializeHandler([this](ln::MeshNode* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNMeshNodeSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_MeshNodeSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNMeshNodeSerializeHandlerCallback callback)
    {
        if (!MeshNodeSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNMeshNodeSerializeHandler_Create(LNMeshNodeSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_MeshNodeSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using AnimationControllerSerializeHandler = ln::Delegate<void(ln::AnimationController* self, ln::Serializer2* ar)>;

class LNWS_AnimationControllerSerializeHandler : public AnimationControllerSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationControllerSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNAnimationControllerSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationControllerSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationControllerSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationControllerSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNAnimationControllerSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNAnimationControllerSerializeHandlerCallback m_callback;

    LNWS_AnimationControllerSerializeHandler()
      : AnimationControllerSerializeHandler([this](ln::AnimationController* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNAnimationControllerSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_AnimationControllerSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNAnimationControllerSerializeHandlerCallback callback)
    {
        if (!AnimationControllerSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNAnimationControllerSerializeHandler_Create(LNAnimationControllerSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_AnimationControllerSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using MeshModelSerializeHandler = ln::Delegate<void(ln::MeshModel* self, ln::Serializer2* ar)>;

class LNWS_MeshModelSerializeHandler : public MeshModelSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshModelSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNMeshModelSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshModelSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshModelSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshModelSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNMeshModelSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNMeshModelSerializeHandlerCallback m_callback;

    LNWS_MeshModelSerializeHandler()
      : MeshModelSerializeHandler([this](ln::MeshModel* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNMeshModelSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_MeshModelSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNMeshModelSerializeHandlerCallback callback)
    {
        if (!MeshModelSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNMeshModelSerializeHandler_Create(LNMeshModelSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_MeshModelSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using MeshImportSettingsSerializeHandler = ln::Delegate<void(ln::MeshImportSettings* self, ln::Serializer2* ar)>;

class LNWS_MeshImportSettingsSerializeHandler : public MeshImportSettingsSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshImportSettingsSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNMeshImportSettingsSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshImportSettingsSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshImportSettingsSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshImportSettingsSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNMeshImportSettingsSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNMeshImportSettingsSerializeHandlerCallback m_callback;

    LNWS_MeshImportSettingsSerializeHandler()
      : MeshImportSettingsSerializeHandler([this](ln::MeshImportSettings* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNMeshImportSettingsSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_MeshImportSettingsSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNMeshImportSettingsSerializeHandlerCallback callback)
    {
        if (!MeshImportSettingsSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNMeshImportSettingsSerializeHandler_Create(LNMeshImportSettingsSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_MeshImportSettingsSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SkinnedMeshModelSerializeHandler = ln::Delegate<void(ln::SkinnedMeshModel* self, ln::Serializer2* ar)>;

class LNWS_SkinnedMeshModelSerializeHandler : public SkinnedMeshModelSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSkinnedMeshModelSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSkinnedMeshModelSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSkinnedMeshModelSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSkinnedMeshModelSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSkinnedMeshModelSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNSkinnedMeshModelSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSkinnedMeshModelSerializeHandlerCallback m_callback;

    LNWS_SkinnedMeshModelSerializeHandler()
      : SkinnedMeshModelSerializeHandler([this](ln::SkinnedMeshModel* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNSkinnedMeshModelSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SkinnedMeshModelSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSkinnedMeshModelSerializeHandlerCallback callback)
    {
        if (!SkinnedMeshModelSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNSkinnedMeshModelSerializeHandler_Create(LNSkinnedMeshModelSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SkinnedMeshModelSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using CollisionShapeSerializeHandler = ln::Delegate<void(ln::CollisionShape* self, ln::Serializer2* ar)>;

class LNWS_CollisionShapeSerializeHandler : public CollisionShapeSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCollisionShapeSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCollisionShapeSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCollisionShapeSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCollisionShapeSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCollisionShapeSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNCollisionShapeSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCollisionShapeSerializeHandlerCallback m_callback;

    LNWS_CollisionShapeSerializeHandler()
      : CollisionShapeSerializeHandler([this](ln::CollisionShape* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNCollisionShapeSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_CollisionShapeSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCollisionShapeSerializeHandlerCallback callback)
    {
        if (!CollisionShapeSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNCollisionShapeSerializeHandler_Create(LNCollisionShapeSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_CollisionShapeSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using BoxCollisionShapeSerializeHandler = ln::Delegate<void(ln::BoxCollisionShape* self, ln::Serializer2* ar)>;

class LNWS_BoxCollisionShapeSerializeHandler : public BoxCollisionShapeSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNBoxCollisionShapeSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNBoxCollisionShapeSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNBoxCollisionShapeSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNBoxCollisionShapeSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNBoxCollisionShapeSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNBoxCollisionShapeSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNBoxCollisionShapeSerializeHandlerCallback m_callback;

    LNWS_BoxCollisionShapeSerializeHandler()
      : BoxCollisionShapeSerializeHandler([this](ln::BoxCollisionShape* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNBoxCollisionShapeSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_BoxCollisionShapeSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNBoxCollisionShapeSerializeHandlerCallback callback)
    {
        if (!BoxCollisionShapeSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNBoxCollisionShapeSerializeHandler_Create(LNBoxCollisionShapeSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_BoxCollisionShapeSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using AnimationCurveSerializeHandler = ln::Delegate<void(ln::AnimationCurve* self, ln::Serializer2* ar)>;

class LNWS_AnimationCurveSerializeHandler : public AnimationCurveSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationCurveSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNAnimationCurveSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationCurveSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationCurveSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationCurveSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNAnimationCurveSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNAnimationCurveSerializeHandlerCallback m_callback;

    LNWS_AnimationCurveSerializeHandler()
      : AnimationCurveSerializeHandler([this](ln::AnimationCurve* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNAnimationCurveSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_AnimationCurveSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNAnimationCurveSerializeHandlerCallback callback)
    {
        if (!AnimationCurveSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNAnimationCurveSerializeHandler_Create(LNAnimationCurveSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_AnimationCurveSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using KeyFrameAnimationCurveSerializeHandler = ln::Delegate<void(ln::KeyFrameAnimationCurve* self, ln::Serializer2* ar)>;

class LNWS_KeyFrameAnimationCurveSerializeHandler : public KeyFrameAnimationCurveSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNKeyFrameAnimationCurveSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNKeyFrameAnimationCurveSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNKeyFrameAnimationCurveSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNKeyFrameAnimationCurveSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNKeyFrameAnimationCurveSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNKeyFrameAnimationCurveSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNKeyFrameAnimationCurveSerializeHandlerCallback m_callback;

    LNWS_KeyFrameAnimationCurveSerializeHandler()
      : KeyFrameAnimationCurveSerializeHandler([this](ln::KeyFrameAnimationCurve* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNKeyFrameAnimationCurveSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_KeyFrameAnimationCurveSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNKeyFrameAnimationCurveSerializeHandlerCallback callback)
    {
        if (!KeyFrameAnimationCurveSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNKeyFrameAnimationCurveSerializeHandler_Create(LNKeyFrameAnimationCurveSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_KeyFrameAnimationCurveSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using AnimationClipSerializeHandler = ln::Delegate<void(ln::AnimationClip* self, ln::Serializer2* ar)>;

class LNWS_AnimationClipSerializeHandler : public AnimationClipSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationClipSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNAnimationClipSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationClipSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationClipSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationClipSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNAnimationClipSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNAnimationClipSerializeHandlerCallback m_callback;

    LNWS_AnimationClipSerializeHandler()
      : AnimationClipSerializeHandler([this](ln::AnimationClip* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNAnimationClipSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_AnimationClipSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNAnimationClipSerializeHandlerCallback callback)
    {
        if (!AnimationClipSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNAnimationClipSerializeHandler_Create(LNAnimationClipSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_AnimationClipSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using AnimationStateSerializeHandler = ln::Delegate<void(ln::AnimationState* self, ln::Serializer2* ar)>;

class LNWS_AnimationStateSerializeHandler : public AnimationStateSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationStateSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNAnimationStateSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationStateSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationStateSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationStateSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNAnimationStateSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNAnimationStateSerializeHandlerCallback m_callback;

    LNWS_AnimationStateSerializeHandler()
      : AnimationStateSerializeHandler([this](ln::AnimationState* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNAnimationStateSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_AnimationStateSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNAnimationStateSerializeHandlerCallback callback)
    {
        if (!AnimationStateSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNAnimationStateSerializeHandler_Create(LNAnimationStateSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_AnimationStateSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using EffectResourceSerializeHandler = ln::Delegate<void(ln::EffectResource* self, ln::Serializer2* ar)>;

class LNWS_EffectResourceSerializeHandler : public EffectResourceSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEffectResourceSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNEffectResourceSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEffectResourceSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEffectResourceSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEffectResourceSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNEffectResourceSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNEffectResourceSerializeHandlerCallback m_callback;

    LNWS_EffectResourceSerializeHandler()
      : EffectResourceSerializeHandler([this](ln::EffectResource* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNEffectResourceSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_EffectResourceSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNEffectResourceSerializeHandlerCallback callback)
    {
        if (!EffectResourceSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNEffectResourceSerializeHandler_Create(LNEffectResourceSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_EffectResourceSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ParticleEmitterModelSerializeHandler = ln::Delegate<void(ln::ParticleEmitterModel* self, ln::Serializer2* ar)>;

class LNWS_ParticleEmitterModelSerializeHandler : public ParticleEmitterModelSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleEmitterModelSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNParticleEmitterModelSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleEmitterModelSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleEmitterModelSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleEmitterModelSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNParticleEmitterModelSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNParticleEmitterModelSerializeHandlerCallback m_callback;

    LNWS_ParticleEmitterModelSerializeHandler()
      : ParticleEmitterModelSerializeHandler([this](ln::ParticleEmitterModel* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNParticleEmitterModelSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ParticleEmitterModelSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNParticleEmitterModelSerializeHandlerCallback callback)
    {
        if (!ParticleEmitterModelSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNParticleEmitterModelSerializeHandler_Create(LNParticleEmitterModelSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ParticleEmitterModelSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ParticleModelSerializeHandler = ln::Delegate<void(ln::ParticleModel* self, ln::Serializer2* ar)>;

class LNWS_ParticleModelSerializeHandler : public ParticleModelSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleModelSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNParticleModelSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleModelSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleModelSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleModelSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNParticleModelSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNParticleModelSerializeHandlerCallback m_callback;

    LNWS_ParticleModelSerializeHandler()
      : ParticleModelSerializeHandler([this](ln::ParticleModel* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNParticleModelSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ParticleModelSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNParticleModelSerializeHandlerCallback callback)
    {
        if (!ParticleModelSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNParticleModelSerializeHandler_Create(LNParticleModelSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ParticleModelSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ComponentSerializeHandler = ln::Delegate<void(ln::Component* self, ln::Serializer2* ar)>;

class LNWS_ComponentSerializeHandler : public ComponentSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNComponentSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNComponentSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNComponentSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNComponentSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNComponentSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNComponentSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNComponentSerializeHandlerCallback m_callback;

    LNWS_ComponentSerializeHandler()
      : ComponentSerializeHandler([this](ln::Component* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNComponentSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ComponentSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNComponentSerializeHandlerCallback callback)
    {
        if (!ComponentSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNComponentSerializeHandler_Create(LNComponentSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ComponentSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using VisualComponentSerializeHandler = ln::Delegate<void(ln::VisualComponent* self, ln::Serializer2* ar)>;

class LNWS_VisualComponentSerializeHandler : public VisualComponentSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVisualComponentSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNVisualComponentSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVisualComponentSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVisualComponentSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVisualComponentSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNVisualComponentSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNVisualComponentSerializeHandlerCallback m_callback;

    LNWS_VisualComponentSerializeHandler()
      : VisualComponentSerializeHandler([this](ln::VisualComponent* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNVisualComponentSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_VisualComponentSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNVisualComponentSerializeHandlerCallback callback)
    {
        if (!VisualComponentSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNVisualComponentSerializeHandler_Create(LNVisualComponentSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_VisualComponentSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SpriteComponentSerializeHandler = ln::Delegate<void(ln::SpriteComponent* self, ln::Serializer2* ar)>;

class LNWS_SpriteComponentSerializeHandler : public SpriteComponentSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpriteComponentSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSpriteComponentSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpriteComponentSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpriteComponentSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpriteComponentSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNSpriteComponentSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSpriteComponentSerializeHandlerCallback m_callback;

    LNWS_SpriteComponentSerializeHandler()
      : SpriteComponentSerializeHandler([this](ln::SpriteComponent* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNSpriteComponentSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SpriteComponentSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSpriteComponentSerializeHandlerCallback callback)
    {
        if (!SpriteComponentSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNSpriteComponentSerializeHandler_Create(LNSpriteComponentSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SpriteComponentSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using CharacterControllerSerializeHandler = ln::Delegate<void(ln::CharacterController* self, ln::Serializer2* ar)>;

class LNWS_CharacterControllerSerializeHandler : public CharacterControllerSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCharacterControllerSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCharacterControllerSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCharacterControllerSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCharacterControllerSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCharacterControllerSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNCharacterControllerSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCharacterControllerSerializeHandlerCallback m_callback;

    LNWS_CharacterControllerSerializeHandler()
      : CharacterControllerSerializeHandler([this](ln::CharacterController* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNCharacterControllerSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_CharacterControllerSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCharacterControllerSerializeHandlerCallback callback)
    {
        if (!CharacterControllerSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNCharacterControllerSerializeHandler_Create(LNCharacterControllerSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_CharacterControllerSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using WorldSerializeHandler = ln::Delegate<void(ln::World* self, ln::Serializer2* ar)>;

class LNWS_WorldSerializeHandler : public WorldSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorldSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNWorldSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorldSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorldSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorldSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNWorldSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWorldSerializeHandlerCallback m_callback;

    LNWS_WorldSerializeHandler()
      : WorldSerializeHandler([this](ln::World* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNWorldSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_WorldSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNWorldSerializeHandlerCallback callback)
    {
        if (!WorldSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNWorldSerializeHandler_Create(LNWorldSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_WorldSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ComponentListSerializeHandler = ln::Delegate<void(ln::ComponentList* self, ln::Serializer2* ar)>;

class LNWS_ComponentListSerializeHandler : public ComponentListSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNComponentListSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNComponentListSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNComponentListSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNComponentListSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNComponentListSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNComponentListSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNComponentListSerializeHandlerCallback m_callback;

    LNWS_ComponentListSerializeHandler()
      : ComponentListSerializeHandler([this](ln::ComponentList* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNComponentListSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ComponentListSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNComponentListSerializeHandlerCallback callback)
    {
        if (!ComponentListSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNComponentListSerializeHandler_Create(LNComponentListSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ComponentListSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using WorldObjectSerializeHandler = ln::Delegate<void(ln::WorldObject* self, ln::Serializer2* ar)>;

class LNWS_WorldObjectSerializeHandler : public WorldObjectSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorldObjectSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNWorldObjectSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorldObjectSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorldObjectSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorldObjectSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNWorldObjectSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWorldObjectSerializeHandlerCallback m_callback;

    LNWS_WorldObjectSerializeHandler()
      : WorldObjectSerializeHandler([this](ln::WorldObject* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNWorldObjectSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_WorldObjectSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNWorldObjectSerializeHandlerCallback callback)
    {
        if (!WorldObjectSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNWorldObjectSerializeHandler_Create(LNWorldObjectSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_WorldObjectSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using WorldObjectPreUpdateHandler = ln::Delegate<void(ln::WorldObject* self)>;

class LNWS_WorldObjectPreUpdateHandler : public WorldObjectPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorldObjectPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNWorldObjectPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorldObjectPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorldObjectPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorldObjectPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNWorldObjectPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWorldObjectPreUpdateHandlerCallback m_callback;

    LNWS_WorldObjectPreUpdateHandler()
      : WorldObjectPreUpdateHandler([this](ln::WorldObject* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNWorldObjectPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_WorldObjectPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNWorldObjectPreUpdateHandlerCallback callback)
    {
        if (!WorldObjectPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNWorldObjectPreUpdateHandler_Create(LNWorldObjectPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_WorldObjectPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using WorldObjectUpdateHandler = ln::Delegate<void(ln::WorldObject* self, float elapsedSeconds)>;

class LNWS_WorldObjectUpdateHandler : public WorldObjectUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorldObjectUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNWorldObjectUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorldObjectUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorldObjectUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorldObjectUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNWorldObjectUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWorldObjectUpdateHandlerCallback m_callback;

    LNWS_WorldObjectUpdateHandler()
      : WorldObjectUpdateHandler([this](ln::WorldObject* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNWorldObjectUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_WorldObjectUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNWorldObjectUpdateHandlerCallback callback)
    {
        if (!WorldObjectUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNWorldObjectUpdateHandler_Create(LNWorldObjectUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_WorldObjectUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using VisualObjectSerializeHandler = ln::Delegate<void(ln::VisualObject* self, ln::Serializer2* ar)>;

class LNWS_VisualObjectSerializeHandler : public VisualObjectSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVisualObjectSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNVisualObjectSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVisualObjectSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVisualObjectSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVisualObjectSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNVisualObjectSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNVisualObjectSerializeHandlerCallback m_callback;

    LNWS_VisualObjectSerializeHandler()
      : VisualObjectSerializeHandler([this](ln::VisualObject* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNVisualObjectSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_VisualObjectSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNVisualObjectSerializeHandlerCallback callback)
    {
        if (!VisualObjectSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNVisualObjectSerializeHandler_Create(LNVisualObjectSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_VisualObjectSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using VisualObjectPreUpdateHandler = ln::Delegate<void(ln::VisualObject* self)>;

class LNWS_VisualObjectPreUpdateHandler : public VisualObjectPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVisualObjectPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNVisualObjectPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVisualObjectPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVisualObjectPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVisualObjectPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNVisualObjectPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNVisualObjectPreUpdateHandlerCallback m_callback;

    LNWS_VisualObjectPreUpdateHandler()
      : VisualObjectPreUpdateHandler([this](ln::VisualObject* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNVisualObjectPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_VisualObjectPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNVisualObjectPreUpdateHandlerCallback callback)
    {
        if (!VisualObjectPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNVisualObjectPreUpdateHandler_Create(LNVisualObjectPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_VisualObjectPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using VisualObjectUpdateHandler = ln::Delegate<void(ln::VisualObject* self, float elapsedSeconds)>;

class LNWS_VisualObjectUpdateHandler : public VisualObjectUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVisualObjectUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNVisualObjectUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVisualObjectUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVisualObjectUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVisualObjectUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNVisualObjectUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNVisualObjectUpdateHandlerCallback m_callback;

    LNWS_VisualObjectUpdateHandler()
      : VisualObjectUpdateHandler([this](ln::VisualObject* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNVisualObjectUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_VisualObjectUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNVisualObjectUpdateHandlerCallback callback)
    {
        if (!VisualObjectUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNVisualObjectUpdateHandler_Create(LNVisualObjectUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_VisualObjectUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using CameraSerializeHandler = ln::Delegate<void(ln::Camera* self, ln::Serializer2* ar)>;

class LNWS_CameraSerializeHandler : public CameraSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCameraSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCameraSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCameraSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCameraSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCameraSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNCameraSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCameraSerializeHandlerCallback m_callback;

    LNWS_CameraSerializeHandler()
      : CameraSerializeHandler([this](ln::Camera* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNCameraSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_CameraSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCameraSerializeHandlerCallback callback)
    {
        if (!CameraSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNCameraSerializeHandler_Create(LNCameraSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_CameraSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using CameraPreUpdateHandler = ln::Delegate<void(ln::Camera* self)>;

class LNWS_CameraPreUpdateHandler : public CameraPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCameraPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCameraPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCameraPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCameraPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCameraPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNCameraPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCameraPreUpdateHandlerCallback m_callback;

    LNWS_CameraPreUpdateHandler()
      : CameraPreUpdateHandler([this](ln::Camera* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNCameraPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_CameraPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCameraPreUpdateHandlerCallback callback)
    {
        if (!CameraPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNCameraPreUpdateHandler_Create(LNCameraPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_CameraPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using CameraUpdateHandler = ln::Delegate<void(ln::Camera* self, float elapsedSeconds)>;

class LNWS_CameraUpdateHandler : public CameraUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCameraUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCameraUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCameraUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCameraUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCameraUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNCameraUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCameraUpdateHandlerCallback m_callback;

    LNWS_CameraUpdateHandler()
      : CameraUpdateHandler([this](ln::Camera* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNCameraUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_CameraUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCameraUpdateHandlerCallback callback)
    {
        if (!CameraUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNCameraUpdateHandler_Create(LNCameraUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_CameraUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using EnvironmentLightSerializeHandler = ln::Delegate<void(ln::EnvironmentLight* self, ln::Serializer2* ar)>;

class LNWS_EnvironmentLightSerializeHandler : public EnvironmentLightSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEnvironmentLightSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNEnvironmentLightSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEnvironmentLightSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEnvironmentLightSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNEnvironmentLightSerializeHandlerCallback m_callback;

    LNWS_EnvironmentLightSerializeHandler()
      : EnvironmentLightSerializeHandler([this](ln::EnvironmentLight* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNEnvironmentLightSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_EnvironmentLightSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNEnvironmentLightSerializeHandlerCallback callback)
    {
        if (!EnvironmentLightSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNEnvironmentLightSerializeHandler_Create(LNEnvironmentLightSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_EnvironmentLightSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using EnvironmentLightPreUpdateHandler = ln::Delegate<void(ln::EnvironmentLight* self)>;

class LNWS_EnvironmentLightPreUpdateHandler : public EnvironmentLightPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEnvironmentLightPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNEnvironmentLightPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEnvironmentLightPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEnvironmentLightPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEnvironmentLightPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNEnvironmentLightPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNEnvironmentLightPreUpdateHandlerCallback m_callback;

    LNWS_EnvironmentLightPreUpdateHandler()
      : EnvironmentLightPreUpdateHandler([this](ln::EnvironmentLight* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNEnvironmentLightPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_EnvironmentLightPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNEnvironmentLightPreUpdateHandlerCallback callback)
    {
        if (!EnvironmentLightPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNEnvironmentLightPreUpdateHandler_Create(LNEnvironmentLightPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_EnvironmentLightPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using EnvironmentLightUpdateHandler = ln::Delegate<void(ln::EnvironmentLight* self, float elapsedSeconds)>;

class LNWS_EnvironmentLightUpdateHandler : public EnvironmentLightUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEnvironmentLightUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNEnvironmentLightUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEnvironmentLightUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEnvironmentLightUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNEnvironmentLightUpdateHandlerCallback m_callback;

    LNWS_EnvironmentLightUpdateHandler()
      : EnvironmentLightUpdateHandler([this](ln::EnvironmentLight* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNEnvironmentLightUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_EnvironmentLightUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNEnvironmentLightUpdateHandlerCallback callback)
    {
        if (!EnvironmentLightUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNEnvironmentLightUpdateHandler_Create(LNEnvironmentLightUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_EnvironmentLightUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using DirectionalLightSerializeHandler = ln::Delegate<void(ln::DirectionalLight* self, ln::Serializer2* ar)>;

class LNWS_DirectionalLightSerializeHandler : public DirectionalLightSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNDirectionalLightSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNDirectionalLightSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNDirectionalLightSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNDirectionalLightSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNDirectionalLightSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNDirectionalLightSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNDirectionalLightSerializeHandlerCallback m_callback;

    LNWS_DirectionalLightSerializeHandler()
      : DirectionalLightSerializeHandler([this](ln::DirectionalLight* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNDirectionalLightSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_DirectionalLightSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNDirectionalLightSerializeHandlerCallback callback)
    {
        if (!DirectionalLightSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNDirectionalLightSerializeHandler_Create(LNDirectionalLightSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_DirectionalLightSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using DirectionalLightPreUpdateHandler = ln::Delegate<void(ln::DirectionalLight* self)>;

class LNWS_DirectionalLightPreUpdateHandler : public DirectionalLightPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNDirectionalLightPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNDirectionalLightPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNDirectionalLightPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNDirectionalLightPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNDirectionalLightPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNDirectionalLightPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNDirectionalLightPreUpdateHandlerCallback m_callback;

    LNWS_DirectionalLightPreUpdateHandler()
      : DirectionalLightPreUpdateHandler([this](ln::DirectionalLight* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNDirectionalLightPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_DirectionalLightPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNDirectionalLightPreUpdateHandlerCallback callback)
    {
        if (!DirectionalLightPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNDirectionalLightPreUpdateHandler_Create(LNDirectionalLightPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_DirectionalLightPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using DirectionalLightUpdateHandler = ln::Delegate<void(ln::DirectionalLight* self, float elapsedSeconds)>;

class LNWS_DirectionalLightUpdateHandler : public DirectionalLightUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNDirectionalLightUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNDirectionalLightUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNDirectionalLightUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNDirectionalLightUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNDirectionalLightUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNDirectionalLightUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNDirectionalLightUpdateHandlerCallback m_callback;

    LNWS_DirectionalLightUpdateHandler()
      : DirectionalLightUpdateHandler([this](ln::DirectionalLight* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNDirectionalLightUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_DirectionalLightUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNDirectionalLightUpdateHandlerCallback callback)
    {
        if (!DirectionalLightUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNDirectionalLightUpdateHandler_Create(LNDirectionalLightUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_DirectionalLightUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using PointLightSerializeHandler = ln::Delegate<void(ln::PointLight* self, ln::Serializer2* ar)>;

class LNWS_PointLightSerializeHandler : public PointLightSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPointLightSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNPointLightSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPointLightSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPointLightSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPointLightSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNPointLightSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNPointLightSerializeHandlerCallback m_callback;

    LNWS_PointLightSerializeHandler()
      : PointLightSerializeHandler([this](ln::PointLight* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNPointLightSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_PointLightSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNPointLightSerializeHandlerCallback callback)
    {
        if (!PointLightSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNPointLightSerializeHandler_Create(LNPointLightSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_PointLightSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using PointLightPreUpdateHandler = ln::Delegate<void(ln::PointLight* self)>;

class LNWS_PointLightPreUpdateHandler : public PointLightPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPointLightPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNPointLightPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPointLightPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPointLightPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPointLightPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNPointLightPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNPointLightPreUpdateHandlerCallback m_callback;

    LNWS_PointLightPreUpdateHandler()
      : PointLightPreUpdateHandler([this](ln::PointLight* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNPointLightPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_PointLightPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNPointLightPreUpdateHandlerCallback callback)
    {
        if (!PointLightPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNPointLightPreUpdateHandler_Create(LNPointLightPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_PointLightPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using PointLightUpdateHandler = ln::Delegate<void(ln::PointLight* self, float elapsedSeconds)>;

class LNWS_PointLightUpdateHandler : public PointLightUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPointLightUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNPointLightUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPointLightUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPointLightUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPointLightUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNPointLightUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNPointLightUpdateHandlerCallback m_callback;

    LNWS_PointLightUpdateHandler()
      : PointLightUpdateHandler([this](ln::PointLight* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNPointLightUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_PointLightUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNPointLightUpdateHandlerCallback callback)
    {
        if (!PointLightUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNPointLightUpdateHandler_Create(LNPointLightUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_PointLightUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SpotLightSerializeHandler = ln::Delegate<void(ln::SpotLight* self, ln::Serializer2* ar)>;

class LNWS_SpotLightSerializeHandler : public SpotLightSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpotLightSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSpotLightSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpotLightSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpotLightSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpotLightSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNSpotLightSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSpotLightSerializeHandlerCallback m_callback;

    LNWS_SpotLightSerializeHandler()
      : SpotLightSerializeHandler([this](ln::SpotLight* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNSpotLightSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SpotLightSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSpotLightSerializeHandlerCallback callback)
    {
        if (!SpotLightSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNSpotLightSerializeHandler_Create(LNSpotLightSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SpotLightSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SpotLightPreUpdateHandler = ln::Delegate<void(ln::SpotLight* self)>;

class LNWS_SpotLightPreUpdateHandler : public SpotLightPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpotLightPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSpotLightPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpotLightPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpotLightPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpotLightPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNSpotLightPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSpotLightPreUpdateHandlerCallback m_callback;

    LNWS_SpotLightPreUpdateHandler()
      : SpotLightPreUpdateHandler([this](ln::SpotLight* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNSpotLightPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SpotLightPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSpotLightPreUpdateHandlerCallback callback)
    {
        if (!SpotLightPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNSpotLightPreUpdateHandler_Create(LNSpotLightPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SpotLightPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SpotLightUpdateHandler = ln::Delegate<void(ln::SpotLight* self, float elapsedSeconds)>;

class LNWS_SpotLightUpdateHandler : public SpotLightUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpotLightUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSpotLightUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpotLightUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpotLightUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpotLightUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNSpotLightUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSpotLightUpdateHandlerCallback m_callback;

    LNWS_SpotLightUpdateHandler()
      : SpotLightUpdateHandler([this](ln::SpotLight* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNSpotLightUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SpotLightUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSpotLightUpdateHandlerCallback callback)
    {
        if (!SpotLightUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNSpotLightUpdateHandler_Create(LNSpotLightUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SpotLightUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SpriteSerializeHandler = ln::Delegate<void(ln::Sprite* self, ln::Serializer2* ar)>;

class LNWS_SpriteSerializeHandler : public SpriteSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpriteSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSpriteSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpriteSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpriteSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpriteSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNSpriteSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSpriteSerializeHandlerCallback m_callback;

    LNWS_SpriteSerializeHandler()
      : SpriteSerializeHandler([this](ln::Sprite* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNSpriteSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SpriteSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSpriteSerializeHandlerCallback callback)
    {
        if (!SpriteSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNSpriteSerializeHandler_Create(LNSpriteSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SpriteSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SpritePreUpdateHandler = ln::Delegate<void(ln::Sprite* self)>;

class LNWS_SpritePreUpdateHandler : public SpritePreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpritePreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSpritePreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpritePreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpritePreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpritePreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNSpritePreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSpritePreUpdateHandlerCallback m_callback;

    LNWS_SpritePreUpdateHandler()
      : SpritePreUpdateHandler([this](ln::Sprite* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNSpritePreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SpritePreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSpritePreUpdateHandlerCallback callback)
    {
        if (!SpritePreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNSpritePreUpdateHandler_Create(LNSpritePreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SpritePreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using SpriteUpdateHandler = ln::Delegate<void(ln::Sprite* self, float elapsedSeconds)>;

class LNWS_SpriteUpdateHandler : public SpriteUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpriteUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNSpriteUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpriteUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpriteUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpriteUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNSpriteUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNSpriteUpdateHandlerCallback m_callback;

    LNWS_SpriteUpdateHandler()
      : SpriteUpdateHandler([this](ln::Sprite* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNSpriteUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_SpriteUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNSpriteUpdateHandlerCallback callback)
    {
        if (!SpriteUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNSpriteUpdateHandler_Create(LNSpriteUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_SpriteUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using CameraOrbitControlComponentSerializeHandler = ln::Delegate<void(ln::CameraOrbitControlComponent* self, ln::Serializer2* ar)>;

class LNWS_CameraOrbitControlComponentSerializeHandler : public CameraOrbitControlComponentSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCameraOrbitControlComponentSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCameraOrbitControlComponentSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCameraOrbitControlComponentSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCameraOrbitControlComponentSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCameraOrbitControlComponentSerializeHandlerCallback m_callback;

    LNWS_CameraOrbitControlComponentSerializeHandler()
      : CameraOrbitControlComponentSerializeHandler([this](ln::CameraOrbitControlComponent* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNCameraOrbitControlComponentSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_CameraOrbitControlComponentSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCameraOrbitControlComponentSerializeHandlerCallback callback)
    {
        if (!CameraOrbitControlComponentSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNCameraOrbitControlComponentSerializeHandler_Create(LNCameraOrbitControlComponentSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_CameraOrbitControlComponentSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using RaycasterSerializeHandler = ln::Delegate<void(ln::Raycaster* self, ln::Serializer2* ar)>;

class LNWS_RaycasterSerializeHandler : public RaycasterSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNRaycasterSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNRaycasterSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNRaycasterSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNRaycasterSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNRaycasterSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNRaycasterSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNRaycasterSerializeHandlerCallback m_callback;

    LNWS_RaycasterSerializeHandler()
      : RaycasterSerializeHandler([this](ln::Raycaster* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNRaycasterSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_RaycasterSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNRaycasterSerializeHandlerCallback callback)
    {
        if (!RaycasterSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNRaycasterSerializeHandler_Create(LNRaycasterSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_RaycasterSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using RaycastResultSerializeHandler = ln::Delegate<void(ln::RaycastResult* self, ln::Serializer2* ar)>;

class LNWS_RaycastResultSerializeHandler : public RaycastResultSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNRaycastResultSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNRaycastResultSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNRaycastResultSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNRaycastResultSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNRaycastResultSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNRaycastResultSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNRaycastResultSerializeHandlerCallback m_callback;

    LNWS_RaycastResultSerializeHandler()
      : RaycastResultSerializeHandler([this](ln::RaycastResult* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNRaycastResultSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_RaycastResultSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNRaycastResultSerializeHandlerCallback callback)
    {
        if (!RaycastResultSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNRaycastResultSerializeHandler_Create(LNRaycastResultSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_RaycastResultSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using WorldRenderViewSerializeHandler = ln::Delegate<void(ln::WorldRenderView* self, ln::Serializer2* ar)>;

class LNWS_WorldRenderViewSerializeHandler : public WorldRenderViewSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorldRenderViewSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNWorldRenderViewSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorldRenderViewSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorldRenderViewSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWorldRenderViewSerializeHandlerCallback m_callback;

    LNWS_WorldRenderViewSerializeHandler()
      : WorldRenderViewSerializeHandler([this](ln::WorldRenderView* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNWorldRenderViewSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_WorldRenderViewSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNWorldRenderViewSerializeHandlerCallback callback)
    {
        if (!WorldRenderViewSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNWorldRenderViewSerializeHandler_Create(LNWorldRenderViewSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_WorldRenderViewSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ShapeObjectSerializeHandler = ln::Delegate<void(ln::ShapeObject* self, ln::Serializer2* ar)>;

class LNWS_ShapeObjectSerializeHandler : public ShapeObjectSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNShapeObjectSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNShapeObjectSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNShapeObjectSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNShapeObjectSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNShapeObjectSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNShapeObjectSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNShapeObjectSerializeHandlerCallback m_callback;

    LNWS_ShapeObjectSerializeHandler()
      : ShapeObjectSerializeHandler([this](ln::ShapeObject* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNShapeObjectSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ShapeObjectSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNShapeObjectSerializeHandlerCallback callback)
    {
        if (!ShapeObjectSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNShapeObjectSerializeHandler_Create(LNShapeObjectSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ShapeObjectSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ShapeObjectPreUpdateHandler = ln::Delegate<void(ln::ShapeObject* self)>;

class LNWS_ShapeObjectPreUpdateHandler : public ShapeObjectPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNShapeObjectPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNShapeObjectPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNShapeObjectPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNShapeObjectPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNShapeObjectPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNShapeObjectPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNShapeObjectPreUpdateHandlerCallback m_callback;

    LNWS_ShapeObjectPreUpdateHandler()
      : ShapeObjectPreUpdateHandler([this](ln::ShapeObject* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNShapeObjectPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ShapeObjectPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNShapeObjectPreUpdateHandlerCallback callback)
    {
        if (!ShapeObjectPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNShapeObjectPreUpdateHandler_Create(LNShapeObjectPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ShapeObjectPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ShapeObjectUpdateHandler = ln::Delegate<void(ln::ShapeObject* self, float elapsedSeconds)>;

class LNWS_ShapeObjectUpdateHandler : public ShapeObjectUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNShapeObjectUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNShapeObjectUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNShapeObjectUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNShapeObjectUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNShapeObjectUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNShapeObjectUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNShapeObjectUpdateHandlerCallback m_callback;

    LNWS_ShapeObjectUpdateHandler()
      : ShapeObjectUpdateHandler([this](ln::ShapeObject* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNShapeObjectUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ShapeObjectUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNShapeObjectUpdateHandlerCallback callback)
    {
        if (!ShapeObjectUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNShapeObjectUpdateHandler_Create(LNShapeObjectUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ShapeObjectUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using PlaneMeshSerializeHandler = ln::Delegate<void(ln::PlaneMesh* self, ln::Serializer2* ar)>;

class LNWS_PlaneMeshSerializeHandler : public PlaneMeshSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPlaneMeshSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNPlaneMeshSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPlaneMeshSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPlaneMeshSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPlaneMeshSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNPlaneMeshSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNPlaneMeshSerializeHandlerCallback m_callback;

    LNWS_PlaneMeshSerializeHandler()
      : PlaneMeshSerializeHandler([this](ln::PlaneMesh* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNPlaneMeshSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_PlaneMeshSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNPlaneMeshSerializeHandlerCallback callback)
    {
        if (!PlaneMeshSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNPlaneMeshSerializeHandler_Create(LNPlaneMeshSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_PlaneMeshSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using PlaneMeshPreUpdateHandler = ln::Delegate<void(ln::PlaneMesh* self)>;

class LNWS_PlaneMeshPreUpdateHandler : public PlaneMeshPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPlaneMeshPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNPlaneMeshPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPlaneMeshPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPlaneMeshPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPlaneMeshPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNPlaneMeshPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNPlaneMeshPreUpdateHandlerCallback m_callback;

    LNWS_PlaneMeshPreUpdateHandler()
      : PlaneMeshPreUpdateHandler([this](ln::PlaneMesh* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNPlaneMeshPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_PlaneMeshPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNPlaneMeshPreUpdateHandlerCallback callback)
    {
        if (!PlaneMeshPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNPlaneMeshPreUpdateHandler_Create(LNPlaneMeshPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_PlaneMeshPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using PlaneMeshUpdateHandler = ln::Delegate<void(ln::PlaneMesh* self, float elapsedSeconds)>;

class LNWS_PlaneMeshUpdateHandler : public PlaneMeshUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPlaneMeshUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNPlaneMeshUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPlaneMeshUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPlaneMeshUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPlaneMeshUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNPlaneMeshUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNPlaneMeshUpdateHandlerCallback m_callback;

    LNWS_PlaneMeshUpdateHandler()
      : PlaneMeshUpdateHandler([this](ln::PlaneMesh* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNPlaneMeshUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_PlaneMeshUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNPlaneMeshUpdateHandlerCallback callback)
    {
        if (!PlaneMeshUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNPlaneMeshUpdateHandler_Create(LNPlaneMeshUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_PlaneMeshUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using BoxMeshSerializeHandler = ln::Delegate<void(ln::BoxMesh* self, ln::Serializer2* ar)>;

class LNWS_BoxMeshSerializeHandler : public BoxMeshSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNBoxMeshSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNBoxMeshSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNBoxMeshSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNBoxMeshSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNBoxMeshSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNBoxMeshSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNBoxMeshSerializeHandlerCallback m_callback;

    LNWS_BoxMeshSerializeHandler()
      : BoxMeshSerializeHandler([this](ln::BoxMesh* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNBoxMeshSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_BoxMeshSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNBoxMeshSerializeHandlerCallback callback)
    {
        if (!BoxMeshSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNBoxMeshSerializeHandler_Create(LNBoxMeshSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_BoxMeshSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using BoxMeshPreUpdateHandler = ln::Delegate<void(ln::BoxMesh* self)>;

class LNWS_BoxMeshPreUpdateHandler : public BoxMeshPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNBoxMeshPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNBoxMeshPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNBoxMeshPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNBoxMeshPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNBoxMeshPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNBoxMeshPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNBoxMeshPreUpdateHandlerCallback m_callback;

    LNWS_BoxMeshPreUpdateHandler()
      : BoxMeshPreUpdateHandler([this](ln::BoxMesh* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNBoxMeshPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_BoxMeshPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNBoxMeshPreUpdateHandlerCallback callback)
    {
        if (!BoxMeshPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNBoxMeshPreUpdateHandler_Create(LNBoxMeshPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_BoxMeshPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using BoxMeshUpdateHandler = ln::Delegate<void(ln::BoxMesh* self, float elapsedSeconds)>;

class LNWS_BoxMeshUpdateHandler : public BoxMeshUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNBoxMeshUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNBoxMeshUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNBoxMeshUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNBoxMeshUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNBoxMeshUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNBoxMeshUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNBoxMeshUpdateHandlerCallback m_callback;

    LNWS_BoxMeshUpdateHandler()
      : BoxMeshUpdateHandler([this](ln::BoxMesh* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNBoxMeshUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_BoxMeshUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNBoxMeshUpdateHandlerCallback callback)
    {
        if (!BoxMeshUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNBoxMeshUpdateHandler_Create(LNBoxMeshUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_BoxMeshUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using MeshComponentSerializeHandler = ln::Delegate<void(ln::MeshComponent* self, ln::Serializer2* ar)>;

class LNWS_MeshComponentSerializeHandler : public MeshComponentSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshComponentSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNMeshComponentSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshComponentSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshComponentSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshComponentSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNMeshComponentSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNMeshComponentSerializeHandlerCallback m_callback;

    LNWS_MeshComponentSerializeHandler()
      : MeshComponentSerializeHandler([this](ln::MeshComponent* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNMeshComponentSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_MeshComponentSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNMeshComponentSerializeHandlerCallback callback)
    {
        if (!MeshComponentSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNMeshComponentSerializeHandler_Create(LNMeshComponentSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_MeshComponentSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using CollisionSerializeHandler = ln::Delegate<void(ln::Collision* self, ln::Serializer2* ar)>;

class LNWS_CollisionSerializeHandler : public CollisionSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCollisionSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCollisionSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCollisionSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCollisionSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCollisionSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNCollisionSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCollisionSerializeHandlerCallback m_callback;

    LNWS_CollisionSerializeHandler()
      : CollisionSerializeHandler([this](ln::Collision* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNCollisionSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_CollisionSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCollisionSerializeHandlerCallback callback)
    {
        if (!CollisionSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNCollisionSerializeHandler_Create(LNCollisionSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_CollisionSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using TriggerBodyComponentSerializeHandler = ln::Delegate<void(ln::TriggerBodyComponent* self, ln::Serializer2* ar)>;

class LNWS_TriggerBodyComponentSerializeHandler : public TriggerBodyComponentSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTriggerBodyComponentSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNTriggerBodyComponentSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTriggerBodyComponentSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTriggerBodyComponentSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTriggerBodyComponentSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNTriggerBodyComponentSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNTriggerBodyComponentSerializeHandlerCallback m_callback;

    LNWS_TriggerBodyComponentSerializeHandler()
      : TriggerBodyComponentSerializeHandler([this](ln::TriggerBodyComponent* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNTriggerBodyComponentSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_TriggerBodyComponentSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNTriggerBodyComponentSerializeHandlerCallback callback)
    {
        if (!TriggerBodyComponentSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNTriggerBodyComponentSerializeHandler_Create(LNTriggerBodyComponentSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_TriggerBodyComponentSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ParticleEmitterSerializeHandler = ln::Delegate<void(ln::ParticleEmitter* self, ln::Serializer2* ar)>;

class LNWS_ParticleEmitterSerializeHandler : public ParticleEmitterSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleEmitterSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNParticleEmitterSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleEmitterSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleEmitterSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleEmitterSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNParticleEmitterSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNParticleEmitterSerializeHandlerCallback m_callback;

    LNWS_ParticleEmitterSerializeHandler()
      : ParticleEmitterSerializeHandler([this](ln::ParticleEmitter* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNParticleEmitterSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ParticleEmitterSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNParticleEmitterSerializeHandlerCallback callback)
    {
        if (!ParticleEmitterSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNParticleEmitterSerializeHandler_Create(LNParticleEmitterSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ParticleEmitterSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ParticleEmitterPreUpdateHandler = ln::Delegate<void(ln::ParticleEmitter* self)>;

class LNWS_ParticleEmitterPreUpdateHandler : public ParticleEmitterPreUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleEmitterPreUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNParticleEmitterPreUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleEmitterPreUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleEmitterPreUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleEmitterPreUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNParticleEmitterPreUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNParticleEmitterPreUpdateHandlerCallback m_callback;

    LNWS_ParticleEmitterPreUpdateHandler()
      : ParticleEmitterPreUpdateHandler([this](ln::ParticleEmitter* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNParticleEmitterPreUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ParticleEmitterPreUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNParticleEmitterPreUpdateHandlerCallback callback)
    {
        if (!ParticleEmitterPreUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNParticleEmitterPreUpdateHandler_Create(LNParticleEmitterPreUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ParticleEmitterPreUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ParticleEmitterUpdateHandler = ln::Delegate<void(ln::ParticleEmitter* self, float elapsedSeconds)>;

class LNWS_ParticleEmitterUpdateHandler : public ParticleEmitterUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleEmitterUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNParticleEmitterUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleEmitterUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleEmitterUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleEmitterUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNParticleEmitterUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNParticleEmitterUpdateHandlerCallback m_callback;

    LNWS_ParticleEmitterUpdateHandler()
      : ParticleEmitterUpdateHandler([this](ln::ParticleEmitter* self, float elapsedSeconds) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), elapsedSeconds);
        if (r != LN_OK) { LN_ERROR("LNParticleEmitterUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ParticleEmitterUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNParticleEmitterUpdateHandlerCallback callback)
    {
        if (!ParticleEmitterUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNParticleEmitterUpdateHandler_Create(LNParticleEmitterUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ParticleEmitterUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using LevelSerializeHandler = ln::Delegate<void(ln::Level* self, ln::Serializer2* ar)>;

class LNWS_LevelSerializeHandler : public LevelSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNLevelSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNLevelSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNLevelSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNLevelSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNLevelSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNLevelSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNLevelSerializeHandlerCallback m_callback;

    LNWS_LevelSerializeHandler()
      : LevelSerializeHandler([this](ln::Level* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNLevelSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_LevelSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNLevelSerializeHandlerCallback callback)
    {
        if (!LevelSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNLevelSerializeHandler_Create(LNLevelSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_LevelSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using LevelStartHandler = ln::Delegate<void(ln::Level* self)>;

class LNWS_LevelStartHandler : public LevelStartHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNLevelStartHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNLevelStartHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNLevelStartHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNLevelStartHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNLevelStartHandler_SubclassRegistrationInfo* subclassInfo() { static LNLevelStartHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNLevelStartHandlerCallback m_callback;

    LNWS_LevelStartHandler()
      : LevelStartHandler([this](ln::Level* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNLevelStartHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_LevelStartHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNLevelStartHandlerCallback callback)
    {
        if (!LevelStartHandler::init()) return false;
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


LN_FLAT_API LNResult LNLevelStartHandler_Create(LNLevelStartHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_LevelStartHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using LevelStopHandler = ln::Delegate<void(ln::Level* self)>;

class LNWS_LevelStopHandler : public LevelStopHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNLevelStopHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNLevelStopHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNLevelStopHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNLevelStopHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNLevelStopHandler_SubclassRegistrationInfo* subclassInfo() { static LNLevelStopHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNLevelStopHandlerCallback m_callback;

    LNWS_LevelStopHandler()
      : LevelStopHandler([this](ln::Level* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNLevelStopHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_LevelStopHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNLevelStopHandlerCallback callback)
    {
        if (!LevelStopHandler::init()) return false;
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


LN_FLAT_API LNResult LNLevelStopHandler_Create(LNLevelStopHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_LevelStopHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using LevelPauseHandler = ln::Delegate<void(ln::Level* self)>;

class LNWS_LevelPauseHandler : public LevelPauseHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNLevelPauseHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNLevelPauseHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNLevelPauseHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNLevelPauseHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNLevelPauseHandler_SubclassRegistrationInfo* subclassInfo() { static LNLevelPauseHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNLevelPauseHandlerCallback m_callback;

    LNWS_LevelPauseHandler()
      : LevelPauseHandler([this](ln::Level* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNLevelPauseHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_LevelPauseHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNLevelPauseHandlerCallback callback)
    {
        if (!LevelPauseHandler::init()) return false;
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


LN_FLAT_API LNResult LNLevelPauseHandler_Create(LNLevelPauseHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_LevelPauseHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using LevelResumeHandler = ln::Delegate<void(ln::Level* self)>;

class LNWS_LevelResumeHandler : public LevelResumeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNLevelResumeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNLevelResumeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNLevelResumeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNLevelResumeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNLevelResumeHandler_SubclassRegistrationInfo* subclassInfo() { static LNLevelResumeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNLevelResumeHandlerCallback m_callback;

    LNWS_LevelResumeHandler()
      : LevelResumeHandler([this](ln::Level* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNLevelResumeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_LevelResumeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNLevelResumeHandlerCallback callback)
    {
        if (!LevelResumeHandler::init()) return false;
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


LN_FLAT_API LNResult LNLevelResumeHandler_Create(LNLevelResumeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_LevelResumeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using LevelUpdateHandler = ln::Delegate<void(ln::Level* self)>;

class LNWS_LevelUpdateHandler : public LevelUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNLevelUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNLevelUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNLevelUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNLevelUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNLevelUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNLevelUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNLevelUpdateHandlerCallback m_callback;

    LNWS_LevelUpdateHandler()
      : LevelUpdateHandler([this](ln::Level* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNLevelUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_LevelUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNLevelUpdateHandlerCallback callback)
    {
        if (!LevelUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNLevelUpdateHandler_Create(LNLevelUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_LevelUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIEventArgsSerializeHandler = ln::Delegate<void(ln::UIEventArgs* self, ln::Serializer2* ar)>;

class LNWS_UIEventArgsSerializeHandler : public UIEventArgsSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIEventArgsSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIEventArgsSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIEventArgsSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIEventArgsSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIEventArgsSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIEventArgsSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIEventArgsSerializeHandlerCallback m_callback;

    LNWS_UIEventArgsSerializeHandler()
      : UIEventArgsSerializeHandler([this](ln::UIEventArgs* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIEventArgsSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIEventArgsSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIEventArgsSerializeHandlerCallback callback)
    {
        if (!UIEventArgsSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIEventArgsSerializeHandler_Create(LNUIEventArgsSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIEventArgsSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UILayoutElementSerializeHandler = ln::Delegate<void(ln::UILayoutElement* self, ln::Serializer2* ar)>;

class LNWS_UILayoutElementSerializeHandler : public UILayoutElementSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUILayoutElementSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUILayoutElementSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUILayoutElementSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUILayoutElementSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUILayoutElementSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUILayoutElementSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUILayoutElementSerializeHandlerCallback m_callback;

    LNWS_UILayoutElementSerializeHandler()
      : UILayoutElementSerializeHandler([this](ln::UILayoutElement* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUILayoutElementSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UILayoutElementSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUILayoutElementSerializeHandlerCallback callback)
    {
        if (!UILayoutElementSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUILayoutElementSerializeHandler_Create(LNUILayoutElementSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UILayoutElementSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIElementSerializeHandler = ln::Delegate<void(ln::UIElement* self, ln::Serializer2* ar)>;

class LNWS_UIElementSerializeHandler : public UIElementSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIElementSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIElementSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIElementSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIElementSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIElementSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIElementSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIElementSerializeHandlerCallback m_callback;

    LNWS_UIElementSerializeHandler()
      : UIElementSerializeHandler([this](ln::UIElement* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIElementSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIElementSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIElementSerializeHandlerCallback callback)
    {
        if (!UIElementSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIElementSerializeHandler_Create(LNUIElementSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIElementSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UITextSerializeHandler = ln::Delegate<void(ln::UIText* self, ln::Serializer2* ar)>;

class LNWS_UITextSerializeHandler : public UITextSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUITextSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUITextSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUITextSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUITextSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUITextSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUITextSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUITextSerializeHandlerCallback m_callback;

    LNWS_UITextSerializeHandler()
      : UITextSerializeHandler([this](ln::UIText* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUITextSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UITextSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUITextSerializeHandlerCallback callback)
    {
        if (!UITextSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUITextSerializeHandler_Create(LNUITextSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UITextSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UISpriteSerializeHandler = ln::Delegate<void(ln::UISprite* self, ln::Serializer2* ar)>;

class LNWS_UISpriteSerializeHandler : public UISpriteSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUISpriteSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUISpriteSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUISpriteSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUISpriteSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUISpriteSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUISpriteSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUISpriteSerializeHandlerCallback m_callback;

    LNWS_UISpriteSerializeHandler()
      : UISpriteSerializeHandler([this](ln::UISprite* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUISpriteSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UISpriteSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUISpriteSerializeHandlerCallback callback)
    {
        if (!UISpriteSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUISpriteSerializeHandler_Create(LNUISpriteSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UISpriteSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIIconSerializeHandler = ln::Delegate<void(ln::UIIcon* self, ln::Serializer2* ar)>;

class LNWS_UIIconSerializeHandler : public UIIconSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIIconSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIIconSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIIconSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIIconSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIIconSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIIconSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIIconSerializeHandlerCallback m_callback;

    LNWS_UIIconSerializeHandler()
      : UIIconSerializeHandler([this](ln::UIIcon* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIIconSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIIconSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIIconSerializeHandlerCallback callback)
    {
        if (!UIIconSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIIconSerializeHandler_Create(LNUIIconSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIIconSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIMessageTextAreaSerializeHandler = ln::Delegate<void(ln::UIMessageTextArea* self, ln::Serializer2* ar)>;

class LNWS_UIMessageTextAreaSerializeHandler : public UIMessageTextAreaSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIMessageTextAreaSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIMessageTextAreaSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIMessageTextAreaSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIMessageTextAreaSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIMessageTextAreaSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIMessageTextAreaSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIMessageTextAreaSerializeHandlerCallback m_callback;

    LNWS_UIMessageTextAreaSerializeHandler()
      : UIMessageTextAreaSerializeHandler([this](ln::UIMessageTextArea* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIMessageTextAreaSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIMessageTextAreaSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIMessageTextAreaSerializeHandlerCallback callback)
    {
        if (!UIMessageTextAreaSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIMessageTextAreaSerializeHandler_Create(LNUIMessageTextAreaSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIMessageTextAreaSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UILayoutPanelSerializeHandler = ln::Delegate<void(ln::UILayoutPanel* self, ln::Serializer2* ar)>;

class LNWS_UILayoutPanelSerializeHandler : public UILayoutPanelSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUILayoutPanelSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUILayoutPanelSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUILayoutPanelSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUILayoutPanelSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUILayoutPanelSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUILayoutPanelSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUILayoutPanelSerializeHandlerCallback m_callback;

    LNWS_UILayoutPanelSerializeHandler()
      : UILayoutPanelSerializeHandler([this](ln::UILayoutPanel* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUILayoutPanelSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UILayoutPanelSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUILayoutPanelSerializeHandlerCallback callback)
    {
        if (!UILayoutPanelSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUILayoutPanelSerializeHandler_Create(LNUILayoutPanelSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UILayoutPanelSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIBoxLayoutSerializeHandler = ln::Delegate<void(ln::UIBoxLayout* self, ln::Serializer2* ar)>;

class LNWS_UIBoxLayoutSerializeHandler : public UIBoxLayoutSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIBoxLayoutSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIBoxLayoutSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIBoxLayoutSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIBoxLayoutSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIBoxLayoutSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIBoxLayoutSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIBoxLayoutSerializeHandlerCallback m_callback;

    LNWS_UIBoxLayoutSerializeHandler()
      : UIBoxLayoutSerializeHandler([this](ln::UIBoxLayout* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIBoxLayoutSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIBoxLayoutSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIBoxLayoutSerializeHandlerCallback callback)
    {
        if (!UIBoxLayoutSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIBoxLayoutSerializeHandler_Create(LNUIBoxLayoutSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIBoxLayoutSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIStackLayoutSerializeHandler = ln::Delegate<void(ln::UIStackLayout* self, ln::Serializer2* ar)>;

class LNWS_UIStackLayoutSerializeHandler : public UIStackLayoutSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIStackLayoutSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIStackLayoutSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIStackLayoutSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIStackLayoutSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIStackLayoutSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIStackLayoutSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIStackLayoutSerializeHandlerCallback m_callback;

    LNWS_UIStackLayoutSerializeHandler()
      : UIStackLayoutSerializeHandler([this](ln::UIStackLayout* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIStackLayoutSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIStackLayoutSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIStackLayoutSerializeHandlerCallback callback)
    {
        if (!UIStackLayoutSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIStackLayoutSerializeHandler_Create(LNUIStackLayoutSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIStackLayoutSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIGridLayoutSerializeHandler = ln::Delegate<void(ln::UIGridLayout* self, ln::Serializer2* ar)>;

class LNWS_UIGridLayoutSerializeHandler : public UIGridLayoutSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIGridLayoutSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIGridLayoutSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIGridLayoutSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIGridLayoutSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIGridLayoutSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIGridLayoutSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIGridLayoutSerializeHandlerCallback m_callback;

    LNWS_UIGridLayoutSerializeHandler()
      : UIGridLayoutSerializeHandler([this](ln::UIGridLayout* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIGridLayoutSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIGridLayoutSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIGridLayoutSerializeHandlerCallback callback)
    {
        if (!UIGridLayoutSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIGridLayoutSerializeHandler_Create(LNUIGridLayoutSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIGridLayoutSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIControlSerializeHandler = ln::Delegate<void(ln::UIControl* self, ln::Serializer2* ar)>;

class LNWS_UIControlSerializeHandler : public UIControlSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIControlSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIControlSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIControlSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIControlSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIControlSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIControlSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIControlSerializeHandlerCallback m_callback;

    LNWS_UIControlSerializeHandler()
      : UIControlSerializeHandler([this](ln::UIControl* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIControlSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIControlSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIControlSerializeHandlerCallback callback)
    {
        if (!UIControlSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIControlSerializeHandler_Create(LNUIControlSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIControlSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIButtonBaseSerializeHandler = ln::Delegate<void(ln::UIButtonBase* self, ln::Serializer2* ar)>;

class LNWS_UIButtonBaseSerializeHandler : public UIButtonBaseSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIButtonBaseSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIButtonBaseSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIButtonBaseSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIButtonBaseSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIButtonBaseSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIButtonBaseSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIButtonBaseSerializeHandlerCallback m_callback;

    LNWS_UIButtonBaseSerializeHandler()
      : UIButtonBaseSerializeHandler([this](ln::UIButtonBase* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIButtonBaseSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIButtonBaseSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIButtonBaseSerializeHandlerCallback callback)
    {
        if (!UIButtonBaseSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIButtonBaseSerializeHandler_Create(LNUIButtonBaseSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIButtonBaseSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIButtonSerializeHandler = ln::Delegate<void(ln::UIButton* self, ln::Serializer2* ar)>;

class LNWS_UIButtonSerializeHandler : public UIButtonSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIButtonSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIButtonSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIButtonSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIButtonSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIButtonSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIButtonSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIButtonSerializeHandlerCallback m_callback;

    LNWS_UIButtonSerializeHandler()
      : UIButtonSerializeHandler([this](ln::UIButton* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIButtonSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIButtonSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIButtonSerializeHandlerCallback callback)
    {
        if (!UIButtonSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIButtonSerializeHandler_Create(LNUIButtonSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIButtonSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIWindowSerializeHandler = ln::Delegate<void(ln::UIWindow* self, ln::Serializer2* ar)>;

class LNWS_UIWindowSerializeHandler : public UIWindowSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIWindowSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIWindowSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIWindowSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIWindowSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIWindowSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIWindowSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIWindowSerializeHandlerCallback m_callback;

    LNWS_UIWindowSerializeHandler()
      : UIWindowSerializeHandler([this](ln::UIWindow* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIWindowSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIWindowSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIWindowSerializeHandlerCallback callback)
    {
        if (!UIWindowSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIWindowSerializeHandler_Create(LNUIWindowSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIWindowSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIListItemSerializeHandler = ln::Delegate<void(ln::UIListItem* self, ln::Serializer2* ar)>;

class LNWS_UIListItemSerializeHandler : public UIListItemSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListItemSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIListItemSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListItemSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListItemSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListItemSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIListItemSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIListItemSerializeHandlerCallback m_callback;

    LNWS_UIListItemSerializeHandler()
      : UIListItemSerializeHandler([this](ln::UIListItem* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIListItemSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIListItemSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIListItemSerializeHandlerCallback callback)
    {
        if (!UIListItemSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIListItemSerializeHandler_Create(LNUIListItemSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIListItemSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIListItemsControlSerializeHandler = ln::Delegate<void(ln::UIListItemsControl* self, ln::Serializer2* ar)>;

class LNWS_UIListItemsControlSerializeHandler : public UIListItemsControlSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListItemsControlSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIListItemsControlSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListItemsControlSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListItemsControlSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListItemsControlSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIListItemsControlSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIListItemsControlSerializeHandlerCallback m_callback;

    LNWS_UIListItemsControlSerializeHandler()
      : UIListItemsControlSerializeHandler([this](ln::UIListItemsControl* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIListItemsControlSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIListItemsControlSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIListItemsControlSerializeHandlerCallback callback)
    {
        if (!UIListItemsControlSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIListItemsControlSerializeHandler_Create(LNUIListItemsControlSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIListItemsControlSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIListBoxItemSerializeHandler = ln::Delegate<void(ln::UIListBoxItem* self, ln::Serializer2* ar)>;

class LNWS_UIListBoxItemSerializeHandler : public UIListBoxItemSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListBoxItemSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIListBoxItemSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListBoxItemSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListBoxItemSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListBoxItemSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIListBoxItemSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIListBoxItemSerializeHandlerCallback m_callback;

    LNWS_UIListBoxItemSerializeHandler()
      : UIListBoxItemSerializeHandler([this](ln::UIListBoxItem* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIListBoxItemSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIListBoxItemSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIListBoxItemSerializeHandlerCallback callback)
    {
        if (!UIListBoxItemSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIListBoxItemSerializeHandler_Create(LNUIListBoxItemSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIListBoxItemSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using UIListBoxSerializeHandler = ln::Delegate<void(ln::UIListBox* self, ln::Serializer2* ar)>;

class LNWS_UIListBoxSerializeHandler : public UIListBoxSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListBoxSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIListBoxSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListBoxSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListBoxSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListBoxSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIListBoxSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIListBoxSerializeHandlerCallback m_callback;

    LNWS_UIListBoxSerializeHandler()
      : UIListBoxSerializeHandler([this](ln::UIListBox* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNUIListBoxSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UIListBoxSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIListBoxSerializeHandlerCallback callback)
    {
        if (!UIListBoxSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUIListBoxSerializeHandler_Create(LNUIListBoxSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UIListBoxSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using InputGestureSerializeHandler = ln::Delegate<void(ln::InputGesture* self, ln::Serializer2* ar)>;

class LNWS_InputGestureSerializeHandler : public InputGestureSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInputGestureSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNInputGestureSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInputGestureSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInputGestureSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInputGestureSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNInputGestureSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNInputGestureSerializeHandlerCallback m_callback;

    LNWS_InputGestureSerializeHandler()
      : InputGestureSerializeHandler([this](ln::InputGesture* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNInputGestureSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_InputGestureSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNInputGestureSerializeHandlerCallback callback)
    {
        if (!InputGestureSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNInputGestureSerializeHandler_Create(LNInputGestureSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_InputGestureSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using KeyGestureSerializeHandler = ln::Delegate<void(ln::KeyGesture* self, ln::Serializer2* ar)>;

class LNWS_KeyGestureSerializeHandler : public KeyGestureSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNKeyGestureSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNKeyGestureSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNKeyGestureSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNKeyGestureSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNKeyGestureSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNKeyGestureSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNKeyGestureSerializeHandlerCallback m_callback;

    LNWS_KeyGestureSerializeHandler()
      : KeyGestureSerializeHandler([this](ln::KeyGesture* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNKeyGestureSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_KeyGestureSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNKeyGestureSerializeHandlerCallback callback)
    {
        if (!KeyGestureSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNKeyGestureSerializeHandler_Create(LNKeyGestureSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_KeyGestureSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using InterpreterCommandSerializeHandler = ln::Delegate<void(ln::InterpreterCommand* self, ln::Serializer2* ar)>;

class LNWS_InterpreterCommandSerializeHandler : public InterpreterCommandSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreterCommandSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNInterpreterCommandSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreterCommandSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreterCommandSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNInterpreterCommandSerializeHandlerCallback m_callback;

    LNWS_InterpreterCommandSerializeHandler()
      : InterpreterCommandSerializeHandler([this](ln::InterpreterCommand* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNInterpreterCommandSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_InterpreterCommandSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNInterpreterCommandSerializeHandlerCallback callback)
    {
        if (!InterpreterCommandSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNInterpreterCommandSerializeHandler_Create(LNInterpreterCommandSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_InterpreterCommandSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using InterpreterCommandListSerializeHandler = ln::Delegate<void(ln::InterpreterCommandList* self, ln::Serializer2* ar)>;

class LNWS_InterpreterCommandListSerializeHandler : public InterpreterCommandListSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreterCommandListSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNInterpreterCommandListSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreterCommandListSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreterCommandListSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNInterpreterCommandListSerializeHandlerCallback m_callback;

    LNWS_InterpreterCommandListSerializeHandler()
      : InterpreterCommandListSerializeHandler([this](ln::InterpreterCommandList* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNInterpreterCommandListSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_InterpreterCommandListSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNInterpreterCommandListSerializeHandlerCallback callback)
    {
        if (!InterpreterCommandListSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNInterpreterCommandListSerializeHandler_Create(LNInterpreterCommandListSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_InterpreterCommandListSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using InterpreterSerializeHandler = ln::Delegate<void(ln::Interpreter* self, ln::Serializer2* ar)>;

class LNWS_InterpreterSerializeHandler : public InterpreterSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreterSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNInterpreterSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreterSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreterSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreterSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNInterpreterSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNInterpreterSerializeHandlerCallback m_callback;

    LNWS_InterpreterSerializeHandler()
      : InterpreterSerializeHandler([this](ln::Interpreter* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNInterpreterSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_InterpreterSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNInterpreterSerializeHandlerCallback callback)
    {
        if (!InterpreterSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNInterpreterSerializeHandler_Create(LNInterpreterSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_InterpreterSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using InterpreterUpdateWaitHandler = ln::Delegate<bool(ln::Interpreter* self)>;

class LNWS_InterpreterUpdateWaitHandler : public InterpreterUpdateWaitHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreterUpdateWaitHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNInterpreterUpdateWaitHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreterUpdateWaitHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreterUpdateWaitHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo* subclassInfo() { static LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNInterpreterUpdateWaitHandlerCallback m_callback;

    LNWS_InterpreterUpdateWaitHandler()
      : InterpreterUpdateWaitHandler([this](ln::Interpreter* self) -> bool
    {
        LNBool ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), &ret);
        if (r != LN_OK) { LN_ERROR("LNInterpreterUpdateWaitHandlerCallback"); }
        return ret != LN_FALSE;
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_InterpreterUpdateWaitHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNInterpreterUpdateWaitHandlerCallback callback)
    {
        if (!InterpreterUpdateWaitHandler::init()) return false;
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


LN_FLAT_API LNResult LNInterpreterUpdateWaitHandler_Create(LNInterpreterUpdateWaitHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_InterpreterUpdateWaitHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ApplicationSerializeHandler = ln::Delegate<void(ln::Application* self, ln::Serializer2* ar)>;

class LNWS_ApplicationSerializeHandler : public ApplicationSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNApplicationSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNApplicationSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNApplicationSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNApplicationSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNApplicationSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNApplicationSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNApplicationSerializeHandlerCallback m_callback;

    LNWS_ApplicationSerializeHandler()
      : ApplicationSerializeHandler([this](ln::Application* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_OK) { LN_ERROR("LNApplicationSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ApplicationSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNApplicationSerializeHandlerCallback callback)
    {
        if (!ApplicationSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNApplicationSerializeHandler_Create(LNApplicationSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ApplicationSerializeHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ApplicationInitHandler = ln::Delegate<void(ln::Application* self)>;

class LNWS_ApplicationInitHandler : public ApplicationInitHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNApplicationInitHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNApplicationInitHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNApplicationInitHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNApplicationInitHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNApplicationInitHandler_SubclassRegistrationInfo* subclassInfo() { static LNApplicationInitHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNApplicationInitHandlerCallback m_callback;

    LNWS_ApplicationInitHandler()
      : ApplicationInitHandler([this](ln::Application* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNApplicationInitHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ApplicationInitHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNApplicationInitHandlerCallback callback)
    {
        if (!ApplicationInitHandler::init()) return false;
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


LN_FLAT_API LNResult LNApplicationInitHandler_Create(LNApplicationInitHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ApplicationInitHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


// Auto generated override handler
using ApplicationUpdateHandler = ln::Delegate<void(ln::Application* self)>;

class LNWS_ApplicationUpdateHandler : public ApplicationUpdateHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNApplicationUpdateHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNApplicationUpdateHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNApplicationUpdateHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNApplicationUpdateHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNApplicationUpdateHandler_SubclassRegistrationInfo* subclassInfo() { static LNApplicationUpdateHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNApplicationUpdateHandlerCallback m_callback;

    LNWS_ApplicationUpdateHandler()
      : ApplicationUpdateHandler([this](ln::Application* self) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self));
        if (r != LN_OK) { LN_ERROR("LNApplicationUpdateHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ApplicationUpdateHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNApplicationUpdateHandlerCallback callback)
    {
        if (!ApplicationUpdateHandler::init()) return false;
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


LN_FLAT_API LNResult LNApplicationUpdateHandler_Create(LNApplicationUpdateHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ApplicationUpdateHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_Object : public ln::Object
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNObject_OverridePrototypes
    {
        ln::Ref<LNWS_ObjectSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LNObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNObject_SubclassRegistrationInfo* subclassInfo() { static LNObject_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Object()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Object()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNObject_OnSerialize_OverrideCallback s_LNObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNObject_OnSerialize_OverrideCallback) s_LNObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Object::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Object::onSerialize(ar);
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

LNObject_OnSerialize_OverrideCallback LNWS_ln_Object::s_LNObject_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_EventConnection : public ln::EventConnection
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEventConnection_OverridePrototypes
    {
        ln::Ref<LNWS_EventConnectionSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNEventConnection_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEventConnection_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEventConnection_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEventConnection_SubclassRegistrationInfo* subclassInfo() { static LNEventConnection_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_EventConnection()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_EventConnection()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNEventConnection_OnSerialize_OverrideCallback s_LNEventConnection_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNEventConnection_OnSerialize_OverrideCallback) s_LNEventConnection_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::EventConnection::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::EventConnection::onSerialize(ar);
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

LNEventConnection_OnSerialize_OverrideCallback LNWS_ln_EventConnection::s_LNEventConnection_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_PromiseFailureDelegate : public ln::PromiseFailureDelegate
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPromiseFailureDelegate_OverridePrototypes
    {

    };
    std::unique_ptr<LNPromiseFailureDelegate_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPromiseFailureDelegate_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPromiseFailureDelegate_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPromiseFailureDelegate_SubclassRegistrationInfo* subclassInfo() { static LNPromiseFailureDelegate_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNPromiseFailureDelegateCallback m_callback;

    LNWS_ln_PromiseFailureDelegate()
      : ln::PromiseFailureDelegate([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_OK) { LN_ERROR("LNPromiseFailureDelegateCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_PromiseFailureDelegate()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNPromiseFailureDelegateCallback callback)
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


LN_FLAT_API LNResult LNPromiseFailureDelegate_Create(LNPromiseFailureDelegateCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_PromiseFailureDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_Variant : public ln::Variant
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVariant_OverridePrototypes
    {
        ln::Ref<LNWS_VariantSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNVariant_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVariant_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVariant_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVariant_SubclassRegistrationInfo* subclassInfo() { static LNVariant_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Variant()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Variant()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNVariant_OnSerialize_OverrideCallback s_LNVariant_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNVariant_OnSerialize_OverrideCallback) s_LNVariant_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Variant::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Variant::onSerialize(ar);
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

LNVariant_OnSerialize_OverrideCallback LNWS_ln_Variant::s_LNVariant_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ZVTestDelegate1 : public ln::ZVTestDelegate1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestDelegate1_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestDelegate1_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestDelegate1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestDelegate1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestDelegate1_SubclassRegistrationInfo* subclassInfo() { static LNZVTestDelegate1_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNZVTestDelegate1Callback m_callback;

    LNWS_ln_ZVTestDelegate1()
      : ln::ZVTestDelegate1([this](int p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1);
        if (r != LN_OK) { LN_ERROR("LNZVTestDelegate1Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestDelegate1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNZVTestDelegate1Callback callback)
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


LN_FLAT_API LNResult LNZVTestDelegate1_Create(LNZVTestDelegate1Callback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate1, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_ZVTestDelegate2 : public ln::ZVTestDelegate2
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestDelegate2_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestDelegate2_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestDelegate2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestDelegate2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestDelegate2_SubclassRegistrationInfo* subclassInfo() { static LNZVTestDelegate2_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNZVTestDelegate2Callback m_callback;

    LNWS_ln_ZVTestDelegate2()
      : ln::ZVTestDelegate2([this](int p1, int p2) -> int
    {
        int ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1, p2, &ret);
        if (r != LN_OK) { LN_ERROR("LNZVTestDelegate2Callback"); }
        return ret;
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestDelegate2()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNZVTestDelegate2Callback callback)
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


LN_FLAT_API LNResult LNZVTestDelegate2_Create(LNZVTestDelegate2Callback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate2, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_ZVTestDelegate3 : public ln::ZVTestDelegate3
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestDelegate3_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestDelegate3_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestDelegate3_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestDelegate3_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestDelegate3_SubclassRegistrationInfo* subclassInfo() { static LNZVTestDelegate3_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNZVTestDelegate3Callback m_callback;

    LNWS_ln_ZVTestDelegate3()
      : ln::ZVTestDelegate3([this](ln::ZVTestClass1* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(p1));
        if (r != LN_OK) { LN_ERROR("LNZVTestDelegate3Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestDelegate3()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNZVTestDelegate3Callback callback)
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


LN_FLAT_API LNResult LNZVTestDelegate3_Create(LNZVTestDelegate3Callback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate3, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_ZVTestEventHandler1 : public ln::ZVTestEventHandler1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestEventHandler1_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestEventHandler1_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestEventHandler1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestEventHandler1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestEventHandler1_SubclassRegistrationInfo* subclassInfo() { static LNZVTestEventHandler1_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNZVTestEventHandler1Callback m_callback;

    LNWS_ln_ZVTestEventHandler1()
      : ln::ZVTestEventHandler1([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_OK) { LN_ERROR("LNZVTestEventHandler1Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestEventHandler1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNZVTestEventHandler1Callback callback)
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


LN_FLAT_API LNResult LNZVTestEventHandler1_Create(LNZVTestEventHandler1Callback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestEventHandler1, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_ZVTestEventHandler2 : public ln::ZVTestEventHandler2
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestEventHandler2_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestEventHandler2_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestEventHandler2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestEventHandler2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestEventHandler2_SubclassRegistrationInfo* subclassInfo() { static LNZVTestEventHandler2_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNZVTestEventHandler2Callback m_callback;

    LNWS_ln_ZVTestEventHandler2()
      : ln::ZVTestEventHandler2([this](ln::ZVTestEventArgs1* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1));
        if (r != LN_OK) { LN_ERROR("LNZVTestEventHandler2Callback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestEventHandler2()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNZVTestEventHandler2Callback callback)
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


LN_FLAT_API LNResult LNZVTestEventHandler2_Create(LNZVTestEventHandler2Callback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestEventHandler2, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_ZVTestPromise1 : public ln::ZVTestPromise1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestPromise1_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestPromise1_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestPromise1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestPromise1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestPromise1_SubclassRegistrationInfo* subclassInfo() { static LNZVTestPromise1_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

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
    struct LNZVTestPromise2_OverridePrototypes
    {

    };
    std::unique_ptr<LNZVTestPromise2_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestPromise2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestPromise2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestPromise2_SubclassRegistrationInfo* subclassInfo() { static LNZVTestPromise2_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

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
    struct LNZVTestClass1_OverridePrototypes
    {
        ln::Ref<LNWS_ZVTestClass1SerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNZVTestClass1_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestClass1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestClass1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestClass1_SubclassRegistrationInfo* subclassInfo() { static LNZVTestClass1_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_ZVTestClass1()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestClass1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNZVTestClass1_OnSerialize_OverrideCallback s_LNZVTestClass1_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNZVTestClass1_OnSerialize_OverrideCallback) s_LNZVTestClass1_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::ZVTestClass1::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::ZVTestClass1::onSerialize(ar);
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

LNZVTestClass1_OnSerialize_OverrideCallback LNWS_ln_ZVTestClass1::s_LNZVTestClass1_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ZVTestEventArgs1 : public ln::ZVTestEventArgs1
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNZVTestEventArgs1_OverridePrototypes
    {
        ln::Ref<LNWS_ZVTestEventArgs1SerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNZVTestEventArgs1_OverridePrototypes> m_overridePrototypes = nullptr;
    LNZVTestEventArgs1_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNZVTestEventArgs1_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNZVTestEventArgs1_SubclassRegistrationInfo* subclassInfo() { static LNZVTestEventArgs1_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_ZVTestEventArgs1()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ZVTestEventArgs1()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNZVTestEventArgs1_OnSerialize_OverrideCallback s_LNZVTestEventArgs1_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNZVTestEventArgs1_OnSerialize_OverrideCallback) s_LNZVTestEventArgs1_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::ZVTestEventArgs1::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::ZVTestEventArgs1::onSerialize(ar);
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

LNZVTestEventArgs1_OnSerialize_OverrideCallback LNWS_ln_ZVTestEventArgs1::s_LNZVTestEventArgs1_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Log : public ln::Log
{
public:
};



class LNWS_ln_Serializer2 : public ln::Serializer2
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSerializer2_OverridePrototypes
    {
        ln::Ref<LNWS_Serializer2SerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNSerializer2_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSerializer2_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSerializer2_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSerializer2_SubclassRegistrationInfo* subclassInfo() { static LNSerializer2_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Serializer2()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Serializer2()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNSerializer2_OnSerialize_OverrideCallback s_LNSerializer2_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNSerializer2_OnSerialize_OverrideCallback) s_LNSerializer2_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Serializer2::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Serializer2::onSerialize(ar);
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

LNSerializer2_OnSerialize_OverrideCallback LNWS_ln_Serializer2::s_LNSerializer2_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AssetObject : public ln::AssetObject
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAssetObject_OverridePrototypes
    {
        ln::Ref<LNWS_AssetObjectSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNAssetObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAssetObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAssetObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAssetObject_SubclassRegistrationInfo* subclassInfo() { static LNAssetObject_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_AssetObject()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AssetObject()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNAssetObject_OnSerialize_OverrideCallback s_LNAssetObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNAssetObject_OnSerialize_OverrideCallback) s_LNAssetObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::AssetObject::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::AssetObject::onSerialize(ar);
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

LNAssetObject_OnSerialize_OverrideCallback LNWS_ln_AssetObject::s_LNAssetObject_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AssetImportSettings : public ln::AssetImportSettings
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAssetImportSettings_OverridePrototypes
    {
        ln::Ref<LNWS_AssetImportSettingsSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNAssetImportSettings_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAssetImportSettings_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAssetImportSettings_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAssetImportSettings_SubclassRegistrationInfo* subclassInfo() { static LNAssetImportSettings_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_AssetImportSettings()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AssetImportSettings()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNAssetImportSettings_OnSerialize_OverrideCallback s_LNAssetImportSettings_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNAssetImportSettings_OnSerialize_OverrideCallback) s_LNAssetImportSettings_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::AssetImportSettings::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::AssetImportSettings::onSerialize(ar);
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

LNAssetImportSettings_OnSerialize_OverrideCallback LNWS_ln_AssetImportSettings::s_LNAssetImportSettings_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AssetModel : public ln::AssetModel
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAssetModel_OverridePrototypes
    {
        ln::Ref<LNWS_AssetModelSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNAssetModel_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAssetModel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAssetModel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAssetModel_SubclassRegistrationInfo* subclassInfo() { static LNAssetModel_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_AssetModel()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AssetModel()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNAssetModel_OnSerialize_OverrideCallback s_LNAssetModel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNAssetModel_OnSerialize_OverrideCallback) s_LNAssetModel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::AssetModel::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::AssetModel::onSerialize(ar);
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

LNAssetModel_OnSerialize_OverrideCallback LNWS_ln_AssetModel::s_LNAssetModel_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Assets : public ln::Assets
{
public:
};



class LNWS_ln_Sound : public ln::Sound
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSound_OverridePrototypes
    {
        ln::Ref<LNWS_SoundSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNSound_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSound_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSound_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSound_SubclassRegistrationInfo* subclassInfo() { static LNSound_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Sound()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Sound()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNSound_OnSerialize_OverrideCallback s_LNSound_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNSound_OnSerialize_OverrideCallback) s_LNSound_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Sound::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Sound::onSerialize(ar);
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

LNSound_OnSerialize_OverrideCallback LNWS_ln_Sound::s_LNSound_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Audio : public ln::Audio
{
public:
};



class LNWS_ln_Texture2DDelegate : public ln::Texture2DDelegate
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTexture2DDelegate_OverridePrototypes
    {

    };
    std::unique_ptr<LNTexture2DDelegate_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTexture2DDelegate_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTexture2DDelegate_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTexture2DDelegate_SubclassRegistrationInfo* subclassInfo() { static LNTexture2DDelegate_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNTexture2DDelegateCallback m_callback;

    LNWS_ln_Texture2DDelegate()
      : ln::Texture2DDelegate([this](ln::Texture2D* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(p1));
        if (r != LN_OK) { LN_ERROR("LNTexture2DDelegateCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Texture2DDelegate()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNTexture2DDelegateCallback callback)
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


LN_FLAT_API LNResult LNTexture2DDelegate_Create(LNTexture2DDelegateCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_Texture2DDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_Texture2DPromise : public ln::Texture2DPromise
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTexture2DPromise_OverridePrototypes
    {

    };
    std::unique_ptr<LNTexture2DPromise_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTexture2DPromise_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTexture2DPromise_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTexture2DPromise_SubclassRegistrationInfo* subclassInfo() { static LNTexture2DPromise_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

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



class LNWS_ln_Graphics : public ln::Graphics
{
public:
};



class LNWS_ln_Texture : public ln::Texture
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTexture_OverridePrototypes
    {
        ln::Ref<LNWS_TextureSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNTexture_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTexture_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTexture_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTexture_SubclassRegistrationInfo* subclassInfo() { static LNTexture_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Texture()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Texture()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNTexture_OnSerialize_OverrideCallback s_LNTexture_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNTexture_OnSerialize_OverrideCallback) s_LNTexture_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Texture::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Texture::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<AssetObject>();
    }

};

LNTexture_OnSerialize_OverrideCallback LNWS_ln_Texture::s_LNTexture_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Texture2D : public ln::Texture2D
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTexture2D_OverridePrototypes
    {
        ln::Ref<LNWS_Texture2DSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNTexture2D_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTexture2D_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTexture2D_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTexture2D_SubclassRegistrationInfo* subclassInfo() { static LNTexture2D_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Texture2D()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Texture2D()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNTexture2D_OnSerialize_OverrideCallback s_LNTexture2D_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNTexture2D_OnSerialize_OverrideCallback) s_LNTexture2D_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Texture2D::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Texture2D::onSerialize(ar);
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

LNTexture2D_OnSerialize_OverrideCallback LNWS_ln_Texture2D::s_LNTexture2D_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Shader : public ln::Shader
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNShader_OverridePrototypes
    {
        ln::Ref<LNWS_ShaderSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNShader_OverridePrototypes> m_overridePrototypes = nullptr;
    LNShader_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNShader_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNShader_SubclassRegistrationInfo* subclassInfo() { static LNShader_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Shader()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Shader()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNShader_OnSerialize_OverrideCallback s_LNShader_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNShader_OnSerialize_OverrideCallback) s_LNShader_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Shader::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Shader::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<AssetObject>();
    }

};

LNShader_OnSerialize_OverrideCallback LNWS_ln_Shader::s_LNShader_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_RenderView : public ln::RenderView
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNRenderView_OverridePrototypes
    {
        ln::Ref<LNWS_RenderViewSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNRenderView_OverridePrototypes> m_overridePrototypes = nullptr;
    LNRenderView_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNRenderView_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNRenderView_SubclassRegistrationInfo* subclassInfo() { static LNRenderView_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_RenderView()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_RenderView()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNRenderView_OnSerialize_OverrideCallback s_LNRenderView_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNRenderView_OnSerialize_OverrideCallback) s_LNRenderView_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::RenderView::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::RenderView::onSerialize(ar);
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

LNRenderView_OnSerialize_OverrideCallback LNWS_ln_RenderView::s_LNRenderView_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Material : public ln::Material
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMaterial_OverridePrototypes
    {
        ln::Ref<LNWS_MaterialSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNMaterial_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMaterial_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMaterial_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMaterial_SubclassRegistrationInfo* subclassInfo() { static LNMaterial_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Material()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Material()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNMaterial_OnSerialize_OverrideCallback s_LNMaterial_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNMaterial_OnSerialize_OverrideCallback) s_LNMaterial_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Material::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Material::onSerialize(ar);
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

LNMaterial_OnSerialize_OverrideCallback LNWS_ln_Material::s_LNMaterial_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_MeshNode : public ln::MeshNode
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshNode_OverridePrototypes
    {
        ln::Ref<LNWS_MeshNodeSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNMeshNode_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshNode_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshNode_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshNode_SubclassRegistrationInfo* subclassInfo() { static LNMeshNode_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_MeshNode()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_MeshNode()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNMeshNode_OnSerialize_OverrideCallback s_LNMeshNode_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNMeshNode_OnSerialize_OverrideCallback) s_LNMeshNode_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::MeshNode::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::MeshNode::onSerialize(ar);
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

LNMeshNode_OnSerialize_OverrideCallback LNWS_ln_MeshNode::s_LNMeshNode_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AnimationController : public ln::AnimationController
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationController_OverridePrototypes
    {
        ln::Ref<LNWS_AnimationControllerSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNAnimationController_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationController_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationController_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationController_SubclassRegistrationInfo* subclassInfo() { static LNAnimationController_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_AnimationController()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AnimationController()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNAnimationController_OnSerialize_OverrideCallback s_LNAnimationController_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNAnimationController_OnSerialize_OverrideCallback) s_LNAnimationController_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::AnimationController::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::AnimationController::onSerialize(ar);
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

LNAnimationController_OnSerialize_OverrideCallback LNWS_ln_AnimationController::s_LNAnimationController_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_MeshModel : public ln::MeshModel
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshModel_OverridePrototypes
    {
        ln::Ref<LNWS_MeshModelSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNMeshModel_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshModel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshModel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshModel_SubclassRegistrationInfo* subclassInfo() { static LNMeshModel_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_MeshModel()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_MeshModel()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNMeshModel_OnSerialize_OverrideCallback s_LNMeshModel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNMeshModel_OnSerialize_OverrideCallback) s_LNMeshModel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::MeshModel::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::MeshModel::onSerialize(ar);
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

LNMeshModel_OnSerialize_OverrideCallback LNWS_ln_MeshModel::s_LNMeshModel_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_MeshImportSettings : public ln::MeshImportSettings
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshImportSettings_OverridePrototypes
    {
        ln::Ref<LNWS_MeshImportSettingsSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNMeshImportSettings_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshImportSettings_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshImportSettings_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshImportSettings_SubclassRegistrationInfo* subclassInfo() { static LNMeshImportSettings_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_MeshImportSettings()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_MeshImportSettings()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNMeshImportSettings_OnSerialize_OverrideCallback s_LNMeshImportSettings_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNMeshImportSettings_OnSerialize_OverrideCallback) s_LNMeshImportSettings_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::MeshImportSettings::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::MeshImportSettings::onSerialize(ar);
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

LNMeshImportSettings_OnSerialize_OverrideCallback LNWS_ln_MeshImportSettings::s_LNMeshImportSettings_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_SkinnedMeshModel : public ln::SkinnedMeshModel
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSkinnedMeshModel_OverridePrototypes
    {
        ln::Ref<LNWS_SkinnedMeshModelSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNSkinnedMeshModel_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSkinnedMeshModel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSkinnedMeshModel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSkinnedMeshModel_SubclassRegistrationInfo* subclassInfo() { static LNSkinnedMeshModel_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_SkinnedMeshModel()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_SkinnedMeshModel()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNSkinnedMeshModel_OnSerialize_OverrideCallback s_LNSkinnedMeshModel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNSkinnedMeshModel_OnSerialize_OverrideCallback) s_LNSkinnedMeshModel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::SkinnedMeshModel::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::SkinnedMeshModel::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<MeshModel>();
    }

};

LNSkinnedMeshModel_OnSerialize_OverrideCallback LNWS_ln_SkinnedMeshModel::s_LNSkinnedMeshModel_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_CollisionShape : public ln::CollisionShape
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCollisionShape_OverridePrototypes
    {
        ln::Ref<LNWS_CollisionShapeSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNCollisionShape_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCollisionShape_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCollisionShape_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCollisionShape_SubclassRegistrationInfo* subclassInfo() { static LNCollisionShape_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_CollisionShape()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_CollisionShape()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNCollisionShape_OnSerialize_OverrideCallback s_LNCollisionShape_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNCollisionShape_OnSerialize_OverrideCallback) s_LNCollisionShape_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::CollisionShape::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::CollisionShape::onSerialize(ar);
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

LNCollisionShape_OnSerialize_OverrideCallback LNWS_ln_CollisionShape::s_LNCollisionShape_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_BoxCollisionShape : public ln::BoxCollisionShape
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNBoxCollisionShape_OverridePrototypes
    {
        ln::Ref<LNWS_BoxCollisionShapeSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNBoxCollisionShape_OverridePrototypes> m_overridePrototypes = nullptr;
    LNBoxCollisionShape_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNBoxCollisionShape_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNBoxCollisionShape_SubclassRegistrationInfo* subclassInfo() { static LNBoxCollisionShape_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_BoxCollisionShape()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_BoxCollisionShape()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNBoxCollisionShape_OnSerialize_OverrideCallback s_LNBoxCollisionShape_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNBoxCollisionShape_OnSerialize_OverrideCallback) s_LNBoxCollisionShape_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::BoxCollisionShape::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::BoxCollisionShape::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<CollisionShape>();
    }

};

LNBoxCollisionShape_OnSerialize_OverrideCallback LNWS_ln_BoxCollisionShape::s_LNBoxCollisionShape_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AnimationCurve : public ln::AnimationCurve
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationCurve_OverridePrototypes
    {
        ln::Ref<LNWS_AnimationCurveSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNAnimationCurve_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationCurve_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationCurve_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationCurve_SubclassRegistrationInfo* subclassInfo() { static LNAnimationCurve_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_AnimationCurve()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AnimationCurve()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNAnimationCurve_OnSerialize_OverrideCallback s_LNAnimationCurve_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNAnimationCurve_OnSerialize_OverrideCallback) s_LNAnimationCurve_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::AnimationCurve::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::AnimationCurve::onSerialize(ar);
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

LNAnimationCurve_OnSerialize_OverrideCallback LNWS_ln_AnimationCurve::s_LNAnimationCurve_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_KeyFrameAnimationCurve : public ln::KeyFrameAnimationCurve
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNKeyFrameAnimationCurve_OverridePrototypes
    {
        ln::Ref<LNWS_KeyFrameAnimationCurveSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNKeyFrameAnimationCurve_OverridePrototypes> m_overridePrototypes = nullptr;
    LNKeyFrameAnimationCurve_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNKeyFrameAnimationCurve_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNKeyFrameAnimationCurve_SubclassRegistrationInfo* subclassInfo() { static LNKeyFrameAnimationCurve_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_KeyFrameAnimationCurve()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_KeyFrameAnimationCurve()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback s_LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback) s_LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::KeyFrameAnimationCurve::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::KeyFrameAnimationCurve::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<AnimationCurve>();
    }

};

LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback LNWS_ln_KeyFrameAnimationCurve::s_LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AnimationClip : public ln::AnimationClip
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationClip_OverridePrototypes
    {
        ln::Ref<LNWS_AnimationClipSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNAnimationClip_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationClip_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationClip_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationClip_SubclassRegistrationInfo* subclassInfo() { static LNAnimationClip_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_AnimationClip()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AnimationClip()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNAnimationClip_OnSerialize_OverrideCallback s_LNAnimationClip_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNAnimationClip_OnSerialize_OverrideCallback) s_LNAnimationClip_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::AnimationClip::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::AnimationClip::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<AssetObject>();
    }

};

LNAnimationClip_OnSerialize_OverrideCallback LNWS_ln_AnimationClip::s_LNAnimationClip_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AnimationState : public ln::AnimationState
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNAnimationState_OverridePrototypes
    {
        ln::Ref<LNWS_AnimationStateSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNAnimationState_OverridePrototypes> m_overridePrototypes = nullptr;
    LNAnimationState_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNAnimationState_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNAnimationState_SubclassRegistrationInfo* subclassInfo() { static LNAnimationState_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_AnimationState()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_AnimationState()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNAnimationState_OnSerialize_OverrideCallback s_LNAnimationState_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNAnimationState_OnSerialize_OverrideCallback) s_LNAnimationState_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::AnimationState::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::AnimationState::onSerialize(ar);
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

LNAnimationState_OnSerialize_OverrideCallback LNWS_ln_AnimationState::s_LNAnimationState_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_EffectResource : public ln::EffectResource
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEffectResource_OverridePrototypes
    {
        ln::Ref<LNWS_EffectResourceSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNEffectResource_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEffectResource_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEffectResource_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEffectResource_SubclassRegistrationInfo* subclassInfo() { static LNEffectResource_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_EffectResource()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_EffectResource()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNEffectResource_OnSerialize_OverrideCallback s_LNEffectResource_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNEffectResource_OnSerialize_OverrideCallback) s_LNEffectResource_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::EffectResource::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::EffectResource::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<AssetObject>();
    }

};

LNEffectResource_OnSerialize_OverrideCallback LNWS_ln_EffectResource::s_LNEffectResource_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ParticleEmitterModel : public ln::ParticleEmitterModel
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleEmitterModel_OverridePrototypes
    {
        ln::Ref<LNWS_ParticleEmitterModelSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNParticleEmitterModel_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleEmitterModel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleEmitterModel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleEmitterModel_SubclassRegistrationInfo* subclassInfo() { static LNParticleEmitterModel_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_ParticleEmitterModel()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ParticleEmitterModel()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNParticleEmitterModel_OnSerialize_OverrideCallback s_LNParticleEmitterModel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNParticleEmitterModel_OnSerialize_OverrideCallback) s_LNParticleEmitterModel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::ParticleEmitterModel::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::ParticleEmitterModel::onSerialize(ar);
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

LNParticleEmitterModel_OnSerialize_OverrideCallback LNWS_ln_ParticleEmitterModel::s_LNParticleEmitterModel_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ParticleModel : public ln::ParticleModel
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleModel_OverridePrototypes
    {
        ln::Ref<LNWS_ParticleModelSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNParticleModel_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleModel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleModel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleModel_SubclassRegistrationInfo* subclassInfo() { static LNParticleModel_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_ParticleModel()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ParticleModel()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNParticleModel_OnSerialize_OverrideCallback s_LNParticleModel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNParticleModel_OnSerialize_OverrideCallback) s_LNParticleModel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::ParticleModel::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::ParticleModel::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<EffectResource>();
    }

};

LNParticleModel_OnSerialize_OverrideCallback LNWS_ln_ParticleModel::s_LNParticleModel_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Component : public ln::Component
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNComponent_OverridePrototypes
    {
        ln::Ref<LNWS_ComponentSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LNComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNComponent_SubclassRegistrationInfo* subclassInfo() { static LNComponent_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Component()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Component()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNComponent_OnSerialize_OverrideCallback s_LNComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNComponent_OnSerialize_OverrideCallback) s_LNComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Component::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Component::onSerialize(ar);
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

LNComponent_OnSerialize_OverrideCallback LNWS_ln_Component::s_LNComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_VisualComponent : public ln::VisualComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVisualComponent_OverridePrototypes
    {
        ln::Ref<LNWS_VisualComponentSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNVisualComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVisualComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVisualComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVisualComponent_SubclassRegistrationInfo* subclassInfo() { static LNVisualComponent_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_VisualComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_VisualComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNVisualComponent_OnSerialize_OverrideCallback s_LNVisualComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNVisualComponent_OnSerialize_OverrideCallback) s_LNVisualComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::VisualComponent::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::VisualComponent::onSerialize(ar);
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

LNVisualComponent_OnSerialize_OverrideCallback LNWS_ln_VisualComponent::s_LNVisualComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_SpriteComponent : public ln::SpriteComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpriteComponent_OverridePrototypes
    {
        ln::Ref<LNWS_SpriteComponentSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNSpriteComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpriteComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpriteComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpriteComponent_SubclassRegistrationInfo* subclassInfo() { static LNSpriteComponent_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_SpriteComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_SpriteComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNSpriteComponent_OnSerialize_OverrideCallback s_LNSpriteComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNSpriteComponent_OnSerialize_OverrideCallback) s_LNSpriteComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::SpriteComponent::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::SpriteComponent::onSerialize(ar);
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

LNSpriteComponent_OnSerialize_OverrideCallback LNWS_ln_SpriteComponent::s_LNSpriteComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_CollisionEventHandler : public ln::CollisionEventHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCollisionEventHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNCollisionEventHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCollisionEventHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCollisionEventHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCollisionEventHandler_SubclassRegistrationInfo* subclassInfo() { static LNCollisionEventHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNCollisionEventHandlerCallback m_callback;

    LNWS_ln_CollisionEventHandler()
      : ln::CollisionEventHandler([this](ln::Collision* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1));
        if (r != LN_OK) { LN_ERROR("LNCollisionEventHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_CollisionEventHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNCollisionEventHandlerCallback callback)
    {
        if (!ln::CollisionEventHandler::init()) return false;
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


LN_FLAT_API LNResult LNCollisionEventHandler_Create(LNCollisionEventHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_CollisionEventHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_CharacterController : public ln::CharacterController
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCharacterController_OverridePrototypes
    {
        ln::Ref<LNWS_CharacterControllerSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNCharacterController_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCharacterController_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCharacterController_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCharacterController_SubclassRegistrationInfo* subclassInfo() { static LNCharacterController_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_CharacterController()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_CharacterController()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNCharacterController_OnSerialize_OverrideCallback s_LNCharacterController_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNCharacterController_OnSerialize_OverrideCallback) s_LNCharacterController_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::CharacterController::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::CharacterController::onSerialize(ar);
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

LNCharacterController_OnSerialize_OverrideCallback LNWS_ln_CharacterController::s_LNCharacterController_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_World : public ln::World
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorld_OverridePrototypes
    {
        ln::Ref<LNWS_WorldSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNWorld_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorld_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorld_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorld_SubclassRegistrationInfo* subclassInfo() { static LNWorld_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_World()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_World()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNWorld_OnSerialize_OverrideCallback s_LNWorld_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNWorld_OnSerialize_OverrideCallback) s_LNWorld_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::World::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::World::onSerialize(ar);
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

LNWorld_OnSerialize_OverrideCallback LNWS_ln_World::s_LNWorld_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ComponentList : public ln::ComponentList
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNComponentList_OverridePrototypes
    {
        ln::Ref<LNWS_ComponentListSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNComponentList_OverridePrototypes> m_overridePrototypes = nullptr;
    LNComponentList_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNComponentList_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNComponentList_SubclassRegistrationInfo* subclassInfo() { static LNComponentList_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_ComponentList()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ComponentList()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNComponentList_OnSerialize_OverrideCallback s_LNComponentList_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNComponentList_OnSerialize_OverrideCallback) s_LNComponentList_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::ComponentList::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::ComponentList::onSerialize(ar);
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

LNComponentList_OnSerialize_OverrideCallback LNWS_ln_ComponentList::s_LNComponentList_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_WorldObject : public ln::WorldObject
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorldObject_OverridePrototypes
    {
        ln::Ref<LNWS_WorldObjectSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_WorldObjectPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_WorldObjectUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNWorldObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorldObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorldObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorldObject_SubclassRegistrationInfo* subclassInfo() { static LNWorldObject_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_WorldObject()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_WorldObject()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNWorldObject_OnSerialize_OverrideCallback s_LNWorldObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNWorldObject_OnSerialize_OverrideCallback) s_LNWorldObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::WorldObject::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::WorldObject::onSerialize(ar);
    }

    static LNWorldObject_OnPreUpdate_OverrideCallback s_LNWorldObject_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNWorldObject_OnPreUpdate_OverrideCallback) s_LNWorldObject_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::WorldObject::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::WorldObject::onPreUpdate();
    }

    static LNWorldObject_OnUpdate_OverrideCallback s_LNWorldObject_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNWorldObject_OnUpdate_OverrideCallback) s_LNWorldObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::WorldObject::onUpdate(elapsedSeconds);
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

LNWorldObject_OnSerialize_OverrideCallback LNWS_ln_WorldObject::s_LNWorldObject_OnSerialize_OverrideCallback = nullptr;
LNWorldObject_OnPreUpdate_OverrideCallback LNWS_ln_WorldObject::s_LNWorldObject_OnPreUpdate_OverrideCallback = nullptr;
LNWorldObject_OnUpdate_OverrideCallback LNWS_ln_WorldObject::s_LNWorldObject_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_VisualObject : public ln::VisualObject
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVisualObject_OverridePrototypes
    {
        ln::Ref<LNWS_VisualObjectSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_VisualObjectPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_VisualObjectUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNVisualObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LNVisualObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNVisualObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNVisualObject_SubclassRegistrationInfo* subclassInfo() { static LNVisualObject_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_VisualObject()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_VisualObject()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNVisualObject_OnSerialize_OverrideCallback s_LNVisualObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNVisualObject_OnSerialize_OverrideCallback) s_LNVisualObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::VisualObject::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::VisualObject::onSerialize(ar);
    }

    static LNVisualObject_OnPreUpdate_OverrideCallback s_LNVisualObject_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNVisualObject_OnPreUpdate_OverrideCallback) s_LNVisualObject_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::VisualObject::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::VisualObject::onPreUpdate();
    }

    static LNVisualObject_OnUpdate_OverrideCallback s_LNVisualObject_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNVisualObject_OnUpdate_OverrideCallback) s_LNVisualObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::VisualObject::onUpdate(elapsedSeconds);
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

LNVisualObject_OnSerialize_OverrideCallback LNWS_ln_VisualObject::s_LNVisualObject_OnSerialize_OverrideCallback = nullptr;
LNVisualObject_OnPreUpdate_OverrideCallback LNWS_ln_VisualObject::s_LNVisualObject_OnPreUpdate_OverrideCallback = nullptr;
LNVisualObject_OnUpdate_OverrideCallback LNWS_ln_VisualObject::s_LNVisualObject_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_Camera : public ln::Camera
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCamera_OverridePrototypes
    {
        ln::Ref<LNWS_CameraSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_CameraPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_CameraUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNCamera_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCamera_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCamera_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCamera_SubclassRegistrationInfo* subclassInfo() { static LNCamera_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Camera()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Camera()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNCamera_OnSerialize_OverrideCallback s_LNCamera_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNCamera_OnSerialize_OverrideCallback) s_LNCamera_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Camera::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Camera::onSerialize(ar);
    }

    static LNCamera_OnPreUpdate_OverrideCallback s_LNCamera_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNCamera_OnPreUpdate_OverrideCallback) s_LNCamera_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Camera::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::Camera::onPreUpdate();
    }

    static LNCamera_OnUpdate_OverrideCallback s_LNCamera_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNCamera_OnUpdate_OverrideCallback) s_LNCamera_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::Camera::onUpdate(elapsedSeconds);
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

LNCamera_OnSerialize_OverrideCallback LNWS_ln_Camera::s_LNCamera_OnSerialize_OverrideCallback = nullptr;
LNCamera_OnPreUpdate_OverrideCallback LNWS_ln_Camera::s_LNCamera_OnPreUpdate_OverrideCallback = nullptr;
LNCamera_OnUpdate_OverrideCallback LNWS_ln_Camera::s_LNCamera_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_EnvironmentLight : public ln::EnvironmentLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEnvironmentLight_OverridePrototypes
    {
        ln::Ref<LNWS_EnvironmentLightSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_EnvironmentLightPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_EnvironmentLightUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNEnvironmentLight_OverridePrototypes> m_overridePrototypes = nullptr;
    LNEnvironmentLight_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNEnvironmentLight_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNEnvironmentLight_SubclassRegistrationInfo* subclassInfo() { static LNEnvironmentLight_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_EnvironmentLight()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_EnvironmentLight()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNEnvironmentLight_OnSerialize_OverrideCallback s_LNEnvironmentLight_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNEnvironmentLight_OnSerialize_OverrideCallback) s_LNEnvironmentLight_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::EnvironmentLight::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::EnvironmentLight::onSerialize(ar);
    }

    static LNEnvironmentLight_OnPreUpdate_OverrideCallback s_LNEnvironmentLight_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNEnvironmentLight_OnPreUpdate_OverrideCallback) s_LNEnvironmentLight_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::EnvironmentLight::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::EnvironmentLight::onPreUpdate();
    }

    static LNEnvironmentLight_OnUpdate_OverrideCallback s_LNEnvironmentLight_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNEnvironmentLight_OnUpdate_OverrideCallback) s_LNEnvironmentLight_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::EnvironmentLight::onUpdate(elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::EnvironmentLight::onUpdate(elapsedSeconds);
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

LNEnvironmentLight_OnSerialize_OverrideCallback LNWS_ln_EnvironmentLight::s_LNEnvironmentLight_OnSerialize_OverrideCallback = nullptr;
LNEnvironmentLight_OnPreUpdate_OverrideCallback LNWS_ln_EnvironmentLight::s_LNEnvironmentLight_OnPreUpdate_OverrideCallback = nullptr;
LNEnvironmentLight_OnUpdate_OverrideCallback LNWS_ln_EnvironmentLight::s_LNEnvironmentLight_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_DirectionalLight : public ln::DirectionalLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNDirectionalLight_OverridePrototypes
    {
        ln::Ref<LNWS_DirectionalLightSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_DirectionalLightPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_DirectionalLightUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNDirectionalLight_OverridePrototypes> m_overridePrototypes = nullptr;
    LNDirectionalLight_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNDirectionalLight_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNDirectionalLight_SubclassRegistrationInfo* subclassInfo() { static LNDirectionalLight_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_DirectionalLight()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_DirectionalLight()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNDirectionalLight_OnSerialize_OverrideCallback s_LNDirectionalLight_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNDirectionalLight_OnSerialize_OverrideCallback) s_LNDirectionalLight_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::DirectionalLight::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::DirectionalLight::onSerialize(ar);
    }

    static LNDirectionalLight_OnPreUpdate_OverrideCallback s_LNDirectionalLight_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNDirectionalLight_OnPreUpdate_OverrideCallback) s_LNDirectionalLight_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::DirectionalLight::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::DirectionalLight::onPreUpdate();
    }

    static LNDirectionalLight_OnUpdate_OverrideCallback s_LNDirectionalLight_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNDirectionalLight_OnUpdate_OverrideCallback) s_LNDirectionalLight_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::DirectionalLight::onUpdate(elapsedSeconds);
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

LNDirectionalLight_OnSerialize_OverrideCallback LNWS_ln_DirectionalLight::s_LNDirectionalLight_OnSerialize_OverrideCallback = nullptr;
LNDirectionalLight_OnPreUpdate_OverrideCallback LNWS_ln_DirectionalLight::s_LNDirectionalLight_OnPreUpdate_OverrideCallback = nullptr;
LNDirectionalLight_OnUpdate_OverrideCallback LNWS_ln_DirectionalLight::s_LNDirectionalLight_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_PointLight : public ln::PointLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPointLight_OverridePrototypes
    {
        ln::Ref<LNWS_PointLightSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_PointLightPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_PointLightUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNPointLight_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPointLight_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPointLight_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPointLight_SubclassRegistrationInfo* subclassInfo() { static LNPointLight_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_PointLight()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_PointLight()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNPointLight_OnSerialize_OverrideCallback s_LNPointLight_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNPointLight_OnSerialize_OverrideCallback) s_LNPointLight_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::PointLight::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::PointLight::onSerialize(ar);
    }

    static LNPointLight_OnPreUpdate_OverrideCallback s_LNPointLight_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNPointLight_OnPreUpdate_OverrideCallback) s_LNPointLight_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::PointLight::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::PointLight::onPreUpdate();
    }

    static LNPointLight_OnUpdate_OverrideCallback s_LNPointLight_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNPointLight_OnUpdate_OverrideCallback) s_LNPointLight_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::PointLight::onUpdate(elapsedSeconds);
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

LNPointLight_OnSerialize_OverrideCallback LNWS_ln_PointLight::s_LNPointLight_OnSerialize_OverrideCallback = nullptr;
LNPointLight_OnPreUpdate_OverrideCallback LNWS_ln_PointLight::s_LNPointLight_OnPreUpdate_OverrideCallback = nullptr;
LNPointLight_OnUpdate_OverrideCallback LNWS_ln_PointLight::s_LNPointLight_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_SpotLight : public ln::SpotLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpotLight_OverridePrototypes
    {
        ln::Ref<LNWS_SpotLightSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_SpotLightPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_SpotLightUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNSpotLight_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSpotLight_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSpotLight_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSpotLight_SubclassRegistrationInfo* subclassInfo() { static LNSpotLight_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_SpotLight()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_SpotLight()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNSpotLight_OnSerialize_OverrideCallback s_LNSpotLight_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNSpotLight_OnSerialize_OverrideCallback) s_LNSpotLight_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::SpotLight::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::SpotLight::onSerialize(ar);
    }

    static LNSpotLight_OnPreUpdate_OverrideCallback s_LNSpotLight_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNSpotLight_OnPreUpdate_OverrideCallback) s_LNSpotLight_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::SpotLight::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::SpotLight::onPreUpdate();
    }

    static LNSpotLight_OnUpdate_OverrideCallback s_LNSpotLight_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNSpotLight_OnUpdate_OverrideCallback) s_LNSpotLight_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::SpotLight::onUpdate(elapsedSeconds);
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

LNSpotLight_OnSerialize_OverrideCallback LNWS_ln_SpotLight::s_LNSpotLight_OnSerialize_OverrideCallback = nullptr;
LNSpotLight_OnPreUpdate_OverrideCallback LNWS_ln_SpotLight::s_LNSpotLight_OnPreUpdate_OverrideCallback = nullptr;
LNSpotLight_OnUpdate_OverrideCallback LNWS_ln_SpotLight::s_LNSpotLight_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_TestDelegate : public ln::TestDelegate
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTestDelegate_OverridePrototypes
    {

    };
    std::unique_ptr<LNTestDelegate_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTestDelegate_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTestDelegate_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTestDelegate_SubclassRegistrationInfo* subclassInfo() { static LNTestDelegate_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNTestDelegateCallback m_callback;

    LNWS_ln_TestDelegate()
      : ln::TestDelegate([this](int p1) -> int
    {
        int ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1, &ret);
        if (r != LN_OK) { LN_ERROR("LNTestDelegateCallback"); }
        return ret;
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_TestDelegate()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNTestDelegateCallback callback)
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


LN_FLAT_API LNResult LNTestDelegate_Create(LNTestDelegateCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_TestDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_Sprite : public ln::Sprite
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSprite_OverridePrototypes
    {
        ln::Ref<LNWS_SpriteSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_SpritePreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_SpriteUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNSprite_OverridePrototypes> m_overridePrototypes = nullptr;
    LNSprite_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNSprite_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNSprite_SubclassRegistrationInfo* subclassInfo() { static LNSprite_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Sprite()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Sprite()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNSprite_OnSerialize_OverrideCallback s_LNSprite_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNSprite_OnSerialize_OverrideCallback) s_LNSprite_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Sprite::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Sprite::onSerialize(ar);
    }

    static LNSprite_OnPreUpdate_OverrideCallback s_LNSprite_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNSprite_OnPreUpdate_OverrideCallback) s_LNSprite_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Sprite::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::Sprite::onPreUpdate();
    }

    static LNSprite_OnUpdate_OverrideCallback s_LNSprite_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNSprite_OnUpdate_OverrideCallback) s_LNSprite_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::Sprite::onUpdate(elapsedSeconds);
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

LNSprite_OnSerialize_OverrideCallback LNWS_ln_Sprite::s_LNSprite_OnSerialize_OverrideCallback = nullptr;
LNSprite_OnPreUpdate_OverrideCallback LNWS_ln_Sprite::s_LNSprite_OnPreUpdate_OverrideCallback = nullptr;
LNSprite_OnUpdate_OverrideCallback LNWS_ln_Sprite::s_LNSprite_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_CameraOrbitControlComponent : public ln::CameraOrbitControlComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCameraOrbitControlComponent_OverridePrototypes
    {
        ln::Ref<LNWS_CameraOrbitControlComponentSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNCameraOrbitControlComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCameraOrbitControlComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCameraOrbitControlComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCameraOrbitControlComponent_SubclassRegistrationInfo* subclassInfo() { static LNCameraOrbitControlComponent_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_CameraOrbitControlComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_CameraOrbitControlComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNCameraOrbitControlComponent_OnSerialize_OverrideCallback s_LNCameraOrbitControlComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNCameraOrbitControlComponent_OnSerialize_OverrideCallback) s_LNCameraOrbitControlComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::CameraOrbitControlComponent::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::CameraOrbitControlComponent::onSerialize(ar);
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

LNCameraOrbitControlComponent_OnSerialize_OverrideCallback LNWS_ln_CameraOrbitControlComponent::s_LNCameraOrbitControlComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Raycaster : public ln::Raycaster
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNRaycaster_OverridePrototypes
    {
        ln::Ref<LNWS_RaycasterSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNRaycaster_OverridePrototypes> m_overridePrototypes = nullptr;
    LNRaycaster_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNRaycaster_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNRaycaster_SubclassRegistrationInfo* subclassInfo() { static LNRaycaster_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Raycaster()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Raycaster()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNRaycaster_OnSerialize_OverrideCallback s_LNRaycaster_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNRaycaster_OnSerialize_OverrideCallback) s_LNRaycaster_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Raycaster::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Raycaster::onSerialize(ar);
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

LNRaycaster_OnSerialize_OverrideCallback LNWS_ln_Raycaster::s_LNRaycaster_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_RaycastResult : public ln::RaycastResult
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNRaycastResult_OverridePrototypes
    {
        ln::Ref<LNWS_RaycastResultSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNRaycastResult_OverridePrototypes> m_overridePrototypes = nullptr;
    LNRaycastResult_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNRaycastResult_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNRaycastResult_SubclassRegistrationInfo* subclassInfo() { static LNRaycastResult_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_RaycastResult()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_RaycastResult()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNRaycastResult_OnSerialize_OverrideCallback s_LNRaycastResult_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNRaycastResult_OnSerialize_OverrideCallback) s_LNRaycastResult_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::RaycastResult::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::RaycastResult::onSerialize(ar);
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

LNRaycastResult_OnSerialize_OverrideCallback LNWS_ln_RaycastResult::s_LNRaycastResult_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_WorldRenderView : public ln::WorldRenderView
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNWorldRenderView_OverridePrototypes
    {
        ln::Ref<LNWS_WorldRenderViewSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNWorldRenderView_OverridePrototypes> m_overridePrototypes = nullptr;
    LNWorldRenderView_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNWorldRenderView_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNWorldRenderView_SubclassRegistrationInfo* subclassInfo() { static LNWorldRenderView_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_WorldRenderView()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_WorldRenderView()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNWorldRenderView_OnSerialize_OverrideCallback s_LNWorldRenderView_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNWorldRenderView_OnSerialize_OverrideCallback) s_LNWorldRenderView_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::WorldRenderView::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::WorldRenderView::onSerialize(ar);
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

LNWorldRenderView_OnSerialize_OverrideCallback LNWS_ln_WorldRenderView::s_LNWorldRenderView_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ShapeObject : public ln::ShapeObject
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNShapeObject_OverridePrototypes
    {
        ln::Ref<LNWS_ShapeObjectSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_ShapeObjectPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_ShapeObjectUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNShapeObject_OverridePrototypes> m_overridePrototypes = nullptr;
    LNShapeObject_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNShapeObject_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNShapeObject_SubclassRegistrationInfo* subclassInfo() { static LNShapeObject_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_ShapeObject()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ShapeObject()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNShapeObject_OnSerialize_OverrideCallback s_LNShapeObject_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNShapeObject_OnSerialize_OverrideCallback) s_LNShapeObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::ShapeObject::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::ShapeObject::onSerialize(ar);
    }

    static LNShapeObject_OnPreUpdate_OverrideCallback s_LNShapeObject_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNShapeObject_OnPreUpdate_OverrideCallback) s_LNShapeObject_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::ShapeObject::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::ShapeObject::onPreUpdate();
    }

    static LNShapeObject_OnUpdate_OverrideCallback s_LNShapeObject_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNShapeObject_OnUpdate_OverrideCallback) s_LNShapeObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::ShapeObject::onUpdate(elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::ShapeObject::onUpdate(elapsedSeconds);
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

LNShapeObject_OnSerialize_OverrideCallback LNWS_ln_ShapeObject::s_LNShapeObject_OnSerialize_OverrideCallback = nullptr;
LNShapeObject_OnPreUpdate_OverrideCallback LNWS_ln_ShapeObject::s_LNShapeObject_OnPreUpdate_OverrideCallback = nullptr;
LNShapeObject_OnUpdate_OverrideCallback LNWS_ln_ShapeObject::s_LNShapeObject_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_PlaneMesh : public ln::PlaneMesh
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPlaneMesh_OverridePrototypes
    {
        ln::Ref<LNWS_PlaneMeshSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_PlaneMeshPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_PlaneMeshUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNPlaneMesh_OverridePrototypes> m_overridePrototypes = nullptr;
    LNPlaneMesh_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNPlaneMesh_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNPlaneMesh_SubclassRegistrationInfo* subclassInfo() { static LNPlaneMesh_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_PlaneMesh()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_PlaneMesh()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNPlaneMesh_OnSerialize_OverrideCallback s_LNPlaneMesh_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNPlaneMesh_OnSerialize_OverrideCallback) s_LNPlaneMesh_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::PlaneMesh::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::PlaneMesh::onSerialize(ar);
    }

    static LNPlaneMesh_OnPreUpdate_OverrideCallback s_LNPlaneMesh_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNPlaneMesh_OnPreUpdate_OverrideCallback) s_LNPlaneMesh_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::PlaneMesh::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::PlaneMesh::onPreUpdate();
    }

    static LNPlaneMesh_OnUpdate_OverrideCallback s_LNPlaneMesh_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNPlaneMesh_OnUpdate_OverrideCallback) s_LNPlaneMesh_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::PlaneMesh::onUpdate(elapsedSeconds);
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
            return ln::TypeInfo::getTypeInfo<ShapeObject>();
    }

};

LNPlaneMesh_OnSerialize_OverrideCallback LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnSerialize_OverrideCallback = nullptr;
LNPlaneMesh_OnPreUpdate_OverrideCallback LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnPreUpdate_OverrideCallback = nullptr;
LNPlaneMesh_OnUpdate_OverrideCallback LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_BoxMesh : public ln::BoxMesh
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNBoxMesh_OverridePrototypes
    {
        ln::Ref<LNWS_BoxMeshSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_BoxMeshPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_BoxMeshUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNBoxMesh_OverridePrototypes> m_overridePrototypes = nullptr;
    LNBoxMesh_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNBoxMesh_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNBoxMesh_SubclassRegistrationInfo* subclassInfo() { static LNBoxMesh_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_BoxMesh()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_BoxMesh()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNBoxMesh_OnSerialize_OverrideCallback s_LNBoxMesh_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNBoxMesh_OnSerialize_OverrideCallback) s_LNBoxMesh_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::BoxMesh::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::BoxMesh::onSerialize(ar);
    }

    static LNBoxMesh_OnPreUpdate_OverrideCallback s_LNBoxMesh_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNBoxMesh_OnPreUpdate_OverrideCallback) s_LNBoxMesh_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::BoxMesh::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::BoxMesh::onPreUpdate();
    }

    static LNBoxMesh_OnUpdate_OverrideCallback s_LNBoxMesh_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNBoxMesh_OnUpdate_OverrideCallback) s_LNBoxMesh_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::BoxMesh::onUpdate(elapsedSeconds);
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
            return ln::TypeInfo::getTypeInfo<StaticMesh>();
    }

};

LNBoxMesh_OnSerialize_OverrideCallback LNWS_ln_BoxMesh::s_LNBoxMesh_OnSerialize_OverrideCallback = nullptr;
LNBoxMesh_OnPreUpdate_OverrideCallback LNWS_ln_BoxMesh::s_LNBoxMesh_OnPreUpdate_OverrideCallback = nullptr;
LNBoxMesh_OnUpdate_OverrideCallback LNWS_ln_BoxMesh::s_LNBoxMesh_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_MeshComponent : public ln::MeshComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNMeshComponent_OverridePrototypes
    {
        ln::Ref<LNWS_MeshComponentSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNMeshComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LNMeshComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNMeshComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNMeshComponent_SubclassRegistrationInfo* subclassInfo() { static LNMeshComponent_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_MeshComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_MeshComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNMeshComponent_OnSerialize_OverrideCallback s_LNMeshComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNMeshComponent_OnSerialize_OverrideCallback) s_LNMeshComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::MeshComponent::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::MeshComponent::onSerialize(ar);
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

LNMeshComponent_OnSerialize_OverrideCallback LNWS_ln_MeshComponent::s_LNMeshComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Collision : public ln::Collision
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCollision_OverridePrototypes
    {
        ln::Ref<LNWS_CollisionSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNCollision_OverridePrototypes> m_overridePrototypes = nullptr;
    LNCollision_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNCollision_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNCollision_SubclassRegistrationInfo* subclassInfo() { static LNCollision_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Collision()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Collision()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNCollision_OnSerialize_OverrideCallback s_LNCollision_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNCollision_OnSerialize_OverrideCallback) s_LNCollision_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Collision::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Collision::onSerialize(ar);
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

LNCollision_OnSerialize_OverrideCallback LNWS_ln_Collision::s_LNCollision_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_TriggerBodyComponent : public ln::TriggerBodyComponent
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNTriggerBodyComponent_OverridePrototypes
    {
        ln::Ref<LNWS_TriggerBodyComponentSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNTriggerBodyComponent_OverridePrototypes> m_overridePrototypes = nullptr;
    LNTriggerBodyComponent_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNTriggerBodyComponent_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNTriggerBodyComponent_SubclassRegistrationInfo* subclassInfo() { static LNTriggerBodyComponent_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_TriggerBodyComponent()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_TriggerBodyComponent()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNTriggerBodyComponent_OnSerialize_OverrideCallback s_LNTriggerBodyComponent_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNTriggerBodyComponent_OnSerialize_OverrideCallback) s_LNTriggerBodyComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::TriggerBodyComponent::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::TriggerBodyComponent::onSerialize(ar);
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

LNTriggerBodyComponent_OnSerialize_OverrideCallback LNWS_ln_TriggerBodyComponent::s_LNTriggerBodyComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ParticleEmitter : public ln::ParticleEmitter
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNParticleEmitter_OverridePrototypes
    {
        ln::Ref<LNWS_ParticleEmitterSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_ParticleEmitterPreUpdateHandler> OnPreUpdate_OverrideFunc;
        ln::Ref<LNWS_ParticleEmitterUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNParticleEmitter_OverridePrototypes> m_overridePrototypes = nullptr;
    LNParticleEmitter_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNParticleEmitter_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNParticleEmitter_SubclassRegistrationInfo* subclassInfo() { static LNParticleEmitter_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_ParticleEmitter()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_ParticleEmitter()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNParticleEmitter_OnSerialize_OverrideCallback s_LNParticleEmitter_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNParticleEmitter_OnSerialize_OverrideCallback) s_LNParticleEmitter_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::ParticleEmitter::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::ParticleEmitter::onSerialize(ar);
    }

    static LNParticleEmitter_OnPreUpdate_OverrideCallback s_LNParticleEmitter_OnPreUpdate_OverrideCallback; // deprecated
    virtual void onPreUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPreUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNParticleEmitter_OnPreUpdate_OverrideCallback) s_LNParticleEmitter_OnPreUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::ParticleEmitter::onPreUpdate();
    }
    void onPreUpdate_CallBase()
    {
        ln::ParticleEmitter::onPreUpdate();
    }

    static LNParticleEmitter_OnUpdate_OverrideCallback s_LNParticleEmitter_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this, elapsedSeconds);
                return;
            }
        }
        if (s_LNParticleEmitter_OnUpdate_OverrideCallback) s_LNParticleEmitter_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
        ln::ParticleEmitter::onUpdate(elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::ParticleEmitter::onUpdate(elapsedSeconds);
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

LNParticleEmitter_OnSerialize_OverrideCallback LNWS_ln_ParticleEmitter::s_LNParticleEmitter_OnSerialize_OverrideCallback = nullptr;
LNParticleEmitter_OnPreUpdate_OverrideCallback LNWS_ln_ParticleEmitter::s_LNParticleEmitter_OnPreUpdate_OverrideCallback = nullptr;
LNParticleEmitter_OnUpdate_OverrideCallback LNWS_ln_ParticleEmitter::s_LNParticleEmitter_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_Scene : public ln::Scene
{
public:
};



class LNWS_ln_Level : public ln::Level
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNLevel_OverridePrototypes
    {
        ln::Ref<LNWS_LevelSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_LevelStartHandler> OnStart_OverrideFunc;
        ln::Ref<LNWS_LevelStopHandler> OnStop_OverrideFunc;
        ln::Ref<LNWS_LevelPauseHandler> OnPause_OverrideFunc;
        ln::Ref<LNWS_LevelResumeHandler> OnResume_OverrideFunc;
        ln::Ref<LNWS_LevelUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNLevel_OverridePrototypes> m_overridePrototypes = nullptr;
    LNLevel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNLevel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNLevel_SubclassRegistrationInfo* subclassInfo() { static LNLevel_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Level()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Level()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNLevel_OnSerialize_OverrideCallback s_LNLevel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNLevel_OnSerialize_OverrideCallback) s_LNLevel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Level::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Level::onSerialize(ar);
    }

    static LNLevel_OnStart_OverrideCallback s_LNLevel_OnStart_OverrideCallback; // deprecated
    virtual void onStart() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnStart_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNLevel_OnStart_OverrideCallback) s_LNLevel_OnStart_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Level::onStart();
    }
    void onStart_CallBase()
    {
        ln::Level::onStart();
    }

    static LNLevel_OnStop_OverrideCallback s_LNLevel_OnStop_OverrideCallback; // deprecated
    virtual void onStop() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnStop_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNLevel_OnStop_OverrideCallback) s_LNLevel_OnStop_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Level::onStop();
    }
    void onStop_CallBase()
    {
        ln::Level::onStop();
    }

    static LNLevel_OnPause_OverrideCallback s_LNLevel_OnPause_OverrideCallback; // deprecated
    virtual void onPause() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnPause_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNLevel_OnPause_OverrideCallback) s_LNLevel_OnPause_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Level::onPause();
    }
    void onPause_CallBase()
    {
        ln::Level::onPause();
    }

    static LNLevel_OnResume_OverrideCallback s_LNLevel_OnResume_OverrideCallback; // deprecated
    virtual void onResume() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnResume_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNLevel_OnResume_OverrideCallback) s_LNLevel_OnResume_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Level::onResume();
    }
    void onResume_CallBase()
    {
        ln::Level::onResume();
    }

    static LNLevel_OnUpdate_OverrideCallback s_LNLevel_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNLevel_OnUpdate_OverrideCallback) s_LNLevel_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Level::onUpdate();
    }
    void onUpdate_CallBase()
    {
        ln::Level::onUpdate();
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
            return ln::TypeInfo::getTypeInfo<AssetObject>();
    }

};

LNLevel_OnSerialize_OverrideCallback LNWS_ln_Level::s_LNLevel_OnSerialize_OverrideCallback = nullptr;
LNLevel_OnStart_OverrideCallback LNWS_ln_Level::s_LNLevel_OnStart_OverrideCallback = nullptr;
LNLevel_OnStop_OverrideCallback LNWS_ln_Level::s_LNLevel_OnStop_OverrideCallback = nullptr;
LNLevel_OnPause_OverrideCallback LNWS_ln_Level::s_LNLevel_OnPause_OverrideCallback = nullptr;
LNLevel_OnResume_OverrideCallback LNWS_ln_Level::s_LNLevel_OnResume_OverrideCallback = nullptr;
LNLevel_OnUpdate_OverrideCallback LNWS_ln_Level::s_LNLevel_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_UIColors : public ln::UIColors
{
public:
};



class LNWS_ln_UIEventArgs : public ln::UIEventArgs
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIEventArgs_OverridePrototypes
    {
        ln::Ref<LNWS_UIEventArgsSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIEventArgs_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIEventArgs_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIEventArgs_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIEventArgs_SubclassRegistrationInfo* subclassInfo() { static LNUIEventArgs_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIEventArgs()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIEventArgs()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIEventArgs_OnSerialize_OverrideCallback s_LNUIEventArgs_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIEventArgs_OnSerialize_OverrideCallback) s_LNUIEventArgs_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIEventArgs::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIEventArgs::onSerialize(ar);
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

LNUIEventArgs_OnSerialize_OverrideCallback LNWS_ln_UIEventArgs::s_LNUIEventArgs_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIGeneralEventHandler : public ln::UIGeneralEventHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIGeneralEventHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIGeneralEventHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIGeneralEventHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIGeneralEventHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIGeneralEventHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIGeneralEventHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIGeneralEventHandlerCallback m_callback;

    LNWS_ln_UIGeneralEventHandler()
      : ln::UIGeneralEventHandler([this](ln::UIEventArgs* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1));
        if (r != LN_OK) { LN_ERROR("LNUIGeneralEventHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIGeneralEventHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIGeneralEventHandlerCallback callback)
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


LN_FLAT_API LNResult LNUIGeneralEventHandler_Create(LNUIGeneralEventHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_UIGeneralEventHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_UIEventHandler : public ln::UIEventHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIEventHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUIEventHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIEventHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIEventHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIEventHandler_SubclassRegistrationInfo* subclassInfo() { static LNUIEventHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUIEventHandlerCallback m_callback;

    LNWS_ln_UIEventHandler()
      : ln::UIEventHandler([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_OK) { LN_ERROR("LNUIEventHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIEventHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUIEventHandlerCallback callback)
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


LN_FLAT_API LNResult LNUIEventHandler_Create(LNUIEventHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_UIEventHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_UILayoutElement : public ln::UILayoutElement
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUILayoutElement_OverridePrototypes
    {
        ln::Ref<LNWS_UILayoutElementSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUILayoutElement_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUILayoutElement_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUILayoutElement_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUILayoutElement_SubclassRegistrationInfo* subclassInfo() { static LNUILayoutElement_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UILayoutElement()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UILayoutElement()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUILayoutElement_OnSerialize_OverrideCallback s_LNUILayoutElement_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUILayoutElement_OnSerialize_OverrideCallback) s_LNUILayoutElement_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UILayoutElement::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UILayoutElement::onSerialize(ar);
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

LNUILayoutElement_OnSerialize_OverrideCallback LNWS_ln_UILayoutElement::s_LNUILayoutElement_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIElement : public ln::UIElement
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIElement_OverridePrototypes
    {
        ln::Ref<LNWS_UIElementSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIElement_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIElement_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIElement_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIElement_SubclassRegistrationInfo* subclassInfo() { static LNUIElement_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIElement()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIElement()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIElement_OnSerialize_OverrideCallback s_LNUIElement_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIElement_OnSerialize_OverrideCallback) s_LNUIElement_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIElement::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIElement::onSerialize(ar);
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

LNUIElement_OnSerialize_OverrideCallback LNWS_ln_UIElement::s_LNUIElement_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIText : public ln::UIText
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIText_OverridePrototypes
    {
        ln::Ref<LNWS_UITextSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIText_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIText_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIText_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIText_SubclassRegistrationInfo* subclassInfo() { static LNUIText_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIText()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIText()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIText_OnSerialize_OverrideCallback s_LNUIText_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIText_OnSerialize_OverrideCallback) s_LNUIText_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIText::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIText::onSerialize(ar);
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

LNUIText_OnSerialize_OverrideCallback LNWS_ln_UIText::s_LNUIText_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UISprite : public ln::UISprite
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUISprite_OverridePrototypes
    {
        ln::Ref<LNWS_UISpriteSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUISprite_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUISprite_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUISprite_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUISprite_SubclassRegistrationInfo* subclassInfo() { static LNUISprite_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UISprite()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UISprite()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUISprite_OnSerialize_OverrideCallback s_LNUISprite_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUISprite_OnSerialize_OverrideCallback) s_LNUISprite_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UISprite::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UISprite::onSerialize(ar);
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

LNUISprite_OnSerialize_OverrideCallback LNWS_ln_UISprite::s_LNUISprite_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIIcon : public ln::UIIcon
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIIcon_OverridePrototypes
    {
        ln::Ref<LNWS_UIIconSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIIcon_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIIcon_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIIcon_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIIcon_SubclassRegistrationInfo* subclassInfo() { static LNUIIcon_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIIcon()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIIcon()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIIcon_OnSerialize_OverrideCallback s_LNUIIcon_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIIcon_OnSerialize_OverrideCallback) s_LNUIIcon_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIIcon::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIIcon::onSerialize(ar);
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

LNUIIcon_OnSerialize_OverrideCallback LNWS_ln_UIIcon::s_LNUIIcon_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIMessageTextArea : public ln::UIMessageTextArea
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIMessageTextArea_OverridePrototypes
    {
        ln::Ref<LNWS_UIMessageTextAreaSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIMessageTextArea_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIMessageTextArea_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIMessageTextArea_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIMessageTextArea_SubclassRegistrationInfo* subclassInfo() { static LNUIMessageTextArea_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIMessageTextArea()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIMessageTextArea()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIMessageTextArea_OnSerialize_OverrideCallback s_LNUIMessageTextArea_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIMessageTextArea_OnSerialize_OverrideCallback) s_LNUIMessageTextArea_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIMessageTextArea::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIMessageTextArea::onSerialize(ar);
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

LNUIMessageTextArea_OnSerialize_OverrideCallback LNWS_ln_UIMessageTextArea::s_LNUIMessageTextArea_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UI : public ln::UI
{
public:
};



class LNWS_ln_UILayoutPanel : public ln::UILayoutPanel
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUILayoutPanel_OverridePrototypes
    {
        ln::Ref<LNWS_UILayoutPanelSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUILayoutPanel_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUILayoutPanel_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUILayoutPanel_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUILayoutPanel_SubclassRegistrationInfo* subclassInfo() { static LNUILayoutPanel_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UILayoutPanel()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UILayoutPanel()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUILayoutPanel_OnSerialize_OverrideCallback s_LNUILayoutPanel_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUILayoutPanel_OnSerialize_OverrideCallback) s_LNUILayoutPanel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UILayoutPanel::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UILayoutPanel::onSerialize(ar);
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

LNUILayoutPanel_OnSerialize_OverrideCallback LNWS_ln_UILayoutPanel::s_LNUILayoutPanel_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIBoxLayout : public ln::UIBoxLayout
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIBoxLayout_OverridePrototypes
    {
        ln::Ref<LNWS_UIBoxLayoutSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIBoxLayout_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIBoxLayout_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIBoxLayout_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIBoxLayout_SubclassRegistrationInfo* subclassInfo() { static LNUIBoxLayout_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIBoxLayout()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIBoxLayout()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIBoxLayout_OnSerialize_OverrideCallback s_LNUIBoxLayout_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIBoxLayout_OnSerialize_OverrideCallback) s_LNUIBoxLayout_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIBoxLayout::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIBoxLayout::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UILayoutPanel>();
    }

};

LNUIBoxLayout_OnSerialize_OverrideCallback LNWS_ln_UIBoxLayout::s_LNUIBoxLayout_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIStackLayout : public ln::UIStackLayout
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIStackLayout_OverridePrototypes
    {
        ln::Ref<LNWS_UIStackLayoutSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIStackLayout_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIStackLayout_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIStackLayout_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIStackLayout_SubclassRegistrationInfo* subclassInfo() { static LNUIStackLayout_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIStackLayout()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIStackLayout()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIStackLayout_OnSerialize_OverrideCallback s_LNUIStackLayout_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIStackLayout_OnSerialize_OverrideCallback) s_LNUIStackLayout_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIStackLayout::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIStackLayout::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UILayoutPanel>();
    }

};

LNUIStackLayout_OnSerialize_OverrideCallback LNWS_ln_UIStackLayout::s_LNUIStackLayout_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIGridLayout : public ln::UIGridLayout
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIGridLayout_OverridePrototypes
    {
        ln::Ref<LNWS_UIGridLayoutSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIGridLayout_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIGridLayout_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIGridLayout_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIGridLayout_SubclassRegistrationInfo* subclassInfo() { static LNUIGridLayout_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIGridLayout()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIGridLayout()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIGridLayout_OnSerialize_OverrideCallback s_LNUIGridLayout_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIGridLayout_OnSerialize_OverrideCallback) s_LNUIGridLayout_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIGridLayout::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIGridLayout::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UILayoutPanel>();
    }

};

LNUIGridLayout_OnSerialize_OverrideCallback LNWS_ln_UIGridLayout::s_LNUIGridLayout_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIControl : public ln::UIControl
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIControl_OverridePrototypes
    {
        ln::Ref<LNWS_UIControlSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIControl_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIControl_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIControl_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIControl_SubclassRegistrationInfo* subclassInfo() { static LNUIControl_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIControl()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIControl()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIControl_OnSerialize_OverrideCallback s_LNUIControl_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIControl_OnSerialize_OverrideCallback) s_LNUIControl_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIControl::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIControl::onSerialize(ar);
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

LNUIControl_OnSerialize_OverrideCallback LNWS_ln_UIControl::s_LNUIControl_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIButtonBase : public ln::UIButtonBase
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIButtonBase_OverridePrototypes
    {
        ln::Ref<LNWS_UIButtonBaseSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIButtonBase_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIButtonBase_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIButtonBase_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIButtonBase_SubclassRegistrationInfo* subclassInfo() { static LNUIButtonBase_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIButtonBase()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIButtonBase()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIButtonBase_OnSerialize_OverrideCallback s_LNUIButtonBase_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIButtonBase_OnSerialize_OverrideCallback) s_LNUIButtonBase_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIButtonBase::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIButtonBase::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UIControl>();
    }

};

LNUIButtonBase_OnSerialize_OverrideCallback LNWS_ln_UIButtonBase::s_LNUIButtonBase_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIButton : public ln::UIButton
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIButton_OverridePrototypes
    {
        ln::Ref<LNWS_UIButtonSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIButton_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIButton_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIButton_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIButton_SubclassRegistrationInfo* subclassInfo() { static LNUIButton_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIButton()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIButton()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIButton_OnSerialize_OverrideCallback s_LNUIButton_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIButton_OnSerialize_OverrideCallback) s_LNUIButton_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIButton::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIButton::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UIButtonBase>();
    }

};

LNUIButton_OnSerialize_OverrideCallback LNWS_ln_UIButton::s_LNUIButton_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIWindow : public ln::UIWindow
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIWindow_OverridePrototypes
    {
        ln::Ref<LNWS_UIWindowSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIWindow_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIWindow_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIWindow_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIWindow_SubclassRegistrationInfo* subclassInfo() { static LNUIWindow_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIWindow()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIWindow()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIWindow_OnSerialize_OverrideCallback s_LNUIWindow_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIWindow_OnSerialize_OverrideCallback) s_LNUIWindow_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIWindow::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIWindow::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UIControl>();
    }

};

LNUIWindow_OnSerialize_OverrideCallback LNWS_ln_UIWindow::s_LNUIWindow_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIListItem : public ln::UIListItem
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListItem_OverridePrototypes
    {
        ln::Ref<LNWS_UIListItemSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIListItem_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListItem_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListItem_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListItem_SubclassRegistrationInfo* subclassInfo() { static LNUIListItem_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIListItem()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIListItem()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIListItem_OnSerialize_OverrideCallback s_LNUIListItem_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIListItem_OnSerialize_OverrideCallback) s_LNUIListItem_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIListItem::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIListItem::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UIControl>();
    }

};

LNUIListItem_OnSerialize_OverrideCallback LNWS_ln_UIListItem::s_LNUIListItem_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIListItemsControl : public ln::UIListItemsControl
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListItemsControl_OverridePrototypes
    {
        ln::Ref<LNWS_UIListItemsControlSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIListItemsControl_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListItemsControl_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListItemsControl_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListItemsControl_SubclassRegistrationInfo* subclassInfo() { static LNUIListItemsControl_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIListItemsControl()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIListItemsControl()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIListItemsControl_OnSerialize_OverrideCallback s_LNUIListItemsControl_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIListItemsControl_OnSerialize_OverrideCallback) s_LNUIListItemsControl_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIListItemsControl::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIListItemsControl::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UIControl>();
    }

};

LNUIListItemsControl_OnSerialize_OverrideCallback LNWS_ln_UIListItemsControl::s_LNUIListItemsControl_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIListBoxItem : public ln::UIListBoxItem
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListBoxItem_OverridePrototypes
    {
        ln::Ref<LNWS_UIListBoxItemSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIListBoxItem_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListBoxItem_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListBoxItem_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListBoxItem_SubclassRegistrationInfo* subclassInfo() { static LNUIListBoxItem_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIListBoxItem()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIListBoxItem()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIListBoxItem_OnSerialize_OverrideCallback s_LNUIListBoxItem_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIListBoxItem_OnSerialize_OverrideCallback) s_LNUIListBoxItem_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIListBoxItem::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIListBoxItem::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UIListItem>();
    }

};

LNUIListBoxItem_OnSerialize_OverrideCallback LNWS_ln_UIListBoxItem::s_LNUIListBoxItem_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIListBox : public ln::UIListBox
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUIListBox_OverridePrototypes
    {
        ln::Ref<LNWS_UIListBoxSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUIListBox_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUIListBox_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUIListBox_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUIListBox_SubclassRegistrationInfo* subclassInfo() { static LNUIListBox_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UIListBox()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UIListBox()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUIListBox_OnSerialize_OverrideCallback s_LNUIListBox_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUIListBox_OnSerialize_OverrideCallback) s_LNUIListBox_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UIListBox::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UIListBox::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<UIListItemsControl>();
    }

};

LNUIListBox_OnSerialize_OverrideCallback LNWS_ln_UIListBox::s_LNUIListBox_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_InputGesture : public ln::InputGesture
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInputGesture_OverridePrototypes
    {
        ln::Ref<LNWS_InputGestureSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNInputGesture_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInputGesture_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInputGesture_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInputGesture_SubclassRegistrationInfo* subclassInfo() { static LNInputGesture_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_InputGesture()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_InputGesture()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNInputGesture_OnSerialize_OverrideCallback s_LNInputGesture_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNInputGesture_OnSerialize_OverrideCallback) s_LNInputGesture_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::InputGesture::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::InputGesture::onSerialize(ar);
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

LNInputGesture_OnSerialize_OverrideCallback LNWS_ln_InputGesture::s_LNInputGesture_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_KeyGesture : public ln::KeyGesture
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNKeyGesture_OverridePrototypes
    {
        ln::Ref<LNWS_KeyGestureSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNKeyGesture_OverridePrototypes> m_overridePrototypes = nullptr;
    LNKeyGesture_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNKeyGesture_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNKeyGesture_SubclassRegistrationInfo* subclassInfo() { static LNKeyGesture_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_KeyGesture()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_KeyGesture()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNKeyGesture_OnSerialize_OverrideCallback s_LNKeyGesture_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNKeyGesture_OnSerialize_OverrideCallback) s_LNKeyGesture_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::KeyGesture::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::KeyGesture::onSerialize(ar);
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
            return ln::TypeInfo::getTypeInfo<InputGesture>();
    }

};

LNKeyGesture_OnSerialize_OverrideCallback LNWS_ln_KeyGesture::s_LNKeyGesture_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Input : public ln::Input
{
public:
};



class LNWS_ln_Mouse : public ln::Mouse
{
public:
};



class LNWS_ln_InterpreterCommand : public ln::InterpreterCommand
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreterCommand_OverridePrototypes
    {
        ln::Ref<LNWS_InterpreterCommandSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNInterpreterCommand_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreterCommand_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreterCommand_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreterCommand_SubclassRegistrationInfo* subclassInfo() { static LNInterpreterCommand_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_InterpreterCommand()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_InterpreterCommand()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNInterpreterCommand_OnSerialize_OverrideCallback s_LNInterpreterCommand_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNInterpreterCommand_OnSerialize_OverrideCallback) s_LNInterpreterCommand_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::InterpreterCommand::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::InterpreterCommand::onSerialize(ar);
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

LNInterpreterCommand_OnSerialize_OverrideCallback LNWS_ln_InterpreterCommand::s_LNInterpreterCommand_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_InterpreterCommandList : public ln::InterpreterCommandList
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreterCommandList_OverridePrototypes
    {
        ln::Ref<LNWS_InterpreterCommandListSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNInterpreterCommandList_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreterCommandList_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreterCommandList_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreterCommandList_SubclassRegistrationInfo* subclassInfo() { static LNInterpreterCommandList_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_InterpreterCommandList()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_InterpreterCommandList()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNInterpreterCommandList_OnSerialize_OverrideCallback s_LNInterpreterCommandList_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNInterpreterCommandList_OnSerialize_OverrideCallback) s_LNInterpreterCommandList_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::InterpreterCommandList::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::InterpreterCommandList::onSerialize(ar);
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

LNInterpreterCommandList_OnSerialize_OverrideCallback LNWS_ln_InterpreterCommandList::s_LNInterpreterCommandList_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_InterpreterCommandDelegate : public ln::InterpreterCommandDelegate
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreterCommandDelegate_OverridePrototypes
    {

    };
    std::unique_ptr<LNInterpreterCommandDelegate_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreterCommandDelegate_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreterCommandDelegate_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreterCommandDelegate_SubclassRegistrationInfo* subclassInfo() { static LNInterpreterCommandDelegate_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNInterpreterCommandDelegateCallback m_callback;

    LNWS_ln_InterpreterCommandDelegate()
      : ln::InterpreterCommandDelegate([this](ln::InterpreterCommand* p1) -> bool
    {
        LNBool ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1), &ret);
        if (r != LN_OK) { LN_ERROR("LNInterpreterCommandDelegateCallback"); }
        return ret != LN_FALSE;
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_InterpreterCommandDelegate()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNInterpreterCommandDelegateCallback callback)
    {
        if (!ln::InterpreterCommandDelegate::init()) return false;
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


LN_FLAT_API LNResult LNInterpreterCommandDelegate_Create(LNInterpreterCommandDelegateCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_InterpreterCommandDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}


class LNWS_ln_Interpreter : public ln::Interpreter
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNInterpreter_OverridePrototypes
    {
        ln::Ref<LNWS_InterpreterSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_InterpreterUpdateWaitHandler> OnUpdateWait_OverrideFunc;

    };
    std::unique_ptr<LNInterpreter_OverridePrototypes> m_overridePrototypes = nullptr;
    LNInterpreter_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNInterpreter_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNInterpreter_SubclassRegistrationInfo* subclassInfo() { static LNInterpreter_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Interpreter()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Interpreter()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNInterpreter_OnSerialize_OverrideCallback s_LNInterpreter_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNInterpreter_OnSerialize_OverrideCallback) s_LNInterpreter_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Interpreter::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Interpreter::onSerialize(ar);
    }

    static LNInterpreter_OnUpdateWait_OverrideCallback s_LNInterpreter_OnUpdateWait_OverrideCallback; // deprecated
    virtual bool onUpdateWait() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdateWait_OverrideFunc) {
                return func->call(this);
            }
        }
        return ln::Interpreter::onUpdateWait();
    }
    bool onUpdateWait_CallBase()
    {
        return ln::Interpreter::onUpdateWait();
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

LNInterpreter_OnSerialize_OverrideCallback LNWS_ln_Interpreter::s_LNInterpreter_OnSerialize_OverrideCallback = nullptr;
LNInterpreter_OnUpdateWait_OverrideCallback LNWS_ln_Interpreter::s_LNInterpreter_OnUpdateWait_OverrideCallback = nullptr;


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
    struct LNApplication_OverridePrototypes
    {
        ln::Ref<LNWS_ApplicationSerializeHandler> OnSerialize_OverrideFunc;
        ln::Ref<LNWS_ApplicationInitHandler> OnInit_OverrideFunc;
        ln::Ref<LNWS_ApplicationUpdateHandler> OnUpdate_OverrideFunc;

    };
    std::unique_ptr<LNApplication_OverridePrototypes> m_overridePrototypes = nullptr;
    LNApplication_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNApplication_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNApplication_SubclassRegistrationInfo* subclassInfo() { static LNApplication_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_Application()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_Application()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNApplication_OnSerialize_OverrideCallback s_LNApplication_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNApplication_OnSerialize_OverrideCallback) s_LNApplication_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::Application::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::Application::onSerialize(ar);
    }

    static LNApplication_OnInit_OverrideCallback s_LNApplication_OnInit_OverrideCallback; // deprecated
    virtual void onInit() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnInit_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNApplication_OnInit_OverrideCallback) s_LNApplication_OnInit_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Application::onInit();
    }
    void onInit_CallBase()
    {
        ln::Application::onInit();
    }

    static LNApplication_OnUpdate_OverrideCallback s_LNApplication_OnUpdate_OverrideCallback; // deprecated
    virtual void onUpdate() override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnUpdate_OverrideFunc) {
                func->call(this);
                return;
            }
        }
        if (s_LNApplication_OnUpdate_OverrideCallback) s_LNApplication_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
        ln::Application::onUpdate();
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

LNApplication_OnSerialize_OverrideCallback LNWS_ln_Application::s_LNApplication_OnSerialize_OverrideCallback = nullptr;
LNApplication_OnInit_OverrideCallback LNWS_ln_Application::s_LNApplication_OnInit_OverrideCallback = nullptr;
LNApplication_OnUpdate_OverrideCallback LNWS_ln_Application::s_LNApplication_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_Debug : public ln::Debug
{
public:
};





extern "C"
{

LN_FLAT_API LNResult LNVector3_SetZeros(LNVector3* vector3)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector3*>(vector3)) ln::Vector3();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVector3_Set(LNVector3* vector3, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector3*>(vector3)) ln::Vector3(x, y, z);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVector3_Get(const LNVector3* vector3, float* outX, float* outY, float* outZ)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<const ln::Vector3*>(vector3)->get(outX, outY, outZ));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVector3_Length(const LNVector3* vector3, float* outReturn)
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


LN_FLAT_API LNResult LNVector3_LengthSquared(const LNVector3* vector3, float* outReturn)
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


LN_FLAT_API LNResult LNVector3_MutatingNormalize(LNVector3* vector3)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::Vector3*>(vector3)->mutatingNormalize());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVector3_NormalizeXYZ(float x, float y, float z, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(ln::Vector3::normalize(x, y, z));
    }
    else {
        (ln::Vector3::normalize(x, y, z));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVector3_Normalize(const LNVector3* vec, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(ln::Vector3::normalize(*reinterpret_cast<const ln::Vector3*>(vec)));
    }
    else {
        (ln::Vector3::normalize(*reinterpret_cast<const ln::Vector3*>(vec)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVector4_SetZeros(LNVector4* vector4)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector4*>(vector4)) ln::Vector4();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVector4_Set(LNVector4* vector4, float x, float y, float z, float w)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Vector4*>(vector4)) ln::Vector4(x, y, z, w);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNQuaternion_SetZeros(LNQuaternion* quaternion)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Quaternion*>(quaternion)) ln::Quaternion();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNQuaternion_Set(LNQuaternion* quaternion, float x, float y, float z, float w)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Quaternion*>(quaternion)) ln::Quaternion(x, y, z, w);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNQuaternion_SetFromAxis(LNQuaternion* quaternion, const LNVector3* axis, float r)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Quaternion*>(quaternion)) ln::Quaternion(*reinterpret_cast<const ln::Vector3*>(axis), r);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMatrix_SetZeros(LNMatrix* matrix)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Matrix*>(matrix)) ln::Matrix();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMatrix_Set(LNMatrix* matrix, float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Matrix*>(matrix)) ln::Matrix(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNColor_SetZeros(LNColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Color*>(color)) ln::Color();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNColor_Set(LNColor* color, float r_, float g_, float b_, float a_)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Color*>(color)) ln::Color(r_, g_, b_, a_);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNColorTone_SetZeros(LNColorTone* colortone)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::ColorTone*>(colortone)) ln::ColorTone();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNColorTone_Set(LNColorTone* colortone, float r_, float g_, float b_, float s_)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::ColorTone*>(colortone)) ln::ColorTone(r_, g_, b_, s_);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPoint_SetZeros(LNPoint* point)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Point*>(point)) ln::Point();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPoint_Set(LNPoint* point, float x_, float y_)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Point*>(point)) ln::Point(x_, y_);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPoint_Get(const LNPoint* point, float* outX, float* outY)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<const ln::Point*>(point)->get(outX, outY));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSize_SetZeros(LNSize* size)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Size*>(size)) ln::Size();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSize_Set(LNSize* size, float w, float h)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Size*>(size)) ln::Size(w, h);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNRect_SetZeros(LNRect* rect)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Rect*>(rect)) ln::Rect();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNRect_Set(LNRect* rect, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Rect*>(rect)) ln::Rect(x, y, width, height);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNRect_GetLeft(const LNRect* rect, float* outReturn)
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


LN_FLAT_API LNResult LNRect_SetSize(LNRect* rect, const LNSize* size)
{
    LNI_FUNC_TRY_BEGIN;
    (reinterpret_cast<ln::Rect*>(rect)->setSize(*reinterpret_cast<const ln::Size*>(size)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNRect_GetSize(const LNRect* rect, LNSize* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNSize>(reinterpret_cast<const ln::Rect*>(rect)->getSize());
    }
    else {
        (reinterpret_cast<const ln::Rect*>(rect)->getSize());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNThickness_SetZeros(LNThickness* thickness)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Thickness*>(thickness)) ln::Thickness();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNThickness_Set(LNThickness* thickness, float left_, float top_, float right_, float bottom_)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::Thickness*>(thickness)) ln::Thickness(left_, top_, right_, bottom_);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCornerRadius_SetZeros(LNCornerRadius* cornerradius)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::CornerRadius*>(cornerradius)) ln::CornerRadius();
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCornerRadius_Set(LNCornerRadius* cornerradius, float topLeft, float topRight, float bottomRight, float bottomLeft)
{
    LNI_FUNC_TRY_BEGIN;
    new (reinterpret_cast<ln::CornerRadius*>(cornerradius)) ln::CornerRadius(topLeft, topRight, bottomRight, bottomLeft);
    LNI_FUNC_TRY_END_RETURN;
}




LN_FLAT_API LNResult LNObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNObject_OnSerialize_SetOverrideCallback(LNObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Object::s_LNObject_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNObject_SetPrototype_OnSerialize(LNHandle object, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ObjectSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Object>()->id();
}

LN_FLAT_API void LNObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Object>(), id);
}

void LNObject_RegisterSubclassTypeInfo(const LNObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Object>(), info->subclassId);
        *LNWS_ln_Object::subclassInfo() = *info;
    }
}

LNSubinstanceId LNObject_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNEventConnection_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNEventConnection_OnSerialize_SetOverrideCallback(LNEventConnection_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_EventConnection::s_LNEventConnection_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNEventConnection_SetPrototype_OnSerialize(LNHandle eventconnection, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, eventconnection)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EventConnectionSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNEventConnection_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::EventConnection>()->id();
}

LN_FLAT_API void LNEventConnection_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EventConnection>(), id);
}

void LNEventConnection_RegisterSubclassTypeInfo(const LNEventConnection_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EventConnection>(), info->subclassId);
        *LNWS_ln_EventConnection::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEventConnection_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNPromiseFailureDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PromiseFailureDelegate>(), id);
}

void LNPromiseFailureDelegate_RegisterSubclassTypeInfo(const LNPromiseFailureDelegate_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PromiseFailureDelegate>(), info->subclassId);
        *LNWS_ln_PromiseFailureDelegate::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPromiseFailureDelegate_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_PromiseFailureDelegate, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNVariant_Create(LNHandle* outVariant)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outVariant, LNWS_ln_Variant, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVariant_SetInt(LNHandle variant, int value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Variant, variant)->setInt(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVariant_GetInt(LNHandle variant, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Variant, variant)->getInt());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Variant, variant)->getInt());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVariant_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Variant, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNVariant_OnSerialize_SetOverrideCallback(LNVariant_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Variant::s_LNVariant_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNVariant_SetPrototype_OnSerialize(LNHandle variant, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Variant, variant)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VariantSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNVariant_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Variant>()->id();
}

LN_FLAT_API void LNVariant_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Variant>(), id);
}

void LNVariant_RegisterSubclassTypeInfo(const LNVariant_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Variant>(), info->subclassId);
        *LNWS_ln_Variant::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVariant_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Variant, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNZVTestDelegate1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate1>(), id);
}

void LNZVTestDelegate1_RegisterSubclassTypeInfo(const LNZVTestDelegate1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate1>(), info->subclassId);
        *LNWS_ln_ZVTestDelegate1::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestDelegate1_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestDelegate1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNZVTestDelegate2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate2>(), id);
}

void LNZVTestDelegate2_RegisterSubclassTypeInfo(const LNZVTestDelegate2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate2>(), info->subclassId);
        *LNWS_ln_ZVTestDelegate2::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestDelegate2_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestDelegate2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNZVTestDelegate3_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate3>(), id);
}

void LNZVTestDelegate3_RegisterSubclassTypeInfo(const LNZVTestDelegate3_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate3>(), info->subclassId);
        *LNWS_ln_ZVTestDelegate3::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestDelegate3_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestDelegate3, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNZVTestEventHandler1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler1>(), id);
}

void LNZVTestEventHandler1_RegisterSubclassTypeInfo(const LNZVTestEventHandler1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler1>(), info->subclassId);
        *LNWS_ln_ZVTestEventHandler1::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestEventHandler1_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventHandler1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNZVTestEventHandler2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler2>(), id);
}

void LNZVTestEventHandler2_RegisterSubclassTypeInfo(const LNZVTestEventHandler2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventHandler2>(), info->subclassId);
        *LNWS_ln_ZVTestEventHandler2::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestEventHandler2_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventHandler2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNZVTestPromise1_ThenWith(LNHandle zvtestpromise1, LNHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise1, zvtestpromise1)->thenWith(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate3, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestPromise1_CatchWith(LNHandle zvtestpromise1, LNHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise1, zvtestpromise1)->catchWith(LNI_HANDLE_TO_OBJECT(ln::PromiseFailureDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


extern LN_FLAT_API int LNZVTestPromise1_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestPromise1>()->id();
}

LN_FLAT_API void LNZVTestPromise1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise1>(), id);
}

void LNZVTestPromise1_RegisterSubclassTypeInfo(const LNZVTestPromise1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise1>(), info->subclassId);
        *LNWS_ln_ZVTestPromise1::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestPromise1_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNZVTestPromise2_ThenWith(LNHandle zvtestpromise2, LNHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise2, zvtestpromise2)->thenWith(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate1, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestPromise2_CatchWith(LNHandle zvtestpromise2, LNHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise2, zvtestpromise2)->catchWith(LNI_HANDLE_TO_OBJECT(ln::PromiseFailureDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


extern LN_FLAT_API int LNZVTestPromise2_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestPromise2>()->id();
}

LN_FLAT_API void LNZVTestPromise2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise2>(), id);
}

void LNZVTestPromise2_RegisterSubclassTypeInfo(const LNZVTestPromise2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestPromise2>(), info->subclassId);
        *LNWS_ln_ZVTestPromise2::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestPromise2_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestPromise2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNZVTestClass1_Create(LNHandle* outZVTestClass1)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outZVTestClass1, LNWS_ln_ZVTestClass1, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate1(LNHandle zvtestclass1, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->setTestDelegate1(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate1, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate2(LNHandle zvtestclass1, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->setTestDelegate2(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate2, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_SetTestDelegate3(LNHandle zvtestclass1, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->setTestDelegate3(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate3, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate1(LNHandle zvtestclass1, int a)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->callTestDelegate1(a));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate2(LNHandle zvtestclass1, int a, int b, int* outReturn)
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


LN_FLAT_API LNResult LNZVTestClass1_CallTestDelegate3(LNHandle zvtestclass1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->callTestDelegate3());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_LoadAsync(const LNChar* filePath, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNZVTestClass1_LoadAsyncA(const char* filePath, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::ZVTestClass1::loadAsync(LNI_ASTRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::ZVTestClass1::loadAsync(LNI_ASTRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_ExecuteAsync(LNHandle zvtestclass1, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNZVTestClass1_GetFilePath(LNHandle zvtestclass1, const LNChar** outReturn)
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


LN_FLAT_API LNResult LNZVTestClass1_GetFilePathA(LNHandle zvtestclass1, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_A(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->filePath());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->filePath());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_ConnectOnEvent1(LNHandle zvtestclass1, LNHandle handler, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNZVTestClass1_RaiseEvent1(LNHandle zvtestclass1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->raiseEvent1());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_ConnectOnEvent2(LNHandle zvtestclass1, LNHandle handler, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNZVTestClass1_RaiseEvent2(LNHandle zvtestclass1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->raiseEvent2());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestClass1_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNZVTestClass1_OnSerialize_SetOverrideCallback(LNZVTestClass1_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ZVTestClass1::s_LNZVTestClass1_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNZVTestClass1_SetPrototype_OnSerialize(LNHandle zvtestclass1, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ZVTestClass1SerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNZVTestClass1_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>()->id();
}

LN_FLAT_API void LNZVTestClass1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>(), id);
}

void LNZVTestClass1_RegisterSubclassTypeInfo(const LNZVTestClass1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>(), info->subclassId);
        *LNWS_ln_ZVTestClass1::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestClass1_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNZVTestEventArgs1_Create(LNHandle* outZVTestEventArgs1)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outZVTestEventArgs1, LNWS_ln_ZVTestEventArgs1, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestEventArgs1_CreateWithValue(int v, LNHandle* outZVTestEventArgs1)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outZVTestEventArgs1, LNWS_ln_ZVTestEventArgs1, init, v);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNZVTestEventArgs1_GetValue(LNHandle zvtesteventargs1, int* outReturn)
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


LN_FLAT_API LNResult LNZVTestEventArgs1_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNZVTestEventArgs1_OnSerialize_SetOverrideCallback(LNZVTestEventArgs1_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ZVTestEventArgs1::s_LNZVTestEventArgs1_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNZVTestEventArgs1_SetPrototype_OnSerialize(LNHandle zvtesteventargs1, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, zvtesteventargs1)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ZVTestEventArgs1SerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNZVTestEventArgs1_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestEventArgs1>()->id();
}

LN_FLAT_API void LNZVTestEventArgs1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventArgs1>(), id);
}

void LNZVTestEventArgs1_RegisterSubclassTypeInfo(const LNZVTestEventArgs1_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestEventArgs1>(), info->subclassId);
        *LNWS_ln_ZVTestEventArgs1::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestEventArgs1_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNLog_SetLevel(LNLogLevel level)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Log::setLevel(static_cast<ln::LogLevel>(level)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLog_AllocConsole()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Log::allocConsole());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLog_Write(LNLogLevel level, const LNChar* tag, const LNChar* text)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Log::write(static_cast<ln::LogLevel>(level), tag, text));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLog_WriteA(LNLogLevel level, const char* tag, const char* text)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Log::write(static_cast<ln::LogLevel>(level), LNI_ASTRPTR_TO_STRING(tag), LNI_ASTRPTR_TO_STRING(text)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSerializer2_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSerializer2_OnSerialize_SetOverrideCallback(LNSerializer2_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Serializer2::s_LNSerializer2_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSerializer2_SetPrototype_OnSerialize(LNHandle serializer2, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, serializer2)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_Serializer2SerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNSerializer2_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Serializer2>()->id();
}

LN_FLAT_API void LNSerializer2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Serializer2>(), id);
}

void LNSerializer2_RegisterSubclassTypeInfo(const LNSerializer2_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Serializer2>(), info->subclassId);
        *LNWS_ln_Serializer2::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSerializer2_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAssetObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNAssetObject_OnSerialize_SetOverrideCallback(LNAssetObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AssetObject::s_LNAssetObject_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNAssetObject_SetPrototype_OnSerialize(LNHandle assetobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetObject, assetobject)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AssetObjectSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNAssetObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AssetObject>()->id();
}

LN_FLAT_API void LNAssetObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetObject>(), id);
}

void LNAssetObject_RegisterSubclassTypeInfo(const LNAssetObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetObject>(), info->subclassId);
        *LNWS_ln_AssetObject::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAssetObject_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetObject, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAssetImportSettings_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetImportSettings, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNAssetImportSettings_OnSerialize_SetOverrideCallback(LNAssetImportSettings_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AssetImportSettings::s_LNAssetImportSettings_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNAssetImportSettings_SetPrototype_OnSerialize(LNHandle assetimportsettings, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetImportSettings, assetimportsettings)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AssetImportSettingsSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNAssetImportSettings_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AssetImportSettings>()->id();
}

LN_FLAT_API void LNAssetImportSettings_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetImportSettings>(), id);
}

void LNAssetImportSettings_RegisterSubclassTypeInfo(const LNAssetImportSettings_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetImportSettings>(), info->subclassId);
        *LNWS_ln_AssetImportSettings::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAssetImportSettings_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetImportSettings, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAssetModel_Create(LNHandle target, LNHandle* outAssetModel)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outAssetModel, LNWS_ln_AssetModel, init, LNI_HANDLE_TO_OBJECT(ln::Object, target));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssetModel_Target(LNHandle assetmodel, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNAssetModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNAssetModel_OnSerialize_SetOverrideCallback(LNAssetModel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AssetModel::s_LNAssetModel_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNAssetModel_SetPrototype_OnSerialize(LNHandle assetmodel, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, assetmodel)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AssetModelSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNAssetModel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AssetModel>()->id();
}

LN_FLAT_API void LNAssetModel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetModel>(), id);
}

void LNAssetModel_RegisterSubclassTypeInfo(const LNAssetModel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetModel>(), info->subclassId);
        *LNWS_ln_AssetModel::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAssetModel_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAssets_SaveAssetToLocalFile(LNHandle asset, const LNChar* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::saveAssetToLocalFile(LNI_HANDLE_TO_OBJECT(ln::AssetModel, asset), filePath));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssets_SaveAssetToLocalFileA(LNHandle asset, const char* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::saveAssetToLocalFile(LNI_HANDLE_TO_OBJECT(ln::AssetModel, asset), LNI_ASTRPTR_TO_STRING(filePath)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssets_LoadAssetFromLocalFile(const LNChar* filePath, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNAssets_LoadAssetFromLocalFileA(const char* filePath, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAssetFromLocalFile(LNI_ASTRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Assets::loadAssetFromLocalFile(LNI_ASTRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssets_LoadAsset(const LNChar* filePath, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNAssets_LoadAssetA(const char* filePath, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAsset(LNI_ASTRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Assets::loadAsset(LNI_ASTRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssets_ReloadAsset(const LNChar* filePath, LNHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::reloadAsset(filePath, LNI_HANDLE_TO_OBJECT(ln::Object, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssets_ReloadAssetA(const char* filePath, LNHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Assets::reloadAsset(LNI_ASTRPTR_TO_STRING(filePath), LNI_HANDLE_TO_OBJECT(ln::Object, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssets_ReadAllText(const LNChar* filePath, LNEncodingType encoding, const LNChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(ln::Assets::readAllText(filePath, static_cast<ln::EncodingType>(encoding)));
    }
    else {
        (ln::Assets::readAllText(filePath, static_cast<ln::EncodingType>(encoding)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAssets_ReadAllTextA(const char* filePath, LNEncodingType encoding, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_A(ln::Assets::readAllText(LNI_ASTRPTR_TO_STRING(filePath), static_cast<ln::EncodingType>(encoding)));
    }
    else {
        (ln::Assets::readAllText(LNI_ASTRPTR_TO_STRING(filePath), static_cast<ln::EncodingType>(encoding)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_SetVolume(LNHandle sound, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->setVolume(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_GetVolume(LNHandle sound, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->getVolume());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->getVolume());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_SetPitch(LNHandle sound, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->setPitch(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_GetPitch(LNHandle sound, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->getPitch());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->getPitch());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_SetLoopEnabled(LNHandle sound, LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->setLoopEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_IsLoopEnabled(LNHandle sound, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->isLoopEnabled());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->isLoopEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_SetLoopRange(LNHandle sound, uint32_t begin, uint32_t length)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->SetLoopRange(begin, length));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_Play(LNHandle sound)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->play());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_Stop(LNHandle sound)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->stop());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_Pause(LNHandle sound)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->pause());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_Resume(LNHandle sound)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->resume());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_FadeVolume(LNHandle sound, float targetVolume, float time, LNSoundFadeBehavior behavior)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->fadeVolume(targetVolume, time, static_cast<ln::SoundFadeBehavior>(behavior)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSound_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSound_OnSerialize_SetOverrideCallback(LNSound_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Sound::s_LNSound_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSound_SetPrototype_OnSerialize(LNHandle sound, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, sound)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SoundSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNSound_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Sound>()->id();
}

LN_FLAT_API void LNSound_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sound>(), id);
}

void LNSound_RegisterSubclassTypeInfo(const LNSound_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sound>(), info->subclassId);
        *LNWS_ln_Sound::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSound_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sound, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAudio_PlayBGM(const LNChar* filePath, float volume, float pitch, double fadeTime)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playBGM(filePath, volume, pitch, fadeTime));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlayBGMA(const char* filePath, float volume, float pitch, double fadeTime)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playBGM(LNI_ASTRPTR_TO_STRING(filePath), volume, pitch, fadeTime));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_StopBGM(double fadeTime)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::stopBGM(fadeTime));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlayBGS(const LNChar* filePath, float volume, float pitch, double fadeTime)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playBGS(filePath, volume, pitch, fadeTime));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlayBGSA(const char* filePath, float volume, float pitch, double fadeTime)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playBGS(LNI_ASTRPTR_TO_STRING(filePath), volume, pitch, fadeTime));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_StopBGS(double fadeTime)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::stopBGS(fadeTime));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlayME(const LNChar* filePath, float volume, float pitch)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playME(filePath, volume, pitch));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlayMEA(const char* filePath, float volume, float pitch)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playME(LNI_ASTRPTR_TO_STRING(filePath), volume, pitch));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_StopME()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::stopME());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlaySE(const LNChar* filePath, float volume, float pitch)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playSE(filePath, volume, pitch));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlaySEA(const char* filePath, float volume, float pitch)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playSE(LNI_ASTRPTR_TO_STRING(filePath), volume, pitch));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlaySE3D(const LNChar* filePath, const LNVector3* position, float distance, float volume, float pitch)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playSE3D(filePath, *reinterpret_cast<const ln::Vector3*>(position), distance, volume, pitch));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_PlaySE3DA(const char* filePath, const LNVector3* position, float distance, float volume, float pitch)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::playSE3D(LNI_ASTRPTR_TO_STRING(filePath), *reinterpret_cast<const ln::Vector3*>(position), distance, volume, pitch));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAudio_StopSE()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Audio::stopSE());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API void LNTexture2DDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DDelegate>(), id);
}

void LNTexture2DDelegate_RegisterSubclassTypeInfo(const LNTexture2DDelegate_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DDelegate>(), info->subclassId);
        *LNWS_ln_Texture2DDelegate::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTexture2DDelegate_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DDelegate, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNTexture2DPromise_ThenWith(LNHandle texture2dpromise, LNHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DPromise, texture2dpromise)->thenWith(LNI_HANDLE_TO_OBJECT(ln::Texture2DDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTexture2DPromise_CatchWith(LNHandle texture2dpromise, LNHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DPromise, texture2dpromise)->catchWith(LNI_HANDLE_TO_OBJECT(ln::PromiseFailureDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


extern LN_FLAT_API int LNTexture2DPromise_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture2DPromise>()->id();
}

LN_FLAT_API void LNTexture2DPromise_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DPromise>(), id);
}

void LNTexture2DPromise_RegisterSubclassTypeInfo(const LNTexture2DPromise_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2DPromise>(), info->subclassId);
        *LNWS_ln_Texture2DPromise::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTexture2DPromise_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2DPromise, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNGraphics_GetActiveGraphicsAPI(LNGraphicsAPI* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNGraphicsAPI>(ln::Graphics::activeGraphicsAPI());
    }
    else {
        (ln::Graphics::activeGraphicsAPI());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTexture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNTexture_OnSerialize_SetOverrideCallback(LNTexture_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Texture::s_LNTexture_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNTexture_SetPrototype_OnSerialize(LNHandle texture, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, texture)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_TextureSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNTexture_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture>()->id();
}

LN_FLAT_API void LNTexture_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture>(), id);
}

void LNTexture_RegisterSubclassTypeInfo(const LNTexture_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture>(), info->subclassId);
        *LNWS_ln_Texture::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTexture_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNTexture2D_Create(int width, int height, LNHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, width, height);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTexture2D_CreateWithFormat(int width, int height, LNTextureFormat format, LNHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, width, height, static_cast<ln::TextureFormat>(format));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTexture2D_Load(const LNChar* filePath, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNTexture2D_LoadA(const char* filePath, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::load(LNI_ASTRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Texture2D::load(LNI_ASTRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTexture2D_LoadEmoji(const LNChar* code, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNTexture2D_LoadEmojiA(const char* code, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::loadEmoji(LNI_ASTRPTR_TO_STRING(code)));
    }
    else {
        (ln::Texture2D::loadEmoji(LNI_ASTRPTR_TO_STRING(code)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTexture2D_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNTexture2D_OnSerialize_SetOverrideCallback(LNTexture2D_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Texture2D::s_LNTexture2D_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNTexture2D_SetPrototype_OnSerialize(LNHandle texture2d, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, texture2d)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_Texture2DSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNTexture2D_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture2D>()->id();
}

LN_FLAT_API void LNTexture2D_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2D>(), id);
}

void LNTexture2D_RegisterSubclassTypeInfo(const LNTexture2D_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2D>(), info->subclassId);
        *LNWS_ln_Texture2D::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTexture2D_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNShader_Load(const LNChar* filePath, LNHandle settings, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Shader::load(filePath, LNI_HANDLE_TO_OBJECT(ln::AssetImportSettings, settings)));
    }
    else {
        (ln::Shader::load(filePath, LNI_HANDLE_TO_OBJECT(ln::AssetImportSettings, settings)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_LoadA(const char* filePath, LNHandle settings, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Shader::load(LNI_ASTRPTR_TO_STRING(filePath), LNI_HANDLE_TO_OBJECT(ln::AssetImportSettings, settings)));
    }
    else {
        (ln::Shader::load(LNI_ASTRPTR_TO_STRING(filePath), LNI_HANDLE_TO_OBJECT(ln::AssetImportSettings, settings)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetFloat(LNHandle shader, const LNChar* parameterName, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setFloat(parameterName, value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetFloatA(LNHandle shader, const char* parameterName, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setFloat(LNI_ASTRPTR_TO_STRING(parameterName), value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetVector3(LNHandle shader, const LNChar* parameterName, const LNVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setVector(parameterName, *reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetVector3A(LNHandle shader, const char* parameterName, const LNVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setVector(LNI_ASTRPTR_TO_STRING(parameterName), *reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetVector4(LNHandle shader, const LNChar* parameterName, const LNVector4* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setVector(parameterName, *reinterpret_cast<const ln::Vector4*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetVector4A(LNHandle shader, const char* parameterName, const LNVector4* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setVector(LNI_ASTRPTR_TO_STRING(parameterName), *reinterpret_cast<const ln::Vector4*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetTexture(LNHandle shader, const LNChar* parameterName, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setTexture(parameterName, LNI_HANDLE_TO_OBJECT(ln::Texture, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_SetTextureA(LNHandle shader, const char* parameterName, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->setTexture(LNI_ASTRPTR_TO_STRING(parameterName), LNI_HANDLE_TO_OBJECT(ln::Texture, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNShader_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNShader_OnSerialize_SetOverrideCallback(LNShader_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Shader::s_LNShader_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNShader_SetPrototype_OnSerialize(LNHandle shader, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, shader)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ShaderSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNShader_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Shader>()->id();
}

LN_FLAT_API void LNShader_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Shader>(), id);
}

void LNShader_RegisterSubclassTypeInfo(const LNShader_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Shader>(), info->subclassId);
        *LNWS_ln_Shader::subclassInfo() = *info;
    }
}

LNSubinstanceId LNShader_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Shader, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNRenderView_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNRenderView_OnSerialize_SetOverrideCallback(LNRenderView_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_RenderView::s_LNRenderView_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNRenderView_SetPrototype_OnSerialize(LNHandle renderview, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, renderview)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_RenderViewSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNRenderView_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::RenderView>()->id();
}

LN_FLAT_API void LNRenderView_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RenderView>(), id);
}

void LNRenderView_RegisterSubclassTypeInfo(const LNRenderView_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RenderView>(), info->subclassId);
        *LNWS_ln_RenderView::subclassInfo() = *info;
    }
}

LNSubinstanceId LNRenderView_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNMaterial_Create(LNHandle* outMaterial)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outMaterial, LNWS_ln_Material, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_SetMainTexture(LNHandle material, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->setMainTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_GetMainTexture(LNHandle material, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->mainTexture());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->mainTexture());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_SetColor(LNHandle material, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->setColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_SetRoughness(LNHandle material, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->setRoughness(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_SetMetallic(LNHandle material, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->setMetallic(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_SetEmissive(LNHandle material, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->setEmissive(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_SetShadingModel(LNHandle material, LNShadingModel value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->setShadingModel(static_cast<ln::ShadingModel>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_GetShadingModel(LNHandle material, LNShadingModel* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNShadingModel>(LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->shadingModel());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->shadingModel());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_SetShader(LNHandle material, LNHandle shader)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->setShader(LNI_HANDLE_TO_OBJECT(ln::Shader, shader)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_GetShader(LNHandle material, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->shader());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->shader());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMaterial_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNMaterial_OnSerialize_SetOverrideCallback(LNMaterial_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Material::s_LNMaterial_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNMaterial_SetPrototype_OnSerialize(LNHandle material, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, material)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_MaterialSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNMaterial_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Material>()->id();
}

LN_FLAT_API void LNMaterial_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Material>(), id);
}

void LNMaterial_RegisterSubclassTypeInfo(const LNMaterial_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Material>(), info->subclassId);
        *LNWS_ln_Material::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMaterial_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Material, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNMeshNode_SetVisible(LNHandle meshnode, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshNode, meshnode)->setVisible(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshNode_IsVisible(LNHandle meshnode, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshNode, meshnode)->isVisible());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshNode, meshnode)->isVisible());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshNode_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshNode, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNMeshNode_OnSerialize_SetOverrideCallback(LNMeshNode_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_MeshNode::s_LNMeshNode_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNMeshNode_SetPrototype_OnSerialize(LNHandle meshnode, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshNode, meshnode)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_MeshNodeSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNMeshNode_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::MeshNode>()->id();
}

LN_FLAT_API void LNMeshNode_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshNode>(), id);
}

void LNMeshNode_RegisterSubclassTypeInfo(const LNMeshNode_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshNode>(), info->subclassId);
        *LNWS_ln_MeshNode::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshNode_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshNode, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAnimationController_AddClip(LNHandle animationcontroller, LNHandle animationClip, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationController, animationcontroller)->addClip(LNI_HANDLE_TO_OBJECT(ln::AnimationClip, animationClip)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationController, animationcontroller)->addClip(LNI_HANDLE_TO_OBJECT(ln::AnimationClip, animationClip)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationController_Play(LNHandle animationcontroller, LNHandle state, float duration)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationController, animationcontroller)->play(LNI_HANDLE_TO_OBJECT(ln::AnimationState, state), duration));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationController_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationController, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNAnimationController_OnSerialize_SetOverrideCallback(LNAnimationController_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AnimationController::s_LNAnimationController_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNAnimationController_SetPrototype_OnSerialize(LNHandle animationcontroller, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationController, animationcontroller)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AnimationControllerSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNAnimationController_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AnimationController>()->id();
}

LN_FLAT_API void LNAnimationController_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationController>(), id);
}

void LNAnimationController_RegisterSubclassTypeInfo(const LNAnimationController_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationController>(), info->subclassId);
        *LNWS_ln_AnimationController::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationController_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationController, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNMeshModel_Load(const LNChar* filePath, LNHandle settings, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::MeshModel::load(filePath, LNI_HANDLE_TO_OBJECT(ln::MeshImportSettings, settings)));
    }
    else {
        (ln::MeshModel::load(filePath, LNI_HANDLE_TO_OBJECT(ln::MeshImportSettings, settings)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_LoadA(const char* filePath, LNHandle settings, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::MeshModel::load(LNI_ASTRPTR_TO_STRING(filePath), LNI_HANDLE_TO_OBJECT(ln::MeshImportSettings, settings)));
    }
    else {
        (ln::MeshModel::load(LNI_ASTRPTR_TO_STRING(filePath), LNI_HANDLE_TO_OBJECT(ln::MeshImportSettings, settings)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_FindNode(LNHandle meshmodel, const LNChar* name, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findNode(name));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findNode(name));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_FindNodeA(LNHandle meshmodel, const char* name, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findNode(LNI_ASTRPTR_TO_STRING(name)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findNode(LNI_ASTRPTR_TO_STRING(name)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_FindMaterial(LNHandle meshmodel, const LNChar* name, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findMaterial(name));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findMaterial(name));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_FindMaterialA(LNHandle meshmodel, const char* name, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findMaterial(LNI_ASTRPTR_TO_STRING(name)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->findMaterial(LNI_ASTRPTR_TO_STRING(name)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_MaterialCount(LNHandle meshmodel, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->materialCount());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->materialCount());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_Material(LNHandle meshmodel, int index, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->material(index));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->material(index));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_GetAnimationController(LNHandle meshmodel, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->animationController());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->animationController());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNMeshModel_OnSerialize_SetOverrideCallback(LNMeshModel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_MeshModel::s_LNMeshModel_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNMeshModel_SetPrototype_OnSerialize(LNHandle meshmodel, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, meshmodel)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_MeshModelSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNMeshModel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::MeshModel>()->id();
}

LN_FLAT_API void LNMeshModel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshModel>(), id);
}

void LNMeshModel_RegisterSubclassTypeInfo(const LNMeshModel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshModel>(), info->subclassId);
        *LNWS_ln_MeshModel::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshModel_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshModel, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNMeshImportSettings_Create(LNHandle* outMeshImportSettings)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outMeshImportSettings, LNWS_ln_MeshImportSettings, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshImportSettings_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshImportSettings, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNMeshImportSettings_OnSerialize_SetOverrideCallback(LNMeshImportSettings_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_MeshImportSettings::s_LNMeshImportSettings_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNMeshImportSettings_SetPrototype_OnSerialize(LNHandle meshimportsettings, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshImportSettings, meshimportsettings)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_MeshImportSettingsSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNMeshImportSettings_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::MeshImportSettings>()->id();
}

LN_FLAT_API void LNMeshImportSettings_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshImportSettings>(), id);
}

void LNMeshImportSettings_RegisterSubclassTypeInfo(const LNMeshImportSettings_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshImportSettings>(), info->subclassId);
        *LNWS_ln_MeshImportSettings::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshImportSettings_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshImportSettings, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNSkinnedMeshModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SkinnedMeshModel, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSkinnedMeshModel_OnSerialize_SetOverrideCallback(LNSkinnedMeshModel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_SkinnedMeshModel::s_LNSkinnedMeshModel_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSkinnedMeshModel_SetPrototype_OnSerialize(LNHandle skinnedmeshmodel, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SkinnedMeshModel, skinnedmeshmodel)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SkinnedMeshModelSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNSkinnedMeshModel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::SkinnedMeshModel>()->id();
}

LN_FLAT_API void LNSkinnedMeshModel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SkinnedMeshModel>(), id);
}

void LNSkinnedMeshModel_RegisterSubclassTypeInfo(const LNSkinnedMeshModel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SkinnedMeshModel>(), info->subclassId);
        *LNWS_ln_SkinnedMeshModel::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSkinnedMeshModel_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_SkinnedMeshModel, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNCollisionShape_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CollisionShape, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNCollisionShape_OnSerialize_SetOverrideCallback(LNCollisionShape_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_CollisionShape::s_LNCollisionShape_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNCollisionShape_SetPrototype_OnSerialize(LNHandle collisionshape, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_CollisionShape, collisionshape)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CollisionShapeSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNCollisionShape_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::CollisionShape>()->id();
}

LN_FLAT_API void LNCollisionShape_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CollisionShape>(), id);
}

void LNCollisionShape_RegisterSubclassTypeInfo(const LNCollisionShape_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CollisionShape>(), info->subclassId);
        *LNWS_ln_CollisionShape::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCollisionShape_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_CollisionShape, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNBoxCollisionShape_Create(const LNVector3* size, LNHandle* outBoxCollisionShape)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outBoxCollisionShape, LNWS_ln_BoxCollisionShape, init, *reinterpret_cast<const ln::Vector3*>(size));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNBoxCollisionShape_CreateWHD(float width, float height, float depth, LNHandle* outBoxCollisionShape)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outBoxCollisionShape, LNWS_ln_BoxCollisionShape, init, width, height, depth);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNBoxCollisionShape_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxCollisionShape, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNBoxCollisionShape_OnSerialize_SetOverrideCallback(LNBoxCollisionShape_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_BoxCollisionShape::s_LNBoxCollisionShape_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNBoxCollisionShape_SetPrototype_OnSerialize(LNHandle boxcollisionshape, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxCollisionShape, boxcollisionshape)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_BoxCollisionShapeSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNBoxCollisionShape_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::BoxCollisionShape>()->id();
}

LN_FLAT_API void LNBoxCollisionShape_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::BoxCollisionShape>(), id);
}

void LNBoxCollisionShape_RegisterSubclassTypeInfo(const LNBoxCollisionShape_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::BoxCollisionShape>(), info->subclassId);
        *LNWS_ln_BoxCollisionShape::subclassInfo() = *info;
    }
}

LNSubinstanceId LNBoxCollisionShape_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxCollisionShape, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAnimationCurve_Evaluate(LNHandle animationcurve, float time, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationCurve, animationcurve)->evaluate(time));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationCurve, animationcurve)->evaluate(time));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationCurve_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationCurve, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNAnimationCurve_OnSerialize_SetOverrideCallback(LNAnimationCurve_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AnimationCurve::s_LNAnimationCurve_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNAnimationCurve_SetPrototype_OnSerialize(LNHandle animationcurve, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationCurve, animationcurve)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AnimationCurveSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNAnimationCurve_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AnimationCurve>()->id();
}

LN_FLAT_API void LNAnimationCurve_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationCurve>(), id);
}

void LNAnimationCurve_RegisterSubclassTypeInfo(const LNAnimationCurve_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationCurve>(), info->subclassId);
        *LNWS_ln_AnimationCurve::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationCurve_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationCurve, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNKeyFrameAnimationCurve_Create(LNHandle* outKeyFrameAnimationCurve)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outKeyFrameAnimationCurve, LNWS_ln_KeyFrameAnimationCurve, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNKeyFrameAnimationCurve_AddKeyFrame(LNHandle keyframeanimationcurve, float time, float value, LNTangentMode rightTangentMode, float tangent)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_KeyFrameAnimationCurve, keyframeanimationcurve)->addKeyFrame(time, value, static_cast<ln::TangentMode>(rightTangentMode), tangent));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNKeyFrameAnimationCurve_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_KeyFrameAnimationCurve, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNKeyFrameAnimationCurve_OnSerialize_SetOverrideCallback(LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_KeyFrameAnimationCurve::s_LNKeyFrameAnimationCurve_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNKeyFrameAnimationCurve_SetPrototype_OnSerialize(LNHandle keyframeanimationcurve, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_KeyFrameAnimationCurve, keyframeanimationcurve)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_KeyFrameAnimationCurveSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNKeyFrameAnimationCurve_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::KeyFrameAnimationCurve>()->id();
}

LN_FLAT_API void LNKeyFrameAnimationCurve_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::KeyFrameAnimationCurve>(), id);
}

void LNKeyFrameAnimationCurve_RegisterSubclassTypeInfo(const LNKeyFrameAnimationCurve_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::KeyFrameAnimationCurve>(), info->subclassId);
        *LNWS_ln_KeyFrameAnimationCurve::subclassInfo() = *info;
    }
}

LNSubinstanceId LNKeyFrameAnimationCurve_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_KeyFrameAnimationCurve, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAnimationClip_Load(const LNChar* filePath, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::AnimationClip::load(filePath));
    }
    else {
        (ln::AnimationClip::load(filePath));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationClip_LoadA(const char* filePath, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::AnimationClip::load(LNI_ASTRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::AnimationClip::load(LNI_ASTRPTR_TO_STRING(filePath)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationClip_SetWrapMode(LNHandle animationclip, LNAnimationWrapMode value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, animationclip)->setWrapMode(static_cast<ln::AnimationWrapMode>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationClip_GetWrapMode(LNHandle animationclip, LNAnimationWrapMode* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNAnimationWrapMode>(LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, animationclip)->wrapMode());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, animationclip)->wrapMode());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationClip_SetHierarchicalAnimationMode(LNHandle animationclip, LNHierarchicalAnimationMode value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, animationclip)->setHierarchicalAnimationMode(static_cast<ln::HierarchicalAnimationMode>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationClip_GetHierarchicalAnimationMode(LNHandle animationclip, LNHierarchicalAnimationMode* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNHierarchicalAnimationMode>(LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, animationclip)->hierarchicalAnimationMode());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, animationclip)->hierarchicalAnimationMode());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNAnimationClip_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNAnimationClip_OnSerialize_SetOverrideCallback(LNAnimationClip_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AnimationClip::s_LNAnimationClip_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNAnimationClip_SetPrototype_OnSerialize(LNHandle animationclip, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, animationclip)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AnimationClipSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNAnimationClip_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AnimationClip>()->id();
}

LN_FLAT_API void LNAnimationClip_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationClip>(), id);
}

void LNAnimationClip_RegisterSubclassTypeInfo(const LNAnimationClip_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationClip>(), info->subclassId);
        *LNWS_ln_AnimationClip::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationClip_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationClip, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNAnimationState_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationState, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNAnimationState_OnSerialize_SetOverrideCallback(LNAnimationState_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_AnimationState::s_LNAnimationState_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNAnimationState_SetPrototype_OnSerialize(LNHandle animationstate, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationState, animationstate)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AnimationStateSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNAnimationState_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AnimationState>()->id();
}

LN_FLAT_API void LNAnimationState_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationState>(), id);
}

void LNAnimationState_RegisterSubclassTypeInfo(const LNAnimationState_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AnimationState>(), info->subclassId);
        *LNWS_ln_AnimationState::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationState_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_AnimationState, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNEffectResource_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EffectResource, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNEffectResource_OnSerialize_SetOverrideCallback(LNEffectResource_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_EffectResource::s_LNEffectResource_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNEffectResource_SetPrototype_OnSerialize(LNHandle effectresource, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EffectResource, effectresource)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EffectResourceSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNEffectResource_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::EffectResource>()->id();
}

LN_FLAT_API void LNEffectResource_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EffectResource>(), id);
}

void LNEffectResource_RegisterSubclassTypeInfo(const LNEffectResource_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EffectResource>(), info->subclassId);
        *LNWS_ln_EffectResource::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEffectResource_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_EffectResource, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNParticleEmitterModel_Create(LNHandle* outParticleEmitterModel)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outParticleEmitterModel, LNWS_ln_ParticleEmitterModel, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetMaxParticles(LNHandle particleemittermodel, int count)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setMaxParticles(count));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetSpawnRate(LNHandle particleemittermodel, float rate)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setSpawnRate(rate));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetLifeTime(LNHandle particleemittermodel, float time)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setLifeTime(time));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetupBoxShape(LNHandle particleemittermodel, const LNVector3* size)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setupBoxShape(*reinterpret_cast<const ln::Vector3*>(size)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetSize(LNHandle particleemittermodel, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setSize(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetSizeVelocity(LNHandle particleemittermodel, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setSizeVelocity(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetSizeAcceleration(LNHandle particleemittermodel, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setSizeAcceleration(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetForwardVelocityMin(LNHandle particleemittermodel, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setForwardVelocityMin(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetForwardVelocityMax(LNHandle particleemittermodel, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setForwardVelocityMax(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetForwardScale(LNHandle particleemittermodel, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setForwardScale(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetGeometryDirection(LNHandle particleemittermodel, LNParticleGeometryDirection value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setGeometryDirection(static_cast<ln::ParticleGeometryDirection>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_SetupSpriteModule(LNHandle particleemittermodel, LNHandle material)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->setupSpriteModule(LNI_HANDLE_TO_OBJECT(ln::Material, material)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitterModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNParticleEmitterModel_OnSerialize_SetOverrideCallback(LNParticleEmitterModel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ParticleEmitterModel::s_LNParticleEmitterModel_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNParticleEmitterModel_SetPrototype_OnSerialize(LNHandle particleemittermodel, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, particleemittermodel)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterModelSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNParticleEmitterModel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ParticleEmitterModel>()->id();
}

LN_FLAT_API void LNParticleEmitterModel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ParticleEmitterModel>(), id);
}

void LNParticleEmitterModel_RegisterSubclassTypeInfo(const LNParticleEmitterModel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ParticleEmitterModel>(), info->subclassId);
        *LNWS_ln_ParticleEmitterModel::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleEmitterModel_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitterModel, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNParticleModel_Create(LNHandle* outParticleModel)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outParticleModel, LNWS_ln_ParticleModel, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleModel_SetLoop(LNHandle particlemodel, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleModel, particlemodel)->setLoop(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleModel_IsLoop(LNHandle particlemodel, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleModel, particlemodel)->isLoop());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleModel, particlemodel)->isLoop());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleModel_AddEmitter(LNHandle particlemodel, LNHandle emitter)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleModel, particlemodel)->addEmitter(LNI_HANDLE_TO_OBJECT(ln::ParticleEmitterModel, emitter)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleModel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleModel, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNParticleModel_OnSerialize_SetOverrideCallback(LNParticleModel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ParticleModel::s_LNParticleModel_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNParticleModel_SetPrototype_OnSerialize(LNHandle particlemodel, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleModel, particlemodel)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ParticleModelSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNParticleModel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ParticleModel>()->id();
}

LN_FLAT_API void LNParticleModel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ParticleModel>(), id);
}

void LNParticleModel_RegisterSubclassTypeInfo(const LNParticleModel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ParticleModel>(), info->subclassId);
        *LNWS_ln_ParticleModel::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleModel_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleModel, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNComponent_OnSerialize_SetOverrideCallback(LNComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Component::s_LNComponent_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNComponent_SetPrototype_OnSerialize(LNHandle component, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, component)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ComponentSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Component>()->id();
}

LN_FLAT_API void LNComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Component>(), id);
}

void LNComponent_RegisterSubclassTypeInfo(const LNComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Component>(), info->subclassId);
        *LNWS_ln_Component::subclassInfo() = *info;
    }
}

LNSubinstanceId LNComponent_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNVisualComponent_SetVisible(LNHandle visualcomponent, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, visualcomponent)->setVisible(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVisualComponent_IsVisible(LNHandle visualcomponent, LNBool* outReturn)
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


LN_FLAT_API LNResult LNVisualComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNVisualComponent_OnSerialize_SetOverrideCallback(LNVisualComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_VisualComponent::s_LNVisualComponent_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNVisualComponent_SetPrototype_OnSerialize(LNHandle visualcomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, visualcomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VisualComponentSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNVisualComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::VisualComponent>()->id();
}

LN_FLAT_API void LNVisualComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualComponent>(), id);
}

void LNVisualComponent_RegisterSubclassTypeInfo(const LNVisualComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualComponent>(), info->subclassId);
        *LNWS_ln_VisualComponent::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVisualComponent_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNSpriteComponent_SetTexture(LNHandle spritecomponent, LNHandle texture)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, spritecomponent)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, texture)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpriteComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSpriteComponent_OnSerialize_SetOverrideCallback(LNSpriteComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_SpriteComponent::s_LNSpriteComponent_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSpriteComponent_SetPrototype_OnSerialize(LNHandle spritecomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, spritecomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpriteComponentSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNSpriteComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::SpriteComponent>()->id();
}

LN_FLAT_API void LNSpriteComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpriteComponent>(), id);
}

void LNSpriteComponent_RegisterSubclassTypeInfo(const LNSpriteComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpriteComponent>(), info->subclassId);
        *LNWS_ln_SpriteComponent::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpriteComponent_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCollisionEventHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CollisionEventHandler>(), id);
}

void LNCollisionEventHandler_RegisterSubclassTypeInfo(const LNCollisionEventHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CollisionEventHandler>(), info->subclassId);
        *LNWS_ln_CollisionEventHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCollisionEventHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_CollisionEventHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNCharacterController_Create(LNHandle* outCharacterController)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outCharacterController, LNWS_ln_CharacterController, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetWalkVelocity(LNHandle charactercontroller, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setWalkVelocity(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_GetWalkVelocity(LNHandle charactercontroller, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->walkVelocity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->walkVelocity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetVelocity(LNHandle charactercontroller, const LNVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setVelocity(*reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_GetVelocity(LNHandle charactercontroller, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->velocity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->velocity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetInputControlEnabled(LNHandle charactercontroller, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setInputControlEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetCameraControlEnabled(LNHandle charactercontroller, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setCameraControlEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetHeight(LNHandle charactercontroller, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setHeight(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_GetHeight(LNHandle charactercontroller, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->height());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->height());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetCameraRadius(LNHandle charactercontroller, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setCameraRadius(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_GetCameraRadius(LNHandle charactercontroller, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->cameraRadius());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->cameraRadius());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetCollisionEnter(LNHandle charactercontroller, LNHandle handler)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setCollisionEnter(LNI_HANDLE_TO_OBJECT(ln::CollisionEventHandler, handler)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetCollisionLeave(LNHandle charactercontroller, LNHandle handler)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setCollisionLeave(LNI_HANDLE_TO_OBJECT(ln::CollisionEventHandler, handler)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_SetCollisionStay(LNHandle charactercontroller, LNHandle handler)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->setCollisionStay(LNI_HANDLE_TO_OBJECT(ln::CollisionEventHandler, handler)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCharacterController_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNCharacterController_OnSerialize_SetOverrideCallback(LNCharacterController_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_CharacterController::s_LNCharacterController_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNCharacterController_SetPrototype_OnSerialize(LNHandle charactercontroller, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, charactercontroller)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CharacterControllerSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNCharacterController_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::CharacterController>()->id();
}

LN_FLAT_API void LNCharacterController_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CharacterController>(), id);
}

void LNCharacterController_RegisterSubclassTypeInfo(const LNCharacterController_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CharacterController>(), info->subclassId);
        *LNWS_ln_CharacterController::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCharacterController_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_CharacterController, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNWorld_Add(LNHandle world, LNHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_World, world)->add(LNI_HANDLE_TO_OBJECT(ln::WorldObject, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorld_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_World, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNWorld_OnSerialize_SetOverrideCallback(LNWorld_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_World::s_LNWorld_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNWorld_SetPrototype_OnSerialize(LNHandle world, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_World, world)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNWorld_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::World>()->id();
}

LN_FLAT_API void LNWorld_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::World>(), id);
}

void LNWorld_RegisterSubclassTypeInfo(const LNWorld_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::World>(), info->subclassId);
        *LNWS_ln_World::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorld_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_World, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNComponentList_GetLength(LNHandle componentlist, int* outReturn)
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


LN_FLAT_API LNResult LNComponentList_GetItem(LNHandle componentlist, int index, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNComponentList_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNComponentList_OnSerialize_SetOverrideCallback(LNComponentList_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ComponentList::s_LNComponentList_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNComponentList_SetPrototype_OnSerialize(LNHandle componentlist, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ComponentListSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNComponentList_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ComponentList>()->id();
}

LN_FLAT_API void LNComponentList_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ComponentList>(), id);
}

void LNComponentList_RegisterSubclassTypeInfo(const LNComponentList_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ComponentList>(), info->subclassId);
        *LNWS_ln_ComponentList::subclassInfo() = *info;
    }
}

LNSubinstanceId LNComponentList_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNWorldObject_Create(LNHandle* outWorldObject)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outWorldObject, LNWS_ln_WorldObject, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetPosition(LNHandle worldobject, const LNVector3* pos)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setPosition(*reinterpret_cast<const ln::Vector3*>(pos)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetPositionXYZ(LNHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setPosition(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_GetPosition(LNHandle worldobject, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->position());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->position());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetRotation(LNHandle worldobject, const LNQuaternion* rot)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setRotation(*reinterpret_cast<const ln::Quaternion*>(rot)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetRotationXYZ(LNHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setRotation(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_GetRotation(LNHandle worldobject, LNQuaternion* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNQuaternion>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->rotation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->rotation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetScale(LNHandle worldobject, const LNVector3* scale)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setScale(*reinterpret_cast<const ln::Vector3*>(scale)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetScaleS(LNHandle worldobject, float xyz)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setScale(xyz));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetScaleXYZ(LNHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setScale(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_GetScale(LNHandle worldobject, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->scale());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->scale());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetCenterPoint(LNHandle worldobject, const LNVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setCenterPoint(*reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetCenterPointXYZ(LNHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setCenterPoint(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_GetCenterPoint(LNHandle worldobject, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->centerPoint());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->centerPoint());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_LookAt(LNHandle worldobject, const LNVector3* target)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->lookAt(*reinterpret_cast<const ln::Vector3*>(target)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_LookAtXYZ(LNHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->lookAt(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_AddComponent(LNHandle worldobject, LNHandle component)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->addComponent(LNI_HANDLE_TO_OBJECT(ln::Component, component)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_RemoveComponent(LNHandle worldobject, LNHandle component)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->removeComponent(LNI_HANDLE_TO_OBJECT(ln::Component, component)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_AddTag(LNHandle worldobject, const LNChar* tag)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->addTag(tag));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_AddTagA(LNHandle worldobject, const char* tag)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->addTag(LNI_ASTRPTR_TO_STRING(tag)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_RemoveTag(LNHandle worldobject, const LNChar* tag)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->removeTag(tag));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_RemoveTagA(LNHandle worldobject, const char* tag)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->removeTag(LNI_ASTRPTR_TO_STRING(tag)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_HasTag(LNHandle worldobject, const LNChar* tag, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->hasTag(tag));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->hasTag(tag));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_HasTagA(LNHandle worldobject, const char* tag, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->hasTag(LNI_ASTRPTR_TO_STRING(tag)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->hasTag(LNI_ASTRPTR_TO_STRING(tag)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_Destroy(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->destroy());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_GetComponents(LNHandle worldobject, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNWorldObject_AddInto(LNHandle worldobject, LNHandle world)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->addInto(LNI_HANDLE_TO_OBJECT(ln::World, world)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNWorldObject_OnSerialize_SetOverrideCallback(LNWorldObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_WorldObject::s_LNWorldObject_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNWorldObject_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNWorldObject_OnPreUpdate_SetOverrideCallback(LNWorldObject_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_WorldObject::s_LNWorldObject_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNWorldObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNWorldObject_OnUpdate_SetOverrideCallback(LNWorldObject_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_WorldObject::s_LNWorldObject_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnSerialize(LNHandle worldobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnPreUpdate(LNHandle worldobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnUpdate(LNHandle worldobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNWorldObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::WorldObject>()->id();
}

LN_FLAT_API void LNWorldObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldObject>(), id);
}

void LNWorldObject_RegisterSubclassTypeInfo(const LNWorldObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldObject>(), info->subclassId);
        *LNWS_ln_WorldObject::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorldObject_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNVisualObject_SetVisible(LNHandle visualobject, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->setVisible(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVisualObject_IsVisible(LNHandle visualobject, LNBool* outReturn)
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


LN_FLAT_API LNResult LNVisualObject_SetBlendMode2(LNHandle visualobject, LNBlendMode value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->setBlendMode2(static_cast<ln::BlendMode>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVisualObject_SetOpacity(LNHandle visualobject, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->setOpacity(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVisualObject_GetOpacity(LNHandle visualobject, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->opacity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->opacity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNVisualObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNVisualObject_OnSerialize_SetOverrideCallback(LNVisualObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_VisualObject::s_LNVisualObject_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNVisualObject_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNVisualObject_OnPreUpdate_SetOverrideCallback(LNVisualObject_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_VisualObject::s_LNVisualObject_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNVisualObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNVisualObject_OnUpdate_SetOverrideCallback(LNVisualObject_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_VisualObject::s_LNVisualObject_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnSerialize(LNHandle visualobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnPreUpdate(LNHandle visualobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnUpdate(LNHandle visualobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNVisualObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::VisualObject>()->id();
}

LN_FLAT_API void LNVisualObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualObject>(), id);
}

void LNVisualObject_RegisterSubclassTypeInfo(const LNVisualObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualObject>(), info->subclassId);
        *LNWS_ln_VisualObject::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVisualObject_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNCamera_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNCamera_OnSerialize_SetOverrideCallback(LNCamera_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Camera::s_LNCamera_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNCamera_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNCamera_OnPreUpdate_SetOverrideCallback(LNCamera_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_Camera::s_LNCamera_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNCamera_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNCamera_OnUpdate_SetOverrideCallback(LNCamera_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_Camera::s_LNCamera_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNCamera_SetPrototype_OnSerialize(LNHandle camera, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, camera)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CameraSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNCamera_SetPrototype_OnPreUpdate(LNHandle camera, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, camera)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CameraPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNCamera_SetPrototype_OnUpdate(LNHandle camera, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, camera)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CameraUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNCamera_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Camera>()->id();
}

LN_FLAT_API void LNCamera_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Camera>(), id);
}

void LNCamera_RegisterSubclassTypeInfo(const LNCamera_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Camera>(), info->subclassId);
        *LNWS_ln_Camera::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCamera_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNEnvironmentLight_SetEnabled(LNHandle environmentlight, LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_IsEnabled(LNHandle environmentlight, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->isEnabled());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->isEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_SetColor(LNHandle environmentlight, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_GetColor(LNHandle environmentlight, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_SetAmbientColor(LNHandle environmentlight, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setAmbientColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_GetAmbientColor(LNHandle environmentlight, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getAmbientColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getAmbientColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_GetSkyColor(LNHandle environmentlight, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getSkyColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getSkyColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_SetSkyColor(LNHandle environmentlight, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setSkyColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_GetGroundColor(LNHandle environmentlight, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getGroundColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getGroundColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_SetGroundColor(LNHandle environmentlight, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setGroundColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_SetIntensity(LNHandle environmentlight, float intensity)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setIntensity(intensity));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_GetIntensity(LNHandle environmentlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getIntensity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->getIntensity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_SetShadowEffectiveDistance(LNHandle environmentlight, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setShadowEffectiveDistance(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_GetShadowEffectiveDistance(LNHandle environmentlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->shadowEffectiveDistance());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->shadowEffectiveDistance());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_SetShadowEffectiveDepth(LNHandle environmentlight, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->setShadowEffectiveDepth(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_GetShadowEffectiveDepth(LNHandle environmentlight, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->shadowEffectiveDepth());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->shadowEffectiveDepth());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEnvironmentLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNEnvironmentLight_OnSerialize_SetOverrideCallback(LNEnvironmentLight_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_EnvironmentLight::s_LNEnvironmentLight_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNEnvironmentLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNEnvironmentLight_OnPreUpdate_SetOverrideCallback(LNEnvironmentLight_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_EnvironmentLight::s_LNEnvironmentLight_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNEnvironmentLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNEnvironmentLight_OnUpdate_SetOverrideCallback(LNEnvironmentLight_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_EnvironmentLight::s_LNEnvironmentLight_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnSerialize(LNHandle environmentlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnPreUpdate(LNHandle environmentlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnUpdate(LNHandle environmentlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNEnvironmentLight_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::EnvironmentLight>()->id();
}

LN_FLAT_API void LNEnvironmentLight_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EnvironmentLight>(), id);
}

void LNEnvironmentLight_RegisterSubclassTypeInfo(const LNEnvironmentLight_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EnvironmentLight>(), info->subclassId);
        *LNWS_ln_EnvironmentLight::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEnvironmentLight_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNDirectionalLight_Create(LNHandle* outDirectionalLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDirectionalLight, LNWS_ln_DirectionalLight, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_CreateWithColor(const LNColor* color, LNHandle* outDirectionalLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDirectionalLight, LNWS_ln_DirectionalLight, init, *reinterpret_cast<const ln::Color*>(color));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_SetEnabled(LNHandle directionallight, LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_IsEnabled(LNHandle directionallight, LNBool* outReturn)
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


LN_FLAT_API LNResult LNDirectionalLight_SetColor(LNHandle directionallight, const LNColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setColor(*reinterpret_cast<const ln::Color*>(color)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_GetColor(LNHandle directionallight, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->getColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->getColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_SetIntensity(LNHandle directionallight, float intensity)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setIntensity(intensity));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_GetIntensity(LNHandle directionallight, float* outReturn)
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


LN_FLAT_API LNResult LNDirectionalLight_SetShadowEffectiveDistance(LNHandle directionallight, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setShadowEffectiveDistance(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_GetShadowEffectiveDistance(LNHandle directionallight, float* outReturn)
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


LN_FLAT_API LNResult LNDirectionalLight_SetShadowEffectiveDepth(LNHandle directionallight, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->setShadowEffectiveDepth(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDirectionalLight_GetShadowEffectiveDepth(LNHandle directionallight, float* outReturn)
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


LN_FLAT_API LNResult LNDirectionalLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNDirectionalLight_OnSerialize_SetOverrideCallback(LNDirectionalLight_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_DirectionalLight::s_LNDirectionalLight_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNDirectionalLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNDirectionalLight_OnPreUpdate_SetOverrideCallback(LNDirectionalLight_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_DirectionalLight::s_LNDirectionalLight_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNDirectionalLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNDirectionalLight_OnUpdate_SetOverrideCallback(LNDirectionalLight_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_DirectionalLight::s_LNDirectionalLight_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnSerialize(LNHandle directionallight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnPreUpdate(LNHandle directionallight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnUpdate(LNHandle directionallight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNDirectionalLight_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::DirectionalLight>()->id();
}

LN_FLAT_API void LNDirectionalLight_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::DirectionalLight>(), id);
}

void LNDirectionalLight_RegisterSubclassTypeInfo(const LNDirectionalLight_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::DirectionalLight>(), info->subclassId);
        *LNWS_ln_DirectionalLight::subclassInfo() = *info;
    }
}

LNSubinstanceId LNDirectionalLight_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNPointLight_Create(LNHandle* outPointLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outPointLight, LNWS_ln_PointLight, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_CreateWithColorAndRange(const LNColor* color, float range, LNHandle* outPointLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outPointLight, LNWS_ln_PointLight, init, *reinterpret_cast<const ln::Color*>(color), range);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_SetEnabled(LNHandle pointlight, LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_IsEnabled(LNHandle pointlight, LNBool* outReturn)
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


LN_FLAT_API LNResult LNPointLight_SetColor(LNHandle pointlight, const LNColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setColor(*reinterpret_cast<const ln::Color*>(color)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_GetColor(LNHandle pointlight, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->getColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_SetIntensity(LNHandle pointlight, float intensity)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setIntensity(intensity));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_GetIntensity(LNHandle pointlight, float* outReturn)
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


LN_FLAT_API LNResult LNPointLight_SetRange(LNHandle pointlight, float range)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setRange(range));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_GetRange(LNHandle pointlight, float* outReturn)
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


LN_FLAT_API LNResult LNPointLight_SetAttenuation(LNHandle pointlight, float attenuation)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->setAttenuation(attenuation));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPointLight_GetAttenuation(LNHandle pointlight, float* outReturn)
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


LN_FLAT_API LNResult LNPointLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNPointLight_OnSerialize_SetOverrideCallback(LNPointLight_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_PointLight::s_LNPointLight_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNPointLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNPointLight_OnPreUpdate_SetOverrideCallback(LNPointLight_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_PointLight::s_LNPointLight_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNPointLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNPointLight_OnUpdate_SetOverrideCallback(LNPointLight_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_PointLight::s_LNPointLight_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNPointLight_SetPrototype_OnSerialize(LNHandle pointlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PointLightSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnPreUpdate(LNHandle pointlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PointLightPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnUpdate(LNHandle pointlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PointLightUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNPointLight_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::PointLight>()->id();
}

LN_FLAT_API void LNPointLight_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PointLight>(), id);
}

void LNPointLight_RegisterSubclassTypeInfo(const LNPointLight_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PointLight>(), info->subclassId);
        *LNWS_ln_PointLight::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPointLight_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNSpotLight_Create(LNHandle* outSpotLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSpotLight, LNWS_ln_SpotLight, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_CreateWithColorAndRange(const LNColor* color, float range, float angle, LNHandle* outSpotLight)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSpotLight, LNWS_ln_SpotLight, init, *reinterpret_cast<const ln::Color*>(color), range, angle);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_SetEnabled(LNHandle spotlight, LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_IsEnabled(LNHandle spotlight, LNBool* outReturn)
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


LN_FLAT_API LNResult LNSpotLight_SetColor(LNHandle spotlight, const LNColor* color)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setColor(*reinterpret_cast<const ln::Color*>(color)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_GetColor(LNHandle spotlight, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->getColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_SetIntensity(LNHandle spotlight, float intensity)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setIntensity(intensity));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_GetIntensity(LNHandle spotlight, float* outReturn)
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


LN_FLAT_API LNResult LNSpotLight_SetRange(LNHandle spotlight, float range)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setRange(range));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_GetRange(LNHandle spotlight, float* outReturn)
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


LN_FLAT_API LNResult LNSpotLight_SetAttenuation(LNHandle spotlight, float attenuation)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setAttenuation(attenuation));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_GetAttenuation(LNHandle spotlight, float* outReturn)
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


LN_FLAT_API LNResult LNSpotLight_SetAngle(LNHandle spotlight, float angle)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setAngle(angle));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_GetAngle(LNHandle spotlight, float* outReturn)
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


LN_FLAT_API LNResult LNSpotLight_SetPenumbra(LNHandle spotlight, float penumbra)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->setPenumbra(penumbra));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSpotLight_GetPenumbra(LNHandle spotlight, float* outReturn)
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


LN_FLAT_API LNResult LNSpotLight_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSpotLight_OnSerialize_SetOverrideCallback(LNSpotLight_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_SpotLight::s_LNSpotLight_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSpotLight_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSpotLight_OnPreUpdate_SetOverrideCallback(LNSpotLight_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_SpotLight::s_LNSpotLight_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSpotLight_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSpotLight_OnUpdate_SetOverrideCallback(LNSpotLight_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_SpotLight::s_LNSpotLight_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnSerialize(LNHandle spotlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpotLightSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnPreUpdate(LNHandle spotlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpotLightPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnUpdate(LNHandle spotlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpotLightUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNSpotLight_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::SpotLight>()->id();
}

LN_FLAT_API void LNSpotLight_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpotLight>(), id);
}

void LNSpotLight_RegisterSubclassTypeInfo(const LNSpotLight_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpotLight>(), info->subclassId);
        *LNWS_ln_SpotLight::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpotLight_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNTestDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::TestDelegate>(), id);
}

void LNTestDelegate_RegisterSubclassTypeInfo(const LNTestDelegate_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::TestDelegate>(), info->subclassId);
        *LNWS_ln_TestDelegate::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTestDelegate_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_TestDelegate, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNSprite_Create(LNHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_CreateWithTexture(LNHandle texture, LNHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_CreateWithTextureAndSize(LNHandle texture, float width, float height, LNHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture), width, height);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_SetTexture(LNHandle sprite, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_SetSize(LNHandle sprite, const LNSize* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setSize(*reinterpret_cast<const ln::Size*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_SetSizeWH(LNHandle sprite, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setSize(width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_SetSourceRectXYWH(LNHandle sprite, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setSourceRect(x, y, width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_SetCallerTest(LNHandle sprite, LNHandle callback)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setCallerTest(LNI_HANDLE_TO_OBJECT(ln::TestDelegate, callback)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNSprite_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSprite_OnSerialize_SetOverrideCallback(LNSprite_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Sprite::s_LNSprite_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSprite_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSprite_OnPreUpdate_SetOverrideCallback(LNSprite_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_Sprite::s_LNSprite_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSprite_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSprite_OnUpdate_SetOverrideCallback(LNSprite_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_Sprite::s_LNSprite_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNSprite_SetPrototype_OnSerialize(LNHandle sprite, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpriteSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNSprite_SetPrototype_OnPreUpdate(LNHandle sprite, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpritePreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNSprite_SetPrototype_OnUpdate(LNHandle sprite, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpriteUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNSprite_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Sprite>()->id();
}

LN_FLAT_API void LNSprite_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sprite>(), id);
}

void LNSprite_RegisterSubclassTypeInfo(const LNSprite_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sprite>(), info->subclassId);
        *LNWS_ln_Sprite::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSprite_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNCameraOrbitControlComponent_Create(LNHandle* outCameraOrbitControlComponent)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outCameraOrbitControlComponent, LNWS_ln_CameraOrbitControlComponent, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCameraOrbitControlComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNCameraOrbitControlComponent_OnSerialize_SetOverrideCallback(LNCameraOrbitControlComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_CameraOrbitControlComponent::s_LNCameraOrbitControlComponent_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNCameraOrbitControlComponent_SetPrototype_OnSerialize(LNHandle cameraorbitcontrolcomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, cameraorbitcontrolcomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CameraOrbitControlComponentSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNCameraOrbitControlComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::CameraOrbitControlComponent>()->id();
}

LN_FLAT_API void LNCameraOrbitControlComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CameraOrbitControlComponent>(), id);
}

void LNCameraOrbitControlComponent_RegisterSubclassTypeInfo(const LNCameraOrbitControlComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::CameraOrbitControlComponent>(), info->subclassId);
        *LNWS_ln_CameraOrbitControlComponent::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCameraOrbitControlComponent_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNRaycaster_FromScreen(const LNPoint* point, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNRaycaster_IntersectPlane(LNHandle raycaster, float normalX, float normalY, float normalZ, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNRaycaster_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNRaycaster_OnSerialize_SetOverrideCallback(LNRaycaster_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Raycaster::s_LNRaycaster_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNRaycaster_SetPrototype_OnSerialize(LNHandle raycaster, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, raycaster)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_RaycasterSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNRaycaster_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Raycaster>()->id();
}

LN_FLAT_API void LNRaycaster_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Raycaster>(), id);
}

void LNRaycaster_RegisterSubclassTypeInfo(const LNRaycaster_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Raycaster>(), info->subclassId);
        *LNWS_ln_Raycaster::subclassInfo() = *info;
    }
}

LNSubinstanceId LNRaycaster_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNRaycastResult_GetPoint(LNHandle raycastresult, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, raycastresult)->point());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, raycastresult)->point());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNRaycastResult_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNRaycastResult_OnSerialize_SetOverrideCallback(LNRaycastResult_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_RaycastResult::s_LNRaycastResult_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNRaycastResult_SetPrototype_OnSerialize(LNHandle raycastresult, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, raycastresult)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_RaycastResultSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNRaycastResult_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::RaycastResult>()->id();
}

LN_FLAT_API void LNRaycastResult_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RaycastResult>(), id);
}

void LNRaycastResult_RegisterSubclassTypeInfo(const LNRaycastResult_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::RaycastResult>(), info->subclassId);
        *LNWS_ln_RaycastResult::subclassInfo() = *info;
    }
}

LNSubinstanceId LNRaycastResult_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNWorldRenderView_SetGuideGridEnabled(LNHandle worldrenderview, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, worldrenderview)->setGuideGridEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldRenderView_GetGuideGridEnabled(LNHandle worldrenderview, LNBool* outReturn)
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


LN_FLAT_API LNResult LNWorldRenderView_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNWorldRenderView_OnSerialize_SetOverrideCallback(LNWorldRenderView_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_WorldRenderView::s_LNWorldRenderView_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNWorldRenderView_SetPrototype_OnSerialize(LNHandle worldrenderview, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, worldrenderview)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldRenderViewSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNWorldRenderView_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::WorldRenderView>()->id();
}

LN_FLAT_API void LNWorldRenderView_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldRenderView>(), id);
}

void LNWorldRenderView_RegisterSubclassTypeInfo(const LNWorldRenderView_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldRenderView>(), info->subclassId);
        *LNWS_ln_WorldRenderView::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorldRenderView_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNShapeObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ShapeObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNShapeObject_OnSerialize_SetOverrideCallback(LNShapeObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ShapeObject::s_LNShapeObject_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNShapeObject_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ShapeObject, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNShapeObject_OnPreUpdate_SetOverrideCallback(LNShapeObject_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_ShapeObject::s_LNShapeObject_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNShapeObject_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ShapeObject, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNShapeObject_OnUpdate_SetOverrideCallback(LNShapeObject_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_ShapeObject::s_LNShapeObject_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNShapeObject_SetPrototype_OnSerialize(LNHandle shapeobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ShapeObject, shapeobject)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ShapeObjectSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNShapeObject_SetPrototype_OnPreUpdate(LNHandle shapeobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ShapeObject, shapeobject)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ShapeObjectPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNShapeObject_SetPrototype_OnUpdate(LNHandle shapeobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ShapeObject, shapeobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ShapeObjectUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNShapeObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ShapeObject>()->id();
}

LN_FLAT_API void LNShapeObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ShapeObject>(), id);
}

void LNShapeObject_RegisterSubclassTypeInfo(const LNShapeObject_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ShapeObject>(), info->subclassId);
        *LNWS_ln_ShapeObject::subclassInfo() = *info;
    }
}

LNSubinstanceId LNShapeObject_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ShapeObject, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNPlaneMesh_Create(LNHandle* outPlaneMesh)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outPlaneMesh, LNWS_ln_PlaneMesh, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNPlaneMesh_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNPlaneMesh_OnSerialize_SetOverrideCallback(LNPlaneMesh_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNPlaneMesh_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNPlaneMesh_OnPreUpdate_SetOverrideCallback(LNPlaneMesh_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNPlaneMesh_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNPlaneMesh_OnUpdate_SetOverrideCallback(LNPlaneMesh_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnSerialize(LNHandle planemesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, planemesh)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnPreUpdate(LNHandle planemesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, planemesh)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnUpdate(LNHandle planemesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, planemesh)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNPlaneMesh_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::PlaneMesh>()->id();
}

LN_FLAT_API void LNPlaneMesh_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PlaneMesh>(), id);
}

void LNPlaneMesh_RegisterSubclassTypeInfo(const LNPlaneMesh_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PlaneMesh>(), info->subclassId);
        *LNWS_ln_PlaneMesh::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPlaneMesh_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNBoxMesh_Create(LNHandle* outBoxMesh)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outBoxMesh, LNWS_ln_BoxMesh, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNBoxMesh_CreateWithSize(float width, float height, float depth, LNHandle* outBoxMesh)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outBoxMesh, LNWS_ln_BoxMesh, init, width, height, depth);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNBoxMesh_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNBoxMesh_OnSerialize_SetOverrideCallback(LNBoxMesh_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_BoxMesh::s_LNBoxMesh_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNBoxMesh_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNBoxMesh_OnPreUpdate_SetOverrideCallback(LNBoxMesh_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_BoxMesh::s_LNBoxMesh_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNBoxMesh_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNBoxMesh_OnUpdate_SetOverrideCallback(LNBoxMesh_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_BoxMesh::s_LNBoxMesh_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnSerialize(LNHandle boxmesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, boxmesh)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnPreUpdate(LNHandle boxmesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, boxmesh)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnUpdate(LNHandle boxmesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, boxmesh)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNBoxMesh_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::BoxMesh>()->id();
}

LN_FLAT_API void LNBoxMesh_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::BoxMesh>(), id);
}

void LNBoxMesh_RegisterSubclassTypeInfo(const LNBoxMesh_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::BoxMesh>(), info->subclassId);
        *LNWS_ln_BoxMesh::subclassInfo() = *info;
    }
}

LNSubinstanceId LNBoxMesh_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNMeshComponent_Create(LNHandle* outMeshComponent)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outMeshComponent, LNWS_ln_MeshComponent, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshComponent_SetModel(LNHandle meshcomponent, LNHandle model)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshComponent, meshcomponent)->setModel(LNI_HANDLE_TO_OBJECT(ln::MeshModel, model)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshComponent_MakeCollisionBody(LNHandle meshcomponent, const LNChar* meshContainerName)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshComponent, meshcomponent)->makeCollisionBody(meshContainerName));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshComponent_MakeCollisionBodyA(LNHandle meshcomponent, const char* meshContainerName)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshComponent, meshcomponent)->makeCollisionBody(LNI_ASTRPTR_TO_STRING(meshContainerName)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMeshComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNMeshComponent_OnSerialize_SetOverrideCallback(LNMeshComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_MeshComponent::s_LNMeshComponent_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNMeshComponent_SetPrototype_OnSerialize(LNHandle meshcomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshComponent, meshcomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_MeshComponentSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNMeshComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::MeshComponent>()->id();
}

LN_FLAT_API void LNMeshComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshComponent>(), id);
}

void LNMeshComponent_RegisterSubclassTypeInfo(const LNMeshComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::MeshComponent>(), info->subclassId);
        *LNWS_ln_MeshComponent::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshComponent_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_MeshComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNCollision_GetWorldObject(LNHandle collision, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_Collision, collision)->worldObject());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Collision, collision)->worldObject());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNCollision_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Collision, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNCollision_OnSerialize_SetOverrideCallback(LNCollision_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Collision::s_LNCollision_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNCollision_SetPrototype_OnSerialize(LNHandle collision, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Collision, collision)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CollisionSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNCollision_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Collision>()->id();
}

LN_FLAT_API void LNCollision_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Collision>(), id);
}

void LNCollision_RegisterSubclassTypeInfo(const LNCollision_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Collision>(), info->subclassId);
        *LNWS_ln_Collision::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCollision_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Collision, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNTriggerBodyComponent_Create(LNHandle* outTriggerBodyComponent)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTriggerBodyComponent, LNWS_ln_TriggerBodyComponent, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTriggerBodyComponent_AddCollisionShape(LNHandle triggerbodycomponent, LNHandle shape)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_TriggerBodyComponent, triggerbodycomponent)->addCollisionShape(LNI_HANDLE_TO_OBJECT(ln::CollisionShape, shape)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNTriggerBodyComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_TriggerBodyComponent, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNTriggerBodyComponent_OnSerialize_SetOverrideCallback(LNTriggerBodyComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_TriggerBodyComponent::s_LNTriggerBodyComponent_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNTriggerBodyComponent_SetPrototype_OnSerialize(LNHandle triggerbodycomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_TriggerBodyComponent, triggerbodycomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_TriggerBodyComponentSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNTriggerBodyComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::TriggerBodyComponent>()->id();
}

LN_FLAT_API void LNTriggerBodyComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::TriggerBodyComponent>(), id);
}

void LNTriggerBodyComponent_RegisterSubclassTypeInfo(const LNTriggerBodyComponent_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::TriggerBodyComponent>(), info->subclassId);
        *LNWS_ln_TriggerBodyComponent::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTriggerBodyComponent_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_TriggerBodyComponent, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNParticleEmitter_Create(LNHandle model, LNHandle* outParticleEmitter)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outParticleEmitter, LNWS_ln_ParticleEmitter, init, LNI_HANDLE_TO_OBJECT(ln::ParticleModel, model));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNParticleEmitter_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitter, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNParticleEmitter_OnSerialize_SetOverrideCallback(LNParticleEmitter_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_ParticleEmitter::s_LNParticleEmitter_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNParticleEmitter_OnPreUpdate_CallOverrideBase(LNHandle worldobject)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitter, worldobject)->onPreUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNParticleEmitter_OnPreUpdate_SetOverrideCallback(LNParticleEmitter_OnPreUpdate_OverrideCallback callback)
{
    LNWS_ln_ParticleEmitter::s_LNParticleEmitter_OnPreUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNParticleEmitter_OnUpdate_CallOverrideBase(LNHandle worldobject, float elapsedSeconds)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitter, worldobject)->onUpdate_CallBase(elapsedSeconds));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNParticleEmitter_OnUpdate_SetOverrideCallback(LNParticleEmitter_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_ParticleEmitter::s_LNParticleEmitter_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNParticleEmitter_SetPrototype_OnSerialize(LNHandle particleemitter, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitter, particleemitter)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNParticleEmitter_SetPrototype_OnPreUpdate(LNHandle particleemitter, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitter, particleemitter)->acquireOverridePrototypes()->OnPreUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterPreUpdateHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNParticleEmitter_SetPrototype_OnUpdate(LNHandle particleemitter, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitter, particleemitter)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNParticleEmitter_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ParticleEmitter>()->id();
}

LN_FLAT_API void LNParticleEmitter_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ParticleEmitter>(), id);
}

void LNParticleEmitter_RegisterSubclassTypeInfo(const LNParticleEmitter_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ParticleEmitter>(), info->subclassId);
        *LNWS_ln_ParticleEmitter::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleEmitter_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_ParticleEmitter, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNScene_SetClearMode(LNSceneClearMode value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setClearMode(static_cast<ln::SceneClearMode>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetSkyColor(const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setSkyColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetSkyHorizonColor(const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setSkyHorizonColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetSkyCloudColor(const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setSkyCloudColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetSkyOverlayColor(const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setSkyOverlayColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_GotoLevel(LNHandle level, LNBool withEffect)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::gotoLevel(LNI_HANDLE_TO_OBJECT(ln::Level, level), LNI_LNBOOL_TO_BOOL(withEffect)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_CallLevel(LNHandle level, LNBool withEffect)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::callLevel(LNI_HANDLE_TO_OBJECT(ln::Level, level), LNI_LNBOOL_TO_BOOL(withEffect)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_ReturnLevel(LNBool withEffect)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::returnLevel(LNI_LNBOOL_TO_BOOL(withEffect)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_ActiveLevel(LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Scene::activeLevel());
    }
    else {
        (ln::Scene::activeLevel());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsTransitionEffectRunning(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isTransitionEffectRunning());
    }
    else {
        (ln::Scene::isTransitionEffectRunning());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTransitionEffectMode(LNLevelTransitionEffectMode value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTransitionEffectMode(static_cast<ln::LevelTransitionEffectMode>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_TransitionEffectMode(LNLevelTransitionEffectMode* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNLevelTransitionEffectMode>(ln::Scene::transitionEffectMode());
    }
    else {
        (ln::Scene::transitionEffectMode());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTransitionDuration(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTransitionDuration(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_TransitionDuration(float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (ln::Scene::transitionDuration());
    }
    else {
        (ln::Scene::transitionDuration());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTransitionEffectColor(const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTransitionEffectColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_TransitionEffectColor(LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::Scene::transitionEffectColor());
    }
    else {
        (ln::Scene::transitionEffectColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTransitionEffectMaskTexture(LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTransitionEffectMaskTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_TransitionEffectMaskTexture(LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Scene::transitionEffectMaskTexture());
    }
    else {
        (ln::Scene::transitionEffectMaskTexture());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTransitionEffectVague(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTransitionEffectVague(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_TransitionEffectVague(float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (ln::Scene::transitionEffectVague());
    }
    else {
        (ln::Scene::transitionEffectVague());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_StartFadeOut()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::startFadeOut());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_StartFadeIn()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::startFadeIn());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetFogStartDistance(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setFogStartDistance(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetFogColor(const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setFogColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetFogDensity(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setFogDensity(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetFogHeightDensity(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setFogHeightDensity(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetFogLowerHeight(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setFogLowerHeight(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetFogUpperHeight(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setFogUpperHeight(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetHDREnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setHDREnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsHDREnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isHDREnabled());
    }
    else {
        (ln::Scene::isHDREnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetScreenBlendColor(const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setScreenBlendColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_ScreenBlendColor(LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::Scene::screenBlendColor());
    }
    else {
        (ln::Scene::screenBlendColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetColorTone(const LNColorTone* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setColorTone(*reinterpret_cast<const ln::ColorTone*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_ColorTone(LNColorTone* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColorTone>(ln::Scene::colorTone());
    }
    else {
        (ln::Scene::colorTone());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetAntialiasEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setAntialiasEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsAntialiasEnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isAntialiasEnabled());
    }
    else {
        (ln::Scene::isAntialiasEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetSSREnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setSSREnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsSSREnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isSSREnabled());
    }
    else {
        (ln::Scene::isSSREnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetSSAOEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setSSAOEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsSSAOEnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isSSAOEnabled());
    }
    else {
        (ln::Scene::isSSAOEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetBloomEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setBloomEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsBloomEnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isBloomEnabled());
    }
    else {
        (ln::Scene::isBloomEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetDOFEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setDOFEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsDOFEnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isDOFEnabled());
    }
    else {
        (ln::Scene::isDOFEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsTonemapEnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isTonemapEnabled());
    }
    else {
        (ln::Scene::isTonemapEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetVignetteEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setVignetteEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsVignetteEnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isVignetteEnabled());
    }
    else {
        (ln::Scene::isVignetteEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetGammaEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setGammaEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_IsGammaEnabled(LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Scene::isGammaEnabled());
    }
    else {
        (ln::Scene::isGammaEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapExposure(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapExposure(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapLinearWhite(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapLinearWhite(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapShoulderStrength(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapShoulderStrength(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapLinearStrength(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapLinearStrength(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapLinearAngle(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapLinearAngle(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapToeStrength(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapToeStrength(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapToeNumerator(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapToeNumerator(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNScene_SetTonemapToeDenominator(float value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Scene::setTonemapToeDenominator(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLevel_Create(LNHandle* outLevel)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outLevel, LNWS_ln_Level, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLevel_AddObject(LNHandle level, LNHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->addObject(LNI_HANDLE_TO_OBJECT(ln::WorldObject, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLevel_RemoveObject(LNHandle level, LNHandle obj)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->removeObject(LNI_HANDLE_TO_OBJECT(ln::WorldObject, obj)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLevel_AddSubLevel(LNHandle level, LNHandle sublevel)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->addSubLevel(LNI_HANDLE_TO_OBJECT(ln::Level, sublevel)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLevel_RemoveSubLevel(LNHandle level, LNHandle sublevel)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->removeSubLevel(LNI_HANDLE_TO_OBJECT(ln::Level, sublevel)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLevel_RemoveAllSubLevels(LNHandle level)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->removeAllSubLevels());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNLevel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNLevel_OnSerialize_SetOverrideCallback(LNLevel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Level::s_LNLevel_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNLevel_OnStart_CallOverrideBase(LNHandle level)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->onStart_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNLevel_OnStart_SetOverrideCallback(LNLevel_OnStart_OverrideCallback callback)
{
    LNWS_ln_Level::s_LNLevel_OnStart_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNLevel_OnStop_CallOverrideBase(LNHandle level)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->onStop_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNLevel_OnStop_SetOverrideCallback(LNLevel_OnStop_OverrideCallback callback)
{
    LNWS_ln_Level::s_LNLevel_OnStop_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNLevel_OnPause_CallOverrideBase(LNHandle level)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->onPause_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNLevel_OnPause_SetOverrideCallback(LNLevel_OnPause_OverrideCallback callback)
{
    LNWS_ln_Level::s_LNLevel_OnPause_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNLevel_OnResume_CallOverrideBase(LNHandle level)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->onResume_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNLevel_OnResume_SetOverrideCallback(LNLevel_OnResume_OverrideCallback callback)
{
    LNWS_ln_Level::s_LNLevel_OnResume_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNLevel_OnUpdate_CallOverrideBase(LNHandle level)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->onUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNLevel_OnUpdate_SetOverrideCallback(LNLevel_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_Level::s_LNLevel_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNLevel_SetPrototype_OnSerialize(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStart(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnStart_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelStartHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStop(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnStop_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelStopHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnPause(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnPause_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelPauseHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnResume(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnResume_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelResumeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnUpdate(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNLevel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Level>()->id();
}

LN_FLAT_API void LNLevel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Level>(), id);
}

void LNLevel_RegisterSubclassTypeInfo(const LNLevel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Level>(), info->subclassId);
        *LNWS_ln_Level::subclassInfo() = *info;
    }
}

LNSubinstanceId LNLevel_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIColors_Red(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::red(shades));
    }
    else {
        (ln::UIColors::red(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Pink(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::pink(shades));
    }
    else {
        (ln::UIColors::pink(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Purple(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::purple(shades));
    }
    else {
        (ln::UIColors::purple(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_DeepPurple(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::deepPurple(shades));
    }
    else {
        (ln::UIColors::deepPurple(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Indigo(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::indigo(shades));
    }
    else {
        (ln::UIColors::indigo(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Blue(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::blue(shades));
    }
    else {
        (ln::UIColors::blue(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_LightBlue(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::lightBlue(shades));
    }
    else {
        (ln::UIColors::lightBlue(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Cyan(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::cyan(shades));
    }
    else {
        (ln::UIColors::cyan(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Teal(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::teal(shades));
    }
    else {
        (ln::UIColors::teal(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Green(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::green(shades));
    }
    else {
        (ln::UIColors::green(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_LightGreen(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::lightGreen(shades));
    }
    else {
        (ln::UIColors::lightGreen(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Lime(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::lime(shades));
    }
    else {
        (ln::UIColors::lime(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Yellow(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::yellow(shades));
    }
    else {
        (ln::UIColors::yellow(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Amber(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::amber(shades));
    }
    else {
        (ln::UIColors::amber(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Orange(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::orange(shades));
    }
    else {
        (ln::UIColors::orange(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_DeepOrange(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::deepOrange(shades));
    }
    else {
        (ln::UIColors::deepOrange(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Brown(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::brown(shades));
    }
    else {
        (ln::UIColors::brown(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Grey(int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::grey(shades));
    }
    else {
        (ln::UIColors::grey(shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIColors_Get(LNUIColorHues hue, int shades, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(ln::UIColors::get(static_cast<ln::UIColorHues>(hue), shades));
    }
    else {
        (ln::UIColors::get(static_cast<ln::UIColorHues>(hue), shades));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIEventArgs_Sender(LNHandle uieventargs, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNUIEventArgs_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIEventArgs_OnSerialize_SetOverrideCallback(LNUIEventArgs_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIEventArgs::s_LNUIEventArgs_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIEventArgs_SetPrototype_OnSerialize(LNHandle uieventargs, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, uieventargs)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIEventArgsSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIEventArgs_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIEventArgs>()->id();
}

LN_FLAT_API void LNUIEventArgs_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventArgs>(), id);
}

void LNUIEventArgs_RegisterSubclassTypeInfo(const LNUIEventArgs_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventArgs>(), info->subclassId);
        *LNWS_ln_UIEventArgs::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIEventArgs_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIGeneralEventHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIGeneralEventHandler>(), id);
}

void LNUIGeneralEventHandler_RegisterSubclassTypeInfo(const LNUIGeneralEventHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIGeneralEventHandler>(), info->subclassId);
        *LNWS_ln_UIGeneralEventHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIGeneralEventHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIGeneralEventHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIEventHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventHandler>(), id);
}

void LNUIEventHandler_RegisterSubclassTypeInfo(const LNUIEventHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventHandler>(), info->subclassId);
        *LNWS_ln_UIEventHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIEventHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUILayoutElement_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUILayoutElement_OnSerialize_SetOverrideCallback(LNUILayoutElement_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UILayoutElement::s_LNUILayoutElement_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUILayoutElement_SetPrototype_OnSerialize(LNHandle uilayoutelement, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, uilayoutelement)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UILayoutElementSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUILayoutElement_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UILayoutElement>()->id();
}

LN_FLAT_API void LNUILayoutElement_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UILayoutElement>(), id);
}

void LNUILayoutElement_RegisterSubclassTypeInfo(const LNUILayoutElement_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UILayoutElement>(), info->subclassId);
        *LNWS_ln_UILayoutElement::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUILayoutElement_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIElement_SetSize(LNHandle uielement, const LNSize* size)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setSize(*reinterpret_cast<const ln::Size*>(size)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetSizeWH(LNHandle uielement, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setSize(width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetWidth(LNHandle uielement, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setWidth(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetWidth(LNHandle uielement, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->width());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->width());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetHeight(LNHandle uielement, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setHeight(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetHeight(LNHandle uielement, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->height());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->height());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetMargin(LNHandle uielement, const LNThickness* margin)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setMargin(*reinterpret_cast<const ln::Thickness*>(margin)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetMargin(LNHandle uielement, LNThickness* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNThickness>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->margin());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->margin());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetPadding(LNHandle uielement, const LNThickness* padding)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setPadding(*reinterpret_cast<const ln::Thickness*>(padding)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetPadding(LNHandle uielement, LNThickness* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNThickness>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->padding());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->padding());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetHAlignment(LNHandle uielement, LNUIHAlignment value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setHAlignment(static_cast<ln::UIHAlignment>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetHAlignment(LNHandle uielement, LNUIHAlignment* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNUIHAlignment>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->hAlignment());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->hAlignment());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetVAlignment(LNHandle uielement, LNUIVAlignment value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setVAlignment(static_cast<ln::UIVAlignment>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetVAlignment(LNHandle uielement, LNUIVAlignment* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNUIVAlignment>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->vAlignment());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->vAlignment());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetAlignments(LNHandle uielement, LNUIHAlignment halign, LNUIVAlignment valign)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setAlignments(static_cast<ln::UIHAlignment>(halign), static_cast<ln::UIVAlignment>(valign)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetPosition(LNHandle uielement, const LNVector3* pos)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setPosition(*reinterpret_cast<const ln::Vector3*>(pos)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetPositionXYZ(LNHandle uielement, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setPosition(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetPosition(LNHandle uielement, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->position());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->position());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetRotation(LNHandle uielement, const LNQuaternion* rot)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setRotation(*reinterpret_cast<const ln::Quaternion*>(rot)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetEulerAngles(LNHandle uielement, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setEulerAngles(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetRotation(LNHandle uielement, LNQuaternion* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNQuaternion>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->rotation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->rotation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetScale(LNHandle uielement, const LNVector3* scale)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setScale(*reinterpret_cast<const ln::Vector3*>(scale)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetScaleS(LNHandle uielement, float xyz)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setScale(xyz));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetScaleXY(LNHandle uielement, float x, float y)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setScale(x, y));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetScale(LNHandle uielement, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->scale());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->scale());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetCenterPoint(LNHandle uielement, const LNVector3* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setCenterPoint(*reinterpret_cast<const ln::Vector3*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetCenterPointXYZ(LNHandle uielement, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setCenterPoint(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetCenterPoint(LNHandle uielement, LNVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNVector3>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->centerPoint());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->centerPoint());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetEnabled(LNHandle uielement, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_IsEnabled(LNHandle uielement, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->isEnabled());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->isEnabled());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetData(LNHandle uielement, LNHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setData(LNI_HANDLE_TO_OBJECT(ln::Variant, value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetData(LNHandle uielement, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->data());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->data());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetBackgroundColor(LNHandle uielement, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setBackgroundColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetBackgroundColor(LNHandle uielement, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->backgroundColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->backgroundColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetBorderThickness(LNHandle uielement, const LNThickness* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setBorderThickness(*reinterpret_cast<const ln::Thickness*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetBorderThickness(LNHandle uielement, LNThickness* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNThickness>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->borderThickness());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->borderThickness());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetBorderColor(LNHandle uielement, const LNColor* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setBorderColor(*reinterpret_cast<const ln::Color*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetBorderColor(LNHandle uielement, LNColor* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNColor>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->borderColor());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->borderColor());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetCornerRadius(LNHandle uielement, const LNCornerRadius* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setCornerRadius(*reinterpret_cast<const ln::CornerRadius*>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetCornerRadius(LNHandle uielement, LNCornerRadius* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNCornerRadius>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->cornerRadius());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->cornerRadius());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetVisibility(LNHandle uielement, LNUIVisibility value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setVisibility(static_cast<ln::UIVisibility>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetVisibility(LNHandle uielement, LNUIVisibility* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNUIVisibility>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->visibility());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->visibility());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetOpacity(LNHandle uielement, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setOpacity(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetOpacity(LNHandle uielement, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->opacity());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->opacity());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_AddChild(LNHandle uielement, LNHandle child)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->addChild(LNI_HANDLE_TO_OBJECT(ln::UIElement, child)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetFocusable(LNHandle uielement, LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setFocusable(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetFocusable(LNHandle uielement, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->focusable());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->focusable());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_AddInto(LNHandle uielement, LNHandle parent)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->addInto(LNI_HANDLE_TO_OBJECT(ln::UIElement, parent)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIElement_OnSerialize_SetOverrideCallback(LNUIElement_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIElement::s_LNUIElement_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIElement_SetPrototype_OnSerialize(LNHandle uielement, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIElementSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIElement_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIElement>()->id();
}

LN_FLAT_API void LNUIElement_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIElement>(), id);
}

void LNUIElement_RegisterSubclassTypeInfo(const LNUIElement_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIElement>(), info->subclassId);
        *LNWS_ln_UIElement::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIElement_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIText_Create(LNHandle* outUIText)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIText, LNWS_ln_UIText, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIText_CreateWithText(const LNChar* text, LNHandle* outUIText)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIText, LNWS_ln_UIText, init, text);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIText_CreateWithTextA(const char* text, LNHandle* outUIText)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIText, LNWS_ln_UIText, init, LNI_ASTRPTR_TO_STRING(text));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIText_SetText(LNHandle uitext, const LNChar* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, uitext)->setText(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIText_SetTextA(LNHandle uitext, const char* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, uitext)->setText(LNI_ASTRPTR_TO_STRING(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIText_GetText(LNHandle uitext, const LNChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, uitext)->text());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, uitext)->text());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIText_GetTextA(LNHandle uitext, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_A(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, uitext)->text());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, uitext)->text());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIText_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIText_OnSerialize_SetOverrideCallback(LNUIText_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIText::s_LNUIText_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIText_SetPrototype_OnSerialize(LNHandle uitext, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, uitext)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UITextSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIText_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIText>()->id();
}

LN_FLAT_API void LNUIText_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIText>(), id);
}

void LNUIText_RegisterSubclassTypeInfo(const LNUIText_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIText>(), info->subclassId);
        *LNWS_ln_UIText::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIText_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIText, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUISprite_Create(LNHandle* outUISprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUISprite, LNWS_ln_UISprite, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUISprite_CreateWithTexture(LNHandle texture, LNHandle* outUISprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUISprite, LNWS_ln_UISprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUISprite_SetTexture(LNHandle uisprite, LNHandle texture)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, texture)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUISprite_SetSourceRect(LNHandle uisprite, const LNRect* rect)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->setSourceRect(*reinterpret_cast<const ln::Rect*>(rect)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUISprite_SetSourceRectXYWH(LNHandle uisprite, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->setSourceRect(x, y, width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUISprite_GetSourceRect(LNHandle uisprite, LNRect* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNRect>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->sourceRect());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->sourceRect());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUISprite_SetShader(LNHandle uisprite, LNHandle shader)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->setShader(LNI_HANDLE_TO_OBJECT(ln::Shader, shader)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUISprite_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUISprite_OnSerialize_SetOverrideCallback(LNUISprite_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UISprite::s_LNUISprite_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUISprite_SetPrototype_OnSerialize(LNHandle uisprite, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UISpriteSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUISprite_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UISprite>()->id();
}

LN_FLAT_API void LNUISprite_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UISprite>(), id);
}

void LNUISprite_RegisterSubclassTypeInfo(const LNUISprite_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UISprite>(), info->subclassId);
        *LNWS_ln_UISprite::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUISprite_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIIcon_LoadFontIcon(const LNChar* iconName, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::UIIcon::loadFontIcon(iconName));
    }
    else {
        (ln::UIIcon::loadFontIcon(iconName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIIcon_LoadFontIconA(const char* iconName, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::UIIcon::loadFontIcon(LNI_ASTRPTR_TO_STRING(iconName)));
    }
    else {
        (ln::UIIcon::loadFontIcon(LNI_ASTRPTR_TO_STRING(iconName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSize(const LNChar* iconName, int size, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::UIIcon::loadFontIcon(iconName, size));
    }
    else {
        (ln::UIIcon::loadFontIcon(iconName, size));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSizeA(const char* iconName, int size, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::UIIcon::loadFontIcon(LNI_ASTRPTR_TO_STRING(iconName), size));
    }
    else {
        (ln::UIIcon::loadFontIcon(LNI_ASTRPTR_TO_STRING(iconName), size));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSizeColor(const LNChar* iconName, int size, const LNColor* color, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::UIIcon::loadFontIcon(iconName, size, *reinterpret_cast<const ln::Color*>(color)));
    }
    else {
        (ln::UIIcon::loadFontIcon(iconName, size, *reinterpret_cast<const ln::Color*>(color)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIIcon_LoadFontIconWithNameSizeColorA(const char* iconName, int size, const LNColor* color, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::UIIcon::loadFontIcon(LNI_ASTRPTR_TO_STRING(iconName), size, *reinterpret_cast<const ln::Color*>(color)));
    }
    else {
        (ln::UIIcon::loadFontIcon(LNI_ASTRPTR_TO_STRING(iconName), size, *reinterpret_cast<const ln::Color*>(color)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIIcon_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIIcon, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIIcon_OnSerialize_SetOverrideCallback(LNUIIcon_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIIcon::s_LNUIIcon_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIIcon_SetPrototype_OnSerialize(LNHandle uiicon, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIIcon, uiicon)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIIconSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIIcon_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIIcon>()->id();
}

LN_FLAT_API void LNUIIcon_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIIcon>(), id);
}

void LNUIIcon_RegisterSubclassTypeInfo(const LNUIIcon_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIIcon>(), info->subclassId);
        *LNWS_ln_UIIcon::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIIcon_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIIcon, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIMessageTextArea_Create(LNHandle* outUIMessageTextArea)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIMessageTextArea, LNWS_ln_UIMessageTextArea, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIMessageTextArea_SetText(LNHandle uimessagetextarea, const LNChar* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIMessageTextArea, uimessagetextarea)->setText(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIMessageTextArea_SetTextA(LNHandle uimessagetextarea, const char* value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIMessageTextArea, uimessagetextarea)->setText(LNI_ASTRPTR_TO_STRING(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIMessageTextArea_SetTypingSpeed(LNHandle uimessagetextarea, float value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIMessageTextArea, uimessagetextarea)->setTypingSpeed(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIMessageTextArea_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIMessageTextArea, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIMessageTextArea_OnSerialize_SetOverrideCallback(LNUIMessageTextArea_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIMessageTextArea::s_LNUIMessageTextArea_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIMessageTextArea_SetPrototype_OnSerialize(LNHandle uimessagetextarea, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIMessageTextArea, uimessagetextarea)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIMessageTextAreaSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIMessageTextArea_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIMessageTextArea>()->id();
}

LN_FLAT_API void LNUIMessageTextArea_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIMessageTextArea>(), id);
}

void LNUIMessageTextArea_RegisterSubclassTypeInfo(const LNUIMessageTextArea_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIMessageTextArea>(), info->subclassId);
        *LNWS_ln_UIMessageTextArea::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIMessageTextArea_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIMessageTextArea, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUI_Add(LNHandle element)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::UI::add(LNI_HANDLE_TO_OBJECT(ln::UIElement, element)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUI_Remove(LNHandle element)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::UI::remove(LNI_HANDLE_TO_OBJECT(ln::UIElement, element)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUILayoutPanel_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutPanel, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUILayoutPanel_OnSerialize_SetOverrideCallback(LNUILayoutPanel_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UILayoutPanel::s_LNUILayoutPanel_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUILayoutPanel_SetPrototype_OnSerialize(LNHandle uilayoutpanel, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutPanel, uilayoutpanel)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UILayoutPanelSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUILayoutPanel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UILayoutPanel>()->id();
}

LN_FLAT_API void LNUILayoutPanel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UILayoutPanel>(), id);
}

void LNUILayoutPanel_RegisterSubclassTypeInfo(const LNUILayoutPanel_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UILayoutPanel>(), info->subclassId);
        *LNWS_ln_UILayoutPanel::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUILayoutPanel_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutPanel, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIBoxLayout_Create(LNHandle* outUIBoxLayout)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIBoxLayout, LNWS_ln_UIBoxLayout, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIBoxLayout_SetOrientation(LNHandle uiboxlayout, LNUILayoutOrientation orientation)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIBoxLayout, uiboxlayout)->setOrientation(static_cast<ln::UILayoutOrientation>(orientation)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIBoxLayout_GetOrientation(LNHandle uiboxlayout, LNUILayoutOrientation* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNUILayoutOrientation>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIBoxLayout, uiboxlayout)->orientation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIBoxLayout, uiboxlayout)->orientation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIBoxLayout_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIBoxLayout, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIBoxLayout_OnSerialize_SetOverrideCallback(LNUIBoxLayout_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIBoxLayout::s_LNUIBoxLayout_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIBoxLayout_SetPrototype_OnSerialize(LNHandle uiboxlayout, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIBoxLayout, uiboxlayout)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIBoxLayoutSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIBoxLayout_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIBoxLayout>()->id();
}

LN_FLAT_API void LNUIBoxLayout_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIBoxLayout>(), id);
}

void LNUIBoxLayout_RegisterSubclassTypeInfo(const LNUIBoxLayout_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIBoxLayout>(), info->subclassId);
        *LNWS_ln_UIBoxLayout::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIBoxLayout_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIBoxLayout, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIStackLayout_Create(LNHandle* outUIStackLayout)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIStackLayout, LNWS_ln_UIStackLayout, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIStackLayout_SetOrientation(LNHandle uistacklayout, LNUILayoutOrientation orientation)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIStackLayout, uistacklayout)->setOrientation(static_cast<ln::UILayoutOrientation>(orientation)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIStackLayout_GetOrientation(LNHandle uistacklayout, LNUILayoutOrientation* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNUILayoutOrientation>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIStackLayout, uistacklayout)->getOrientation());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIStackLayout, uistacklayout)->getOrientation());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIStackLayout_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIStackLayout, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIStackLayout_OnSerialize_SetOverrideCallback(LNUIStackLayout_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIStackLayout::s_LNUIStackLayout_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIStackLayout_SetPrototype_OnSerialize(LNHandle uistacklayout, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIStackLayout, uistacklayout)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIStackLayoutSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIStackLayout_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIStackLayout>()->id();
}

LN_FLAT_API void LNUIStackLayout_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIStackLayout>(), id);
}

void LNUIStackLayout_RegisterSubclassTypeInfo(const LNUIStackLayout_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIStackLayout>(), info->subclassId);
        *LNWS_ln_UIStackLayout::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIStackLayout_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIStackLayout, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIGridLayout_Create(LNHandle* outUIGridLayout)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIGridLayout, LNWS_ln_UIGridLayout, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIGridLayout_SetColumnCount(LNHandle uigridlayout, int value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIGridLayout, uigridlayout)->setColumnCount(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIGridLayout_SetRow(LNHandle element, int row)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::UIGridLayout::setRow(LNI_HANDLE_TO_OBJECT(ln::UIElement, element), row));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIGridLayout_SetColumn(LNHandle element, int column)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::UIGridLayout::setColumn(LNI_HANDLE_TO_OBJECT(ln::UIElement, element), column));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIGridLayout_SetPlacement(LNHandle element, int row, int column, int rowSpan, int columnSpan)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::UIGridLayout::setPlacement(LNI_HANDLE_TO_OBJECT(ln::UIElement, element), row, column, rowSpan, columnSpan));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIGridLayout_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIGridLayout, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIGridLayout_OnSerialize_SetOverrideCallback(LNUIGridLayout_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIGridLayout::s_LNUIGridLayout_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIGridLayout_SetPrototype_OnSerialize(LNHandle uigridlayout, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIGridLayout, uigridlayout)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIGridLayoutSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIGridLayout_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIGridLayout>()->id();
}

LN_FLAT_API void LNUIGridLayout_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIGridLayout>(), id);
}

void LNUIGridLayout_RegisterSubclassTypeInfo(const LNUIGridLayout_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIGridLayout>(), info->subclassId);
        *LNWS_ln_UIGridLayout::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIGridLayout_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIGridLayout, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIControl_Create(LNHandle* outUIControl)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIControl, LNWS_ln_UIControl, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIControl_AddInlineVisual(LNHandle uicontrol, LNHandle element, LNUIInlinePlacement layout)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIControl, uicontrol)->addInlineVisual(LNI_HANDLE_TO_OBJECT(ln::UIElement, element), static_cast<ln::UIInlinePlacement>(layout)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIControl_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIControl, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIControl_OnSerialize_SetOverrideCallback(LNUIControl_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIControl::s_LNUIControl_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIControl_SetPrototype_OnSerialize(LNHandle uicontrol, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIControl, uicontrol)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIControlSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIControl_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIControl>()->id();
}

LN_FLAT_API void LNUIControl_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIControl>(), id);
}

void LNUIControl_RegisterSubclassTypeInfo(const LNUIControl_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIControl>(), info->subclassId);
        *LNWS_ln_UIControl::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIControl_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIControl, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIButtonBase_SetText(LNHandle uibuttonbase, const LNChar* text)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, uibuttonbase)->setText(text));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIButtonBase_SetTextA(LNHandle uibuttonbase, const char* text)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, uibuttonbase)->setText(LNI_ASTRPTR_TO_STRING(text)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIButtonBase_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIButtonBase_OnSerialize_SetOverrideCallback(LNUIButtonBase_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIButtonBase::s_LNUIButtonBase_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIButtonBase_SetPrototype_OnSerialize(LNHandle uibuttonbase, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, uibuttonbase)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIButtonBaseSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIButtonBase_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIButtonBase>()->id();
}

LN_FLAT_API void LNUIButtonBase_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIButtonBase>(), id);
}

void LNUIButtonBase_RegisterSubclassTypeInfo(const LNUIButtonBase_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIButtonBase>(), info->subclassId);
        *LNWS_ln_UIButtonBase::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIButtonBase_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIButton_Create(LNHandle* outUIButton)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIButton, LNWS_ln_UIButton, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIButton_CreateWithText(const LNChar* text, LNHandle* outUIButton)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIButton, LNWS_ln_UIButton, init, text);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIButton_CreateWithTextA(const char* text, LNHandle* outUIButton)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIButton, LNWS_ln_UIButton, init, LNI_ASTRPTR_TO_STRING(text));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIButton_ConnectOnClicked(LNHandle uibutton, LNHandle handler, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, uibutton)->connectOnClicked(LNI_HANDLE_TO_OBJECT(ln::UIEventHandler, handler)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, uibutton)->connectOnClicked(LNI_HANDLE_TO_OBJECT(ln::UIEventHandler, handler)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIButton_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIButton_OnSerialize_SetOverrideCallback(LNUIButton_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIButton::s_LNUIButton_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIButton_SetPrototype_OnSerialize(LNHandle uibutton, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, uibutton)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIButtonSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIButton_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIButton>()->id();
}

LN_FLAT_API void LNUIButton_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIButton>(), id);
}

void LNUIButton_RegisterSubclassTypeInfo(const LNUIButton_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIButton>(), info->subclassId);
        *LNWS_ln_UIButton::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIButton_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIWindow_Create(LNHandle* outUIWindow)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIWindow, LNWS_ln_UIWindow, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIWindow_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIWindow, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIWindow_OnSerialize_SetOverrideCallback(LNUIWindow_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIWindow::s_LNUIWindow_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIWindow_SetPrototype_OnSerialize(LNHandle uiwindow, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIWindow, uiwindow)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIWindowSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIWindow_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIWindow>()->id();
}

LN_FLAT_API void LNUIWindow_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIWindow>(), id);
}

void LNUIWindow_RegisterSubclassTypeInfo(const LNUIWindow_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIWindow>(), info->subclassId);
        *LNWS_ln_UIWindow::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIWindow_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIWindow, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIListItem_ConnectOnSubmit(LNHandle uilistitem, LNHandle handler, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItem, uilistitem)->connectOnSubmit(LNI_HANDLE_TO_OBJECT(ln::UIGeneralEventHandler, handler)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItem, uilistitem)->connectOnSubmit(LNI_HANDLE_TO_OBJECT(ln::UIGeneralEventHandler, handler)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListItem_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItem, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIListItem_OnSerialize_SetOverrideCallback(LNUIListItem_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIListItem::s_LNUIListItem_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIListItem_SetPrototype_OnSerialize(LNHandle uilistitem, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItem, uilistitem)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIListItemSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIListItem_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIListItem>()->id();
}

LN_FLAT_API void LNUIListItem_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListItem>(), id);
}

void LNUIListItem_RegisterSubclassTypeInfo(const LNUIListItem_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListItem>(), info->subclassId);
        *LNWS_ln_UIListItem::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListItem_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItem, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIListItemsControl_SetItemsLayoutPanel(LNHandle uilistitemscontrol, LNHandle layout)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->setItemsLayoutPanel(LNI_HANDLE_TO_OBJECT(ln::UILayoutPanel, layout)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListItemsControl_GetItemsLayoutPanel(LNHandle uilistitemscontrol, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->itemsLayoutPanel());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->itemsLayoutPanel());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListItemsControl_SetSubmitMode(LNHandle uilistitemscontrol, LNUIListSubmitMode value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->setSubmitMode(static_cast<ln::UIListSubmitMode>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListItemsControl_GetSubmitMode(LNHandle uilistitemscontrol, LNUIListSubmitMode* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNUIListSubmitMode>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->submitMode());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->submitMode());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListItemsControl_ConnectOnSubmit(LNHandle uilistitemscontrol, LNHandle handler, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->connectOnSubmit(LNI_HANDLE_TO_OBJECT(ln::UIGeneralEventHandler, handler)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->connectOnSubmit(LNI_HANDLE_TO_OBJECT(ln::UIGeneralEventHandler, handler)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListItemsControl_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIListItemsControl_OnSerialize_SetOverrideCallback(LNUIListItemsControl_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIListItemsControl::s_LNUIListItemsControl_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIListItemsControl_SetPrototype_OnSerialize(LNHandle uilistitemscontrol, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, uilistitemscontrol)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIListItemsControlSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIListItemsControl_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIListItemsControl>()->id();
}

LN_FLAT_API void LNUIListItemsControl_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListItemsControl>(), id);
}

void LNUIListItemsControl_RegisterSubclassTypeInfo(const LNUIListItemsControl_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListItemsControl>(), info->subclassId);
        *LNWS_ln_UIListItemsControl::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListItemsControl_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListItemsControl, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIListBoxItem_Create(LNHandle content, LNHandle* outUIListBoxItem)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIListBoxItem, LNWS_ln_UIListBoxItem, init, LNI_HANDLE_TO_OBJECT(ln::UIElement, content));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListBoxItem_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBoxItem, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIListBoxItem_OnSerialize_SetOverrideCallback(LNUIListBoxItem_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIListBoxItem::s_LNUIListBoxItem_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIListBoxItem_SetPrototype_OnSerialize(LNHandle uilistboxitem, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBoxItem, uilistboxitem)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIListBoxItemSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIListBoxItem_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIListBoxItem>()->id();
}

LN_FLAT_API void LNUIListBoxItem_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListBoxItem>(), id);
}

void LNUIListBoxItem_RegisterSubclassTypeInfo(const LNUIListBoxItem_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListBoxItem>(), info->subclassId);
        *LNWS_ln_UIListBoxItem::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListBoxItem_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBoxItem, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNUIListBox_Create(LNHandle* outUIListBox)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIListBox, LNWS_ln_UIListBox, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListBox_AddItem(LNHandle uilistbox, LNHandle content, LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBox, uilistbox)->addItem(LNI_HANDLE_TO_OBJECT(ln::UIElement, content)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBox, uilistbox)->addItem(LNI_HANDLE_TO_OBJECT(ln::UIElement, content)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIListBox_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBox, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUIListBox_OnSerialize_SetOverrideCallback(LNUIListBox_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIListBox::s_LNUIListBox_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNUIListBox_SetPrototype_OnSerialize(LNHandle uilistbox, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBox, uilistbox)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIListBoxSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNUIListBox_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIListBox>()->id();
}

LN_FLAT_API void LNUIListBox_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListBox>(), id);
}

void LNUIListBox_RegisterSubclassTypeInfo(const LNUIListBox_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIListBox>(), info->subclassId);
        *LNWS_ln_UIListBox::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListBox_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIListBox, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNInputGesture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InputGesture, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNInputGesture_OnSerialize_SetOverrideCallback(LNInputGesture_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_InputGesture::s_LNInputGesture_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNInputGesture_SetPrototype_OnSerialize(LNHandle inputgesture, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_InputGesture, inputgesture)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InputGestureSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNInputGesture_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::InputGesture>()->id();
}

LN_FLAT_API void LNInputGesture_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InputGesture>(), id);
}

void LNInputGesture_RegisterSubclassTypeInfo(const LNInputGesture_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InputGesture>(), info->subclassId);
        *LNWS_ln_InputGesture::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInputGesture_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_InputGesture, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNKeyGesture_Create(LNKeys key, LNHandle* outKeyGesture)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outKeyGesture, LNWS_ln_KeyGesture, init, static_cast<ln::Keys>(key));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNKeyGesture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_KeyGesture, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNKeyGesture_OnSerialize_SetOverrideCallback(LNKeyGesture_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_KeyGesture::s_LNKeyGesture_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNKeyGesture_SetPrototype_OnSerialize(LNHandle keygesture, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_KeyGesture, keygesture)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_KeyGestureSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNKeyGesture_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::KeyGesture>()->id();
}

LN_FLAT_API void LNKeyGesture_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::KeyGesture>(), id);
}

void LNKeyGesture_RegisterSubclassTypeInfo(const LNKeyGesture_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::KeyGesture>(), info->subclassId);
        *LNWS_ln_KeyGesture::subclassInfo() = *info;
    }
}

LNSubinstanceId LNKeyGesture_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_KeyGesture, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNInput_IsPressed(const LNChar* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isPressed(buttonName));
    }
    else {
        (ln::Input::isPressed(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_IsPressedA(const char* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isPressed(LNI_ASTRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::isPressed(LNI_ASTRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_IsTriggered(const LNChar* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isTriggered(buttonName));
    }
    else {
        (ln::Input::isTriggered(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_IsTriggeredA(const char* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isTriggered(LNI_ASTRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::isTriggered(LNI_ASTRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_IsTriggeredOff(const LNChar* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isTriggeredOff(buttonName));
    }
    else {
        (ln::Input::isTriggeredOff(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_IsTriggeredOffA(const char* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isTriggeredOff(LNI_ASTRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::isTriggeredOff(LNI_ASTRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_IsRepeated(const LNChar* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isRepeated(buttonName));
    }
    else {
        (ln::Input::isRepeated(buttonName));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_IsRepeatedA(const char* buttonName, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(ln::Input::isRepeated(LNI_ASTRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::isRepeated(LNI_ASTRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_GetAxisValue(const LNChar* buttonName, float* outReturn)
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


LN_FLAT_API LNResult LNInput_GetAxisValueA(const char* buttonName, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (ln::Input::getAxisValue(LNI_ASTRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::getAxisValue(LNI_ASTRPTR_TO_STRING(buttonName)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_AddBinding(const LNChar* buttonName, LNHandle gesture)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Input::addBinding(buttonName, LNI_HANDLE_TO_OBJECT(ln::InputGesture, gesture)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_AddBindingA(const char* buttonName, LNHandle gesture)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Input::addBinding(LNI_ASTRPTR_TO_STRING(buttonName), LNI_HANDLE_TO_OBJECT(ln::InputGesture, gesture)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_RemoveBinding(LNHandle gesture)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Input::removeBinding(LNI_HANDLE_TO_OBJECT(ln::InputGesture, gesture)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_ClearBindings(const LNChar* buttonName)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Input::clearBindings(buttonName));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_ClearBindingsA(const char* buttonName)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Input::clearBindings(LNI_ASTRPTR_TO_STRING(buttonName)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInput_ClearAllBindings()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Input::clearAllBindings());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNMouse_Pressed(LNMouseButtons button, LNBool* outReturn)
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


LN_FLAT_API LNResult LNMouse_Triggered(LNMouseButtons button, LNBool* outReturn)
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


LN_FLAT_API LNResult LNMouse_TriggeredOff(LNMouseButtons button, LNBool* outReturn)
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


LN_FLAT_API LNResult LNMouse_Repeated(LNMouseButtons button, LNBool* outReturn)
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


LN_FLAT_API LNResult LNMouse_GetPosition(LNPoint* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = ln::detail::convertStructForced<LNPoint>(ln::Mouse::position());
    }
    else {
        (ln::Mouse::position());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommand_Code(LNHandle interpretercommand, const LNChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->code());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->code());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommand_CodeA(LNHandle interpretercommand, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_A(LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->code());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->code());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommand_ParamsCount(LNHandle interpretercommand, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->paramsCount());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->paramsCount());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommand_Param(LNHandle interpretercommand, int index, const LNChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->param(index));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->param(index));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommand_ParamA(LNHandle interpretercommand, int index, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_A(LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->param(index));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->param(index));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommand_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNInterpreterCommand_OnSerialize_SetOverrideCallback(LNInterpreterCommand_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_InterpreterCommand::s_LNInterpreterCommand_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNInterpreterCommand_SetPrototype_OnSerialize(LNHandle interpretercommand, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterCommandSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNInterpreterCommand_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::InterpreterCommand>()->id();
}

LN_FLAT_API void LNInterpreterCommand_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InterpreterCommand>(), id);
}

void LNInterpreterCommand_RegisterSubclassTypeInfo(const LNInterpreterCommand_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InterpreterCommand>(), info->subclassId);
        *LNWS_ln_InterpreterCommand::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreterCommand_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNInterpreterCommandList_Create(LNHandle* outInterpreterCommandList)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outInterpreterCommandList, LNWS_ln_InterpreterCommandList, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand(LNHandle interpretercommandlist, const LNChar* code)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand(code));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommandA(LNHandle interpretercommandlist, const char* code)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand(LNI_ASTRPTR_TO_STRING(code)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand1(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand1(code, param0));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand1A(LNHandle interpretercommandlist, const char* code, const char* param0)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand1(LNI_ASTRPTR_TO_STRING(code), LNI_ASTRPTR_TO_STRING(param0)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand2(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand2(code, param0, param1));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand2A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand2(LNI_ASTRPTR_TO_STRING(code), LNI_ASTRPTR_TO_STRING(param0), LNI_ASTRPTR_TO_STRING(param1)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand3(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1, const LNChar* param2)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand3(code, param0, param1, param2));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand3A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1, const char* param2)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand3(LNI_ASTRPTR_TO_STRING(code), LNI_ASTRPTR_TO_STRING(param0), LNI_ASTRPTR_TO_STRING(param1), LNI_ASTRPTR_TO_STRING(param2)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand4(LNHandle interpretercommandlist, const LNChar* code, const LNChar* param0, const LNChar* param1, const LNChar* param2, const LNChar* param3)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand4(code, param0, param1, param2, param3));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_AddCommand4A(LNHandle interpretercommandlist, const char* code, const char* param0, const char* param1, const char* param2, const char* param3)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand4(LNI_ASTRPTR_TO_STRING(code), LNI_ASTRPTR_TO_STRING(param0), LNI_ASTRPTR_TO_STRING(param1), LNI_ASTRPTR_TO_STRING(param2), LNI_ASTRPTR_TO_STRING(param3)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreterCommandList_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNInterpreterCommandList_OnSerialize_SetOverrideCallback(LNInterpreterCommandList_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_InterpreterCommandList::s_LNInterpreterCommandList_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNInterpreterCommandList_SetPrototype_OnSerialize(LNHandle interpretercommandlist, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterCommandListSerializeHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNInterpreterCommandList_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::InterpreterCommandList>()->id();
}

LN_FLAT_API void LNInterpreterCommandList_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InterpreterCommandList>(), id);
}

void LNInterpreterCommandList_RegisterSubclassTypeInfo(const LNInterpreterCommandList_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InterpreterCommandList>(), info->subclassId);
        *LNWS_ln_InterpreterCommandList::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreterCommandList_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNInterpreterCommandDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InterpreterCommandDelegate>(), id);
}

void LNInterpreterCommandDelegate_RegisterSubclassTypeInfo(const LNInterpreterCommandDelegate_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::InterpreterCommandDelegate>(), info->subclassId);
        *LNWS_ln_InterpreterCommandDelegate::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreterCommandDelegate_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandDelegate, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNInterpreter_Create(LNHandle* outInterpreter)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outInterpreter, LNWS_ln_Interpreter, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_Clear(LNHandle interpreter)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->clear());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_Run(LNHandle interpreter, LNHandle commandList)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->run(LNI_HANDLE_TO_OBJECT(ln::InterpreterCommandList, commandList)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_IsRunning(LNHandle interpreter, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->isRunning());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->isRunning());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_Update(LNHandle interpreter)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->update());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_Terminate(LNHandle interpreter)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->terminate());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_RegisterCommandHandler(LNHandle interpreter, const LNChar* name, LNHandle handler)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->registerCommandHandler(name, LNI_HANDLE_TO_OBJECT(ln::InterpreterCommandDelegate, handler)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_RegisterCommandHandlerA(LNHandle interpreter, const char* name, LNHandle handler)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->registerCommandHandler(LNI_ASTRPTR_TO_STRING(name), LNI_HANDLE_TO_OBJECT(ln::InterpreterCommandDelegate, handler)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_SetWaitMode(LNHandle interpreter, const LNChar* mode)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->setWaitMode(mode));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_SetWaitModeA(LNHandle interpreter, const char* mode)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->setWaitMode(LNI_ASTRPTR_TO_STRING(mode)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_GetWaitMode(LNHandle interpreter, const LNChar** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_UTF16(LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->waitMode());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->waitMode());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_GetWaitModeA(LNHandle interpreter, const char** outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_STRING_TO_STRPTR_A(LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->waitMode());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->waitMode());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_SetWaitCount(LNHandle interpreter, int count)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->setWaitCount(count));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_GetWaitCount(LNHandle interpreter, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->waitCount());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->waitCount());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNInterpreter_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNInterpreter_OnSerialize_SetOverrideCallback(LNInterpreter_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Interpreter::s_LNInterpreter_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNInterpreter_OnUpdateWait_CallOverrideBase(LNHandle interpreter, LNBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->onUpdateWait_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNInterpreter_OnUpdateWait_SetOverrideCallback(LNInterpreter_OnUpdateWait_OverrideCallback callback)
{
    LNWS_ln_Interpreter::s_LNInterpreter_OnUpdateWait_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnSerialize(LNHandle interpreter, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnUpdateWait(LNHandle interpreter, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->acquireOverridePrototypes()->OnUpdateWait_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterUpdateWaitHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNInterpreter_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Interpreter>()->id();
}

LN_FLAT_API void LNInterpreter_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Interpreter>(), id);
}

void LNInterpreter_RegisterSubclassTypeInfo(const LNInterpreter_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Interpreter>(), info->subclassId);
        *LNWS_ln_Interpreter::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreter_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitle(const LNChar* title)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowTitle(title));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitleA(const char* title)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowTitle(LNI_ASTRPTR_TO_STRING(title)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetMainWindowSize(int width, int height)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowSize(width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetMainWorldViewSize(int width, int height)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWorldViewSize(width, height));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetMainWindowResizable(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowResizable(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_AddAssetDirectory(const LNChar* path)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetDirectory(path));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_AddAssetDirectoryA(const char* path)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetDirectory(LNI_ASTRPTR_TO_STRING(path)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_AddAssetArchive(const LNChar* fileFullPath, const LNChar* password)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetArchive(fileFullPath, password));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_AddAssetArchiveA(const char* fileFullPath, const char* password)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::addAssetArchive(LNI_ASTRPTR_TO_STRING(fileFullPath), LNI_ASTRPTR_TO_STRING(password)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetFrameRate(int value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setFrameRate(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetUITheme(const LNChar* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setUITheme(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetUIThemeA(const char* value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setUITheme(LNI_ASTRPTR_TO_STRING(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetFontFile(const LNChar* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setFontFile(filePath));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetFontFileA(const char* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setFontFile(LNI_ASTRPTR_TO_STRING(filePath)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetDebugToolEnabled(LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setDebugToolEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetDebugMode(LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setDebugMode(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetEngineLogFilePath(const LNChar* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setEngineLogFilePath(filePath));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetEngineLogFilePathA(const char* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setEngineLogFilePath(LNI_ASTRPTR_TO_STRING(filePath)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetPriorityGPUName(const LNChar* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setPriorityGPUName(filePath));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetPriorityGPUNameA(const char* filePath)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setPriorityGPUName(LNI_ASTRPTR_TO_STRING(filePath)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetDeveloperToolEnabled(LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setDevelopmentToolsEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetUserMainWindow(intptr_t value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setUserMainWindow(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_Initialize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::initialize());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_Terminate()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::terminate());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_Update(LNBool* outReturn)
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


LN_FLAT_API LNResult LNEngine_GetTime(double* outReturn)
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


LN_FLAT_API LNResult LNEngine_GetWorld(LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Engine::world());
    }
    else {
        (ln::Engine::world());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_GetMainCamera(LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Engine::mainCamera());
    }
    else {
        (ln::Engine::mainCamera());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_GetMainLight(LNHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE(ln::Engine::mainLight());
    }
    else {
        (ln::Engine::mainLight());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_GetRenderView(LNHandle* outReturn)
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


LN_FLAT_API LNResult LNApplication_Create(LNHandle* outApplication)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outApplication, LNWS_ln_Application, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNApplication_OnInit(LNHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->LNWS_ln_Application::onInit_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNApplication_OnUpdate(LNHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->LNWS_ln_Application::onUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNApplication_World(LNHandle application, LNHandle* outReturn)
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


LN_FLAT_API LNResult LNApplication_Run(LNHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->run());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNApplication_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNApplication_OnSerialize_SetOverrideCallback(LNApplication_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Application::s_LNApplication_OnSerialize_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNApplication_OnInit_CallOverrideBase(LNHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->onInit_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNApplication_OnInit_SetOverrideCallback(LNApplication_OnInit_OverrideCallback callback)
{
    LNWS_ln_Application::s_LNApplication_OnInit_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNApplication_OnUpdate_CallOverrideBase(LNHandle application)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->onUpdate_CallBase());
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNApplication_OnUpdate_SetOverrideCallback(LNApplication_OnUpdate_OverrideCallback callback)
{
    LNWS_ln_Application::s_LNApplication_OnUpdate_OverrideCallback = callback;
    return LN_OK;
}

LN_FLAT_API LNResult LNApplication_SetPrototype_OnSerialize(LNHandle application, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ApplicationSerializeHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNApplication_SetPrototype_OnInit(LNHandle application, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnInit_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ApplicationInitHandler, callback);
    return LN_OK;
}
LN_FLAT_API LNResult LNApplication_SetPrototype_OnUpdate(LNHandle application, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ApplicationUpdateHandler, callback);
    return LN_OK;
}
extern LN_FLAT_API int LNApplication_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Application>()->id();
}

LN_FLAT_API void LNApplication_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Application>(), id);
}

void LNApplication_RegisterSubclassTypeInfo(const LNApplication_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Application>(), info->subclassId);
        *LNWS_ln_Application::subclassInfo() = *info;
    }
}

LNSubinstanceId LNApplication_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API LNResult LNDebug_SetGuideGridEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::setGuideGridEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_SetPhysicsDebugDrawEnabled(LNBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::setPhysicsDebugDrawEnabled(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_Print(const LNChar* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(str));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_PrintA(const char* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(LNI_ASTRPTR_TO_STRING(str)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_PrintWithTime(float time, const LNChar* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, str));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_PrintWithTimeA(float time, const char* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, LNI_ASTRPTR_TO_STRING(str)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_PrintWithTimeAndColor(float time, const LNColor* color, const LNChar* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, *reinterpret_cast<const ln::Color*>(color), str));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_PrintWithTimeAndColorA(float time, const LNColor* color, const char* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(time, *reinterpret_cast<const ln::Color*>(color), LNI_ASTRPTR_TO_STRING(str)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API void LNObjectSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ObjectSerializeHandler>(), id);
}

void LNObjectSerializeHandler_RegisterSubclassTypeInfo(const LNObjectSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ObjectSerializeHandler>(), info->subclassId);
        *LNWS_ObjectSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNObjectSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ObjectSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNEventConnectionSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EventConnectionSerializeHandler>(), id);
}

void LNEventConnectionSerializeHandler_RegisterSubclassTypeInfo(const LNEventConnectionSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EventConnectionSerializeHandler>(), info->subclassId);
        *LNWS_EventConnectionSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEventConnectionSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_EventConnectionSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNVariantSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VariantSerializeHandler>(), id);
}

void LNVariantSerializeHandler_RegisterSubclassTypeInfo(const LNVariantSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VariantSerializeHandler>(), info->subclassId);
        *LNWS_VariantSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVariantSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_VariantSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNZVTestClass1SerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ZVTestClass1SerializeHandler>(), id);
}

void LNZVTestClass1SerializeHandler_RegisterSubclassTypeInfo(const LNZVTestClass1SerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ZVTestClass1SerializeHandler>(), info->subclassId);
        *LNWS_ZVTestClass1SerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestClass1SerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ZVTestClass1SerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNZVTestEventArgs1SerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ZVTestEventArgs1SerializeHandler>(), id);
}

void LNZVTestEventArgs1SerializeHandler_RegisterSubclassTypeInfo(const LNZVTestEventArgs1SerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ZVTestEventArgs1SerializeHandler>(), info->subclassId);
        *LNWS_ZVTestEventArgs1SerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNZVTestEventArgs1SerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ZVTestEventArgs1SerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSerializer2SerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<Serializer2SerializeHandler>(), id);
}

void LNSerializer2SerializeHandler_RegisterSubclassTypeInfo(const LNSerializer2SerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<Serializer2SerializeHandler>(), info->subclassId);
        *LNWS_Serializer2SerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSerializer2SerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_Serializer2SerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNAssetObjectSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AssetObjectSerializeHandler>(), id);
}

void LNAssetObjectSerializeHandler_RegisterSubclassTypeInfo(const LNAssetObjectSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AssetObjectSerializeHandler>(), info->subclassId);
        *LNWS_AssetObjectSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAssetObjectSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_AssetObjectSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNAssetImportSettingsSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AssetImportSettingsSerializeHandler>(), id);
}

void LNAssetImportSettingsSerializeHandler_RegisterSubclassTypeInfo(const LNAssetImportSettingsSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AssetImportSettingsSerializeHandler>(), info->subclassId);
        *LNWS_AssetImportSettingsSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAssetImportSettingsSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_AssetImportSettingsSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNAssetModelSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AssetModelSerializeHandler>(), id);
}

void LNAssetModelSerializeHandler_RegisterSubclassTypeInfo(const LNAssetModelSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AssetModelSerializeHandler>(), info->subclassId);
        *LNWS_AssetModelSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAssetModelSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_AssetModelSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSoundSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SoundSerializeHandler>(), id);
}

void LNSoundSerializeHandler_RegisterSubclassTypeInfo(const LNSoundSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SoundSerializeHandler>(), info->subclassId);
        *LNWS_SoundSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSoundSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SoundSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNTextureSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<TextureSerializeHandler>(), id);
}

void LNTextureSerializeHandler_RegisterSubclassTypeInfo(const LNTextureSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<TextureSerializeHandler>(), info->subclassId);
        *LNWS_TextureSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTextureSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_TextureSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNTexture2DSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<Texture2DSerializeHandler>(), id);
}

void LNTexture2DSerializeHandler_RegisterSubclassTypeInfo(const LNTexture2DSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<Texture2DSerializeHandler>(), info->subclassId);
        *LNWS_Texture2DSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTexture2DSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_Texture2DSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNShaderSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShaderSerializeHandler>(), id);
}

void LNShaderSerializeHandler_RegisterSubclassTypeInfo(const LNShaderSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShaderSerializeHandler>(), info->subclassId);
        *LNWS_ShaderSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNShaderSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ShaderSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNRenderViewSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<RenderViewSerializeHandler>(), id);
}

void LNRenderViewSerializeHandler_RegisterSubclassTypeInfo(const LNRenderViewSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<RenderViewSerializeHandler>(), info->subclassId);
        *LNWS_RenderViewSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNRenderViewSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_RenderViewSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNMaterialSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MaterialSerializeHandler>(), id);
}

void LNMaterialSerializeHandler_RegisterSubclassTypeInfo(const LNMaterialSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MaterialSerializeHandler>(), info->subclassId);
        *LNWS_MaterialSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMaterialSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_MaterialSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNMeshNodeSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshNodeSerializeHandler>(), id);
}

void LNMeshNodeSerializeHandler_RegisterSubclassTypeInfo(const LNMeshNodeSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshNodeSerializeHandler>(), info->subclassId);
        *LNWS_MeshNodeSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshNodeSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_MeshNodeSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNAnimationControllerSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationControllerSerializeHandler>(), id);
}

void LNAnimationControllerSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationControllerSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationControllerSerializeHandler>(), info->subclassId);
        *LNWS_AnimationControllerSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationControllerSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_AnimationControllerSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNMeshModelSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshModelSerializeHandler>(), id);
}

void LNMeshModelSerializeHandler_RegisterSubclassTypeInfo(const LNMeshModelSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshModelSerializeHandler>(), info->subclassId);
        *LNWS_MeshModelSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshModelSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_MeshModelSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNMeshImportSettingsSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshImportSettingsSerializeHandler>(), id);
}

void LNMeshImportSettingsSerializeHandler_RegisterSubclassTypeInfo(const LNMeshImportSettingsSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshImportSettingsSerializeHandler>(), info->subclassId);
        *LNWS_MeshImportSettingsSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshImportSettingsSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_MeshImportSettingsSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSkinnedMeshModelSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SkinnedMeshModelSerializeHandler>(), id);
}

void LNSkinnedMeshModelSerializeHandler_RegisterSubclassTypeInfo(const LNSkinnedMeshModelSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SkinnedMeshModelSerializeHandler>(), info->subclassId);
        *LNWS_SkinnedMeshModelSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSkinnedMeshModelSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SkinnedMeshModelSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCollisionShapeSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CollisionShapeSerializeHandler>(), id);
}

void LNCollisionShapeSerializeHandler_RegisterSubclassTypeInfo(const LNCollisionShapeSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CollisionShapeSerializeHandler>(), info->subclassId);
        *LNWS_CollisionShapeSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCollisionShapeSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_CollisionShapeSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNBoxCollisionShapeSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxCollisionShapeSerializeHandler>(), id);
}

void LNBoxCollisionShapeSerializeHandler_RegisterSubclassTypeInfo(const LNBoxCollisionShapeSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxCollisionShapeSerializeHandler>(), info->subclassId);
        *LNWS_BoxCollisionShapeSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNBoxCollisionShapeSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_BoxCollisionShapeSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNAnimationCurveSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationCurveSerializeHandler>(), id);
}

void LNAnimationCurveSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationCurveSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationCurveSerializeHandler>(), info->subclassId);
        *LNWS_AnimationCurveSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationCurveSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_AnimationCurveSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNKeyFrameAnimationCurveSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<KeyFrameAnimationCurveSerializeHandler>(), id);
}

void LNKeyFrameAnimationCurveSerializeHandler_RegisterSubclassTypeInfo(const LNKeyFrameAnimationCurveSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<KeyFrameAnimationCurveSerializeHandler>(), info->subclassId);
        *LNWS_KeyFrameAnimationCurveSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNKeyFrameAnimationCurveSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_KeyFrameAnimationCurveSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNAnimationClipSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationClipSerializeHandler>(), id);
}

void LNAnimationClipSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationClipSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationClipSerializeHandler>(), info->subclassId);
        *LNWS_AnimationClipSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationClipSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_AnimationClipSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNAnimationStateSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationStateSerializeHandler>(), id);
}

void LNAnimationStateSerializeHandler_RegisterSubclassTypeInfo(const LNAnimationStateSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<AnimationStateSerializeHandler>(), info->subclassId);
        *LNWS_AnimationStateSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNAnimationStateSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_AnimationStateSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNEffectResourceSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EffectResourceSerializeHandler>(), id);
}

void LNEffectResourceSerializeHandler_RegisterSubclassTypeInfo(const LNEffectResourceSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EffectResourceSerializeHandler>(), info->subclassId);
        *LNWS_EffectResourceSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEffectResourceSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_EffectResourceSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNParticleEmitterModelSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterModelSerializeHandler>(), id);
}

void LNParticleEmitterModelSerializeHandler_RegisterSubclassTypeInfo(const LNParticleEmitterModelSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterModelSerializeHandler>(), info->subclassId);
        *LNWS_ParticleEmitterModelSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleEmitterModelSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterModelSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNParticleModelSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleModelSerializeHandler>(), id);
}

void LNParticleModelSerializeHandler_RegisterSubclassTypeInfo(const LNParticleModelSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleModelSerializeHandler>(), info->subclassId);
        *LNWS_ParticleModelSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleModelSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ParticleModelSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNComponentSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ComponentSerializeHandler>(), id);
}

void LNComponentSerializeHandler_RegisterSubclassTypeInfo(const LNComponentSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ComponentSerializeHandler>(), info->subclassId);
        *LNWS_ComponentSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNComponentSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ComponentSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNVisualComponentSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualComponentSerializeHandler>(), id);
}

void LNVisualComponentSerializeHandler_RegisterSubclassTypeInfo(const LNVisualComponentSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualComponentSerializeHandler>(), info->subclassId);
        *LNWS_VisualComponentSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVisualComponentSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_VisualComponentSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSpriteComponentSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpriteComponentSerializeHandler>(), id);
}

void LNSpriteComponentSerializeHandler_RegisterSubclassTypeInfo(const LNSpriteComponentSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpriteComponentSerializeHandler>(), info->subclassId);
        *LNWS_SpriteComponentSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpriteComponentSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SpriteComponentSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCharacterControllerSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CharacterControllerSerializeHandler>(), id);
}

void LNCharacterControllerSerializeHandler_RegisterSubclassTypeInfo(const LNCharacterControllerSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CharacterControllerSerializeHandler>(), info->subclassId);
        *LNWS_CharacterControllerSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCharacterControllerSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_CharacterControllerSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNWorldSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldSerializeHandler>(), id);
}

void LNWorldSerializeHandler_RegisterSubclassTypeInfo(const LNWorldSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldSerializeHandler>(), info->subclassId);
        *LNWS_WorldSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorldSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_WorldSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNComponentListSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ComponentListSerializeHandler>(), id);
}

void LNComponentListSerializeHandler_RegisterSubclassTypeInfo(const LNComponentListSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ComponentListSerializeHandler>(), info->subclassId);
        *LNWS_ComponentListSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNComponentListSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ComponentListSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNWorldObjectSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldObjectSerializeHandler>(), id);
}

void LNWorldObjectSerializeHandler_RegisterSubclassTypeInfo(const LNWorldObjectSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldObjectSerializeHandler>(), info->subclassId);
        *LNWS_WorldObjectSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorldObjectSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNWorldObjectPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldObjectPreUpdateHandler>(), id);
}

void LNWorldObjectPreUpdateHandler_RegisterSubclassTypeInfo(const LNWorldObjectPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldObjectPreUpdateHandler>(), info->subclassId);
        *LNWS_WorldObjectPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorldObjectPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNWorldObjectUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldObjectUpdateHandler>(), id);
}

void LNWorldObjectUpdateHandler_RegisterSubclassTypeInfo(const LNWorldObjectUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldObjectUpdateHandler>(), info->subclassId);
        *LNWS_WorldObjectUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorldObjectUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNVisualObjectSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualObjectSerializeHandler>(), id);
}

void LNVisualObjectSerializeHandler_RegisterSubclassTypeInfo(const LNVisualObjectSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualObjectSerializeHandler>(), info->subclassId);
        *LNWS_VisualObjectSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVisualObjectSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNVisualObjectPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualObjectPreUpdateHandler>(), id);
}

void LNVisualObjectPreUpdateHandler_RegisterSubclassTypeInfo(const LNVisualObjectPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualObjectPreUpdateHandler>(), info->subclassId);
        *LNWS_VisualObjectPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVisualObjectPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNVisualObjectUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualObjectUpdateHandler>(), id);
}

void LNVisualObjectUpdateHandler_RegisterSubclassTypeInfo(const LNVisualObjectUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<VisualObjectUpdateHandler>(), info->subclassId);
        *LNWS_VisualObjectUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNVisualObjectUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCameraSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraSerializeHandler>(), id);
}

void LNCameraSerializeHandler_RegisterSubclassTypeInfo(const LNCameraSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraSerializeHandler>(), info->subclassId);
        *LNWS_CameraSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCameraSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_CameraSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCameraPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraPreUpdateHandler>(), id);
}

void LNCameraPreUpdateHandler_RegisterSubclassTypeInfo(const LNCameraPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraPreUpdateHandler>(), info->subclassId);
        *LNWS_CameraPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCameraPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_CameraPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCameraUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraUpdateHandler>(), id);
}

void LNCameraUpdateHandler_RegisterSubclassTypeInfo(const LNCameraUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraUpdateHandler>(), info->subclassId);
        *LNWS_CameraUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCameraUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_CameraUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNEnvironmentLightSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EnvironmentLightSerializeHandler>(), id);
}

void LNEnvironmentLightSerializeHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EnvironmentLightSerializeHandler>(), info->subclassId);
        *LNWS_EnvironmentLightSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEnvironmentLightSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNEnvironmentLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EnvironmentLightPreUpdateHandler>(), id);
}

void LNEnvironmentLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EnvironmentLightPreUpdateHandler>(), info->subclassId);
        *LNWS_EnvironmentLightPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEnvironmentLightPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNEnvironmentLightUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EnvironmentLightUpdateHandler>(), id);
}

void LNEnvironmentLightUpdateHandler_RegisterSubclassTypeInfo(const LNEnvironmentLightUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<EnvironmentLightUpdateHandler>(), info->subclassId);
        *LNWS_EnvironmentLightUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNEnvironmentLightUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNDirectionalLightSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<DirectionalLightSerializeHandler>(), id);
}

void LNDirectionalLightSerializeHandler_RegisterSubclassTypeInfo(const LNDirectionalLightSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<DirectionalLightSerializeHandler>(), info->subclassId);
        *LNWS_DirectionalLightSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNDirectionalLightSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNDirectionalLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<DirectionalLightPreUpdateHandler>(), id);
}

void LNDirectionalLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNDirectionalLightPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<DirectionalLightPreUpdateHandler>(), info->subclassId);
        *LNWS_DirectionalLightPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNDirectionalLightPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNDirectionalLightUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<DirectionalLightUpdateHandler>(), id);
}

void LNDirectionalLightUpdateHandler_RegisterSubclassTypeInfo(const LNDirectionalLightUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<DirectionalLightUpdateHandler>(), info->subclassId);
        *LNWS_DirectionalLightUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNDirectionalLightUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNPointLightSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PointLightSerializeHandler>(), id);
}

void LNPointLightSerializeHandler_RegisterSubclassTypeInfo(const LNPointLightSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PointLightSerializeHandler>(), info->subclassId);
        *LNWS_PointLightSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPointLightSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_PointLightSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNPointLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PointLightPreUpdateHandler>(), id);
}

void LNPointLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNPointLightPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PointLightPreUpdateHandler>(), info->subclassId);
        *LNWS_PointLightPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPointLightPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_PointLightPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNPointLightUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PointLightUpdateHandler>(), id);
}

void LNPointLightUpdateHandler_RegisterSubclassTypeInfo(const LNPointLightUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PointLightUpdateHandler>(), info->subclassId);
        *LNWS_PointLightUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPointLightUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_PointLightUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSpotLightSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpotLightSerializeHandler>(), id);
}

void LNSpotLightSerializeHandler_RegisterSubclassTypeInfo(const LNSpotLightSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpotLightSerializeHandler>(), info->subclassId);
        *LNWS_SpotLightSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpotLightSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SpotLightSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSpotLightPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpotLightPreUpdateHandler>(), id);
}

void LNSpotLightPreUpdateHandler_RegisterSubclassTypeInfo(const LNSpotLightPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpotLightPreUpdateHandler>(), info->subclassId);
        *LNWS_SpotLightPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpotLightPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SpotLightPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSpotLightUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpotLightUpdateHandler>(), id);
}

void LNSpotLightUpdateHandler_RegisterSubclassTypeInfo(const LNSpotLightUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpotLightUpdateHandler>(), info->subclassId);
        *LNWS_SpotLightUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpotLightUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SpotLightUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSpriteSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpriteSerializeHandler>(), id);
}

void LNSpriteSerializeHandler_RegisterSubclassTypeInfo(const LNSpriteSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpriteSerializeHandler>(), info->subclassId);
        *LNWS_SpriteSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpriteSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SpriteSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSpritePreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpritePreUpdateHandler>(), id);
}

void LNSpritePreUpdateHandler_RegisterSubclassTypeInfo(const LNSpritePreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpritePreUpdateHandler>(), info->subclassId);
        *LNWS_SpritePreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpritePreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SpritePreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNSpriteUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpriteUpdateHandler>(), id);
}

void LNSpriteUpdateHandler_RegisterSubclassTypeInfo(const LNSpriteUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<SpriteUpdateHandler>(), info->subclassId);
        *LNWS_SpriteUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNSpriteUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_SpriteUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCameraOrbitControlComponentSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraOrbitControlComponentSerializeHandler>(), id);
}

void LNCameraOrbitControlComponentSerializeHandler_RegisterSubclassTypeInfo(const LNCameraOrbitControlComponentSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CameraOrbitControlComponentSerializeHandler>(), info->subclassId);
        *LNWS_CameraOrbitControlComponentSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCameraOrbitControlComponentSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_CameraOrbitControlComponentSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNRaycasterSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<RaycasterSerializeHandler>(), id);
}

void LNRaycasterSerializeHandler_RegisterSubclassTypeInfo(const LNRaycasterSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<RaycasterSerializeHandler>(), info->subclassId);
        *LNWS_RaycasterSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNRaycasterSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_RaycasterSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNRaycastResultSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<RaycastResultSerializeHandler>(), id);
}

void LNRaycastResultSerializeHandler_RegisterSubclassTypeInfo(const LNRaycastResultSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<RaycastResultSerializeHandler>(), info->subclassId);
        *LNWS_RaycastResultSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNRaycastResultSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_RaycastResultSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNWorldRenderViewSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldRenderViewSerializeHandler>(), id);
}

void LNWorldRenderViewSerializeHandler_RegisterSubclassTypeInfo(const LNWorldRenderViewSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<WorldRenderViewSerializeHandler>(), info->subclassId);
        *LNWS_WorldRenderViewSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNWorldRenderViewSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_WorldRenderViewSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNShapeObjectSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShapeObjectSerializeHandler>(), id);
}

void LNShapeObjectSerializeHandler_RegisterSubclassTypeInfo(const LNShapeObjectSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShapeObjectSerializeHandler>(), info->subclassId);
        *LNWS_ShapeObjectSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNShapeObjectSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ShapeObjectSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNShapeObjectPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShapeObjectPreUpdateHandler>(), id);
}

void LNShapeObjectPreUpdateHandler_RegisterSubclassTypeInfo(const LNShapeObjectPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShapeObjectPreUpdateHandler>(), info->subclassId);
        *LNWS_ShapeObjectPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNShapeObjectPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ShapeObjectPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNShapeObjectUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShapeObjectUpdateHandler>(), id);
}

void LNShapeObjectUpdateHandler_RegisterSubclassTypeInfo(const LNShapeObjectUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ShapeObjectUpdateHandler>(), info->subclassId);
        *LNWS_ShapeObjectUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNShapeObjectUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ShapeObjectUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNPlaneMeshSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PlaneMeshSerializeHandler>(), id);
}

void LNPlaneMeshSerializeHandler_RegisterSubclassTypeInfo(const LNPlaneMeshSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PlaneMeshSerializeHandler>(), info->subclassId);
        *LNWS_PlaneMeshSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPlaneMeshSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNPlaneMeshPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PlaneMeshPreUpdateHandler>(), id);
}

void LNPlaneMeshPreUpdateHandler_RegisterSubclassTypeInfo(const LNPlaneMeshPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PlaneMeshPreUpdateHandler>(), info->subclassId);
        *LNWS_PlaneMeshPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPlaneMeshPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNPlaneMeshUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PlaneMeshUpdateHandler>(), id);
}

void LNPlaneMeshUpdateHandler_RegisterSubclassTypeInfo(const LNPlaneMeshUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<PlaneMeshUpdateHandler>(), info->subclassId);
        *LNWS_PlaneMeshUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNPlaneMeshUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNBoxMeshSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxMeshSerializeHandler>(), id);
}

void LNBoxMeshSerializeHandler_RegisterSubclassTypeInfo(const LNBoxMeshSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxMeshSerializeHandler>(), info->subclassId);
        *LNWS_BoxMeshSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNBoxMeshSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNBoxMeshPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxMeshPreUpdateHandler>(), id);
}

void LNBoxMeshPreUpdateHandler_RegisterSubclassTypeInfo(const LNBoxMeshPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxMeshPreUpdateHandler>(), info->subclassId);
        *LNWS_BoxMeshPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNBoxMeshPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNBoxMeshUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxMeshUpdateHandler>(), id);
}

void LNBoxMeshUpdateHandler_RegisterSubclassTypeInfo(const LNBoxMeshUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<BoxMeshUpdateHandler>(), info->subclassId);
        *LNWS_BoxMeshUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNBoxMeshUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNMeshComponentSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshComponentSerializeHandler>(), id);
}

void LNMeshComponentSerializeHandler_RegisterSubclassTypeInfo(const LNMeshComponentSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<MeshComponentSerializeHandler>(), info->subclassId);
        *LNWS_MeshComponentSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNMeshComponentSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_MeshComponentSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNCollisionSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CollisionSerializeHandler>(), id);
}

void LNCollisionSerializeHandler_RegisterSubclassTypeInfo(const LNCollisionSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<CollisionSerializeHandler>(), info->subclassId);
        *LNWS_CollisionSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNCollisionSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_CollisionSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNTriggerBodyComponentSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<TriggerBodyComponentSerializeHandler>(), id);
}

void LNTriggerBodyComponentSerializeHandler_RegisterSubclassTypeInfo(const LNTriggerBodyComponentSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<TriggerBodyComponentSerializeHandler>(), info->subclassId);
        *LNWS_TriggerBodyComponentSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNTriggerBodyComponentSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_TriggerBodyComponentSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNParticleEmitterSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterSerializeHandler>(), id);
}

void LNParticleEmitterSerializeHandler_RegisterSubclassTypeInfo(const LNParticleEmitterSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterSerializeHandler>(), info->subclassId);
        *LNWS_ParticleEmitterSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleEmitterSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNParticleEmitterPreUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterPreUpdateHandler>(), id);
}

void LNParticleEmitterPreUpdateHandler_RegisterSubclassTypeInfo(const LNParticleEmitterPreUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterPreUpdateHandler>(), info->subclassId);
        *LNWS_ParticleEmitterPreUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleEmitterPreUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterPreUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNParticleEmitterUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterUpdateHandler>(), id);
}

void LNParticleEmitterUpdateHandler_RegisterSubclassTypeInfo(const LNParticleEmitterUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ParticleEmitterUpdateHandler>(), info->subclassId);
        *LNWS_ParticleEmitterUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNParticleEmitterUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ParticleEmitterUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNLevelSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelSerializeHandler>(), id);
}

void LNLevelSerializeHandler_RegisterSubclassTypeInfo(const LNLevelSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelSerializeHandler>(), info->subclassId);
        *LNWS_LevelSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNLevelSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_LevelSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNLevelStartHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelStartHandler>(), id);
}

void LNLevelStartHandler_RegisterSubclassTypeInfo(const LNLevelStartHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelStartHandler>(), info->subclassId);
        *LNWS_LevelStartHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNLevelStartHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_LevelStartHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNLevelStopHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelStopHandler>(), id);
}

void LNLevelStopHandler_RegisterSubclassTypeInfo(const LNLevelStopHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelStopHandler>(), info->subclassId);
        *LNWS_LevelStopHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNLevelStopHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_LevelStopHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNLevelPauseHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelPauseHandler>(), id);
}

void LNLevelPauseHandler_RegisterSubclassTypeInfo(const LNLevelPauseHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelPauseHandler>(), info->subclassId);
        *LNWS_LevelPauseHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNLevelPauseHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_LevelPauseHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNLevelResumeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelResumeHandler>(), id);
}

void LNLevelResumeHandler_RegisterSubclassTypeInfo(const LNLevelResumeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelResumeHandler>(), info->subclassId);
        *LNWS_LevelResumeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNLevelResumeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_LevelResumeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNLevelUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelUpdateHandler>(), id);
}

void LNLevelUpdateHandler_RegisterSubclassTypeInfo(const LNLevelUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<LevelUpdateHandler>(), info->subclassId);
        *LNWS_LevelUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNLevelUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_LevelUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIEventArgsSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIEventArgsSerializeHandler>(), id);
}

void LNUIEventArgsSerializeHandler_RegisterSubclassTypeInfo(const LNUIEventArgsSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIEventArgsSerializeHandler>(), info->subclassId);
        *LNWS_UIEventArgsSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIEventArgsSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIEventArgsSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUILayoutElementSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UILayoutElementSerializeHandler>(), id);
}

void LNUILayoutElementSerializeHandler_RegisterSubclassTypeInfo(const LNUILayoutElementSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UILayoutElementSerializeHandler>(), info->subclassId);
        *LNWS_UILayoutElementSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUILayoutElementSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UILayoutElementSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIElementSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIElementSerializeHandler>(), id);
}

void LNUIElementSerializeHandler_RegisterSubclassTypeInfo(const LNUIElementSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIElementSerializeHandler>(), info->subclassId);
        *LNWS_UIElementSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIElementSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIElementSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUITextSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UITextSerializeHandler>(), id);
}

void LNUITextSerializeHandler_RegisterSubclassTypeInfo(const LNUITextSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UITextSerializeHandler>(), info->subclassId);
        *LNWS_UITextSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUITextSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UITextSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUISpriteSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UISpriteSerializeHandler>(), id);
}

void LNUISpriteSerializeHandler_RegisterSubclassTypeInfo(const LNUISpriteSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UISpriteSerializeHandler>(), info->subclassId);
        *LNWS_UISpriteSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUISpriteSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UISpriteSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIIconSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIIconSerializeHandler>(), id);
}

void LNUIIconSerializeHandler_RegisterSubclassTypeInfo(const LNUIIconSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIIconSerializeHandler>(), info->subclassId);
        *LNWS_UIIconSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIIconSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIIconSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIMessageTextAreaSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIMessageTextAreaSerializeHandler>(), id);
}

void LNUIMessageTextAreaSerializeHandler_RegisterSubclassTypeInfo(const LNUIMessageTextAreaSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIMessageTextAreaSerializeHandler>(), info->subclassId);
        *LNWS_UIMessageTextAreaSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIMessageTextAreaSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIMessageTextAreaSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUILayoutPanelSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UILayoutPanelSerializeHandler>(), id);
}

void LNUILayoutPanelSerializeHandler_RegisterSubclassTypeInfo(const LNUILayoutPanelSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UILayoutPanelSerializeHandler>(), info->subclassId);
        *LNWS_UILayoutPanelSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUILayoutPanelSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UILayoutPanelSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIBoxLayoutSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIBoxLayoutSerializeHandler>(), id);
}

void LNUIBoxLayoutSerializeHandler_RegisterSubclassTypeInfo(const LNUIBoxLayoutSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIBoxLayoutSerializeHandler>(), info->subclassId);
        *LNWS_UIBoxLayoutSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIBoxLayoutSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIBoxLayoutSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIStackLayoutSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIStackLayoutSerializeHandler>(), id);
}

void LNUIStackLayoutSerializeHandler_RegisterSubclassTypeInfo(const LNUIStackLayoutSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIStackLayoutSerializeHandler>(), info->subclassId);
        *LNWS_UIStackLayoutSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIStackLayoutSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIStackLayoutSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIGridLayoutSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIGridLayoutSerializeHandler>(), id);
}

void LNUIGridLayoutSerializeHandler_RegisterSubclassTypeInfo(const LNUIGridLayoutSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIGridLayoutSerializeHandler>(), info->subclassId);
        *LNWS_UIGridLayoutSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIGridLayoutSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIGridLayoutSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIControlSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIControlSerializeHandler>(), id);
}

void LNUIControlSerializeHandler_RegisterSubclassTypeInfo(const LNUIControlSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIControlSerializeHandler>(), info->subclassId);
        *LNWS_UIControlSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIControlSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIControlSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIButtonBaseSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIButtonBaseSerializeHandler>(), id);
}

void LNUIButtonBaseSerializeHandler_RegisterSubclassTypeInfo(const LNUIButtonBaseSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIButtonBaseSerializeHandler>(), info->subclassId);
        *LNWS_UIButtonBaseSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIButtonBaseSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIButtonBaseSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIButtonSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIButtonSerializeHandler>(), id);
}

void LNUIButtonSerializeHandler_RegisterSubclassTypeInfo(const LNUIButtonSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIButtonSerializeHandler>(), info->subclassId);
        *LNWS_UIButtonSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIButtonSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIButtonSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIWindowSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIWindowSerializeHandler>(), id);
}

void LNUIWindowSerializeHandler_RegisterSubclassTypeInfo(const LNUIWindowSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIWindowSerializeHandler>(), info->subclassId);
        *LNWS_UIWindowSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIWindowSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIWindowSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIListItemSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListItemSerializeHandler>(), id);
}

void LNUIListItemSerializeHandler_RegisterSubclassTypeInfo(const LNUIListItemSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListItemSerializeHandler>(), info->subclassId);
        *LNWS_UIListItemSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListItemSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIListItemSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIListItemsControlSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListItemsControlSerializeHandler>(), id);
}

void LNUIListItemsControlSerializeHandler_RegisterSubclassTypeInfo(const LNUIListItemsControlSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListItemsControlSerializeHandler>(), info->subclassId);
        *LNWS_UIListItemsControlSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListItemsControlSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIListItemsControlSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIListBoxItemSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListBoxItemSerializeHandler>(), id);
}

void LNUIListBoxItemSerializeHandler_RegisterSubclassTypeInfo(const LNUIListBoxItemSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListBoxItemSerializeHandler>(), info->subclassId);
        *LNWS_UIListBoxItemSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListBoxItemSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIListBoxItemSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNUIListBoxSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListBoxSerializeHandler>(), id);
}

void LNUIListBoxSerializeHandler_RegisterSubclassTypeInfo(const LNUIListBoxSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UIListBoxSerializeHandler>(), info->subclassId);
        *LNWS_UIListBoxSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUIListBoxSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UIListBoxSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNInputGestureSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InputGestureSerializeHandler>(), id);
}

void LNInputGestureSerializeHandler_RegisterSubclassTypeInfo(const LNInputGestureSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InputGestureSerializeHandler>(), info->subclassId);
        *LNWS_InputGestureSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInputGestureSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_InputGestureSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNKeyGestureSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<KeyGestureSerializeHandler>(), id);
}

void LNKeyGestureSerializeHandler_RegisterSubclassTypeInfo(const LNKeyGestureSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<KeyGestureSerializeHandler>(), info->subclassId);
        *LNWS_KeyGestureSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNKeyGestureSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_KeyGestureSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNInterpreterCommandSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterCommandSerializeHandler>(), id);
}

void LNInterpreterCommandSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterCommandSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterCommandSerializeHandler>(), info->subclassId);
        *LNWS_InterpreterCommandSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreterCommandSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_InterpreterCommandSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNInterpreterCommandListSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterCommandListSerializeHandler>(), id);
}

void LNInterpreterCommandListSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterCommandListSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterCommandListSerializeHandler>(), info->subclassId);
        *LNWS_InterpreterCommandListSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreterCommandListSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_InterpreterCommandListSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNInterpreterSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterSerializeHandler>(), id);
}

void LNInterpreterSerializeHandler_RegisterSubclassTypeInfo(const LNInterpreterSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterSerializeHandler>(), info->subclassId);
        *LNWS_InterpreterSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreterSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_InterpreterSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNInterpreterUpdateWaitHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterUpdateWaitHandler>(), id);
}

void LNInterpreterUpdateWaitHandler_RegisterSubclassTypeInfo(const LNInterpreterUpdateWaitHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<InterpreterUpdateWaitHandler>(), info->subclassId);
        *LNWS_InterpreterUpdateWaitHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNInterpreterUpdateWaitHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_InterpreterUpdateWaitHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNApplicationSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ApplicationSerializeHandler>(), id);
}

void LNApplicationSerializeHandler_RegisterSubclassTypeInfo(const LNApplicationSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ApplicationSerializeHandler>(), info->subclassId);
        *LNWS_ApplicationSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNApplicationSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ApplicationSerializeHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNApplicationInitHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ApplicationInitHandler>(), id);
}

void LNApplicationInitHandler_RegisterSubclassTypeInfo(const LNApplicationInitHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ApplicationInitHandler>(), info->subclassId);
        *LNWS_ApplicationInitHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNApplicationInitHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ApplicationInitHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}

LN_FLAT_API void LNApplicationUpdateHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ApplicationUpdateHandler>(), id);
}

void LNApplicationUpdateHandler_RegisterSubclassTypeInfo(const LNApplicationUpdateHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ApplicationUpdateHandler>(), info->subclassId);
        *LNWS_ApplicationUpdateHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNApplicationUpdateHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ApplicationUpdateHandler, handle))->m_subinstance;
        LNI_FUNC_TRY_END_RETURN;
    }
    return 0;
}


	
} // extern "C"
