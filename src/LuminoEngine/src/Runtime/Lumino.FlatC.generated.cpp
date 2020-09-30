#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>

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
        if (r != LN_SUCCESS) { LN_ERROR("LNObjectSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNEventConnectionSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNZVTestClass1SerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNZVTestEventArgs1SerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNSerializer2SerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNAssetModelSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNTextureSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNTexture2DSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNRenderViewSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNComponentSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNVisualComponentSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNSpriteComponentSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNWorldSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNComponentListSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNWorldObjectSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNWorldObjectUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNVisualObjectSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNVisualObjectUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNCameraSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNCameraUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNEnvironmentLightSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNEnvironmentLightUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNDirectionalLightSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNDirectionalLightUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNPointLightSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNPointLightUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNSpotLightSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNSpotLightUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNSpriteSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNSpriteUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNCameraOrbitControlComponentSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNRaycasterSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNRaycastResultSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNWorldRenderViewSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNBoxMeshSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNBoxMeshUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNPlaneMeshSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNPlaneMeshUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNLevelSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNLevelStartHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNLevelStopHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNLevelPauseHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNLevelResumeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNLevelUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNUIEventArgsSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNUILayoutElementSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNUIElementSerializeHandlerCallback"); }
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
using UITextBlockSerializeHandler = ln::Delegate<void(ln::UITextBlock* self, ln::Serializer2* ar)>;

