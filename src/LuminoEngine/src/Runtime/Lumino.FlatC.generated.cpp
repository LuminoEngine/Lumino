#include <LuminoEngine/Runtime/Runtime.hpp>
#include <LuminoEngine/Runtime/Lumino.FlatC.generated.h>

#include <LuminoEngine.hpp>

class LNWS_ln_Engine : public ln::Engine
{
public:


};


class LNWS_ln_GraphicsResource : public ln::GraphicsResource
{
public:


};


class LNWS_ln_Texture : public ln::Texture
{
public:


};


class LNWS_ln_Texture2D : public ln::Texture2D
{
public:


};


class LNWS_ln_Component : public ln::Component
{
public:


};


class LNWS_ln_VisualComponent : public ln::VisualComponent
{
public:


};


class LNWS_ln_SpriteComponent : public ln::SpriteComponent
{
public:


};


class LNWS_ln_ComponentList : public ln::ComponentList
{
public:


};


class LNWS_ln_WorldObject : public ln::WorldObject
{
public:
    static LnWorldObject_OnUpdate_OverrideCallback s_LnWorldObject_OnUpdate_OverrideCallback;
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (s_LnWorldObject_OnUpdate_OverrideCallback) s_LnWorldObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::WorldObject::onUpdate(elapsedSeconds);
    }
};
LnWorldObject_OnUpdate_OverrideCallback LNWS_ln_WorldObject::s_LnWorldObject_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_VisualObject : public ln::VisualObject
{
public:
    static LnVisualObject_OnUpdate_OverrideCallback s_LnVisualObject_OnUpdate_OverrideCallback;
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (s_LnVisualObject_OnUpdate_OverrideCallback) s_LnVisualObject_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::VisualObject::onUpdate(elapsedSeconds);
    }
};
LnVisualObject_OnUpdate_OverrideCallback LNWS_ln_VisualObject::s_LnVisualObject_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_Sprite : public ln::Sprite
{
public:
    static LnSprite_OnUpdate_OverrideCallback s_LnSprite_OnUpdate_OverrideCallback;
    virtual void onUpdate(float elapsedSeconds) override
    {
        if (s_LnSprite_OnUpdate_OverrideCallback) s_LnSprite_OnUpdate_OverrideCallback(LNI_OBJECT_TO_HANDLE(this), elapsedSeconds);
    }
    void onUpdate_CallBase(float elapsedSeconds)
    {
        ln::Sprite::onUpdate(elapsedSeconds);
    }
};
LnSprite_OnUpdate_OverrideCallback LNWS_ln_Sprite::s_LnSprite_OnUpdate_OverrideCallback = nullptr;

class LNWS_ln_UIEventArgs : public ln::UIEventArgs
{
public:


};


class LNWS_ln_UILayoutElement : public ln::UILayoutElement
{
public:


};


class LNWS_ln_UIElement : public ln::UIElement
{
public:


};


class LNWS_ln_UIControl : public ln::UIControl
{
public:


};


class LNWS_ln_UIButtonBase : public ln::UIButtonBase
{
public:


};


class LNWS_ln_UIButton : public ln::UIButton
{
public:


};




extern "C"
{

LN_FLAT_API LnResult LnVector3_Length(const LnVector3* vector3, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const ln::Vector3*>(vector3)->length());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVector3_LengthSquared(const LnVector3* vector3, float* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const ln::Vector3*>(vector3)->lengthSquared());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVector3_NormalizeXYZ(float x, float y, float z, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnVector3&>(ln::Vector3::normalize(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVector3_Normalize(const LnVector3* vec, LnVector3* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnVector3&>(ln::Vector3::normalize(*reinterpret_cast<const ln::Vector3*>(vec)));
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
    *outReturn = LNI_BOOL_TO_LNBOOL(ln::Engine::update());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnEngine_MainUIView(LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_OBJECT_TO_HANDLE(ln::Engine::mainUIView());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API void LnGraphicsResource_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::GraphicsResource>(), id);
}

LN_FLAT_API void LnTexture_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture>(), id);
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
    LnResult result = LnTexture2D_CreateFromFile(ln::String::fromCString(filePath, -1, ln::TextEncoding::utf8Encoding()).c_str(), format, outTexture2D);
    return result;
}

LN_FLAT_API void LnTexture2D_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Texture2D>(), id);
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
    *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualComponent, visualcomponent)->isVisible());
    LNI_FUNC_TRY_END_RETURN;
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

