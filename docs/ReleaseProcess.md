リリース手順
==========

バージョン番号の修正
----------

* src/LuminoCore/NuGet/Lumino.Core.nuspec
* tools/LuminoBuild/LuminoBuildTool.cs


ビルド
----------
### Windows

[Developer Command Prompt for VS2017] で以下を実行する。

```
dotnet run -- MakePackage
```

`build/NuGetPackage-LuminoCore/Lumino.Core.x.y.z.nupkg` を配布する。


Documents
----------
### Web Site

docs/WebDocument/_site

の中身を丸ごと配置する。


