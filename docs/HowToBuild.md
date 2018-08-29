




### Windows


スタートボタン + X > [Windows PowerShell(管理者)]

```
choco install cmake
choco install nuget.commandline
docfx
doxygen.install
```



on Developer Command Prompt for VS2017
```
> dotnet run -- MakeVSProjects
```

After this, please check in the "build/..." folder.


### macOS

```
$ brew install git

brew update
$ brew install openssl
$ brew install python3
```

Error downloading URL 'https://github.com/kripken/emscripten/archive/1.38.11.tar.gz': <urlopen error [SSL: TLSV1_ALERT_PROTOCOL_VERSION] tlsv1 alert protocol version (_ssl.c:590)>

wget https://github.com/kripken/emscripten/archive/1.38.10.tar.gz

### Linux(Ubuntu)

install `dotnet`

https://www.microsoft.com/net/download/linux-package-manager/ubuntu16-04/sdk-current

```
$ dotnet run -- BuildExternalProjects
$ dotnet run -- BuildEngine_Linux
```





Note
--------------------

on Developer Command Prompt for VS2017
```
> dotnet run -- MakeVSProjects
```

After this, please check in the "build/..." folder.


emscripten

emsdk Windows installer
http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html

https://gist.github.com/faithandbrave/9b3d439d135e63abdbe7
