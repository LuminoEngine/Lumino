


Rake に関する情報は https://ruby.github.io/rake/doc/rakefile_rdoc.html

Install requirments
----------

https://community.chocolatey.org/packages/msys2

```
cinst ruby
cinst msys2 --params "/NoUpdate"
Update-SessionEnvironment
ridk install 2 3
```

ターミナルを起動するために次のコマンドが必要。

```
ridk enable
```

Create pkg
----------

```
rake build
```

Test compile
----------

`rake build` では実際にコンパイルしない。インストールした時にコンパイルされる。

パッケージを作らずにコンパイルエラーをチェックしたい場合は次のコマンドを使う。

```
rake compile
```

Gem directory structure
----------

- 📂gem_project
  - 📂ext
    - 📂lumino
      - 📄extconf.rb


[rake-compiler README.md](https://github.com/rake-compiler/rake-compiler) によると、 ext フォルダの中はパッケージ名で掘るのが良いらしい。
Rake::ExtensionTask のコストラクタには Extension name が必須だが、これを基に ext/[name]/extconf.rb を探しに行くため。

----------

### 準備

```
choco install -y ruby
choco install -y msys2
choco install -y ruby2.devkit
Update-SessionEnvironment
ridk install 1 2 3
bundle install

ruby dk.rb init
ruby dk.rb install
```


### ドキュメントのビルド

```
rake yard
```

確認は doc/index.html から。

### ビルド

```
rake build
```

※パッケージを作るだけで、実際にソースコードのビルドはまだ行われない

### ローカルインストール

```
gem install pkg/lumino-0.10.0.gem
```

### アンインストール

```
gem uninstall lumino
```


### テスト
```
rake test
rake test TESTOPTS="-n'test_delegate_object_block'"
```

### ruby.h の場所

C:/Ruby26-x64/include/ruby-2.6.0/ruby.h 等。

--------------------------------------------------






https://docs.ruby-lang.org/en/2.1.0/README_EXT_ja.html


set PATH=C:\Ruby26\bin;C:\Ruby26\msys32\mingw32\bin;C:\Ruby26\msys32\usr\bin;%PATH%
set PATH=C:\tools\msys64\mingw64\bin;C:\tools\msys64\usr\bin;%PATH%
ruby extconf.rb --with-Lumino-include=../FlatC/include
make
ruby Test.rb





https://docs.ruby-lang.org/en/2.4.0/extension_ja_rdoc.html#label-E5-A4-A7-E5-9F-9F-E5-A4-89-E6-95-B0-E5-AE-9A-E7-BE-A9



```
choco install -y ruby
choco install -y msys2
Update-SessionEnvironment
ridk install 2 3





choco install -y ruby2.devkit
```
C:\tools\msys64

Documentation
----------
yard を使用します。

```
gem install yard
```

```
yardoc --main YARD.Main Lumino.RubyYARDOCSource.generated.rb
```

./doc フォルダに生成される。


Gem
----------

準備
```
gem install bundler 
gem install rake-compiler
or
bundle install
```

テストのためインストールせずにコンパイルする

```
set PATH=C:\tools\msys64\mingw64\bin;C:\tools\msys64\usr\bin;%PATH%

```


ビルド
```
rake build
```

デバッグ
```
gem install pkg/lumino-0.10.0.pre.gem
gem uninstall lumino
```

インストール時にビルドが走る
```
>gem install pkg/lumino-0.10.0.gem
Temporarily enhancing PATH for MSYS/MINGW...
Building native extensions. This could take a while...
Successfully installed lumino-0.10.0
Parsing documentation for lumino-0.10.0
Installing ri documentation for lumino-0.10.0
Done installing documentation for lumino after 0 seconds
1 gem installed
```

C:\tools\ruby26\lib\ruby\gems\2.6.0\gems\lumino-0.10.0


spec.files
https://www.xmisao.com/2014/08/26/get-all-files-in-gemspec.html

拡張ライブラリの gem をつくる
http://tohka383.hatenablog.jp/entry/20130930/1380471977

BundlerでC拡張を含んだgemを公開する
https://gam0022.net/blog/2013/10/18/gems-with-extensions/

gmake に paht が通っていると、それを使ってしまってエラーになるので注意
https://github.com/rake-compiler/rake-compiler/blob/master/lib/rake/extensiontask.rb#L488
gmake: *** No rule to make target '/C/Ruby26/include/ruby-2.6.0/ruby.h', needed by 'Lumino.RubyExt.generated.o'.  Stop.
とかエラーになる

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

```cpp
static void LnUIEventHandlerCallback_Tmp(LNHandle e)
{
    printf("Clicked!!! self:%d\n", self);
    
    
    Wrap_UIButton* selfObj;
    Data_Get_Struct(LuminoRubyRuntimeManager::instance->wrapObject(self), Wrap_UIButton, selfObj);
    printf("rb_funcall\n");
    printf("selfObj:%p\n", selfObj);
    printf("connectOnClicked_Signal:%d\n", selfObj->connectOnClicked_Signal);
    VALUE retval = rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("raise"), 0);

    printf("Clicked!!! e\n");
}

static VALUE Wrap_LnUIButton_ConnectOnClicked(int argc, VALUE* argv, VALUE self)
{
    Wrap_UIButton* selfObj;
    Data_Get_Struct(self, Wrap_UIButton, selfObj);
    printf("selfObj:%p\n", selfObj);
    
    if (!selfObj->connectOnClicked_EventConnect) {  // differed initialization.
        printf("con i.\n");
        selfObj->connectOnClicked_Signal = rb_funcall(LuminoRubyRuntimeManager::instance->eventSignalClass(), rb_intern("new"), 0);
        printf("con %d\n", selfObj->connectOnClicked_Signal);
        LnUIButton_ConnectOnClicked(selfObj->handle, LnUIEventHandlerCallback_Tmp);
        selfObj->connectOnClicked_EventConnect = true;
        printf("con e.\n");
    }
    
    VALUE handler;
    rb_scan_args(argc, argv, "1", &handler);
    VALUE retval = rb_funcall(selfObj->connectOnClicked_Signal, rb_intern("add"), 1, handler);

    return retval;
}
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


