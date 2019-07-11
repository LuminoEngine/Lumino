
#include <ruby.h>
#include "LuminoRubyRuntimeManager.h"
#include <Lumino.FlatC.generated.h>



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

inline bool LNRB_VALUE_IS_OBJECT(VALUE v) { return (TYPE(v) == T_DATA) || (v == Qnil); }
inline bool LNRB_VALUE_IS_NUMBER(VALUE v) { return (TYPE(v) == T_FIXNUM || TYPE(v) == T_BIGNUM) || (v == Qnil); }
inline bool LNRB_VALUE_IS_FLOAT(VALUE v) { return (TYPE(v) == T_FLOAT) || (LNRB_VALUE_IS_NUMBER(v)) || (v == Qnil); }
inline bool LNRB_VALUE_IS_STRING(VALUE v) { return (TYPE(v) == T_STRING) || (v == Qnil); }
inline bool LNRB_VALUE_IS_BOOL(VALUE v) { return (TYPE(v) == T_FALSE || TYPE(v) == T_TRUE) || (v == Qnil); }

inline int LNRB_VALUE_TO_NUMBER(VALUE v) { return FIX2INT(v); }
inline float LNRB_VALUE_TO_FLOAT(VALUE v) { return (float)NUM2DBL(v); }
inline double LNRB_VALUE_TO_DOUBLE(VALUE v) { return NUM2DBL(v); }
inline const char* LNRB_VALUE_TO_STRING(VALUE v) { return StringValuePtr(v); }
inline LnBool LNRB_VALUE_TO_BOOL(VALUE v) { return (TYPE(v) == T_TRUE) ? LN_TRUE : LN_FALSE; }

extern "C" void InitLuminoRubyRuntimeManager();

