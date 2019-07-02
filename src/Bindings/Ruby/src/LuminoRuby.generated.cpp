
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

VALUE g_module;
VALUE g_class_RefObject;

extern "C" void Init_Lumino()
{

  g_module = rb_define_module("Lumino");
  rb_define_module_function(g_module, "hello", LN_TO_RUBY_FUNC(Wrap::wrap_hello), 0);
	

	g_class_RefObject = rb_define_class_under(g_module, "RefObject", rb_cObject);
	
	VALUE v = rb_path2class("Lumino::RefObject");
	
	printf("%d %d\n", g_class_RefObject, v);

}
