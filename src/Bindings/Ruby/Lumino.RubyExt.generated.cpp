
#include <ruby.h>

#pragma once

#if !defined(LN_API)
    #if defined(_WIN32)
        #define LN_API __declspec(dllexport)
    #elif defined(_WIN32) && defined(LUMINO_DLL)
        #define LN_API __declspec(dllimport)
    #elif defined(__GNUC__) && defined(LUMINO_BUILD_DLL)
        #define LN_API __attribute__((visibility("default")))
    #else
        #define LN_API
    #endif
#endif


#define LNI_FUNC_TRY_BEGIN    try {
#define LNI_FUNC_TRY_END_RETURN    } \
    catch (ln::Exception& e) { \
        return ln::detail::RuntimeManager::procException(&e); \
    } \
    catch (...) { \
        return LN_ERROR_UNKNOWN; \
    } \
	return LN_SUCCESS;

#define LNI_BOOL_TO_LNBOOL(x)    (x) ? LN_TRUE : LN_FALSE
#define LNI_LNBOOL_TO_BOOL(x)    (x != LN_FALSE)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** 結果・エラーコード */
typedef enum tagLnResult
{
    /** 成功 */
	LN_SUCCESS = 0,

    /** 不明なエラー */
    LN_ERROR_UNKNOWN = -1,

} LnResult;

/** 真偽値 */
typedef enum tagLnBool
{
    /** 偽 */
    LN_FALSE = 0,

    /** 真 */
    LN_TRUE = 1,

} LnBool;

inline const char* LnRuntime_GetLastErrorMessage() { return ""; }  // TODO

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus




#define LN_TO_RUBY_FUNC(f) reinterpret_cast<VALUE(__cdecl *)(...)>(f)

