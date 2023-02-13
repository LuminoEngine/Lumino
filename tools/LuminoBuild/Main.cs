using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Linq;
using CommandLine;

namespace LuminoBuild
{
    class CmdOptions
    {
        [Value(0, MetaName = "task", HelpText = "Name of task to execute.")]
        public string Task { get; set; } = "";

        [Value(1, MetaName = "triplet", HelpText = "Triplet.")]
        public string? Triplet { get; set; } = null;

        [Option("external-vcpkg-dir", HelpText = "Externak vcpkg directory.")]
        public string? ExternalVcpkgDir { get; set; } = null;

        [Option("debug", HelpText = "Enabled debug build.")]
        public bool Debug { get; set; } = false;

        [Option("components", HelpText = "; separated component list.")]
        public string Components { get; set; } = "";
    }

    class Program
    {
        // Usage: <Command> <Target> <Configuration>
        //
        // Command:         Task or Rule name
        // Target:          BuildEnvironment.Targets
        // Configuration:   Debug or Release or "" ("" は .sln など開発用で使いたいとき用)
        static int Main(string[] args)
        {
            // default
            if (args.Length == 0)
            {
                args = new string[] { "Bootstrap" };
                //args = new string[] { "BuildEngine" };

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
                //args = new string[] { "BuildEngine_Emscripten" };
                //args = new string[] { "BuildEngine_Android" };
                //args = new string[] { "MakePackage_Ruby" };
                //args = new string[] { "BuildLLVM" };
                //args = new string[] { "BuildEmbeddedResources" };

                //args = new string[] { "MakeNativePackage" };
                //args = new string[] { "MakeInstaller_Win32" };

                //args = new string[] { "BuildExternals", "wasm32-emscripten" };
                //args = new string[] { "BuildEngine_Emscripten", "wasm32-emscripten" };

                //args = new string[] { "BuildExternals", "x64-windows" };
                //args = new string[] { "BuildEngine", "wasm32-emscripten" };

                //args = new string[] { "MakePackage_Ruby" };
            }

            try
            {
                Parser.Default.ParseArguments<CmdOptions>(args).WithParsed(o =>
                {
                    Run(o, args);
                });

                return 0;
            }
            catch (Exception e)
            {
                Console.Error.WriteLine(e.ToString());
                return 1;
            }
        }

        static void Run(CmdOptions options, string[] args)
        {
            var triplet = (options.Triplet != null) ? options.Triplet : "x64-windows-static";
            var b = new Build(triplet, options);
            b.Args = args;
            Logger.Init(b);

            try
            {
                b.LuminoBindingsDir = Path.GetFullPath(Path.Combine(b.RootDir, "bindings"));
                b.LuminoLibDir = Path.GetFullPath(Path.Combine(b.RootDir, "lib"));
                b.LuminoToolsDir = Path.GetFullPath(Path.Combine(b.RootDir, "tools"));
                b.LuminoDocDir = Path.GetFullPath(Path.Combine(b.RootDir, "docs"));
                b.LuminoPackageDir = Path.GetFullPath(Path.Combine(b.BuildDir, "Package"));
                b.LuminoPackageLibDir = Path.GetFullPath(Path.Combine(b.LuminoPackageDir, "lib"));
                b.LuminoPackageSourceDir = Path.GetFullPath(Path.Combine(b.RootDir, "tools/PackageSource"));
                b.LuminoExternalDir = Path.GetFullPath(Path.Combine(b.RootDir, "external"));

                var positionalArgs = args.Where(x => !x.Contains("--")).ToList();

                BuildEnvironment.Target = (positionalArgs.Count > 1) ? args[1] : "";
                BuildEnvironment.Configuration = (positionalArgs.Count > 2) ? args[2] : "";
                BuildEnvironment.Initialize(b);


                if (b.System == "emscripten") EmscriptenEnv.Setup(b);


                var taskManager = new TaskManager();
                taskManager.Tasks.Add(new Tasks.SetupTools());
                taskManager.Tasks.Add(new Tasks.BuildExternalProjects());
                taskManager.Tasks.Add(new Tasks.BuildExternals());
                taskManager.Tasks.Add(new Tasks.BuildLLVM());
                taskManager.Tasks.Add(new Tasks.BuildEngine_Linux());
                taskManager.Tasks.Add(new Tasks.BuildEngine_macOS());
                taskManager.Tasks.Add(new Tasks.BuildEngine_iOS());
                taskManager.Tasks.Add(new Tasks.BuildDocuments());
                taskManager.Tasks.Add(new Tasks.BuildEmbeddedResources());
                taskManager.Tasks.Add(new Tasks.CompressPackage());
                taskManager.Tasks.Add(new Tasks.MakeNuGetPackage_Core());
                taskManager.Tasks.Add(new Tasks.BuildEngine_Android());
                taskManager.Tasks.Add(new Tasks.BuildEngine());
                taskManager.Tasks.Add(new Tasks.MakeNativePackage());
                taskManager.Tasks.Add(new Tasks.MakeInstaller_Win32());
                taskManager.Tasks.Add(new Tasks.MakePackage_HSP3());
                taskManager.Tasks.Add(new Tasks.MakePackage_macOS());
                taskManager.Tasks.Add(new Tasks.MakePackage_Ruby());
                taskManager.Tasks.Add(new Tasks.Bootstrap());
                //taskManager.Rules.Add(new Rules.BuildPackage());
                //taskManager.Rules.Add(new Rules.BuildLocalPackage());


                if (args.Length >= 1)
                {
                    taskManager.DoTask(b, options.Task);
                }
                else
                {
                    //while (true)
                    //{
                    //    Console.WriteLine("----------------------------------------");
                    //    Console.WriteLine("{0,-8}   {1}", "Command", "Description");
                    //    foreach (var rule in taskManager.Tasks)
                    //    {
                    //        Console.WriteLine("{0,-8}", rule.CommandName);
                    //    }
                    //    Console.WriteLine("all      : Build all.");
                    //    Console.WriteLine("exit     : Exit.");
                    //    Console.WriteLine("----------------------------------------");
                    //    Console.Write("Enter commands:");
                    //    string commands = Console.ReadLine();

                    //    if (commands == "exit") break;

                    //    //b.DoTaskOrRule(commands);
                    //}
                }
            }
            finally
            {
                Logger.Close();
            }
        }
    }
}
