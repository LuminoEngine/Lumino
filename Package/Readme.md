
PackageSource フォルダには、配布するパッケージに含めるファイルを置いておく。

例えば、インストールスクリプトや Readme のソースファイルなど。



"C:\Program Files (x86)\WiX Toolset v3.11\bin\heat" dir Release/Lumino -dr LUMINO_DIR -cg LuminoFiles -gg -g1 -sfrag -srd  -var "var.LuminoFilesDir" -out LuminoFiles.wxs

heat" dir <フォルダパス> -dr <関連付けるディレクトリID> -cg <ComponentGroupの名前> -gg -g1 -sfrag -srd  -var <SourceDirを置き換える変数> -out <出力ファイル名>


http://www.02.246.ne.jp/~torutk/windows/wixandwixedit.html#SEC2

```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo LuminoInstaller.wxs
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo -dLuminoFilesDir=Release/Lumino LuminoFiles.wxs
```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\light.exe" -nologo -ext WixUIExtension -cultures:ja-jp LuminoInstaller.wixobj LuminoFiles.wixobj -out LuminoInstaller.msi

