using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace LuminoBuild
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            Assembly thisAssembly = Assembly.GetEntryAssembly();
            string exeDir = Path.GetDirectoryName(thisAssembly.Location);
            
            var builder = new LuminoBuild.Builder();

            builder.VersionString = "0.3.0";
            builder.InstallerProductGUID_MSVC2013 = "174BEA6E-BE1D-46B1-B363-EAA03ABED741";
            builder.InstallerProductGUID_MSVC2015 = "499FDE66-FE07-4180-94E8-5A7BF690BEEF";
            builder.InstallerProductGUID_MSVC2017 = "6F45F9FB-9E6D-4751-A98D-9D0942639E7C";

            builder.LuminoRootDir = Path.GetFullPath(Path.Combine(exeDir, "../../../../")) + "/"; // .sln のあるフォルダ
            builder.LuminoBuildDir = builder.LuminoRootDir + "Build/";
            builder.LuminoBindingsDir = builder.LuminoRootDir + "Bindings/";
            builder.LuminoLibDir = builder.LuminoRootDir + "lib/";
            builder.LuminoToolsDir = builder.LuminoRootDir + "Tools/";
            builder.LuminoDocDir = builder.LuminoRootDir + "Documentation/";
            builder.LuminoPackageDir = builder.LuminoRootDir + "Package/";
            builder.LuminoPackageSourceDir = builder.LuminoRootDir + "Package/PackageSource/";
            builder.LuminoPackageReleaseDir = builder.LuminoRootDir + "Package/Release/Lumino/";
            builder.LuminoDependenciesDir = builder.LuminoRootDir + "External/LuminoDependencies/";

            builder.Rules = new List<LuminoBuild.ModuleRule>();
            builder.Rules.Add(new SetupDependencies());
            builder.Rules.Add(new MakeVSProjectsRule());
            builder.Rules.Add(new LuminoEngineRule());
            builder.Rules.Add(new CppPackageRule());
            builder.Rules.Add(new LuminoDotNetRule());
            builder.Rules.Add(new DotNetPackageRule());
            builder.Rules.Add(new LuminoRubyRule());
            builder.Rules.Add(new RubyPackageRule());
            builder.Rules.Add(new LuminoCRule());
            builder.Rules.Add(new CPackageRule());
            builder.Rules.Add(new MakeInstaller());
            builder.Rules.Add(new Rules.BuildEngine_MSVCUnicodeMD());
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

                try
                {
                    builder.Execute(commands);
                }
                catch (Exception e)
                {
                    Console.ForegroundColor = ConsoleColor.Red;
                    Console.WriteLine(e.ToString());
                    Console.ResetColor(); // 色のリセット
                }
            }
        }
    }
}
