using System;
using System.IO;
using LuminoBuildTool;

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
        string releaseDir = builder.LuminoPackageReleaseDir + "Ruby_" + builder.VersionString + "/";
        string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/Ruby/";
        Directory.CreateDirectory(releaseDir);

        // lib
        Utils.CopyFile(rubyBuildDir + "Lumino.so", releaseDir);
        Utils.CopyFile(rubyBuildDir + "LuminoC.dll", releaseDir);

        // doc
        Directory.CreateDirectory(releaseDir + "doc");
        Utils.CopyDirectory(rubyBuildDir + "doc", releaseDir + "doc");
    }
}
