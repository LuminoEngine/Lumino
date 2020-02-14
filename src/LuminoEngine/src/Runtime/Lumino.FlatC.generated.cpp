#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>

#include <LuminoEngine.hpp>
#include "BindingValidation.hpp"

class LNWS_ln_PromiseFailureDelegate : public ln::PromiseFailureDelegate
{
public:
    LnPromiseFailureDelegateCallback m_callback;

    LNWS_ln_PromiseFailureDelegate() : ln::PromiseFailureDelegate([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_SUCCESS) { LN_ERROR("LnPromiseFailureDelegateCallback"); }
    })
    {}

    void init(LnPromiseFailureDelegateCallback callback)
    {
        ln::PromiseFailureDelegate::init();
        m_callback = callback;
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
    LnZVTestDelegate1Callback m_callback;

    LNWS_ln_ZVTestDelegate1() : ln::ZVTestDelegate1([this](int p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1);
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestDelegate1Callback"); }
    })
    {}

    void init(LnZVTestDelegate1Callback callback)
    {
        ln::ZVTestDelegate1::init();
        m_callback = callback;
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
    LnZVTestDelegate2Callback m_callback;

    LNWS_ln_ZVTestDelegate2() : ln::ZVTestDelegate2([this](int p1, int p2) -> int
    {
        int ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1, p2, &ret);
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestDelegate2Callback"); }
        return ret;
    })
    {}

    void init(LnZVTestDelegate2Callback callback)
    {
        ln::ZVTestDelegate2::init();
        m_callback = callback;
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
    LnZVTestDelegate3Callback m_callback;

    LNWS_ln_ZVTestDelegate3() : ln::ZVTestDelegate3([this](ln::ZVTestClass1* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(p1));
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestDelegate3Callback"); }
    })
    {}

    void init(LnZVTestDelegate3Callback callback)
    {
        ln::ZVTestDelegate3::init();
        m_callback = callback;
    }
};

LN_FLAT_API LnResult LnZVTestDelegate3_Create(LnZVTestDelegate3Callback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate3, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}
class LNWS_ln_ZVTestDelegate4 : public ln::ZVTestDelegate4
{
public:
    LnZVTestDelegate4Callback m_callback;

    LNWS_ln_ZVTestDelegate4() : ln::ZVTestDelegate4([this]() -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this));
        if (r != LN_SUCCESS) { LN_ERROR("LnZVTestDelegate4Callback"); }
    })
    {}

    void init(LnZVTestDelegate4Callback callback)
    {
        ln::ZVTestDelegate4::init();
        m_callback = callback;
    }
};

LN_FLAT_API LnResult LnZVTestDelegate4_Create(LnZVTestDelegate4Callback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_ZVTestDelegate4, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}
class LNWS_ln_TestDelegate : public ln::TestDelegate
{
public:
    LnTestDelegateCallback m_callback;

    LNWS_ln_TestDelegate() : ln::TestDelegate([this](int p1) -> int
    {
        int ret = {};
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), p1, &ret);
        if (r != LN_SUCCESS) { LN_ERROR("LnTestDelegateCallback"); }
        return ret;
    })
    {}

    void init(LnTestDelegateCallback callback)
    {
        ln::TestDelegate::init();
        m_callback = callback;
    }
};

LN_FLAT_API LnResult LnTestDelegate_Create(LnTestDelegateCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_TestDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}
class LNWS_ln_UIEventHandler : public ln::UIEventHandler
{
public:
    LnUIEventHandlerCallback m_callback;

    LNWS_ln_UIEventHandler() : ln::UIEventHandler([this](ln::UIEventArgs* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1));
        if (r != LN_SUCCESS) { LN_ERROR("LnUIEventHandlerCallback"); }
    })
    {}

    void init(LnUIEventHandlerCallback callback)
    {
        ln::UIEventHandler::init();
        m_callback = callback;
    }
};

LN_FLAT_API LnResult LnUIEventHandler_Create(LnUIEventHandlerCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_UIEventHandler, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}
class LNWS_ln_UIEventHandlerDelegate : public ln::UIEventHandlerDelegate
{
public:
    LnUIEventHandlerDelegateCallback m_callback;

