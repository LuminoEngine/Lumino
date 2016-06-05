using System;
using System.IO;
using LuminoBuildTool;
using System.Text.RegularExpressions;

class LuminoHSPRule : ModuleRule
{
    private string _msbuild;
    private string _SHFBROOT;

    /// <summary>
    /// ルールの名前
    /// </summary>
    public override string Name
    {
        get { return "hspb"; }
    }

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
    }
}
