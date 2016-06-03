
@echo off
set MY_DOCUMENTS=%USERPROFILE%\Documents
set VS_TEMPLATES_DIR=%MY_DOCUMENTS%\Visual Studio 2015\Templates\ProjectTemplates\

echo %VS_TEMPLATES_DIR%

if exist "%VS_TEMPLATES_DIR%" (
copy "tools\LuminoProjectCS.zip" "%VS_TEMPLATES_DIR%\LuminoProjectCS.zip"
echo "VisualStudio2015 にプロジェクトテンプレートをインストールしました。"
echo "[新しいプロジェクト] の [テンプレート] > [Visual C#] を確認してください。"
) else (
echo "VisualStudio2015 のプロジェクトテンプレートフォルダが見つからなかったため、テンプレートのインストールを行いませんでした。"
)

pause
