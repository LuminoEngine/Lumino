using System;
using System.IO;
using LuminoBuildTool;

class LuminoRubyRule : ModuleRule
{
	public override void Build(Builder builder)
	{
		var rubyDir = builder.RootDir + "Ruby/";
        var rubyDevKitDir = builder.RootDir + "../../DevKit-mingw64-32-4.7.2-20130224-1151-sfx/";
        var rubyBuildDir = rubyDir + "build/";

        // フルパスにしないと PATH を切れない
        var rubyBin = Path.GetFullPath(rubyDevKitDir + "bin");
        var mingwBin = Path.GetFullPath(rubyDevKitDir + "mingw/bin");

		var envPath = Environment.GetEnvironmentVariable("PATH");
        Environment.SetEnvironmentVariable("PATH", rubyBin + ";" + mingwBin + ";" + envPath);

        // build フォルダへ必要なファイルをコピーする
        Directory.CreateDirectory(rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.cpp", rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.h", rubyBuildDir);
        Utils.CopyFile(rubyDir + "extconf.rb", rubyBuildDir);
        Utils.CopyFile(rubyDir + "LuminoRubyDoc.rb", rubyBuildDir);     // ドキュメント用のソースも。yardoc は readme.md を自動的に取り込んでしまう
        Utils.CopyFile(builder.LuminoLibDir + "Release/LuminoC_x86_MT.dll", rubyBuildDir);

        // ビルド
        Directory.SetCurrentDirectory(rubyBuildDir);
        Utils.CallProcess("ruby", "extconf.rb");
        Utils.CallProcess("make");

        // ドキュメントの作成
        Utils.CallProcessShell("yardoc", "-o doc LuminoRubyDoc.rb");

        // プロセスの環境変数を元に戻しておく
        Environment.SetEnvironmentVariable("PATH", envPath);
	}
}
