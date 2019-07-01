
//==============================================================================
// ln::Engine

struct Wrap_Engine
{
    Wrap_Engine()
    {}
};

static VALUE Wrap_LnEngine_Initialize(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "ln::Engine::initialize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_Finalize(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "ln::Engine::finalize - wrong argument type.");
    return Qnil;
}

static VALUE Wrap_LnEngine_Update(int argc, VALUE *argv, VALUE self)
{
    rb_raise(rb_eArgError, "ln::Engine::update - wrong argument type.");
    return Qnil;
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

