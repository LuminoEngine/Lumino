#include "Internal.h"
#include "WrapperIF.generated.h"

extern "C" {

struct LNVector3
{
    float x;
    float y;
    float z;
};
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
LN_API LNResultCode LNVector3_Normalize(LNVector3* vector3)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Vector3*>(vector3)->normalize());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_Clamp(LNVector3* vector3, const LNVector3* minVec, const LNVector3* maxVec)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Vector3*>(vector3)->clamp(*reinterpret_cast<const Vector3*>(minVec), *reinterpret_cast<const Vector3*>(maxVec)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_NormalizeXYZ(float x, float y, float z, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize(x, y, z));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_NormalizeV(const LNVector3* vec, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::normalize(*reinterpret_cast<const Vector3*>(vec)));
    LWIG_FUNC_TRY_END_RETURN;
}
struct LNSize
{
    float width;
    float height;
};
struct LNRectF
{
    float x;
    float y;
    float width;
    float height;
};
LN_API LNResultCode LNRectF_RectF(LNRectF* rectf)
{
    LWIG_FUNC_TRY_BEGIN;
    new (reinterpret_cast<Rect*>(rectf)) Rect();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_RectFXYWH(LNRectF* rectf, float x, float y, float width, float height)
{
    LWIG_FUNC_TRY_BEGIN;
    new (reinterpret_cast<Rect*>(rectf)) Rect(x, y, width, height);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_GetLeft(const LNRectF* rectf, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const Rect*>(rectf)->getLeft());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_SetSize(LNRectF* rectf, const LNSize* size)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Rect*>(rectf)->setSize(*reinterpret_cast<const Size*>(size)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_GetSize(const LNRectF* rectf, LNSize* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNSize&>(reinterpret_cast<const Rect*>(rectf)->getSize());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNEngine_Initialize()
{
    LWIG_FUNC_TRY_BEGIN;
    LFManager::PreInitialize();(Engine::initialize());LFManager::postInitialize();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNEngine_Terminate()
{
    LWIG_FUNC_TRY_BEGIN;
    (Engine::terminate());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNEngine_Update(bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (Engine::update());
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
LN_API LNResultCode LNSound_SetLoopEnabled(LNHandle sound, bool enabled)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->setLoopEnabled(enabled));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_IsLoopEnabled(LNHandle sound, bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSound, sound)->isLoopEnabled());
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
LN_API LNResultCode LNSound_Initialize(const LNChar* filePath, LNHandle* outSound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSound, LNSound, Initialize, filePath);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNSound_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sound>()->setBindingTypeInfo(data);
}
LN_API void LNTexture_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Texture>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNTexture2D_Initialize(int width, int height, TextureFormat format, bool mipmap, LNHandle* outTexture2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outTexture2D, LNTexture2D, Initialize, width, height, format, mipmap);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNTexture2D_InitializeFFM(const LNChar* filePath, TextureFormat format, bool mipmap, LNHandle* outTexture2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outTexture2D, LNTexture2D, Initialize, filePath, format, mipmap);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNTexture2D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Texture2D>()->setBindingTypeInfo(data);
}
LN_API void LNGraphicsResourceObject_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<GraphicsResourceObject>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSceneNode_SetPosition(LNHandle scenenode, const LNVector3* pos)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSceneNode, scenenode)->setPosition(*reinterpret_cast<const Vector3*>(pos)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSceneNode_GetPosition(LNHandle scenenode, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(LWIG_TO_OBJECT(LNSceneNode, scenenode)->getPosition());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSceneNode_SetVisible(LNHandle scenenode, bool visible)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSceneNode, scenenode)->setVisible(visible));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSceneNode_IsVisible(LNHandle scenenode, bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSceneNode, scenenode)->isVisible());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNSceneNode_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<SceneNode>()->setBindingTypeInfo(data);
}
LN_API void LNVisualNode_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<VisualComponent>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSprite_SetTexture(LNHandle sprite, LNHandle texture)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSprite, sprite)->setTexture(LWIG_TO_OBJECT(Texture, texture)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSprite_GetTexture(LNHandle sprite, LNHandle* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_HANDLE(LWIG_TO_OBJECT(LNSprite, sprite)->getTexture());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNSprite_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<SpriteComponent>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNSprite2D_Initialize(LNHandle* outSprite2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite2D, LNSprite2D, Initialize, );
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSprite2D_InitializeT(LNHandle texture, LNHandle* outSprite2D)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outSprite2D, LNSprite2D, Initialize, LWIG_TO_OBJECT(Texture, texture));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNSprite2D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sprite2DComponent>()->setBindingTypeInfo(data);
}
LN_API void LNSprite3D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sprite3DComponent>()->setBindingTypeInfo(data);
}
LN_API void LNStaticMesh_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<StaticMeshComponent>()->setBindingTypeInfo(data);
}
LN_API void LNComponent_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Component>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNGameApplication_Run(LNHandle gameapplication, LNHandle initialScene)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNGameApplication, gameapplication)->run(LWIG_TO_OBJECT(GameScene, initialScene)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNGameApplication_Initialize(LNHandle* outGameApplication)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outGameApplication, LNGameApplication, Initialize, );
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNGameApplication_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<GameApplication>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNGameScene_OnStart(LNHandle gamescene)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNGameScene, gamescene)->onStart());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNGameScene_OnStart_CallVirtualBase(LNHandle gamescene)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNGameScene, gamescene)->LNGameScene::OnStart_CallBase());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNGameScene_OnStart_SetOverrideCaller(GameScene_OnStart_OverrideCaller callback)
{
    LWIG_FUNC_TRY_BEGIN;
    LNGameScene::m_OnStart_OverrideCaller = callback;
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNGameScene_Initialize(LNHandle* outGameScene)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outGameScene, LNGameScene, Initialize, );
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNGameScene_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<GameScene>()->setBindingTypeInfo(data);
}
LN_API void LNCollisionShape_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<CollisionShape>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNBoxCollisionShape_Initialize(const LNVector3* size, LNHandle* outBoxCollisionShape)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outBoxCollisionShape, LNBoxCollisionShape, Initialize, *reinterpret_cast<const Vector3*>(size));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNBoxCollisionShape_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<BoxCollisionShape>()->setBindingTypeInfo(data);
}
LN_API void LNPhysicsObject_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<PhysicsObject>()->setBindingTypeInfo(data);
}
LN_API LNResultCode LNCollider_AddShape(LNHandle collider, LNHandle shape)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNCollider, collider)->addShape(LWIG_TO_OBJECT(CollisionShape, shape)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNCollider_SetTrigger(LNHandle collider, bool enabled)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNCollider, collider)->setTrigger(enabled));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNCollider_IsTrigger(LNHandle collider, bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNCollider, collider)->isTrigger());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNCollider_ConnectOnTriggerEnter(LNHandle self, LNCollisionEventHandler callback)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(LNCollider, self)->m_ConnectOnTriggerEnter_EventWrapper.connect(callback);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNCollider_ConnectOnTriggerLeave(LNHandle self, LNCollisionEventHandler callback)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(LNCollider, self)->m_ConnectOnTriggerLeave_EventWrapper.connect(callback);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNCollider_ConnectOnTriggerStay(LNHandle self, LNCollisionEventHandler callback)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(LNCollider, self)->m_ConnectOnTriggerStay_EventWrapper.connect(callback);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNCollider_Initialize(LNHandle* outCollider)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outCollider, LNCollider, Initialize, );
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNCollider_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<CollisionBody>()->setBindingTypeInfo(data);
}


} // extern "C"
