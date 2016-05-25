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


        Directory.CreateDirectory(rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.cpp", rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.h", rubyBuildDir);
        Utils.CopyFile(rubyDir + "extconf.rb", rubyBuildDir);
        Utils.CopyFile(builder.LuminoLibDir + "LuminoC_x86MT.dll", rubyBuildDir);

        Directory.SetCurrentDirectory(rubyBuildDir);
        Utils.CallProcess("ruby", "extconf.rb");
        Utils.CallProcess("make");

        // プロセスの環境変数を元に戻しておく
		Environment.SetEnvironmentVariable("PATH", envPath);
	}
}
