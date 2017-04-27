using System;
using System.Text;
using System.IO;
using LuminoBuildTool;

class DotNetPackageRule : ModuleRule
{
    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "cs-p"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Make package for C# (.Net)."; } }

    /// <summary>
    /// 前提条件の確認
    /// </summary>
    public override void CheckPrerequisite(Builder builder)
    {
        Buildable = true;
    }

    /// <summary>
    /// ビルド実行
    /// </summary>
    public override void Build(Builder builder)
    {
        string dotnetDir = builder.LuminoBindingsDir + "DotNet/";
        string releaseDir = builder.LuminoPackageReleaseDir + "LuminoDotNet_" + builder.VersionString + "/";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/DotNet/";
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoDotNet_" + builder.VersionString + ".zip";
        
        Directory.CreateDirectory(releaseDir);

        // lib
        string outputDir = dotnetDir + "LuminoDotNet/bin/x86/Release/";
        string releaseLibDir = releaseDir + "lib/";
        Logger.WriteLine("copy lib files...");
        Directory.CreateDirectory(releaseLibDir);
        Utils.CopyFile(outputDir + "LuminoDotNet.dll", releaseLibDir);
        Utils.CopyFile(outputDir + "LuminoDotNet.XML", releaseLibDir);
        if (Utils.IsWin32)
            File.Copy(builder.LuminoLibDir + "MSVC140/x86/Release/LuminoCU.dll", releaseLibDir + "LuminoCU.dll", true);
        else
            File.Copy(builder.LuminoLibDir + "x86/Release/LuminoCU.so", releaseLibDir + "LuminoCU.so", true);
        
        // Help
        if (Utils.IsWin32)
        {
            Logger.WriteLine("copy Help files...");
            Directory.CreateDirectory(releaseDir + "doc");
            Utils.CopyDirectory(dotnetDir + "Help", releaseDir + "doc", true);
        }

        // Readme.txt (バージョン名を埋め込む)
        string text = File.ReadAllText(pkgSrcDir + "Readme.txt");
        text = text.Replace("$(LuminoVersion)", builder.VersionString);
        File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));

        // ReleaseNote
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", releaseDir);

        // VSプロジェクトテンプレート
        Logger.WriteLine("make project template...");
        string templateProjDir = builder.LuminoToolsDir + "VS2015ProjectTemplate/LuminoProjectCS/";
        Directory.CreateDirectory(templateProjDir + "Assets");
        Directory.CreateDirectory(releaseDir + "tools");
        Utils.CopyFile(builder.LuminoDocDir + "Logo/icon256.png", templateProjDir + "Assets");
        Utils.CopyDirectory(releaseLibDir, templateProjDir + "LuminoLibrary");
        Utils.CreateZipFile(templateProjDir, releaseDir + "tools/LuminoProjectCS.zip", false);

        // installer
        Utils.CopyFile(pkgSrcDir + "Lumino_Install.bat", releaseDir);
        Utils.CopyFile(pkgSrcDir + "Lumino_Uninstall.bat", releaseDir);

        // .zip に圧縮する
        Logger.WriteLine("compressing files...");
        Utils.CreateZipFile(releaseDir, zipFilePath);
    }
}
