#include "LuminoRuby.h"
#include "RubyStructs.h"

VALUE g_struct_Vector2;
VALUE g_struct_Vector3;
VALUE g_struct_Vector4;
VALUE g_struct_Matrix;
VALUE g_struct_Quaternion;



void LNVector2_delete(LNVector2* obj)
{
    free(obj);
}

VALUE LNVector2_allocate( VALUE klass )
{
    VALUE obj;
    LNVector2* internalObj;

    internalObj = (LNVector2*)malloc(sizeof(LNVector2));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNVector2_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNVector2_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNVector2));

    return obj;
}

VALUE LNVector2_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    if (argc == 0) {    // 引数 0 個を許可する
        return self;
    }
    VALUE X_;
    VALUE Y_;
    rb_scan_args(argc, argv, "2", &X_, &Y_);
    selfObj->X = ((float)NUM2DBL(X_));
    selfObj->Y = ((float)NUM2DBL(Y_));
    return self;
}
static VALUE LNVector2_X_set(VALUE self, VALUE v)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    selfObj->X = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector2_X_get(VALUE self)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    return toVALUE(selfObj->X);
}

static VALUE LNVector2_Y_set(VALUE self, VALUE v)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    selfObj->Y = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector2_Y_get(VALUE self)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    return toVALUE(selfObj->Y);
}

