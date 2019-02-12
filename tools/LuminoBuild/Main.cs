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
                args = new string[] { "MakeLocalPackage", "Windows" };
            }

            Assembly thisAssembly = Assembly.GetEntryAssembly();
            string exeDir = Path.GetDirectoryName(thisAssembly.Location);

            var builder = new LuminoBuild.Builder();
            builder.Args = args;

            builder.LuminoRootDir = Path.GetFullPath(Path.Combine(exeDir, "../../../../../../")) + "/";
            builder.LuminoBuildDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "build"));
            builder.LuminoBindingsDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "bindings"));
            builder.LuminoLibDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "lib"));
            builder.LuminoToolsDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "tools"));
            builder.LuminoDocDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "docs"));
            builder.LuminoSourceDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "src"));
            builder.LuminoPackageDir = Path.GetFullPath(Path.Combine(builder.LuminoBuildDir, "Package"));
            builder.LuminoPackageLibDir = Path.GetFullPath(Path.Combine(builder.LuminoPackageDir, "lib"));
            builder.LuminoPackageSourceDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "tools/PackageSource"));
            builder.LuminoExternalDir = Path.GetFullPath(Path.Combine(builder.LuminoRootDir, "external"));

            BuildEnvironment.BuildTarget = BuildTargetFlags.None;
            if (builder.HasFlagArgument("Windows")) BuildEnvironment.BuildTarget |= BuildTargetFlags.Windows;
            if (builder.HasFlagArgument("Android")) BuildEnvironment.BuildTarget |= BuildTargetFlags.Android;
            if (builder.HasFlagArgument("macOS")) BuildEnvironment.BuildTarget |= BuildTargetFlags.macOS;
            if (builder.HasFlagArgument("iOS")) BuildEnvironment.BuildTarget |= BuildTargetFlags.iOS;
            if (builder.HasFlagArgument("Web")) BuildEnvironment.BuildTarget |= BuildTargetFlags.Web;
            if (BuildEnvironment.BuildTarget == BuildTargetFlags.None) BuildEnvironment.BuildTarget = BuildTargetFlags.All;

            BuildEnvironment.Initialize(builder.LuminoRootDir);


            Console.WriteLine("RootDir: {0}", builder.LuminoRootDir);


            builder.Tasks = new List<LuminoBuild.BuildTask>();
            builder.Tasks.Add(new Tasks.SetupTools());
            builder.Tasks.Add(new Tasks.BuildExternalProjects());
            builder.Tasks.Add(new Tasks.BuildLLVM());
            builder.Tasks.Add(new Tasks.MakeVSProjects());
            builder.Tasks.Add(new Tasks.BuildEngine_Linux());
            builder.Tasks.Add(new Tasks.BuildEngine_macOS());
            builder.Tasks.Add(new Tasks.BuildEngine_iOS());
            builder.Tasks.Add(new Tasks.BuildDocuments());
            builder.Tasks.Add(new Tasks.BuildEmbeddedResources());
            builder.Tasks.Add(new Tasks.BuildEngine_MSVC());
            builder.Tasks.Add(new Tasks.CompressPackage());
            builder.Tasks.Add(new Tasks.CopyEngineLibsToRepoRoot());
            builder.Tasks.Add(new Tasks.MakeNuGetPackage_Core());
            builder.Tasks.Add(new Tasks.BuildEngine_AndroidJNI());
            builder.Tasks.Add(new Tasks.BuildEngine_Emscripten());
            builder.Tasks.Add(new Tasks.MakeReleasePackage());
            builder.Tasks.Add(new Tasks.MakeInstaller_Win32());
            builder.Rules.Add(new Rules.MakePackage());
            builder.Rules.Add(new Rules.MakeLocalPackage());
            builder.Rules.Add(new Rules.BuildForCI_1());
            builder.Rules.Add(new Rules.BuildForCI_2());
            builder.Rules.Add(new Rules.BuildForCI_3());
            
            
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
