using System;
using System.IO;
using LuminoBuildTool;

class LuminoHSPRule : ModuleRule
{
	public override void Build(Builder builder)
	{
		var hspDir = builder.RootDir + "HSP/";

        // ビルドする
        Utils.CallProcess(builder.MSBuildPath, hspDir + "LuminoHSP/LuminoHSP.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"Win32\" /m");
	}
}
