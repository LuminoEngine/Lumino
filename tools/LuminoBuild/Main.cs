using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

namespace LuminoBuild
{
    class Program
    {
        static void Main(string[] args)
        {
            // default
            if (args.Length == 0)
            {
                args = new string[] { "MakeNuGetPackage_Core" };
            }

            Assembly thisAssembly = Assembly.GetEntryAssembly();
            string exeDir = Path.GetDirectoryName(thisAssembly.Location);

            var builder = new LuminoBuild.Builder();

            builder.MajorVersion = 0;
            builder.MinorVersion = 4;
            builder.RevisionVersion = 0;
            builder.BuildVersion = 0;
            builder.VersionString = string.Format("{0}.{1}.{2}", builder.MajorVersion, builder.MinorVersion, builder.RevisionVersion);

            builder.LuminoRootDir = Path.GetFullPath(Path.Combine(exeDir, "../../../../../../")) + "/";
            builder.LuminoBuildDir = builder.LuminoRootDir + "build/";
            builder.LuminoBindingsDir = builder.LuminoRootDir + "bindings/";
            builder.LuminoLibDir = builder.LuminoRootDir + "lib/";
            builder.LuminoToolsDir = builder.LuminoRootDir + "tools/";
            builder.LuminoDocDir = builder.LuminoRootDir + "doc/";
            builder.LuminoPackageDir = builder.LuminoRootDir + "package/";
            builder.LuminoPackageSourceDir = builder.LuminoRootDir + "package/PackageSource/";
            builder.LuminoPackageReleaseDir = builder.LuminoRootDir + "package/Release/Lumino/";
            builder.LuminoDependenciesDir = builder.LuminoRootDir + "external/LuminoDependencies/";
            
            Console.WriteLine("RootDir: {0}", builder.LuminoRootDir);


            builder.Tasks = new List<LuminoBuild.BuildTask>();
            builder.Tasks.Add(new Tasks.MakeVSProjects());
            builder.Tasks.Add(new Tasks.BuildEngine());
            builder.Tasks.Add(new Tasks.MakeNuGetPackage_Core());
            builder.Rules.Add(new Rules.MakePackage());

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
