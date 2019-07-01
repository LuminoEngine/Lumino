
#include <stdio.h>
#include "ruby.h"

void hello()
{
  printf("Hello, world!\n");
}

struct Wrap
{
	static VALUE wrap_hello(VALUE self)
	{
	  hello();
	  return Qnil;
	}
};


static VALUE wrap_hello(VALUE self)
{
  hello();
  return Qnil;
}

#define LN_TO_RUBY_FUNC( f ) reinterpret_cast< VALUE(__cdecl *)(...) >( f )

extern "C" void Init_Lumino()
{
  VALUE module;

  module = rb_define_module("Lumino");
  rb_define_module_function(module, "hello", LN_TO_RUBY_FUNC(Wrap::wrap_hello), 0);
}
