using System;
using System.IO;
using LuminoBuildTool;

class LuminoHSPRule : ModuleRule
{
	public override void Build(Builder builder)
	{
		var hspDir = builder.RootDir + "HSP/";

        Utils.CallProcess(builder.MSBuildPath, hspDir + "LuminoHSP/LuminoHSP.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"Win32\" /m");


  //      // フルパスにしないと PATH を切れない
  //      var rubyBin = Path.GetFullPath(rubyDevKitDir + "bin");
  //      var mingwBin = Path.GetFullPath(rubyDevKitDir + "mingw/bin");

		//var envPath = Environment.GetEnvironmentVariable("PATH");
  //      Environment.SetEnvironmentVariable("PATH", rubyBin + ";" + mingwBin + ";" + envPath);


  //      Directory.CreateDirectory(rubyBuildDir);
  //      Utils.CopyFiles(rubyDir, "*.cpp", rubyBuildDir);
  //      Utils.CopyFiles(rubyDir, "*.h", rubyBuildDir);
  //      Utils.CopyFile(rubyDir + "extconf.rb", rubyBuildDir);
  //      Utils.CopyFile(builder.LuminoLibDir + "LuminoC_x86MT.dll", rubyBuildDir);

  //      Directory.SetCurrentDirectory(rubyBuildDir);
  //      Utils.CallProcess("ruby", "extconf.rb");
  //      Utils.CallProcess("make");

  //      // プロセスの環境変数を元に戻しておく
		//Environment.SetEnvironmentVariable("PATH", envPath);
	}
}
