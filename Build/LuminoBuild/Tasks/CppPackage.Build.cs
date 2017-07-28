using System;
using System.Text;
using System.IO;
using LuminoBuild;

class CppPackageRule : BuildTask
{
    [Flags]
    enum VSBuildFlags
    {
        None = 0x0000,
        VS2013 = 0x0001,
        VS2015 = 0x0002,
        VS2017 = 0x0004,
    }

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
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoCpp.zip";

        VSBuildFlags vsTarget = VSBuildFlags.None;
        if (Directory.Exists(builder.LuminoLibDir + "MSVC120")) vsTarget |= VSBuildFlags.VS2013;
        if (Directory.Exists(builder.LuminoLibDir + "MSVC140")) vsTarget |= VSBuildFlags.VS2015;
        if (Directory.Exists(builder.LuminoLibDir + "MSVC150")) vsTarget |= VSBuildFlags.VS2017;

        if (vsTarget.HasFlag(VSBuildFlags.VS2013))
        {
            string releaseDir = builder.LuminoPackageReleaseDir + "MSVC2013/";
            Directory.CreateDirectory(releaseDir);
            CopyCommonFiles(builder, releaseDir, builder.LuminoLibDir + "MSVC120");

            Logger.WriteLine("copy other files...");
            Directory.CreateDirectory(releaseDir + "Tools/VS2013ProjectTemplate");
            Utils.CreateZipFile(builder.LuminoToolsDir + "VS2013ProjectTemplate/LuminoProjectCpp", releaseDir + "Tools/VS2013ProjectTemplate/LuminoProjectCpp.zip", false);
        }

        if (vsTarget.HasFlag(VSBuildFlags.VS2015))
        {
            string releaseDir = builder.LuminoPackageReleaseDir + "MSVC2015/";
            Directory.CreateDirectory(releaseDir);
            CopyCommonFiles(builder, releaseDir, builder.LuminoLibDir + "MSVC140");

            Logger.WriteLine("copy other files...");
            Directory.CreateDirectory(releaseDir + "Tools/VS2015ProjectTemplate");
            Utils.CreateZipFile(builder.LuminoToolsDir + "VS2015ProjectTemplate/LuminoProjectCpp", releaseDir + "Tools/VS2015ProjectTemplate/LuminoProjectCpp.zip", false);
        }

        if (vsTarget.HasFlag(VSBuildFlags.VS2017))
        {
            string releaseDir = builder.LuminoPackageReleaseDir + "MSVC2017/";
            Directory.CreateDirectory(releaseDir);
            CopyCommonFiles(builder, releaseDir, builder.LuminoLibDir + "MSVC150");

            Logger.WriteLine("copy other files...");
            Directory.CreateDirectory(releaseDir + "Tools/VS2017ProjectTemplate");
            Utils.CreateZipFile(builder.LuminoToolsDir + "VS2017ProjectTemplate/LuminoProjectCpp", releaseDir + "Tools/VS2017ProjectTemplate/LuminoProjectCpp.zip", false);
            // ↑ Kaspersky で、このタイミングでのzip圧縮でウィルス扱いされることがあった。
            // clone 直後など、クリーンな状態では発生しない。Release フォルダに上書きコピー → zpi 化がキーだろうか？
            // Release フォルダを削除しておくとセーフ。
        }

        // .zip に圧縮する
        //Logger.WriteLine("compressing files...");
        //Utils.CreateZipFile(releaseDir, zipFilePath);
    }

    void CopyCommonFiles(Builder builder, string releaseDir, string libTargetDir)
    {
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/";

        // include
        Logger.WriteLine("copy include files...");
        string releaseIncludeDir = Path.Combine(releaseDir, "Include");
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Source/LuminoCore/Include"), releaseIncludeDir, true);
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Source/LuminoEngine/Include"), releaseIncludeDir, true);

        // .lib
        Logger.WriteLine("copy lib files...");
        Utils.CopyDirectory(libTargetDir, releaseDir + "Lib", true, "*.lib");
        Utils.CopyDirectory(libTargetDir, releaseDir + "Lib", true, "*.dll");

        // Assets
        Logger.WriteLine("copy Assets files...");
        Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Samples/Assets"), releaseDir + "Assets", true);

        // Readme.txt (バージョン名を埋め込む)
        string text = File.ReadAllText(pkgSrcDir + "Readme.template.txt");
        text = text.Replace("$(LuminoVersion)", builder.VersionString);
        File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));

        // ReleaseNote
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", releaseDir);
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", builder.LuminoPackageReleaseDir);
    }
}
