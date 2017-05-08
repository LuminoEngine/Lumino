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
        string releaseDir = builder.LuminoPackageReleaseDir + "LuminoCpp/";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/Cpp/";
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoCpp.zip";
        bool vs2013 = Directory.Exists(builder.LuminoLibDir + "MSVC120");
        
        Directory.CreateDirectory(releaseDir);

        // include には Engine, Core, Math のヘッダを全てコピーする
        Logger.WriteLine("copy include files...");
        string releaseIncludeDir = Path.Combine(releaseDir, "Include");
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Source/LuminoCore/Include"), releaseIncludeDir, true);
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Source/LuminoEngine/Include"), releaseIncludeDir, true);

        // .lib
        //Logger.WriteLine("copy dependencies lib files...");
        //foreach (string dir in Directory.GetDirectories(builder.LuminoRootDir + "External/LuminoDependencies"))
        //{
        //    // 今のところ LuminoDependencies/ライブラリ名/lib でフォルダ構成は統一されている。
        //    string libDir = dir + "/lib";
        //    if (Directory.Exists(libDir))
        //    {
        //        Utils.CopyDirectory(libDir, releaseDir + "Lib");
        //    }
        //}

        // .lib
        Logger.WriteLine("copy lib files...");
        Utils.CopyDirectory(builder.LuminoLibDir, releaseDir + "Lib", true, "*.lib");
        Utils.CopyDirectory(builder.LuminoLibDir, releaseDir + "Lib", true, "*.dll");

        // インストールスクリプトとか、プロジェクトテンプレート
        Logger.WriteLine("copy other files...");
        if (vs2013)
        {
            Directory.CreateDirectory(releaseDir + "Tools/VS2013ProjectTemplate");
        }
        Directory.CreateDirectory(releaseDir + "Tools/VS2015ProjectTemplate");
        Utils.CopyFile(pkgSrcDir + "Lumino_Install.bat", releaseDir);
        Utils.CopyFile(pkgSrcDir + "Lumino_Uninstall.bat", releaseDir);
        if (vs2013)
        {
            Utils.CreateZipFile(builder.LuminoToolsDir + "VS2013ProjectTemplate/LuminoProjectCpp", releaseDir + "Tools/VS2013ProjectTemplate/LuminoProjectCpp.zip", false);
        }
        Utils.CreateZipFile(builder.LuminoToolsDir + "VS2015ProjectTemplate/LuminoProjectCpp", releaseDir + "Tools/VS2015ProjectTemplate/LuminoProjectCpp.zip", false);

        // Readme.txt (バージョン名を埋め込む)
        string text = File.ReadAllText(pkgSrcDir + "Readme.template.txt");
        text = text.Replace("$(LuminoVersion)", builder.VersionString);
        File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));
        
        // ReleaseNote
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", releaseDir);
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", builder.LuminoPackageReleaseDir);

        // .zip に圧縮する
        //Logger.WriteLine("compressing files...");
        //Utils.CreateZipFile(releaseDir, zipFilePath);
    }
}
