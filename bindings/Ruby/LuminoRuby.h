#include <vector>
#include <stack>
#include "ruby.h"
#include "../../C_API/include/LuminoC.h"

// Internal functions.
extern "C" LNUserData LNObject_GetBindingTypeData(LNHandle hadnleObject);


extern VALUE g_luminoModule;
extern VALUE g_luminoError;  // exception
extern VALUE g_class_RefObject;
extern VALUE g_class_ObjectList;

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

// WrapperObject �̃x�[�X�N���X
struct wrapRefObject
{
	LNHandle	Handle;

	wrapRefObject() : Handle(0) {}
};

struct wrapObjectList
    : public wrapRefObject
{
    std::vector<VALUE> m_cacheList;
};

class Manager
{
public:
	static void Initialize();
	static void Finalize();
	static VALUE GetWrapperObjectFromHandle(LNHandle handle);
	static LNHandle GetHandleFromtWrapperObject(VALUE obj);
	static void RegisterWrapperObject(VALUE obj);
	static void UnregisterWrapperObject(LNHandle handle);

private:
	static void RegisterTypeInfo();

	static const int InitialListSize = 1024;
	static std::vector<TypeInfo>	m_typeInfoList;
	static std::vector<VALUE>		m_objectList;	// ���g�̓q�[�v�Ɋm�ۂ����铮�I�z���Ȃ̂ŁAmark ���Ȃ����Ύ��Q�Ƃ� VALUE �Ƃ��Ďg����
	static std::stack<int>			m_objectListIndexStack;
};


/* BinderMaker �����G�ɂȂ肷���Ȃ��悤�A�������x�̓}�N���ŃJ�o�[�����B*/

// typeName	: "LN" �����̃N���X��
#define LNRB_REGISTER_TYPEINFO(typeName) \
	{ \
		TypeInfo t; \
		t.klass = g_class_##typeName; \
		t.factory = LN##typeName##_allocateForGetRefObject; \
		m_typeInfoList.push_back(t); \
		LN##typeName##_SetBindingTypeInfo((void*)(m_typeInfoList.size() - 1)); \
	}


// Utils
extern bool checkEqualHandle(VALUE obj, LNHandle handle);
