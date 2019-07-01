
#include <stdio.h>
#include "ruby.h"

void hello()
{
  printf("Hello, world!\n");
}

VALUE wrap_hello(self)
     VALUE self;
{
  hello();
  return Qnil;
}

void Init_Lumino()
{
  VALUE module;

  module = rb_define_module("Lumino");
  rb_define_module_function(module, "hello", wrap_hello, 0);
}
