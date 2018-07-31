




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
