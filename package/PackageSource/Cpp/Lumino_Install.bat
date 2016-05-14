
set LUMINO_DIR=%~dp0

@echo off
setx "LuminoInstallDir" %LUMINO_DIR%

echo ----------
echo %LUMINO_DIR% を環境変数 LuminoInstallDir に設定しました。このフォルダは移動したり削除しないようにしてください。
echo ----------

set MY_DOCUMENTS=%USERPROFILE%\Documents
set VS_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2015\Templates\ProjectTemplates\

echo %VS_TEMPLATES_DIR%

if exist "%VS_TEMPLATES_DIR%" (
copy "tools\LuminoProject.zip" "%VS_TEMPLATES_DIR%\LuminoProject.zip"
echo "VisualStudio2015 にプロジェクトテンプレートをインストールしました。"
echo "[新しいプロジェクト] の [テンプレート] > [Visual C++] を確認してください。"
) else (
echo "VisualStudio2015 のプロジェクトテンプレートフォルダが見つからなかったため、テンプレートのインストールを行いませんでした。"
)

pause
