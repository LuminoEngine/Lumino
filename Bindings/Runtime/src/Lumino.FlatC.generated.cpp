#include "Internal.h"
#include "Lumino.Wrapper.generated.h"

extern "C"
{



LN_API LNResultCode LNEngine_Initialize(LNHandle* outEngine)
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
    LWIG_CREATE_OBJECT(outSound, LNSound, initialize, filePath);

    LWIG_FUNC_TRY_END_RETURN;
}

LN_API void LNSound_SetBindingTypeInfo(void* data)
{
    tr::TypeInfo::getTypeInfo<Sound>()->setBindingTypeInfo(data);
}

	
} // extern "C"