class LNWS_UITextBlockSerializeHandler : public UITextBlockSerializeHandler
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUITextBlockSerializeHandler_OverridePrototypes
    {

    };
    std::unique_ptr<LNUITextBlockSerializeHandler_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUITextBlockSerializeHandler_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUITextBlockSerializeHandler_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUITextBlockSerializeHandler_SubclassRegistrationInfo* subclassInfo() { static LNUITextBlockSerializeHandler_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNUITextBlockSerializeHandlerCallback m_callback;

    LNWS_UITextBlockSerializeHandler()
      : UITextBlockSerializeHandler([this](ln::UITextBlock* self, ln::Serializer2* ar) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(self), LNI_OBJECT_TO_HANDLE(ar));
        if (r != LN_SUCCESS) { LN_ERROR("LNUITextBlockSerializeHandlerCallback"); }
    })
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_UITextBlockSerializeHandler()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    bool init(LNUITextBlockSerializeHandlerCallback callback)
    {
        if (!UITextBlockSerializeHandler::init()) return false;
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


LN_FLAT_API LNResult LNUITextBlockSerializeHandler_Create(LNUITextBlockSerializeHandlerCallback callback, LNHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_UITextBlockSerializeHandler, init, callback);
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
        if (r != LN_SUCCESS) { LN_ERROR("LNUISpriteSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNInterpreterCommandSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNInterpreterCommandListSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNInterpreterSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNInterpreterUpdateWaitHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNApplicationSerializeHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNApplicationInitHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNApplicationUpdateHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNPromiseFailureDelegateCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNZVTestDelegate1Callback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNZVTestDelegate2Callback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNZVTestDelegate3Callback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNZVTestEventHandler1Callback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNZVTestEventHandler2Callback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNTexture2DDelegateCallback"); }
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
            return ln::TypeInfo::getTypeInfo<Object>();
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
LNWorldObject_OnUpdate_OverrideCallback LNWS_ln_WorldObject::s_LNWorldObject_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_VisualObject : public ln::VisualObject
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNVisualObject_OverridePrototypes
    {
        ln::Ref<LNWS_VisualObjectSerializeHandler> OnSerialize_OverrideFunc;
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
LNVisualObject_OnUpdate_OverrideCallback LNWS_ln_VisualObject::s_LNVisualObject_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_Camera : public ln::Camera
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNCamera_OverridePrototypes
    {
        ln::Ref<LNWS_CameraSerializeHandler> OnSerialize_OverrideFunc;
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
LNCamera_OnUpdate_OverrideCallback LNWS_ln_Camera::s_LNCamera_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_EnvironmentLight : public ln::EnvironmentLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNEnvironmentLight_OverridePrototypes
    {
        ln::Ref<LNWS_EnvironmentLightSerializeHandler> OnSerialize_OverrideFunc;
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
LNEnvironmentLight_OnUpdate_OverrideCallback LNWS_ln_EnvironmentLight::s_LNEnvironmentLight_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_DirectionalLight : public ln::DirectionalLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNDirectionalLight_OverridePrototypes
    {
        ln::Ref<LNWS_DirectionalLightSerializeHandler> OnSerialize_OverrideFunc;
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
LNDirectionalLight_OnUpdate_OverrideCallback LNWS_ln_DirectionalLight::s_LNDirectionalLight_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_PointLight : public ln::PointLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPointLight_OverridePrototypes
    {
        ln::Ref<LNWS_PointLightSerializeHandler> OnSerialize_OverrideFunc;
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
LNPointLight_OnUpdate_OverrideCallback LNWS_ln_PointLight::s_LNPointLight_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_SpotLight : public ln::SpotLight
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNSpotLight_OverridePrototypes
    {
        ln::Ref<LNWS_SpotLightSerializeHandler> OnSerialize_OverrideFunc;
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
        if (r != LN_SUCCESS) { LN_ERROR("LNTestDelegateCallback"); }
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


class LNWS_ln_BoxMesh : public ln::BoxMesh
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNBoxMesh_OverridePrototypes
    {
        ln::Ref<LNWS_BoxMeshSerializeHandler> OnSerialize_OverrideFunc;
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
            return ln::TypeInfo::getTypeInfo<VisualObject>();
    }

};

LNBoxMesh_OnSerialize_OverrideCallback LNWS_ln_BoxMesh::s_LNBoxMesh_OnSerialize_OverrideCallback = nullptr;
LNBoxMesh_OnUpdate_OverrideCallback LNWS_ln_BoxMesh::s_LNBoxMesh_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_PlaneMesh : public ln::PlaneMesh
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNPlaneMesh_OverridePrototypes
    {
        ln::Ref<LNWS_PlaneMeshSerializeHandler> OnSerialize_OverrideFunc;
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
            return ln::TypeInfo::getTypeInfo<VisualObject>();
    }

};

LNPlaneMesh_OnSerialize_OverrideCallback LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnSerialize_OverrideCallback = nullptr;
LNPlaneMesh_OnUpdate_OverrideCallback LNWS_ln_PlaneMesh::s_LNPlaneMesh_OnUpdate_OverrideCallback = nullptr;


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
            return ln::TypeInfo::getTypeInfo<Object>();
    }

};

LNLevel_OnSerialize_OverrideCallback LNWS_ln_Level::s_LNLevel_OnSerialize_OverrideCallback = nullptr;
LNLevel_OnStart_OverrideCallback LNWS_ln_Level::s_LNLevel_OnStart_OverrideCallback = nullptr;
LNLevel_OnStop_OverrideCallback LNWS_ln_Level::s_LNLevel_OnStop_OverrideCallback = nullptr;
LNLevel_OnPause_OverrideCallback LNWS_ln_Level::s_LNLevel_OnPause_OverrideCallback = nullptr;
LNLevel_OnResume_OverrideCallback LNWS_ln_Level::s_LNLevel_OnResume_OverrideCallback = nullptr;
LNLevel_OnUpdate_OverrideCallback LNWS_ln_Level::s_LNLevel_OnUpdate_OverrideCallback = nullptr;


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
        if (r != LN_SUCCESS) { LN_ERROR("LNUIGeneralEventHandlerCallback"); }
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
        if (r != LN_SUCCESS) { LN_ERROR("LNUIEventHandlerCallback"); }
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


class LNWS_ln_UITextBlock : public ln::UITextBlock
{
public:
    // Override functions per instance for FlatAPI User.
    struct LNUITextBlock_OverridePrototypes
    {
        ln::Ref<LNWS_UITextBlockSerializeHandler> OnSerialize_OverrideFunc;

    };
    std::unique_ptr<LNUITextBlock_OverridePrototypes> m_overridePrototypes = nullptr;
    LNUITextBlock_OverridePrototypes* acquireOverridePrototypes() { if (!m_overridePrototypes) m_overridePrototypes = std::make_unique<LNUITextBlock_OverridePrototypes>(); return m_overridePrototypes.get(); }

    static LNUITextBlock_SubclassRegistrationInfo* subclassInfo() { static LNUITextBlock_SubclassRegistrationInfo info; return &info; }
    LNSubinstanceId m_subinstance = 0;

    LNWS_ln_UITextBlock()
    {
        if (subclassInfo()->subinstanceAllocFunc) m_subinstance = subclassInfo()->subinstanceAllocFunc(LNI_OBJECT_TO_HANDLE(this));
    }

    ~LNWS_ln_UITextBlock()
    {
        if (subclassInfo()->subinstanceFreeFunc) subclassInfo()->subinstanceFreeFunc(LNI_OBJECT_TO_HANDLE(this), m_subinstance);
    }

    // Overrides
    static LNUITextBlock_OnSerialize_OverrideCallback s_LNUITextBlock_OnSerialize_OverrideCallback; // deprecated
    virtual void onSerialize(ln::Serializer2* ar) override
    {
        if (m_overridePrototypes) {
            if (auto func = m_overridePrototypes->OnSerialize_OverrideFunc) {
                func->call(this, ar);
                return;
            }
        }
        if (s_LNUITextBlock_OnSerialize_OverrideCallback) s_LNUITextBlock_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
        ln::UITextBlock::onSerialize(ar);
    }
    void onSerialize_CallBase(ln::Serializer2* ar)
    {
        ln::UITextBlock::onSerialize(ar);
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

LNUITextBlock_OnSerialize_OverrideCallback LNWS_ln_UITextBlock::s_LNUITextBlock_OnSerialize_OverrideCallback = nullptr;


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
        if (r != LN_SUCCESS) { LN_ERROR("LNInterpreterCommandDelegateCallback"); }
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




LN_FLAT_API LNResult LNObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNObject_OnSerialize_SetOverrideCallback(LNObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Object::s_LNObject_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNObject_SetPrototype_OnSerialize(LNHandle object, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Object, object)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ObjectSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNEventConnection_SetPrototype_OnSerialize(LNHandle eventconnection, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EventConnection, eventconnection)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EventConnectionSerializeHandler, callback);
    return LN_SUCCESS;
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
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::ZVTestClass1::loadAsync(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::ZVTestClass1::loadAsync(LNI_UTF8STRPTR_TO_STRING(filePath)));
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
        *outReturn = LNI_STRING_TO_STRPTR_UTF8(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->filePath());
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNZVTestClass1_SetPrototype_OnSerialize(LNHandle zvtestclass1, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ZVTestClass1SerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNZVTestEventArgs1_SetPrototype_OnSerialize(LNHandle zvtesteventargs1, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestEventArgs1, zvtesteventargs1)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ZVTestEventArgs1SerializeHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNSerializer2_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNSerializer2_OnSerialize_SetOverrideCallback(LNSerializer2_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Serializer2::s_LNSerializer2_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNSerializer2_SetPrototype_OnSerialize(LNHandle serializer2, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Serializer2, serializer2)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_Serializer2SerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNAssetModel_SetPrototype_OnSerialize(LNHandle assetmodel, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_AssetModel, assetmodel)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_AssetModelSerializeHandler, callback);
    return LN_SUCCESS;
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
    (ln::Assets::saveAssetToLocalFile(LNI_HANDLE_TO_OBJECT(ln::AssetModel, asset), LNI_UTF8STRPTR_TO_STRING(filePath)));
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
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAssetFromLocalFile(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Assets::loadAssetFromLocalFile(LNI_UTF8STRPTR_TO_STRING(filePath)));
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
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Assets::loadAsset(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Assets::loadAsset(LNI_UTF8STRPTR_TO_STRING(filePath)));
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
    (ln::Assets::reloadAsset(LNI_UTF8STRPTR_TO_STRING(filePath), LNI_HANDLE_TO_OBJECT(ln::Object, obj)));
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

LN_FLAT_API LNResult LNTexture_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNTexture_OnSerialize_SetOverrideCallback(LNTexture_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Texture::s_LNTexture_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNTexture_SetPrototype_OnSerialize(LNHandle texture, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture, texture)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_TextureSerializeHandler, callback);
    return LN_SUCCESS;
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
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::load(LNI_UTF8STRPTR_TO_STRING(filePath)));
    }
    else {
        (ln::Texture2D::load(LNI_UTF8STRPTR_TO_STRING(filePath)));
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
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(ln::Texture2D::loadEmoji(LNI_UTF8STRPTR_TO_STRING(code)));
    }
    else {
        (ln::Texture2D::loadEmoji(LNI_UTF8STRPTR_TO_STRING(code)));
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNTexture2D_SetPrototype_OnSerialize(LNHandle texture2d, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Texture2D, texture2d)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_Texture2DSerializeHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNRenderView_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNRenderView_OnSerialize_SetOverrideCallback(LNRenderView_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_RenderView::s_LNRenderView_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNRenderView_SetPrototype_OnSerialize(LNHandle renderview, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RenderView, renderview)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_RenderViewSerializeHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNComponent_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNComponent_OnSerialize_SetOverrideCallback(LNComponent_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Component::s_LNComponent_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNComponent_SetPrototype_OnSerialize(LNHandle component, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Component, component)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ComponentSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNVisualComponent_SetPrototype_OnSerialize(LNHandle visualcomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, visualcomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VisualComponentSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNSpriteComponent_SetPrototype_OnSerialize(LNHandle spritecomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpriteComponent, spritecomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpriteComponentSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNWorld_SetPrototype_OnSerialize(LNHandle world, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_World, world)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNComponentList_SetPrototype_OnSerialize(LNHandle componentlist, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ComponentListSerializeHandler, callback);
    return LN_SUCCESS;
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


LN_FLAT_API LNResult LNWorldObject_SetRotationQuaternion(LNHandle worldobject, const LNQuaternion* rot)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setRotation(*reinterpret_cast<const ln::Quaternion*>(rot)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNWorldObject_SetRotation(LNHandle worldobject, float x, float y, float z)
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


LN_FLAT_API LNResult LNWorldObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNWorldObject_OnSerialize_SetOverrideCallback(LNWorldObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_WorldObject::s_LNWorldObject_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnSerialize(LNHandle worldobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNWorldObject_SetPrototype_OnUpdate(LNHandle worldobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldObjectUpdateHandler, callback);
    return LN_SUCCESS;
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


LN_FLAT_API LNResult LNVisualObject_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNVisualObject_OnSerialize_SetOverrideCallback(LNVisualObject_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_VisualObject::s_LNVisualObject_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnSerialize(LNHandle visualobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNVisualObject_SetPrototype_OnUpdate(LNHandle visualobject, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_VisualObjectUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNCamera_SetPrototype_OnSerialize(LNHandle camera, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, camera)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CameraSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNCamera_SetPrototype_OnUpdate(LNHandle camera, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Camera, camera)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CameraUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnSerialize(LNHandle environmentlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNEnvironmentLight_SetPrototype_OnUpdate(LNHandle environmentlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_EnvironmentLight, environmentlight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_EnvironmentLightUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnSerialize(LNHandle directionallight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNDirectionalLight_SetPrototype_OnUpdate(LNHandle directionallight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_DirectionalLight, directionallight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_DirectionalLightUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNPointLight_SetPrototype_OnSerialize(LNHandle pointlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PointLightSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNPointLight_SetPrototype_OnUpdate(LNHandle pointlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PointLight, pointlight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PointLightUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnSerialize(LNHandle spotlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpotLightSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNSpotLight_SetPrototype_OnUpdate(LNHandle spotlight, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_SpotLight, spotlight)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpotLightUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNSprite_SetPrototype_OnSerialize(LNHandle sprite, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpriteSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNSprite_SetPrototype_OnUpdate(LNHandle sprite, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_SpriteUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNCameraOrbitControlComponent_SetPrototype_OnSerialize(LNHandle cameraorbitcontrolcomponent, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_CameraOrbitControlComponent, cameraorbitcontrolcomponent)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_CameraOrbitControlComponentSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNRaycaster_SetPrototype_OnSerialize(LNHandle raycaster, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Raycaster, raycaster)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_RaycasterSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNRaycastResult_SetPrototype_OnSerialize(LNHandle raycastresult, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_RaycastResult, raycastresult)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_RaycastResultSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNWorldRenderView_SetPrototype_OnSerialize(LNHandle worldrenderview, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldRenderView, worldrenderview)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_WorldRenderViewSerializeHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNBoxMesh_Create(LNHandle* outBoxMesh)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outBoxMesh, LNWS_ln_BoxMesh, init, );
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnSerialize(LNHandle boxmesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, boxmesh)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNBoxMesh_SetPrototype_OnUpdate(LNHandle boxmesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_BoxMesh, boxmesh)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_BoxMeshUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnSerialize(LNHandle planemesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, planemesh)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNPlaneMesh_SetPrototype_OnUpdate(LNHandle planemesh, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_PlaneMesh, planemesh)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_PlaneMeshUpdateHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNLevel_Create(LNHandle* outLevel)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outLevel, LNWS_ln_Level, init, );
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNLevel_SetPrototype_OnSerialize(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStart(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnStart_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelStartHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnStop(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnStop_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelStopHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnPause(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnPause_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelPauseHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnResume(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnResume_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelResumeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNLevel_SetPrototype_OnUpdate(LNHandle level, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Level, level)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_LevelUpdateHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNUIEventArgs_SetPrototype_OnSerialize(LNHandle uieventargs, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, uieventargs)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIEventArgsSerializeHandler, callback);
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNUILayoutElement_SetPrototype_OnSerialize(LNHandle uilayoutelement, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UILayoutElement, uilayoutelement)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UILayoutElementSerializeHandler, callback);
    return LN_SUCCESS;
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


LN_FLAT_API LNResult LNUIElement_SetHAlignment(LNHandle uielement, LNHAlignment value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setHAlignment(static_cast<ln::HAlignment>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetHAlignment(LNHandle uielement, LNHAlignment* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNHAlignment>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->hAlignment());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->hAlignment());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetVAlignment(LNHandle uielement, LNVAlignment value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setVAlignment(static_cast<ln::VAlignment>(value)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_GetVAlignment(LNHandle uielement, LNVAlignment* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = static_cast<LNVAlignment>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->vAlignment());
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->vAlignment());
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUIElement_SetAlignments(LNHandle uielement, LNHAlignment halign, LNVAlignment valign)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->setAlignments(static_cast<ln::HAlignment>(halign), static_cast<ln::VAlignment>(valign)));
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


LN_FLAT_API LNResult LNUIElement_AddChild(LNHandle uielement, LNHandle child)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->addChild(LNI_HANDLE_TO_OBJECT(ln::UIElement, child)));
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNUIElement_SetPrototype_OnSerialize(LNHandle uielement, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UIElementSerializeHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNUITextBlock_Create(LNHandle* outUITextBlock)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUITextBlock, LNWS_ln_UITextBlock, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUITextBlock_CreateWithText(const LNChar* text, LNHandle* outUITextBlock)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUITextBlock, LNWS_ln_UITextBlock, init, text);
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUITextBlock_CreateWithTextA(const char* text, LNHandle* outUITextBlock)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUITextBlock, LNWS_ln_UITextBlock, init, LNI_UTF8STRPTR_TO_STRING(text));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNUITextBlock_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUITextBlock_OnSerialize_SetOverrideCallback(LNUITextBlock_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UITextBlock::s_LNUITextBlock_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNUITextBlock_SetPrototype_OnSerialize(LNHandle uitextblock, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, uitextblock)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UITextBlockSerializeHandler, callback);
    return LN_SUCCESS;
}
extern LN_FLAT_API int LNUITextBlock_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UITextBlock>()->id();
}

LN_FLAT_API void LNUITextBlock_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UITextBlock>(), id);
}

void LNUITextBlock_RegisterSubclassTypeInfo(const LNUITextBlock_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UITextBlock>(), info->subclassId);
        *LNWS_ln_UITextBlock::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUITextBlock_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_ln_UITextBlock, handle))->m_subinstance;
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


LN_FLAT_API LNResult LNUISprite_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNUISprite_OnSerialize_SetOverrideCallback(LNUISprite_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UISprite::s_LNUISprite_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNUISprite_SetPrototype_OnSerialize(LNHandle uisprite, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_UISprite, uisprite)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_UISpriteSerializeHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNInput_Pressed(const LNChar* buttonName, LNBool* outReturn)
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


LN_FLAT_API LNResult LNInput_PressedA(const char* buttonName, LNBool* outReturn)
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


LN_FLAT_API LNResult LNInput_Triggered(const LNChar* buttonName, LNBool* outReturn)
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


LN_FLAT_API LNResult LNInput_TriggeredA(const char* buttonName, LNBool* outReturn)
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


LN_FLAT_API LNResult LNInput_TriggeredOff(const LNChar* buttonName, LNBool* outReturn)
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


LN_FLAT_API LNResult LNInput_TriggeredOffA(const char* buttonName, LNBool* outReturn)
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


LN_FLAT_API LNResult LNInput_Repeated(const LNChar* buttonName, LNBool* outReturn)
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


LN_FLAT_API LNResult LNInput_RepeatedA(const char* buttonName, LNBool* outReturn)
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
        *outReturn = (ln::Input::getAxisValue(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }
    else {
        (ln::Input::getAxisValue(LNI_UTF8STRPTR_TO_STRING(buttonName)));
    }

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


LN_FLAT_API LNResult LNMouse_Position(LNPoint* outReturn)
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
        *outReturn = LNI_STRING_TO_STRPTR_UTF8(LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->code());
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
        *outReturn = LNI_STRING_TO_STRPTR_UTF8(LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->param(index));
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNInterpreterCommand_SetPrototype_OnSerialize(LNHandle interpretercommand, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommand, interpretercommand)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterCommandSerializeHandler, callback);
    return LN_SUCCESS;
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
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand(LNI_UTF8STRPTR_TO_STRING(code)));
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
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand1(LNI_UTF8STRPTR_TO_STRING(code), LNI_UTF8STRPTR_TO_STRING(param0)));
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
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand2(LNI_UTF8STRPTR_TO_STRING(code), LNI_UTF8STRPTR_TO_STRING(param0), LNI_UTF8STRPTR_TO_STRING(param1)));
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
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand3(LNI_UTF8STRPTR_TO_STRING(code), LNI_UTF8STRPTR_TO_STRING(param0), LNI_UTF8STRPTR_TO_STRING(param1), LNI_UTF8STRPTR_TO_STRING(param2)));
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
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->addCommand4(LNI_UTF8STRPTR_TO_STRING(code), LNI_UTF8STRPTR_TO_STRING(param0), LNI_UTF8STRPTR_TO_STRING(param1), LNI_UTF8STRPTR_TO_STRING(param2), LNI_UTF8STRPTR_TO_STRING(param3)));
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNInterpreterCommandList_SetPrototype_OnSerialize(LNHandle interpretercommandlist, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_InterpreterCommandList, interpretercommandlist)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterCommandListSerializeHandler, callback);
    return LN_SUCCESS;
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
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->registerCommandHandler(LNI_UTF8STRPTR_TO_STRING(name), LNI_HANDLE_TO_OBJECT(ln::InterpreterCommandDelegate, handler)));
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
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->setWaitMode(LNI_UTF8STRPTR_TO_STRING(mode)));
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
        *outReturn = LNI_STRING_TO_STRPTR_UTF8(LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->waitMode());
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnSerialize(LNHandle interpreter, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNInterpreter_SetPrototype_OnUpdateWait(LNHandle interpreter, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Interpreter, interpreter)->acquireOverridePrototypes()->OnUpdateWait_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_InterpreterUpdateWaitHandler, callback);
    return LN_SUCCESS;
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


LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitle(const LNChar* title)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowTitle(title));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetMainWindowTitleA(const char* title)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setMainWindowTitle(LNI_UTF8STRPTR_TO_STRING(title)));
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
    (ln::EngineSettings::addAssetDirectory(LNI_UTF8STRPTR_TO_STRING(path)));
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
    (ln::EngineSettings::addAssetArchive(LNI_UTF8STRPTR_TO_STRING(fileFullPath), LNI_UTF8STRPTR_TO_STRING(password)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetFrameRate(int value)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setFrameRate(value));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetDebugToolEnabled(LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setDebugToolEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngineSettings_SetEngineLogEnabled(LNBool enabled)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::EngineSettings::setEngineLogEnabled(LNI_LNBOOL_TO_BOOL(enabled)));
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
    (ln::EngineSettings::setEngineLogFilePath(LNI_UTF8STRPTR_TO_STRING(filePath)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_Initialize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::initialize());
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_Finalize()
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::finalize());
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


LN_FLAT_API LNResult LNEngine_Run(LNHandle app)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Engine::run(LNI_HANDLE_TO_OBJECT(ln::Application, app)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNEngine_Time(double* outReturn)
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


LN_FLAT_API LNResult LNEngine_GetCamera(LNHandle* outReturn)
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


LN_FLAT_API LNResult LNApplication_OnSerialize_CallOverrideBase(LNHandle object, LNHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer2, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LNResult LNApplication_OnSerialize_SetOverrideCallback(LNApplication_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_Application::s_LNApplication_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
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
    return LN_SUCCESS;
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
    return LN_SUCCESS;
}

LN_FLAT_API LNResult LNApplication_SetPrototype_OnSerialize(LNHandle application, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnSerialize_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ApplicationSerializeHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNApplication_SetPrototype_OnInit(LNHandle application, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnInit_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ApplicationInitHandler, callback);
    return LN_SUCCESS;
}
LN_FLAT_API LNResult LNApplication_SetPrototype_OnUpdate(LNHandle application, LNHandle callback)
{
    LNI_HANDLE_TO_OBJECT(LNWS_ln_Application, application)->acquireOverridePrototypes()->OnUpdate_OverrideFunc = LNI_HANDLE_TO_OBJECT(LNWS_ApplicationUpdateHandler, callback);
    return LN_SUCCESS;
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

LN_FLAT_API LNResult LNDebug_Print(const LNChar* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(str));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LNResult LNDebug_PrintA(const char* str)
{
    LNI_FUNC_TRY_BEGIN;
    (ln::Debug::print(LNI_UTF8STRPTR_TO_STRING(str)));
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
    (ln::Debug::print(time, LNI_UTF8STRPTR_TO_STRING(str)));
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
    (ln::Debug::print(time, *reinterpret_cast<const ln::Color*>(color), LNI_UTF8STRPTR_TO_STRING(str)));
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

LN_FLAT_API void LNUITextBlockSerializeHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UITextBlockSerializeHandler>(), id);
}

void LNUITextBlockSerializeHandler_RegisterSubclassTypeInfo(const LNUITextBlockSerializeHandler_SubclassRegistrationInfo* info)
{
    if (info) {
        ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<UITextBlockSerializeHandler>(), info->subclassId);
        *LNWS_UITextBlockSerializeHandler::subclassInfo() = *info;
    }
}

LNSubinstanceId LNUITextBlockSerializeHandler_GetSubinstanceId(LNHandle handle)
{
    if (handle) {
        LNI_FUNC_TRY_BEGIN;
        return (LNI_HANDLE_TO_OBJECT(LNWS_UITextBlockSerializeHandler, handle))->m_subinstance;
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
