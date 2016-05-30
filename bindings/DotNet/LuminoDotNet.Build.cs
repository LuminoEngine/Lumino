using System;
using System.IO;
using LuminoBuildTool;

class LuminoDotNetRule : ModuleRule
{
    public override string Name
    {
        get { return "DotNet"; }
    }

    public override void Build(Builder builder)
	{
		var dotnetDir = builder.RootDir + "DotNet/";
        var sandcastleDir = "D:/Program Files (x86)/EWSoftware/Sandcastle Help File Builder";

        // ライブラリをビルドする
        Logger.WriteLine("Building dll...");
        string sln = '"' + Path.GetFullPath(dotnetDir + "LuminoDotNet.sln") + '"';
        Utils.CallProcess(builder.MSBuildPath, "/t:Build /p:Configuration=\"Release\" /p:Platform=\"x86\" /m " + sln);

        // ドキュメントをビルドする
        if (Utils.IsWin32)
        {
            Logger.WriteLine("Building documents...");
            string shfbproj = '"' + dotnetDir + "LuminoDotNet.shfbproj" + '"';
            Utils.CallProcess(builder.MSBuildPath, "/p:Configuration=Release /property:SHFBROOT=\"" + sandcastleDir + "\" " + shfbproj);
        }

        // テスト出力場所に dll をコピーする
        string testOutputDir = dotnetDir + "Test/bin/x86/Release/";
        Utils.CopyFile(builder.LuminoLibDir + "Debug/LuminoC_x86uMTd.dll", testOutputDir);
        Utils.CopyFile(builder.LuminoLibDir + "Release/LuminoC_x86uMT.dll", testOutputDir);

        // テスト実行
        Logger.WriteLine("Running test...");
        Utils.CallProcess(testOutputDir + "Test.exe");
    }
}
