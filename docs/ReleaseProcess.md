リリース手順
==========

バージョン番号の修正
----------

- src/LuminoCore/NuGet/Lumino.Core.nuspec
- tools/LuminoBuild/LuminoBuildTool.cs
    - GUID も修正する。`powershell -Command "[Guid]::NewGuid()"` で打ち出せる。
- tools/Bindings/Ruby/GemProject/lib/lumino/version.rb



ビルド
----------
### Windows

[Developer Command Prompt] で以下を実行する。

```
dotnet run
```

`build/NuGetPackage-LuminoCore/Lumino.Core.x.y.z.nupkg` を配布する。


Documents
----------

### Website

https://github.com/LuminoEngine/Website

build.py の中で Lumino リポジトリをタグ指定 clone している個所を変更する。

### リリースノート
※1.0.0 までは用意しない。


