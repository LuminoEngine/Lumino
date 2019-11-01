
set PATH=C:\Ruby26\bin;C:\Ruby26\msys32\mingw32\bin;C:\Ruby26\msys32\usr\bin;%PATH%
ruby extconf.rb --with-Lumino-include=../FlatC/include
make
ruby Test.rb





https://docs.ruby-lang.org/en/2.4.0/extension_ja_rdoc.html#label-E5-A4-A7-E5-9F-9F-E5-A4-89-E6-95-B0-E5-AE-9A-E7-BE-A9




Important
----------
LuminoRuntime.dll は Release ビルドでなければ、ロードに失敗する。
Debug 版でリンクされているライブラリが、Ruby からは参照できていない。
※/MT でビルドすれば大丈夫かも。


```
int state = 0;
rb_eval_string_protect(
      "class Event_Void\n"
      "  def initialize\n"
      "    @handlers = []\n"
      "  end\n"
      "  def add(handler)\n"
      "    @handlers.push(handler)\n"
      "  end\n"
      "  def raise(*args)\n"
      "    @handlers.each do |h|\n"
      "      h.call(*args)\n"
      "    end\n"
      "  end\n"
      "end\n",
      &state
);
```

```
static VALUE Wrap_LnUIButton_ConnectOnClicked(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(self, Wrap_UIButton, selfObj);

    if (!selfObj->connectOnClicked_EventConnect) {  // differed initialization.
        selfObj->handle->connectOnClicked_Signal = rb_funcall(g_class_Event_Void, rb_intern("new"), 0);
        LnUIButton_ConnectOnClicked(selfObj->handle, );
        selfObj->connectOnClicked_EventConnect = true;
    }
    
    VALUE handler;
    rb_scan_args(argc, argv, "1", &handler);
    VALUE retval = rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("add"), 1, handler);

    test
    rb_raise(rb_eArgError, "ln::UIButton::connectOnClicked - wrong argument type.");
    return Qnil;
}



VALUE a = rb_intern("Lumino::UIButton");
VALUE b = rb_eval_string("Lumino::UIButton");
printf("%d\n", a);
printf("%d\n", b);
printf("%d\n", g_class_UIButton);
```

-----------------------------------------



C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional>dumpbin /HEADERS C:\Proj\LN\Lumino\src\Bindings\Ruby\Lumino.so  | findstr machine
             14C machine (x86)
                   32 bit word machine

C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional>dumpbin /HEADERS C:\Ruby26\bin\ruby.exe  | findstr machine
             14C machine (x86)
                   32 bit word machine

C:\Program Files (x86)\Microsoft Visual Studio\2017\Professional>dumpbin /HEADERS C:\Proj\LN\Lumino\src\Bindings\Ruby\LuminoRuntime.dll  | findstr machine
             14C machine (x86)
                   32 bit word machine



```
choco install ruby
choco install ruby2.devkit
```

http://algo13.net/setup_win/ruby.html


### Build

```
set PATH=C:\tools\DevKit2\mingw\bin;C:\tools\DevKit2\bin;%PATH%
set PATH=C:\tools\Ruby26-x86\bin;C:\tools\Ruby26-x86\msys32\mingw32\bin;C:\tools\Ruby26-x86\msys32\usr\bin;%PATH%
ruby ./extconf.rb
make
```
※ PATH は前に追加しておく。そうしないと、mingw のものではない実行ファイルを参照してしまうことがある。

Test
```
ruby -r'./Lumino' -e'Lumino.hello'
```


set PATH=C:\tools\Ruby26-x86\bin;C:/tools/Ruby26-x86/msys32/usr/local/bin;C:/tools/Ruby26-x86/msys32/usr/bin;C:/tools/Ruby26-x86/msys32/bin;C:/tools/Ruby26-x86/msys32/opt/bin;%PATH%

PATH=/C/tools/Ruby26-x86/bin:${PATH}


ruby dk.rb init

 /c/Proj/LN/Lumino/src/Bindings/Ruby

https://www.javadrive.jp/ruby/install/index1.html



mingw32.exe



ridk install
ridk enable

set PATH=%CD%;%PATH%


