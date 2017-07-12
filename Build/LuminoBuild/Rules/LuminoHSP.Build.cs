using System;
using System.IO;
using LuminoBuild;
using System.Text.RegularExpressions;

class LuminoHSPRule : ModuleRule
{
    private string _msbuild;

    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "hsp-b"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Build engine for HSP."; } }

    /// <summary>
    /// 前提条件の確認
    /// </summary>
    public override void CheckPrerequisite(Builder builder)
    {
        // MSBuild
        _msbuild = Utils.FindMSBuild();
        if (_msbuild == null)
        {
            Logger.WriteLineError("Not found MSBuild or xbuild.");
            return;
        }

        Buildable = true;
    }

    /// <summary>
    /// ビルド実行
    /// </summary>
    public override void Build(Builder builder)
    {
        var hspDir = builder.LuminoBindingsDir + "HSP/";

        Utils.CallProcess(_msbuild, hspDir + "LuminoHSP/LuminoHSP.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"Win32\" /m");

        // sample (Media)
        Utils.CopyDirectory(builder.LuminoPackageSourceDir + "Common/Media", hspDir + "Samples/Media");
    }
}