static VALUE lnrbLNVector2_GetLength(int argc, VALUE *argv, VALUE self)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            float _outLength;
            LNResult errorCode = LNVector2_GetLength(selfObj, &_outLength);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return toVALUE(_outLength);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector2.length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector2_GetSquareLength(int argc, VALUE *argv, VALUE self)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            float _outLength;
            LNResult errorCode = LNVector2_GetSquareLength(selfObj, &_outLength);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return toVALUE(_outLength);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector2.square_length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector2_Set(int argc, VALUE *argv, VALUE self)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE x;
        VALUE y;
        rb_scan_args(argc, argv, "2", &x, &y);
        if (isRbFloat(x) && isRbFloat(y)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            LNResult errorCode = LNVector2_Set(selfObj, _x, _y);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector2.set - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector2_Normalize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            LNVector2* tmp__vec; Data_Get_Struct(vec, LNVector2, tmp__vec);LNVector2& _vec = *tmp__vec;
            LNVector2 _outVec;
            LNResult errorCode = LNVector2_Normalize(&_vec, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector2_allocate(g_struct_Vector2);
    *((LNVector2*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector2.normalize - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector2_NormalizeV(int argc, VALUE *argv, VALUE self)
{
    LNVector2* selfObj;
    Data_Get_Struct(self, LNVector2, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNVector2_NormalizeV(selfObj);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector2.normalize - wrong argument type.");
    return Qnil;
}


void LNVector3_delete(LNVector3* obj)
{
    free(obj);
}

VALUE LNVector3_allocate( VALUE klass )
{
    VALUE obj;
    LNVector3* internalObj;

    internalObj = (LNVector3*)malloc(sizeof(LNVector3));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNVector3_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNVector3_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNVector3));

    return obj;
}

VALUE LNVector3_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    if (argc == 0) {    // 引数 0 個を許可する
        return self;
    }
    VALUE X_;
    VALUE Y_;
    VALUE Z_;
    rb_scan_args(argc, argv, "3", &X_, &Y_, &Z_);
    selfObj->X = ((float)NUM2DBL(X_));
    selfObj->Y = ((float)NUM2DBL(Y_));
    selfObj->Z = ((float)NUM2DBL(Z_));
    return self;
}
static VALUE LNVector3_X_set(VALUE self, VALUE v)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    selfObj->X = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector3_X_get(VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    return toVALUE(selfObj->X);
}

static VALUE LNVector3_Y_set(VALUE self, VALUE v)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    selfObj->Y = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector3_Y_get(VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    return toVALUE(selfObj->Y);
}

static VALUE LNVector3_Z_set(VALUE self, VALUE v)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    selfObj->Z = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector3_Z_get(VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    return toVALUE(selfObj->Z);
}

static VALUE lnrbLNVector3_GetLength(int argc, VALUE *argv, VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            float _outLength;
            LNResult errorCode = LNVector3_GetLength(selfObj, &_outLength);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return toVALUE(_outLength);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_GetSquareLength(int argc, VALUE *argv, VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            float _outLength;
            LNResult errorCode = LNVector3_GetSquareLength(selfObj, &_outLength);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return toVALUE(_outLength);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.square_length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Set(int argc, VALUE *argv, VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNResult errorCode = LNVector3_Set(selfObj, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.set - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_SetVZ(int argc, VALUE *argv, VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE vec2;
        VALUE z;
        rb_scan_args(argc, argv, "2", &vec2, &z);
        if (isRbObject(vec2) && isRbFloat(z)) {
            LNVector2* tmp__vec2; Data_Get_Struct(vec2, LNVector2, tmp__vec2);LNVector2& _vec2 = *tmp__vec2;
            float _z = ((float)NUM2DBL(z));
            LNResult errorCode = LNVector3_SetVZ(selfObj, &_vec2, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.set_vz - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_Normalize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            LNVector3* tmp__vec; Data_Get_Struct(vec, LNVector3, tmp__vec);LNVector3& _vec = *tmp__vec;
            LNVector3 _outVec;
            LNResult errorCode = LNVector3_Normalize(&_vec, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.normalize - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_NormalizeV(int argc, VALUE *argv, VALUE self)
{
    LNVector3* selfObj;
    Data_Get_Struct(self, LNVector3, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNVector3_NormalizeV(selfObj);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.normalize - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_Dot(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec1;
        VALUE vec2;
        rb_scan_args(argc, argv, "2", &vec1, &vec2);
        if (isRbObject(vec1) && isRbObject(vec2)) {
            LNVector3* tmp__vec1; Data_Get_Struct(vec1, LNVector3, tmp__vec1);LNVector3& _vec1 = *tmp__vec1;
            LNVector3* tmp__vec2; Data_Get_Struct(vec2, LNVector3, tmp__vec2);LNVector3& _vec2 = *tmp__vec2;
            float _dot;
            LNResult errorCode = LNVector3_Dot(&_vec1, &_vec2, &_dot);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return toVALUE(_dot);
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.dot - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_Cross(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec1;
        VALUE vec2;
        rb_scan_args(argc, argv, "2", &vec1, &vec2);
        if (isRbObject(vec1) && isRbObject(vec2)) {
            LNVector3* tmp__vec1; Data_Get_Struct(vec1, LNVector3, tmp__vec1);LNVector3& _vec1 = *tmp__vec1;
            LNVector3* tmp__vec2; Data_Get_Struct(vec2, LNVector3, tmp__vec2);LNVector3& _vec2 = *tmp__vec2;
            LNVector3 _outVec;
            LNResult errorCode = LNVector3_Cross(&_vec1, &_vec2, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.cross - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_Reflect(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE normal;
        rb_scan_args(argc, argv, "2", &vec, &normal);
        if (isRbObject(vec) && isRbObject(normal)) {
            LNVector3* tmp__vec; Data_Get_Struct(vec, LNVector3, tmp__vec);LNVector3& _vec = *tmp__vec;
            LNVector3* tmp__normal; Data_Get_Struct(normal, LNVector3, tmp__normal);LNVector3& _normal = *tmp__normal;
            LNVector3 _outVec;
            LNResult errorCode = LNVector3_Reflect(&_vec, &_normal, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.reflect - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_Slide(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE normal;
        rb_scan_args(argc, argv, "2", &vec, &normal);
        if (isRbObject(vec) && isRbObject(normal)) {
            LNVector3* tmp__vec; Data_Get_Struct(vec, LNVector3, tmp__vec);LNVector3& _vec = *tmp__vec;
            LNVector3* tmp__normal; Data_Get_Struct(normal, LNVector3, tmp__normal);LNVector3& _normal = *tmp__normal;
            LNVector3 _outVec;
            LNResult errorCode = LNVector3_Slide(&_vec, &_normal, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.slide - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_Lerp(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE vec1;
        VALUE vec2;
        VALUE t;
        rb_scan_args(argc, argv, "3", &vec1, &vec2, &t);
        if (isRbObject(vec1) && isRbObject(vec2) && isRbFloat(t)) {
            LNVector3* tmp__vec1; Data_Get_Struct(vec1, LNVector3, tmp__vec1);LNVector3& _vec1 = *tmp__vec1;
            LNVector3* tmp__vec2; Data_Get_Struct(vec2, LNVector3, tmp__vec2);LNVector3& _vec2 = *tmp__vec2;
            float _t = ((float)NUM2DBL(t));
            LNVector3 _outVec;
            LNResult errorCode = LNVector3_Lerp(&_vec1, &_vec2, _t, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.lerp - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_CatmullRom(int argc, VALUE *argv, VALUE self)
{
    if (5 <= argc && argc <= 5) {
        VALUE vec1;
        VALUE vec2;
        VALUE vec3;
        VALUE vec4;
        VALUE t;
        rb_scan_args(argc, argv, "5", &vec1, &vec2, &vec3, &vec4, &t);
        if (isRbObject(vec1) && isRbObject(vec2) && isRbObject(vec3) && isRbObject(vec4) && isRbFloat(t)) {
            LNVector3* tmp__vec1; Data_Get_Struct(vec1, LNVector3, tmp__vec1);LNVector3& _vec1 = *tmp__vec1;
            LNVector3* tmp__vec2; Data_Get_Struct(vec2, LNVector3, tmp__vec2);LNVector3& _vec2 = *tmp__vec2;
            LNVector3* tmp__vec3; Data_Get_Struct(vec3, LNVector3, tmp__vec3);LNVector3& _vec3 = *tmp__vec3;
            LNVector3* tmp__vec4; Data_Get_Struct(vec4, LNVector3, tmp__vec4);LNVector3& _vec4 = *tmp__vec4;
            float _t = ((float)NUM2DBL(t));
            LNVector3 _outVec;
            LNResult errorCode = LNVector3_CatmullRom(&_vec1, &_vec2, &_vec3, &_vec4, _t, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.catmull_rom - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_Transform(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE mat;
        rb_scan_args(argc, argv, "2", &vec, &mat);
        if (isRbObject(vec) && isRbObject(mat)) {
            LNVector3* tmp__vec; Data_Get_Struct(vec, LNVector3, tmp__vec);LNVector3& _vec = *tmp__vec;
            LNMatrix* tmp__mat; Data_Get_Struct(mat, LNMatrix, tmp__mat);LNMatrix& _mat = *tmp__mat;
            LNVector4 _outVec;
            LNResult errorCode = LNVector3_Transform(&_vec, &_mat, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector4_allocate(g_struct_Vector4);
    *((LNVector4*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.transform - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNVector3_TransformCoord(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE mat;
        rb_scan_args(argc, argv, "2", &vec, &mat);
        if (isRbObject(vec) && isRbObject(mat)) {
            LNVector3* tmp__vec; Data_Get_Struct(vec, LNVector3, tmp__vec);LNVector3& _vec = *tmp__vec;
            LNMatrix* tmp__mat; Data_Get_Struct(mat, LNMatrix, tmp__mat);LNMatrix& _mat = *tmp__mat;
            LNVector3 _outVec;
            LNResult errorCode = LNVector3_TransformCoord(&_vec, &_mat, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector3.transform_coord - wrong argument type.");
    return Qnil;
}


void LNVector4_delete(LNVector4* obj)
{
    free(obj);
}

VALUE LNVector4_allocate( VALUE klass )
{
    VALUE obj;
    LNVector4* internalObj;

    internalObj = (LNVector4*)malloc(sizeof(LNVector4));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNVector4_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNVector4_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNVector4));

    return obj;
}

VALUE LNVector4_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    if (argc == 0) {    // 引数 0 個を許可する
        return self;
    }
    VALUE X_;
    VALUE Y_;
    VALUE Z_;
    VALUE W_;
    rb_scan_args(argc, argv, "4", &X_, &Y_, &Z_, &W_);
    selfObj->X = ((float)NUM2DBL(X_));
    selfObj->Y = ((float)NUM2DBL(Y_));
    selfObj->Z = ((float)NUM2DBL(Z_));
    selfObj->W = ((float)NUM2DBL(W_));
    return self;
}
static VALUE LNVector4_X_set(VALUE self, VALUE v)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    selfObj->X = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector4_X_get(VALUE self)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    return toVALUE(selfObj->X);
}

static VALUE LNVector4_Y_set(VALUE self, VALUE v)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    selfObj->Y = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector4_Y_get(VALUE self)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    return toVALUE(selfObj->Y);
}

static VALUE LNVector4_Z_set(VALUE self, VALUE v)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    selfObj->Z = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector4_Z_get(VALUE self)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    return toVALUE(selfObj->Z);
}

static VALUE LNVector4_W_set(VALUE self, VALUE v)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    selfObj->W = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNVector4_W_get(VALUE self)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    return toVALUE(selfObj->W);
}

static VALUE lnrbLNVector4_Set(int argc, VALUE *argv, VALUE self)
{
    LNVector4* selfObj;
    Data_Get_Struct(self, LNVector4, selfObj);
    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE z;
        VALUE w;
        rb_scan_args(argc, argv, "4", &x, &y, &z, &w);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z) && isRbFloat(w)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            float _w = ((float)NUM2DBL(w));
            LNResult errorCode = LNVector4_Set(selfObj, _x, _y, _z, _w);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Vector4.set - wrong argument type.");
    return Qnil;
}


void LNMatrix_delete(LNMatrix* obj)
{
    free(obj);
}

VALUE LNMatrix_allocate( VALUE klass )
{
    VALUE obj;
    LNMatrix* internalObj;

    internalObj = (LNMatrix*)malloc(sizeof(LNMatrix));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNMatrix_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNMatrix_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNMatrix));

    return obj;
}

VALUE LNMatrix_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (argc == 0) {    // 引数 0 個を許可する
        return self;
    }
    VALUE M11_;
    VALUE M12_;
    VALUE M13_;
    VALUE M14_;
    VALUE M21_;
    VALUE M22_;
    VALUE M23_;
    VALUE M24_;
    VALUE M31_;
    VALUE M32_;
    VALUE M33_;
    VALUE M34_;
    VALUE M41_;
    VALUE M42_;
    VALUE M43_;
    VALUE M44_;
    rb_scan_args(argc, argv, "16", &M11_, &M12_, &M13_, &M14_, &M21_, &M22_, &M23_, &M24_, &M31_, &M32_, &M33_, &M34_, &M41_, &M42_, &M43_, &M44_);
    selfObj->M11 = ((float)NUM2DBL(M11_));
    selfObj->M12 = ((float)NUM2DBL(M12_));
    selfObj->M13 = ((float)NUM2DBL(M13_));
    selfObj->M14 = ((float)NUM2DBL(M14_));
    selfObj->M21 = ((float)NUM2DBL(M21_));
    selfObj->M22 = ((float)NUM2DBL(M22_));
    selfObj->M23 = ((float)NUM2DBL(M23_));
    selfObj->M24 = ((float)NUM2DBL(M24_));
    selfObj->M31 = ((float)NUM2DBL(M31_));
    selfObj->M32 = ((float)NUM2DBL(M32_));
    selfObj->M33 = ((float)NUM2DBL(M33_));
    selfObj->M34 = ((float)NUM2DBL(M34_));
    selfObj->M41 = ((float)NUM2DBL(M41_));
    selfObj->M42 = ((float)NUM2DBL(M42_));
    selfObj->M43 = ((float)NUM2DBL(M43_));
    selfObj->M44 = ((float)NUM2DBL(M44_));
    return self;
}
static VALUE LNMatrix_M11_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M11 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M11_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M11);
}

static VALUE LNMatrix_M12_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M12 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M12_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M12);
}

static VALUE LNMatrix_M13_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M13 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M13_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M13);
}

static VALUE LNMatrix_M14_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M14 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M14_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M14);
}

static VALUE LNMatrix_M21_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M21 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M21_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M21);
}

static VALUE LNMatrix_M22_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M22 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M22_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M22);
}

static VALUE LNMatrix_M23_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M23 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M23_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M23);
}

static VALUE LNMatrix_M24_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M24 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M24_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M24);
}

static VALUE LNMatrix_M31_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M31 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M31_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M31);
}

static VALUE LNMatrix_M32_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M32 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M32_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M32);
}

static VALUE LNMatrix_M33_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M33 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M33_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M33);
}

static VALUE LNMatrix_M34_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M34 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M34_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M34);
}

static VALUE LNMatrix_M41_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M41 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M41_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M41);
}

