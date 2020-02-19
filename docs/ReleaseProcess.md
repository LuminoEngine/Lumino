リリース手順
==========

バージョン番号の修正
----------

- src/LuminoCore/NuGet/Lumino.Core.nuspec
- tools/LuminoBuild/LuminoBuildTool.cs
    - GUID も修正する。`powershell -Command "[Guid]::NewGuid()"` で打ち出せる。





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
### Web Site

docs/WebDocument/_site

の中身を丸ごと配置する。

### リリースノート
※1.0.0 までは用意しない。


