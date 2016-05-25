#include <vector>
#include <stack>
#include "ruby.h"
#include "../../C_API/include/LuminoC.h"

// Internal functions.
extern "C" LNUserData LNObject_GetBindingTypeData(LNHandle hadnleObject);


extern VALUE g_luminoModule;
extern VALUE g_luminoError;  // exception

extern void InitEnums();
extern void InitStructs();
extern void InitClasses();

#define LN_TO_RUBY_FUNC( f ) reinterpret_cast< VALUE(__cdecl *)(...) >( f )

inline bool isRbObject(VALUE v) { return (TYPE(v) == T_DATA/*T_OBJECT*/) || (v == Qnil); }
inline bool isRbNumber(VALUE v) { return (TYPE(v) == T_FIXNUM || TYPE(v) == T_BIGNUM) || (v == Qnil); }
inline bool isRbFloat(VALUE v) { return (TYPE(v) == T_FLOAT) || (isRbNumber(v)) || (v == Qnil); }
inline bool isRbString(VALUE v) { return (TYPE(v) == T_STRING) || (v == Qnil); }
inline bool isRbBool(VALUE v) { return (TYPE(v) == T_FALSE || TYPE(v) == T_TRUE) || (v == Qnil); }

inline LNBool RbBooltoBool(VALUE v) { return (TYPE(v) == T_TRUE) ? LN_TRUE : LN_FALSE; }

inline VALUE toVALUE(const char* v)		{ return rb_str_new2(v); }
inline VALUE toVALUE(LNBool v)			{ return (v != LN_FALSE) ? Qtrue : Qfalse; }
inline VALUE toVALUE(uint8_t v)			{ return INT2FIX(v); }
inline VALUE toVALUE(int v)				{ return INT2NUM(v); }
inline VALUE toVALUE(uint32_t v)			{ return INT2NUM(v); }
inline VALUE toVALUE(int64_t v)			{ return INT2NUM(v); }
inline VALUE toVALUE(uint64_t v)			{ return INT2NUM(v); }
inline VALUE toVALUE(float v)			{ return DBL2NUM(v); }
inline VALUE toVALUE(double v)			{ return DBL2NUM(v); }
//inline VALUE toVALUE(lnErrorCode v)		{ return INT2FIX(v); }
//inline VALUE toVALUE(lnIntPtr v)		{ return INT2NUM(v); }

extern "C"
{

// Internal API
extern const char* LNInternal_ConvertToUTF8String(const LNChar* str, int len);
extern const char* LNGetLastErrorMessage();

} // extern "C"

typedef VALUE(*ObjectFactory)(VALUE klass, LNHandle handle);

struct TypeInfo
{
	VALUE			klass;
	ObjectFactory	factory;
};

class Manager
{
public:
	static void Initialize();
	static void Finalize();
	static VALUE GetWrapperObjectFromHandle(LNHandle handle);
	static void RegisterWrapperObject(VALUE obj);
	static void UnregisterWrapperObject(LNHandle handle);
	
private:
	static void RegisterTypeInfo();
	
	static const int InitialListSize = 1024;
	static std::vector<TypeInfo>	m_typeInfoList;
	static std::vector<VALUE>		m_objectList;
	static std::stack<int>			m_objectListIndexStack;
};

/* BinderMaker が複雑になりすぎないよう、ある程度はマクロでカバーする。*/

// typeName	: "LN" 無しのクラス名
#define LNRB_REGISTER_TYPEINFO(typeName) \
	{ \
		extern void LN##typeName##_SetBindingTypeInfo(void* data); \
		TypeInfo t; \
		t.klass = g_class_##typeName; \
		t.factory = LN##typeName##_allocateForGetRefObject; \
		m_typeInfoList.push_back(t); \
		LN##typeName##_SetBindingTypeInfo((void*)(m_typeInfoList.size() - 1)); \
	}







