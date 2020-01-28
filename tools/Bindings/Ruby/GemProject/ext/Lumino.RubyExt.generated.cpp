
#include <ruby.h>
#include "LuminoRubyRuntimeManager.h"
#include "FlatC.generated.h"



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



VALUE g_enum_PixelFormat;
VALUE g_enum_TextureFormat;
VALUE g_enum_DepthBufferFormat;

VALUE g_rootModule;
VALUE g_class_Object;
VALUE g_class_PromiseFailureDelegate;
VALUE g_class_ZVTestDelegate1;
VALUE g_class_ZVTestDelegate2;
VALUE g_class_ZVTestDelegate3;
VALUE g_class_ZVTestPromise1;
VALUE g_class_ZVTestPromise2;
VALUE g_class_ZVTestClass1;
VALUE g_class_Serializer;
VALUE g_class_AssetModel;
VALUE g_class_Assets;
VALUE g_class_EngineSettings;
VALUE g_class_Engine;
VALUE g_class_Application;
VALUE g_class_GraphicsResource;
VALUE g_class_Texture;
VALUE g_class_Texture2D;
VALUE g_class_Component;
VALUE g_class_VisualComponent;
VALUE g_class_SpriteComponent;
VALUE g_class_World;
VALUE g_class_ComponentList;
VALUE g_class_WorldObject;
VALUE g_class_VisualObject;
VALUE g_class_TestDelegate;
VALUE g_class_Sprite;
VALUE g_class_UIEventArgs;
VALUE g_class_UILayoutElement;
VALUE g_class_UIElement;
VALUE g_class_UIControl;
VALUE g_class_UIButtonBase;
VALUE g_class_UIButton;


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