    LNWS_ln_UIEventHandlerDelegate() : ln::UIEventHandlerDelegate([this](ln::UIEventArgs* p1) -> void
    {
        auto r = m_callback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(p1));
        if (r != LN_SUCCESS) { LN_ERROR("LnUIEventHandlerDelegateCallback"); }
    })
    {}

    void init(LnUIEventHandlerDelegateCallback callback)
    {
        ln::UIEventHandlerDelegate::init();
        m_callback = callback;
    }
};

LN_FLAT_API LnResult LnUIEventHandlerDelegate_Create(LnUIEventHandlerDelegateCallback callback, LnHandle* outDelegate)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outDelegate, LNWS_ln_UIEventHandlerDelegate, init, callback);
    LNI_FUNC_TRY_END_RETURN;
}

class LNWS_ln_Object : public ln::Object
{
public:
    static LnObject_OnSerialize_OverrideCallback s_LnObject_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnObject_OnSerialize_OverrideCallback) s_LnObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Object::onSerialize(ar);
    }

};
LnObject_OnSerialize_OverrideCallback LNWS_ln_Object::s_LnObject_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_EventConnection : public ln::EventConnection
{
public:
    static LnEventConnection_OnSerialize_OverrideCallback s_LnEventConnection_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnEventConnection_OnSerialize_OverrideCallback) s_LnEventConnection_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::EventConnection::onSerialize(ar);
    }

};
LnEventConnection_OnSerialize_OverrideCallback LNWS_ln_EventConnection::s_LnEventConnection_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ZVTestPromise1 : public ln::ZVTestPromise1
{
public:
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
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
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
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
    static LnZVTestClass1_OnSerialize_OverrideCallback s_LnZVTestClass1_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnZVTestClass1_OnSerialize_OverrideCallback) s_LnZVTestClass1_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::ZVTestClass1::onSerialize(ar);
    }

};
LnZVTestClass1_OnSerialize_OverrideCallback LNWS_ln_ZVTestClass1::s_LnZVTestClass1_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Serializer : public ln::Serializer
{
public:
    static LnSerializer_OnSerialize_OverrideCallback s_LnSerializer_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnSerializer_OnSerialize_OverrideCallback) s_LnSerializer_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Serializer::onSerialize(ar);
    }

};
LnSerializer_OnSerialize_OverrideCallback LNWS_ln_Serializer::s_LnSerializer_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_AssetModel : public ln::AssetModel
{
public:
    static LnAssetModel_OnSerialize_OverrideCallback s_LnAssetModel_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnAssetModel_OnSerialize_OverrideCallback) s_LnAssetModel_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::AssetModel::onSerialize(ar);
    }

};
LnAssetModel_OnSerialize_OverrideCallback LNWS_ln_AssetModel::s_LnAssetModel_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Assets : public ln::Assets
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
    static LnApplication_OnSerialize_OverrideCallback s_LnApplication_OnSerialize_OverrideCallback;
    static LnApplication_OnInit_OverrideCallback s_LnApplication_OnInit_OverrideCallback;
    static LnApplication_OnUpdate_OverrideCallback s_LnApplication_OnUpdate_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnApplication_OnSerialize_OverrideCallback) s_LnApplication_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Application::onSerialize(ar);
    }
    virtual void onInit() override
    {
        if (s_LnApplication_OnInit_OverrideCallback) s_LnApplication_OnInit_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
    }

    void onInit_CallBase()
    {
        ln::Application::onInit();
    }
    virtual void onUpdate() override
    {
        if (s_LnApplication_OnUpdate_OverrideCallback) s_LnApplication_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this));
    }

    void onUpdate_CallBase()
    {
        ln::Application::onUpdate();
    }

};
LnApplication_OnSerialize_OverrideCallback LNWS_ln_Application::s_LnApplication_OnSerialize_OverrideCallback = nullptr;
LnApplication_OnInit_OverrideCallback LNWS_ln_Application::s_LnApplication_OnInit_OverrideCallback = nullptr;
LnApplication_OnUpdate_OverrideCallback LNWS_ln_Application::s_LnApplication_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_GraphicsResource : public ln::GraphicsResource
{
public:
    static LnGraphicsResource_OnSerialize_OverrideCallback s_LnGraphicsResource_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnGraphicsResource_OnSerialize_OverrideCallback) s_LnGraphicsResource_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::GraphicsResource::onSerialize(ar);
    }

};
LnGraphicsResource_OnSerialize_OverrideCallback LNWS_ln_GraphicsResource::s_LnGraphicsResource_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Texture : public ln::Texture
{
public:
    static LnTexture_OnSerialize_OverrideCallback s_LnTexture_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<GraphicsResource>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnTexture_OnSerialize_OverrideCallback) s_LnTexture_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Texture::onSerialize(ar);
    }

};
LnTexture_OnSerialize_OverrideCallback LNWS_ln_Texture::s_LnTexture_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Texture2D : public ln::Texture2D
{
public:
    static LnTexture2D_OnSerialize_OverrideCallback s_LnTexture2D_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnTexture2D_OnSerialize_OverrideCallback) s_LnTexture2D_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Texture2D::onSerialize(ar);
    }

};
LnTexture2D_OnSerialize_OverrideCallback LNWS_ln_Texture2D::s_LnTexture2D_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_Component : public ln::Component
{
public:
    static LnComponent_OnSerialize_OverrideCallback s_LnComponent_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnComponent_OnSerialize_OverrideCallback) s_LnComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Component::onSerialize(ar);
    }

};
LnComponent_OnSerialize_OverrideCallback LNWS_ln_Component::s_LnComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_VisualComponent : public ln::VisualComponent
{
public:
    static LnVisualComponent_OnSerialize_OverrideCallback s_LnVisualComponent_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnVisualComponent_OnSerialize_OverrideCallback) s_LnVisualComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::VisualComponent::onSerialize(ar);
    }

};
LnVisualComponent_OnSerialize_OverrideCallback LNWS_ln_VisualComponent::s_LnVisualComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_SpriteComponent : public ln::SpriteComponent
{
public:
    static LnSpriteComponent_OnSerialize_OverrideCallback s_LnSpriteComponent_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnSpriteComponent_OnSerialize_OverrideCallback) s_LnSpriteComponent_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::SpriteComponent::onSerialize(ar);
    }

};
LnSpriteComponent_OnSerialize_OverrideCallback LNWS_ln_SpriteComponent::s_LnSpriteComponent_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_World : public ln::World
{
public:
    static LnWorld_OnSerialize_OverrideCallback s_LnWorld_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnWorld_OnSerialize_OverrideCallback) s_LnWorld_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::World::onSerialize(ar);
    }

};
LnWorld_OnSerialize_OverrideCallback LNWS_ln_World::s_LnWorld_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_ComponentList : public ln::ComponentList
{
public:
    static LnComponentList_OnSerialize_OverrideCallback s_LnComponentList_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnComponentList_OnSerialize_OverrideCallback) s_LnComponentList_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::ComponentList::onSerialize(ar);
    }

};
LnComponentList_OnSerialize_OverrideCallback LNWS_ln_ComponentList::s_LnComponentList_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_WorldObject : public ln::WorldObject
{
public:
    static LnWorldObject_OnSerialize_OverrideCallback s_LnWorldObject_OnSerialize_OverrideCallback;
    static LnWorldObject_OnUpdate_OverrideCallback s_LnWorldObject_OnUpdate_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnWorldObject_OnSerialize_OverrideCallback) s_LnWorldObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::WorldObject::onSerialize(ar);
    }
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (s_LnWorldObject_OnUpdate_OverrideCallback) s_LnWorldObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }

    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::WorldObject::onUpdate(elapsedSeconds);
    }

};
LnWorldObject_OnSerialize_OverrideCallback LNWS_ln_WorldObject::s_LnWorldObject_OnSerialize_OverrideCallback = nullptr;
LnWorldObject_OnUpdate_OverrideCallback LNWS_ln_WorldObject::s_LnWorldObject_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_VisualObject : public ln::VisualObject
{
public:
    static LnVisualObject_OnSerialize_OverrideCallback s_LnVisualObject_OnSerialize_OverrideCallback;
    static LnVisualObject_OnUpdate_OverrideCallback s_LnVisualObject_OnUpdate_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnVisualObject_OnSerialize_OverrideCallback) s_LnVisualObject_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::VisualObject::onSerialize(ar);
    }
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (s_LnVisualObject_OnUpdate_OverrideCallback) s_LnVisualObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }

    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::VisualObject::onUpdate(elapsedSeconds);
    }

};
LnVisualObject_OnSerialize_OverrideCallback LNWS_ln_VisualObject::s_LnVisualObject_OnSerialize_OverrideCallback = nullptr;
LnVisualObject_OnUpdate_OverrideCallback LNWS_ln_VisualObject::s_LnVisualObject_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_Sprite : public ln::Sprite
{
public:
    static LnSprite_OnSerialize_OverrideCallback s_LnSprite_OnSerialize_OverrideCallback;
    static LnSprite_OnUpdate_OverrideCallback s_LnSprite_OnUpdate_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnSprite_OnSerialize_OverrideCallback) s_LnSprite_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::Sprite::onSerialize(ar);
    }
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (s_LnSprite_OnUpdate_OverrideCallback) s_LnSprite_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }

    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::Sprite::onUpdate(elapsedSeconds);
    }

};
LnSprite_OnSerialize_OverrideCallback LNWS_ln_Sprite::s_LnSprite_OnSerialize_OverrideCallback = nullptr;
LnSprite_OnUpdate_OverrideCallback LNWS_ln_Sprite::s_LnSprite_OnUpdate_OverrideCallback = nullptr;


