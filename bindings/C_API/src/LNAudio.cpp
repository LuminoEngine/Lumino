
#include "LNInternal.h"
#include "../../../src/Audio/AudioManager.h"
#include "../../../src/Audio/GameAudioImpl.h"
#include "../include/LNAudio.h"

//==============================================================================
// LNGameAudio
//==============================================================================

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGM(const LNChar* filePath, float volume, float pitch, double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGM(filePath, volume, pitch, fadeTime);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGMMem(const void* data, int dataSize, float volume, float pitch, double fadeTime)
{
    LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj = Sound::CreateInternal(LFManager::Engine->GetAudioManager(), stream, SoundLoadingMode::Sync);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGMFromSound(obj, volume, pitch, fadeTime);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_StopBGM(double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->StopBGM( fadeTime );
    LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGS( const LNChar* filename, float volume, float pitch, double fadeTime )
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGS( filename, volume, pitch, fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGSMem(const void* data, int dataSize, float volume, float pitch, double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj = Sound::CreateInternal(LFManager::Engine->GetAudioManager(), stream, SoundLoadingMode::Sync);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGSFromSound(obj, volume, pitch, fadeTime);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_StopBGS(double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->StopBGS( fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlayME( const LNChar* filename, float volume, float pitch )
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayME( filename, volume, pitch );
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlayMEMem(const void* data, int dataSize, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj = Sound::CreateInternal(LFManager::Engine->GetAudioManager(), stream, SoundLoadingMode::Sync);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayMEFromSound(obj, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_StopME()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->StopME();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE( const LNChar* filename, float volume, float pitch )
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySE( filename, volume, pitch );
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3D(const LNChar* filename, const LNVector3* position, float distance, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySE3D(filename, *cp_cast<Vector3>(position), distance, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3DXYZ( const LNChar* filename, float x, float y, float z, float distance, float volume, float pitch )
{
	LNVector3 pos = { x, y, z };
	return LNGameAudio_PlaySE3D(filename, &pos, distance, volume, pitch);
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlaySEMem(const void* data, int dataSize, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj = Sound::CreateInternal(LFManager::Engine->GetAudioManager(), stream, SoundLoadingMode::Sync);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySEFromSound(obj, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3DMem(const void* data, int dataSize, const LNVector3* position, float distance, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj = Sound::CreateInternal(LFManager::Engine->GetAudioManager(), stream, SoundLoadingMode::Sync);
	obj->SetEmitterPosition(*cp_cast<Vector3>(position));
	obj->SetEmitterMaxDistance(distance);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySEFromSound(obj, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3DMemXYZ(const void* data, int dataSize, float x, float y, float z, float distance, float volume, float pitch)
{
	LNVector3 pos = { x, y, z };
	return LNGameAudio_PlaySE3DMem(data, dataSize, &pos, distance, volume, pitch);
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_StopSE()
{
    LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->StopSE();
	LN_FUNC_TRY_END_RETURN;
}


///**
//@brief		3D 空間の1メートル相当の距離を設定します。
//@param[in]  distance	: 距離
//*/
//LN_STATIC_API
//LNResult LNGameAudio_Set3DMetreUnitDistance(float distance);
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//LNResult LNGameAudio_Set3DMetreUnitDistance(float distance)
//{
//	LN_FUNC_TRY_BEGIN;
//	LFManager::Engine->GetAudioManager()->SetMetreUnitDistance(distance);
//	LN_FUNC_TRY_END_RETURN;
//}
//
//------------------------------------------------------------------------------
LNResult LNGameAudio_SetBGMVolume(float volume, double fadeTime)
{
    LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->SetBGMVolume( volume, fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNGameAudio_SetBGSVolume(float volume, double fadeTime)
{
    LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->SetBGSVolume( volume, fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//==============================================================================
// LNSoundListener
//==============================================================================

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetPosition(const LNVector3* position)
{
	LN_CHECK_ARG_RETURN(position != NULL);
	SoundListener::SetPosition(*cp_cast<Vector3>(position));
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetPositionXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetPosition(&v);
}

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetDirection(const LNVector3* direction)
{
	LN_CHECK_ARG_RETURN(direction != NULL);
	SoundListener::SetDirection(*cp_cast<Vector3>(direction));
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetDirectionXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetDirection(&v);
}

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetUpDirection(const LNVector3* direction)
{
	LN_CHECK_ARG_RETURN(direction != NULL);
	SoundListener::SetUpDirection(*cp_cast<Vector3>(direction));
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetUpDirectionXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetUpDirection(&v);
}

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetVelocity(const LNVector3* velocity)
{
	LN_CHECK_ARG_RETURN(velocity != NULL);
	SoundListener::SetVelocity(*cp_cast<Vector3>(velocity));
	return ::LN_OK;
}

//------------------------------------------------------------------------------
LNResult LNSoundListener_SetVelocityXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetVelocity(&v);
}

//==============================================================================
// LNSound
//==============================================================================
LN_TYPE_INFO_IMPL(ln::Sound, LNSound);

//------------------------------------------------------------------------------
LNResult LNSound_Create(const LNChar* filePath, LNHandle* sound)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<Sound> obj = Sound::CreateInternal(LFManager::Engine->GetAudioManager(), filePath);
	*sound = LFManager::CheckRegisterObject(obj);
	obj.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_CreateMem(const void* data, int dataSize, LN_OUT LN_HANDLE(LNSound)* sound)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize, true), false);
	RefPtr<Sound> obj = Sound::CreateInternal(LFManager::Engine->GetAudioManager(), stream, SoundLoadingMode::Sync);
	*sound = LFManager::CheckRegisterObject(obj);
	obj.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetVolume(LN_HANDLE(LNSound) sound, float volume)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetVolume(volume);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_GetVolume(LN_HANDLE(LNSound) sound, float* outVolume)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outVolume != NULL);
	LN_FUNC_TRY_BEGIN;
	*outVolume = TO_REFOBJ(Sound, sound)->GetVolume();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetPitch(LN_HANDLE(LNSound) sound, float pitch)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetPitch(pitch);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_GetPitch(LN_HANDLE(LNSound) sound, float* outPitch)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outPitch != NULL);
	LN_FUNC_TRY_BEGIN;
	*outPitch = TO_REFOBJ(Sound, sound)->GetPitch();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetLoopEnabled(LN_HANDLE(LNSound) sound, LNBool loopEnable)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetLoopEnabled(LNC_TO_BOOL(loopEnable));
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_IsLoopEnabled(LN_HANDLE(LNSound) sound, LNBool* outEnabled)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outEnabled != NULL);
	LN_FUNC_TRY_BEGIN;
	*outEnabled = LNC_TO_LNBOOL(TO_REFOBJ(Sound, sound)->IsLoopEnabled());
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetLoopRange(LN_HANDLE(LNSound) sound, int begin, int length)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetLoopRange(begin, length);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_Set3DEnabled(LN_HANDLE(LNSound) sound, LNBool enabled)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Set3DEnabled(LNC_TO_BOOL(enabled));
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_Is3DEnabled(LN_HANDLE(LNSound) sound, LNBool* outEnabled)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outEnabled != NULL);
	LN_FUNC_TRY_BEGIN;
	*outEnabled = LNC_TO_LNBOOL(TO_REFOBJ(Sound, sound)->Is3DEnabled());
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetPlayingMode(LN_HANDLE(LNSound) sound, LNSoundPlayingMode mode)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetPlayingMode((SoundPlayingMode)mode);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_GetPlayingMode(LN_HANDLE(LNSound) sound, LNSoundPlayingMode* outType)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outType != NULL);
	LN_FUNC_TRY_BEGIN;
	*outType = (LNSoundPlayingMode)TO_REFOBJ(Sound, sound)->GetPlayingMode();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_GetPlayingState(LN_HANDLE(LNSound) sound, LNSoundPlayingState* outState)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outState != NULL);
	LN_FUNC_TRY_BEGIN;
	*outState = (LNSoundPlayingState)TO_REFOBJ(Sound, sound)->GetPlayingState();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_Play(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Play();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_Stop(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Stop();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_Pause(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Pause();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_Resume(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Resume();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_FadeVolume(LN_HANDLE(LNSound) sound, float targetVolume, double time, LNSoundFadeBehavior behavior)
{
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->FadeVolume(targetVolume, time, (SoundFadeBehavior)behavior);
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_GetPlayedSamples(LN_HANDLE(LNSound) sound, int64_t* outSamples)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outSamples != NULL);
	LN_FUNC_TRY_BEGIN;
	*outSamples = TO_REFOBJ(Sound, sound)->GetPlayedSamples();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_GetTotalSamples(LN_HANDLE(LNSound) sound, int64_t* outSamples)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outSamples != NULL);
	LN_FUNC_TRY_BEGIN;
	*outSamples = TO_REFOBJ(Sound, sound)->GetTotalSamples();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_GetSamplingRate(LN_HANDLE(LNSound) sound, int32_t* outRate)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(outRate != NULL);
	LN_FUNC_TRY_BEGIN;
	*outRate = TO_REFOBJ(Sound, sound)->GetSamplingRate();
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetEmitterPosition(LN_HANDLE(LNSound) sound, const LNVector3* position)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(position != NULL);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetEmitterPosition(*cp_cast<Vector3>(position));
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetEmitterPositionXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z)
{
	LNVector3 pos = { x, y, z };
	return LNSound_SetEmitterPosition(sound, &pos);
}

//------------------------------------------------------------------------------
LNResult LNSound_SetEmitterVelocity(LN_HANDLE(LNSound) sound, const LNVector3* velocity)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG_RETURN(velocity != NULL);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetEmitterVelocity(*cp_cast<Vector3>(velocity));
	LN_FUNC_TRY_END_RETURN;
}

//------------------------------------------------------------------------------
LNResult LNSound_SetEmitterVelocityXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z)
{
	LNVector3 pos = { x, y, z };
	return LNSound_SetEmitterVelocity(sound, &pos);
}

//------------------------------------------------------------------------------
LNResult LNSound_SetEmitterMaxDistance(LN_HANDLE(LNSound) sound, float distance)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetEmitterMaxDistance(distance);
	LN_FUNC_TRY_END_RETURN;
}
