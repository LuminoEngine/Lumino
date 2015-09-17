#include "LuminoRuby.h"
#include "RubyStructs.h"

struct wrapReferenceObject
{
	LNHandle	Handle;
};

struct wrapote
{

};

struct wrapApplication
{

};

struct wrapAudio
{

};

struct wrapSoundListener
{

};

struct wrapSound
    : public wrapReferenceObject
{

};



VALUE g_class_ote;
VALUE g_class_Application;
VALUE g_class_Audio;
VALUE g_class_SoundListener;
VALUE g_class_Sound;


static VALUE static_lnrbLNConfig_SetApplicationLogEnabled(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::ote.set_application_log_enabled - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetConsoleEnabled(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::ote.set_console_enabled - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetUserWindowHandle(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::ote.set_user_window_handle - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetSoundCacheSize(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::ote.set_sound_cache_size - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetDirectMusicInitializeMode(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::ote.set_direct_music_initialize_mode - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetDirectMusicReverbLevel(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::ote.set_direct_music_reverb_level - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_Initialize(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Application.initialize - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_InitializeAudio(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Application.initialize_audio - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_Update(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Application.update - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_ResetFrameDelay(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Application.reset_frame_delay - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_IsEndRequested(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Application.end_requested? - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_Finalize(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Application.finalize - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayBGM(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_bgm - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayBGMMem(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_bgm_mem - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopBGM(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.stop_bgm - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayBGS(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_bgs - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayBGSMem(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_bgs_mem - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopBGS(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.stop_bgs - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayME(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_me - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayMEMem(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_me_mem - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopME(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.stop_me - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlaySE(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_se - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlaySE3D(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_se3d - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlaySEMem(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_se_mem - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlaySE3DMem(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.play_se3d_mem - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopSE(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.stop_se - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_SetBGMVolume(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.set_bgm_volume - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_SetBGSVolume(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::Audio.set_bgs_volume - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetPosition(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::SoundListener.position= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetDirection(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::SoundListener.direction= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetUpDirection(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::SoundListener.up_direction= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetVelocity(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::SoundListener.velocity= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetPositionXYZ(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::SoundListener.set_position_xyz - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetVelocityXYZ(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "Lumino::SoundListener.velocity - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.volume= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.volume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetPitch(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.pitch= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPitch(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.pitch - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetLoopEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.loop_enabled= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_IsLoopEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.loop_enabled? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetLoopRange(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.loop_range= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Set3DEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.3d_enabled= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Is3DEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.is_3d_enabled? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetPlayingMode(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.playing_mode= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPlayingMode(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.playing_mode - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPlayingState(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.playing_state - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPlayedSamples(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.played_samples - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetTotalSamples(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.total_samples - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetSamplingRate(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.sampling_rate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterPosition(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.emitter_position= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterVelocity(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.emitter_velocity= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterDistance(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.emitter_distance= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Create(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.sound - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Play(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.play - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Stop(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.stop - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Pause(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.pause - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Resume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.resume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_FadeVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    rb_raise(rb_eArgError, "Lumino::Sound.fade_volume - wrong argument type.");
    return Qnil;
}



void InitClasses()
{
    g_class_ote = rb_define_class_under(g_luminoModule, "ote", rb_cObject);
    rb_define_singleton_method(g_class_ote, "set_application_log_enabled", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetApplicationLogEnabled), -1);
    rb_define_singleton_method(g_class_ote, "set_console_enabled", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetConsoleEnabled), -1);
    rb_define_singleton_method(g_class_ote, "set_user_window_handle", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetUserWindowHandle), -1);
    rb_define_singleton_method(g_class_ote, "set_sound_cache_size", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetSoundCacheSize), -1);
    rb_define_singleton_method(g_class_ote, "set_direct_music_initialize_mode", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetDirectMusicInitializeMode), -1);
    rb_define_singleton_method(g_class_ote, "set_direct_music_reverb_level", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetDirectMusicReverbLevel), -1);

    g_class_Application = rb_define_class_under(g_luminoModule, "Application", rb_cObject);
    rb_define_singleton_method(g_class_Application, "initialize", LN_TO_RUBY_FUNC(static_lnrbLNApplication_Initialize), -1);
    rb_define_singleton_method(g_class_Application, "initialize_audio", LN_TO_RUBY_FUNC(static_lnrbLNApplication_InitializeAudio), -1);
    rb_define_singleton_method(g_class_Application, "update", LN_TO_RUBY_FUNC(static_lnrbLNApplication_Update), -1);
    rb_define_singleton_method(g_class_Application, "reset_frame_delay", LN_TO_RUBY_FUNC(static_lnrbLNApplication_ResetFrameDelay), -1);
    rb_define_singleton_method(g_class_Application, "end_requested?", LN_TO_RUBY_FUNC(static_lnrbLNApplication_IsEndRequested), -1);
    rb_define_singleton_method(g_class_Application, "finalize", LN_TO_RUBY_FUNC(static_lnrbLNApplication_Finalize), -1);

    g_class_Audio = rb_define_class_under(g_luminoModule, "Audio", rb_cObject);
    rb_define_singleton_method(g_class_Audio, "play_bgm", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayBGM), -1);
    rb_define_singleton_method(g_class_Audio, "play_bgm_mem", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayBGMMem), -1);
    rb_define_singleton_method(g_class_Audio, "stop_bgm", LN_TO_RUBY_FUNC(static_lnrbLNAudio_StopBGM), -1);
    rb_define_singleton_method(g_class_Audio, "play_bgs", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayBGS), -1);
    rb_define_singleton_method(g_class_Audio, "play_bgs_mem", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayBGSMem), -1);
    rb_define_singleton_method(g_class_Audio, "stop_bgs", LN_TO_RUBY_FUNC(static_lnrbLNAudio_StopBGS), -1);
    rb_define_singleton_method(g_class_Audio, "play_me", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayME), -1);
    rb_define_singleton_method(g_class_Audio, "play_me_mem", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayMEMem), -1);
    rb_define_singleton_method(g_class_Audio, "stop_me", LN_TO_RUBY_FUNC(static_lnrbLNAudio_StopME), -1);
    rb_define_singleton_method(g_class_Audio, "play_se", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlaySE), -1);
    rb_define_singleton_method(g_class_Audio, "play_se3d", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlaySE3D), -1);
    rb_define_singleton_method(g_class_Audio, "play_se_mem", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlaySEMem), -1);
    rb_define_singleton_method(g_class_Audio, "play_se3d_mem", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlaySE3DMem), -1);
    rb_define_singleton_method(g_class_Audio, "stop_se", LN_TO_RUBY_FUNC(static_lnrbLNAudio_StopSE), -1);
    rb_define_singleton_method(g_class_Audio, "set_bgm_volume", LN_TO_RUBY_FUNC(static_lnrbLNAudio_SetBGMVolume), -1);
    rb_define_singleton_method(g_class_Audio, "set_bgs_volume", LN_TO_RUBY_FUNC(static_lnrbLNAudio_SetBGSVolume), -1);

    g_class_SoundListener = rb_define_class_under(g_luminoModule, "SoundListener", rb_cObject);
    rb_define_singleton_method(g_class_SoundListener, "position=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetPosition), -1);
    rb_define_singleton_method(g_class_SoundListener, "direction=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetDirection), -1);
    rb_define_singleton_method(g_class_SoundListener, "up_direction=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetUpDirection), -1);
    rb_define_singleton_method(g_class_SoundListener, "velocity=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetVelocity), -1);
    rb_define_singleton_method(g_class_SoundListener, "set_position_xyz", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetPositionXYZ), -1);
    rb_define_singleton_method(g_class_SoundListener, "velocity", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetVelocityXYZ), -1);

    g_class_Sound = rb_define_class_under(g_luminoModule, "Sound", rb_cObject);
    rb_define_alloc_func(g_class_Sound, LNSound_allocate);
    rb_define_method(g_class_Sound, "volume=", LN_TO_RUBY_FUNC(lnrbLNSound_SetVolume), -1);
    rb_define_method(g_class_Sound, "volume", LN_TO_RUBY_FUNC(lnrbLNSound_GetVolume), -1);
    rb_define_method(g_class_Sound, "pitch=", LN_TO_RUBY_FUNC(lnrbLNSound_SetPitch), -1);
    rb_define_method(g_class_Sound, "pitch", LN_TO_RUBY_FUNC(lnrbLNSound_GetPitch), -1);
    rb_define_method(g_class_Sound, "loop_enabled=", LN_TO_RUBY_FUNC(lnrbLNSound_SetLoopEnabled), -1);
    rb_define_method(g_class_Sound, "loop_enabled?", LN_TO_RUBY_FUNC(lnrbLNSound_IsLoopEnabled), -1);
    rb_define_method(g_class_Sound, "loop_range=", LN_TO_RUBY_FUNC(lnrbLNSound_SetLoopRange), -1);
    rb_define_method(g_class_Sound, "3d_enabled=", LN_TO_RUBY_FUNC(lnrbLNSound_Set3DEnabled), -1);
    rb_define_method(g_class_Sound, "is_3d_enabled?", LN_TO_RUBY_FUNC(lnrbLNSound_Is3DEnabled), -1);
    rb_define_method(g_class_Sound, "playing_mode=", LN_TO_RUBY_FUNC(lnrbLNSound_SetPlayingMode), -1);
    rb_define_method(g_class_Sound, "playing_mode", LN_TO_RUBY_FUNC(lnrbLNSound_GetPlayingMode), -1);
    rb_define_method(g_class_Sound, "playing_state", LN_TO_RUBY_FUNC(lnrbLNSound_GetPlayingState), -1);
    rb_define_method(g_class_Sound, "played_samples", LN_TO_RUBY_FUNC(lnrbLNSound_GetPlayedSamples), -1);
    rb_define_method(g_class_Sound, "total_samples", LN_TO_RUBY_FUNC(lnrbLNSound_GetTotalSamples), -1);
    rb_define_method(g_class_Sound, "sampling_rate", LN_TO_RUBY_FUNC(lnrbLNSound_GetSamplingRate), -1);
    rb_define_method(g_class_Sound, "emitter_position=", LN_TO_RUBY_FUNC(lnrbLNSound_SetEmitterPosition), -1);
    rb_define_method(g_class_Sound, "emitter_velocity=", LN_TO_RUBY_FUNC(lnrbLNSound_SetEmitterVelocity), -1);
    rb_define_method(g_class_Sound, "emitter_distance=", LN_TO_RUBY_FUNC(lnrbLNSound_SetEmitterDistance), -1);
    rb_define_private_method(g_class_Sound, "initialize", LN_TO_RUBY_FUNC(lnrbLNSound_Create), -1);
    rb_define_method(g_class_Sound, "play", LN_TO_RUBY_FUNC(lnrbLNSound_Play), -1);
    rb_define_method(g_class_Sound, "stop", LN_TO_RUBY_FUNC(lnrbLNSound_Stop), -1);
    rb_define_method(g_class_Sound, "pause", LN_TO_RUBY_FUNC(lnrbLNSound_Pause), -1);
    rb_define_method(g_class_Sound, "resume", LN_TO_RUBY_FUNC(lnrbLNSound_Resume), -1);
    rb_define_method(g_class_Sound, "fade_volume", LN_TO_RUBY_FUNC(lnrbLNSound_FadeVolume), -1);


}

