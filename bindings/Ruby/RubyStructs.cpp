#include "LuminoRuby.h"

VALUE g_struct_Vector2;
VALUE g_struct_Vector3;
VALUE g_struct_Vector4;
VALUE g_struct_Matrix;
VALUE g_struct_Quaternion;



static void LNVector2_delete(LNVector2* obj)
{
    free(obj);
}

static VALUE LNVector2_allocate( VALUE klass )
{
    VALUE obj;
    LNVector2* internalObj;

    internalObj = (LNVector2*)malloc(sizeof(LNVector2));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNVector2_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNVector2_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNVector2));

    return obj;
}

static VALUE LNVector2_struct_initialize( int argc, VALUE *argv, VALUE self )
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


static void LNVector3_delete(LNVector3* obj)
{
    free(obj);
}

static VALUE LNVector3_allocate( VALUE klass )
{
    VALUE obj;
    LNVector3* internalObj;

    internalObj = (LNVector3*)malloc(sizeof(LNVector3));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNVector3_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNVector3_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNVector3));

    return obj;
}

static VALUE LNVector3_struct_initialize( int argc, VALUE *argv, VALUE self )
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


static void LNVector4_delete(LNVector4* obj)
{
    free(obj);
}

static VALUE LNVector4_allocate( VALUE klass )
{
    VALUE obj;
    LNVector4* internalObj;

    internalObj = (LNVector4*)malloc(sizeof(LNVector4));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNVector4_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNVector4_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNVector4));

    return obj;
}

static VALUE LNVector4_struct_initialize( int argc, VALUE *argv, VALUE self )
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


static void LNMatrix_delete(LNMatrix* obj)
{
    free(obj);
}

static VALUE LNMatrix_allocate( VALUE klass )
{
    VALUE obj;
    LNMatrix* internalObj;

    internalObj = (LNMatrix*)malloc(sizeof(LNMatrix));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNMatrix_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNMatrix_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNMatrix));

    return obj;
}

static VALUE LNMatrix_struct_initialize( int argc, VALUE *argv, VALUE self )
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


static void LNQuaternion_delete(LNQuaternion* obj)
{
    free(obj);
}

static VALUE LNQuaternion_allocate( VALUE klass )
{
    VALUE obj;
    LNQuaternion* internalObj;

    internalObj = (LNQuaternion*)malloc(sizeof(LNQuaternion));
    if (internalObj == NULL) rb_raise( g_luminoModule, "Faild alloc - LNQuaternion_allocate" );
    obj = Data_Wrap_Struct(klass, NULL, LNQuaternion_delete, internalObj);
    
    memset(internalObj, 0, sizeof(LNQuaternion));

    return obj;
}

static VALUE LNQuaternion_struct_initialize( int argc, VALUE *argv, VALUE self )
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



void InitStructs()
{
    g_struct_Vector2 = rb_define_class_under(g_luminoModule, "Vector2", rb_cObject);
    rb_define_alloc_func(g_struct_Vector2, LNVector2_allocate);
    rb_define_private_method(g_struct_Vector2, "initialize", LN_TO_RUBY_FUNC(LNVector2_struct_initialize), -1);
    rb_define_method(g_struct_Vector2, "x=", LN_TO_RUBY_FUNC(LNVector2_X_set), 1);
    rb_define_method(g_struct_Vector2, "x", LN_TO_RUBY_FUNC(LNVector2_X_get), 0);
    rb_define_method(g_struct_Vector2, "y=", LN_TO_RUBY_FUNC(LNVector2_Y_set), 1);
    rb_define_method(g_struct_Vector2, "y", LN_TO_RUBY_FUNC(LNVector2_Y_get), 0);

    g_struct_Vector3 = rb_define_class_under(g_luminoModule, "Vector3", rb_cObject);
    rb_define_alloc_func(g_struct_Vector3, LNVector3_allocate);
    rb_define_private_method(g_struct_Vector3, "initialize", LN_TO_RUBY_FUNC(LNVector3_struct_initialize), -1);
    rb_define_method(g_struct_Vector3, "x=", LN_TO_RUBY_FUNC(LNVector3_X_set), 1);
    rb_define_method(g_struct_Vector3, "x", LN_TO_RUBY_FUNC(LNVector3_X_get), 0);
    rb_define_method(g_struct_Vector3, "y=", LN_TO_RUBY_FUNC(LNVector3_Y_set), 1);
    rb_define_method(g_struct_Vector3, "y", LN_TO_RUBY_FUNC(LNVector3_Y_get), 0);
    rb_define_method(g_struct_Vector3, "z=", LN_TO_RUBY_FUNC(LNVector3_Z_set), 1);
    rb_define_method(g_struct_Vector3, "z", LN_TO_RUBY_FUNC(LNVector3_Z_get), 0);

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


}

