
set LUMINO_DIR=%~dp0

@echo off
setx "LuminoInstallDir" %LUMINO_DIR%

echo ----------
echo %LUMINO_DIR% を環境変数 LuminoInstallDir に設定しました。このフォルダは移動したり削除しないようにしてください。
echo ----------

set MY_DOCUMENTS=%USERPROFILE%\Documents
set VS2013_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2013\Templates\ProjectTemplates\
set VS2015_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2015\Templates\ProjectTemplates\

echo %VS2013_TEMPLATES_DIR%
echo %VS2015_TEMPLATES_DIR%

if exist "%VS2013_TEMPLATES_DIR%" (
copy "tools\VS2013ProjectTemplate\LuminoProjectCpp.zip" "%VS2013_TEMPLATES_DIR%\LuminoProjectCpp.zip"
echo "VisualStudio2015 にプロジェクトテンプレートをインストールしました。"
echo "[新しいプロジェクト] の [テンプレート] > [Visual C++] を確認してください。"
) else (
echo "VisualStudio2013 のプロジェクトテンプレートフォルダが見つからなかったため、テンプレートのインストールを行いませんでした。"
)

if exist "%VS2015_TEMPLATES_DIR%" (
copy "tools\VS2015ProjectTemplate\LuminoProjectCpp.zip" "%VS2015_TEMPLATES_DIR%\LuminoProjectCpp.zip"
echo "VisualStudio2015 にプロジェクトテンプレートをインストールしました。"
echo "[新しいプロジェクト] の [テンプレート] > [Visual C++] を確認してください。"
) else (
echo "VisualStudio2015 のプロジェクトテンプレートフォルダが見つからなかったため、テンプレートのインストールを行いませんでした。"
)

pause
