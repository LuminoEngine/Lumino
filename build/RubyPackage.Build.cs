using System;
using System.IO;
using LuminoBuildTool;
using System.Text;
using System.IO.Compression;

class RubyPackageRule : ModuleRule
{
    /// <summary>
    /// ルールの名前
    /// </summary>
    public override string Name
    {
        get { return "Ruby"; }
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
        string rubyDir = builder.LuminoBindingsDir + "Ruby/";
        string rubyBuildDir = rubyDir + "build/";
        string releaseDir = builder.LuminoPackageReleaseDir + "LuminoRuby_" + builder.VersionString + "/";
        string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoRuby_" + builder.VersionString + ".zip";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/Ruby/";
        Directory.CreateDirectory(releaseDir);

        // lib
        Utils.CopyFile(rubyBuildDir + "Lumino.so", releaseDir);
        Utils.CopyFile(rubyBuildDir + "LuminoC.dll", releaseDir);

        // doc
        Directory.CreateDirectory(releaseDir + "doc");
        Utils.CopyDirectory(rubyBuildDir + "doc", releaseDir + "doc");

        // Readme.txt (バージョン名を埋め込む)
        string text = File.ReadAllText(pkgSrcDir + "Readme.txt");
        text = text.Replace("$(LuminoVersion)", builder.VersionString);
        File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));

        // sample
        Utils.CopyDirectory(rubyDir + "Samples", releaseDir + "sample");

        // .zip に圧縮する
        Logger.WriteLine("compressing files...");
        File.Delete(zipFilePath);
        ZipFile.CreateFromDirectory(releaseDir, zipFilePath, CompressionLevel.Optimal, true);
    }
}
