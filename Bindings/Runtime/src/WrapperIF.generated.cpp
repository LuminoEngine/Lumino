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
    *outReturn = (reinterpret_cast<const Vector3*>(vector3)->GetLength());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_GetLengthSquared(const LNVector3* vector3, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const Vector3*>(vector3)->GetLengthSquared());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_Normalize(LNVector3* vector3)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Vector3*>(vector3)->Normalize());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_Clamp(LNVector3* vector3, const LNVector3* minVec, const LNVector3* maxVec)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<Vector3*>(vector3)->Clamp(*reinterpret_cast<const Vector3*>(minVec), *reinterpret_cast<const Vector3*>(maxVec)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_NormalizeXYZ(float x, float y, float z, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::Normalize(x, y, z));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNVector3_NormalizeV(const LNVector3* vec, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(Vector3::Normalize(*reinterpret_cast<const Vector3*>(vec)));
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
    new (reinterpret_cast<RectF*>(rectf)) RectF();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_RectFXYWH(LNRectF* rectf, float x, float y, float width, float height)
{
    LWIG_FUNC_TRY_BEGIN;
    new (reinterpret_cast<RectF*>(rectf)) RectF(x, y, width, height);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_GetLeft(const LNRectF* rectf, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (reinterpret_cast<const RectF*>(rectf)->GetLeft());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_SetSize(LNRectF* rectf, const LNSize* size)
{
    LWIG_FUNC_TRY_BEGIN;
    (reinterpret_cast<RectF*>(rectf)->SetSize(*reinterpret_cast<const Size*>(size)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNRectF_GetSize(const LNRectF* rectf, LNSize* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNSize&>(reinterpret_cast<const RectF*>(rectf)->GetSize());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNEngine_Initialize()
{
    LWIG_FUNC_TRY_BEGIN;
    LFManager::PreInitialize();Engine::Initialize();LFManager::PostInitialize();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNEngine_Terminate()
{
    LWIG_FUNC_TRY_BEGIN;
    Engine::Terminate();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNEngine_Update(bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = Engine::Update();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_SetVolume(LNHandle sound, float volume)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->SetVolume(volume));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_GetVolume(LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSound, sound)->GetVolume());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_SetPitch(LNHandle sound, float pitch)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->SetPitch(pitch));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_GetPitch(LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSound, sound)->GetPitch());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_SetLoopEnabled(LNHandle sound, bool enabled)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->SetLoopEnabled(enabled));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_IsLoopEnabled(LNHandle sound, bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSound, sound)->IsLoopEnabled());
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
    (LWIG_TO_OBJECT(LNSound, sound)->Play());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_Stop(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->Stop());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_Pause(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSound, sound)->Pause());
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
    tr::TypeInfo::GetTypeInfo<Sound>()->SetBindingTypeInfo(data);
}
LN_API void LNTexture_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<Texture>()->SetBindingTypeInfo(data);
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
    tr::TypeInfo::GetTypeInfo<Texture2D>()->SetBindingTypeInfo(data);
}
LN_API void LNGraphicsResourceObject_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<GraphicsResourceObject>()->SetBindingTypeInfo(data);
}
LN_API LNResultCode LNSceneNode_SetPosition(LNHandle scenenode, const LNVector3* pos)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSceneNode, scenenode)->SetPosition(*reinterpret_cast<const Vector3*>(pos)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSceneNode_GetPosition(LNHandle scenenode, LNVector3* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const LNVector3&>(LWIG_TO_OBJECT(LNSceneNode, scenenode)->GetPosition());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSceneNode_SetVisible(LNHandle scenenode, bool visible)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSceneNode, scenenode)->SetVisible(visible));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSceneNode_IsVisible(LNHandle scenenode, bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = (LWIG_TO_OBJECT(LNSceneNode, scenenode)->IsVisible());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNSceneNode_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<SceneNode>()->SetBindingTypeInfo(data);
}
LN_API void LNVisualNode_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<VisualNode>()->SetBindingTypeInfo(data);
}
LN_API LNResultCode LNSprite_SetTexture(LNHandle sprite, LNHandle texture)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNSprite, sprite)->SetTexture(LWIG_TO_OBJECT(Texture, texture)));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSprite_GetTexture(LNHandle sprite, LNHandle* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_HANDLE(LWIG_TO_OBJECT(LNSprite, sprite)->GetTexture());
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNSprite_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<Sprite>()->SetBindingTypeInfo(data);
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
    tr::TypeInfo::GetTypeInfo<Sprite2D>()->SetBindingTypeInfo(data);
}
LN_API void LNSprite3D_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<Sprite3D>()->SetBindingTypeInfo(data);
}
LN_API void LNStaticMesh_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<StaticMesh>()->SetBindingTypeInfo(data);
}
LN_API LNResultCode LNBoxMesh_Initialize(LNHandle* outBoxMesh)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outBoxMesh, LNBoxMesh, Initialize, );
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNBoxMesh_InitializeS(const LNVector3* size, LNHandle* outBoxMesh)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outBoxMesh, LNBoxMesh, Initialize, *reinterpret_cast<const Vector3*>(size));
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNBoxMesh_InitializeWHD(float width, float height, float depth, LNHandle* outBoxMesh)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outBoxMesh, LNBoxMesh, Initialize, width, height, depth);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API void LNBoxMesh_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<BoxMesh>()->SetBindingTypeInfo(data);
}
LN_API void LNComponent_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::GetTypeInfo<Component>()->SetBindingTypeInfo(data);
}
LN_API LNResultCode LNGameApplication_Run(LNHandle gameapplication, LNHandle initialScene)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNGameApplication, gameapplication)->Run(LWIG_TO_OBJECT(GameScene, initialScene)));
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
    tr::TypeInfo::GetTypeInfo<GameApplication>()->SetBindingTypeInfo(data);
}
LN_API LNResultCode LNGameScene_OnStart(LNHandle gamescene)
{
    LWIG_FUNC_TRY_BEGIN;
    (LWIG_TO_OBJECT(LNGameScene, gamescene)->OnStart());
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
    tr::TypeInfo::GetTypeInfo<GameScene>()->SetBindingTypeInfo(data);
}


} // extern "C"