extern "C" LN_FLAT_API LnResult LnEngine_Initialize();
extern "C" LN_FLAT_API LnResult LnEngine_Finalize();
extern "C" LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn);
extern "C" LN_FLAT_API void LnGraphicsResource_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnTexture_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D);
extern "C" LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D);
extern "C" LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D);
extern "C" LN_FLAT_API void LnTexture2D_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnWorldObject_Setposition(LnHandle worldobject, const LnVector3* pos);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetpositionXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_Position(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_Setrotation(LnHandle worldobject, const LnQuaternion* rot);
extern "C" LN_FLAT_API LnResult LnWorldObject_Seteulerangles(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_Rotation(LnHandle worldobject, LnQuaternion* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_Setscale(LnHandle worldobject, const LnVector3* scale);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetscaleS(LnHandle worldobject, float xyz);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetscaleXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_Scale(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_Setcenterpoint(LnHandle worldobject, const LnVector3* value);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetcenterpointXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_Centerpoint(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API void LnWorldObject_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnVisualObject_Setvisible(LnHandle visualobject, LnBool value);
extern "C" LN_FLAT_API LnResult LnVisualObject_Isvisible(LnHandle visualobject, LnBool* outReturn);
extern "C" LN_FLAT_API void LnVisualObject_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnSprite_Settexture(LnHandle sprite, LnHandle texture);
extern "C" LN_FLAT_API LnResult LnSprite_SetsourcerectXYWH(LnHandle sprite, float x, float y, float width, float height);
extern "C" LN_FLAT_API LnResult LnSprite_Create(LnHandle texture, float width, float height, LnHandle* outSprite);
extern "C" LN_FLAT_API void LnSprite_SetManagedTypeInfoId(int64_t id);


VALUE g_enum_PixelFormat;
VALUE g_enum_TextureFormat;
VALUE g_enum_DepthBufferFormat;

VALUE g_rootModule;
VALUE g_class_Engine;
VALUE g_class_GraphicsResource;
VALUE g_class_Texture;
VALUE g_class_Texture2D;
VALUE g_class_WorldObject;
VALUE g_class_VisualObject;
VALUE g_class_Sprite;


//==============================================================================
// ln::Vector3

VALUE g_class_Vector3;

void LnVector3_delete(LnVector3* obj)
{
    free(obj);
}

VALUE LnVector3_allocate( VALUE klass )
{
    VALUE obj;
    LnVector3* internalObj;

    internalObj = (LnVector3*)malloc(sizeof(LnVector3));
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVector3_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LnVector3_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LnVector3));

    return obj;
}

//==============================================================================
// ln::Quaternion

VALUE g_class_Quaternion;

void LnQuaternion_delete(LnQuaternion* obj)
{
    free(obj);
}

VALUE LnQuaternion_allocate( VALUE klass )
{
    VALUE obj;
    LnQuaternion* internalObj;

    internalObj = (LnQuaternion*)malloc(sizeof(LnQuaternion));
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnQuaternion_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LnQuaternion_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LnQuaternion));

    return obj;
}

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
    : public Wrap_Object
{
    Wrap_GraphicsResource()
    {}
};

static void LnGraphicsResource_delete(Wrap_GraphicsResource* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnGraphicsResource_mark(Wrap_GraphicsResource* obj)
{
}

static VALUE LnGraphicsResource_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_GraphicsResource* internalObj;

    internalObj = new Wrap_GraphicsResource();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnGraphicsResource_allocate");
    obj = Data_Wrap_Struct(klass, LnGraphicsResource_mark, LnGraphicsResource_delete, internalObj);

    return obj;
}

static VALUE LnGraphicsResource_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_GraphicsResource* internalObj;

    internalObj = new Wrap_GraphicsResource();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnGraphicsResource_allocate");
    obj = Data_Wrap_Struct(klass, LnGraphicsResource_mark, LnGraphicsResource_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

//==============================================================================
// ln::Texture

struct Wrap_Texture
    : public Wrap_GraphicsResource
{
    Wrap_Texture()
    {}
};

static void LnTexture_delete(Wrap_Texture* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnTexture_mark(Wrap_Texture* obj)
{
}

static VALUE LnTexture_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Texture* internalObj;

    internalObj = new Wrap_Texture();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture_mark, LnTexture_delete, internalObj);

    return obj;
}

static VALUE LnTexture_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Texture* internalObj;

    internalObj = new Wrap_Texture();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture_mark, LnTexture_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

//==============================================================================
// ln::Texture2D

struct Wrap_Texture2D
    : public Wrap_Texture
{
    Wrap_Texture2D()
    {}
};

static void LnTexture2D_delete(Wrap_Texture2D* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnTexture2D_mark(Wrap_Texture2D* obj)
{
}

static VALUE LnTexture2D_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Texture2D* internalObj;

    internalObj = new Wrap_Texture2D();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture2D_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture2D_mark, LnTexture2D_delete, internalObj);

    return obj;
}

