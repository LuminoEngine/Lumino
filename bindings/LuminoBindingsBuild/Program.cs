using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using LuminoBuildTool;

namespace LuminoBindingsBuild
{
	class MainClass
	{
		public static void Main (string[] args)
		{
            Assembly thisAssembly = Assembly.GetEntryAssembly();
            string exeDir = Path.GetDirectoryName(thisAssembly.Location);

			var builder = new LuminoBuildTool.Builder();
            builder.MSBuildPath = @"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe";
            builder.RootDir = Path.GetFullPath(Path.Combine(exeDir, "../../..")) + "/";	// .sln のあるフォルダ
            builder.LuminoLibDir = Path.GetFullPath(builder.RootDir + "../lib") + "/";

            builder.Rules = new List<LuminoBuildTool.ModuleRule>();
            builder.Rules.Add(new LuminoDotNetRule());
            builder.Rules.Add(new LuminoRubyRule());
            if (Utils.IsWin32) builder.Rules.Add(new LuminoHSPRule());

            builder.Build();
        }
	}
}
