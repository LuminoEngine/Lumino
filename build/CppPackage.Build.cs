//#define VS2013
using System;
using System.Text;
using System.IO;
using LuminoBuildTool;

class CppPackageRule : ModuleRule
{
    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "cpp-p"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Make package for C++."; } }

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
        string releaseDir = builder.LuminoPackageReleaseDir + "LuminoCpp_" + builder.VersionString + "/";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/Cpp/";
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoCpp_" + builder.VersionString + ".zip";
        
        Directory.CreateDirectory(releaseDir);

        // include には Engine, Core, Math のヘッダを全てコピーする
        Logger.WriteLine("copy include files...");
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "include"), Path.Combine(releaseDir, "include"), true);
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "external/Lumino.Core/include"), Path.Combine(releaseDir, "include"), true);

        // .lib
        Logger.WriteLine("copy lib files...");
#if VS2013
        Directory.CreateDirectory(releaseDir + "lib/MSVC120/x86");
        Utils.CopyFiles(builder.LuminoLibDir + "MSVC120/x86/Debug", "*.lib", releaseDir + "lib/MSVC120/x86");
        Utils.CopyFiles(builder.LuminoLibDir + "MSVC120/x86/Release", "*.lib", releaseDir + "lib/MSVC120/x86");
#endif
        Directory.CreateDirectory(releaseDir + "lib/MSVC140/x86");
        Utils.CopyFiles(builder.LuminoLibDir + "MSVC140/x86/Debug", "*.lib", releaseDir + "lib/MSVC140/x86");
        Utils.CopyFiles(builder.LuminoLibDir + "MSVC140/x86/Release", "*.lib", releaseDir + "lib/MSVC140/x86");

        // インストールスクリプトとか、プロジェクトテンプレート
        Logger.WriteLine("copy other files...");
#if VS2013
        Directory.CreateDirectory(releaseDir + "tools/VS2013ProjectTemplate");
#endif
        Directory.CreateDirectory(releaseDir + "tools/VS2015ProjectTemplate");
        Utils.CopyFile(pkgSrcDir + "Lumino_Install.bat", releaseDir);
        Utils.CopyFile(pkgSrcDir + "Lumino_Uninstall.bat", releaseDir);
#if VS2013
        Utils.CreateZipFile(builder.LuminoToolsDir + "VS2013ProjectTemplate/LuminoProjectCpp", releaseDir + "tools/VS2013ProjectTemplate/LuminoProjectCpp.zip", false);
#endif
        Utils.CreateZipFile(builder.LuminoToolsDir + "VS2015ProjectTemplate/LuminoProjectCpp", releaseDir + "tools/VS2015ProjectTemplate/LuminoProjectCpp.zip", false);

        // Readme.txt (バージョン名を埋め込む)
        string text = File.ReadAllText(pkgSrcDir + "Readme.txt");
        text = text.Replace("$(LuminoVersion)", builder.VersionString);
        File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));
        
        // ReleaseNote
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", releaseDir);
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", builder.LuminoPackageReleaseDir);

        // .zip に圧縮する
        Logger.WriteLine("compressing files...");
        Utils.CreateZipFile(releaseDir, zipFilePath);
    }
}