static VALUE LnTexture2D_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Texture2D* internalObj;

    internalObj = new Wrap_Texture2D();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTexture2D_allocate");
    obj = Data_Wrap_Struct(klass, LnTexture2D_mark, LnTexture2D_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnTexture2D_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_Texture2D* selfObj;
    Data_Get_Struct(self, Wrap_Texture2D, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (LNRB_VALUE_IS_NUMBER(width) && LNRB_VALUE_IS_NUMBER(height))
        {
            int _width = LNRB_VALUE_TO_NUMBER(width);
            int _height = LNRB_VALUE_TO_NUMBER(height);
            LnResult errorCode = LnTexture2D_Create(_width, _height, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    if (3 <= argc && argc <= 3) {
        VALUE width;
        VALUE height;
        VALUE format;
        rb_scan_args(argc, argv, "3", &width, &height, &format);
        if (LNRB_VALUE_IS_NUMBER(width) && LNRB_VALUE_IS_NUMBER(height) && LNRB_VALUE_IS_NUMBER(format))
        {
            int _width = LNRB_VALUE_TO_NUMBER(width);
            int _height = LNRB_VALUE_TO_NUMBER(height);
            LnTextureFormat _format = (LnTextureFormat)FIX2INT(format);
            LnResult errorCode = LnTexture2D_CreateWithFormat(_width, _height, _format, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 2) {
        VALUE filePath;
        VALUE format;
        rb_scan_args(argc, argv, "2", &filePath, &format);
        if (LNRB_VALUE_IS_STRING(filePath) && LNRB_VALUE_IS_NUMBER(format))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnTextureFormat _format = (LnTextureFormat)FIX2INT(format);
            LnResult errorCode = LnTexture2D_CreateFromFileA(_filePath, _format, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Texture2D::init - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::WorldObject

struct Wrap_WorldObject
    : public Wrap_Object
{
    Wrap_WorldObject()
    {}
};

static void LnWorldObject_delete(Wrap_WorldObject* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnWorldObject_mark(Wrap_WorldObject* obj)
{
}

static VALUE LnWorldObject_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_WorldObject* internalObj;

    internalObj = new Wrap_WorldObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnWorldObject_allocate");
    obj = Data_Wrap_Struct(klass, LnWorldObject_mark, LnWorldObject_delete, internalObj);

    return obj;
}

static VALUE LnWorldObject_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_WorldObject* internalObj;

    internalObj = new Wrap_WorldObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnWorldObject_allocate");
    obj = Data_Wrap_Struct(klass, LnWorldObject_mark, LnWorldObject_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnWorldObject_Setposition(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE worldobject;
        VALUE pos;
        rb_scan_args(argc, argv, "2", &worldobject, &pos);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_OBJECT(pos))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnVector3* tmp__pos; Data_Get_Struct(pos, LnVector3, tmp__pos);LnVector3& _pos = *tmp__pos;
            LnResult errorCode = LnWorldObject_Setposition(_worldobject, &_pos);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (4 <= argc && argc <= 4) {
        VALUE worldobject;
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "4", &worldobject, &x, &y, &z);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnWorldObject_SetpositionXYZ(_worldobject, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setPosition - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Position(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE worldobject;
        rb_scan_args(argc, argv, "1", &worldobject);
        if (LNRB_VALUE_IS_OBJECT(worldobject))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_Position(_worldobject, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::position - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Setrotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE worldobject;
        VALUE rot;
        rb_scan_args(argc, argv, "2", &worldobject, &rot);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_OBJECT(rot))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnQuaternion* tmp__rot; Data_Get_Struct(rot, LnQuaternion, tmp__rot);LnQuaternion& _rot = *tmp__rot;
            LnResult errorCode = LnWorldObject_Setrotation(_worldobject, &_rot);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setRotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Seteulerangles(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (4 <= argc && argc <= 4) {
        VALUE worldobject;
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "4", &worldobject, &x, &y, &z);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnWorldObject_Seteulerangles(_worldobject, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setEulerAngles - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Rotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE worldobject;
        rb_scan_args(argc, argv, "1", &worldobject);
        if (LNRB_VALUE_IS_OBJECT(worldobject))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnQuaternion _outReturn;
            LnResult errorCode = LnWorldObject_Rotation(_worldobject, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnQuaternion_allocate(g_class_Quaternion);
            *((LnQuaternion*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::rotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Setscale(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE worldobject;
        VALUE scale;
        rb_scan_args(argc, argv, "2", &worldobject, &scale);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_OBJECT(scale))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnVector3* tmp__scale; Data_Get_Struct(scale, LnVector3, tmp__scale);LnVector3& _scale = *tmp__scale;
            LnResult errorCode = LnWorldObject_Setscale(_worldobject, &_scale);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 2) {
        VALUE worldobject;
        VALUE xyz;
        rb_scan_args(argc, argv, "2", &worldobject, &xyz);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_FLOAT(xyz))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            float _xyz = LNRB_VALUE_TO_FLOAT(xyz);
            LnResult errorCode = LnWorldObject_SetscaleS(_worldobject, _xyz);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (4 <= argc && argc <= 4) {
        VALUE worldobject;
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "4", &worldobject, &x, &y, &z);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnWorldObject_SetscaleXYZ(_worldobject, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setScale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Scale(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE worldobject;
        rb_scan_args(argc, argv, "1", &worldobject);
        if (LNRB_VALUE_IS_OBJECT(worldobject))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_Scale(_worldobject, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::scale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Setcenterpoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE worldobject;
        VALUE value;
        rb_scan_args(argc, argv, "2", &worldobject, &value);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_OBJECT(value))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnVector3* tmp__value; Data_Get_Struct(value, LnVector3, tmp__value);LnVector3& _value = *tmp__value;
            LnResult errorCode = LnWorldObject_Setcenterpoint(_worldobject, &_value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (4 <= argc && argc <= 4) {
        VALUE worldobject;
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "4", &worldobject, &x, &y, &z);
        if (LNRB_VALUE_IS_OBJECT(worldobject) && LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnWorldObject_SetcenterpointXYZ(_worldobject, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setCenterPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_Centerpoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE worldobject;
        rb_scan_args(argc, argv, "1", &worldobject);
        if (LNRB_VALUE_IS_OBJECT(worldobject))
        {
            LnHandle _worldobject = LuminoRubyRuntimeManager::instance->getHandle(worldobject);
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_Centerpoint(_worldobject, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::centerPoint - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::VisualObject

struct Wrap_VisualObject
    : public Wrap_WorldObject
{
    Wrap_VisualObject()
    {}
};

static void LnVisualObject_delete(Wrap_VisualObject* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnVisualObject_mark(Wrap_VisualObject* obj)
{
}

static VALUE LnVisualObject_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_VisualObject* internalObj;

    internalObj = new Wrap_VisualObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVisualObject_allocate");
    obj = Data_Wrap_Struct(klass, LnVisualObject_mark, LnVisualObject_delete, internalObj);

    return obj;
}

static VALUE LnVisualObject_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_VisualObject* internalObj;

    internalObj = new Wrap_VisualObject();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVisualObject_allocate");
    obj = Data_Wrap_Struct(klass, LnVisualObject_mark, LnVisualObject_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnVisualObject_Setvisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualObject* selfObj;
    Data_Get_Struct(self, Wrap_VisualObject, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE visualobject;
        VALUE value;
        rb_scan_args(argc, argv, "2", &visualobject, &value);
        if (LNRB_VALUE_IS_OBJECT(visualobject) && LNRB_VALUE_IS_BOOL(value))
        {
            LnHandle _visualobject = LuminoRubyRuntimeManager::instance->getHandle(visualobject);
            LnBool _value = LNRB_VALUE_TO_BOOL(value);
            LnResult errorCode = LnVisualObject_Setvisible(_visualobject, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::VisualObject::setVisible - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVisualObject_Isvisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualObject* selfObj;
    Data_Get_Struct(self, Wrap_VisualObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE visualobject;
        rb_scan_args(argc, argv, "1", &visualobject);
        if (LNRB_VALUE_IS_OBJECT(visualobject))
        {
            LnHandle _visualobject = LuminoRubyRuntimeManager::instance->getHandle(visualobject);
            LnBool _outReturn;
            LnResult errorCode = LnVisualObject_Isvisible(_visualobject, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::VisualObject::isVisible - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::Sprite

struct Wrap_Sprite
    : public Wrap_VisualObject
{
    Wrap_Sprite()
    {}
};

static void LnSprite_delete(Wrap_Sprite* obj)
{
    LuminoRubyRuntimeManager::instance->unregisterWrapperObject(obj->handle);
    delete obj;
}

static void LnSprite_mark(Wrap_Sprite* obj)
{
}

static VALUE LnSprite_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Sprite* internalObj;

    internalObj = new Wrap_Sprite();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSprite_allocate");
    obj = Data_Wrap_Struct(klass, LnSprite_mark, LnSprite_delete, internalObj);

    return obj;
}

static VALUE LnSprite_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Sprite* internalObj;

    internalObj = new Wrap_Sprite();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSprite_allocate");
    obj = Data_Wrap_Struct(klass, LnSprite_mark, LnSprite_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}

static VALUE Wrap_LnSprite_Settexture(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE sprite;
        VALUE texture;
        rb_scan_args(argc, argv, "2", &sprite, &texture);
        if (LNRB_VALUE_IS_OBJECT(sprite) && LNRB_VALUE_IS_OBJECT(texture))
        {
            LnHandle _sprite = LuminoRubyRuntimeManager::instance->getHandle(sprite);
            LnHandle _texture = LuminoRubyRuntimeManager::instance->getHandle(texture);
            LnResult errorCode = LnSprite_Settexture(_sprite, _texture);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::setTexture - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSprite_SetsourcerectXYWH(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (5 <= argc && argc <= 5) {
        VALUE sprite;
        VALUE x;
        VALUE y;
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "5", &sprite, &x, &y, &width, &height);
        if (LNRB_VALUE_IS_OBJECT(sprite) && LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(width) && LNRB_VALUE_IS_FLOAT(height))
        {
            LnHandle _sprite = LuminoRubyRuntimeManager::instance->getHandle(sprite);
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _width = LNRB_VALUE_TO_FLOAT(width);
            float _height = LNRB_VALUE_TO_FLOAT(height);
            LnResult errorCode = LnSprite_SetsourcerectXYWH(_sprite, _x, _y, _width, _height);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::setSourceRect - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSprite_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE texture;
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "3", &texture, &width, &height);
        if (LNRB_VALUE_IS_OBJECT(texture) && LNRB_VALUE_IS_FLOAT(width) && LNRB_VALUE_IS_FLOAT(height))
        {
            LnHandle _texture = LuminoRubyRuntimeManager::instance->getHandle(texture);
            float _width = LNRB_VALUE_TO_FLOAT(width);
            float _height = LNRB_VALUE_TO_FLOAT(height);
            LnResult errorCode = LnSprite_Create(_texture, _width, _height, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::init - wrong argument type.");
    return Qnil;
}



extern "C" void Init_Lumino()
{
    InitLuminoRubyRuntimeManager();
    g_rootModule = rb_define_module("Lumino");

    g_enum_PixelFormat = rb_define_module_under(g_rootModule, "PixelFormat");
    rb_define_const(g_enum_PixelFormat, "UNKNOWN", INT2FIX(0)); 
    rb_define_const(g_enum_PixelFormat, "A8", INT2FIX(1)); 
    rb_define_const(g_enum_PixelFormat, "RGBA8", INT2FIX(2)); 
    rb_define_const(g_enum_PixelFormat, "RGB8", INT2FIX(3)); 
    rb_define_const(g_enum_PixelFormat, "RGBA32F", INT2FIX(4)); 

    g_enum_TextureFormat = rb_define_module_under(g_rootModule, "TextureFormat");
    rb_define_const(g_enum_TextureFormat, "UNKNOWN", INT2FIX(0)); 
    rb_define_const(g_enum_TextureFormat, "RGBA8", INT2FIX(1)); 
    rb_define_const(g_enum_TextureFormat, "RGB8", INT2FIX(2)); 
    rb_define_const(g_enum_TextureFormat, "RGBA16F", INT2FIX(3)); 
    rb_define_const(g_enum_TextureFormat, "RGBA32F", INT2FIX(4)); 
    rb_define_const(g_enum_TextureFormat, "R16F", INT2FIX(5)); 
    rb_define_const(g_enum_TextureFormat, "R32F", INT2FIX(6)); 
    rb_define_const(g_enum_TextureFormat, "R32U", INT2FIX(7)); 

    g_enum_DepthBufferFormat = rb_define_module_under(g_rootModule, "DepthBufferFormat");
    rb_define_const(g_enum_DepthBufferFormat, "D24S8", INT2FIX(0));

    g_class_Vector3 = rb_define_class_under(g_rootModule, "Vector3", rb_cObject);
    rb_define_alloc_func(g_class_Vector3, LnVector3_allocate);

    g_class_Quaternion = rb_define_class_under(g_rootModule, "Quaternion", rb_cObject);
    rb_define_alloc_func(g_class_Quaternion, LnQuaternion_allocate);

    g_class_Engine = rb_define_class_under(g_rootModule, "Engine", rb_cObject);
    rb_define_singleton_method(g_class_Engine, "initialize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Initialize), -1);
    rb_define_singleton_method(g_class_Engine, "finalize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Finalize), -1);
    rb_define_singleton_method(g_class_Engine, "update", LN_TO_RUBY_FUNC(Wrap_LnEngine_Update), -1);

    g_class_GraphicsResource = rb_define_class_under(g_rootModule, "GraphicsResource", rb_cObject);
    rb_define_alloc_func(g_class_GraphicsResource, LnGraphicsResource_allocate);
    LnGraphicsResource_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_GraphicsResource, LnGraphicsResource_allocateForGetObject));

    g_class_Texture = rb_define_class_under(g_rootModule, "Texture", rb_cObject);
    rb_define_alloc_func(g_class_Texture, LnTexture_allocate);
    LnTexture_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Texture, LnTexture_allocateForGetObject));

    g_class_Texture2D = rb_define_class_under(g_rootModule, "Texture2D", rb_cObject);
    rb_define_alloc_func(g_class_Texture2D, LnTexture2D_allocate);
    rb_define_private_method(g_class_Texture2D, "initialize", LN_TO_RUBY_FUNC(Wrap_LnTexture2D_Create), -1);
    LnTexture2D_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Texture2D, LnTexture2D_allocateForGetObject));

    g_class_WorldObject = rb_define_class_under(g_rootModule, "WorldObject", rb_cObject);
    rb_define_alloc_func(g_class_WorldObject, LnWorldObject_allocate);
    rb_define_method(g_class_WorldObject, "set_position", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Setposition), -1);
    rb_define_method(g_class_WorldObject, "position", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Position), -1);
    rb_define_method(g_class_WorldObject, "set_rotation", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Setrotation), -1);
    rb_define_method(g_class_WorldObject, "set_euler_angles", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Seteulerangles), -1);
    rb_define_method(g_class_WorldObject, "rotation", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Rotation), -1);
    rb_define_method(g_class_WorldObject, "set_scale", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Setscale), -1);
    rb_define_method(g_class_WorldObject, "scale", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Scale), -1);
    rb_define_method(g_class_WorldObject, "set_center_point", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Setcenterpoint), -1);
    rb_define_method(g_class_WorldObject, "center_point", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_Centerpoint), -1);
    LnWorldObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_WorldObject, LnWorldObject_allocateForGetObject));

    g_class_VisualObject = rb_define_class_under(g_rootModule, "VisualObject", rb_cObject);
    rb_define_alloc_func(g_class_VisualObject, LnVisualObject_allocate);
    rb_define_method(g_class_VisualObject, "set_visible", LN_TO_RUBY_FUNC(Wrap_LnVisualObject_Setvisible), -1);
    rb_define_method(g_class_VisualObject, "is_visible", LN_TO_RUBY_FUNC(Wrap_LnVisualObject_Isvisible), -1);
    LnVisualObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_VisualObject, LnVisualObject_allocateForGetObject));

    g_class_Sprite = rb_define_class_under(g_rootModule, "Sprite", rb_cObject);
    rb_define_alloc_func(g_class_Sprite, LnSprite_allocate);
    rb_define_method(g_class_Sprite, "set_texture", LN_TO_RUBY_FUNC(Wrap_LnSprite_Settexture), -1);
    rb_define_method(g_class_Sprite, "set_source_rect", LN_TO_RUBY_FUNC(Wrap_LnSprite_SetsourcerectXYWH), -1);
    rb_define_private_method(g_class_Sprite, "initialize", LN_TO_RUBY_FUNC(Wrap_LnSprite_Create), -1);
    LnSprite_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Sprite, LnSprite_allocateForGetObject));

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