class LNWS_ln_UIEventArgs : public ln::UIEventArgs
{
public:
    static LnUIEventArgs_OnSerialize_OverrideCallback s_LnUIEventArgs_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnUIEventArgs_OnSerialize_OverrideCallback) s_LnUIEventArgs_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UIEventArgs::onSerialize(ar);
    }

};
LnUIEventArgs_OnSerialize_OverrideCallback LNWS_ln_UIEventArgs::s_LnUIEventArgs_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UILayoutElement : public ln::UILayoutElement
{
public:
    static LnUILayoutElement_OnSerialize_OverrideCallback s_LnUILayoutElement_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
    virtual void setTypeInfoOverride(ln::TypeInfo* value) override
    {
        m_typeInfoOverride = value;
    }
    virtual ::ln::TypeInfo* _lnref_getThisTypeInfo() const override
    {
        if (m_typeInfoOverride)
            return m_typeInfoOverride;
        else
            return ln::TypeInfo::getTypeInfo<Object>();
    }


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnUILayoutElement_OnSerialize_OverrideCallback) s_LnUILayoutElement_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UILayoutElement::onSerialize(ar);
    }

};
LnUILayoutElement_OnSerialize_OverrideCallback LNWS_ln_UILayoutElement::s_LnUILayoutElement_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIElement : public ln::UIElement
{
public:
    static LnUIElement_OnSerialize_OverrideCallback s_LnUIElement_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnUIElement_OnSerialize_OverrideCallback) s_LnUIElement_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UIElement::onSerialize(ar);
    }

};
LnUIElement_OnSerialize_OverrideCallback LNWS_ln_UIElement::s_LnUIElement_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIControl : public ln::UIControl
{
public:
    static LnUIControl_OnSerialize_OverrideCallback s_LnUIControl_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnUIControl_OnSerialize_OverrideCallback) s_LnUIControl_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UIControl::onSerialize(ar);
    }

};
LnUIControl_OnSerialize_OverrideCallback LNWS_ln_UIControl::s_LnUIControl_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIButtonBase : public ln::UIButtonBase
{
public:
    static LnUIButtonBase_OnSerialize_OverrideCallback s_LnUIButtonBase_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnUIButtonBase_OnSerialize_OverrideCallback) s_LnUIButtonBase_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UIButtonBase::onSerialize(ar);
    }

};
LnUIButtonBase_OnSerialize_OverrideCallback LNWS_ln_UIButtonBase::s_LnUIButtonBase_OnSerialize_OverrideCallback = nullptr;


