#include "LuminoRuby.h"
#include "RubyStructs.h"

//-----------------------------------------------------------------------------
// WrapStructs


struct wrapConfig
{

    wrapConfig()
    {}
};

struct wrapEngine
{

    wrapEngine()
    {}
};

struct wrapVersion
{

    wrapVersion()
    {}
};

struct wrapDiag
{

    wrapDiag()
    {}
};

struct wrapGameAudio
{

    wrapGameAudio()
    {}
};

struct wrapSoundListener
{

    wrapSoundListener()
    {}
};

struct wrapSound
    : public wrapRefObject
{

    wrapSound()
    {}
};

struct wrapTexture
    : public wrapRefObject
{

    wrapTexture()
    {}
};

struct wrapTexture2D
    : public wrapTexture
{

    wrapTexture2D()
    {}
};

struct wrapViewportLayer
    : public wrapRefObject
{

    wrapViewportLayer()
    {}
};

struct wrapViewport
    : public wrapRefObject
{
    VALUE MainViewport;
    VALUE Layers;

    wrapViewport()
    :     MainViewport(Qnil), Layers(Qnil)
    {}
};

struct wrapSceneNode
    : public wrapRefObject
{
    VALUE Children;

    wrapSceneNode()
    :     Children(Qnil)
    {}
};

struct wrapSprite
    : public wrapSceneNode
{
    VALUE Texture;

    wrapSprite()
    :     Texture(Qnil)
    {}
};

struct wrapSprite2D
    : public wrapSprite
{

    wrapSprite2D()
    {}
};

struct wrapUIFrameWindow
    : public wrapRefObject
{

    wrapUIFrameWindow()
    {}
};

struct wrapUINativeHostWindow
    : public wrapUIFrameWindow
{

    wrapUINativeHostWindow()
    {}
};

struct wrapViewportLayerList
    : public wrapRefObject
{

    wrapViewportLayerList()
    {}
};

struct wrapSceneNodeList
    : public wrapRefObject
{

    wrapSceneNodeList()
    {}
};



VALUE g_class_Config;
VALUE g_class_Engine;
VALUE g_class_Version;
VALUE g_class_Diag;
VALUE g_class_GameAudio;
VALUE g_class_SoundListener;
VALUE g_class_Sound;
VALUE g_class_Texture;
VALUE g_class_Texture2D;
VALUE g_class_ViewportLayer;
VALUE g_class_Viewport;
VALUE g_class_SceneNode;
VALUE g_class_Sprite;
VALUE g_class_Sprite2D;
VALUE g_class_UIFrameWindow;
VALUE g_class_UINativeHostWindow;
VALUE g_class_ViewportLayerList;
VALUE g_class_SceneNodeList;


