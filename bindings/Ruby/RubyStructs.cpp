
#include "ruby.h"

VALUE g_struct_Vector2;
VALUE g_struct_Vector3;
VALUE g_struct_Vector4;
VALUE g_struct_Matrix;
VALUE g_struct_Quaternion;


void InitStructs()
{
    g_struct_Vector2 = rb_define_class_under(g_luminoModule, "Vector2", rb_cObject);
    rb_define_alloc_func(g_struct_Vector2, LNVector2_allocate);
    rb_define_private_method(g_struct_Vector2, "initialize", LN_TO_RUBY_FUNC(LNVector2_struct_initialize), -1);

    g_struct_Vector3 = rb_define_class_under(g_luminoModule, "Vector3", rb_cObject);
    rb_define_alloc_func(g_struct_Vector3, LNVector3_allocate);
    rb_define_private_method(g_struct_Vector3, "initialize", LN_TO_RUBY_FUNC(LNVector3_struct_initialize), -1);

    g_struct_Vector4 = rb_define_class_under(g_luminoModule, "Vector4", rb_cObject);
    rb_define_alloc_func(g_struct_Vector4, LNVector4_allocate);
    rb_define_private_method(g_struct_Vector4, "initialize", LN_TO_RUBY_FUNC(LNVector4_struct_initialize), -1);

    g_struct_Matrix = rb_define_class_under(g_luminoModule, "Matrix", rb_cObject);
    rb_define_alloc_func(g_struct_Matrix, LNMatrix_allocate);
    rb_define_private_method(g_struct_Matrix, "initialize", LN_TO_RUBY_FUNC(LNMatrix_struct_initialize), -1);

    g_struct_Quaternion = rb_define_class_under(g_luminoModule, "Quaternion", rb_cObject);
    rb_define_alloc_func(g_struct_Quaternion, LNQuaternion_allocate);
    rb_define_private_method(g_struct_Quaternion, "initialize", LN_TO_RUBY_FUNC(LNQuaternion_struct_initialize), -1);


}

