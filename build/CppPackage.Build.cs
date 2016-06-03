using System;
using System.Text;
using System.IO;
using System.IO.Compression;
using LuminoBuildTool;

class CppPackageRule : ModuleRule
{
    /// <summary>
    /// ルールの名前
    /// </summary>
    public override string Name
    {
        get { return "CppPackage"; }
    }

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
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "external/Lumino.Math/include"), Path.Combine(releaseDir, "include"), true);

        // .lib
        Logger.WriteLine("copy lib files...");
        Directory.CreateDirectory(releaseDir + "lib");
        Utils.CopyFiles(builder.LuminoLibDir + "Debug", "*.lib", releaseDir + "lib");
        Utils.CopyFiles(builder.LuminoLibDir + "Release", "*.lib", releaseDir + "lib");

        // インストールスクリプトとか、プロジェクトテンプレート
        Logger.WriteLine("copy other files...");
        Directory.CreateDirectory(releaseDir + "tools");
        Utils.CopyFile(builder.LuminoToolsDir + "VS2015ProjectTemplate/LuminoProject.zip", releaseDir + "tools");
        Utils.CopyFile(pkgSrcDir + "Lumino_Install.bat", releaseDir);
        Utils.CopyFile(pkgSrcDir + "Lumino_Uninstall.bat", releaseDir);

        // Readme.txt (バージョン名を埋め込む)
        string text = File.ReadAllText(pkgSrcDir + "Readme.txt");
        text = text.Replace("$(LuminoVersion)", builder.VersionString);
        File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));

        // .zip に圧縮する
        Logger.WriteLine("compressing files...");
        File.Delete(zipFilePath);
        ZipFile.CreateFromDirectory(releaseDir, zipFilePath, CompressionLevel.Optimal, true);
    }
}
