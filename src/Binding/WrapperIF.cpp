#include "Internal.h"
#include "WrapperIF.h"
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
LNResultCode LN_API LNRectF_RectF(    LNRectF* rectf, LNHandle* outrectf)
{
    LWIG_FUNC_TRY_BEGIN;
    reinterpret_cast<RectF*>(rectf)->RectF();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNRectF_RectF(    LNRectF* rectf, float x, float y, float width, float height, LNHandle* outrectf)
{
    LWIG_FUNC_TRY_BEGIN;
    reinterpret_cast<RectF*>(rectf)->RectF(x, y, width, height);
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNRectF_GetLeft(    const LNRectF* rectf, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const RectF*>(rectf)->GetLeft();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNRectF_SetSize(    LNRectF* rectf, Size size)
{
    LWIG_FUNC_TRY_BEGIN;
    reinterpret_cast<RectF*>(rectf)->SetSize(size);
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNRectF_GetSize(    const LNRectF* rectf, Size* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const RectF*>(rectf)->GetSize();
    LWIG_FUNC_TRY_END_RETURN;
}
struct LNVector3
{
    float x;
    float y;
    float z;
};
LNResultCode LN_API LNVector3_GetLength(    const LNVector3* vector3, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const Vector3*>(vector3)->GetLength();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNVector3_GetLengthSquared(    const LNVector3* vector3, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = reinterpret_cast<const Vector3*>(vector3)->GetLengthSquared();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNVector3_Normalize(    LNVector3* vector3)
{
    LWIG_FUNC_TRY_BEGIN;
    reinterpret_cast<Vector3*>(vector3)->Normalize();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNVector3_Clamp(    LNVector3* vector3, Vector3 minVec, Vector3 maxVec)
{
    LWIG_FUNC_TRY_BEGIN;
    reinterpret_cast<Vector3*>(vector3)->Clamp(minVec, maxVec);
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNEngine_Initialize(    LNHandle* outengine)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outengine, Engine, Initialize, );
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNEngine_Terminate()
{
    LWIG_FUNC_TRY_BEGIN;
    Engine::Terminate();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNEngine_Update(    bool* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = Engine::Update();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_SetVolume(    LNHandle sound, float volume)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->SetVolume(volume);
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_GetVolume(    LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_OBJECT(Sound, sound)->GetVolume();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_SetPitch(    LNHandle sound, float pitch)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->SetPitch(pitch);
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_GetPitch(    LNHandle sound, float* outReturn)
{
    LWIG_FUNC_TRY_BEGIN;
    *outReturn = LWIG_TO_OBJECT(Sound, sound)->GetPitch();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_Play(    LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Play();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_Stop(    LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Stop();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_Pause(    LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Pause();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_Resume(    LNHandle sound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_TO_OBJECT(Sound, sound)->Resume();
    LWIG_FUNC_TRY_END_RETURN;
}
LNResultCode LN_API LNSound_Initialize(    const LNChar* filePath, LNHandle* outsound)
{
    LWIG_FUNC_TRY_BEGIN;
    LWIG_CREATE_OBJECT(outsound, Sound, Initialize, filePath);
    LWIG_FUNC_TRY_END_RETURN;
}

