using System;
using System.IO;
using LuminoBuild;
using System.Text.RegularExpressions;

class LuminoDotNetRule : BuildTask
{
    private string _msbuild;
    private string _SHFBROOT;

    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "cs-b"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Build engine for C# (.Net)."; } }

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

        // Sandcastle
        // https://github.com/EWSoftware/SHFB/releases
        if (Utils.IsWin32)
        {
            // C:\Program Files (x86)\EWSoftware\Sandcastle Help File Builder\
            _SHFBROOT = Environment.GetEnvironmentVariable("SHFBROOT", System.EnvironmentVariableTarget.Machine);
            if (_SHFBROOT == null)
            {
                Logger.WriteLineError("Not installed Sandcastle Help File Builder.");
                return;
            }
            _SHFBROOT = Regex.Replace(_SHFBROOT, @"\\$", "");
        }

        Buildable = true;
    }

    /// <summary>
    /// ビルド実行
    /// </summary>
    public override void Build(Builder builder)
	{
        var dotnetDir = builder.LuminoBindingsDir + "DotNet/";

        // ライブラリをビルドする
        Logger.WriteLine("Building dll...");
        string sln = '"' + Path.GetFullPath(dotnetDir + "LuminoDotNet.sln") + '"';
        Utils.CallProcess(_msbuild, "/t:Build /p:Configuration=\"Release\" /p:Platform=\"x86\" /m " + sln);

        // ドキュメントをビルドする
        if (Utils.IsWin32)
        {
            Logger.WriteLine("Building documents...");
            string shfbproj = '"' + Path.GetFullPath(dotnetDir + "LuminoDotNet.shfbproj") + '"';
            Utils.CallProcess(_msbuild, "/p:Configuration=Release /property:SHFBROOT=\"" + _SHFBROOT + "\" " + shfbproj);
        }

        // テスト出力場所に dll をコピーする
        string testOutputDir = dotnetDir + "Test/bin/x86/Release/";
        if (Utils.IsWin32)
            File.Copy(builder.LuminoLibDir + "MSVC140/x86/Release/LuminoCU.dll", testOutputDir + "LuminoCU.dll", true);
        else
            File.Copy(builder.LuminoLibDir + "x86/Release/LuminoCU.so", testOutputDir + "LuminoCU.so", true);

        // テスト実行
        Logger.WriteLine("Running test...");
        Utils.CallProcess(testOutputDir + "Test.exe");
    }
}
