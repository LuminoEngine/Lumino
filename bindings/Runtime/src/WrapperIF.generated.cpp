#include "Internal.h"
#include "WrapperIF.generated.h"

extern "C" {

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
    LWIG_TO_OBJECT(Sound, sound)->SetVolume(volume);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_GetVolume(LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_OBJECT(Sound, sound)->GetVolume();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_SetPitch(LNHandle sound, float pitch)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->SetPitch(pitch);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_GetPitch(LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_OBJECT(Sound, sound)->GetPitch();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_SetLoopEnabled(LNHandle sound, bool enabled)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->SetLoopEnabled(enabled);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_IsLoopEnabled(LNHandle sound, bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_OBJECT(Sound, sound)->IsLoopEnabled();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_SetLoopRange(LNHandle sound, uint32_t begin, uint32_t length)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->SetLoopRange(begin, length);
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_Play(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Play();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_Stop(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Stop();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_Pause(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Pause();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNSound_Resume(LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Resume();
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
LN_API LNResultCode LNGameApplication_Run(LNHandle gameapplication, LNHandle initialScene)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(GameApplication, gameapplication)->Run(LWIG_TO_OBJECT(GameScene, initialScene));
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
    LWIG_TO_OBJECT(GameScene, gamescene)->OnStart();
    LWIG_FUNC_TRY_END_RETURN;
}
LN_API LNResultCode LNGameScene_OnStart_CallVirtualBase(LNHandle gamescene)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(GameScene, gamescene)->GameScene::OnStart();
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
