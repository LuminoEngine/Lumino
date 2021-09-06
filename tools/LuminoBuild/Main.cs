using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Linq;

namespace LuminoBuild
{
    class Program
    {
        // Usage: <Command> <Target> <Configuration>
        //
        // Command:         Task or Rule name
        // Target:          BuildEnvironment.Targets
        // Configuration:   Debug or Release or "" ("" は .sln など開発用で使いたいとき用)
        static void Main(string[] args)
        {
            // default
            if (args.Length == 0)
            {
                //args = new string[] { "BuildLocalPackage", "MSVC2019-x64-MT" };
                //args = new string[] { "BuildLocalPackage", "MSVC2019-x86-MT" };
                //args = new string[] { "BuildLocalPackage", "Android-x86_64" };
                //args = new string[] { "BuildLocalPackage", "Emscripten" };


                //args = new string[] { "BuildEngine_Android" };
                //args = new string[] { "BuildExternalProjects", "MSVC2019-x64-MT" };//, "--enable-Effekseer" };
                //args = new string[] { "BuildExternalProjects", "Emscripten" };
                //args = new string[] { "BuildExternalProjects", "Android-x86_64" };
                //args = new string[] { "BuildExternalProjects", "MSVC2019-x86-MT" };
                //args = new string[] { "BuildExternalProjects", "MSVC2019-x64-MD" };
                //args = new string[] { "BuildEngine_MSVC", "MSVC2019-x64-MT" };
                //args = new string[] { "BuildEngine_MSVC", "MSVC2019-x86-MT" };
                //args = new string[] { "BuildEngine_Emscripten" };
                //args = new string[] { "BuildEngine_Android" };
                //args = new string[] { "MakePackage_Ruby" };
                //args = new string[] { "BuildLLVM" };
                //args = new string[] { "BuildEmbeddedResources" };

                //args = new string[] { "MakeNativePackage" };
                //args = new string[] { "MakeInstaller_Win32" };

                //args = new string[] { "BuildExternals" };
                args = new string[] { "BuildEngine_MSVC" };

            }

            var triplet = /*(args.Length >= 2) ? args[1] :*/ "x64-windows";
            var builder = new Build(triplet);
            builder.Args = args;

            //builder.LuminoBuildCacheDir = Path.GetFullPath(Path.Combine(builder.LuminoBuildDir, "BuildCache"));
            builder.LuminoBindingsDir = Path.GetFullPath(Path.Combine(builder.RootDir, "bindings"));
            builder.LuminoLibDir = Path.GetFullPath(Path.Combine(builder.RootDir, "lib"));
            builder.LuminoToolsDir = Path.GetFullPath(Path.Combine(builder.RootDir, "tools"));
            builder.LuminoDocDir = Path.GetFullPath(Path.Combine(builder.RootDir, "docs"));
            builder.LuminoSourceDir = Path.GetFullPath(Path.Combine(builder.RootDir, "src"));
            builder.LuminoPackageDir = Path.GetFullPath(Path.Combine(builder.BuildDir, "Package"));
            builder.LuminoPackageLibDir = Path.GetFullPath(Path.Combine(builder.LuminoPackageDir, "lib"));
            builder.LuminoPackageSourceDir = Path.GetFullPath(Path.Combine(builder.RootDir, "tools/PackageSource"));
            builder.LuminoExternalDir = Path.GetFullPath(Path.Combine(builder.RootDir, "external"));

            var positionalArgs = args.Where(x => !x.Contains("--")).ToList();

            BuildEnvironment.Target = (positionalArgs.Count > 1) ? args[1] : "";
            BuildEnvironment.Configuration = (positionalArgs.Count > 2) ? args[2] : "";
            BuildEnvironment.Initialize(builder);


            builder.Tasks = new List<LuminoBuild.BuildTask>();
            builder.Tasks.Add(new Tasks.SetupTools());
            builder.Tasks.Add(new Tasks.BuildExternalProjects());
            builder.Tasks.Add(new Tasks.BuildExternals());
            builder.Tasks.Add(new Tasks.BuildLLVM());
            builder.Tasks.Add(new Tasks.BuildEngine_Linux());
            builder.Tasks.Add(new Tasks.BuildEngine_macOS());
            builder.Tasks.Add(new Tasks.BuildEngine_iOS());
            builder.Tasks.Add(new Tasks.BuildDocuments());
            builder.Tasks.Add(new Tasks.BuildEmbeddedResources());
            builder.Tasks.Add(new Tasks.BuildEngine_MSVC());
            builder.Tasks.Add(new Tasks.CompressPackage());
            builder.Tasks.Add(new Tasks.MakeNuGetPackage_Core());
            builder.Tasks.Add(new Tasks.BuildEngine_Android());
            builder.Tasks.Add(new Tasks.BuildEngine_Emscripten());
            builder.Tasks.Add(new Tasks.MakeNativePackage());
            builder.Tasks.Add(new Tasks.MakeInstaller_Win32());
            builder.Tasks.Add(new Tasks.MakePackage_HSP3());
            builder.Tasks.Add(new Tasks.MakePackage_macOS());
            builder.Tasks.Add(new Tasks.MakePackage_Ruby());
            builder.Rules.Add(new Rules.BuildPackage());
            builder.Rules.Add(new Rules.BuildLocalPackage());
            
            
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
                        Console.WriteLine("{0,-8}", rule.CommandName);
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