static VALUE Wrap_LnVector3_GetX(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->x);
    }
    rb_raise(rb_eArgError, "ln::Vector3::getX - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_SetX(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->x = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::setX - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_GetY(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->y);
    }
    rb_raise(rb_eArgError, "ln::Vector3::getY - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_SetY(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->y = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::setY - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_GetZ(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->z);
    }
    rb_raise(rb_eArgError, "ln::Vector3::getZ - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_SetZ(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->z = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::setZ - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_Set(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnVector3_SetZeros(selfObj);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnVector3_Set(selfObj, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::Vector3 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_Length(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            float _outReturn;
            LnResult errorCode = LnVector3_Length(selfObj, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::length - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_LengthSquared(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            float _outReturn;
            LnResult errorCode = LnVector3_LengthSquared(selfObj, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::lengthSquared - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_MutatingNormalize(int argc, VALUE* argv, VALUE self)
{
    LnVector3* selfObj;
    Data_Get_Struct(self, LnVector3, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnVector3_MutatingNormalize(selfObj);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::mutatingNormalize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector3_Normalize(int argc, VALUE* argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnVector3 _outReturn;
            LnResult errorCode = LnVector3_NormalizeXYZ(_x, _y, _z, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (LNRB_VALUE_IS_OBJECT(vec))
        {
            LnVector3* tmp__vec; Data_Get_Struct(vec, LnVector3, tmp__vec);LnVector3& _vec = *tmp__vec;
            LnVector3 _outReturn;
            LnResult errorCode = LnVector3_Normalize(&_vec, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector3::normalize - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::Vector4

VALUE g_class_Vector4;

void LnVector4_delete(LnVector4* obj)
{
    free(obj);
}

VALUE LnVector4_allocate( VALUE klass )
{
    VALUE obj;
    LnVector4* internalObj;

    internalObj = (LnVector4*)malloc(sizeof(LnVector4));
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVector4_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LnVector4_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LnVector4));

    return obj;
}

static VALUE Wrap_LnVector4_GetX(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->x);
    }
    rb_raise(rb_eArgError, "ln::Vector4::getX - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_SetX(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->x = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector4::setX - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_GetY(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->y);
    }
    rb_raise(rb_eArgError, "ln::Vector4::getY - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_SetY(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->y = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector4::setY - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_GetZ(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->z);
    }
    rb_raise(rb_eArgError, "ln::Vector4::getZ - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_SetZ(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->z = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector4::setZ - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_GetW(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->w);
    }
    rb_raise(rb_eArgError, "ln::Vector4::getW - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_SetW(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->w = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector4::setW - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVector4_Set(int argc, VALUE* argv, VALUE self)
{
    LnVector4* selfObj;
    Data_Get_Struct(self, LnVector4, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnVector4_SetZeros(selfObj);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE z;
        VALUE w;
        rb_scan_args(argc, argv, "4", &x, &y, &z, &w);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z) && LNRB_VALUE_IS_FLOAT(w))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            float _w = LNRB_VALUE_TO_FLOAT(w);
            LnResult errorCode = LnVector4_Set(selfObj, _x, _y, _z, _w);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Vector4::Vector4 - wrong argument type.");
    return Qnil;
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

static VALUE Wrap_LnQuaternion_GetX(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->x);
    }
    rb_raise(rb_eArgError, "ln::Quaternion::getX - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_SetX(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->x = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Quaternion::setX - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_GetY(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->y);
    }
    rb_raise(rb_eArgError, "ln::Quaternion::getY - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_SetY(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->y = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Quaternion::setY - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_GetZ(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->z);
    }
    rb_raise(rb_eArgError, "ln::Quaternion::getZ - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_SetZ(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->z = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Quaternion::setZ - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_GetW(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 0) {
        return LNI_TO_RUBY_VALUE(selfObj->w);
    }
    rb_raise(rb_eArgError, "ln::Quaternion::getW - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_SetW(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_FLOAT(value)) {
            float _value = LNRB_VALUE_TO_FLOAT(value);
            selfObj->w = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Quaternion::setW - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnQuaternion_Set(int argc, VALUE* argv, VALUE self)
{
    LnQuaternion* selfObj;
    Data_Get_Struct(self, LnQuaternion, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnQuaternion_SetZeros(selfObj);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE z;
        VALUE w;
        rb_scan_args(argc, argv, "4", &x, &y, &z, &w);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z) && LNRB_VALUE_IS_FLOAT(w))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            float _w = LNRB_VALUE_TO_FLOAT(w);
            LnResult errorCode = LnQuaternion_Set(selfObj, _x, _y, _z, _w);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 2) {
        VALUE axis;
        VALUE r;
        rb_scan_args(argc, argv, "2", &axis, &r);
        if (LNRB_VALUE_IS_OBJECT(axis) && LNRB_VALUE_IS_FLOAT(r))
        {
            LnVector3* tmp__axis; Data_Get_Struct(axis, LnVector3, tmp__axis);LnVector3& _axis = *tmp__axis;
            float _r = LNRB_VALUE_TO_FLOAT(r);
            LnResult errorCode = LnQuaternion_SetFromAxis(selfObj, &_axis, _r);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Quaternion::Quaternion - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::Matrix

VALUE g_class_Matrix;

void LnMatrix_delete(LnMatrix* obj)
{
    free(obj);
}

VALUE LnMatrix_allocate( VALUE klass )
{
    VALUE obj;
    LnMatrix* internalObj;

    internalObj = (LnMatrix*)malloc(sizeof(LnMatrix));
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnMatrix_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LnMatrix_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LnMatrix));

    return obj;
}

static VALUE Wrap_LnMatrix_GetRow0(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 0) {
        VALUE retObj = LnVector4_allocate(g_class_Vector4);
        *((LnVector4*)DATA_PTR(retObj)) = selfObj->row0;
        return retObj;

    }
    rb_raise(rb_eArgError, "ln::Matrix::getRow0 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_SetRow0(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value)) {
            LnVector4* tmp__value; Data_Get_Struct(value, LnVector4, tmp__value);LnVector4& _value = *tmp__value;
            selfObj->row0 = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Matrix::setRow0 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_GetRow1(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 0) {
        VALUE retObj = LnVector4_allocate(g_class_Vector4);
        *((LnVector4*)DATA_PTR(retObj)) = selfObj->row1;
        return retObj;

    }
    rb_raise(rb_eArgError, "ln::Matrix::getRow1 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_SetRow1(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value)) {
            LnVector4* tmp__value; Data_Get_Struct(value, LnVector4, tmp__value);LnVector4& _value = *tmp__value;
            selfObj->row1 = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Matrix::setRow1 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_GetRow2(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 0) {
        VALUE retObj = LnVector4_allocate(g_class_Vector4);
        *((LnVector4*)DATA_PTR(retObj)) = selfObj->row2;
        return retObj;

    }
    rb_raise(rb_eArgError, "ln::Matrix::getRow2 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_SetRow2(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value)) {
            LnVector4* tmp__value; Data_Get_Struct(value, LnVector4, tmp__value);LnVector4& _value = *tmp__value;
            selfObj->row2 = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Matrix::setRow2 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_GetRow3(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 0) {
        VALUE retObj = LnVector4_allocate(g_class_Vector4);
        *((LnVector4*)DATA_PTR(retObj)) = selfObj->row3;
        return retObj;

    }
    rb_raise(rb_eArgError, "ln::Matrix::getRow3 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_SetRow3(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (argc == 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value)) {
            LnVector4* tmp__value; Data_Get_Struct(value, LnVector4, tmp__value);LnVector4& _value = *tmp__value;
            selfObj->row3 = _value;
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Matrix::setRow3 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnMatrix_Set(int argc, VALUE* argv, VALUE self)
{
    LnMatrix* selfObj;
    Data_Get_Struct(self, LnMatrix, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnMatrix_SetZeros(selfObj);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    if (16 <= argc && argc <= 16) {
        VALUE m11;
        VALUE m12;
        VALUE m13;
        VALUE m14;
        VALUE m21;
        VALUE m22;
        VALUE m23;
        VALUE m24;
        VALUE m31;
        VALUE m32;
        VALUE m33;
        VALUE m34;
        VALUE m41;
        VALUE m42;
        VALUE m43;
        VALUE m44;
        rb_scan_args(argc, argv, "16", &m11, &m12, &m13, &m14, &m21, &m22, &m23, &m24, &m31, &m32, &m33, &m34, &m41, &m42, &m43, &m44);
        if (LNRB_VALUE_IS_FLOAT(m11) && LNRB_VALUE_IS_FLOAT(m12) && LNRB_VALUE_IS_FLOAT(m13) && LNRB_VALUE_IS_FLOAT(m14) && LNRB_VALUE_IS_FLOAT(m21) && LNRB_VALUE_IS_FLOAT(m22) && LNRB_VALUE_IS_FLOAT(m23) && LNRB_VALUE_IS_FLOAT(m24) && LNRB_VALUE_IS_FLOAT(m31) && LNRB_VALUE_IS_FLOAT(m32) && LNRB_VALUE_IS_FLOAT(m33) && LNRB_VALUE_IS_FLOAT(m34) && LNRB_VALUE_IS_FLOAT(m41) && LNRB_VALUE_IS_FLOAT(m42) && LNRB_VALUE_IS_FLOAT(m43) && LNRB_VALUE_IS_FLOAT(m44))
        {
            float _m11 = LNRB_VALUE_TO_FLOAT(m11);
            float _m12 = LNRB_VALUE_TO_FLOAT(m12);
            float _m13 = LNRB_VALUE_TO_FLOAT(m13);
            float _m14 = LNRB_VALUE_TO_FLOAT(m14);
            float _m21 = LNRB_VALUE_TO_FLOAT(m21);
            float _m22 = LNRB_VALUE_TO_FLOAT(m22);
            float _m23 = LNRB_VALUE_TO_FLOAT(m23);
            float _m24 = LNRB_VALUE_TO_FLOAT(m24);
            float _m31 = LNRB_VALUE_TO_FLOAT(m31);
            float _m32 = LNRB_VALUE_TO_FLOAT(m32);
            float _m33 = LNRB_VALUE_TO_FLOAT(m33);
            float _m34 = LNRB_VALUE_TO_FLOAT(m34);
            float _m41 = LNRB_VALUE_TO_FLOAT(m41);
            float _m42 = LNRB_VALUE_TO_FLOAT(m42);
            float _m43 = LNRB_VALUE_TO_FLOAT(m43);
            float _m44 = LNRB_VALUE_TO_FLOAT(m44);
            LnResult errorCode = LnMatrix_Set(selfObj, _m11, _m12, _m13, _m14, _m21, _m22, _m23, _m24, _m31, _m32, _m33, _m34, _m41, _m42, _m43, _m44);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Matrix::Matrix - wrong argument type.");
    return Qnil;
}

//==============================================================================
// ln::Object

struct Wrap_Object
    : public Wrap_RubyObject
{

    Wrap_Object()
    {}
};

static void LnObject_delete(Wrap_Object* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnObject_mark(Wrap_Object* obj)
{
	

}

static VALUE LnObject_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Object* internalObj;

    internalObj = new Wrap_Object();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnObject_allocate");
    obj = Data_Wrap_Struct(klass, LnObject_mark, LnObject_delete, internalObj);

    return obj;
}

static VALUE LnObject_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Object* internalObj;

    internalObj = new Wrap_Object();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnObject_allocate");
    obj = Data_Wrap_Struct(klass, LnObject_mark, LnObject_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnObject_OnSerialize(int argc, VALUE* argv, VALUE self)
{
    Wrap_Object* selfObj;
    Data_Get_Struct(self, Wrap_Object, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE ar;
        rb_scan_args(argc, argv, "1", &ar);
        if (LNRB_VALUE_IS_OBJECT(ar))
        {
            LnHandle _ar = LuminoRubyRuntimeManager::instance->getHandle(ar);
            LnResult errorCode = LnObject_OnSerialize_CallOverrideBase(selfObj->handle, _ar);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Object::onSerialize - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnObject_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::PromiseFailureDelegate

struct Wrap_PromiseFailureDelegate
    : public Wrap_Object
{

    VALUE m_proc = Qnil;
    Wrap_PromiseFailureDelegate()
    {}
};

static void LnPromiseFailureDelegate_delete(Wrap_PromiseFailureDelegate* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnPromiseFailureDelegate_mark(Wrap_PromiseFailureDelegate* obj)
{
	
rb_gc_mark(obj->m_proc);

}

static VALUE LnPromiseFailureDelegate_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_PromiseFailureDelegate* internalObj;

    internalObj = new Wrap_PromiseFailureDelegate();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnPromiseFailureDelegate_allocate");
    obj = Data_Wrap_Struct(klass, LnPromiseFailureDelegate_mark, LnPromiseFailureDelegate_delete, internalObj);

    return obj;
}

static VALUE LnPromiseFailureDelegate_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_PromiseFailureDelegate* internalObj;

    internalObj = new Wrap_PromiseFailureDelegate();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnPromiseFailureDelegate_allocate");
    obj = Data_Wrap_Struct(klass, LnPromiseFailureDelegate_mark, LnPromiseFailureDelegate_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static LnResult Wrap_LnPromiseFailureDelegate_PromiseFailureDelegate_ProcCaller(LnHandle promisefailuredelegate)
{
    Wrap_PromiseFailureDelegate* selfObj;
    Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE(promisefailuredelegate), Wrap_PromiseFailureDelegate, selfObj);
    VALUE retval = rb_funcall(selfObj->m_proc, rb_intern("call"), 0, );
    return LN_SUCCESS;	// TODO: error handling.
}

static VALUE Wrap_LnPromiseFailureDelegate_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_PromiseFailureDelegate* selfObj;
    Data_Get_Struct(self, Wrap_PromiseFailureDelegate, selfObj);
    if (0 <= argc && argc <= 1) {
        VALUE proc, block;
        rb_scan_args(argc, argv, "01&", &proc, &block); // (handler=nil, &block)
        if (proc != Qnil) selfObj->m_proc = proc;
        if (block != Qnil) selfObj->m_proc = block;
        LnResult result = LnPromiseFailureDelegate_Create(Wrap_LnPromiseFailureDelegate_PromiseFailureDelegate_ProcCaller, &selfObj->handle);
        if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
        LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);
        return Qnil;
    }
    rb_raise(rb_eArgError, "ln::PromiseFailureDelegate::init - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::ZVTestDelegate1

struct Wrap_ZVTestDelegate1
    : public Wrap_Object
{

    VALUE m_proc = Qnil;
    Wrap_ZVTestDelegate1()
    {}
};

static void LnZVTestDelegate1_delete(Wrap_ZVTestDelegate1* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnZVTestDelegate1_mark(Wrap_ZVTestDelegate1* obj)
{
	
rb_gc_mark(obj->m_proc);

}

static VALUE LnZVTestDelegate1_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_ZVTestDelegate1* internalObj;

    internalObj = new Wrap_ZVTestDelegate1();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestDelegate1_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestDelegate1_mark, LnZVTestDelegate1_delete, internalObj);

    return obj;
}

static VALUE LnZVTestDelegate1_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_ZVTestDelegate1* internalObj;

    internalObj = new Wrap_ZVTestDelegate1();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestDelegate1_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestDelegate1_mark, LnZVTestDelegate1_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static LnResult Wrap_LnZVTestDelegate1_ZVTestDelegate1_ProcCaller(LnHandle zvtestdelegate1, int p1)
{
    Wrap_ZVTestDelegate1* selfObj;
    Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE(zvtestdelegate1), Wrap_ZVTestDelegate1, selfObj);
    VALUE retval = rb_funcall(selfObj->m_proc, rb_intern("call"), 1, LNI_TO_RUBY_VALUE(p1));
    return LN_SUCCESS;	// TODO: error handling.
}

static VALUE Wrap_LnZVTestDelegate1_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestDelegate1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestDelegate1, selfObj);
    if (0 <= argc && argc <= 1) {
        VALUE proc, block;
        rb_scan_args(argc, argv, "01&", &proc, &block); // (handler=nil, &block)
        if (proc != Qnil) selfObj->m_proc = proc;
        if (block != Qnil) selfObj->m_proc = block;
        LnResult result = LnZVTestDelegate1_Create(Wrap_LnZVTestDelegate1_ZVTestDelegate1_ProcCaller, &selfObj->handle);
        if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
        LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);
        return Qnil;
    }
    rb_raise(rb_eArgError, "ln::ZVTestDelegate1::init - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::ZVTestDelegate2

struct Wrap_ZVTestDelegate2
    : public Wrap_Object
{

    VALUE m_proc = Qnil;
    Wrap_ZVTestDelegate2()
    {}
};

static void LnZVTestDelegate2_delete(Wrap_ZVTestDelegate2* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnZVTestDelegate2_mark(Wrap_ZVTestDelegate2* obj)
{
	
rb_gc_mark(obj->m_proc);

}

static VALUE LnZVTestDelegate2_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_ZVTestDelegate2* internalObj;

    internalObj = new Wrap_ZVTestDelegate2();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestDelegate2_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestDelegate2_mark, LnZVTestDelegate2_delete, internalObj);

    return obj;
}

static VALUE LnZVTestDelegate2_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_ZVTestDelegate2* internalObj;

    internalObj = new Wrap_ZVTestDelegate2();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestDelegate2_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestDelegate2_mark, LnZVTestDelegate2_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static LnResult Wrap_LnZVTestDelegate2_ZVTestDelegate2_ProcCaller(LnHandle zvtestdelegate2, int p1, int p2, int* outReturn)
{
    Wrap_ZVTestDelegate2* selfObj;
    Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE(zvtestdelegate2), Wrap_ZVTestDelegate2, selfObj);
    VALUE retval = rb_funcall(selfObj->m_proc, rb_intern("call"), 2, LNI_TO_RUBY_VALUE(p1), LNI_TO_RUBY_VALUE(p2));
    *outReturn = LNRB_VALUE_TO_NUMBER(retval);
    return LN_SUCCESS;	// TODO: error handling.
}

static VALUE Wrap_LnZVTestDelegate2_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestDelegate2* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestDelegate2, selfObj);
    if (0 <= argc && argc <= 1) {
        VALUE proc, block;
        rb_scan_args(argc, argv, "01&", &proc, &block); // (handler=nil, &block)
        if (proc != Qnil) selfObj->m_proc = proc;
        if (block != Qnil) selfObj->m_proc = block;
        LnResult result = LnZVTestDelegate2_Create(Wrap_LnZVTestDelegate2_ZVTestDelegate2_ProcCaller, &selfObj->handle);
        if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
        LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);
        return Qnil;
    }
    rb_raise(rb_eArgError, "ln::ZVTestDelegate2::init - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::ZVTestDelegate3

struct Wrap_ZVTestDelegate3
    : public Wrap_Object
{

    VALUE m_proc = Qnil;
    Wrap_ZVTestDelegate3()
    {}
};

static void LnZVTestDelegate3_delete(Wrap_ZVTestDelegate3* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnZVTestDelegate3_mark(Wrap_ZVTestDelegate3* obj)
{
	
rb_gc_mark(obj->m_proc);

}

static VALUE LnZVTestDelegate3_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_ZVTestDelegate3* internalObj;

    internalObj = new Wrap_ZVTestDelegate3();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestDelegate3_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestDelegate3_mark, LnZVTestDelegate3_delete, internalObj);

    return obj;
}

static VALUE LnZVTestDelegate3_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_ZVTestDelegate3* internalObj;

    internalObj = new Wrap_ZVTestDelegate3();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestDelegate3_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestDelegate3_mark, LnZVTestDelegate3_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static LnResult Wrap_LnZVTestDelegate3_ZVTestDelegate3_ProcCaller(LnHandle zvtestdelegate3, LnHandle p1)
{
    Wrap_ZVTestDelegate3* selfObj;
    Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE(zvtestdelegate3), Wrap_ZVTestDelegate3, selfObj);
    VALUE retval = rb_funcall(selfObj->m_proc, rb_intern("call"), 1, LNRB_HANDLE_WRAP_TO_VALUE(p1));
    return LN_SUCCESS;	// TODO: error handling.
}

static VALUE Wrap_LnZVTestDelegate3_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestDelegate3* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestDelegate3, selfObj);
    if (0 <= argc && argc <= 1) {
        VALUE proc, block;
        rb_scan_args(argc, argv, "01&", &proc, &block); // (handler=nil, &block)
        if (proc != Qnil) selfObj->m_proc = proc;
        if (block != Qnil) selfObj->m_proc = block;
        LnResult result = LnZVTestDelegate3_Create(Wrap_LnZVTestDelegate3_ZVTestDelegate3_ProcCaller, &selfObj->handle);
        if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
        LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);
        return Qnil;
    }
    rb_raise(rb_eArgError, "ln::ZVTestDelegate3::init - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::ZVTestPromise1

struct Wrap_ZVTestPromise1
    : public Wrap_Object
{

    Wrap_ZVTestPromise1()
    {}
};

static void LnZVTestPromise1_delete(Wrap_ZVTestPromise1* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnZVTestPromise1_mark(Wrap_ZVTestPromise1* obj)
{
	

}

static VALUE LnZVTestPromise1_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_ZVTestPromise1* internalObj;

    internalObj = new Wrap_ZVTestPromise1();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestPromise1_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestPromise1_mark, LnZVTestPromise1_delete, internalObj);

    return obj;
}

static VALUE LnZVTestPromise1_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_ZVTestPromise1* internalObj;

    internalObj = new Wrap_ZVTestPromise1();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestPromise1_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestPromise1_mark, LnZVTestPromise1_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnZVTestPromise1_ThenWith(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestPromise1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestPromise1, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE callback;
        rb_scan_args(argc, argv, "1", &callback);
        if (LNRB_VALUE_IS_OBJECT(callback))
        {
            LnHandle _callback = LuminoRubyRuntimeManager::instance->getHandle(callback);
            LnResult errorCode = LnZVTestPromise1_ThenWith(selfObj->handle, _callback);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_ZVTestDelegate3, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnZVTestPromise1_ThenWith(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestPromise1::thenWith - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestPromise1_CatchWith(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestPromise1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestPromise1, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE callback;
        rb_scan_args(argc, argv, "1", &callback);
        if (LNRB_VALUE_IS_OBJECT(callback))
        {
            LnHandle _callback = LuminoRubyRuntimeManager::instance->getHandle(callback);
            LnResult errorCode = LnZVTestPromise1_CatchWith(selfObj->handle, _callback);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_PromiseFailureDelegate, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnZVTestPromise1_CatchWith(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestPromise1::catchWith - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::ZVTestPromise2

struct Wrap_ZVTestPromise2
    : public Wrap_Object
{

    Wrap_ZVTestPromise2()
    {}
};

static void LnZVTestPromise2_delete(Wrap_ZVTestPromise2* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnZVTestPromise2_mark(Wrap_ZVTestPromise2* obj)
{
	

}

static VALUE LnZVTestPromise2_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_ZVTestPromise2* internalObj;

    internalObj = new Wrap_ZVTestPromise2();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestPromise2_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestPromise2_mark, LnZVTestPromise2_delete, internalObj);

    return obj;
}

static VALUE LnZVTestPromise2_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_ZVTestPromise2* internalObj;

    internalObj = new Wrap_ZVTestPromise2();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestPromise2_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestPromise2_mark, LnZVTestPromise2_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnZVTestPromise2_ThenWith(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestPromise2* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestPromise2, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE callback;
        rb_scan_args(argc, argv, "1", &callback);
        if (LNRB_VALUE_IS_OBJECT(callback))
        {
            LnHandle _callback = LuminoRubyRuntimeManager::instance->getHandle(callback);
            LnResult errorCode = LnZVTestPromise2_ThenWith(selfObj->handle, _callback);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_ZVTestDelegate1, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnZVTestPromise2_ThenWith(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestPromise2::thenWith - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestPromise2_CatchWith(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestPromise2* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestPromise2, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE callback;
        rb_scan_args(argc, argv, "1", &callback);
        if (LNRB_VALUE_IS_OBJECT(callback))
        {
            LnHandle _callback = LuminoRubyRuntimeManager::instance->getHandle(callback);
            LnResult errorCode = LnZVTestPromise2_CatchWith(selfObj->handle, _callback);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_PromiseFailureDelegate, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnZVTestPromise2_CatchWith(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestPromise2::catchWith - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::ZVTestClass1

struct Wrap_ZVTestClass1
    : public Wrap_Object
{

    Wrap_ZVTestClass1()
    {}
};

static void LnZVTestClass1_delete(Wrap_ZVTestClass1* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnZVTestClass1_mark(Wrap_ZVTestClass1* obj)
{
	

}

static VALUE LnZVTestClass1_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_ZVTestClass1* internalObj;

    internalObj = new Wrap_ZVTestClass1();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestClass1_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestClass1_mark, LnZVTestClass1_delete, internalObj);

    return obj;
}

static VALUE LnZVTestClass1_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_ZVTestClass1* internalObj;

    internalObj = new Wrap_ZVTestClass1();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnZVTestClass1_allocate");
    obj = Data_Wrap_Struct(klass, LnZVTestClass1_mark, LnZVTestClass1_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnZVTestClass1_SetTestDelegate1(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult errorCode = LnZVTestClass1_SetTestDelegate1(selfObj->handle, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_ZVTestDelegate1, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnZVTestClass1_SetTestDelegate1(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::setTestDelegate1 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_SetTestDelegate2(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult errorCode = LnZVTestClass1_SetTestDelegate2(selfObj->handle, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_ZVTestDelegate2, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnZVTestClass1_SetTestDelegate2(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::setTestDelegate2 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_SetTestDelegate3(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult errorCode = LnZVTestClass1_SetTestDelegate3(selfObj->handle, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_ZVTestDelegate3, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnZVTestClass1_SetTestDelegate3(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::setTestDelegate3 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_CallTestDelegate1(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE a;
        rb_scan_args(argc, argv, "1", &a);
        if (LNRB_VALUE_IS_NUMBER(a))
        {
            int _a = LNRB_VALUE_TO_NUMBER(a);
            LnResult errorCode = LnZVTestClass1_CallTestDelegate1(selfObj->handle, _a);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::callTestDelegate1 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_CallTestDelegate2(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE a;
        VALUE b;
        rb_scan_args(argc, argv, "2", &a, &b);
        if (LNRB_VALUE_IS_NUMBER(a) && LNRB_VALUE_IS_NUMBER(b))
        {
            int _a = LNRB_VALUE_TO_NUMBER(a);
            int _b = LNRB_VALUE_TO_NUMBER(b);
            int _outReturn;
            LnResult errorCode = LnZVTestClass1_CallTestDelegate2(selfObj->handle, _a, _b, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::callTestDelegate2 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_CallTestDelegate3(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnZVTestClass1_CallTestDelegate3(selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::callTestDelegate3 - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_LoadAsync(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (LNRB_VALUE_IS_STRING(filePath))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnHandle _outReturn;
            LnResult errorCode = LnZVTestClass1_LoadAsyncA(_filePath, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::loadAsync - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_ExecuteAsync(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnZVTestClass1_ExecuteAsync(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::executeAsync - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_GetFilePath(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            const char* _outReturn;
            LnResult errorCode = LnZVTestClass1_GetFilePathA(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::filePath - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnZVTestClass1_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_ZVTestClass1* selfObj;
    Data_Get_Struct(self, Wrap_ZVTestClass1, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnZVTestClass1_Create(&selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::ZVTestClass1::init - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnZVTestClass1_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::Serializer

struct Wrap_Serializer
    : public Wrap_Object
{

    Wrap_Serializer()
    {}
};

static void LnSerializer_delete(Wrap_Serializer* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnSerializer_mark(Wrap_Serializer* obj)
{
	

}

static VALUE LnSerializer_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Serializer* internalObj;

    internalObj = new Wrap_Serializer();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSerializer_allocate");
    obj = Data_Wrap_Struct(klass, LnSerializer_mark, LnSerializer_delete, internalObj);

    return obj;
}

static VALUE LnSerializer_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Serializer* internalObj;

    internalObj = new Wrap_Serializer();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSerializer_allocate");
    obj = Data_Wrap_Struct(klass, LnSerializer_mark, LnSerializer_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnSerializer_WriteBool(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE name;
        VALUE value;
        rb_scan_args(argc, argv, "2", &name, &value);
        if (LNRB_VALUE_IS_STRING(name) && LNRB_VALUE_IS_BOOL(value))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            LnBool _value = LNRB_VALUE_TO_BOOL(value);
            LnResult errorCode = LnSerializer_WriteBoolA(selfObj->handle, _name, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::writeBool - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_WriteInt(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE name;
        VALUE value;
        rb_scan_args(argc, argv, "2", &name, &value);
        if (LNRB_VALUE_IS_STRING(name) && LNRB_VALUE_IS_NUMBER(value))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            int _value = LNRB_VALUE_TO_NUMBER(value);
            LnResult errorCode = LnSerializer_WriteIntA(selfObj->handle, _name, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::writeInt - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_WriteFloat(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE name;
        VALUE value;
        rb_scan_args(argc, argv, "2", &name, &value);
        if (LNRB_VALUE_IS_STRING(name) && LNRB_VALUE_IS_FLOAT(value))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            float _value = LNRB_VALUE_TO_FLOAT(value);
            LnResult errorCode = LnSerializer_WriteFloatA(selfObj->handle, _name, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::writeFloat - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_WriteString(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE name;
        VALUE value;
        rb_scan_args(argc, argv, "2", &name, &value);
        if (LNRB_VALUE_IS_STRING(name) && LNRB_VALUE_IS_STRING(value))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            const char* _value = LNRB_VALUE_TO_STRING(value);
            LnResult errorCode = LnSerializer_WriteStringA(selfObj->handle, _name, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::writeString - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_WriteObject(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE name;
        VALUE value;
        rb_scan_args(argc, argv, "2", &name, &value);
        if (LNRB_VALUE_IS_STRING(name) && LNRB_VALUE_IS_OBJECT(value))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult errorCode = LnSerializer_WriteObjectA(selfObj->handle, _name, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::writeObject - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_ReadBool(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "1", &name);
        if (LNRB_VALUE_IS_STRING(name))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            LnBool _outReturn;
            LnResult errorCode = LnSerializer_ReadBoolA(selfObj->handle, _name, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::readBool - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_ReadInt(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "1", &name);
        if (LNRB_VALUE_IS_STRING(name))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            int _outReturn;
            LnResult errorCode = LnSerializer_ReadIntA(selfObj->handle, _name, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::readInt - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_ReadFloat(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "1", &name);
        if (LNRB_VALUE_IS_STRING(name))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            float _outReturn;
            LnResult errorCode = LnSerializer_ReadFloatA(selfObj->handle, _name, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::readFloat - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_ReadString(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "1", &name);
        if (LNRB_VALUE_IS_STRING(name))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            const char* _outReturn;
            LnResult errorCode = LnSerializer_ReadStringA(selfObj->handle, _name, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::readString - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_ReadObject(int argc, VALUE* argv, VALUE self)
{
    Wrap_Serializer* selfObj;
    Data_Get_Struct(self, Wrap_Serializer, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "1", &name);
        if (LNRB_VALUE_IS_STRING(name))
        {
            const char* _name = LNRB_VALUE_TO_STRING(name);
            LnHandle _outReturn;
            LnResult errorCode = LnSerializer_ReadObjectA(selfObj->handle, _name, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::readObject - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_Serialize(int argc, VALUE* argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE value;
        VALUE basePath;
        rb_scan_args(argc, argv, "2", &value, &basePath);
        if (LNRB_VALUE_IS_OBJECT(value) && LNRB_VALUE_IS_STRING(basePath))
        {
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            const char* _basePath = LNRB_VALUE_TO_STRING(basePath);
            const char* _outReturn;
            LnResult errorCode = LnSerializer_SerializeA(_value, _basePath, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::serialize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSerializer_Deserialize(int argc, VALUE* argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE str;
        VALUE basePath;
        rb_scan_args(argc, argv, "2", &str, &basePath);
        if (LNRB_VALUE_IS_STRING(str) && LNRB_VALUE_IS_STRING(basePath))
        {
            const char* _str = LNRB_VALUE_TO_STRING(str);
            const char* _basePath = LNRB_VALUE_TO_STRING(basePath);
            LnHandle _outReturn;
            LnResult errorCode = LnSerializer_DeserializeA(_str, _basePath, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Serializer::deserialize - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnSerializer_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::AssetModel

struct Wrap_AssetModel
    : public Wrap_Object
{

    Wrap_AssetModel()
    {}
};

static void LnAssetModel_delete(Wrap_AssetModel* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnAssetModel_mark(Wrap_AssetModel* obj)
{
	

}

static VALUE LnAssetModel_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_AssetModel* internalObj;

    internalObj = new Wrap_AssetModel();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnAssetModel_allocate");
    obj = Data_Wrap_Struct(klass, LnAssetModel_mark, LnAssetModel_delete, internalObj);

    return obj;
}

static VALUE LnAssetModel_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_AssetModel* internalObj;

    internalObj = new Wrap_AssetModel();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnAssetModel_allocate");
    obj = Data_Wrap_Struct(klass, LnAssetModel_mark, LnAssetModel_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnAssetModel_Target(int argc, VALUE* argv, VALUE self)
{
    Wrap_AssetModel* selfObj;
    Data_Get_Struct(self, Wrap_AssetModel, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnAssetModel_Target(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::AssetModel::target - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnAssetModel_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_AssetModel* selfObj;
    Data_Get_Struct(self, Wrap_AssetModel, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE target;
        rb_scan_args(argc, argv, "1", &target);
        if (LNRB_VALUE_IS_OBJECT(target))
        {
            LnHandle _target = LuminoRubyRuntimeManager::instance->getHandle(target);
            LnResult errorCode = LnAssetModel_Create(_target, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::AssetModel::init - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnAssetModel_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::Assets

struct Wrap_Assets
{

    Wrap_Assets()
    {}
};


static VALUE Wrap_LnAssets_SaveAssetToLocalFile(int argc, VALUE* argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE asset;
        VALUE filePath;
        rb_scan_args(argc, argv, "2", &asset, &filePath);
        if (LNRB_VALUE_IS_OBJECT(asset) && LNRB_VALUE_IS_STRING(filePath))
        {
            LnHandle _asset = LuminoRubyRuntimeManager::instance->getHandle(asset);
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnResult errorCode = LnAssets_SaveAssetToLocalFileA(_asset, _filePath);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Assets::saveAssetToLocalFile - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnAssets_LoadAssetFromLocalFile(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (LNRB_VALUE_IS_STRING(filePath))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnHandle _outReturn;
            LnResult errorCode = LnAssets_LoadAssetFromLocalFileA(_filePath, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Assets::loadAssetFromLocalFile - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnAssets_LoadAsset(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (LNRB_VALUE_IS_STRING(filePath))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnHandle _outReturn;
            LnResult errorCode = LnAssets_LoadAssetA(_filePath, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Assets::loadAsset - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::EngineSettings

struct Wrap_EngineSettings
{

    Wrap_EngineSettings()
    {}
};


static VALUE Wrap_LnEngineSettings_SetMainWindowSize(int argc, VALUE* argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (LNRB_VALUE_IS_NUMBER(width) && LNRB_VALUE_IS_NUMBER(height))
        {
            int _width = LNRB_VALUE_TO_NUMBER(width);
            int _height = LNRB_VALUE_TO_NUMBER(height);
            LnResult errorCode = LnEngineSettings_SetMainWindowSize(_width, _height);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::setMainWindowSize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngineSettings_SetMainWorldViewSize(int argc, VALUE* argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (LNRB_VALUE_IS_NUMBER(width) && LNRB_VALUE_IS_NUMBER(height))
        {
            int _width = LNRB_VALUE_TO_NUMBER(width);
            int _height = LNRB_VALUE_TO_NUMBER(height);
            LnResult errorCode = LnEngineSettings_SetMainWorldViewSize(_width, _height);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::setMainWorldViewSize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngineSettings_SetMainWindowTitle(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE title;
        rb_scan_args(argc, argv, "1", &title);
        if (LNRB_VALUE_IS_STRING(title))
        {
            const char* _title = LNRB_VALUE_TO_STRING(title);
            LnResult errorCode = LnEngineSettings_SetMainWindowTitleA(_title);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::setMainWindowTitle - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngineSettings_AddAssetDirectory(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE path;
        rb_scan_args(argc, argv, "1", &path);
        if (LNRB_VALUE_IS_STRING(path))
        {
            const char* _path = LNRB_VALUE_TO_STRING(path);
            LnResult errorCode = LnEngineSettings_AddAssetDirectoryA(_path);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::addAssetDirectory - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngineSettings_AddAssetArchive(int argc, VALUE* argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE fileFullPath;
        VALUE password;
        rb_scan_args(argc, argv, "2", &fileFullPath, &password);
        if (LNRB_VALUE_IS_STRING(fileFullPath) && LNRB_VALUE_IS_STRING(password))
        {
            const char* _fileFullPath = LNRB_VALUE_TO_STRING(fileFullPath);
            const char* _password = LNRB_VALUE_TO_STRING(password);
            LnResult errorCode = LnEngineSettings_AddAssetArchiveA(_fileFullPath, _password);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::addAssetArchive - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngineSettings_SetEngineLogEnabled(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (LNRB_VALUE_IS_BOOL(enabled))
        {
            LnBool _enabled = LNRB_VALUE_TO_BOOL(enabled);
            LnResult errorCode = LnEngineSettings_SetEngineLogEnabled(_enabled);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::setEngineLogEnabled - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngineSettings_SetEngineLogFilePath(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (LNRB_VALUE_IS_STRING(filePath))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnResult errorCode = LnEngineSettings_SetEngineLogFilePathA(_filePath);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::setEngineLogFilePath - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::Engine

struct Wrap_Engine
{
    VALUE LnEngine_GetWorld_AccessorCache = Qnil;

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

static VALUE Wrap_LnEngine_MainUIView(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnEngine_MainUIView(&_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::mainUIView - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_GetWorld(int argc, VALUE* argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnEngine_GetWorld(&_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Engine::world - wrong argument type.");
    return Qnil;
}


//==============================================================================
// ln::Application

struct Wrap_Application
    : public Wrap_Object
{

    Wrap_Application()
    {}
};

static void LnApplication_delete(Wrap_Application* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnApplication_mark(Wrap_Application* obj)
{
	

}

static VALUE LnApplication_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Application* internalObj;

    internalObj = new Wrap_Application();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnApplication_allocate");
    obj = Data_Wrap_Struct(klass, LnApplication_mark, LnApplication_delete, internalObj);

    return obj;
}

static VALUE LnApplication_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Application* internalObj;

    internalObj = new Wrap_Application();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnApplication_allocate");
    obj = Data_Wrap_Struct(klass, LnApplication_mark, LnApplication_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnApplication_OnInit(int argc, VALUE* argv, VALUE self)
{
    Wrap_Application* selfObj;
    Data_Get_Struct(self, Wrap_Application, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnApplication_OnInit_CallOverrideBase(selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Application::onInit - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnApplication_OnUpdate(int argc, VALUE* argv, VALUE self)
{
    Wrap_Application* selfObj;
    Data_Get_Struct(self, Wrap_Application, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnApplication_OnUpdate_CallOverrideBase(selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Application::onUpdate - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnApplication_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_Application* selfObj;
    Data_Get_Struct(self, Wrap_Application, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnApplication_Create(&selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Application::init - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnApplication_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}
LnResult Wrap_LnApplication_OnInit_OverrideCallback(LnHandle application)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(application);
    VALUE retval = rb_funcall(obj, rb_intern("on_init"), 0, );
    return LN_SUCCESS;
}
LnResult Wrap_LnApplication_OnUpdate_OverrideCallback(LnHandle application)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(application);
    VALUE retval = rb_funcall(obj, rb_intern("on_update"), 0, );
    return LN_SUCCESS;
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
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
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


LnResult Wrap_LnGraphicsResource_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
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
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
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


LnResult Wrap_LnTexture_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
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
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
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


static VALUE Wrap_LnTexture2D_Load(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (LNRB_VALUE_IS_STRING(filePath))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnHandle _outReturn;
            LnResult errorCode = LnTexture2D_LoadA(_filePath, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Texture2D::load - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnTexture2D_LoadEmoji(int argc, VALUE* argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE code;
        rb_scan_args(argc, argv, "1", &code);
        if (LNRB_VALUE_IS_STRING(code))
        {
            const char* _code = LNRB_VALUE_TO_STRING(code);
            LnHandle _outReturn;
	printf("call LnTexture2D_LoadEmojiA\n");
            LnResult errorCode = LnTexture2D_LoadEmojiA(_code, &_outReturn);
	printf("ret LnTexture2D_LoadEmojiA\n");
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
	printf("Wrap_LnTexture2D_LoadEmoji 1\n");
            return LNRB_HANDLE_WRAP_TO_VALUE_NO_RETAIN(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::Texture2D::loadEmoji - wrong argument type.");
    return Qnil;
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
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
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
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 2) {
        VALUE filePath;
        VALUE format;
        rb_scan_args(argc, argv, "11", &filePath, &format);
        if (LNRB_VALUE_IS_STRING(filePath) && LNRB_VALUE_IS_NUMBER(format))
        {
            const char* _filePath = LNRB_VALUE_TO_STRING(filePath);
            LnTextureFormat _format = (format != Qnil) ? (LnTextureFormat)FIX2INT(format) : (LnTextureFormat)1;
            LnResult errorCode = LnTexture2D_CreateFromFileA(_filePath, _format, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Texture2D::init - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnTexture2D_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::Component

struct Wrap_Component
    : public Wrap_Object
{

    Wrap_Component()
    {}
};

static void LnComponent_delete(Wrap_Component* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnComponent_mark(Wrap_Component* obj)
{
	

}

static VALUE LnComponent_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_Component* internalObj;

    internalObj = new Wrap_Component();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnComponent_allocate");
    obj = Data_Wrap_Struct(klass, LnComponent_mark, LnComponent_delete, internalObj);

    return obj;
}

static VALUE LnComponent_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_Component* internalObj;

    internalObj = new Wrap_Component();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnComponent_allocate");
    obj = Data_Wrap_Struct(klass, LnComponent_mark, LnComponent_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


LnResult Wrap_LnComponent_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::VisualComponent

struct Wrap_VisualComponent
    : public Wrap_Component
{

    Wrap_VisualComponent()
    {}
};

static void LnVisualComponent_delete(Wrap_VisualComponent* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnVisualComponent_mark(Wrap_VisualComponent* obj)
{
	

}

static VALUE LnVisualComponent_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_VisualComponent* internalObj;

    internalObj = new Wrap_VisualComponent();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVisualComponent_allocate");
    obj = Data_Wrap_Struct(klass, LnVisualComponent_mark, LnVisualComponent_delete, internalObj);

    return obj;
}

static VALUE LnVisualComponent_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_VisualComponent* internalObj;

    internalObj = new Wrap_VisualComponent();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnVisualComponent_allocate");
    obj = Data_Wrap_Struct(klass, LnVisualComponent_mark, LnVisualComponent_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnVisualComponent_SetVisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualComponent* selfObj;
    Data_Get_Struct(self, Wrap_VisualComponent, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_BOOL(value))
        {
            LnBool _value = LNRB_VALUE_TO_BOOL(value);
            LnResult errorCode = LnVisualComponent_SetVisible(selfObj->handle, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::VisualComponent::setVisible - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVisualComponent_IsVisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualComponent* selfObj;
    Data_Get_Struct(self, Wrap_VisualComponent, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnBool _outReturn;
            LnResult errorCode = LnVisualComponent_IsVisible(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::VisualComponent::isVisible - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnVisualComponent_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::SpriteComponent

struct Wrap_SpriteComponent
    : public Wrap_VisualComponent
{

    Wrap_SpriteComponent()
    {}
};

static void LnSpriteComponent_delete(Wrap_SpriteComponent* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnSpriteComponent_mark(Wrap_SpriteComponent* obj)
{
	

}

static VALUE LnSpriteComponent_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_SpriteComponent* internalObj;

    internalObj = new Wrap_SpriteComponent();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSpriteComponent_allocate");
    obj = Data_Wrap_Struct(klass, LnSpriteComponent_mark, LnSpriteComponent_delete, internalObj);

    return obj;
}

static VALUE LnSpriteComponent_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_SpriteComponent* internalObj;

    internalObj = new Wrap_SpriteComponent();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnSpriteComponent_allocate");
    obj = Data_Wrap_Struct(klass, LnSpriteComponent_mark, LnSpriteComponent_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnSpriteComponent_SetTexture(int argc, VALUE* argv, VALUE self)
{
    Wrap_SpriteComponent* selfObj;
    Data_Get_Struct(self, Wrap_SpriteComponent, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE texture;
        rb_scan_args(argc, argv, "1", &texture);
        if (LNRB_VALUE_IS_OBJECT(texture))
        {
            LnHandle _texture = LuminoRubyRuntimeManager::instance->getHandle(texture);
            LnResult errorCode = LnSpriteComponent_SetTexture(selfObj->handle, _texture);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::SpriteComponent::setTexture - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnSpriteComponent_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::World

struct Wrap_World
    : public Wrap_Object
{

    Wrap_World()
    {}
};

static void LnWorld_delete(Wrap_World* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnWorld_mark(Wrap_World* obj)
{
	

}

static VALUE LnWorld_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_World* internalObj;

    internalObj = new Wrap_World();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnWorld_allocate");
    obj = Data_Wrap_Struct(klass, LnWorld_mark, LnWorld_delete, internalObj);

    return obj;
}

static VALUE LnWorld_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_World* internalObj;

    internalObj = new Wrap_World();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnWorld_allocate");
    obj = Data_Wrap_Struct(klass, LnWorld_mark, LnWorld_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnWorld_Add(int argc, VALUE* argv, VALUE self)
{
    Wrap_World* selfObj;
    Data_Get_Struct(self, Wrap_World, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE obj;
        rb_scan_args(argc, argv, "1", &obj);
        if (LNRB_VALUE_IS_OBJECT(obj))
        {
            LnHandle _obj = LuminoRubyRuntimeManager::instance->getHandle(obj);
            LnResult errorCode = LnWorld_Add(selfObj->handle, _obj);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::World::add - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnWorld_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::ComponentList

struct Wrap_ComponentList
    : public Wrap_Object
{
    std::vector<VALUE> Items_AccessorCache;

    Wrap_ComponentList()
    {}
};

static void LnComponentList_delete(Wrap_ComponentList* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnComponentList_mark(Wrap_ComponentList* obj)
{
	for(VALUE& v : obj->Items_AccessorCache) rb_gc_mark(v);


}

static VALUE LnComponentList_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_ComponentList* internalObj;

    internalObj = new Wrap_ComponentList();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnComponentList_allocate");
    obj = Data_Wrap_Struct(klass, LnComponentList_mark, LnComponentList_delete, internalObj);

    return obj;
}

static VALUE LnComponentList_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_ComponentList* internalObj;

    internalObj = new Wrap_ComponentList();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnComponentList_allocate");
    obj = Data_Wrap_Struct(klass, LnComponentList_mark, LnComponentList_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnComponentList_GetLength(int argc, VALUE* argv, VALUE self)
{
    Wrap_ComponentList* selfObj;
    Data_Get_Struct(self, Wrap_ComponentList, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            int _outReturn;
            LnResult errorCode = LnComponentList_GetLength(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::ComponentList::getLength - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnComponentList_GetItem(int argc, VALUE* argv, VALUE self)
{
    Wrap_ComponentList* selfObj;
    Data_Get_Struct(self, Wrap_ComponentList, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE index;
        rb_scan_args(argc, argv, "1", &index);
        if (LNRB_VALUE_IS_NUMBER(index))
        {
            int _index = LNRB_VALUE_TO_NUMBER(index);
            LnHandle _outReturn;
            LnResult errorCode = LnComponentList_GetItem(selfObj->handle, _index, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE(_outReturn, selfObj->Items_AccessorCache, _index);
        }
    }
    rb_raise(rb_eArgError, "ln::ComponentList::getItem - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnComponentList_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::WorldObject

struct Wrap_WorldObject
    : public Wrap_Object
{
    VALUE LnWorldObject_GetComponents_AccessorCache = Qnil;

    Wrap_WorldObject()
    {}
};

static void LnWorldObject_delete(Wrap_WorldObject* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnWorldObject_mark(Wrap_WorldObject* obj)
{
	rb_gc_mark(obj->LnWorldObject_GetComponents_AccessorCache);


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


static VALUE Wrap_LnWorldObject_SetPosition(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE pos;
        rb_scan_args(argc, argv, "1", &pos);
        if (LNRB_VALUE_IS_OBJECT(pos))
        {
            LnVector3* tmp__pos; Data_Get_Struct(pos, LnVector3, tmp__pos);LnVector3& _pos = *tmp__pos;
            LnResult errorCode = LnWorldObject_SetPosition(selfObj->handle, &_pos);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnWorldObject_SetPositionXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setPosition - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_GetPosition(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_GetPosition(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::position - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetRotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE rot;
        rb_scan_args(argc, argv, "1", &rot);
        if (LNRB_VALUE_IS_OBJECT(rot))
        {
            LnQuaternion* tmp__rot; Data_Get_Struct(rot, LnQuaternion, tmp__rot);LnQuaternion& _rot = *tmp__rot;
            LnResult errorCode = LnWorldObject_SetRotation(selfObj->handle, &_rot);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setRotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetEulerAngles(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnWorldObject_SetEulerAngles(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setEulerAngles - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_GetRotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnQuaternion _outReturn;
            LnResult errorCode = LnWorldObject_GetRotation(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnQuaternion_allocate(g_class_Quaternion);
            *((LnQuaternion*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::rotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetScale(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE scale;
        rb_scan_args(argc, argv, "1", &scale);
        if (LNRB_VALUE_IS_OBJECT(scale))
        {
            LnVector3* tmp__scale; Data_Get_Struct(scale, LnVector3, tmp__scale);LnVector3& _scale = *tmp__scale;
            LnResult errorCode = LnWorldObject_SetScale(selfObj->handle, &_scale);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 1) {
        VALUE xyz;
        rb_scan_args(argc, argv, "1", &xyz);
        if (LNRB_VALUE_IS_FLOAT(xyz))
        {
            float _xyz = LNRB_VALUE_TO_FLOAT(xyz);
            LnResult errorCode = LnWorldObject_SetScaleS(selfObj->handle, _xyz);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 1.000000;
            LnResult errorCode = LnWorldObject_SetScaleXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setScale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_GetScale(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_GetScale(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::scale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_SetCenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnVector3* tmp__value; Data_Get_Struct(value, LnVector3, tmp__value);LnVector3& _value = *tmp__value;
            LnResult errorCode = LnWorldObject_SetCenterPoint(selfObj->handle, &_value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnWorldObject_SetCenterPointXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::setCenterPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_GetCenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnWorldObject_GetCenterPoint(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::centerPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_GetComponents(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnWorldObject_GetComponents(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE(_outReturn, selfObj->LnWorldObject_GetComponents_AccessorCache);
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::components - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnWorldObject_OnUpdate(int argc, VALUE* argv, VALUE self)
{
    Wrap_WorldObject* selfObj;
    Data_Get_Struct(self, Wrap_WorldObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE elapsedSeconds;
        rb_scan_args(argc, argv, "1", &elapsedSeconds);
        if (LNRB_VALUE_IS_FLOAT(elapsedSeconds))
        {
            float _elapsedSeconds = LNRB_VALUE_TO_FLOAT(elapsedSeconds);
            LnResult errorCode = LnWorldObject_OnUpdate_CallOverrideBase(selfObj->handle, _elapsedSeconds);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::WorldObject::onUpdate - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnWorldObject_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}
LnResult Wrap_LnWorldObject_OnUpdate_OverrideCallback(LnHandle worldobject, float elapsedSeconds)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(worldobject);
    VALUE retval = rb_funcall(obj, rb_intern("on_update"), 1, LNI_TO_RUBY_VALUE(elapsedSeconds));
    return LN_SUCCESS;
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
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
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


static VALUE Wrap_LnVisualObject_SetVisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualObject* selfObj;
    Data_Get_Struct(self, Wrap_VisualObject, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_BOOL(value))
        {
            LnBool _value = LNRB_VALUE_TO_BOOL(value);
            LnResult errorCode = LnVisualObject_SetVisible(selfObj->handle, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::VisualObject::setVisible - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnVisualObject_IsVisible(int argc, VALUE* argv, VALUE self)
{
    Wrap_VisualObject* selfObj;
    Data_Get_Struct(self, Wrap_VisualObject, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnBool _outReturn;
            LnResult errorCode = LnVisualObject_IsVisible(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNI_TO_RUBY_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::VisualObject::isVisible - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnVisualObject_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}
LnResult Wrap_LnVisualObject_OnUpdate_OverrideCallback(LnHandle worldobject, float elapsedSeconds)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(worldobject);
    VALUE retval = rb_funcall(obj, rb_intern("on_update"), 1, LNI_TO_RUBY_VALUE(elapsedSeconds));
    return LN_SUCCESS;
}

//==============================================================================
// ln::TestDelegate

struct Wrap_TestDelegate
    : public Wrap_Object
{

    VALUE m_proc = Qnil;
    Wrap_TestDelegate()
    {}
};

static void LnTestDelegate_delete(Wrap_TestDelegate* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnTestDelegate_mark(Wrap_TestDelegate* obj)
{
	
rb_gc_mark(obj->m_proc);

}

static VALUE LnTestDelegate_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_TestDelegate* internalObj;

    internalObj = new Wrap_TestDelegate();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTestDelegate_allocate");
    obj = Data_Wrap_Struct(klass, LnTestDelegate_mark, LnTestDelegate_delete, internalObj);

    return obj;
}

static VALUE LnTestDelegate_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_TestDelegate* internalObj;

    internalObj = new Wrap_TestDelegate();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnTestDelegate_allocate");
    obj = Data_Wrap_Struct(klass, LnTestDelegate_mark, LnTestDelegate_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static LnResult Wrap_LnTestDelegate_TestDelegate_ProcCaller(LnHandle testdelegate, int p1, int* outReturn)
{
    Wrap_TestDelegate* selfObj;
    Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE(testdelegate), Wrap_TestDelegate, selfObj);
    VALUE retval = rb_funcall(selfObj->m_proc, rb_intern("call"), 1, LNI_TO_RUBY_VALUE(p1));
    *outReturn = LNRB_VALUE_TO_NUMBER(retval);
    return LN_SUCCESS;	// TODO: error handling.
}

static VALUE Wrap_LnTestDelegate_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_TestDelegate* selfObj;
    Data_Get_Struct(self, Wrap_TestDelegate, selfObj);
    if (0 <= argc && argc <= 1) {
        VALUE proc, block;
        rb_scan_args(argc, argv, "01&", &proc, &block); // (handler=nil, &block)
        if (proc != Qnil) selfObj->m_proc = proc;
        if (block != Qnil) selfObj->m_proc = block;
        LnResult result = LnTestDelegate_Create(Wrap_LnTestDelegate_TestDelegate_ProcCaller, &selfObj->handle);
        if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
        LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);
        return Qnil;
    }
    rb_raise(rb_eArgError, "ln::TestDelegate::init - wrong argument type.");
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
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
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


static VALUE Wrap_LnSprite_SetTexture(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult errorCode = LnSprite_SetTexture(selfObj->handle, _value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::setTexture - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSprite_SetSourceRectXYWH(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "4", &x, &y, &width, &height);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(width) && LNRB_VALUE_IS_FLOAT(height))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _width = LNRB_VALUE_TO_FLOAT(width);
            float _height = LNRB_VALUE_TO_FLOAT(height);
            LnResult errorCode = LnSprite_SetSourceRectXYWH(selfObj->handle, _x, _y, _width, _height);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::setSourceRect - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSprite_SetCallerTest(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE callback;
        rb_scan_args(argc, argv, "1", &callback);
        if (LNRB_VALUE_IS_OBJECT(callback))
        {
            LnHandle _callback = LuminoRubyRuntimeManager::instance->getHandle(callback);
            LnResult errorCode = LnSprite_SetCallerTest(selfObj->handle, _callback);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (argc == 0) {
        VALUE block;
        rb_scan_args(argc, argv, "0&", &block);
        if (block != Qnil) {
            VALUE value = rb_funcall(g_class_TestDelegate, rb_intern("new"), 1, block);
            LnHandle _value = LuminoRubyRuntimeManager::instance->getHandle(value);
            LnResult result = LnSprite_SetCallerTest(selfObj->handle, _value);
            if (result < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", result, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::setCallerTest - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnSprite_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_Sprite* selfObj;
    Data_Get_Struct(self, Wrap_Sprite, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnSprite_Create(&selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
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
            LnResult errorCode = LnSprite_CreateWithTexture(_texture, _width, _height, &selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::Sprite::init - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnSprite_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}
LnResult Wrap_LnSprite_OnUpdate_OverrideCallback(LnHandle worldobject, float elapsedSeconds)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(worldobject);
    VALUE retval = rb_funcall(obj, rb_intern("on_update"), 1, LNI_TO_RUBY_VALUE(elapsedSeconds));
    return LN_SUCCESS;
}

//==============================================================================
// ln::UIEventArgs

struct Wrap_UIEventArgs
    : public Wrap_Object
{

    Wrap_UIEventArgs()
    {}
};

static void LnUIEventArgs_delete(Wrap_UIEventArgs* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnUIEventArgs_mark(Wrap_UIEventArgs* obj)
{
	

}

static VALUE LnUIEventArgs_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIEventArgs* internalObj;

    internalObj = new Wrap_UIEventArgs();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIEventArgs_allocate");
    obj = Data_Wrap_Struct(klass, LnUIEventArgs_mark, LnUIEventArgs_delete, internalObj);

    return obj;
}

static VALUE LnUIEventArgs_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIEventArgs* internalObj;

    internalObj = new Wrap_UIEventArgs();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIEventArgs_allocate");
    obj = Data_Wrap_Struct(klass, LnUIEventArgs_mark, LnUIEventArgs_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnUIEventArgs_Sender(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIEventArgs* selfObj;
    Data_Get_Struct(self, Wrap_UIEventArgs, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnHandle _outReturn;
            LnResult errorCode = LnUIEventArgs_Sender(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return LNRB_HANDLE_WRAP_TO_VALUE(_outReturn);
        }
    }
    rb_raise(rb_eArgError, "ln::UIEventArgs::sender - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnUIEventArgs_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::UILayoutElement

struct Wrap_UILayoutElement
    : public Wrap_Object
{

    Wrap_UILayoutElement()
    {}
};

static void LnUILayoutElement_delete(Wrap_UILayoutElement* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnUILayoutElement_mark(Wrap_UILayoutElement* obj)
{
	

}

static VALUE LnUILayoutElement_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UILayoutElement* internalObj;

    internalObj = new Wrap_UILayoutElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUILayoutElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUILayoutElement_mark, LnUILayoutElement_delete, internalObj);

    return obj;
}

static VALUE LnUILayoutElement_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UILayoutElement* internalObj;

    internalObj = new Wrap_UILayoutElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUILayoutElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUILayoutElement_mark, LnUILayoutElement_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


LnResult Wrap_LnUILayoutElement_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::UIElement

struct Wrap_UIElement
    : public Wrap_UILayoutElement
{

    Wrap_UIElement()
    {}
};

static void LnUIElement_delete(Wrap_UIElement* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnUIElement_mark(Wrap_UIElement* obj)
{
	

}

static VALUE LnUIElement_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIElement* internalObj;

    internalObj = new Wrap_UIElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUIElement_mark, LnUIElement_delete, internalObj);

    return obj;
}

static VALUE LnUIElement_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIElement* internalObj;

    internalObj = new Wrap_UIElement();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIElement_allocate");
    obj = Data_Wrap_Struct(klass, LnUIElement_mark, LnUIElement_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnUIElement_SetPosition(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE pos;
        rb_scan_args(argc, argv, "1", &pos);
        if (LNRB_VALUE_IS_OBJECT(pos))
        {
            LnVector3* tmp__pos; Data_Get_Struct(pos, LnVector3, tmp__pos);LnVector3& _pos = *tmp__pos;
            LnResult errorCode = LnUIElement_SetPosition(selfObj->handle, &_pos);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnUIElement_SetPositionXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setPosition - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_GetPosition(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnUIElement_GetPosition(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::position - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetRotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE rot;
        rb_scan_args(argc, argv, "1", &rot);
        if (LNRB_VALUE_IS_OBJECT(rot))
        {
            LnQuaternion* tmp__rot; Data_Get_Struct(rot, LnQuaternion, tmp__rot);LnQuaternion& _rot = *tmp__rot;
            LnResult errorCode = LnUIElement_SetRotation(selfObj->handle, &_rot);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setRotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetEulerAngles(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = LNRB_VALUE_TO_FLOAT(z);
            LnResult errorCode = LnUIElement_SetEulerAngles(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setEulerAngles - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_GetRotation(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnQuaternion _outReturn;
            LnResult errorCode = LnUIElement_GetRotation(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnQuaternion_allocate(g_class_Quaternion);
            *((LnQuaternion*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::rotation - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetScale(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE scale;
        rb_scan_args(argc, argv, "1", &scale);
        if (LNRB_VALUE_IS_OBJECT(scale))
        {
            LnVector3* tmp__scale; Data_Get_Struct(scale, LnVector3, tmp__scale);LnVector3& _scale = *tmp__scale;
            LnResult errorCode = LnUIElement_SetScale(selfObj->handle, &_scale);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (1 <= argc && argc <= 1) {
        VALUE xyz;
        rb_scan_args(argc, argv, "1", &xyz);
        if (LNRB_VALUE_IS_FLOAT(xyz))
        {
            float _xyz = LNRB_VALUE_TO_FLOAT(xyz);
            LnResult errorCode = LnUIElement_SetScaleS(selfObj->handle, _xyz);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 2) {
        VALUE x;
        VALUE y;
        rb_scan_args(argc, argv, "2", &x, &y);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            LnResult errorCode = LnUIElement_SetScaleXY(selfObj->handle, _x, _y);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setScale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_GetScale(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnUIElement_GetScale(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::scale - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_SetCenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE value;
        rb_scan_args(argc, argv, "1", &value);
        if (LNRB_VALUE_IS_OBJECT(value))
        {
            LnVector3* tmp__value; Data_Get_Struct(value, LnVector3, tmp__value);LnVector3& _value = *tmp__value;
            LnResult errorCode = LnUIElement_SetCenterPoint(selfObj->handle, &_value);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    if (2 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "21", &x, &y, &z);
        if (LNRB_VALUE_IS_FLOAT(x) && LNRB_VALUE_IS_FLOAT(y) && LNRB_VALUE_IS_FLOAT(z))
        {
            float _x = LNRB_VALUE_TO_FLOAT(x);
            float _y = LNRB_VALUE_TO_FLOAT(y);
            float _z = (z != Qnil) ? LNRB_VALUE_TO_FLOAT(z) : 0.000000;
            LnResult errorCode = LnUIElement_SetCenterPointXYZ(selfObj->handle, _x, _y, _z);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::setCenterPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_GetCenterPoint(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (0 <= argc && argc <= 0) {

        {
            LnVector3 _outReturn;
            LnResult errorCode = LnUIElement_GetCenterPoint(selfObj->handle, &_outReturn);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            VALUE retObj = LnVector3_allocate(g_class_Vector3);
            *((LnVector3*)DATA_PTR(retObj)) = _outReturn;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::centerPoint - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnUIElement_AddChild(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIElement* selfObj;
    Data_Get_Struct(self, Wrap_UIElement, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE child;
        rb_scan_args(argc, argv, "1", &child);
        if (LNRB_VALUE_IS_OBJECT(child))
        {
            LnHandle _child = LuminoRubyRuntimeManager::instance->getHandle(child);
            LnResult errorCode = LnUIElement_AddChild(selfObj->handle, _child);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIElement::addChild - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnUIElement_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::UIControl

struct Wrap_UIControl
    : public Wrap_UIElement
{

    Wrap_UIControl()
    {}
};

static void LnUIControl_delete(Wrap_UIControl* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnUIControl_mark(Wrap_UIControl* obj)
{
	

}

static VALUE LnUIControl_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIControl* internalObj;

    internalObj = new Wrap_UIControl();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIControl_allocate");
    obj = Data_Wrap_Struct(klass, LnUIControl_mark, LnUIControl_delete, internalObj);

    return obj;
}

static VALUE LnUIControl_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIControl* internalObj;

    internalObj = new Wrap_UIControl();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIControl_allocate");
    obj = Data_Wrap_Struct(klass, LnUIControl_mark, LnUIControl_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


LnResult Wrap_LnUIControl_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::UIButtonBase

struct Wrap_UIButtonBase
    : public Wrap_UIControl
{

    Wrap_UIButtonBase()
    {}
};

static void LnUIButtonBase_delete(Wrap_UIButtonBase* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnUIButtonBase_mark(Wrap_UIButtonBase* obj)
{
	

}

static VALUE LnUIButtonBase_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIButtonBase* internalObj;

    internalObj = new Wrap_UIButtonBase();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButtonBase_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButtonBase_mark, LnUIButtonBase_delete, internalObj);

    return obj;
}

static VALUE LnUIButtonBase_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIButtonBase* internalObj;

    internalObj = new Wrap_UIButtonBase();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButtonBase_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButtonBase_mark, LnUIButtonBase_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnUIButtonBase_SetText(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButtonBase* selfObj;
    Data_Get_Struct(self, Wrap_UIButtonBase, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE text;
        rb_scan_args(argc, argv, "1", &text);
        if (LNRB_VALUE_IS_STRING(text))
        {
            const char* _text = LNRB_VALUE_TO_STRING(text);
            LnResult errorCode = LnUIButtonBase_SetTextA(selfObj->handle, _text);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIButtonBase::setText - wrong argument type.");
    return Qnil;
}

LnResult Wrap_LnUIButtonBase_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}

//==============================================================================
// ln::UIButton

struct Wrap_UIButton
    : public Wrap_UIButtonBase
{

    VALUE connectOnClicked_Signal;
    bool connectOnClicked_EventConnect = false;
    Wrap_UIButton()
    {}
};

static void LnUIButton_delete(Wrap_UIButton* obj)
{
    LNRB_SAFE_UNREGISTER_WRAPPER_OBJECT(obj->handle);
    delete obj;
}

static void LnUIButton_mark(Wrap_UIButton* obj)
{
	
rb_gc_mark(obj->connectOnClicked_Signal);

}

static VALUE LnUIButton_allocate(VALUE klass)
{
    VALUE obj;
    Wrap_UIButton* internalObj;

    internalObj = new Wrap_UIButton();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButton_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButton_mark, LnUIButton_delete, internalObj);

    return obj;
}

static VALUE LnUIButton_allocateForGetObject(VALUE klass, LnHandle handle)
{
    VALUE obj;
    Wrap_UIButton* internalObj;

    internalObj = new Wrap_UIButton();
    if (internalObj == NULL) rb_raise(LuminoRubyRuntimeManager::instance->luminoModule(), "Faild alloc - LnUIButton_allocate");
    obj = Data_Wrap_Struct(klass, LnUIButton_mark, LnUIButton_delete, internalObj);
    
    internalObj->handle = handle;
    return obj;
}


static VALUE Wrap_LnUIButton_Create(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(self, Wrap_UIButton, selfObj);
    if (0 <= argc && argc <= 0) {

        {

            LnResult errorCode = LnUIButton_Create(&selfObj->handle);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            LuminoRubyRuntimeManager::instance->registerWrapperObject(self, false);

            if (rb_block_given_p()) rb_yield(self);
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::UIButton::init - wrong argument type.");
    return Qnil;
}

static void Wrap_LnUIButton_ConnectOnClicked_SignalCaller(LnHandle self, LnHandle e)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(LNRB_HANDLE_WRAP_TO_VALUE(self), Wrap_UIButton, selfObj);
    rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("raise"), 1, LNRB_HANDLE_WRAP_TO_VALUE(e));
}

static VALUE Wrap_LnUIButton_ConnectOnClicked(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(self, Wrap_UIButton, selfObj);
    if (!selfObj->connectOnClicked_EventConnect) {  // differed initialization.
        selfObj->connectOnClicked_Signal = rb_funcall(LuminoRubyRuntimeManager::instance->eventSignalClass(), rb_intern("new"), 0);
        LnUIButton_ConnectOnClicked(selfObj->handle, Wrap_LnUIButton_ConnectOnClicked_SignalCaller);
        selfObj->connectOnClicked_EventConnect = true;
    }

    VALUE handler, block;
    rb_scan_args(argc, argv, "01&", &handler, &block);	// (handler=nil, &block)
    if (handler != Qnil) rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("add"), 1, handler);
    if (block != Qnil) rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("add"), 1, block);
    return Qnil;
}

LnResult Wrap_LnUIButton_OnSerialize_OverrideCallback(LnHandle object, LnHandle ar)
{
    VALUE obj = LNRB_HANDLE_WRAP_TO_VALUE(object);
    VALUE retval = rb_funcall(obj, rb_intern("on_serialize"), 1, LNRB_HANDLE_WRAP_TO_VALUE(ar));
    return LN_SUCCESS;
}



extern "C" void Init_Lumino_RubyExt()
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
    rb_define_method(g_class_Vector3, "x", LN_TO_RUBY_FUNC(Wrap_LnVector3_GetX), -1);
    rb_define_method(g_class_Vector3, "x=", LN_TO_RUBY_FUNC(Wrap_LnVector3_SetX), -1);
    rb_define_method(g_class_Vector3, "y", LN_TO_RUBY_FUNC(Wrap_LnVector3_GetY), -1);
    rb_define_method(g_class_Vector3, "y=", LN_TO_RUBY_FUNC(Wrap_LnVector3_SetY), -1);
    rb_define_method(g_class_Vector3, "z", LN_TO_RUBY_FUNC(Wrap_LnVector3_GetZ), -1);
    rb_define_method(g_class_Vector3, "z=", LN_TO_RUBY_FUNC(Wrap_LnVector3_SetZ), -1);
    rb_define_method(g_class_Vector3, "initialize", LN_TO_RUBY_FUNC(Wrap_LnVector3_Set), -1);
    rb_define_method(g_class_Vector3, "length", LN_TO_RUBY_FUNC(Wrap_LnVector3_Length), -1);
    rb_define_method(g_class_Vector3, "length_squared", LN_TO_RUBY_FUNC(Wrap_LnVector3_LengthSquared), -1);
    rb_define_method(g_class_Vector3, "mutating_normalize", LN_TO_RUBY_FUNC(Wrap_LnVector3_MutatingNormalize), -1);
    rb_define_method(g_class_Vector3, "normalize", LN_TO_RUBY_FUNC(Wrap_LnVector3_Normalize), -1);

    g_class_Vector4 = rb_define_class_under(g_rootModule, "Vector4", rb_cObject);
    rb_define_alloc_func(g_class_Vector4, LnVector4_allocate);
    rb_define_method(g_class_Vector4, "x", LN_TO_RUBY_FUNC(Wrap_LnVector4_GetX), -1);
    rb_define_method(g_class_Vector4, "x=", LN_TO_RUBY_FUNC(Wrap_LnVector4_SetX), -1);
    rb_define_method(g_class_Vector4, "y", LN_TO_RUBY_FUNC(Wrap_LnVector4_GetY), -1);
    rb_define_method(g_class_Vector4, "y=", LN_TO_RUBY_FUNC(Wrap_LnVector4_SetY), -1);
    rb_define_method(g_class_Vector4, "z", LN_TO_RUBY_FUNC(Wrap_LnVector4_GetZ), -1);
    rb_define_method(g_class_Vector4, "z=", LN_TO_RUBY_FUNC(Wrap_LnVector4_SetZ), -1);
    rb_define_method(g_class_Vector4, "w", LN_TO_RUBY_FUNC(Wrap_LnVector4_GetW), -1);
    rb_define_method(g_class_Vector4, "w=", LN_TO_RUBY_FUNC(Wrap_LnVector4_SetW), -1);
    rb_define_method(g_class_Vector4, "initialize", LN_TO_RUBY_FUNC(Wrap_LnVector4_Set), -1);

    g_class_Quaternion = rb_define_class_under(g_rootModule, "Quaternion", rb_cObject);
    rb_define_alloc_func(g_class_Quaternion, LnQuaternion_allocate);
    rb_define_method(g_class_Quaternion, "x", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_GetX), -1);
    rb_define_method(g_class_Quaternion, "x=", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_SetX), -1);
    rb_define_method(g_class_Quaternion, "y", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_GetY), -1);
    rb_define_method(g_class_Quaternion, "y=", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_SetY), -1);
    rb_define_method(g_class_Quaternion, "z", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_GetZ), -1);
    rb_define_method(g_class_Quaternion, "z=", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_SetZ), -1);
    rb_define_method(g_class_Quaternion, "w", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_GetW), -1);
    rb_define_method(g_class_Quaternion, "w=", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_SetW), -1);
    rb_define_method(g_class_Quaternion, "initialize", LN_TO_RUBY_FUNC(Wrap_LnQuaternion_Set), -1);

    g_class_Matrix = rb_define_class_under(g_rootModule, "Matrix", rb_cObject);
    rb_define_alloc_func(g_class_Matrix, LnMatrix_allocate);
    rb_define_method(g_class_Matrix, "row_0", LN_TO_RUBY_FUNC(Wrap_LnMatrix_GetRow0), -1);
    rb_define_method(g_class_Matrix, "row_0=", LN_TO_RUBY_FUNC(Wrap_LnMatrix_SetRow0), -1);
    rb_define_method(g_class_Matrix, "row_1", LN_TO_RUBY_FUNC(Wrap_LnMatrix_GetRow1), -1);
    rb_define_method(g_class_Matrix, "row_1=", LN_TO_RUBY_FUNC(Wrap_LnMatrix_SetRow1), -1);
    rb_define_method(g_class_Matrix, "row_2", LN_TO_RUBY_FUNC(Wrap_LnMatrix_GetRow2), -1);
    rb_define_method(g_class_Matrix, "row_2=", LN_TO_RUBY_FUNC(Wrap_LnMatrix_SetRow2), -1);
    rb_define_method(g_class_Matrix, "row_3", LN_TO_RUBY_FUNC(Wrap_LnMatrix_GetRow3), -1);
    rb_define_method(g_class_Matrix, "row_3=", LN_TO_RUBY_FUNC(Wrap_LnMatrix_SetRow3), -1);
    rb_define_method(g_class_Matrix, "initialize", LN_TO_RUBY_FUNC(Wrap_LnMatrix_Set), -1);

    g_class_Object = rb_define_class_under(g_rootModule, "Object", rb_cObject);
    rb_define_alloc_func(g_class_Object, LnObject_allocate);
    rb_define_method(g_class_Object, "on_serialize", LN_TO_RUBY_FUNC(Wrap_LnObject_OnSerialize), -1);
    LnObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Object, LnObject_allocateForGetObject));
    LnObject_OnSerialize_SetOverrideCallback(Wrap_LnObject_OnSerialize_OverrideCallback);

    g_class_PromiseFailureDelegate = rb_define_class_under(g_rootModule, "PromiseFailureDelegate", g_class_Object);
    rb_define_alloc_func(g_class_PromiseFailureDelegate, LnPromiseFailureDelegate_allocate);
    rb_define_private_method(g_class_PromiseFailureDelegate, "initialize", LN_TO_RUBY_FUNC(Wrap_LnPromiseFailureDelegate_Create), -1);
    LnPromiseFailureDelegate_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_PromiseFailureDelegate, LnPromiseFailureDelegate_allocateForGetObject));

    g_class_ZVTestDelegate1 = rb_define_class_under(g_rootModule, "ZVTestDelegate1", g_class_Object);
    rb_define_alloc_func(g_class_ZVTestDelegate1, LnZVTestDelegate1_allocate);
    rb_define_private_method(g_class_ZVTestDelegate1, "initialize", LN_TO_RUBY_FUNC(Wrap_LnZVTestDelegate1_Create), -1);
    LnZVTestDelegate1_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_ZVTestDelegate1, LnZVTestDelegate1_allocateForGetObject));

    g_class_ZVTestDelegate2 = rb_define_class_under(g_rootModule, "ZVTestDelegate2", g_class_Object);
    rb_define_alloc_func(g_class_ZVTestDelegate2, LnZVTestDelegate2_allocate);
    rb_define_private_method(g_class_ZVTestDelegate2, "initialize", LN_TO_RUBY_FUNC(Wrap_LnZVTestDelegate2_Create), -1);
    LnZVTestDelegate2_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_ZVTestDelegate2, LnZVTestDelegate2_allocateForGetObject));

    g_class_ZVTestDelegate3 = rb_define_class_under(g_rootModule, "ZVTestDelegate3", g_class_Object);
    rb_define_alloc_func(g_class_ZVTestDelegate3, LnZVTestDelegate3_allocate);
    rb_define_private_method(g_class_ZVTestDelegate3, "initialize", LN_TO_RUBY_FUNC(Wrap_LnZVTestDelegate3_Create), -1);
    LnZVTestDelegate3_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_ZVTestDelegate3, LnZVTestDelegate3_allocateForGetObject));

    g_class_ZVTestPromise1 = rb_define_class_under(g_rootModule, "ZVTestPromise1", g_class_Object);
    rb_define_alloc_func(g_class_ZVTestPromise1, LnZVTestPromise1_allocate);
    rb_define_method(g_class_ZVTestPromise1, "then_with", LN_TO_RUBY_FUNC(Wrap_LnZVTestPromise1_ThenWith), -1);
    rb_define_method(g_class_ZVTestPromise1, "catch_with", LN_TO_RUBY_FUNC(Wrap_LnZVTestPromise1_CatchWith), -1);
    LnZVTestPromise1_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_ZVTestPromise1, LnZVTestPromise1_allocateForGetObject));

    g_class_ZVTestPromise2 = rb_define_class_under(g_rootModule, "ZVTestPromise2", g_class_Object);
    rb_define_alloc_func(g_class_ZVTestPromise2, LnZVTestPromise2_allocate);
    rb_define_method(g_class_ZVTestPromise2, "then_with", LN_TO_RUBY_FUNC(Wrap_LnZVTestPromise2_ThenWith), -1);
    rb_define_method(g_class_ZVTestPromise2, "catch_with", LN_TO_RUBY_FUNC(Wrap_LnZVTestPromise2_CatchWith), -1);
    LnZVTestPromise2_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_ZVTestPromise2, LnZVTestPromise2_allocateForGetObject));

    g_class_ZVTestClass1 = rb_define_class_under(g_rootModule, "ZVTestClass1", g_class_Object);
    rb_define_alloc_func(g_class_ZVTestClass1, LnZVTestClass1_allocate);
    rb_define_method(g_class_ZVTestClass1, "set_test_delegate_1", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_SetTestDelegate1), -1);
    rb_define_method(g_class_ZVTestClass1, "set_test_delegate_2", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_SetTestDelegate2), -1);
    rb_define_method(g_class_ZVTestClass1, "set_test_delegate_3", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_SetTestDelegate3), -1);
    rb_define_method(g_class_ZVTestClass1, "call_test_delegate_1", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_CallTestDelegate1), -1);
    rb_define_method(g_class_ZVTestClass1, "call_test_delegate_2", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_CallTestDelegate2), -1);
    rb_define_method(g_class_ZVTestClass1, "call_test_delegate_3", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_CallTestDelegate3), -1);
    rb_define_singleton_method(g_class_ZVTestClass1, "load_async", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_LoadAsync), -1);
    rb_define_method(g_class_ZVTestClass1, "execute_async", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_ExecuteAsync), -1);
    rb_define_method(g_class_ZVTestClass1, "file_path", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_GetFilePath), -1);
    rb_define_private_method(g_class_ZVTestClass1, "initialize", LN_TO_RUBY_FUNC(Wrap_LnZVTestClass1_Create), -1);
    LnZVTestClass1_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_ZVTestClass1, LnZVTestClass1_allocateForGetObject));
    LnZVTestClass1_OnSerialize_SetOverrideCallback(Wrap_LnZVTestClass1_OnSerialize_OverrideCallback);

    g_class_Serializer = rb_define_class_under(g_rootModule, "Serializer", g_class_Object);
    rb_define_alloc_func(g_class_Serializer, LnSerializer_allocate);
    rb_define_method(g_class_Serializer, "write_bool", LN_TO_RUBY_FUNC(Wrap_LnSerializer_WriteBool), -1);
    rb_define_method(g_class_Serializer, "write_int", LN_TO_RUBY_FUNC(Wrap_LnSerializer_WriteInt), -1);
    rb_define_method(g_class_Serializer, "write_float", LN_TO_RUBY_FUNC(Wrap_LnSerializer_WriteFloat), -1);
    rb_define_method(g_class_Serializer, "write_string", LN_TO_RUBY_FUNC(Wrap_LnSerializer_WriteString), -1);
    rb_define_method(g_class_Serializer, "write_object", LN_TO_RUBY_FUNC(Wrap_LnSerializer_WriteObject), -1);
    rb_define_method(g_class_Serializer, "read_bool", LN_TO_RUBY_FUNC(Wrap_LnSerializer_ReadBool), -1);
    rb_define_method(g_class_Serializer, "read_int", LN_TO_RUBY_FUNC(Wrap_LnSerializer_ReadInt), -1);
    rb_define_method(g_class_Serializer, "read_float", LN_TO_RUBY_FUNC(Wrap_LnSerializer_ReadFloat), -1);
    rb_define_method(g_class_Serializer, "read_string", LN_TO_RUBY_FUNC(Wrap_LnSerializer_ReadString), -1);
    rb_define_method(g_class_Serializer, "read_object", LN_TO_RUBY_FUNC(Wrap_LnSerializer_ReadObject), -1);
    rb_define_singleton_method(g_class_Serializer, "serialize", LN_TO_RUBY_FUNC(Wrap_LnSerializer_Serialize), -1);
    rb_define_singleton_method(g_class_Serializer, "deserialize", LN_TO_RUBY_FUNC(Wrap_LnSerializer_Deserialize), -1);
    LnSerializer_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Serializer, LnSerializer_allocateForGetObject));
    LnSerializer_OnSerialize_SetOverrideCallback(Wrap_LnSerializer_OnSerialize_OverrideCallback);

    g_class_AssetModel = rb_define_class_under(g_rootModule, "AssetModel", g_class_Object);
    rb_define_alloc_func(g_class_AssetModel, LnAssetModel_allocate);
    rb_define_method(g_class_AssetModel, "target", LN_TO_RUBY_FUNC(Wrap_LnAssetModel_Target), -1);
    rb_define_private_method(g_class_AssetModel, "initialize", LN_TO_RUBY_FUNC(Wrap_LnAssetModel_Create), -1);
    LnAssetModel_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_AssetModel, LnAssetModel_allocateForGetObject));
    LnAssetModel_OnSerialize_SetOverrideCallback(Wrap_LnAssetModel_OnSerialize_OverrideCallback);

    g_class_Assets = rb_define_class_under(g_rootModule, "Assets", rb_cObject);
    rb_define_singleton_method(g_class_Assets, "save_asset_to_local_file", LN_TO_RUBY_FUNC(Wrap_LnAssets_SaveAssetToLocalFile), -1);
    rb_define_singleton_method(g_class_Assets, "load_asset_from_local_file", LN_TO_RUBY_FUNC(Wrap_LnAssets_LoadAssetFromLocalFile), -1);
    rb_define_singleton_method(g_class_Assets, "load_asset", LN_TO_RUBY_FUNC(Wrap_LnAssets_LoadAsset), -1);

    g_class_EngineSettings = rb_define_class_under(g_rootModule, "EngineSettings", rb_cObject);
    rb_define_singleton_method(g_class_EngineSettings, "set_main_window_size", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_SetMainWindowSize), -1);
    rb_define_singleton_method(g_class_EngineSettings, "set_main_world_view_size", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_SetMainWorldViewSize), -1);
    rb_define_singleton_method(g_class_EngineSettings, "set_main_window_title", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_SetMainWindowTitle), -1);
    rb_define_singleton_method(g_class_EngineSettings, "add_asset_directory", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_AddAssetDirectory), -1);
    rb_define_singleton_method(g_class_EngineSettings, "add_asset_archive", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_AddAssetArchive), -1);
    rb_define_singleton_method(g_class_EngineSettings, "set_engine_log_enabled", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_SetEngineLogEnabled), -1);
    rb_define_singleton_method(g_class_EngineSettings, "set_engine_log_file_path", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_SetEngineLogFilePath), -1);

    g_class_Engine = rb_define_class_under(g_rootModule, "Engine", rb_cObject);
    rb_define_singleton_method(g_class_Engine, "initialize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Initialize), -1);
    rb_define_singleton_method(g_class_Engine, "finalize", LN_TO_RUBY_FUNC(Wrap_LnEngine_Finalize), -1);
    rb_define_singleton_method(g_class_Engine, "update", LN_TO_RUBY_FUNC(Wrap_LnEngine_Update), -1);
    rb_define_singleton_method(g_class_Engine, "main_ui_view", LN_TO_RUBY_FUNC(Wrap_LnEngine_MainUIView), -1);
    rb_define_singleton_method(g_class_Engine, "world", LN_TO_RUBY_FUNC(Wrap_LnEngine_GetWorld), -1);

    g_class_Application = rb_define_class_under(g_rootModule, "Application", g_class_Object);
    rb_define_alloc_func(g_class_Application, LnApplication_allocate);
    rb_define_method(g_class_Application, "on_init", LN_TO_RUBY_FUNC(Wrap_LnApplication_OnInit), -1);
    rb_define_method(g_class_Application, "on_update", LN_TO_RUBY_FUNC(Wrap_LnApplication_OnUpdate), -1);
    rb_define_private_method(g_class_Application, "initialize", LN_TO_RUBY_FUNC(Wrap_LnApplication_Create), -1);
    LnApplication_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Application, LnApplication_allocateForGetObject));
    LnApplication_OnSerialize_SetOverrideCallback(Wrap_LnApplication_OnSerialize_OverrideCallback);
    LnApplication_OnInit_SetOverrideCallback(Wrap_LnApplication_OnInit_OverrideCallback);
    LnApplication_OnUpdate_SetOverrideCallback(Wrap_LnApplication_OnUpdate_OverrideCallback);

    g_class_GraphicsResource = rb_define_class_under(g_rootModule, "GraphicsResource", g_class_Object);
    rb_define_alloc_func(g_class_GraphicsResource, LnGraphicsResource_allocate);
    LnGraphicsResource_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_GraphicsResource, LnGraphicsResource_allocateForGetObject));
    LnGraphicsResource_OnSerialize_SetOverrideCallback(Wrap_LnGraphicsResource_OnSerialize_OverrideCallback);

    g_class_Texture = rb_define_class_under(g_rootModule, "Texture", g_class_GraphicsResource);
    rb_define_alloc_func(g_class_Texture, LnTexture_allocate);
    LnTexture_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Texture, LnTexture_allocateForGetObject));
    LnTexture_OnSerialize_SetOverrideCallback(Wrap_LnTexture_OnSerialize_OverrideCallback);

    g_class_Texture2D = rb_define_class_under(g_rootModule, "Texture2D", g_class_Texture);
    rb_define_alloc_func(g_class_Texture2D, LnTexture2D_allocate);
    rb_define_singleton_method(g_class_Texture2D, "load", LN_TO_RUBY_FUNC(Wrap_LnTexture2D_Load), -1);
    rb_define_singleton_method(g_class_Texture2D, "load_emoji", LN_TO_RUBY_FUNC(Wrap_LnTexture2D_LoadEmoji), -1);
    rb_define_private_method(g_class_Texture2D, "initialize", LN_TO_RUBY_FUNC(Wrap_LnTexture2D_Create), -1);
    LnTexture2D_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Texture2D, LnTexture2D_allocateForGetObject));
    LnTexture2D_OnSerialize_SetOverrideCallback(Wrap_LnTexture2D_OnSerialize_OverrideCallback);

    g_class_Component = rb_define_class_under(g_rootModule, "Component", g_class_Object);
    rb_define_alloc_func(g_class_Component, LnComponent_allocate);
    LnComponent_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Component, LnComponent_allocateForGetObject));
    LnComponent_OnSerialize_SetOverrideCallback(Wrap_LnComponent_OnSerialize_OverrideCallback);

    g_class_VisualComponent = rb_define_class_under(g_rootModule, "VisualComponent", g_class_Component);
    rb_define_alloc_func(g_class_VisualComponent, LnVisualComponent_allocate);
    rb_define_method(g_class_VisualComponent, "visible=", LN_TO_RUBY_FUNC(Wrap_LnVisualComponent_SetVisible), -1);
    rb_define_method(g_class_VisualComponent, "visible?", LN_TO_RUBY_FUNC(Wrap_LnVisualComponent_IsVisible), -1);
    LnVisualComponent_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_VisualComponent, LnVisualComponent_allocateForGetObject));
    LnVisualComponent_OnSerialize_SetOverrideCallback(Wrap_LnVisualComponent_OnSerialize_OverrideCallback);

    g_class_SpriteComponent = rb_define_class_under(g_rootModule, "SpriteComponent", g_class_VisualComponent);
    rb_define_alloc_func(g_class_SpriteComponent, LnSpriteComponent_allocate);
    rb_define_method(g_class_SpriteComponent, "texture=", LN_TO_RUBY_FUNC(Wrap_LnSpriteComponent_SetTexture), -1);
    LnSpriteComponent_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_SpriteComponent, LnSpriteComponent_allocateForGetObject));
    LnSpriteComponent_OnSerialize_SetOverrideCallback(Wrap_LnSpriteComponent_OnSerialize_OverrideCallback);

    g_class_World = rb_define_class_under(g_rootModule, "World", g_class_Object);
    rb_define_alloc_func(g_class_World, LnWorld_allocate);
    rb_define_method(g_class_World, "add", LN_TO_RUBY_FUNC(Wrap_LnWorld_Add), -1);
    LnWorld_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_World, LnWorld_allocateForGetObject));
    LnWorld_OnSerialize_SetOverrideCallback(Wrap_LnWorld_OnSerialize_OverrideCallback);

    g_class_ComponentList = rb_define_class_under(g_rootModule, "ComponentList", g_class_Object);
    rb_define_alloc_func(g_class_ComponentList, LnComponentList_allocate);
    rb_define_method(g_class_ComponentList, "get_length", LN_TO_RUBY_FUNC(Wrap_LnComponentList_GetLength), -1);
    rb_define_method(g_class_ComponentList, "get_item", LN_TO_RUBY_FUNC(Wrap_LnComponentList_GetItem), -1);
    LnComponentList_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_ComponentList, LnComponentList_allocateForGetObject));
    LnComponentList_OnSerialize_SetOverrideCallback(Wrap_LnComponentList_OnSerialize_OverrideCallback);

    g_class_WorldObject = rb_define_class_under(g_rootModule, "WorldObject", g_class_Object);
    rb_define_alloc_func(g_class_WorldObject, LnWorldObject_allocate);
    rb_define_method(g_class_WorldObject, "position=", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetPosition), -1);
    rb_define_method(g_class_WorldObject, "position", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_GetPosition), -1);
    rb_define_method(g_class_WorldObject, "rotation=", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetRotation), -1);
    rb_define_method(g_class_WorldObject, "set_euler_angles", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetEulerAngles), -1);
    rb_define_method(g_class_WorldObject, "rotation", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_GetRotation), -1);
    rb_define_method(g_class_WorldObject, "scale=", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetScale), -1);
    rb_define_method(g_class_WorldObject, "scale", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_GetScale), -1);
    rb_define_method(g_class_WorldObject, "center_point=", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_SetCenterPoint), -1);
    rb_define_method(g_class_WorldObject, "center_point", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_GetCenterPoint), -1);
    rb_define_method(g_class_WorldObject, "components", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_GetComponents), -1);
    rb_define_method(g_class_WorldObject, "on_update", LN_TO_RUBY_FUNC(Wrap_LnWorldObject_OnUpdate), -1);
    LnWorldObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_WorldObject, LnWorldObject_allocateForGetObject));
    LnWorldObject_OnSerialize_SetOverrideCallback(Wrap_LnWorldObject_OnSerialize_OverrideCallback);
    LnWorldObject_OnUpdate_SetOverrideCallback(Wrap_LnWorldObject_OnUpdate_OverrideCallback);

    g_class_VisualObject = rb_define_class_under(g_rootModule, "VisualObject", g_class_WorldObject);
    rb_define_alloc_func(g_class_VisualObject, LnVisualObject_allocate);
    rb_define_method(g_class_VisualObject, "visible=", LN_TO_RUBY_FUNC(Wrap_LnVisualObject_SetVisible), -1);
    rb_define_method(g_class_VisualObject, "visible?", LN_TO_RUBY_FUNC(Wrap_LnVisualObject_IsVisible), -1);
    LnVisualObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_VisualObject, LnVisualObject_allocateForGetObject));
    LnVisualObject_OnSerialize_SetOverrideCallback(Wrap_LnVisualObject_OnSerialize_OverrideCallback);
    LnVisualObject_OnUpdate_SetOverrideCallback(Wrap_LnVisualObject_OnUpdate_OverrideCallback);

    g_class_TestDelegate = rb_define_class_under(g_rootModule, "TestDelegate", g_class_Object);
    rb_define_alloc_func(g_class_TestDelegate, LnTestDelegate_allocate);
    rb_define_private_method(g_class_TestDelegate, "initialize", LN_TO_RUBY_FUNC(Wrap_LnTestDelegate_Create), -1);
    LnTestDelegate_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_TestDelegate, LnTestDelegate_allocateForGetObject));

    g_class_Sprite = rb_define_class_under(g_rootModule, "Sprite", g_class_VisualObject);
    rb_define_alloc_func(g_class_Sprite, LnSprite_allocate);
    rb_define_method(g_class_Sprite, "texture=", LN_TO_RUBY_FUNC(Wrap_LnSprite_SetTexture), -1);
    rb_define_method(g_class_Sprite, "set_source_rect", LN_TO_RUBY_FUNC(Wrap_LnSprite_SetSourceRectXYWH), -1);
    rb_define_method(g_class_Sprite, "set_caller_test", LN_TO_RUBY_FUNC(Wrap_LnSprite_SetCallerTest), -1);
    rb_define_private_method(g_class_Sprite, "initialize", LN_TO_RUBY_FUNC(Wrap_LnSprite_Create), -1);
    LnSprite_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Sprite, LnSprite_allocateForGetObject));
    LnSprite_OnSerialize_SetOverrideCallback(Wrap_LnSprite_OnSerialize_OverrideCallback);
    LnSprite_OnUpdate_SetOverrideCallback(Wrap_LnSprite_OnUpdate_OverrideCallback);

    g_class_UIEventArgs = rb_define_class_under(g_rootModule, "UIEventArgs", g_class_Object);
    rb_define_alloc_func(g_class_UIEventArgs, LnUIEventArgs_allocate);
    rb_define_method(g_class_UIEventArgs, "sender", LN_TO_RUBY_FUNC(Wrap_LnUIEventArgs_Sender), -1);
    LnUIEventArgs_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIEventArgs, LnUIEventArgs_allocateForGetObject));
    LnUIEventArgs_OnSerialize_SetOverrideCallback(Wrap_LnUIEventArgs_OnSerialize_OverrideCallback);

    g_class_UILayoutElement = rb_define_class_under(g_rootModule, "UILayoutElement", g_class_Object);
    rb_define_alloc_func(g_class_UILayoutElement, LnUILayoutElement_allocate);
    LnUILayoutElement_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UILayoutElement, LnUILayoutElement_allocateForGetObject));
    LnUILayoutElement_OnSerialize_SetOverrideCallback(Wrap_LnUILayoutElement_OnSerialize_OverrideCallback);

    g_class_UIElement = rb_define_class_under(g_rootModule, "UIElement", g_class_UILayoutElement);
    rb_define_alloc_func(g_class_UIElement, LnUIElement_allocate);
    rb_define_method(g_class_UIElement, "position=", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetPosition), -1);
    rb_define_method(g_class_UIElement, "position", LN_TO_RUBY_FUNC(Wrap_LnUIElement_GetPosition), -1);
    rb_define_method(g_class_UIElement, "rotation=", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetRotation), -1);
    rb_define_method(g_class_UIElement, "set_euler_angles", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetEulerAngles), -1);
    rb_define_method(g_class_UIElement, "rotation", LN_TO_RUBY_FUNC(Wrap_LnUIElement_GetRotation), -1);
    rb_define_method(g_class_UIElement, "scale=", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetScale), -1);
    rb_define_method(g_class_UIElement, "scale", LN_TO_RUBY_FUNC(Wrap_LnUIElement_GetScale), -1);
    rb_define_method(g_class_UIElement, "center_point=", LN_TO_RUBY_FUNC(Wrap_LnUIElement_SetCenterPoint), -1);
    rb_define_method(g_class_UIElement, "center_point", LN_TO_RUBY_FUNC(Wrap_LnUIElement_GetCenterPoint), -1);
    rb_define_method(g_class_UIElement, "add_child", LN_TO_RUBY_FUNC(Wrap_LnUIElement_AddChild), -1);
    LnUIElement_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIElement, LnUIElement_allocateForGetObject));
    LnUIElement_OnSerialize_SetOverrideCallback(Wrap_LnUIElement_OnSerialize_OverrideCallback);

    g_class_UIControl = rb_define_class_under(g_rootModule, "UIControl", g_class_UIElement);
    rb_define_alloc_func(g_class_UIControl, LnUIControl_allocate);
    LnUIControl_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIControl, LnUIControl_allocateForGetObject));
    LnUIControl_OnSerialize_SetOverrideCallback(Wrap_LnUIControl_OnSerialize_OverrideCallback);

    g_class_UIButtonBase = rb_define_class_under(g_rootModule, "UIButtonBase", g_class_UIControl);
    rb_define_alloc_func(g_class_UIButtonBase, LnUIButtonBase_allocate);
    rb_define_method(g_class_UIButtonBase, "set_text", LN_TO_RUBY_FUNC(Wrap_LnUIButtonBase_SetText), -1);
    LnUIButtonBase_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIButtonBase, LnUIButtonBase_allocateForGetObject));
    LnUIButtonBase_OnSerialize_SetOverrideCallback(Wrap_LnUIButtonBase_OnSerialize_OverrideCallback);

    g_class_UIButton = rb_define_class_under(g_rootModule, "UIButton", g_class_UIButtonBase);
    rb_define_alloc_func(g_class_UIButton, LnUIButton_allocate);
    rb_define_private_method(g_class_UIButton, "initialize", LN_TO_RUBY_FUNC(Wrap_LnUIButton_Create), -1);
    rb_define_method(g_class_UIButton, "connect_on_clicked", LN_TO_RUBY_FUNC(Wrap_LnUIButton_ConnectOnClicked), -1);
    LnUIButton_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_UIButton, LnUIButton_allocateForGetObject));
    LnUIButton_OnSerialize_SetOverrideCallback(Wrap_LnUIButton_OnSerialize_OverrideCallback);

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

