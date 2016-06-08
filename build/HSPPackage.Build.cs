using System;
using System.IO;
using LuminoBuildTool;
using System.Text;
using System.IO.Compression;

class HSPPackageRule : ModuleRule
{
    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "hsp-p"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Make package for HSP."; } }

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
        string hspDir = builder.LuminoBindingsDir + "HSP/";
        string releaseDir = builder.LuminoPackageReleaseDir + "LuminoHSP_" + builder.VersionString + "/";
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoHSP_" + builder.VersionString + ".zip";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/HSP/";
        Directory.CreateDirectory(releaseDir);

        // dll
        Utils.CopyFile(hspDir + "LuminoHSP/Release/LuminoHSP.dll", releaseDir);

        // header, help
        Utils.CopyFile(hspDir + "lumino.as", releaseDir);
        Utils.CopyFile(hspDir + "lumino.hs", releaseDir);
        Utils.CopyFile(hspDir + "CommandList.txt", releaseDir);

        // sample
        Utils.CopyDirectory(hspDir + "Samples", releaseDir + "sample");

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
