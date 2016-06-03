using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using LuminoBuildTool;

namespace LuminoBuild
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            Assembly thisAssembly = Assembly.GetEntryAssembly();
            string exeDir = Path.GetDirectoryName(thisAssembly.Location);

            var builder = new LuminoBuildTool.Builder();
            builder.VersionString = "0.2.0a1";
            builder.LuminoRootDir = Path.GetFullPath(Path.Combine(exeDir, "../../../..")) + "/"; // .sln のあるフォルダ
            builder.LuminoBuildDir = builder.LuminoRootDir + "build/";
            builder.LuminoBindingsDir = builder.LuminoRootDir + "bindings/";
            builder.LuminoLibDir = builder.LuminoRootDir + "lib/";
            builder.LuminoToolsDir = builder.LuminoRootDir + "tools/";
            builder.LuminoPackageDir = builder.LuminoRootDir + "package/";
            builder.LuminoPackageReleaseDir = builder.LuminoRootDir + "package/Release/";

            builder.Rules = new List<LuminoBuildTool.ModuleRule>();
            //builder.Rules.Add(new LuminoEngineRule());
            //builder.Rules.Add(new CppPackageRule());
            //builder.Rules.Add(new LuminoRubyRule());
            //builder.Rules.Add(new LuminoDotNetRule());
            builder.Rules.Add(new DotNetPackageRule());
            //if (Utils.IsWin32) builder.Rules.Add(new LuminoHSPRule());

            builder.CheckPrerequisite();
            builder.Build();
        }
    }
}
