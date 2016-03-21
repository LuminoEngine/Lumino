
#include "LNInternal.h"
#include "../../../src/Audio/AudioManager.h"
#include "../../../src/Audio/GameAudioImpl.h"
#include "../include/LNAudio.h"

//=============================================================================
// LNGameAudio
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGM(const LNChar* filePath, float volume, float pitch, double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGM(filePath, volume, pitch, fadeTime);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGMMem(const void* data, int dataSize, float volume, float pitch, double fadeTime)
{
    LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj(Sound::Create(stream, SoundLoadingMode::Sync), false);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGMFromSound(obj, volume, pitch, fadeTime);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_StopBGM(double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->StopBGM( fadeTime );
    LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGS( const LNChar* filename, float volume, float pitch, double fadeTime )
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGS( filename, volume, pitch, fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlayBGSMem(const void* data, int dataSize, float volume, float pitch, double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj(Sound::Create(stream, SoundLoadingMode::Sync), false);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayBGSFromSound(obj, volume, pitch, fadeTime);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_StopBGS(double fadeTime)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->StopBGS( fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlayME( const LNChar* filename, float volume, float pitch )
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayME( filename, volume, pitch );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlayMEMem(const void* data, int dataSize, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj(Sound::Create(stream, SoundLoadingMode::Sync), false);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlayMEFromSound(obj, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_StopME()
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->StopME();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE( const LNChar* filename, float volume, float pitch )
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySE( filename, volume, pitch );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3D(const LNChar* filename, const LNVector3* position, float distance, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySE3D(filename, *cp_cast<Vector3>(position), distance, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3DXYZ( const LNChar* filename, float x, float y, float z, float distance, float volume, float pitch )
{
	LNVector3 pos = { x, y, z };
	return LNGameAudio_PlaySE3D(filename, &pos, distance, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlaySEMem(const void* data, int dataSize, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj(Sound::Create(stream, SoundLoadingMode::Sync), false);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySEFromSound(obj, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3DMem(const void* data, int dataSize, const LNVector3* position, float distance, float volume, float pitch)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize), false);
	RefPtr<Sound> obj(Sound::Create(stream, SoundLoadingMode::Sync), false);
	obj->SetEmitterPosition(*cp_cast<Vector3>(position));
	obj->SetEmitterMaxDistance(distance);
	LFManager::Engine->GetAudioManager()->GetGameAudio()->PlaySEFromSound(obj, volume, pitch);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_PlaySE3DMemXYZ(const void* data, int dataSize, float x, float y, float z, float distance, float volume, float pitch)
{
	LNVector3 pos = { x, y, z };
	return LNGameAudio_PlaySE3DMem(data, dataSize, &pos, distance, volume, pitch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//LNResult LNGameAudio_Set3DMetreUnitDistance(float distance)
//{
//	LN_FUNC_TRY_BEGIN;
//	LFManager::Engine->GetAudioManager()->SetMetreUnitDistance(distance);
//	LN_FUNC_TRY_END_RETURN;
//}
//
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_SetBGMVolume(float volume, double fadeTime)
{
    LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->SetBGMVolume( volume, fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNGameAudio_SetBGSVolume(float volume, double fadeTime)
{
    LN_FUNC_TRY_BEGIN;
	LFManager::Engine->GetAudioManager()->GetGameAudio()->SetBGSVolume( volume, fadeTime );
	LN_FUNC_TRY_END_RETURN;
}

//=============================================================================
// LNSoundListener
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetPosition(const LNVector3* position)
{
	LN_CHECK_ARG(position != NULL);
	SoundListener::SetPosition(*cp_cast<Vector3>(position));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetPositionXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetPosition(&v);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetDirection(const LNVector3* direction)
{
	LN_CHECK_ARG(direction != NULL);
	SoundListener::SetDirection(*cp_cast<Vector3>(direction));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetDirectionXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetDirection(&v);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetUpDirection(const LNVector3* direction)
{
	LN_CHECK_ARG(direction != NULL);
	SoundListener::SetUpDirection(*cp_cast<Vector3>(direction));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetUpDirectionXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetUpDirection(&v);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetVelocity(const LNVector3* velocity)
{
	LN_CHECK_ARG(velocity != NULL);
	SoundListener::SetVelocity(*cp_cast<Vector3>(velocity));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSoundListener_SetVelocityXYZ(float x, float y, float z)
{
	LNVector3 v = { x, y, z };
	return LNSoundListener_SetVelocity(&v);
}

//=============================================================================
// LNSound
//=============================================================================
LN_TYPE_INFO_IMPL(ln::Sound, LNSound);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Create(const LNChar* filePath, LNHandle* sound)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<Sound> obj(Sound::Create(filePath, LFManager::Engine->GetAudioManager()), false);
	*sound = LFManager::CheckRegisterObject(obj);
	obj.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_CreateMem(const void* data, int dataSize, LN_OUT LN_HANDLE(LNSound)* sound)
{
	LN_FUNC_TRY_BEGIN;
	RefPtr<MemoryStream> stream(LN_NEW MemoryStream(data, dataSize, true), false);
	RefPtr<Sound> obj(Sound::Create(stream, SoundLoadingMode::Sync), false);
	*sound = LFManager::CheckRegisterObject(obj);
	obj.SafeAddRef();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetVolume(LN_HANDLE(LNSound) sound, float volume)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetVolume(volume);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetVolume(LN_HANDLE(LNSound) sound, float* outVolume)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outVolume != NULL);
	LN_FUNC_TRY_BEGIN;
	*outVolume = TO_REFOBJ(Sound, sound)->GetVolume();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetPitch(LN_HANDLE(LNSound) sound, float pitch)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetPitch(pitch);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetPitch(LN_HANDLE(LNSound) sound, float* outPitch)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outPitch != NULL);
	LN_FUNC_TRY_BEGIN;
	*outPitch = TO_REFOBJ(Sound, sound)->GetPitch();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetLoopEnabled(LN_HANDLE(LNSound) sound, LNBool loopEnable)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetLoopEnabled(LNC_TO_BOOL(loopEnable));
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_IsLoopEnabled(LN_HANDLE(LNSound) sound, LNBool* outEnabled)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outEnabled != NULL);
	LN_FUNC_TRY_BEGIN;
	*outEnabled = LNC_TO_LNBOOL(TO_REFOBJ(Sound, sound)->IsLoopEnabled());
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetLoopRange(LN_HANDLE(LNSound) sound, int begin, int length)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetLoopRange(begin, length);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Set3DEnabled(LN_HANDLE(LNSound) sound, LNBool enabled)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Set3DEnabled(LNC_TO_BOOL(enabled));
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Is3DEnabled(LN_HANDLE(LNSound) sound, LNBool* outEnabled)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outEnabled != NULL);
	LN_FUNC_TRY_BEGIN;
	*outEnabled = LNC_TO_LNBOOL(TO_REFOBJ(Sound, sound)->Is3DEnabled());
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetPlayingMode(LN_HANDLE(LNSound) sound, LNSoundPlayingMode mode)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetPlayingMode((SoundPlayingMode)mode);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetPlayingMode(LN_HANDLE(LNSound) sound, LNSoundPlayingMode* outType)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outType != NULL);
	LN_FUNC_TRY_BEGIN;
	*outType = (LNSoundPlayingMode)TO_REFOBJ(Sound, sound)->GetPlayingMode();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetPlayingState(LN_HANDLE(LNSound) sound, LNSoundPlayingState* outState)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outState != NULL);
	LN_FUNC_TRY_BEGIN;
	*outState = (LNSoundPlayingState)TO_REFOBJ(Sound, sound)->GetPlayingState();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Play(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Play();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Stop(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Stop();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Pause(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Pause();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Resume(LN_HANDLE(LNSound) sound)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->Resume();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_FadeVolume(LN_HANDLE(LNSound) sound, float targetVolume, double time, LNSoundFadeBehavior behavior)
{
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->FadeVolume(targetVolume, time, (SoundFadeBehavior)behavior);
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetPlayedSamples(LN_HANDLE(LNSound) sound, int64_t* outSamples)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outSamples != NULL);
	LN_FUNC_TRY_BEGIN;
	*outSamples = TO_REFOBJ(Sound, sound)->GetPlayedSamples();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetTotalSamples(LN_HANDLE(LNSound) sound, int64_t* outSamples)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outSamples != NULL);
	LN_FUNC_TRY_BEGIN;
	*outSamples = TO_REFOBJ(Sound, sound)->GetTotalSamples();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetSamplingRate(LN_HANDLE(LNSound) sound, int32_t* outRate)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(outRate != NULL);
	LN_FUNC_TRY_BEGIN;
	*outRate = TO_REFOBJ(Sound, sound)->GetSamplingRate();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterPosition(LN_HANDLE(LNSound) sound, const LNVector3* position)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(position != NULL);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetEmitterPosition(*cp_cast<Vector3>(position));
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterPositionXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z)
{
	LNVector3 pos = { x, y, z };
	return LNSound_SetEmitterPosition(sound, &pos);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterVelocity(LN_HANDLE(LNSound) sound, const LNVector3* velocity)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_CHECK_ARG(velocity != NULL);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetEmitterVelocity(*cp_cast<Vector3>(velocity));
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterVelocityXYZ(LN_HANDLE(LNSound) sound, float x, float y, float z)
{
	LNVector3 pos = { x, y, z };
	return LNSound_SetEmitterVelocity(sound, &pos);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterMaxDistance(LN_HANDLE(LNSound) sound, float distance)
{
	LN_CHECK_ARG_HANDLE(sound);
	LN_FUNC_TRY_BEGIN;
	TO_REFOBJ(Sound, sound)->SetEmitterMaxDistance(distance);
	LN_FUNC_TRY_END_RETURN;
}

#if 0
#if 1;
}
#else
//-----------------------------------------------------------------------------
///**
//  @brief      ファイルから音声を作成する
//  @param[in]  filePath	: ファイル名
//  @param[in]  enable3d	: true の場合、3D 音源として作成する
//  @param[in]  playType	: 音声の再生方法
//  @return     サウンドハンドル
//*/
//-----------------------------------------------------------------------------
LNOTEAPI LNHandle LNSound_Create(const LNChar* filePath, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE), LNSoundPlayingType playType LNOTE_DEFAULT_ARG(LN_SOUNDPLAYINGTYPE_AUTO));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNOTEAPI LNHandle LNSound_Create( const LNChar* filename, LNBool enable3d, LNSoundPlayingType playType )
{
	LN_FUNC_TRY_BEGIN;
	LRefPtr<Core::Audio::Sound> obj(
		FuncLibManager::AudioManager->createSound(filename, (Core::Audio::SoundPlayType)playType, LNOTE_BOOL_TO_LNBOOL(enable3d)));
	obj.addRef();
	return TO_HANDLE( FuncLibManager::pushObject( obj ) );
	LN_FUNC_TRY_END;
	return NULL;
}
#endif

#if 1
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_CreateMem(LNHandle* sound, const void* data, int dataSize, LNBool enable_3d)
{
	LN_FUNC_TRY_BEGIN;

	LRefPtr<Core::FileIO::Stream> stream(
		FileIO::FileUtils::createInStreamFromMemoryManaged(data, dataSize));

	LRefPtr<Core::Audio::Sound> obj(
		FuncLibManager::AudioManager->createSound(
		stream, Core::Audio::SOUNDPLAYTYPE_ONMEMORY, LNOTE_BOOL_TO_LNBOOL(enable_3d), Core::Base::SharingKey::NULL_KEY ) );
	obj.addRef();

	*sound = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
	LN_FUNC_TRY_END_RETURN;
}
#else
//-----------------------------------------------------------------------------
///**
//  @brief      メモリ上のデータから音声を作成する
//  @param[in]  data		: メモリ上の音声データへのポインタ
//  @param[in]  dataSize	: データサイズ (バイト単位)
//  @param[in]  enable3d	: true の場合、3D 音源として作成する
//  @return     サウンドハンドル
//*/
//-----------------------------------------------------------------------------
LNOTEAPI LNHandle LNSound_CreateMem(const LNByte* data, int dataSize, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE));

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNHandle LNSound_CreateMem( const LNByte* data, int dataSize, LNBool enable_3d )
{
    LN_FUNC_TRY_BEGIN;

	LRefPtr<Core::FileIO::IInStream> stream(
		FileIO::File::createInStreamFromMemoryManaged( data, dataSize ) );
            
	LRefPtr<Core::Audio::Sound> obj(
		FuncLibManager::AudioManager->createSound(
			stream, Core::Audio::SOUNDPLAYTYPE_ONMEMORY, LNOTE_BOOL_TO_LNBOOL(enable_3d), Core::Base::SharingKey::NULL_KEY ) );
	obj.addRef();
	return TO_HANDLE( FuncLibManager::pushObject( obj ) );
	LN_FUNC_TRY_END;
	return NULL;
}
#endif

//-----------------------------------------------------------------------------
///**
//  @brief      ファイルから音声を作成する
//  @param[out] outHandle	: サウンドハンドルを格納する変数のポインタ
//  @param[in]  filePath	: ファイル名
//  @param[in]  enable3d	: true の場合、3D 音源として作成する
//  @param[in]  playType	: 音声の再生方法
//                      @arg LN_SOUNDPLAYTYPE_AUTO          : 自動選択
//                      @arg LN_SOUNDPLAYTYPE_ONMEMORY      : オンメモリ
//                      @arg LN_SOUNDPLAYTYPE_STREAMING     : ストリーミング
//*/
//-----------------------------------------------------------------------------[{hsp name=LNSound_Create}]
LNOTEAPI void LNSound_CreateToPtr(LNHandle* outHandle, const LNChar* filePath, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE), LNSoundPlayingType playType LNOTE_DEFAULT_ARG(LN_SOUNDPLAYINGTYPE_AUTO));


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void LNSound_CreateToPtr( LNHandle* out_handle, const LNChar* filename, LNBool enable_3d, LNSoundPlayingType play_type )
//{
//    //*out_handle = LNSound_Create( filename, LNOTE_BOOL_TO_LNBOOL(enable_3d), play_type );
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetVolume(LNHandle sound, int volume)
{
	LN_FUNC_TRY_BEGIN;
    TO_SOUND( sound )->setVolume( volume );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetVolume(LNHandle sound, int* volume)
{
	*volume = TO_SOUND(sound)->getVolume();
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetPitch(LNHandle sound, int pitch)
{
	LN_FUNC_TRY_BEGIN
    TO_SOUND( sound )->setPitch( pitch );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetPitch(LNHandle sound, int* pitch)
{
	*pitch = TO_SOUND(sound)->getPitch();
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetLoopState(LNHandle sound, LNBool loop, int begin, int length)
{
	LN_FUNC_TRY_BEGIN;
    TO_SOUND( sound )->loop( loop != LN_FALSE );
    TO_SOUND( sound )->setLoopState( begin, length );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_IsLoop(int sound, LNBool* enabled)
{
	*enabled = LNOTE_BOOL_TO_LNBOOL(TO_SOUND(sound)->isLoop());
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetPlayState(LNHandle sound, LNSoundPlayingState* outState)
{
	Core::Audio::AudioPlayState state;
	state.PlayingState = Core::Audio::SOUNDPLAYSTATE_STOPPED;
	LN_FUNC_TRY_BEGIN;
	TO_SOUND(sound)->getState(&state);
	*outState = (LNSoundPlayingState)state.PlayingState;
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Play(LNHandle sound)
{
	LN_FUNC_TRY_BEGIN;
    TO_SOUND( sound )->play();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Stop(LNHandle sound)
{
	LN_FUNC_TRY_BEGIN;
    TO_SOUND( sound )->stop();
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Pause(LNHandle sound, LNBool pause)
{
	LN_FUNC_TRY_BEGIN;
    TO_SOUND( sound )->pause( pause != LN_FALSE );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_FadeVolume(LNHandle sound, int target_volume, int time, LNSoundFadeState fade_state)
{
	LN_FUNC_TRY_BEGIN;
    TO_SOUND( sound )->fadeVolume( target_volume, time, (Core::Audio::SoundFadeState)fade_state );
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetUnitsPlayed(LNHandle sound, int* samples)
{
    Core::Audio::AudioPlayState state;
	LN_FUNC_TRY_BEGIN;
	TO_SOUND(sound)->getState(&state);
	*samples = state.CurrentUnits;
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetTotalUnits(LNHandle sound, int* samples)
{
    Core::Audio::AudioPlayState state;
	LN_FUNC_TRY_BEGIN;
	TO_SOUND(sound)->getState(&state);
	*samples = state.TotalUnits;
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_GetUnitsParSecond(LNHandle sound, int* frequency)
{
    Core::Audio::AudioPlayState state;
	LN_FUNC_TRY_BEGIN;
    TO_SOUND( sound )->getState( &state );
	*frequency = state.SamplesPerSec;
	LN_FUNC_TRY_END_RETURN;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_Is3DSound(LNHandle sound, LNBool* enabled)
{
	*enabled = LNOTE_BOOL_TO_LNBOOL(TO_SOUND(sound)->is3DSound());
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterPosition(LNHandle sound, const LNVector3* position)
{
	TO_SOUND(sound)->setPosition(TO_CORE_VEC3_REF(position));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterPositionXYZ(LNHandle sound, float x, float y, float z)
{
	TO_SOUND(sound)->setPosition(LVector3(x, y, z));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterVelocity(int sound, const LNVector3* velocity)
{
	TO_SOUND(sound)->setVelocity(TO_CORE_VEC3_REF(velocity));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterVelocityXYZ(int sound, float x, float y, float z)
{
	TO_SOUND(sound)->setVelocity(LVector3(x, y, z));
	return ::LN_OK;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
LNResult LNSound_SetEmitterDistance(int sound, float distance)
{
	TO_SOUND( sound )->setMaxDistance( distance );
	return ::LN_OK;
}
#endif

#if 0

#pragma once
#include "LFAudio2.h"

#if 0

//=============================================================================
// LFAudio 
//-------------------------------------------------------------------------------------
///**
//  @file       LFAudio.h
//  @brief      LFAudio
//	@defgroup   group_lib_audio Audio
//	@{
//*/
//=============================================================================

#pragma once

#include "LFCommon.h"
#include "LFTypedef.h"

extern "C" {

//=============================================================================
// LNAudio
//-------------------------------------------------------------------------------------
///**
//  @defgroup   group_audio LNAudio
//  @brief      音声管理及びゲームオーディオ機能
//  @{
//*/
//=============================================================================

	//-----------------------------------------------------------------------------
	///**
	//  @brief      BGM を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI LNResult LNGameAudio_PlayBGM( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), double fadeTime LNOTE_DEFAULT_ARG(0) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//-----------------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから BGM を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI LNResult LNGameAudio_PlayBGMMem( const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), double fadeTime LNOTE_DEFAULT_ARG(0) );

	//-----------------------------------------------------------------------------
	///**
	//  @brief      BGM の演奏を停止する
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_StopBGM(double fadeTime LNOTE_DEFAULT_ARG(0));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      BGS を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI LNResult LNGameAudio_PlayBGS( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), double fadeTime LNOTE_DEFAULT_ARG(0) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//-----------------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから BGS を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @param[in]  fadeTime	: フェードインにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_PlayBGSMem(const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100), double fadeTime LNOTE_DEFAULT_ARG(0));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      BGS の演奏を停止する
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_StopBGS(double fadeTime LNOTE_DEFAULT_ARG(0));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ME を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI LNResult LNGameAudio_PlayME( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//-----------------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから ME を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_PlayMEMem(const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ME の演奏を停止する
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI void LNGameAudio_StopME();

	//-----------------------------------------------------------------------------
	///**
	//  @brief      SE を演奏する
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI LNResult LNGameAudio_PlaySE( const LNChar* filePath, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//-----------------------------------------------------------------------------
	///**
	//  @brief      SE を演奏する (3D サウンド)
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  position	: 3D 空間上の座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_PlaySE3D(const LNChar* filePath, const LNVector3* position, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//-----------------------------------------------------------------------------
	///**
	//  @brief      SE を演奏する (3D サウンド)
	//  @param[in]  filePath	: ファイル名
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI LNResult LNGameAudio_PlaySE3DXYZ( const LNChar* filePath, float x, float y, float z, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100) );
	/*DeclOpt
	HSP_Override = true;
	DeclOpt*/

	//-----------------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから SE を演奏する
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_PlaySEMem(const void* data, int dataSize, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから SE を演奏する (3D サウンド)
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  position	: 3D 空間上の座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_PlaySE3DMem(const void* data, int dataSize, const LNVector3* position, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声データから SE を演奏する (3D サウンド)
	//  @param[in]  data		: メモリ上の音声ファイルデータ
	//  @param[in]  dataSize	: データサイズ (バイト単位)
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//  @param[in]  distance	: 減衰距離
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  pitch		: ピッチ (50 ～ 200)
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_PlaySE3DMemXYZ(const void* data, int dataSize, float x, float y, float z, float distance, int volume LNOTE_DEFAULT_ARG(100), int pitch LNOTE_DEFAULT_ARG(100));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      すべての SE の演奏を停止する
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI void LNGameAudio_StopSE();

	//-----------------------------------------------------------------------------
	///**
	//  @brief      3D 空間の1メートル相当の距離の設定
	//  @param[in]  distance	: 距離
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_SetMetreUnitDistance(float distance);

	//-----------------------------------------------------------------------------
	///**
	//  @brief     再生中のBGMの音量を設定する (フェードアウト中は無効)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_SetBGMVolume(int volume, double fadeTime LNOTE_DEFAULT_ARG(0));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      再生中のBGSの音量を設定する (フェードアウト中は無効)
	//  @param[in]  volume		: ボリューム (0 ～ 100)
	//  @param[in]  fadeTime	: フェードアウトにかける時間 (ミリ秒)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNGameAudio_SetBGSVolume(int volume, double fadeTime LNOTE_DEFAULT_ARG(0));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      [使用非推奨] BGM、BGS、ME として再生されているサウンドオブジェクトの取得
	//  @param[in]  type	: 取得するサウンドの種類
	//  @return     サウンドハンドル
	//	@details	この関数は、BGM、BGS、ME の音量、ピッチの変更、及び
	//				再生位置の取得のために用意されています。
	//				この関数から取得したサウンドハンドルに対して
	//				Release、Play、Stop、Pause を行わないでください。
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNHandle LNGameAudio_GetInternalGameSound( LNInternalGameSound type );
	/*DeclOpt
	ReturnHandleType = "LNSound";
	DeclOpt*/

/** @} */

//=============================================================================
// LNSoundListener
//-------------------------------------------------------------------------------------
///**
//  @defgroup   group_soundlistener LNSoundListener
//  @brief      3D音声リスナー
//  @{
//*/
//=============================================================================

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの位置の設定
	//  @param[in]  position	: 3D 空間上の座標
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_SetPosition(const LNVector3* position);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの位置の設定
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI void LNSoundListener_SetPositionXYZ( float x, float y, float z );

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの正面方向の設定
	//  @param[in]  direction		: 向き
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_SetDirection(const LNVector3* direction);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの正面方向の設定
	//  @param[in]  x			: 向きの X 成分
	//  @param[in]  y			: 向きの Y 成分
	//  @param[in]  z			: 向きの Z 成分
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI void LNSoundListener_SetDirectionXYZ(float x, float y, float z);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの上方向の設定 (正面方向とは直交であること)
	//  @param[in]  direction	: 上方向
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_SetUpDirection(const LNVector3* direction);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの上方向の設定 (正面方向とは直交であること)
	//  @param[in]  x			: 向きの X 成分
	//  @param[in]  y			: 向きの Y 成分
	//  @param[in]  z			: 向きの Z 成分
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI void LNSoundListener_SetUpDirectionXYZ( float x, float y, float z );

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの速度の設定
	//  @param[in]  velocity	: 速度
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSoundListener_Velocity(const LNVector3* velocity);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      リスナーの速度の設定
	//  @param[in]  x			: 速度の X 成分
	//  @param[in]  y			: 速度の Y 成分
	//  @param[in]  z			: 速度の Z 成分
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI void LNSoundListener_VelocityXYZ( float x, float y, float z );
    
/** @} */

//=============================================================================
// LNSound
//-------------------------------------------------------------------------------------
///**
//  @defgroup   group_sound LNSound
//  @brief      音声機能
//  @{
//*/
//=============================================================================

	LNOTE_TYPE_INFO_DECL(LNSound);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ファイルから音声を作成する
	//  @param[out]	sound		: sound
	//  @param[in]	filePath	: ファイル名
	//  @param[in]	enable3d	: true の場合、3D 音源として作成する
	//  @param[in]	playType	: 音声の再生方法
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
    LNOTEAPI LNResult LNSound_Create(LNHandle* sound, const LNChar* filePath, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE), LNSoundPlayingType playType LNOTE_DEFAULT_ARG(LN_SOUNDPLAYINGTYPE_AUTO) );
	
	//-----------------------------------------------------------------------------
	///**
	//  @brief      メモリ上の音声ファイルデータから音声を作成する
	//  @param[out]	sound		: sound
	//  @param[in]	data		: メモリ上の音声データへのポインタ
	//  @param[in]	dataSize	: データサイズ (バイト単位)
	//  @param[in]	enable3d	: true の場合、3D 音源として作成する
	//  @return     処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_CreateMem(LNHandle* sound, const void* data, int dataSize, LNBool enable3d LNOTE_DEFAULT_ARG(LN_FALSE));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ボリュームの設定
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  volume          : ボリューム (0～100)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetVolume(LNHandle sound, int volume);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ボリュームの取得
	//  @param[in]	sound		: サウンドハンドル
	//  @param[out]	volume		: ボリュームを格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetVolume(LNHandle sound, int* volume);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ピッチの設定
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  pitch		: ピッチ (50～200)
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetPitch(LNHandle sound, int pitch);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ピッチの取得
	//  @param[in]	sound		: サウンドハンドル
	//  @param[out]	pitch		: ピッチを格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetPitch(LNHandle sound, int* pitch);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ループ再生の有効設定
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  loopEnable	: LN_TRUE = ループ再生する / LN_FALSE = しない
	//  @param[in]  begin		: ループ領域の先頭位置 (サンプル数単位)
	//  @param[in]  length		: ループ領域長さ (サンプル数単位)
	//  @details	begin と length に 0 を指定すると、全体をループ領域として設定します。
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetLoopState(LNHandle sound, LNBool loopEnable, int begin LNOTE_DEFAULT_ARG(0), int length LNOTE_DEFAULT_ARG(0));

	//-----------------------------------------------------------------------------
	///**
	//  @brief      ループ再生が有効かを調べる
	//  @param[in]	sound		: サウンドハンドル
	//  @param[out]	enabled		: ループ再生の有無状態を格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_IsLoop(LNHandle sound, LNBool* enabled);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      サウンドの再生状態を取得する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	state		: 状態を格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetPlayState(LNHandle sound, LNSoundPlayingState* state);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      音声を再生する
	//  @param[in]  sound		: サウンドハンドル
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Play(LNHandle sound);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      再生を停止する
	//  @param[in]  sound		: サウンドハンドル
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Stop(LNHandle sound);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      一時停止
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  pause		: LN_TRUE = 一時停止 / LN_FALSE = 一時停止解除
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Pause(LNHandle sound, LNBool pause);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      音量のフェード
	//  @param[in]  sound			: サウンドハンドル
	//  @param[in]  targetVolume	: 変更先の音量
	//  @param[in]  time			: フェードにかける時間 (ミリ秒)
	//  @param[in]  fadeState		: フェード完了後の動作の指定
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_FadeVolume(LNHandle sound, int targetVolume, int time, LNSoundFadeState fadeState);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      再生したサンプル数の取得
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	samples		: 再生したサンプル数を格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetUnitsPlayed(LNHandle sound, int* samples);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      音声データ全体のサンプル数の取得
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	samples		: 音声データ全体のサンプル数を格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetTotalUnits(LNHandle sound, int* samples);

	//-----------------------------------------------------------------------------
	///**
	//  @brief      サンプリング周波数の取得
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out]	frequency	: サンプリング周波数を格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_GetUnitsParSecond(LNHandle sound, int* frequency);

	//-----------------------------------------------------------------------------
	///**
	//  @brief		3D 音源かを調べる
	//  @param[in]  sound		: サウンドハンドル
	//  @param[out] enabled		: 3D 音源かを示す値を格納する変数
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_Is3DSound(LNHandle sound, LNBool* enabled);

	//-----------------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての位置を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  position	: 3D 空間上の座標
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterPosition(LNHandle sound, const LNVector3* position);

	//-----------------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての位置を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  x			: 3D 空間上の X 座標
	//  @param[in]  y			: 3D 空間上の Y 座標
	//  @param[in]  z			: 3D 空間上の Z 座標
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterPositionXYZ(LNHandle sound, float x, float y, float z);

	//-----------------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての速度を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  velocity	: 速度
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterVelocity(LNHandle sound, const LNVector3* velocity);

	//-----------------------------------------------------------------------------
	///**
	//  @brief		3D 音源としての速度を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  x			: 速度の X 成分
	//  @param[in]  y			: 速度の Y 成分
	//  @param[in]  z			: 速度の Z 成分
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterVelocityXYZ(LNHandle sound, float x, float y, float z);

	//-----------------------------------------------------------------------------
	///**
	//  @brief		3D 音源の減衰距離 (聴こえなくなる距離) を設定する
	//  @param[in]  sound		: サウンドハンドル
	//  @param[in]  distance	: 距離
	//*/
	//-----------------------------------------------------------------------------
	LNOTEAPI LNResult LNSound_SetEmitterDistance(LNHandle sound, float distance);

/** @} */

} // extern "C"

/** @} */	// defgroup
#endif

#endif