class LNWS_ln_UIButton : public ln::UIButton
{
public:
    static LnUIButton_OnSerialize_OverrideCallback s_LnUIButton_OnSerialize_OverrideCallback;
    ln::TypeInfo* m_typeInfoOverride = nullptr;
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


    virtual void onSerialize(ln::Serializer* ar) override
    {
        if (s_LnUIButton_OnSerialize_OverrideCallback) s_LnUIButton_OnSerialize_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), LNI_OBJECT_TO_HANDLE(ar));
    }

    void onSerialize_CallBase(ln::Serializer* ar)
    {
        ln::UIButton::onSerialize(ar);
    }

};
LnUIButton_OnSerialize_OverrideCallback LNWS_ln_UIButton::s_LnUIButton_OnSerialize_OverrideCallback = nullptr;




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
        *outReturn = reinterpret_cast<const LnVector3&>(ln::Vector3::normalize(x, y, z));
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
        *outReturn = reinterpret_cast<const LnVector3&>(ln::Vector3::normalize(*reinterpret_cast<const ln::Vector3*>(vec)));
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

extern LN_FLAT_API int LnObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Object>()->id();
}

LN_FLAT_API void LnObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Object>(), id);
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

extern LN_FLAT_API int LnEventConnection_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::EventConnection>()->id();
}