LN_FLAT_API void LnSpriteComponent_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::SpriteComponent>(), id);
}

LN_FLAT_API LnResult LnComponentList_GetLength(LnHandle componentlist, int* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = (LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->getLength());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnComponentList_GetItem(LnHandle componentlist, int index, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_ComponentList, componentlist)->getItem(index));
    LNI_FUNC_TRY_END_RETURN;
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
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->position());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_SetRotation(LnHandle worldobject, const LnQuaternion* rot)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setRotation(*reinterpret_cast<const ln::Quaternion*>(rot)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_SetEulerAngles(LnHandle worldobject, float x, float y, float z)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->setEulerAngles(x, y, z));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_GetRotation(LnHandle worldobject, LnQuaternion* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LnQuaternion&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->rotation());
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
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->scale());
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
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->centerPoint());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnWorldObject_GetComponents(LnHandle worldobject, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_WorldObject, worldobject)->components());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API void LnWorldObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::WorldObject>(), id);
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
LN_FLAT_API LnResult LnVisualObject_SetVisible(LnHandle visualobject, LnBool value)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->setVisible(LNI_LNBOOL_TO_BOOL(value)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnVisualObject_IsVisible(LnHandle visualobject, LnBool* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_BOOL_TO_LNBOOL(LNI_HANDLE_TO_OBJECT(LNWS_ln_VisualObject, visualobject)->isVisible());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API void LnVisualObject_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::VisualObject>(), id);
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
LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle texture)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setTexture(LNI_HANDLE_TO_OBJECT(ln::Texture, texture)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnSprite_SetSourceRectXYWH(LnHandle sprite, float x, float y, float width, float height)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_Sprite, sprite)->setSourceRect(x, y, width, height));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnSprite_Create(LnHandle texture, float width, float height, LnHandle* outSprite)
{
    LNI_FUNC_TRY_BEGIN;
    LNI_CREATE_OBJECT(outSprite, LNWS_ln_Sprite, init, LNI_HANDLE_TO_OBJECT(ln::Texture, texture), width, height);
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API void LnSprite_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::Sprite>(), id);
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
LN_FLAT_API LnResult LnUIEventArgs_Sender(LnHandle uieventargs, LnHandle* outReturn)
{
    LNI_FUNC_TRY_BEGIN;
    *outReturn = LNI_OBJECT_TO_HANDLE(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIEventArgs, uieventargs)->sender());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API void LnUIEventArgs_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIEventArgs>(), id);
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
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->position());
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
    *outReturn = reinterpret_cast<const LnQuaternion&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->rotation());
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
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->scale());
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
    *outReturn = reinterpret_cast<const LnVector3&>(LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->centerPoint());
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API LnResult LnUIElement_AddChild(LnHandle uielement, LnHandle child)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIElement, uielement)->addChild(LNI_HANDLE_TO_OBJECT(ln::UIElement, child)));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API void LnUIElement_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIElement>(), id);
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
    LnResult result = LnUIButtonBase_SetText(uibuttonbase, ln::String::fromCString(text, -1, ln::TextEncoding::utf8Encoding()).c_str());
    return result;
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

LN_FLAT_API LnResult LnUIButton_ConnectOnClicked(LnHandle uibutton, LnUIEventHandlerCallback handler)
{
    LNI_FUNC_TRY_BEGIN;
    (LNI_HANDLE_TO_OBJECT(LNWS_ln_UIButton, uibutton)->connectOnClicked([=](ln::UIEventArgs* e) { handler(uibutton, LNI_OBJECT_TO_HANDLE(e)); }));
    LNI_FUNC_TRY_END_RETURN;
}

LN_FLAT_API void LnUIButton_SetManagedTypeInfoId(int64_t id)
{
    ::ln::detail::TypeInfoInternal::setManagedTypeInfoId(::ln::TypeInfo::getTypeInfo<ln::UIButton>(), id);
}


	
} // extern "C"
