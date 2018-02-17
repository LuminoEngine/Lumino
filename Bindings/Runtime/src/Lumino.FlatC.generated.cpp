#include "Internal.h"
#include "Lumino.Wrapper.generated.h"

extern "C"
{

LN_API LNResultCode LNVector3_GetLength(const LNVector3* vector3, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const Vector3*>(vector3)->getLength());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNVector3_GetLengthSquared(const LNVector3* vector3, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const Vector3*>(vector3)->getLengthSquared());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNVector3_Normalize(const LNVector3* vec, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize(*reinterpret_cast<const Vector3*>(vec)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNVector3_NormalizeXYZ(float x, float y, float z, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize(x, y, z));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_Clear(LNRect* rect)
{
    LWIG_FUNC_TRY_BEGIN;
    new (reinterpret_cast<Rect*>(rect)) Rect();

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_Set(LNRect* rect, float x, float y, float width, float height)
{
    LWIG_FUNC_TRY_BEGIN;
    new (reinterpret_cast<Rect*>(rect)) Rect(x, y, width, height);

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_GetLeft(const LNRect* rect, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const Rect*>(rect)->getLeft());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_SetSize(LNRect* rect, const LNSize* size)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Rect*>(rect)->setSize(*reinterpret_cast<const Size*>(size)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_GetSize(const LNRect* rect, LNSize* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNSize&>(reinterpret_cast<const Rect*>(rect)->getSize());

    LWIG_FUNC_TRY_END_RETURN;
}



LN_API LNResultCode LNEngine_Initialize()
{
    LWIG_FUNC_TRY_BEGIN;
    LFManager::preInitialize();
(Engine::initialize());
LFManager::postInitialize();

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNEngine_Terminate()
{
    LWIG_FUNC_TRY_BEGIN;
    (Engine::terminate());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNEngine_Update(LNBool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_LNBOOL(Engine::update());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_Create(const LNChar* filePath, LNHandle* outSound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSound, LNSound, initialize, filePath);

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_SetVolume(LNHandle sound, float volume)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->setVolume(volume));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_GetVolume(LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSound, sound)->getVolume());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_SetPitch(LNHandle sound, float pitch)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->setPitch(pitch));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_GetPitch(LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSound, sound)->getPitch());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_SetLoopEnabled(LNHandle sound, LNBool enabled)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->setLoopEnabled(LWIG_TO_BOOL(enabled)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_IsLoopEnabled(LNHandle sound, LNBool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_LNBOOL(LWIG_TO_OBJECT(LNSound, sound)->isLoopEnabled());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_SetLoopRange(LNHandle sound, uint32_t begin, uint32_t length)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->SetLoopRange(begin, length));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_Play(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->play());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_Stop(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->stop());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_Pause(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->pause());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSound_Resume(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->Resume());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSound_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sound>()->setBindingTypeInfo(data);
}
LN_API void LNGraphicsResourceObject_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<GraphicsResourceObject>()->setBindingTypeInfo(data);
}
LN_API void LNTexture_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Texture>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNTexture2D_Create(int width, int height, LNTextureFormat format, LNBool mipmap, LNHandle* outTexture2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outTexture2D, LNTexture2D, initialize, width, height, static_cast<TextureFormat>(format), LWIG_TO_BOOL(mipmap));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNTexture2D_CreateFromFile(const LNChar* filePath, LNTextureFormat format, LNBool mipmap, LNHandle* outTexture2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outTexture2D, LNTexture2D, initialize, filePath, static_cast<TextureFormat>(format), LWIG_TO_BOOL(mipmap));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNTexture2D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Texture2D>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNWorldObject_SetPosition(LNHandle worldobject, const LNVector3* pos)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setPosition(*reinterpret_cast<const Vector3*>(pos)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetPositionXYZ(LNHandle worldobject, float x, float y, float z)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setPosition(x, y, z));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_GetPosition(LNHandle worldobject, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(LWIG_TO_OBJECT(LNWorldObject, worldobject)->getPosition());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetRotation(LNHandle worldobject, const LNQuaternion* rot)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setRotation(*reinterpret_cast<const Quaternion*>(rot)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetEulerAngles(LNHandle worldobject, float x, float y, float z)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setEulerAngles(x, y, z));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_GetRotation(LNHandle worldobject, LNQuaternion* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNQuaternion&>(LWIG_TO_OBJECT(LNWorldObject, worldobject)->getRotation());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetScale(LNHandle worldobject, const LNVector3* scale)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setScale(*reinterpret_cast<const Vector3*>(scale)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetScaleS(LNHandle worldobject, float xyz)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setScale(xyz));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetScaleXYZ(LNHandle worldobject, float x, float y, float z)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setScale(x, y, z));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_GetScale(LNHandle worldobject, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(LWIG_TO_OBJECT(LNWorldObject, worldobject)->getScale());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetTag(LNHandle worldobject, const LNChar* tag)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setTag(tag));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_GetTag(LNHandle worldobject, const LNChar** outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_STRING_TO_STRPTR(LWIG_TO_OBJECT(LNWorldObject, worldobject)->getTag());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetLayer(LNHandle worldobject, int layer)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setLayer(layer));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_GetLayer(LNHandle worldobject, int* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNWorldObject, worldobject)->getLayer());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNWorldObject_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<WorldObject>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNComponent_SetLayer(LNHandle component, int layer)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNComponent, component)->setLayer(layer));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNComponent_GetLayer(LNHandle component, int* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNComponent, component)->getLayer());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNComponent_SetOrderInLayer(LNHandle component, int16_t order)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNComponent, component)->setOrderInLayer(order));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNComponent_GetOrderInLayer(LNHandle component, int* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNComponent, component)->getOrderInLayer());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNComponent_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Component>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSceneNode_SetVisible(LNHandle scenenode, LNBool visible)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSceneNode, scenenode)->setVisible(LWIG_TO_BOOL(visible)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSceneNode_IsVisible(LNHandle scenenode, LNBool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_LNBOOL(LWIG_TO_OBJECT(LNSceneNode, scenenode)->isVisible());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSceneNode_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<SceneNode>()->setBindingTypeInfo(data);
}
LN_API void LNVisualComponent_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<VisualComponent>()->setBindingTypeInfo(data);
}
LN_API void LNVisualObject_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<VisualObject>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSpriteComponent_SetTexture(LNHandle spritecomponent, LNHandle texture)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSpriteComponent, spritecomponent)->setTexture(LWIG_TO_OBJECT(Texture, texture)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteComponent_GetTexture(LNHandle spritecomponent, LNHandle* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_HANDLE(LWIG_TO_OBJECT(LNSpriteComponent, spritecomponent)->getTexture());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSpriteComponent_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<SpriteComponent>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSprite2DComponent_Create(LNHandle* outSprite2DComponent)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite2DComponent, LNSprite2DComponent, initialize, );

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSprite2DComponent_CreateFromTexture(LNHandle texture, LNHandle* outSprite2DComponent)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite2DComponent, LNSprite2DComponent, initialize, LWIG_TO_OBJECT(Texture, texture));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSprite2DComponent_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sprite2DComponent>()->setBindingTypeInfo(data);
}
LN_API void LNSprite3DComponent_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sprite3DComponent>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSpriteBase_SetTexture(LNHandle spritebase, LNHandle texture)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSpriteBase, spritebase)->setTexture(LWIG_TO_OBJECT(Texture, texture)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_GetTexture(LNHandle spritebase, LNHandle* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_HANDLE(LWIG_TO_OBJECT(LNSpriteBase, spritebase)->getTexture());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_SetSourceRect(LNHandle spritebase, const LNRect* rect)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSpriteBase, spritebase)->setSourceRect(*reinterpret_cast<const Rect*>(rect)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_SetSourceRectXYWH(LNHandle spritebase, float x, float y, float width, float height)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSpriteBase, spritebase)->setSourceRect(x, y, width, height));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_GetSourceRect(LNHandle spritebase, LNRect* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNRect&>(LWIG_TO_OBJECT(LNSpriteBase, spritebase)->getSourceRect());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_SetAnchorPoint(LNHandle spritebase, const LNVector2* ratio)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSpriteBase, spritebase)->setAnchorPoint(*reinterpret_cast<const Vector2*>(ratio)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_SetAnchorPointXY(LNHandle spritebase, float ratioX, float ratioY)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSpriteBase, spritebase)->setAnchorPoint(ratioX, ratioY));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSpriteBase_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<SpriteBase>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSprite2D_Create(LNHandle* outSprite2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite2D, LNSprite2D, initialize, );

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSprite2D_CreateFromFile(const LNChar* filePath, LNHandle* outSprite2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite2D, LNSprite2D, initialize, filePath);

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSprite2D_CreateFromTexture(LNHandle texture, LNHandle* outSprite2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite2D, LNSprite2D, initialize, LWIG_TO_OBJECT(Texture, texture));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSprite2D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sprite2D>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSprite3D_Create(LNHandle* outSprite3D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite3D, LNSprite3D, initialize, );

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSprite3D_CreateFromTexture(float width, float height, LNHandle texture, LNHandle* outSprite3D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite3D, LNSprite3D, initialize, width, height, LWIG_TO_OBJECT(Texture, texture));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSprite3D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sprite3D>()->setBindingTypeInfo(data);
}

	
} // extern "C"
