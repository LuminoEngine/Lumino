using System;
using System.IO;
using LuminoBuildTool;

class LuminoRubyRule : ModuleRule
{
    public override string Name
    {
        get { return "Ruby"; }
    }

    public override void Build(Builder builder)
	{
		var rubyDir = builder.RootDir + "Ruby/";
        var rubyDevKitDir = builder.RootDir + "../../DevKit-mingw64-32-4.7.2-20130224-1151-sfx/";
        var rubyBuildDir = rubyDir + "build/";

        // フルパスにしないと PATH を切れない
        var rubyBin = Path.GetFullPath(rubyDevKitDir + "bin");
        var mingwBin = Path.GetFullPath(rubyDevKitDir + "mingw/bin");

		var oldEnv = Environment.GetEnvironmentVariable("PATH");
        Environment.SetEnvironmentVariable("PATH", rubyBin + ";" + mingwBin + ";" + oldEnv);

        // build フォルダへ必要なファイルをコピーする
        Directory.CreateDirectory(rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.cpp", rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.h", rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.rb", rubyBuildDir);   // ドキュメント用のソースも。yardoc は readme.md を自動的に取り込んでしまう
        Utils.CopyFile(builder.LuminoLibDir + "Release/LuminoC_x86MT.dll", rubyBuildDir);

        // ビルド
        Logger.WriteLine("Building ext lib...");
        var oldDir = Directory.GetCurrentDirectory();
        Directory.SetCurrentDirectory(rubyBuildDir);
        Utils.CallProcess("ruby", "extconf.rb");
        Utils.CallProcess("make");

        // ドキュメントの作成
        Logger.WriteLine("Building documents...");
        Utils.CallProcessShell("yardoc", "-o doc LuminoRubyDoc.rb");

        // テスト実行
        Logger.WriteLine("Running test...");
        Utils.CallProcess("ruby", "Test.rb");

        // 変更したものを元に戻す
        Directory.SetCurrentDirectory(oldDir);
        Environment.SetEnvironmentVariable("PATH", oldEnv);
    }
}
