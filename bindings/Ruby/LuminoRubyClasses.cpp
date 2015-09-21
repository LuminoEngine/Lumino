#include "LuminoRuby.h"
#include "RubyStructs.h"

struct wrapReferenceObject
{
	LNHandle	Handle;
};

//-----------------------------------------------------------------------------
// Utils

static LNHandle RbRefObjToHandle(VALUE v)
{
    if (v == Qnil) {
        return NULL;    
    }
	wrapReferenceObject* obj;
	Data_Get_Struct(v, wrapReferenceObject, obj);
	return obj->Handle;
}

bool checkEqualHandle(VALUE obj, LNHandle handle)
{
	if (obj == Qnil)
		return false;
	if (((wrapReferenceObject*)DATA_PTR(obj))->Handle != handle)
		return false;
	return true;
}

//-----------------------------------------------------------------------------
// WrapStructs


struct wrapConfig
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



VALUE g_class_Config;
VALUE g_class_Application;
VALUE g_class_Audio;
VALUE g_class_SoundListener;
VALUE g_class_Sound;


static VALUE static_lnrbLNConfig_SetApplicationLogEnabled(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            LNBool _enabled = RbBooltoBool(enabled);
            LNConfig_SetApplicationLogEnabled(_enabled);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_application_log_enabled - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetConsoleEnabled(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            LNBool _enabled = RbBooltoBool(enabled);
            LNConfig_SetConsoleEnabled(_enabled);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_console_enabled - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_RegisterArchive(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE filePath;
        VALUE password;
        rb_scan_args(argc, argv, "2", &filePath, &password);
        if (isRbString(filePath) && isRbString(password)) {
            char* _filePath = StringValuePtr(filePath);
            char* _password = StringValuePtr(password);
            LNConfig_RegisterArchive(_filePath, _password);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.register_archive - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetFileAccessPriority(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE priority;
        rb_scan_args(argc, argv, "1", &priority);
        if (isRbNumber(priority)) {
            LNFileAccessPriority _priority = (LNFileAccessPriority)FIX2INT(priority);
            LNConfig_SetFileAccessPriority(_priority);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_file_access_priority - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetUserWindowHandle(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE windowHandle;
        rb_scan_args(argc, argv, "1", &windowHandle);
        if (isRbNumber(windowHandle)) {
            void* _windowHandle = ((void*)FIX2INT(windowHandle));
            LNConfig_SetUserWindowHandle(_windowHandle);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_user_window_handle - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetSoundCacheSize(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE count;
        VALUE memorySize;
        rb_scan_args(argc, argv, "2", &count, &memorySize);
        if (isRbNumber(count) && isRbNumber(memorySize)) {
            int _count = FIX2INT(count);
            int _memorySize = FIX2INT(memorySize);
            LNConfig_SetSoundCacheSize(_count, _memorySize);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_sound_cache_size - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetDirectMusicInitializeMode(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mode;
        rb_scan_args(argc, argv, "1", &mode);
        if (isRbNumber(mode)) {
            LNDirectMusicMode _mode = (LNDirectMusicMode)FIX2INT(mode);
            LNConfig_SetDirectMusicInitializeMode(_mode);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_direct_music_initialize_mode - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetDirectMusicReverbLevel(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE level;
        rb_scan_args(argc, argv, "1", &level);
        if (isRbNumber(level)) {
            int _level = FIX2INT(level);
            LNConfig_SetDirectMusicReverbLevel(_level);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_direct_music_reverb_level - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_Initialize(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNApplication_Initialize();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Application.initialize - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_InitializeAudio(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNApplication_InitializeAudio();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Application.initialize_audio - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_Update(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNApplication_Update();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Application.update - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_ResetFrameDelay(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNApplication_ResetFrameDelay();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Application.reset_frame_delay - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_IsEndRequested(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNBool _requested;
            LNApplication_IsEndRequested(&_requested);
            return toVALUE(_requested);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Application.end_requested? - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNApplication_Finalize(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNApplication_Finalize();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Application.finalize - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayBGM(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "13", &filePath, &volume, &pitch, &fadeTime);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch) && isRbFloat(fadeTime)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNAudio_PlayBGM(_filePath, _volume, _pitch, _fadeTime);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.play_bgm - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopBGM(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 1) {
        VALUE fadeTime;
        rb_scan_args(argc, argv, "01", &fadeTime);
        if (isRbFloat(fadeTime)) {
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNAudio_StopBGM(_fadeTime);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.stop_bgm - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayBGS(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "13", &filePath, &volume, &pitch, &fadeTime);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch) && isRbFloat(fadeTime)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNAudio_PlayBGS(_filePath, _volume, _pitch, _fadeTime);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.play_bgs - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopBGS(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 1) {
        VALUE fadeTime;
        rb_scan_args(argc, argv, "01", &fadeTime);
        if (isRbFloat(fadeTime)) {
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNAudio_StopBGS(_fadeTime);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.stop_bgs - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlayME(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "12", &filePath, &volume, &pitch);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            LNAudio_PlayME(_filePath, _volume, _pitch);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.play_me - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopME(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNAudio_StopME();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.stop_me - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlaySE(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "12", &filePath, &volume, &pitch);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            LNAudio_PlaySE(_filePath, _volume, _pitch);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.play_se - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_PlaySE3D(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 5) {
        VALUE filePath;
        VALUE position;
        VALUE distance;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "32", &filePath, &position, &distance, &volume, &pitch);
        if (isRbString(filePath) && isRbObject(position) && isRbFloat(distance) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            LNVector3* tmp__position; Data_Get_Struct(position, LNVector3, tmp__position);LNVector3& _position = *tmp__position;
            float _distance = ((float)NUM2DBL(distance));
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            LNAudio_PlaySE3D(_filePath, &_position, _distance, _volume, _pitch);
            return Qnil;
        }
    }
    if (5 <= argc && argc <= 7) {
        VALUE filePath;
        VALUE x;
        VALUE y;
        VALUE z;
        VALUE distance;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "52", &filePath, &x, &y, &z, &distance, &volume, &pitch);
        if (isRbString(filePath) && isRbFloat(x) && isRbFloat(y) && isRbFloat(z) && isRbFloat(distance) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            float _distance = ((float)NUM2DBL(distance));
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            LNAudio_PlaySE3DXYZ(_filePath, _x, _y, _z, _distance, _volume, _pitch);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.play_se3d - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_StopSE(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNAudio_StopSE();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.stop_se - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_SetBGMVolume(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 2) {
        VALUE volume;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "11", &volume, &fadeTime);
        if (isRbNumber(volume) && isRbFloat(fadeTime)) {
            int _volume = FIX2INT(volume);
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNAudio_SetBGMVolume(_volume, _fadeTime);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.set_bgm_volume - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNAudio_SetBGSVolume(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 2) {
        VALUE volume;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "11", &volume, &fadeTime);
        if (isRbNumber(volume) && isRbFloat(fadeTime)) {
            int _volume = FIX2INT(volume);
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNAudio_SetBGSVolume(_volume, _fadeTime);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Audio.set_bgs_volume - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetPosition(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE position;
        rb_scan_args(argc, argv, "1", &position);
        if (isRbObject(position)) {
            LNVector3* tmp__position; Data_Get_Struct(position, LNVector3, tmp__position);LNVector3& _position = *tmp__position;
            LNSoundListener_SetPosition(&_position);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SoundListener.position= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetDirection(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE direction;
        rb_scan_args(argc, argv, "1", &direction);
        if (isRbObject(direction)) {
            LNVector3* tmp__direction; Data_Get_Struct(direction, LNVector3, tmp__direction);LNVector3& _direction = *tmp__direction;
            LNSoundListener_SetDirection(&_direction);
            return Qnil;
        }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNSoundListener_SetDirectionXYZ(_x, _y, _z);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SoundListener.direction= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetUpDirection(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE direction;
        rb_scan_args(argc, argv, "1", &direction);
        if (isRbObject(direction)) {
            LNVector3* tmp__direction; Data_Get_Struct(direction, LNVector3, tmp__direction);LNVector3& _direction = *tmp__direction;
            LNSoundListener_SetUpDirection(&_direction);
            return Qnil;
        }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNSoundListener_SetUpDirectionXYZ(_x, _y, _z);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SoundListener.up_direction= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetVelocity(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE velocity;
        rb_scan_args(argc, argv, "1", &velocity);
        if (isRbObject(velocity)) {
            LNVector3* tmp__velocity; Data_Get_Struct(velocity, LNVector3, tmp__velocity);LNVector3& _velocity = *tmp__velocity;
            LNSoundListener_SetVelocity(&_velocity);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SoundListener.velocity= - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetPositionXYZ(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNSoundListener_SetPositionXYZ(_x, _y, _z);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SoundListener.set_position_xyz - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetVelocityXYZ(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNSoundListener_SetVelocityXYZ(_x, _y, _z);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SoundListener.velocity - wrong argument type.");
    return Qnil;
}

static void LNSound_delete(wrapSound* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    free(obj);
}

static void LNSound_mark(wrapSound* obj)
{

}

static VALUE LNSound_allocate( VALUE klass )
{
    VALUE obj;
    wrapSound* internalObj;

    internalObj = (wrapSound*)malloc(sizeof(wrapSound));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSound_allocate" );
    obj = Data_Wrap_Struct(klass, LNSound_mark, LNSound_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSound));

    return obj;
}

static VALUE LNSound_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapSound* internalObj;

    internalObj = (wrapSound*)malloc(sizeof(wrapSound));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSound_allocate" );
    obj = Data_Wrap_Struct(klass, LNSound_mark, LNSound_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSound));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSound_SetVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE volume;
        rb_scan_args(argc, argv, "1", &volume);
        if (isRbNumber(volume)) {
            int _volume = FIX2INT(volume);
            LNSound_SetVolume(selfObj->Handle, _volume);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.volume= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outVolume;
            LNSound_GetVolume(selfObj->Handle, &_outVolume);
            return toVALUE(_outVolume);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.volume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetPitch(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE pitch;
        rb_scan_args(argc, argv, "1", &pitch);
        if (isRbNumber(pitch)) {
            int _pitch = FIX2INT(pitch);
            LNSound_SetPitch(selfObj->Handle, _pitch);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.pitch= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPitch(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outPitch;
            LNSound_GetPitch(selfObj->Handle, &_outPitch);
            return toVALUE(_outPitch);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.pitch - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetLoopEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE loopEnable;
        rb_scan_args(argc, argv, "1", &loopEnable);
        if (isRbBool(loopEnable)) {
            LNBool _loopEnable = RbBooltoBool(loopEnable);
            LNSound_SetLoopEnabled(selfObj->Handle, _loopEnable);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.loop_enabled= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_IsLoopEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNBool _outEnabled;
            LNSound_IsLoopEnabled(selfObj->Handle, &_outEnabled);
            return toVALUE(_outEnabled);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.loop_enabled? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetLoopRange(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE begin;
        VALUE length;
        rb_scan_args(argc, argv, "2", &begin, &length);
        if (isRbNumber(begin) && isRbNumber(length)) {
            int _begin = FIX2INT(begin);
            int _length = FIX2INT(length);
            LNSound_SetLoopRange(selfObj->Handle, _begin, _length);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.loop_range= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Set3DEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            LNBool _enabled = RbBooltoBool(enabled);
            LNSound_Set3DEnabled(selfObj->Handle, _enabled);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.3d_enabled= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Is3DEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNBool _outEnabled;
            LNSound_Is3DEnabled(selfObj->Handle, &_outEnabled);
            return toVALUE(_outEnabled);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.is_3d_enabled? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetPlayingMode(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE mode;
        rb_scan_args(argc, argv, "1", &mode);
        if (isRbNumber(mode)) {
            LNSoundPlayingMode _mode = (LNSoundPlayingMode)FIX2INT(mode);
            LNSound_SetPlayingMode(selfObj->Handle, _mode);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.playing_mode= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPlayingMode(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNSoundPlayingMode _outMode;
            LNSound_GetPlayingMode(selfObj->Handle, &_outMode);
            return INT2FIX(_outMode);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.playing_mode - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPlayingState(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNSoundPlayingState _outState;
            LNSound_GetPlayingState(selfObj->Handle, &_outState);
            return INT2FIX(_outState);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.playing_state - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPlayedSamples(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int64_t _outSamples;
            LNSound_GetPlayedSamples(selfObj->Handle, &_outSamples);
            return toVALUE(_outSamples);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.played_samples - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetTotalSamples(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int64_t _outSamples;
            LNSound_GetTotalSamples(selfObj->Handle, &_outSamples);
            return toVALUE(_outSamples);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.total_samples - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetSamplingRate(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outRate;
            LNSound_GetSamplingRate(selfObj->Handle, &_outRate);
            return toVALUE(_outRate);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.sampling_rate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterPosition(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE position;
        rb_scan_args(argc, argv, "1", &position);
        if (isRbObject(position)) {
            LNVector3* tmp__position; Data_Get_Struct(position, LNVector3, tmp__position);LNVector3& _position = *tmp__position;
            LNSound_SetEmitterPosition(selfObj->Handle, &_position);
            return Qnil;
        }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNSound_SetEmitterPositionXYZ(selfObj->Handle, _x, _y, _z);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.emitter_position= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterVelocity(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE velocity;
        rb_scan_args(argc, argv, "1", &velocity);
        if (isRbObject(velocity)) {
            LNVector3* tmp__velocity; Data_Get_Struct(velocity, LNVector3, tmp__velocity);LNVector3& _velocity = *tmp__velocity;
            LNSound_SetEmitterVelocity(selfObj->Handle, &_velocity);
            return Qnil;
        }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNSound_SetEmitterVelocityXYZ(selfObj->Handle, _x, _y, _z);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.emitter_velocity= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterMaxDistance(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE distance;
        rb_scan_args(argc, argv, "1", &distance);
        if (isRbFloat(distance)) {
            float _distance = ((float)NUM2DBL(distance));
            LNSound_SetEmitterMaxDistance(selfObj->Handle, _distance);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.emitter_max_distance= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Create(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            LNSound_Create(_filePath, &selfObj->Handle);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.sound - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Play(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNSound_Play(selfObj->Handle);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.play - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Stop(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNSound_Stop(selfObj->Handle);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.stop - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Pause(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNSound_Pause(selfObj->Handle);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.pause - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Resume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNSound_Resume(selfObj->Handle);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.resume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_FadeVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE targetVolume;
        VALUE time;
        VALUE behavior;
        rb_scan_args(argc, argv, "3", &targetVolume, &time, &behavior);
        if (isRbNumber(targetVolume) && isRbFloat(time) && isRbNumber(behavior)) {
            int _targetVolume = FIX2INT(targetVolume);
            double _time = NUM2DBL(time);
            LNSoundFadeBehavior _behavior = (LNSoundFadeBehavior)FIX2INT(behavior);
            LNSound_FadeVolume(selfObj->Handle, _targetVolume, _time, _behavior);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.fade_volume - wrong argument type.");
    return Qnil;
}



void InitClasses()
{
    g_class_Config = rb_define_class_under(g_luminoModule, "Config", rb_cObject);
    rb_define_singleton_method(g_class_Config, "set_application_log_enabled", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetApplicationLogEnabled), -1);
    rb_define_singleton_method(g_class_Config, "set_console_enabled", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetConsoleEnabled), -1);
    rb_define_singleton_method(g_class_Config, "register_archive", LN_TO_RUBY_FUNC(static_lnrbLNConfig_RegisterArchive), -1);
    rb_define_singleton_method(g_class_Config, "set_file_access_priority", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetFileAccessPriority), -1);
    rb_define_singleton_method(g_class_Config, "set_user_window_handle", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetUserWindowHandle), -1);
    rb_define_singleton_method(g_class_Config, "set_sound_cache_size", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetSoundCacheSize), -1);
    rb_define_singleton_method(g_class_Config, "set_direct_music_initialize_mode", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetDirectMusicInitializeMode), -1);
    rb_define_singleton_method(g_class_Config, "set_direct_music_reverb_level", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetDirectMusicReverbLevel), -1);

    g_class_Application = rb_define_class_under(g_luminoModule, "Application", rb_cObject);
    rb_define_singleton_method(g_class_Application, "initialize", LN_TO_RUBY_FUNC(static_lnrbLNApplication_Initialize), -1);
    rb_define_singleton_method(g_class_Application, "initialize_audio", LN_TO_RUBY_FUNC(static_lnrbLNApplication_InitializeAudio), -1);
    rb_define_singleton_method(g_class_Application, "update", LN_TO_RUBY_FUNC(static_lnrbLNApplication_Update), -1);
    rb_define_singleton_method(g_class_Application, "reset_frame_delay", LN_TO_RUBY_FUNC(static_lnrbLNApplication_ResetFrameDelay), -1);
    rb_define_singleton_method(g_class_Application, "end_requested?", LN_TO_RUBY_FUNC(static_lnrbLNApplication_IsEndRequested), -1);
    rb_define_singleton_method(g_class_Application, "finalize", LN_TO_RUBY_FUNC(static_lnrbLNApplication_Finalize), -1);

    g_class_Audio = rb_define_class_under(g_luminoModule, "Audio", rb_cObject);
    rb_define_singleton_method(g_class_Audio, "play_bgm", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayBGM), -1);
    rb_define_singleton_method(g_class_Audio, "stop_bgm", LN_TO_RUBY_FUNC(static_lnrbLNAudio_StopBGM), -1);
    rb_define_singleton_method(g_class_Audio, "play_bgs", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayBGS), -1);
    rb_define_singleton_method(g_class_Audio, "stop_bgs", LN_TO_RUBY_FUNC(static_lnrbLNAudio_StopBGS), -1);
    rb_define_singleton_method(g_class_Audio, "play_me", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlayME), -1);
    rb_define_singleton_method(g_class_Audio, "stop_me", LN_TO_RUBY_FUNC(static_lnrbLNAudio_StopME), -1);
    rb_define_singleton_method(g_class_Audio, "play_se", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlaySE), -1);
    rb_define_singleton_method(g_class_Audio, "play_se3d", LN_TO_RUBY_FUNC(static_lnrbLNAudio_PlaySE3D), -1);
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
    rb_define_method(g_class_Sound, "emitter_max_distance=", LN_TO_RUBY_FUNC(lnrbLNSound_SetEmitterMaxDistance), -1);
    rb_define_private_method(g_class_Sound, "initialize", LN_TO_RUBY_FUNC(lnrbLNSound_Create), -1);
    rb_define_method(g_class_Sound, "play", LN_TO_RUBY_FUNC(lnrbLNSound_Play), -1);
    rb_define_method(g_class_Sound, "stop", LN_TO_RUBY_FUNC(lnrbLNSound_Stop), -1);
    rb_define_method(g_class_Sound, "pause", LN_TO_RUBY_FUNC(lnrbLNSound_Pause), -1);
    rb_define_method(g_class_Sound, "resume", LN_TO_RUBY_FUNC(lnrbLNSound_Resume), -1);
    rb_define_method(g_class_Sound, "fade_volume", LN_TO_RUBY_FUNC(lnrbLNSound_FadeVolume), -1);


}

