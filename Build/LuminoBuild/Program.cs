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
#if true
            args = new string[] { "MakeProjects" };
#else
            // default
            if (args.Length == 0)
            {
                args = new string[] { "MakeInstaller" };
            }
#endif

            Assembly thisAssembly = Assembly.GetEntryAssembly();
            string exeDir = Path.GetDirectoryName(thisAssembly.Location);
            
            var builder = new LuminoBuild.Builder();
            
            builder.MajorVersion = 0;
            builder.MinorVersion = 3;
            builder.RevisionVersion = 0;
            builder.BuildVersion = 0;
            builder.VersionString = string.Format("{0}.{1}.{2}", builder.MajorVersion, builder.MinorVersion, builder.RevisionVersion);
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

            builder.Tasks = new List<LuminoBuild.BuildTask>();
            builder.Tasks.Add(new SetupDependencies());
            builder.Tasks.Add(new Tasks.MakeVersionHeader());
            builder.Tasks.Add(new MakeVSProjectsRule());
            builder.Tasks.Add(new LuminoEngineRule());
            builder.Tasks.Add(new CppPackageRule());
            builder.Tasks.Add(new LuminoDotNetRule());
            builder.Tasks.Add(new DotNetPackageRule());
            builder.Tasks.Add(new LuminoRubyRule());
            builder.Tasks.Add(new RubyPackageRule());
            builder.Tasks.Add(new LuminoCRule());
            builder.Tasks.Add(new CPackageRule());
            builder.Tasks.Add(new MakeInstaller());
            builder.Tasks.Add(new Tasks.BuildEngine_MSVCUnicodeMD());
            if (Utils.IsWin32) builder.Tasks.Add(new LuminoHSPRule());
            if (Utils.IsWin32) builder.Tasks.Add(new HSPPackageRule());

            builder.Rules.Add(new Rules.MakeProjects());
            builder.Rules.Add(new Rules.MakeInstaller());

            if (args.Length >= 1)
            {
                builder.DoTaskOrRule(args[0]);
            }
            else
            {
                while (true)
                {
                    Console.WriteLine("----------------------------------------");
                    Console.WriteLine("{0,-8}   {1}", "Command", "Description");
                    foreach (var rule in builder.Tasks)
                    {
                        Console.WriteLine("{0,-8} : {1}", rule.CommandName, rule.Description);
                    }
                    Console.WriteLine("all      : Build all.");
                    Console.WriteLine("exit     : Exit.");
                    Console.WriteLine("----------------------------------------");
                    Console.Write("Enter commands:");
                    string commands = Console.ReadLine();

                    if (commands == "exit") break;

                    builder.DoTaskOrRule(commands);
                }
            }
        }

        
    }
}
