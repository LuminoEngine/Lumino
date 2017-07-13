using System;
using System.IO;
using LuminoBuild;
using System.Text;

class RubyPackageRule : BuildTask
{
    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "ruby-p"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Make package for Ruby."; } }

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

        // ReleaseNote
        Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", releaseDir);

        // sample
        Utils.CopyDirectory(rubyDir + "sample", releaseDir + "sample");

        // .zip に圧縮する
        Logger.WriteLine("compressing files...");
        Utils.CreateZipFile(releaseDir, zipFilePath);
    }
}