inline VALUE LNI_TO_RUBY_VALUE(const char* v)   { return rb_str_new2(v); }
inline VALUE LNI_TO_RUBY_VALUE(LnBool v)        { return (v != LN_FALSE) ? Qtrue : Qfalse; }
inline VALUE LNI_TO_RUBY_VALUE(uint8_t v)       { return INT2FIX(v); }
inline VALUE LNI_TO_RUBY_VALUE(int v)           { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(uint32_t v)      { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(int64_t v)       { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(uint64_t v)      { return INT2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(float v)         { return DBL2NUM(v); }
inline VALUE LNI_TO_RUBY_VALUE(double v)        { return DBL2NUM(v); }

extern "C" LN_API LnResult LnEngine_Initialize();
extern "C" LN_API LnResult LnEngine_Finalize();
extern "C" LN_API LnResult LnEngine_Update(LnBool* outReturn);
extern "C" LN_API LnResult LnTexture2D_Init(LnHandle texture2d, int width, int height);
extern "C" LN_API LnResult LnTexture2D_Init(LnHandle texture2d, int width, int height, LNTextureFormat format);


VALUE g_enum_PixelFormat;
VALUE g_enum_TextureFormat;
VALUE g_enum_DepthBufferFormat;

VALUE g_rootModule;
VALUE g_class_Engine;
VALUE g_class_GraphicsResource;
VALUE g_class_Texture;
VALUE g_class_Texture2D;


//==============================================================================
// ln::Engine

struct Wrap_Engine
{
    Wrap_Engine()
    {}
};

static VALUE Wrap_LnEngine_Initialize(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
        {

            LnResult errorCode = LnEngine_Initialize();
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::initialize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_Finalize(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
        {

            LnResult errorCode = LnEngine_Finalize();
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::finalize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_Update(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
        {
            LnBool _outReturn;
            LnResult errorCode = LnEngine_Update(&_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::update - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::GraphicsResource

struct Wrap_GraphicsResource
    : public Wrap_GraphicsResource
{
    Wrap_GraphicsResource()
    {}
};

//==============================================================================
// ln::Texture

struct Wrap_Texture
    : public Wrap_Texture
{
    Wrap_Texture()
    {}
};

//==============================================================================
// ln::Texture2D

struct Wrap_Texture2D
    : public Wrap_Texture2D
{
    Wrap_Texture2D()
    {}
};

static VALUE Wrap_LnTexture2D_Init(int argc, VALUE* argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE texture2d;
        VALUE width;
        VALUE height;

        &texture2d, &width, &height
        {

            LnResult errorCode = LnTexture2D_Init();
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (4 <= argc && argc <= 4) {
        VALUE texture2d;
        VALUE width;
        VALUE height;
        VALUE format;

        &texture2d, &width, &height, &format
        {

            LnResult errorCode = LnTexture2D_Init();
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Texture2D::init - wrong argument type.");
    return Qnil;
}



extern "C" void Init_Lumino()
{
    g_rootModule = rb_define_module("Lumino");

    g_enum_PixelFormat = rb_define_module_under(g_rootModule, "PixelFormat");
    rb_define_const(g_enum_PixelFormat, "Unknown", INT2FIX(0)); 
    rb_define_const(g_enum_PixelFormat, "A8", INT2FIX(1)); 
    rb_define_const(g_enum_PixelFormat, "RGBA8", INT2FIX(2)); 
    rb_define_const(g_enum_PixelFormat, "RGB8", INT2FIX(3)); 
    rb_define_const(g_enum_PixelFormat, "RGBA32F", INT2FIX(4)); 

    g_enum_TextureFormat = rb_define_module_under(g_rootModule, "TextureFormat");
    rb_define_const(g_enum_TextureFormat, "Unknown", INT2FIX(0)); 
    rb_define_const(g_enum_TextureFormat, "RGBA8", INT2FIX(1)); 
    rb_define_const(g_enum_TextureFormat, "RGB8", INT2FIX(2)); 
    rb_define_const(g_enum_TextureFormat, "RGBA16F", INT2FIX(3)); 
    rb_define_const(g_enum_TextureFormat, "RGBA32F", INT2FIX(4)); 
    rb_define_const(g_enum_TextureFormat, "R16F", INT2FIX(5)); 
    rb_define_const(g_enum_TextureFormat, "R32F", INT2FIX(6)); 
    rb_define_const(g_enum_TextureFormat, "R32U", INT2FIX(7)); 

    g_enum_DepthBufferFormat = rb_define_module_under(g_rootModule, "DepthBufferFormat");
    rb_define_const(g_enum_DepthBufferFormat, "D24S8", INT2FIX(0));

    g_class_Engine = rb_define_class_under(g_rootModule, "Engine", rb_cObject);
    rb_define_singleton_method(g_class_Engine, "initialize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Initialize), -1);
    rb_define_singleton_method(g_class_Engine, "finalize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Finalize), -1);
    rb_define_singleton_method(g_class_Engine, "update", LN_TO_RUBY_FUNC(Wrap_LnEngine_Update), -1);

    g_class_GraphicsResource = rb_define_class_under(g_rootModule, "GraphicsResource", rb_cObject);

    g_class_Texture = rb_define_class_under(g_rootModule, "Texture", rb_cObject);

    g_class_Texture2D = rb_define_class_under(g_rootModule, "Texture2D", rb_cObject);
    rb_define_singleton_method(g_class_Texture2D, "init", LN_TO_RUBY_FUNC(Wrap_LnTexture2D_Init), -1);

}


#if 0
#include "LuminoRuby.h"
#include "RubyStructs.h"

//-----------------------------------------------------------------------------
// WrapStructs


__WRAP_STRUCTS__

__GLOBALS__

__FUNCTIONS__

void InitClasses()
{
__DEFINES__
}

//-----------------------------------------------------------------------------
// TypeInfo

void Manager::RegisterTypeInfo()
{
	// dummy
	TypeInfo t;
	t.klass = Qnil;
	t.factory = NULL;
	m_typeInfoList.push_back(t);
	
__TYPEINFO_REGISTERS__
}
#endif