static VALUE LNMatrix_M42_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M42 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M42_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M42);
}

static VALUE LNMatrix_M43_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M43 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M43_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M43);
}

static VALUE LNMatrix_M44_set(VALUE self, VALUE v)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    selfObj->M44 = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNMatrix_M44_get(VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    return toVALUE(selfObj->M44);
}

static VALUE lnrbLNMatrix_GetRight(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNVector3 _outVec;
            LNResult errorCode = LNMatrix_GetRight(selfObj, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.right - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetUp(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNVector3 _outVec;
            LNResult errorCode = LNMatrix_GetUp(selfObj, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.up - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetFront(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNVector3 _outVec;
            LNResult errorCode = LNMatrix_GetFront(selfObj, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.front - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetPosition(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNVector3 _outVec;
            LNResult errorCode = LNMatrix_GetPosition(selfObj, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.position - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Identity(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNResult errorCode = LNMatrix_Identity(selfObj);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.identity - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Translate(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNResult errorCode = LNMatrix_Translate(selfObj, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.translate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_TranslateVec3(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            LNVector3* tmp__vec; Data_Get_Struct(vec, LNVector3, tmp__vec);LNVector3& _vec = *tmp__vec;
            LNResult errorCode = LNMatrix_TranslateVec3(selfObj, &_vec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.translate_vec_3 - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateX(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE radian;
        rb_scan_args(argc, argv, "1", &radian);
        if (isRbFloat(radian)) {
            float _radian = ((float)NUM2DBL(radian));
            LNResult errorCode = LNMatrix_RotateX(selfObj, _radian);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.rotate_x - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateY(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE radian;
        rb_scan_args(argc, argv, "1", &radian);
        if (isRbFloat(radian)) {
            float _radian = ((float)NUM2DBL(radian));
            LNResult errorCode = LNMatrix_RotateY(selfObj, _radian);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.rotate_y - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateZ(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE radian;
        rb_scan_args(argc, argv, "1", &radian);
        if (isRbFloat(radian)) {
            float _radian = ((float)NUM2DBL(radian));
            LNResult errorCode = LNMatrix_RotateZ(selfObj, _radian);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.rotate_z - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Rotate(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (3 <= argc && argc <= 4) {
        VALUE xRad;
        VALUE yRad;
        VALUE zRad;
        VALUE rotOrder;
        rb_scan_args(argc, argv, "31", &xRad, &yRad, &zRad, &rotOrder);
        if (isRbFloat(xRad) && isRbFloat(yRad) && isRbFloat(zRad) && isRbNumber(rotOrder)) {
            float _xRad = ((float)NUM2DBL(xRad));
            float _yRad = ((float)NUM2DBL(yRad));
            float _zRad = ((float)NUM2DBL(zRad));
            LNRotationOrder _rotOrder = (rotOrder != Qnil) ? (LNRotationOrder)FIX2INT(rotOrder) : LN_ROTATIONORDER_XYZ;
            LNResult errorCode = LNMatrix_Rotate(selfObj, _xRad, _yRad, _zRad, _rotOrder);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.rotate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateVec3(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 2) {
        VALUE vec;
        VALUE rotOrder;
        rb_scan_args(argc, argv, "11", &vec, &rotOrder);
        if (isRbObject(vec) && isRbNumber(rotOrder)) {
            LNVector3* tmp__vec; Data_Get_Struct(vec, LNVector3, tmp__vec);LNVector3& _vec = *tmp__vec;
            LNRotationOrder _rotOrder = (rotOrder != Qnil) ? (LNRotationOrder)FIX2INT(rotOrder) : LN_ROTATIONORDER_XYZ;
            LNResult errorCode = LNMatrix_RotateVec3(selfObj, &_vec, _rotOrder);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.rotate_vec_3 - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateAxis(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (2 <= argc && argc <= 2) {
        VALUE axis;
        VALUE radian;
        rb_scan_args(argc, argv, "2", &axis, &radian);
        if (isRbObject(axis) && isRbFloat(radian)) {
            LNVector3* tmp__axis; Data_Get_Struct(axis, LNVector3, tmp__axis);LNVector3& _axis = *tmp__axis;
            float _radian = ((float)NUM2DBL(radian));
            LNResult errorCode = LNMatrix_RotateAxis(selfObj, &_axis, _radian);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.rotate_axis - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateQuaternion(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE qua;
        rb_scan_args(argc, argv, "1", &qua);
        if (isRbObject(qua)) {
            LNQuaternion* tmp__qua; Data_Get_Struct(qua, LNQuaternion, tmp__qua);LNQuaternion& _qua = *tmp__qua;
            LNResult errorCode = LNMatrix_RotateQuaternion(selfObj, &_qua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.rotate_quaternion - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Scale(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE xyz;
        rb_scan_args(argc, argv, "1", &xyz);
        if (isRbFloat(xyz)) {
            float _xyz = ((float)NUM2DBL(xyz));
            LNResult errorCode = LNMatrix_Scale(selfObj, _xyz);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.scale - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_ScaleXYZ(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            LNResult errorCode = LNMatrix_ScaleXYZ(selfObj, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.scale_xyz - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_ScaleVec3(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (1 <= argc && argc <= 1) {
        VALUE scale;
        rb_scan_args(argc, argv, "1", &scale);
        if (isRbObject(scale)) {
            LNVector3* tmp__scale; Data_Get_Struct(scale, LNVector3, tmp__scale);LNVector3& _scale = *tmp__scale;
            LNResult errorCode = LNMatrix_ScaleVec3(selfObj, &_scale);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.scale_vec_3 - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_Multiply(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE mat1;
        VALUE mat2;
        rb_scan_args(argc, argv, "2", &mat1, &mat2);
        if (isRbObject(mat1) && isRbObject(mat2)) {
            LNMatrix* tmp__mat1; Data_Get_Struct(mat1, LNMatrix, tmp__mat1);LNMatrix& _mat1 = *tmp__mat1;
            LNMatrix* tmp__mat2; Data_Get_Struct(mat2, LNMatrix, tmp__mat2);LNMatrix& _mat2 = *tmp__mat2;
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_Multiply(&_mat1, &_mat2, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.multiply - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_Inverse(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            LNMatrix* tmp__mat; Data_Get_Struct(mat, LNMatrix, tmp__mat);LNMatrix& _mat = *tmp__mat;
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_Inverse(&_mat, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.inverse - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_Transpose(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            LNMatrix* tmp__mat; Data_Get_Struct(mat, LNMatrix, tmp__mat);LNMatrix& _mat = *tmp__mat;
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_Transpose(&_mat, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.transpose - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_ViewTransformLH(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE pos;
        VALUE lookAt;
        VALUE upDir;
        rb_scan_args(argc, argv, "3", &pos, &lookAt, &upDir);
        if (isRbObject(pos) && isRbObject(lookAt) && isRbObject(upDir)) {
            LNVector3* tmp__pos; Data_Get_Struct(pos, LNVector3, tmp__pos);LNVector3& _pos = *tmp__pos;
            LNVector3* tmp__lookAt; Data_Get_Struct(lookAt, LNVector3, tmp__lookAt);LNVector3& _lookAt = *tmp__lookAt;
            LNVector3* tmp__upDir; Data_Get_Struct(upDir, LNVector3, tmp__upDir);LNVector3& _upDir = *tmp__upDir;
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_ViewTransformLH(&_pos, &_lookAt, &_upDir, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.view_transform_lh - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_ViewTransformRH(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE pos;
        VALUE lookAt;
        VALUE upDir;
        rb_scan_args(argc, argv, "3", &pos, &lookAt, &upDir);
        if (isRbObject(pos) && isRbObject(lookAt) && isRbObject(upDir)) {
            LNVector3* tmp__pos; Data_Get_Struct(pos, LNVector3, tmp__pos);LNVector3& _pos = *tmp__pos;
            LNVector3* tmp__lookAt; Data_Get_Struct(lookAt, LNVector3, tmp__lookAt);LNVector3& _lookAt = *tmp__lookAt;
            LNVector3* tmp__upDir; Data_Get_Struct(upDir, LNVector3, tmp__upDir);LNVector3& _upDir = *tmp__upDir;
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_ViewTransformRH(&_pos, &_lookAt, &_upDir, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.view_transform_rh - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_PerspectiveFovLH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE fovY;
        VALUE aspect;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &fovY, &aspect, &nearZ, &farZ);
        if (isRbFloat(fovY) && isRbFloat(aspect) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _fovY = ((float)NUM2DBL(fovY));
            float _aspect = ((float)NUM2DBL(aspect));
            float _nearZ = ((float)NUM2DBL(nearZ));
            float _farZ = ((float)NUM2DBL(farZ));
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_PerspectiveFovLH(_fovY, _aspect, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.perspective_fov_lh - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_PerspectiveFovRH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE fovY;
        VALUE aspect;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &fovY, &aspect, &nearZ, &farZ);
        if (isRbFloat(fovY) && isRbFloat(aspect) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _fovY = ((float)NUM2DBL(fovY));
            float _aspect = ((float)NUM2DBL(aspect));
            float _nearZ = ((float)NUM2DBL(nearZ));
            float _farZ = ((float)NUM2DBL(farZ));
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_PerspectiveFovRH(_fovY, _aspect, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.perspective_fov_rh - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_OrthoLH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE width;
        VALUE height;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &width, &height, &nearZ, &farZ);
        if (isRbFloat(width) && isRbFloat(height) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _width = ((float)NUM2DBL(width));
            float _height = ((float)NUM2DBL(height));
            float _nearZ = ((float)NUM2DBL(nearZ));
            float _farZ = ((float)NUM2DBL(farZ));
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_OrthoLH(_width, _height, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.ortho_lh - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNMatrix_OrthoRH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE width;
        VALUE height;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &width, &height, &nearZ, &farZ);
        if (isRbFloat(width) && isRbFloat(height) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _width = ((float)NUM2DBL(width));
            float _height = ((float)NUM2DBL(height));
            float _nearZ = ((float)NUM2DBL(nearZ));
            float _farZ = ((float)NUM2DBL(farZ));
            LNMatrix _matOut;
            LNResult errorCode = LNMatrix_OrthoRH(_width, _height, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNMatrix_allocate(g_struct_Matrix);
    *((LNMatrix*)DATA_PTR(retObj)) = _matOut;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.ortho_rh - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetEulerAngles(int argc, VALUE *argv, VALUE self)
{
    LNMatrix* selfObj;
    Data_Get_Struct(self, LNMatrix, selfObj);
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNVector3 _outVec;
            LNResult errorCode = LNMatrix_GetEulerAngles(selfObj, &_outVec);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNVector3_allocate(g_struct_Vector3);
    *((LNVector3*)DATA_PTR(retObj)) = _outVec;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Matrix.get_euler_angles - wrong argument type.");
    return Qnil;
}


void LNQuaternion_delete(LNQuaternion* obj)
{
    free(obj);
}

VALUE LNQuaternion_allocate( VALUE klass )
{
    VALUE obj;
    LNQuaternion* internalObj;

    internalObj = (LNQuaternion*)malloc(sizeof(LNQuaternion));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNQuaternion_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNQuaternion_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNQuaternion));

    return obj;
}

VALUE LNQuaternion_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    if (argc == 0) {    // 引数 0 個を許可する
        return self;
    }
    VALUE X_;
    VALUE Y_;
    VALUE Z_;
    VALUE W_;
    rb_scan_args(argc, argv, "4", &X_, &Y_, &Z_, &W_);
    selfObj->X = ((float)NUM2DBL(X_));
    selfObj->Y = ((float)NUM2DBL(Y_));
    selfObj->Z = ((float)NUM2DBL(Z_));
    selfObj->W = ((float)NUM2DBL(W_));
    return self;
}
static VALUE LNQuaternion_X_set(VALUE self, VALUE v)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    selfObj->X = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNQuaternion_X_get(VALUE self)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    return toVALUE(selfObj->X);
}

static VALUE LNQuaternion_Y_set(VALUE self, VALUE v)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    selfObj->Y = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNQuaternion_Y_get(VALUE self)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    return toVALUE(selfObj->Y);
}

static VALUE LNQuaternion_Z_set(VALUE self, VALUE v)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    selfObj->Z = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNQuaternion_Z_get(VALUE self)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    return toVALUE(selfObj->Z);
}

static VALUE LNQuaternion_W_set(VALUE self, VALUE v)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    selfObj->W = ((float)NUM2DBL(v));
    return self;
}

static VALUE LNQuaternion_W_get(VALUE self)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    return toVALUE(selfObj->W);
}

static VALUE lnrbLNQuaternion_Set(int argc, VALUE *argv, VALUE self)
{
    LNQuaternion* selfObj;
    Data_Get_Struct(self, LNQuaternion, selfObj);
    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE z;
        VALUE w;
        rb_scan_args(argc, argv, "4", &x, &y, &z, &w);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z) && isRbFloat(w)) {
            float _x = ((float)NUM2DBL(x));
            float _y = ((float)NUM2DBL(y));
            float _z = ((float)NUM2DBL(z));
            float _w = ((float)NUM2DBL(w));
            LNResult errorCode = LNQuaternion_Set(selfObj, _x, _y, _z, _w);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.set - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_Identity(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {
    
        if (true) {
            LNQuaternion _qua;
            LNResult errorCode = LNQuaternion_Identity(&_qua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _qua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.identity - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_RotationAxis(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE axis;
        VALUE r;
        rb_scan_args(argc, argv, "2", &axis, &r);
        if (isRbObject(axis) && isRbFloat(r)) {
            LNVector3* tmp__axis; Data_Get_Struct(axis, LNVector3, tmp__axis);LNVector3& _axis = *tmp__axis;
            float _r = ((float)NUM2DBL(r));
            LNQuaternion _outQua;
            LNResult errorCode = LNQuaternion_RotationAxis(&_axis, _r, &_outQua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _outQua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.rotation_axis - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_RotationMatrix(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            LNMatrix* tmp__mat; Data_Get_Struct(mat, LNMatrix, tmp__mat);LNMatrix& _mat = *tmp__mat;
            LNQuaternion _outQua;
            LNResult errorCode = LNQuaternion_RotationMatrix(&_mat, &_outQua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _outQua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.rotation_matrix - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_RotationYawPitchRoll(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE yaw;
        VALUE pitch;
        VALUE roll;
        rb_scan_args(argc, argv, "3", &yaw, &pitch, &roll);
        if (isRbFloat(yaw) && isRbFloat(pitch) && isRbFloat(roll)) {
            float _yaw = ((float)NUM2DBL(yaw));
            float _pitch = ((float)NUM2DBL(pitch));
            float _roll = ((float)NUM2DBL(roll));
            LNQuaternion _outQua;
            LNResult errorCode = LNQuaternion_RotationYawPitchRoll(_yaw, _pitch, _roll, &_outQua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _outQua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.rotation_yaw_pitch_roll - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_Normalize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE qua;
        rb_scan_args(argc, argv, "1", &qua);
        if (isRbObject(qua)) {
            LNQuaternion* tmp__qua; Data_Get_Struct(qua, LNQuaternion, tmp__qua);LNQuaternion& _qua = *tmp__qua;
            LNQuaternion _outQua;
            LNResult errorCode = LNQuaternion_Normalize(&_qua, &_outQua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _outQua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.normalize - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_Conjugate(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE qua;
        rb_scan_args(argc, argv, "1", &qua);
        if (isRbObject(qua)) {
            LNQuaternion* tmp__qua; Data_Get_Struct(qua, LNQuaternion, tmp__qua);LNQuaternion& _qua = *tmp__qua;
            LNQuaternion _outQua;
            LNResult errorCode = LNQuaternion_Conjugate(&_qua, &_outQua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _outQua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.conjugate - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_Multiply(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE qua1;
        VALUE qua2;
        rb_scan_args(argc, argv, "2", &qua1, &qua2);
        if (isRbObject(qua1) && isRbObject(qua2)) {
            LNQuaternion* tmp__qua1; Data_Get_Struct(qua1, LNQuaternion, tmp__qua1);LNQuaternion& _qua1 = *tmp__qua1;
            LNQuaternion* tmp__qua2; Data_Get_Struct(qua2, LNQuaternion, tmp__qua2);LNQuaternion& _qua2 = *tmp__qua2;
            LNQuaternion _outQua;
            LNResult errorCode = LNQuaternion_Multiply(&_qua1, &_qua2, &_outQua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _outQua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.multiply - wrong argument type.");
    return Qnil;
}

static VALUE static_lnrbLNQuaternion_Slerp(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE qua1;
        VALUE qua2;
        VALUE t;
        rb_scan_args(argc, argv, "3", &qua1, &qua2, &t);
        if (isRbObject(qua1) && isRbObject(qua2) && isRbFloat(t)) {
            LNQuaternion* tmp__qua1; Data_Get_Struct(qua1, LNQuaternion, tmp__qua1);LNQuaternion& _qua1 = *tmp__qua1;
            LNQuaternion* tmp__qua2; Data_Get_Struct(qua2, LNQuaternion, tmp__qua2);LNQuaternion& _qua2 = *tmp__qua2;
            float _t = ((float)NUM2DBL(t));
            LNQuaternion _outQua;
            LNResult errorCode = LNQuaternion_Slerp(&_qua1, &_qua2, _t, &_outQua);
            if (errorCode != LN_OK) rb_raise(g_luminoError, "Lumino error. (%d)\n%s", errorCode, LNInternal_ConvertToUTF8String(LNError_GetLastErrorMessage(), -1));
            VALUE retObj = LNQuaternion_allocate(g_struct_Quaternion);
    *((LNQuaternion*)DATA_PTR(retObj)) = _outQua;
    return retObj;
    
        }
    }
    rb_raise(rb_eArgError, "Lumino::Quaternion.slerp - wrong argument type.");
    return Qnil;
}



void InitStructs()
{
    g_struct_Vector2 = rb_define_class_under(g_luminoModule, "Vector2", rb_cObject);
    rb_define_alloc_func(g_struct_Vector2, LNVector2_allocate);
    rb_define_private_method(g_struct_Vector2, "initialize", LN_TO_RUBY_FUNC(LNVector2_struct_initialize), -1);
    rb_define_method(g_struct_Vector2, "x=", LN_TO_RUBY_FUNC(LNVector2_X_set), 1);
    rb_define_method(g_struct_Vector2, "x", LN_TO_RUBY_FUNC(LNVector2_X_get), 0);
    rb_define_method(g_struct_Vector2, "y=", LN_TO_RUBY_FUNC(LNVector2_Y_set), 1);
    rb_define_method(g_struct_Vector2, "y", LN_TO_RUBY_FUNC(LNVector2_Y_get), 0);
    rb_define_method(g_struct_Vector2, "length", LN_TO_RUBY_FUNC(lnrbLNVector2_GetLength), -1);
    rb_define_method(g_struct_Vector2, "square_length", LN_TO_RUBY_FUNC(lnrbLNVector2_GetSquareLength), -1);
    rb_define_method(g_struct_Vector2, "set", LN_TO_RUBY_FUNC(lnrbLNVector2_Set), -1);
    rb_define_singleton_method(g_struct_Vector2, "normalize", LN_TO_RUBY_FUNC(static_lnrbLNVector2_Normalize), -1);
    rb_define_method(g_struct_Vector2, "normalize", LN_TO_RUBY_FUNC(lnrbLNVector2_NormalizeV), -1);

    g_struct_Vector3 = rb_define_class_under(g_luminoModule, "Vector3", rb_cObject);
    rb_define_alloc_func(g_struct_Vector3, LNVector3_allocate);
    rb_define_private_method(g_struct_Vector3, "initialize", LN_TO_RUBY_FUNC(LNVector3_struct_initialize), -1);
    rb_define_method(g_struct_Vector3, "x=", LN_TO_RUBY_FUNC(LNVector3_X_set), 1);
    rb_define_method(g_struct_Vector3, "x", LN_TO_RUBY_FUNC(LNVector3_X_get), 0);
    rb_define_method(g_struct_Vector3, "y=", LN_TO_RUBY_FUNC(LNVector3_Y_set), 1);
    rb_define_method(g_struct_Vector3, "y", LN_TO_RUBY_FUNC(LNVector3_Y_get), 0);
    rb_define_method(g_struct_Vector3, "z=", LN_TO_RUBY_FUNC(LNVector3_Z_set), 1);
    rb_define_method(g_struct_Vector3, "z", LN_TO_RUBY_FUNC(LNVector3_Z_get), 0);
    rb_define_method(g_struct_Vector3, "length", LN_TO_RUBY_FUNC(lnrbLNVector3_GetLength), -1);
    rb_define_method(g_struct_Vector3, "square_length", LN_TO_RUBY_FUNC(lnrbLNVector3_GetSquareLength), -1);
    rb_define_method(g_struct_Vector3, "set", LN_TO_RUBY_FUNC(lnrbLNVector3_Set), -1);
    rb_define_method(g_struct_Vector3, "set_vz", LN_TO_RUBY_FUNC(lnrbLNVector3_SetVZ), -1);
    rb_define_singleton_method(g_struct_Vector3, "normalize", LN_TO_RUBY_FUNC(static_lnrbLNVector3_Normalize), -1);
    rb_define_method(g_struct_Vector3, "normalize", LN_TO_RUBY_FUNC(lnrbLNVector3_NormalizeV), -1);
    rb_define_singleton_method(g_struct_Vector3, "dot", LN_TO_RUBY_FUNC(static_lnrbLNVector3_Dot), -1);
    rb_define_singleton_method(g_struct_Vector3, "cross", LN_TO_RUBY_FUNC(static_lnrbLNVector3_Cross), -1);
    rb_define_singleton_method(g_struct_Vector3, "reflect", LN_TO_RUBY_FUNC(static_lnrbLNVector3_Reflect), -1);
    rb_define_singleton_method(g_struct_Vector3, "slide", LN_TO_RUBY_FUNC(static_lnrbLNVector3_Slide), -1);
    rb_define_singleton_method(g_struct_Vector3, "lerp", LN_TO_RUBY_FUNC(static_lnrbLNVector3_Lerp), -1);
    rb_define_singleton_method(g_struct_Vector3, "catmull_rom", LN_TO_RUBY_FUNC(static_lnrbLNVector3_CatmullRom), -1);
    rb_define_singleton_method(g_struct_Vector3, "transform", LN_TO_RUBY_FUNC(static_lnrbLNVector3_Transform), -1);
    rb_define_singleton_method(g_struct_Vector3, "transform_coord", LN_TO_RUBY_FUNC(static_lnrbLNVector3_TransformCoord), -1);

    g_struct_Vector4 = rb_define_class_under(g_luminoModule, "Vector4", rb_cObject);
    rb_define_alloc_func(g_struct_Vector4, LNVector4_allocate);
    rb_define_private_method(g_struct_Vector4, "initialize", LN_TO_RUBY_FUNC(LNVector4_struct_initialize), -1);
    rb_define_method(g_struct_Vector4, "x=", LN_TO_RUBY_FUNC(LNVector4_X_set), 1);
    rb_define_method(g_struct_Vector4, "x", LN_TO_RUBY_FUNC(LNVector4_X_get), 0);
    rb_define_method(g_struct_Vector4, "y=", LN_TO_RUBY_FUNC(LNVector4_Y_set), 1);
    rb_define_method(g_struct_Vector4, "y", LN_TO_RUBY_FUNC(LNVector4_Y_get), 0);
    rb_define_method(g_struct_Vector4, "z=", LN_TO_RUBY_FUNC(LNVector4_Z_set), 1);
    rb_define_method(g_struct_Vector4, "z", LN_TO_RUBY_FUNC(LNVector4_Z_get), 0);
    rb_define_method(g_struct_Vector4, "w=", LN_TO_RUBY_FUNC(LNVector4_W_set), 1);
    rb_define_method(g_struct_Vector4, "w", LN_TO_RUBY_FUNC(LNVector4_W_get), 0);
    rb_define_method(g_struct_Vector4, "set", LN_TO_RUBY_FUNC(lnrbLNVector4_Set), -1);

    g_struct_Matrix = rb_define_class_under(g_luminoModule, "Matrix", rb_cObject);
    rb_define_alloc_func(g_struct_Matrix, LNMatrix_allocate);
    rb_define_private_method(g_struct_Matrix, "initialize", LN_TO_RUBY_FUNC(LNMatrix_struct_initialize), -1);
    rb_define_method(g_struct_Matrix, "m11=", LN_TO_RUBY_FUNC(LNMatrix_M11_set), 1);
    rb_define_method(g_struct_Matrix, "m11", LN_TO_RUBY_FUNC(LNMatrix_M11_get), 0);
    rb_define_method(g_struct_Matrix, "m12=", LN_TO_RUBY_FUNC(LNMatrix_M12_set), 1);
    rb_define_method(g_struct_Matrix, "m12", LN_TO_RUBY_FUNC(LNMatrix_M12_get), 0);
    rb_define_method(g_struct_Matrix, "m13=", LN_TO_RUBY_FUNC(LNMatrix_M13_set), 1);
    rb_define_method(g_struct_Matrix, "m13", LN_TO_RUBY_FUNC(LNMatrix_M13_get), 0);
    rb_define_method(g_struct_Matrix, "m14=", LN_TO_RUBY_FUNC(LNMatrix_M14_set), 1);
    rb_define_method(g_struct_Matrix, "m14", LN_TO_RUBY_FUNC(LNMatrix_M14_get), 0);
    rb_define_method(g_struct_Matrix, "m21=", LN_TO_RUBY_FUNC(LNMatrix_M21_set), 1);
    rb_define_method(g_struct_Matrix, "m21", LN_TO_RUBY_FUNC(LNMatrix_M21_get), 0);
    rb_define_method(g_struct_Matrix, "m22=", LN_TO_RUBY_FUNC(LNMatrix_M22_set), 1);
    rb_define_method(g_struct_Matrix, "m22", LN_TO_RUBY_FUNC(LNMatrix_M22_get), 0);
    rb_define_method(g_struct_Matrix, "m23=", LN_TO_RUBY_FUNC(LNMatrix_M23_set), 1);
    rb_define_method(g_struct_Matrix, "m23", LN_TO_RUBY_FUNC(LNMatrix_M23_get), 0);
    rb_define_method(g_struct_Matrix, "m24=", LN_TO_RUBY_FUNC(LNMatrix_M24_set), 1);
    rb_define_method(g_struct_Matrix, "m24", LN_TO_RUBY_FUNC(LNMatrix_M24_get), 0);
    rb_define_method(g_struct_Matrix, "m31=", LN_TO_RUBY_FUNC(LNMatrix_M31_set), 1);
    rb_define_method(g_struct_Matrix, "m31", LN_TO_RUBY_FUNC(LNMatrix_M31_get), 0);
    rb_define_method(g_struct_Matrix, "m32=", LN_TO_RUBY_FUNC(LNMatrix_M32_set), 1);
    rb_define_method(g_struct_Matrix, "m32", LN_TO_RUBY_FUNC(LNMatrix_M32_get), 0);
    rb_define_method(g_struct_Matrix, "m33=", LN_TO_RUBY_FUNC(LNMatrix_M33_set), 1);
    rb_define_method(g_struct_Matrix, "m33", LN_TO_RUBY_FUNC(LNMatrix_M33_get), 0);
    rb_define_method(g_struct_Matrix, "m34=", LN_TO_RUBY_FUNC(LNMatrix_M34_set), 1);
    rb_define_method(g_struct_Matrix, "m34", LN_TO_RUBY_FUNC(LNMatrix_M34_get), 0);
    rb_define_method(g_struct_Matrix, "m41=", LN_TO_RUBY_FUNC(LNMatrix_M41_set), 1);
    rb_define_method(g_struct_Matrix, "m41", LN_TO_RUBY_FUNC(LNMatrix_M41_get), 0);
    rb_define_method(g_struct_Matrix, "m42=", LN_TO_RUBY_FUNC(LNMatrix_M42_set), 1);
    rb_define_method(g_struct_Matrix, "m42", LN_TO_RUBY_FUNC(LNMatrix_M42_get), 0);
    rb_define_method(g_struct_Matrix, "m43=", LN_TO_RUBY_FUNC(LNMatrix_M43_set), 1);
    rb_define_method(g_struct_Matrix, "m43", LN_TO_RUBY_FUNC(LNMatrix_M43_get), 0);
    rb_define_method(g_struct_Matrix, "m44=", LN_TO_RUBY_FUNC(LNMatrix_M44_set), 1);
    rb_define_method(g_struct_Matrix, "m44", LN_TO_RUBY_FUNC(LNMatrix_M44_get), 0);
    rb_define_method(g_struct_Matrix, "right", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetRight), -1);
    rb_define_method(g_struct_Matrix, "up", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetUp), -1);
    rb_define_method(g_struct_Matrix, "front", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetFront), -1);
    rb_define_method(g_struct_Matrix, "position", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetPosition), -1);
    rb_define_method(g_struct_Matrix, "identity", LN_TO_RUBY_FUNC(lnrbLNMatrix_Identity), -1);
    rb_define_method(g_struct_Matrix, "translate", LN_TO_RUBY_FUNC(lnrbLNMatrix_Translate), -1);
    rb_define_method(g_struct_Matrix, "translate_vec_3", LN_TO_RUBY_FUNC(lnrbLNMatrix_TranslateVec3), -1);
    rb_define_method(g_struct_Matrix, "rotate_x", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateX), -1);
    rb_define_method(g_struct_Matrix, "rotate_y", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateY), -1);
    rb_define_method(g_struct_Matrix, "rotate_z", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateZ), -1);
    rb_define_method(g_struct_Matrix, "rotate", LN_TO_RUBY_FUNC(lnrbLNMatrix_Rotate), -1);
    rb_define_method(g_struct_Matrix, "rotate_vec_3", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateVec3), -1);
    rb_define_method(g_struct_Matrix, "rotate_axis", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateAxis), -1);
    rb_define_method(g_struct_Matrix, "rotate_quaternion", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateQuaternion), -1);
    rb_define_method(g_struct_Matrix, "scale", LN_TO_RUBY_FUNC(lnrbLNMatrix_Scale), -1);
    rb_define_method(g_struct_Matrix, "scale_xyz", LN_TO_RUBY_FUNC(lnrbLNMatrix_ScaleXYZ), -1);
    rb_define_method(g_struct_Matrix, "scale_vec_3", LN_TO_RUBY_FUNC(lnrbLNMatrix_ScaleVec3), -1);
    rb_define_singleton_method(g_struct_Matrix, "multiply", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_Multiply), -1);
    rb_define_singleton_method(g_struct_Matrix, "inverse", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_Inverse), -1);
    rb_define_singleton_method(g_struct_Matrix, "transpose", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_Transpose), -1);
    rb_define_singleton_method(g_struct_Matrix, "view_transform_lh", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_ViewTransformLH), -1);
    rb_define_singleton_method(g_struct_Matrix, "view_transform_rh", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_ViewTransformRH), -1);
    rb_define_singleton_method(g_struct_Matrix, "perspective_fov_lh", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_PerspectiveFovLH), -1);
    rb_define_singleton_method(g_struct_Matrix, "perspective_fov_rh", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_PerspectiveFovRH), -1);
    rb_define_singleton_method(g_struct_Matrix, "ortho_lh", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_OrthoLH), -1);
    rb_define_singleton_method(g_struct_Matrix, "ortho_rh", LN_TO_RUBY_FUNC(static_lnrbLNMatrix_OrthoRH), -1);
    rb_define_method(g_struct_Matrix, "get_euler_angles", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetEulerAngles), -1);

    g_struct_Quaternion = rb_define_class_under(g_luminoModule, "Quaternion", rb_cObject);
    rb_define_alloc_func(g_struct_Quaternion, LNQuaternion_allocate);
    rb_define_private_method(g_struct_Quaternion, "initialize", LN_TO_RUBY_FUNC(LNQuaternion_struct_initialize), -1);
    rb_define_method(g_struct_Quaternion, "x=", LN_TO_RUBY_FUNC(LNQuaternion_X_set), 1);
    rb_define_method(g_struct_Quaternion, "x", LN_TO_RUBY_FUNC(LNQuaternion_X_get), 0);
    rb_define_method(g_struct_Quaternion, "y=", LN_TO_RUBY_FUNC(LNQuaternion_Y_set), 1);
    rb_define_method(g_struct_Quaternion, "y", LN_TO_RUBY_FUNC(LNQuaternion_Y_get), 0);
    rb_define_method(g_struct_Quaternion, "z=", LN_TO_RUBY_FUNC(LNQuaternion_Z_set), 1);
    rb_define_method(g_struct_Quaternion, "z", LN_TO_RUBY_FUNC(LNQuaternion_Z_get), 0);
    rb_define_method(g_struct_Quaternion, "w=", LN_TO_RUBY_FUNC(LNQuaternion_W_set), 1);
    rb_define_method(g_struct_Quaternion, "w", LN_TO_RUBY_FUNC(LNQuaternion_W_get), 0);
    rb_define_method(g_struct_Quaternion, "set", LN_TO_RUBY_FUNC(lnrbLNQuaternion_Set), -1);
    rb_define_singleton_method(g_struct_Quaternion, "identity", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_Identity), -1);
    rb_define_singleton_method(g_struct_Quaternion, "rotation_axis", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_RotationAxis), -1);
    rb_define_singleton_method(g_struct_Quaternion, "rotation_matrix", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_RotationMatrix), -1);
    rb_define_singleton_method(g_struct_Quaternion, "rotation_yaw_pitch_roll", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_RotationYawPitchRoll), -1);
    rb_define_singleton_method(g_struct_Quaternion, "normalize", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_Normalize), -1);
    rb_define_singleton_method(g_struct_Quaternion, "conjugate", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_Conjugate), -1);
    rb_define_singleton_method(g_struct_Quaternion, "multiply", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_Multiply), -1);
    rb_define_singleton_method(g_struct_Quaternion, "slerp", LN_TO_RUBY_FUNC(static_lnrbLNQuaternion_Slerp), -1);


}

