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
            builder.LuminoDocDir = builder.LuminoRootDir + "doc/";
            builder.LuminoPackageDir = builder.LuminoRootDir + "package/";
            builder.LuminoPackageSourceDir = builder.LuminoRootDir + "package/PackageSource/";
            builder.LuminoPackageReleaseDir = builder.LuminoRootDir + "package/Release/";

            builder.Rules = new List<LuminoBuildTool.ModuleRule>();
            builder.Rules.Add(new LuminoEngineRule());
            builder.Rules.Add(new CppPackageRule());
            builder.Rules.Add(new LuminoDotNetRule());
            builder.Rules.Add(new DotNetPackageRule());
            builder.Rules.Add(new LuminoRubyRule());
            builder.Rules.Add(new RubyPackageRule());
            builder.Rules.Add(new LuminoCRule());
            builder.Rules.Add(new CPackageRule());
            if (Utils.IsWin32) builder.Rules.Add(new LuminoHSPRule());
            if (Utils.IsWin32) builder.Rules.Add(new HSPPackageRule());

            while (true)
            {
                Console.WriteLine("----------------------------------------");
                Console.WriteLine("{0,-8}   {1}", "Command", "Description");
                foreach (var rule in builder.Rules)
                {
                    Console.WriteLine("{0,-8} : {1}", rule.CommandName, rule.Description);
                }
                Console.WriteLine("all      : Build all.");
                Console.WriteLine("exit     : Exit.");
                Console.WriteLine("----------------------------------------");
                Console.Write("Enter commands:");
                string commands = Console.ReadLine();

                if (commands == "exit") break;

                builder.Execute(commands);
            }
        }
    }
}
