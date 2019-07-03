

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