static VALUE static_lnrbLNConfig_SetGraphicsRenderingType(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE renderingType;
        rb_scan_args(argc, argv, "1", &renderingType);
        if (isRbNumber(renderingType)) {
            LNGraphicsRenderingType _renderingType = (LNGraphicsRenderingType)FIX2INT(renderingType);
            LNConfig_SetGraphicsRenderingType(_renderingType);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_graphics_rendering_type - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetEngineLogEnabled(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            LNBool _enabled = RbBooltoBool(enabled);
            LNConfig_SetEngineLogEnabled(_enabled);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_engine_log_enabled - wrong argument type.");
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
            intptr_t _windowHandle = FIX2INT(windowHandle);
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
        VALUE objectCount;
        VALUE memorySize;
        rb_scan_args(argc, argv, "2", &objectCount, &memorySize);
        if (isRbNumber(objectCount) && isRbNumber(memorySize)) {
            int _objectCount = FIX2INT(objectCount);
            int _memorySize = FIX2INT(memorySize);
            LNConfig_SetSoundCacheSize(_objectCount, _memorySize);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_sound_cache_size - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetDirectMusicMode(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mode;
        rb_scan_args(argc, argv, "1", &mode);
        if (isRbNumber(mode)) {
            LNDirectMusicMode _mode = (LNDirectMusicMode)FIX2INT(mode);
            LNConfig_SetDirectMusicMode(_mode);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_direct_music_mode - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNConfig_SetDirectMusicReverbLevel(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE level;
        rb_scan_args(argc, argv, "1", &level);
        if (isRbFloat(level)) {
            float _level = ((float)NUM2DBL(level));
            LNConfig_SetDirectMusicReverbLevel(_level);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Config.set_direct_music_reverb_level - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNEngine_Initialize(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNEngine_Initialize();
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Engine.initialize - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNEngine_InitializeAudio(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNEngine_InitializeAudio();
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Engine.initialize_audio - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNEngine_UpdateFrame(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNBool _outRequested;
            LNResult errorCode = LNEngine_UpdateFrame(&_outRequested);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return toVALUE(_outRequested);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Engine.update_frame - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNEngine_IsEndRequested(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNBool _outRequested;
            LNResult errorCode = LNEngine_IsEndRequested(&_outRequested);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return toVALUE(_outRequested);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Engine.end_requested? - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNEngine_Terminate(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNEngine_Terminate();
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Engine.terminate - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVersion_GetMajor(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outMajor;
            LNVersion_GetMajor(&_outMajor);
            return toVALUE(_outMajor);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Version.get_major - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVersion_GetMinor(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outMinor;
            LNVersion_GetMinor(&_outMinor);
            return toVALUE(_outMinor);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Version.get_minor - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVersion_GetRevision(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outRevision;
            LNVersion_GetRevision(&_outRevision);
            return toVALUE(_outRevision);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Version.get_revision - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVersion_GetBuild(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outBuild;
            LNVersion_GetBuild(&_outBuild);
            return toVALUE(_outBuild);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Version.get_build - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVersion_GetString(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            const LNChar* _outStr;
            LNVersion_GetString(&_outStr);
            return toVALUE(_outStr);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Version.get_string - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVersion_IsAtLeast(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE major;
        VALUE minor;
        VALUE revision;
        rb_scan_args(argc, argv, "3", &major, &minor, &revision);
        if (isRbNumber(major) && isRbNumber(minor) && isRbNumber(revision)) {
            int _major = FIX2INT(major);
            int _minor = FIX2INT(minor);
            int _revision = FIX2INT(revision);
            LNBool _outResult;
            LNVersion_IsAtLeast(_major, _minor, _revision, &_outResult);
            return toVALUE(_outResult);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Version.at_least? - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNDiag_GetHandleCount(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            int _outCount;
            LNResult errorCode = LNDiag_GetHandleCount(&_outCount);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return toVALUE(_outCount);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Diag.get_handle_count - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_PlayBGM(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "13", &filePath, &volume, &pitch, &fadeTime);
        if (isRbString(filePath) && isRbFloat(volume) && isRbFloat(pitch) && isRbFloat(fadeTime)) {
            char* _filePath = StringValuePtr(filePath);
            float _volume = (volume != Qnil) ? ((float)NUM2DBL(volume)) : 1.0f;
            float _pitch = (pitch != Qnil) ? ((float)NUM2DBL(pitch)) : 1.0f;
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNResult errorCode = LNGameAudio_PlayBGM(_filePath, _volume, _pitch, _fadeTime);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.play_bgm - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_StopBGM(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 1) {
        VALUE fadeTime;
        rb_scan_args(argc, argv, "01", &fadeTime);
        if (isRbFloat(fadeTime)) {
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNResult errorCode = LNGameAudio_StopBGM(_fadeTime);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.stop_bgm - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_PlayBGS(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "13", &filePath, &volume, &pitch, &fadeTime);
        if (isRbString(filePath) && isRbFloat(volume) && isRbFloat(pitch) && isRbFloat(fadeTime)) {
            char* _filePath = StringValuePtr(filePath);
            float _volume = (volume != Qnil) ? ((float)NUM2DBL(volume)) : 1.0f;
            float _pitch = (pitch != Qnil) ? ((float)NUM2DBL(pitch)) : 1.0f;
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNResult errorCode = LNGameAudio_PlayBGS(_filePath, _volume, _pitch, _fadeTime);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.play_bgs - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_StopBGS(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 1) {
        VALUE fadeTime;
        rb_scan_args(argc, argv, "01", &fadeTime);
        if (isRbFloat(fadeTime)) {
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNResult errorCode = LNGameAudio_StopBGS(_fadeTime);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.stop_bgs - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_PlayME(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "12", &filePath, &volume, &pitch);
        if (isRbString(filePath) && isRbFloat(volume) && isRbFloat(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            float _volume = (volume != Qnil) ? ((float)NUM2DBL(volume)) : 1.0f;
            float _pitch = (pitch != Qnil) ? ((float)NUM2DBL(pitch)) : 1.0f;
            LNResult errorCode = LNGameAudio_PlayME(_filePath, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.play_me - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_StopME(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNGameAudio_StopME();
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.stop_me - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_PlaySE(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "12", &filePath, &volume, &pitch);
        if (isRbString(filePath) && isRbFloat(volume) && isRbFloat(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            float _volume = (volume != Qnil) ? ((float)NUM2DBL(volume)) : 1.0f;
            float _pitch = (pitch != Qnil) ? ((float)NUM2DBL(pitch)) : 1.0f;
            LNResult errorCode = LNGameAudio_PlaySE(_filePath, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.play_se - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_PlaySE3D(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 5) {
        VALUE filePath;
        VALUE position;
        VALUE distance;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "32", &filePath, &position, &distance, &volume, &pitch);
        if (isRbString(filePath) && isRbObject(position) && isRbFloat(distance) && isRbFloat(volume) && isRbFloat(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            LNVector3* tmp__position; Data_Get_Struct(position, LNVector3, tmp__position);LNVector3& _position = *tmp__position;
            float _distance = ((float)NUM2DBL(distance));
            float _volume = (volume != Qnil) ? ((float)NUM2DBL(volume)) : 1.0f;
            float _pitch = (pitch != Qnil) ? ((float)NUM2DBL(pitch)) : 1.0f;
            LNResult errorCode = LNGameAudio_PlaySE3D(_filePath, &_position, _distance, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
        if (isRbString(filePath) && isRbFloat(x) && isRbFloat(y) && isRbFloat(z) && isRbFloat(distance) && isRbFloat(volume) && isRbFloat(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            float _distance = ((float)NUM2DBL(distance));
            float _volume = (volume != Qnil) ? ((float)NUM2DBL(volume)) : 1.0f;
            float _pitch = (pitch != Qnil) ? ((float)NUM2DBL(pitch)) : 1.0f;
            LNResult errorCode = LNGameAudio_PlaySE3DXYZ(_filePath, _x, _y, _z, _distance, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.play_se3d - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_StopSE(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNGameAudio_StopSE();
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.stop_se - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_SetBGMVolume(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 2) {
        VALUE volume;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "11", &volume, &fadeTime);
        if (isRbFloat(volume) && isRbFloat(fadeTime)) {
            float _volume = ((float)NUM2DBL(volume));
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNResult errorCode = LNGameAudio_SetBGMVolume(_volume, _fadeTime);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.set_bgm_volume - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNGameAudio_SetBGSVolume(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 2) {
        VALUE volume;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "11", &volume, &fadeTime);
        if (isRbFloat(volume) && isRbFloat(fadeTime)) {
            float _volume = ((float)NUM2DBL(volume));
            double _fadeTime = (fadeTime != Qnil) ? NUM2DBL(fadeTime) : 0.0;
            LNResult errorCode = LNGameAudio_SetBGSVolume(_volume, _fadeTime);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::GameAudio.set_bgs_volume - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNSoundListener_SetPosition(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE position;
        rb_scan_args(argc, argv, "1", &position);
        if (isRbObject(position)) {
            LNVector3* tmp__position; Data_Get_Struct(position, LNVector3, tmp__position);LNVector3& _position = *tmp__position;
            LNResult errorCode = LNSoundListener_SetPosition(&_position);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSoundListener_SetDirection(&_direction);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSoundListener_SetDirectionXYZ(_x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSoundListener_SetUpDirection(&_direction);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSoundListener_SetUpDirectionXYZ(_x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSoundListener_SetVelocity(&_velocity);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSoundListener_SetVelocityXYZ(_x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSoundListener_SetPositionXYZ(_x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SoundListener.set_position_xyz - wrong argument type.");
    return Qnil;
}

static void LNSound_delete(wrapSound* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNSound_mark(wrapSound* obj)
{

}

static VALUE LNSound_allocate( VALUE klass )
{
    VALUE obj;
    wrapSound* internalObj;

    internalObj = new wrapSound();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSound_allocate" );
    obj = Data_Wrap_Struct(klass, LNSound_mark, LNSound_delete, internalObj);

    return obj;
}

static VALUE LNSound_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapSound* internalObj;

    internalObj = new wrapSound();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSound_allocate" );
    obj = Data_Wrap_Struct(klass, LNSound_mark, LNSound_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
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
            LNResult errorCode = LNSound_Create(_filePath, &selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            Manager::RegisterWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.sound - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE volume;
        rb_scan_args(argc, argv, "1", &volume);
        if (isRbFloat(volume)) {
            float _volume = ((float)NUM2DBL(volume));
            LNResult errorCode = LNSound_SetVolume(selfObj->Handle, _volume);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            float _outVolume;
            LNResult errorCode = LNSound_GetVolume(selfObj->Handle, &_outVolume);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
        if (isRbFloat(pitch)) {
            float _pitch = ((float)NUM2DBL(pitch));
            LNResult errorCode = LNSound_SetPitch(selfObj->Handle, _pitch);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            float _outPitch;
            LNResult errorCode = LNSound_GetPitch(selfObj->Handle, &_outPitch);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_SetLoopEnabled(selfObj->Handle, _loopEnable);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_IsLoopEnabled(selfObj->Handle, &_outEnabled);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return toVALUE(_outEnabled);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.loop_enabled? - wrong argument type.");
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
            LNResult errorCode = LNSound_Set3DEnabled(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.is_3d_enabled= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Is3DEnabled(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNBool _outEnabled;
            LNResult errorCode = LNSound_Is3DEnabled(selfObj->Handle, &_outEnabled);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_SetPlayingMode(selfObj->Handle, _mode);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_GetPlayingMode(selfObj->Handle, &_outMode);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_GetPlayingState(selfObj->Handle, &_outState);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_GetPlayedSamples(selfObj->Handle, &_outSamples);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_GetTotalSamples(selfObj->Handle, &_outSamples);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_GetSamplingRate(selfObj->Handle, &_outRate);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_SetEmitterPosition(selfObj->Handle, &_position);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_SetEmitterPositionXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_SetEmitterVelocity(selfObj->Handle, &_velocity);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_SetEmitterVelocityXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_SetEmitterMaxDistance(selfObj->Handle, _distance);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.emitter_max_distance= - wrong argument type.");
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
            LNResult errorCode = LNSound_SetLoopRange(selfObj->Handle, _begin, _length);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.set_loop_range - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Play(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNSound_Play(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_Stop(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_Pause(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
            LNResult errorCode = LNSound_Resume(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
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
        if (isRbFloat(targetVolume) && isRbFloat(time) && isRbNumber(behavior)) {
            float _targetVolume = ((float)NUM2DBL(targetVolume));
            double _time = NUM2DBL(time);
            LNSoundFadeBehavior _behavior = (LNSoundFadeBehavior)FIX2INT(behavior);
            LNResult errorCode = LNSound_FadeVolume(selfObj->Handle, _targetVolume, _time, _behavior);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sound.fade_volume - wrong argument type.");
    return Qnil;
}

static void LNTexture_delete(wrapTexture* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNTexture_mark(wrapTexture* obj)
{

}

static VALUE LNTexture_allocate( VALUE klass )
{
    VALUE obj;
    wrapTexture* internalObj;

    internalObj = new wrapTexture();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNTexture_allocate" );
    obj = Data_Wrap_Struct(klass, LNTexture_mark, LNTexture_delete, internalObj);

    return obj;
}

static VALUE LNTexture_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapTexture* internalObj;

    internalObj = new wrapTexture();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNTexture_allocate" );
    obj = Data_Wrap_Struct(klass, LNTexture_mark, LNTexture_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNTexture_GetSize(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNSize _outSize;
            LNResult errorCode = LNTexture_GetSize(selfObj->Handle, &_outSize);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            VALUE retObj = LNSize_allocate(g_struct_Size);
    *((LNSize*)DATA_PTR(retObj)) = _outSize;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Texture.size - wrong argument type.");
    return Qnil;
}

static void LNTexture2D_delete(wrapTexture2D* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNTexture2D_mark(wrapTexture2D* obj)
{

}

static VALUE LNTexture2D_allocate( VALUE klass )
{
    VALUE obj;
    wrapTexture2D* internalObj;

    internalObj = new wrapTexture2D();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNTexture2D_allocate" );
    obj = Data_Wrap_Struct(klass, LNTexture2D_mark, LNTexture2D_delete, internalObj);

    return obj;
}

static VALUE LNTexture2D_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapTexture2D* internalObj;

    internalObj = new wrapTexture2D();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNTexture2D_allocate" );
    obj = Data_Wrap_Struct(klass, LNTexture2D_mark, LNTexture2D_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNTexture2D_Create(int argc, VALUE *argv, VALUE self)
{
    wrapTexture2D* selfObj;
    Data_Get_Struct(self, wrapTexture2D, selfObj);
    if (2 <= argc && argc <= 4) {
        VALUE width;
        VALUE height;
        VALUE format;
        VALUE mipmap;
        rb_scan_args(argc, argv, "22", &width, &height, &format, &mipmap);
        if (isRbNumber(width) && isRbNumber(height) && isRbNumber(format) && isRbBool(mipmap)) {
            int _width = FIX2INT(width);
            int _height = FIX2INT(height);
            LNTextureFormat _format = (format != Qnil) ? (LNTextureFormat)FIX2INT(format) : LN_FMT_A8R8G8B8;
            LNBool _mipmap = (mipmap != Qnil) ? RbBooltoBool(mipmap) : LN_FALSE;
            LNResult errorCode = LNTexture2D_Create(_width, _height, _format, _mipmap, &selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            Manager::RegisterWrapperObject(self);
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            LNResult errorCode = LNTexture2D_CreateFromFile(_filePath, &selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            Manager::RegisterWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Texture2D.texture_2d - wrong argument type.");
    return Qnil;
}

static void LNViewportLayer_delete(wrapViewportLayer* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNViewportLayer_mark(wrapViewportLayer* obj)
{

}

static VALUE LNViewportLayer_allocate( VALUE klass )
{
    VALUE obj;
    wrapViewportLayer* internalObj;

    internalObj = new wrapViewportLayer();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNViewportLayer_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewportLayer_mark, LNViewportLayer_delete, internalObj);

    return obj;
}

static VALUE LNViewportLayer_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapViewportLayer* internalObj;

    internalObj = new wrapViewportLayer();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNViewportLayer_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewportLayer_mark, LNViewportLayer_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static void LNViewport_delete(wrapViewport* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNViewport_mark(wrapViewport* obj)
{
    rb_gc_mark(obj->MainViewport);
    rb_gc_mark(obj->Layers);

}

static VALUE LNViewport_allocate( VALUE klass )
{
    VALUE obj;
    wrapViewport* internalObj;

    internalObj = new wrapViewport();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNViewport_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewport_mark, LNViewport_delete, internalObj);

    return obj;
}

static VALUE LNViewport_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapViewport* internalObj;

    internalObj = new wrapViewport();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNViewport_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewport_mark, LNViewport_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static VALUE static_lnrbLNViewport_GetMainViewport(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNHandle _outViewport;
            LNResult errorCode = LNViewport_GetMainViewport(&_outViewport);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            if (!checkEqualHandle(selfObj->MainViewport, _outViewport)) {
                selfObj->MainViewport = Manager::GetWrapperObjectFromHandle(_outViewport);
            }
            return selfObj->MainViewport;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Viewport.main_viewport - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNViewport_GetLayers(int argc, VALUE *argv, VALUE self)
{
    wrapViewport* selfObj;
    Data_Get_Struct(self, wrapViewport, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNHandle _outList;
            LNResult errorCode = LNViewport_GetLayers(selfObj->Handle, &_outList);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            if (!checkEqualHandle(selfObj->Layers, _outList)) {
                selfObj->Layers = Manager::GetWrapperObjectFromHandle(_outList);
            }
            return selfObj->Layers;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Viewport.layers - wrong argument type.");
    return Qnil;
}

static void LNSceneNode_delete(wrapSceneNode* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNSceneNode_mark(wrapSceneNode* obj)
{
    rb_gc_mark(obj->Children);

}

static VALUE LNSceneNode_allocate( VALUE klass )
{
    VALUE obj;
    wrapSceneNode* internalObj;

    internalObj = new wrapSceneNode();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSceneNode_allocate" );
    obj = Data_Wrap_Struct(klass, LNSceneNode_mark, LNSceneNode_delete, internalObj);

    return obj;
}

static VALUE LNSceneNode_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapSceneNode* internalObj;

    internalObj = new wrapSceneNode();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSceneNode_allocate" );
    obj = Data_Wrap_Struct(klass, LNSceneNode_mark, LNSceneNode_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSceneNode_SetVisible(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE visible;
        rb_scan_args(argc, argv, "1", &visible);
        if (isRbBool(visible)) {
            LNBool _visible = RbBooltoBool(visible);
            LNResult errorCode = LNSceneNode_SetVisible(selfObj->Handle, _visible);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SceneNode.visible= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_IsVisible(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNBool _outVisible;
            LNResult errorCode = LNSceneNode_IsVisible(selfObj->Handle, &_outVisible);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return toVALUE(_outVisible);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::SceneNode.visible? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetPosition(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE position;
        rb_scan_args(argc, argv, "1", &position);
        if (isRbObject(position)) {
            LNVector3* tmp__position; Data_Get_Struct(position, LNVector3, tmp__position);LNVector3& _position = *tmp__position;
            LNResult errorCode = LNSceneNode_SetPosition(selfObj->Handle, &_position);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::SceneNode.position= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetPosition(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNVector3 _outPosition;
            LNResult errorCode = LNSceneNode_GetPosition(selfObj->Handle, &_outPosition);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outPosition;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::SceneNode.position - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetChildren(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNHandle _outList;
            LNResult errorCode = LNSceneNode_GetChildren(selfObj->Handle, &_outList);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            if (!checkEqualHandle(selfObj->Children, _outList)) {
                selfObj->Children = Manager::GetWrapperObjectFromHandle(_outList);
            }
            return selfObj->Children;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::SceneNode.children - wrong argument type.");
    return Qnil;
}

static void LNSprite_delete(wrapSprite* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNSprite_mark(wrapSprite* obj)
{
    rb_gc_mark(obj->Texture);

}

static VALUE LNSprite_allocate( VALUE klass )
{
    VALUE obj;
    wrapSprite* internalObj;

    internalObj = new wrapSprite();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSprite_allocate" );
    obj = Data_Wrap_Struct(klass, LNSprite_mark, LNSprite_delete, internalObj);

    return obj;
}

static VALUE LNSprite_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapSprite* internalObj;

    internalObj = new wrapSprite();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSprite_allocate" );
    obj = Data_Wrap_Struct(klass, LNSprite_mark, LNSprite_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSprite_SetTexture(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE texture;
        rb_scan_args(argc, argv, "1", &texture);
        if (isRbObject(texture)) {
            LNHandle _texture = Manager::GetHandleFromtWrapperObject(texture);
            LNResult errorCode = LNSprite_SetTexture(selfObj->Handle, _texture);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            selfObj->Texture = texture;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sprite.texture= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_GetTexture(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNHandle _outTexture;
            LNResult errorCode = LNSprite_GetTexture(selfObj->Handle, &_outTexture);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            if (!checkEqualHandle(selfObj->Texture, _outTexture)) {
                selfObj->Texture = Manager::GetWrapperObjectFromHandle(_outTexture);
            }
            return selfObj->Texture;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sprite.texture - wrong argument type.");
    return Qnil;
}

static void LNSprite2D_delete(wrapSprite2D* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNSprite2D_mark(wrapSprite2D* obj)
{

}

static VALUE LNSprite2D_allocate( VALUE klass )
{
    VALUE obj;
    wrapSprite2D* internalObj;

    internalObj = new wrapSprite2D();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSprite2D_allocate" );
    obj = Data_Wrap_Struct(klass, LNSprite2D_mark, LNSprite2D_delete, internalObj);

    return obj;
}

static VALUE LNSprite2D_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapSprite2D* internalObj;

    internalObj = new wrapSprite2D();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSprite2D_allocate" );
    obj = Data_Wrap_Struct(klass, LNSprite2D_mark, LNSprite2D_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSprite2D_Create(int argc, VALUE *argv, VALUE self)
{
    wrapSprite2D* selfObj;
    Data_Get_Struct(self, wrapSprite2D, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNSprite2D_Create(&selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            Manager::RegisterWrapperObject(self);
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 1) {
        VALUE texture;
        rb_scan_args(argc, argv, "1", &texture);
        if (isRbObject(texture)) {
            LNHandle _texture = Manager::GetHandleFromtWrapperObject(texture);
            LNResult errorCode = LNSprite2D_CreateFromTexture(_texture, &selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            Manager::RegisterWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Sprite2D.sprite_2d - wrong argument type.");
    return Qnil;
}

static void LNUIFrameWindow_delete(wrapUIFrameWindow* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNUIFrameWindow_mark(wrapUIFrameWindow* obj)
{

}

static VALUE LNUIFrameWindow_allocate( VALUE klass )
{
    VALUE obj;
    wrapUIFrameWindow* internalObj;

    internalObj = new wrapUIFrameWindow();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNUIFrameWindow_allocate" );
    obj = Data_Wrap_Struct(klass, LNUIFrameWindow_mark, LNUIFrameWindow_delete, internalObj);

    return obj;
}

static VALUE LNUIFrameWindow_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapUIFrameWindow* internalObj;

    internalObj = new wrapUIFrameWindow();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNUIFrameWindow_allocate" );
    obj = Data_Wrap_Struct(klass, LNUIFrameWindow_mark, LNUIFrameWindow_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static void LNUINativeHostWindow_delete(wrapUINativeHostWindow* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNUINativeHostWindow_mark(wrapUINativeHostWindow* obj)
{

}

static VALUE LNUINativeHostWindow_allocate( VALUE klass )
{
    VALUE obj;
    wrapUINativeHostWindow* internalObj;

    internalObj = new wrapUINativeHostWindow();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNUINativeHostWindow_allocate" );
    obj = Data_Wrap_Struct(klass, LNUINativeHostWindow_mark, LNUINativeHostWindow_delete, internalObj);

    return obj;
}

static VALUE LNUINativeHostWindow_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapUINativeHostWindow* internalObj;

    internalObj = new wrapUINativeHostWindow();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNUINativeHostWindow_allocate" );
    obj = Data_Wrap_Struct(klass, LNUINativeHostWindow_mark, LNUINativeHostWindow_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNUINativeHostWindow_Create(int argc, VALUE *argv, VALUE self)
{
    wrapUINativeHostWindow* selfObj;
    Data_Get_Struct(self, wrapUINativeHostWindow, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE windowHandle;
        rb_scan_args(argc, argv, "1", &windowHandle);
        if (isRbNumber(windowHandle)) {
            intptr_t _windowHandle = FIX2INT(windowHandle);
            LNResult errorCode = LNUINativeHostWindow_Create(_windowHandle, &selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            Manager::RegisterWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::UINativeHostWindow.ui_native_host_window - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNUINativeHostWindow_Render(int argc, VALUE *argv, VALUE self)
{
    wrapUINativeHostWindow* selfObj;
    Data_Get_Struct(self, wrapUINativeHostWindow, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNUINativeHostWindow_Render(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNGetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::UINativeHostWindow.render - wrong argument type.");
    return Qnil;
}

static void LNViewportLayerList_delete(wrapViewportLayerList* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNViewportLayerList_mark(wrapViewportLayerList* obj)
{

}

static VALUE LNViewportLayerList_allocate( VALUE klass )
{
    VALUE obj;
    wrapViewportLayerList* internalObj;

    internalObj = new wrapViewportLayerList();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNViewportLayerList_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewportLayerList_mark, LNViewportLayerList_delete, internalObj);

    return obj;
}

static VALUE LNViewportLayerList_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapViewportLayerList* internalObj;

    internalObj = new wrapViewportLayerList();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNViewportLayerList_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewportLayerList_mark, LNViewportLayerList_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}

static void LNSceneNodeList_delete(wrapSceneNodeList* obj)
{
    if (obj->Handle != 0) LNObject_Release(obj->Handle);
    Manager::UnregisterWrapperObject(obj->Handle);
    delete obj;
}

static void LNSceneNodeList_mark(wrapSceneNodeList* obj)
{

}

static VALUE LNSceneNodeList_allocate( VALUE klass )
{
    VALUE obj;
    wrapSceneNodeList* internalObj;

    internalObj = new wrapSceneNodeList();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSceneNodeList_allocate" );
    obj = Data_Wrap_Struct(klass, LNSceneNodeList_mark, LNSceneNodeList_delete, internalObj);

    return obj;
}

static VALUE LNSceneNodeList_allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    wrapSceneNodeList* internalObj;

    internalObj = new wrapSceneNodeList();
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNSceneNodeList_allocate" );
    obj = Data_Wrap_Struct(klass, LNSceneNodeList_mark, LNSceneNodeList_delete, internalObj);
    
    internalObj->Handle = handle;
    return obj;
}



void InitClasses()
{
    g_class_Config = rb_define_class_under(g_luminoModule, "Config", rb_cObject);
    rb_define_singleton_method(g_class_Config, "set_graphics_rendering_type", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetGraphicsRenderingType), -1);
    rb_define_singleton_method(g_class_Config, "set_engine_log_enabled", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetEngineLogEnabled), -1);
    rb_define_singleton_method(g_class_Config, "register_archive", LN_TO_RUBY_FUNC(static_lnrbLNConfig_RegisterArchive), -1);
    rb_define_singleton_method(g_class_Config, "set_file_access_priority", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetFileAccessPriority), -1);
    rb_define_singleton_method(g_class_Config, "set_user_window_handle", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetUserWindowHandle), -1);
    rb_define_singleton_method(g_class_Config, "set_sound_cache_size", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetSoundCacheSize), -1);
    rb_define_singleton_method(g_class_Config, "set_direct_music_mode", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetDirectMusicMode), -1);
    rb_define_singleton_method(g_class_Config, "set_direct_music_reverb_level", LN_TO_RUBY_FUNC(static_lnrbLNConfig_SetDirectMusicReverbLevel), -1);

    g_class_Engine = rb_define_class_under(g_luminoModule, "Engine", rb_cObject);
    rb_define_singleton_method(g_class_Engine, "initialize", LN_TO_RUBY_FUNC(static_lnrbLNEngine_Initialize), -1);
    rb_define_singleton_method(g_class_Engine, "initialize_audio", LN_TO_RUBY_FUNC(static_lnrbLNEngine_InitializeAudio), -1);
    rb_define_singleton_method(g_class_Engine, "update_frame", LN_TO_RUBY_FUNC(static_lnrbLNEngine_UpdateFrame), -1);
    rb_define_singleton_method(g_class_Engine, "end_requested?", LN_TO_RUBY_FUNC(static_lnrbLNEngine_IsEndRequested), -1);
    rb_define_singleton_method(g_class_Engine, "terminate", LN_TO_RUBY_FUNC(static_lnrbLNEngine_Terminate), -1);

    g_class_Version = rb_define_class_under(g_luminoModule, "Version", rb_cObject);
    rb_define_singleton_method(g_class_Version, "get_major", LN_TO_RUBY_FUNC(static_lnrbLNVersion_GetMajor), -1);
    rb_define_singleton_method(g_class_Version, "get_minor", LN_TO_RUBY_FUNC(static_lnrbLNVersion_GetMinor), -1);
    rb_define_singleton_method(g_class_Version, "get_revision", LN_TO_RUBY_FUNC(static_lnrbLNVersion_GetRevision), -1);
    rb_define_singleton_method(g_class_Version, "get_build", LN_TO_RUBY_FUNC(static_lnrbLNVersion_GetBuild), -1);
    rb_define_singleton_method(g_class_Version, "get_string", LN_TO_RUBY_FUNC(static_lnrbLNVersion_GetString), -1);
    rb_define_singleton_method(g_class_Version, "at_least?", LN_TO_RUBY_FUNC(static_lnrbLNVersion_IsAtLeast), -1);

    g_class_Diag = rb_define_class_under(g_luminoModule, "Diag", rb_cObject);
    rb_define_singleton_method(g_class_Diag, "get_handle_count", LN_TO_RUBY_FUNC(static_lnrbLNDiag_GetHandleCount), -1);

    g_class_GameAudio = rb_define_class_under(g_luminoModule, "GameAudio", rb_cObject);
    rb_define_singleton_method(g_class_GameAudio, "play_bgm", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_PlayBGM), -1);
    rb_define_singleton_method(g_class_GameAudio, "stop_bgm", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_StopBGM), -1);
    rb_define_singleton_method(g_class_GameAudio, "play_bgs", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_PlayBGS), -1);
    rb_define_singleton_method(g_class_GameAudio, "stop_bgs", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_StopBGS), -1);
    rb_define_singleton_method(g_class_GameAudio, "play_me", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_PlayME), -1);
    rb_define_singleton_method(g_class_GameAudio, "stop_me", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_StopME), -1);
    rb_define_singleton_method(g_class_GameAudio, "play_se", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_PlaySE), -1);
    rb_define_singleton_method(g_class_GameAudio, "play_se3d", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_PlaySE3D), -1);
    rb_define_singleton_method(g_class_GameAudio, "stop_se", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_StopSE), -1);
    rb_define_singleton_method(g_class_GameAudio, "set_bgm_volume", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_SetBGMVolume), -1);
    rb_define_singleton_method(g_class_GameAudio, "set_bgs_volume", LN_TO_RUBY_FUNC(static_lnrbLNGameAudio_SetBGSVolume), -1);

    g_class_SoundListener = rb_define_class_under(g_luminoModule, "SoundListener", rb_cObject);
    rb_define_singleton_method(g_class_SoundListener, "position=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetPosition), -1);
    rb_define_singleton_method(g_class_SoundListener, "direction=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetDirection), -1);
    rb_define_singleton_method(g_class_SoundListener, "up_direction=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetUpDirection), -1);
    rb_define_singleton_method(g_class_SoundListener, "velocity=", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetVelocity), -1);
    rb_define_singleton_method(g_class_SoundListener, "set_position_xyz", LN_TO_RUBY_FUNC(static_lnrbLNSoundListener_SetPositionXYZ), -1);

    g_class_Sound = rb_define_class_under(g_luminoModule, "Sound", g_class_RefObject);
    rb_define_alloc_func(g_class_Sound, LNSound_allocate);
    rb_define_private_method(g_class_Sound, "initialize", LN_TO_RUBY_FUNC(lnrbLNSound_Create), -1);
    rb_define_method(g_class_Sound, "volume=", LN_TO_RUBY_FUNC(lnrbLNSound_SetVolume), -1);
    rb_define_method(g_class_Sound, "volume", LN_TO_RUBY_FUNC(lnrbLNSound_GetVolume), -1);
    rb_define_method(g_class_Sound, "pitch=", LN_TO_RUBY_FUNC(lnrbLNSound_SetPitch), -1);
    rb_define_method(g_class_Sound, "pitch", LN_TO_RUBY_FUNC(lnrbLNSound_GetPitch), -1);
    rb_define_method(g_class_Sound, "loop_enabled=", LN_TO_RUBY_FUNC(lnrbLNSound_SetLoopEnabled), -1);
    rb_define_method(g_class_Sound, "loop_enabled?", LN_TO_RUBY_FUNC(lnrbLNSound_IsLoopEnabled), -1);
    rb_define_method(g_class_Sound, "is_3d_enabled=", LN_TO_RUBY_FUNC(lnrbLNSound_Set3DEnabled), -1);
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
    rb_define_method(g_class_Sound, "set_loop_range", LN_TO_RUBY_FUNC(lnrbLNSound_SetLoopRange), -1);
    rb_define_method(g_class_Sound, "play", LN_TO_RUBY_FUNC(lnrbLNSound_Play), -1);
    rb_define_method(g_class_Sound, "stop", LN_TO_RUBY_FUNC(lnrbLNSound_Stop), -1);
    rb_define_method(g_class_Sound, "pause", LN_TO_RUBY_FUNC(lnrbLNSound_Pause), -1);
    rb_define_method(g_class_Sound, "resume", LN_TO_RUBY_FUNC(lnrbLNSound_Resume), -1);
    rb_define_method(g_class_Sound, "fade_volume", LN_TO_RUBY_FUNC(lnrbLNSound_FadeVolume), -1);

    g_class_Texture = rb_define_class_under(g_luminoModule, "Texture", g_class_RefObject);
    rb_define_alloc_func(g_class_Texture, LNTexture_allocate);
    rb_define_method(g_class_Texture, "size", LN_TO_RUBY_FUNC(lnrbLNTexture_GetSize), -1);

    g_class_Texture2D = rb_define_class_under(g_luminoModule, "Texture2D", g_class_Texture);
    rb_define_alloc_func(g_class_Texture2D, LNTexture2D_allocate);
    rb_define_private_method(g_class_Texture2D, "initialize", LN_TO_RUBY_FUNC(lnrbLNTexture2D_Create), -1);

    g_class_ViewportLayer = rb_define_class_under(g_luminoModule, "ViewportLayer", g_class_RefObject);
    rb_define_alloc_func(g_class_ViewportLayer, LNViewportLayer_allocate);

    g_class_Viewport = rb_define_class_under(g_luminoModule, "Viewport", g_class_RefObject);
    rb_define_alloc_func(g_class_Viewport, LNViewport_allocate);
    rb_define_singleton_method(g_class_Viewport, "main_viewport", LN_TO_RUBY_FUNC(static_lnrbLNViewport_GetMainViewport), -1);
    rb_define_method(g_class_Viewport, "layers", LN_TO_RUBY_FUNC(lnrbLNViewport_GetLayers), -1);

    g_class_SceneNode = rb_define_class_under(g_luminoModule, "SceneNode", g_class_RefObject);
    rb_define_alloc_func(g_class_SceneNode, LNSceneNode_allocate);
    rb_define_method(g_class_SceneNode, "visible=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetVisible), -1);
    rb_define_method(g_class_SceneNode, "visible?", LN_TO_RUBY_FUNC(lnrbLNSceneNode_IsVisible), -1);
    rb_define_method(g_class_SceneNode, "position=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetPosition), -1);
    rb_define_method(g_class_SceneNode, "position", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetPosition), -1);
    rb_define_method(g_class_SceneNode, "children", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetChildren), -1);

    g_class_Sprite = rb_define_class_under(g_luminoModule, "Sprite", g_class_SceneNode);
    rb_define_alloc_func(g_class_Sprite, LNSprite_allocate);
    rb_define_method(g_class_Sprite, "texture=", LN_TO_RUBY_FUNC(lnrbLNSprite_SetTexture), -1);
    rb_define_method(g_class_Sprite, "texture", LN_TO_RUBY_FUNC(lnrbLNSprite_GetTexture), -1);

    g_class_Sprite2D = rb_define_class_under(g_luminoModule, "Sprite2D", g_class_Sprite);
    rb_define_alloc_func(g_class_Sprite2D, LNSprite2D_allocate);
    rb_define_private_method(g_class_Sprite2D, "initialize", LN_TO_RUBY_FUNC(lnrbLNSprite2D_Create), -1);

    g_class_UIFrameWindow = rb_define_class_under(g_luminoModule, "UIFrameWindow", g_class_RefObject);
    rb_define_alloc_func(g_class_UIFrameWindow, LNUIFrameWindow_allocate);

    g_class_UINativeHostWindow = rb_define_class_under(g_luminoModule, "UINativeHostWindow", g_class_UIFrameWindow);
    rb_define_alloc_func(g_class_UINativeHostWindow, LNUINativeHostWindow_allocate);
    rb_define_private_method(g_class_UINativeHostWindow, "initialize", LN_TO_RUBY_FUNC(lnrbLNUINativeHostWindow_Create), -1);
    rb_define_method(g_class_UINativeHostWindow, "render", LN_TO_RUBY_FUNC(lnrbLNUINativeHostWindow_Render), -1);

    g_class_ViewportLayerList = rb_define_class_under(g_luminoModule, "ViewportLayerList", g_class_ObjectList);
    rb_define_alloc_func(g_class_ViewportLayerList, LNViewportLayerList_allocate);

    g_class_SceneNodeList = rb_define_class_under(g_luminoModule, "SceneNodeList", g_class_ObjectList);
    rb_define_alloc_func(g_class_SceneNodeList, LNSceneNodeList_allocate);


}

//-----------------------------------------------------------------------------
// TypeInfo

void Manager::RegisterTypeInfo()
{
    LNRB_REGISTER_TYPEINFO(Sound);
    LNRB_REGISTER_TYPEINFO(Texture);
    LNRB_REGISTER_TYPEINFO(Texture2D);
    LNRB_REGISTER_TYPEINFO(ViewportLayer);
    LNRB_REGISTER_TYPEINFO(Viewport);
    LNRB_REGISTER_TYPEINFO(SceneNode);
    LNRB_REGISTER_TYPEINFO(Sprite);
    LNRB_REGISTER_TYPEINFO(Sprite2D);
    LNRB_REGISTER_TYPEINFO(UIFrameWindow);
    LNRB_REGISTER_TYPEINFO(UINativeHostWindow);
    LNRB_REGISTER_TYPEINFO(ViewportLayerList);
    LNRB_REGISTER_TYPEINFO(SceneNodeList);

}

