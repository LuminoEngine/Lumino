
@echo off

setx LuminoInstallDir ""

set MY_DOCUMENTS=%USERPROFILE%\Documents
set VS_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2015\Templates\ProjectTemplates\

del "%VS_TEMPLATES_DIR%LuminoProject.zip"

echo ----------
echo Lumino をアンインストールしました。

pause