LN_FLAT_API void LnEventConnection_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::EventConnection>(), id);
}

LN_FLAT_API void LnPromiseFailureDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::PromiseFailureDelegate>(), id);
}

LN_FLAT_API void LnZVTestDelegate1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate1>(), id);
}

LN_FLAT_API void LnZVTestDelegate2_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate2>(), id);
}

LN_FLAT_API void LnZVTestDelegate3_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate3>(), id);
}

LN_FLAT_API void LnZVTestDelegate4_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestDelegate4>(), id);
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
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->connectOnEvent1(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate4, handler)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->connectOnEvent1(LNI_HANDLE_TO_OBJECT(ln::ZVTestDelegate4, handler)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnZVTestClass1_RaiseEvent1(LnHandle zvtestclass1)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_ZVTestClass1, zvtestclass1)->raiseEvent1());
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

extern LN_FLAT_API int LnZVTestClass1_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>()->id();
}

LN_FLAT_API void LnZVTestClass1_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ZVTestClass1>(), id);
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

extern LN_FLAT_API int LnSerializer_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Serializer>()->id();
}

LN_FLAT_API void LnSerializer_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Serializer>(), id);
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

extern LN_FLAT_API int LnAssetModel_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::AssetModel>()->id();
}

LN_FLAT_API void LnAssetModel_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::AssetModel>(), id);
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

extern LN_FLAT_API int LnApplication_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Application>()->id();
}

LN_FLAT_API void LnApplication_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Application>(), id);
}

LN_FLAT_API LnResult LnGraphicsResource_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_GraphicsResource, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnGraphicsResource_OnSerialize_SetOverrideCallback(LnGraphicsResource_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_GraphicsResource::s_LnGraphicsResource_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnGraphicsResource_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::GraphicsResource>()->id();
}

LN_FLAT_API void LnGraphicsResource_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::GraphicsResource>(), id);
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

extern LN_FLAT_API int LnTexture_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture>()->id();
}

LN_FLAT_API void LnTexture_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture>(), id);
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


LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, filePath, static_cast<ln::TextureFormat>(format));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnTexture2D_CreateFromFileA(const char* filePath, LnTextureFormat format, LnHandle* outTexture2D)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outTexture2D, LNWS_ln_Texture2D, init, LNI_UTF8STRPTR_TO_STRING(filePath), static_cast<ln::TextureFormat>(format));
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

extern LN_FLAT_API int LnTexture2D_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Texture2D>()->id();
}

LN_FLAT_API void LnTexture2D_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2D>(), id);
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

extern LN_FLAT_API int LnComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Component>()->id();
}

LN_FLAT_API void LnComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Component>(), id);
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

extern LN_FLAT_API int LnVisualComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::VisualComponent>()->id();
}

LN_FLAT_API void LnVisualComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualComponent>(), id);
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

extern LN_FLAT_API int LnSpriteComponent_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::SpriteComponent>()->id();
}

LN_FLAT_API void LnSpriteComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpriteComponent>(), id);
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

extern LN_FLAT_API int LnWorld_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::World>()->id();
}

LN_FLAT_API void LnWorld_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::World>(), id);
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

extern LN_FLAT_API int LnComponentList_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::ComponentList>()->id();
}

