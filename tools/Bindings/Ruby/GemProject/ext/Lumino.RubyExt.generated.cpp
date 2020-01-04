
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

extern "C" LN_FLAT_API void LnObject_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnSerializer_WriteBool(LnHandle serializer, const LnChar* name, LnBool value);
extern "C" LN_FLAT_API LnResult LnSerializer_WriteInt(LnHandle serializer, const LnChar* name, int value);
extern "C" LN_FLAT_API LnResult LnSerializer_WriteFloat(LnHandle serializer, const LnChar* name, float value);
extern "C" LN_FLAT_API LnResult LnSerializer_WriteString(LnHandle serializer, const LnChar* name, const LnChar* value);
extern "C" LN_FLAT_API LnResult LnSerializer_WriteObject(LnHandle serializer, const LnChar* name, LnHandle value);
extern "C" LN_FLAT_API LnResult LnSerializer_ReadBool(LnHandle serializer, const LnChar* name, LnBool* outReturn);
extern "C" LN_FLAT_API LnResult LnSerializer_ReadInt(LnHandle serializer, const LnChar* name, int* outReturn);
extern "C" LN_FLAT_API LnResult LnSerializer_ReadFloat(LnHandle serializer, const LnChar* name, float* outReturn);
extern "C" LN_FLAT_API LnResult LnSerializer_ReadString(LnHandle serializer, const LnChar* name, const LnChar** outReturn);
extern "C" LN_FLAT_API LnResult LnSerializer_ReadObject(LnHandle serializer, const LnChar* name, LnHandle* outReturn);
extern "C" LN_FLAT_API LnResult LnSerializer_Serialize(LnHandle value, const LnChar* basePath, const LnChar** outReturn);
extern "C" LN_FLAT_API LnResult LnSerializer_Deserialize(const LnChar* str, const LnChar* basePath, LnHandle* outReturn);
extern "C" LN_FLAT_API void LnSerializer_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnAssetModel_Target(LnHandle assetmodel, LnHandle* outReturn);
extern "C" LN_FLAT_API LnResult LnAssetModel_Create(LnHandle target, LnHandle* outAssetModel);
extern "C" LN_FLAT_API void LnAssetModel_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnAssets_SaveAssetToLocalFile(LnHandle asset, const LnChar* filePath);
extern "C" LN_FLAT_API LnResult LnAssets_LoadAssetFromLocalFile(const LnChar* filePath, LnHandle* outReturn);
extern "C" LN_FLAT_API LnResult LnEngineSettings_SetMainWindowSize(int width, int height);
extern "C" LN_FLAT_API LnResult LnEngineSettings_SetMainBackBufferSize(int width, int height);
extern "C" LN_FLAT_API LnResult LnEngineSettings_SetMainWindowTitle(const LnChar* title);
extern "C" LN_FLAT_API LnResult LnEngineSettings_AddAssetDirectory(const LnChar* path);
extern "C" LN_FLAT_API LnResult LnEngineSettings_AddAssetArchive(const LnChar* fileFullPath, const LnChar* password);
extern "C" LN_FLAT_API LnResult LnEngineSettings_SetEngineLogEnabled(LnBool enabled);
extern "C" LN_FLAT_API LnResult LnEngineSettings_SetEngineLogFilePath(const LnChar* filePath);
extern "C" LN_FLAT_API LnResult LnEngine_Initialize();
extern "C" LN_FLAT_API LnResult LnEngine_Finalize();
extern "C" LN_FLAT_API LnResult LnEngine_Update(LnBool* outReturn);
extern "C" LN_FLAT_API LnResult LnEngine_MainUIView(LnHandle* outReturn);
extern "C" LN_FLAT_API LnResult LnApplication_OnInit(LnHandle application);
extern "C" LN_FLAT_API LnResult LnApplication_OnUpdate(LnHandle application);
extern "C" LN_FLAT_API LnResult LnApplication_Create(LnHandle* outApplication);
extern "C" LN_FLAT_API void LnApplication_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnGraphicsResource_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnTexture_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnTexture2D_Load(const LnChar* filePath, LnHandle* outReturn);
extern "C" LN_FLAT_API LnResult LnTexture2D_Create(int width, int height, LnHandle* outTexture2D);
extern "C" LN_FLAT_API LnResult LnTexture2D_CreateWithFormat(int width, int height, LnTextureFormat format, LnHandle* outTexture2D);
extern "C" LN_FLAT_API LnResult LnTexture2D_CreateFromFile(const LnChar* filePath, LnTextureFormat format, LnHandle* outTexture2D);
extern "C" LN_FLAT_API void LnTexture2D_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnComponent_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnVisualComponent_SetVisible(LnHandle visualcomponent, LnBool value);
extern "C" LN_FLAT_API LnResult LnVisualComponent_IsVisible(LnHandle visualcomponent, LnBool* outReturn);
extern "C" LN_FLAT_API void LnVisualComponent_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnSpriteComponent_SetTexture(LnHandle spritecomponent, LnHandle texture);
extern "C" LN_FLAT_API void LnSpriteComponent_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnComponentList_GetLength(LnHandle componentlist, int* outReturn);
extern "C" LN_FLAT_API LnResult LnComponentList_GetItem(LnHandle componentlist, int index, LnHandle* outReturn);
extern "C" LN_FLAT_API void LnComponentList_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetPosition(LnHandle worldobject, const LnVector3* pos);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetPositionXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_GetPosition(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetRotation(LnHandle worldobject, const LnQuaternion* rot);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetEulerAngles(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_GetRotation(LnHandle worldobject, LnQuaternion* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetScale(LnHandle worldobject, const LnVector3* scale);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetScaleS(LnHandle worldobject, float xyz);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetScaleXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_GetScale(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetCenterPoint(LnHandle worldobject, const LnVector3* value);
extern "C" LN_FLAT_API LnResult LnWorldObject_SetCenterPointXYZ(LnHandle worldobject, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnWorldObject_GetCenterPoint(LnHandle worldobject, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnWorldObject_GetComponents(LnHandle worldobject, LnHandle* outReturn);
extern "C" LN_FLAT_API void LnWorldObject_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnVisualObject_SetVisible(LnHandle visualobject, LnBool value);
extern "C" LN_FLAT_API LnResult LnVisualObject_IsVisible(LnHandle visualobject, LnBool* outReturn);
extern "C" LN_FLAT_API void LnVisualObject_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnSprite_SetTexture(LnHandle sprite, LnHandle value);
extern "C" LN_FLAT_API LnResult LnSprite_SetSourceRectXYWH(LnHandle sprite, float x, float y, float width, float height);
extern "C" LN_FLAT_API LnResult LnSprite_SetCallerTest(LnHandle sprite, LnHandle callback);
extern "C" LN_FLAT_API LnResult LnSprite_Create(LnHandle* outSprite);
extern "C" LN_FLAT_API LnResult LnSprite_CreateWithTexture(LnHandle texture, float width, float height, LnHandle* outSprite);
extern "C" LN_FLAT_API void LnSprite_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIEventArgs_Sender(LnHandle uieventargs, LnHandle* outReturn);
extern "C" LN_FLAT_API void LnUIEventArgs_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnUILayoutElement_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIElement_SetPosition(LnHandle uielement, const LnVector3* pos);
extern "C" LN_FLAT_API LnResult LnUIElement_SetPositionXYZ(LnHandle uielement, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnUIElement_GetPosition(LnHandle uielement, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_SetRotation(LnHandle uielement, const LnQuaternion* rot);
extern "C" LN_FLAT_API LnResult LnUIElement_SetEulerAngles(LnHandle uielement, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnUIElement_GetRotation(LnHandle uielement, LnQuaternion* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_SetScale(LnHandle uielement, const LnVector3* scale);
extern "C" LN_FLAT_API LnResult LnUIElement_SetScaleS(LnHandle uielement, float xyz);
extern "C" LN_FLAT_API LnResult LnUIElement_SetScaleXY(LnHandle uielement, float x, float y);
extern "C" LN_FLAT_API LnResult LnUIElement_GetScale(LnHandle uielement, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_SetCenterPoint(LnHandle uielement, const LnVector3* value);
extern "C" LN_FLAT_API LnResult LnUIElement_SetCenterPointXYZ(LnHandle uielement, float x, float y, float z);
extern "C" LN_FLAT_API LnResult LnUIElement_GetCenterPoint(LnHandle uielement, LnVector3* outReturn);
extern "C" LN_FLAT_API LnResult LnUIElement_AddChild(LnHandle uielement, LnHandle child);
extern "C" LN_FLAT_API void LnUIElement_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API void LnUIControl_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIButtonBase_SetText(LnHandle uibuttonbase, const LnChar* text);
extern "C" LN_FLAT_API void LnUIButtonBase_SetManagedTypeInfoId(int64_t id);
extern "C" LN_FLAT_API LnResult LnUIButton_Create(LnHandle* outUIButton);
extern "C" LN_FLAT_API LnResult LnUIButton_ConnectOnClicked(LnHandle uibutton, LnUIEventHandlerCallback handler);
extern "C" LN_FLAT_API void LnUIButton_SetManagedTypeInfoId(int64_t id);


VALUE g_enum_PixelFormat;
VALUE g_enum_TextureFormat;
VALUE g_enum_DepthBufferFormat;

VALUE g_rootModule;
VALUE g_class_Object;
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
VALUE g_class_ComponentList;
VALUE g_class_WorldObject;
VALUE g_class_VisualObject;
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

static VALUE Wrap_LnEngineSettings_SetMainBackBufferSize(int argc, VALUE* argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (LNRB_VALUE_IS_NUMBER(width) && LNRB_VALUE_IS_NUMBER(height))
        {
            int _width = LNRB_VALUE_TO_NUMBER(width);
            int _height = LNRB_VALUE_TO_NUMBER(height);
            LnResult errorCode = LnEngineSettings_SetMainBackBufferSize(_width, _height);
            if (errorCode < 0) rb_raise(rb_eRuntimeError, "Lumino runtime error. (%d)\n%s", errorCode, LnRuntime_GetLastErrorMessage());
            return Qnil;
        }
    }
    rb_raise(rb_eArgError, "ln::EngineSettings::setMainBackBufferSize - wrong argument type.");
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

    g_class_Quaternion = rb_define_class_under(g_rootModule, "Quaternion", rb_cObject);
    rb_define_alloc_func(g_class_Quaternion, LnQuaternion_allocate);

    g_class_Object = rb_define_class_under(g_rootModule, "Object", rb_cObject);
    rb_define_alloc_func(g_class_Object, LnObject_allocate);
    rb_define_method(g_class_Object, "on_serialize", LN_TO_RUBY_FUNC(Wrap_LnObject_OnSerialize), -1);
    LnObject_SetManagedTypeInfoId(LuminoRubyRuntimeManager::instance->registerTypeInfo(g_class_Object, LnObject_allocateForGetObject));
    LnObject_OnSerialize_SetOverrideCallback(Wrap_LnObject_OnSerialize_OverrideCallback);

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

    g_class_EngineSettings = rb_define_class_under(g_rootModule, "EngineSettings", rb_cObject);
    rb_define_singleton_method(g_class_EngineSettings, "set_main_window_size", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_SetMainWindowSize), -1);
    rb_define_singleton_method(g_class_EngineSettings, "set_main_back_buffer_size", LN_TO_RUBY_FUNC(Wrap_LnEngineSettings_SetMainBackBufferSize), -1);
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

