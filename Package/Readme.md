
PackageSource フォルダには、配布するパッケージに含めるファイルを置いておく。

例えば、インストールスクリプトや Readme のソースファイルなど。



"C:\Program Files (x86)\WiX Toolset v3.11\bin\heat" dir Release/Lumino -dr LUMINO_DIR -cg LuminoFiles -gg -g1 -sfrag -srd  -var "var.LuminoFilesDir" -out LuminoFiles.wxs

heat" dir <フォルダパス> -dr <関連付けるディレクトリID> -cg <ComponentGroupの名前> -gg -g1 -sfrag -srd  -var <SourceDirを置き換える変数> -out <出力ファイル名>

LUMINO_MSVC2013

http://www.02.246.ne.jp/~torutk/windows/wixandwixedit.html#SEC2


```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\heat" dir Release/Lumino/MSVC2013 -dr LUMINO_MSVC2013 -cg LuminoFiles -gg -g1 -sfrag -srd  -var "var.LuminoFilesDir" -out obj/Files.wxs

```

```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo LuminoInstaller_VC2013.wxs -out obj/LuminoInstaller.wixobj
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo -dLuminoFilesDir=Release/Lumino/MSVC2013 obj/Files.wxs -out obj/Files.wixobj
```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\light.exe" -nologo -ext WixUIExtension -cultures:ja-jp obj/LuminoInstaller.wixobj obj/Files.wixobj -pdbout out/LuminoInstaller.wixpdb -out Release/LuminoInstaller.msi


--------------------
インストーラ、というより配布パッケージは開発環境(VisualStudioのバージョン)ごとに分ける。
これは、ひとまとめにするとファイルサイズがとんでもなく大きくなるため。
※VS2013,2015,2017のランタイムライブラリ,文字コードセット,Debug/Release のlib で約 2GByte。msi に圧縮しても 500MByteくらいになる。



```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\heat" dir Release/Lumino/MSVC2013 -dr LUMINO_DIR -cg LuminoFiles -gg -g1 -sfrag -srd  -var "var.LuminoFilesDir" -out LuminoFiles.wxs

"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo LuminoInstaller_VC2013.wxs -out LuminoInstaller.wixobj
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo -dLuminoFilesDir=C:\Proj\LN\HC1\External\Lumino\Package\Release\Lumino\MSVC2013 LuminoFiles.wxs -out LuminoFiles.wixobj

"C:\Program Files (x86)\WiX Toolset v3.11\bin\light.exe" -nologo -ext WixUIExtension -cultures:ja-jp LuminoInstaller.wixobj LuminoFiles.wixobj -out LuminoInstaller.msi

```


```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\heat" dir Release/Lumino/MSVC2013 -dr LUMINO_MSVC2013 -cg LuminoFiles -gg -g1 -sfrag -srd -var "var.LuminoFilesDir" -out LuminoFiles.wxs

"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo LuminoInstaller_VC2013.wxs -out LuminoInstaller.wixobj
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo -dLuminoFilesDir=Release/Lumino/MSVC2013 LuminoFiles.wxs -out LuminoFiles.wixobj

"C:\Program Files (x86)\WiX Toolset v3.11\bin\light.exe" -nologo -ext WixUIExtension -cultures:ja-jp LuminoInstaller.wixobj LuminoFiles.wixobj -out LuminoInstaller.msi

```


```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\heat" dir Release/Lumino/MSVC2017 -dr LUMINO_MSVC2017 -cg LuminoFiles -gg -g1 -sfrag -srd -var "var.LuminoFilesDir" -out LuminoFiles.wxs
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo LuminoInstaller_VC2017.wxs -out LuminoInstaller.wixobj
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo -dLuminoFilesDir=Release/Lumino/MSVC2017 LuminoFiles.wxs -out LuminoFiles.wixobj
"C:\Program Files (x86)\WiX Toolset v3.11\bin\light.exe" -nologo -ext WixUIExtension -cultures:ja-jp LuminoInstaller.wixobj LuminoFiles.wixobj -out LuminoInstaller_VC2017.msi
```

```
"C:\Program Files (x86)\WiX Toolset v3.11\bin\heat" dir Release/Lumino/MSVC2017 -dr LUMINO_MSVC2017 -cg LuminoFiles -gg -g1 -sfrag -srd -var "var.LuminoFilesDir" -out LuminoFiles.wxs
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo LuminoInstaller_VC2017.wxs -out LuminoInstaller.wixobj
"C:\Program Files (x86)\WiX Toolset v3.11\bin\candle.exe" -nologo -dLuminoFilesDir=Release/Lumino/MSVC2017 LuminoFiles.wxs -out LuminoFiles.wixobj
"C:\Program Files (x86)\WiX Toolset v3.11\bin\light.exe" -nologo -ext WixUIExtension -cultures:ja-jp LuminoInstaller.wixobj LuminoFiles.wixobj -out LuminoInstaller_VC2017.msi
```

msiexec /i LuminoInstaller.msi /l*vx logfile.txt



https://code.msdn.microsoft.com/windowsdesktop/Programmatically-retrieve-186b12c0

Wix - How do I set property conditionally
http://stackoverflow.com/questions/26753632/wix-how-do-i-set-property-conditionally

SetProperty エレメント
http://cml.s10.xrea.com/ej/WiX/xsd/wix_xsd_setproperty.htm





### プロジェクトテンプレートについて
インストールできるのは2箇所。
- MyDocument\Visual Studio XXXX\Templates\ProjectTemplates
- <VSインストールフォルダ>\Common7\IDE\ProjectTemplates
ただし、後者は VC++ 用のテンプレートはインストールできない。
VC++ はプロジェクトウィザードという別の形式で作らなければならず、インストールフォルダは <VSインストールフォルダ>\VC の中としなければならない。(Qt とか参考になる)


### Visual Studio のテンプレートディレクトリ
VS2015 以前は HKEY_CURRENT_USER\Software\Microsoft\VisualStudio\12.0@UserProjectTemplatesLocation あたりから取り出せる。
しかし、VS2017 からはレジストリには記録されなくなったようだ。（全検索しても見つからず）
AppData の中にも無い・・・。