LN_FLAT_API void LnComponentList_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::ComponentList>(), id);
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
        *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->position());
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
        *outReturn = reinterpret_cast<const LnQuaternion&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->rotation());
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
        *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->scale());
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
        *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->centerPoint());
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

extern LN_FLAT_API int LnWorldObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::WorldObject>()->id();
}

LN_FLAT_API void LnWorldObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldObject>(), id);
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

extern LN_FLAT_API int LnVisualObject_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::VisualObject>()->id();
}

LN_FLAT_API void LnVisualObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualObject>(), id);
}

LN_FLAT_API void LnTestDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::TestDelegate>(), id);
}

LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, value)));
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

extern LN_FLAT_API int LnSprite_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::Sprite>()->id();
}

LN_FLAT_API void LnSprite_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sprite>(), id);
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

extern LN_FLAT_API int LnUIEventArgs_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIEventArgs>()->id();
}

LN_FLAT_API void LnUIEventArgs_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventArgs>(), id);
}

LN_FLAT_API void LnUIEventHandler_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventHandler>(), id);
}

LN_FLAT_API void LnUIEventHandlerDelegate_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventHandlerDelegate>(), id);
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

extern LN_FLAT_API int LnUILayoutElement_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UILayoutElement>()->id();
}

LN_FLAT_API void LnUILayoutElement_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UILayoutElement>(), id);
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
        *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->position());
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
        *outReturn = reinterpret_cast<const LnQuaternion&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->rotation());
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
        *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->scale());
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
        *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->centerPoint());
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

extern LN_FLAT_API int LnUIElement_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIElement>()->id();
}

LN_FLAT_API void LnUIElement_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIElement>(), id);
}

LN_FLAT_API LnResult LnUIControl_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIControl, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUIControl_OnSerialize_SetOverrideCallback(LnUIControl_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIControl::s_LnUIControl_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUIControl_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIControl>()->id();
}

LN_FLAT_API void LnUIControl_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIControl>(), id);
}

LN_FLAT_API LnResult LnUIButtonBase_SetText(LnHandle uibuttonbase, const LnChar* text)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, uibuttonbase)->setText(text));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIButtonBase_SetTextA(LnHandle uibuttonbase, const char* text)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, uibuttonbase)->setText(LNI_UTF8STRPTR_TO_STRING(text)));
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIButtonBase_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButtonBase, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUIButtonBase_OnSerialize_SetOverrideCallback(LnUIButtonBase_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIButtonBase::s_LnUIButtonBase_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUIButtonBase_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIButtonBase>()->id();
}

LN_FLAT_API void LnUIButtonBase_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIButtonBase>(), id);
}

LN_FLAT_API LnResult LnUIButton_Create(LnHandle* outUIButton)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outUIButton, LNWS_ln_UIButton, init, );
    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIButton_ConnectOnClicked(LnHandle uibutton, LnHandle handler, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    if (outReturn) {
        *outReturn = LNI_OBJECT_TO_HANDLE_FROM_STRONG_REFERENCE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, uibutton)->connectOnClicked(LNI_HANDLE_TO_OBJECT(ln::UIEventHandlerDelegate, handler)));
    }
    else {
        (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, uibutton)->connectOnClicked(LNI_HANDLE_TO_OBJECT(ln::UIEventHandlerDelegate, handler)));
    }

    LNI_FUNC_TRY_END_RETURN;
}


LN_FLAT_API LnResult LnUIButton_OnSerialize_CallOverrideBase(LnHandle object, LnHandle ar)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, object)->onSerialize_CallBase(LNI_HANDLE_TO_OBJECT(ln::Serializer, ar)));
    LNI_FUNC_TRY_END_RETURN;
}
LN_FLAT_API LnResult LnUIButton_OnSerialize_SetOverrideCallback(LnUIButton_OnSerialize_OverrideCallback callback)
{
    LNWS_ln_UIButton::s_LnUIButton_OnSerialize_OverrideCallback = callback;
    return LN_SUCCESS;
}

extern LN_FLAT_API int LnUIButton_GetTypeInfoId()
{
    return ln::TypeInfo::getTypeInfo<ln::UIButton>()->id();
}

LN_FLAT_API void LnUIButton_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIButton>(), id);
}


	
} // extern "C"
