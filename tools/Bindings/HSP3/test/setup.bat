copy %~dp0..\..\..\..\build\MSVC2019-x86-MT\EngineBuild\tools\Bindings\HSP3\Debug\LuminoHSP3d.dll %~dp0LuminoHSP3d.dll
copy %~dp0..\..\..\..\build\MSVC2019-x86-MT\EngineBuild\tools\Bindings\HSP3\Release\LuminoHSP3.dll %~dp0LuminoHSP3.dll
copy %~dp0..\lumino.as %~dp0lumino.as

echo "HSP3 エディタのインストールフォルダに Lumino の dll がある場合は削除してください。"

pause


