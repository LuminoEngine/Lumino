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

LN_API LNResultCode LNVector3_Normalize(float x, float y, float z, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize(x, y, z));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNVector3_Normalize(const LNVector3* vec, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize(*reinterpret_cast<Vector3*>(vec)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_Rect(LNRect* rect)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Rect*>(rect)->Rect());

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_Rect(LNRect* rect, float x, float y, float width, float height)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Rect*>(rect)->Rect(x, y, width, height));

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
    (reinterpret_cast<Rect*>(rect)->setSize(*reinterpret_cast<Size*>(size)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNRect_GetSize(const LNRect* rect, LNSize* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNSize&>(reinterpret_cast<const Rect*>(rect)->getSize());

    LWIG_FUNC_TRY_END_RETURN;
}



LN_API LNResultCode LNEngine_Create(LNHandle* outEngine)
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

LN_API LNResultCode LNEngine_Update(LNBool outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (Engine::update());

    LWIG_FUNC_TRY_END_RETURN;
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
    LWIG_CREATE_OBJECT(outTexture2D, LNTexture2D, initialize, width, height, static_cast<TextureFormat>(format), mipmap);

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNTexture2D_CreateFromFile(const LNChar* filePath, LNTextureFormat format, LNBool mipmap, LNHandle* outTexture2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outTexture2D, LNTexture2D, initialize, filePath, static_cast<TextureFormat>(format), mipmap);

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNTexture2D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Texture2D>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNWorldObject_SetPosition(LNHandle worldobject, const LNVector3* pos)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setPosition(*reinterpret_cast<Vector3*>(pos)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetPosition(LNHandle worldobject, float x, float y, float z)
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
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setRotation(*reinterpret_cast<Quaternion*>(rot)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetEulerAngles(LNHandle worldobject, float x, float y, float z)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->SetEulerAngles(x, y, z));

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
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setScale(*reinterpret_cast<Vector3*>(scale)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetScale(LNHandle worldobject, float xyz)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setScale(xyz));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_SetScale(LNHandle worldobject, float x, float y, float z)
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

LN_API LNResultCode LNWorldObject_SetTag(LNHandle worldobject, int tag)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNWorldObject, worldobject)->setTag(tag));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNWorldObject_GetTag(LNHandle worldobject, int* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNWorldObject, worldobject)->getTag());

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
    (LWIG_TO_OBJECT(LNSceneNode, scenenode)->setVisible(visible));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSceneNode_IsVisible(LNHandle scenenode, LNBool outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSceneNode, scenenode)->isVisible());

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

LN_API LNResultCode LNSprite2DComponent_Create(LNHandle texture, LNHandle* outSprite2DComponent)
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
    (LWIG_TO_OBJECT(LNSpriteBase, spritebase)->setSourceRect(*reinterpret_cast<Rect*>(rect)));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_SetSourceRect(LNHandle spritebase, float x, float y, float width, float height)
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

LN_API LNResultCode LNSpriteBase_SetAnchorPoint(LNHandle spritebase, float ratioX, float ratioY)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSpriteBase, spritebase)->setAnchorPoint(ratioX, ratioY));

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API LNResultCode LNSpriteBase_Create(LNHandle* outSpriteBase)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSpriteBase, LNSpriteBase, initialize, );

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
