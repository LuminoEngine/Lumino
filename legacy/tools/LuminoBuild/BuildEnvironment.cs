using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild
{
    class TargetInfo
    {
        public string Name { get; set; }
        public string LibraryExt { get; set; }
        public bool PdbCopy { get; set; } = false;

        public bool Package = false;
    }

    class BuildEnvironment
    {
        public const string VSWhereUrl = @"https://github.com/Microsoft/vswhere/releases/download/2.5.2/vswhere.exe";

        public const string EngineBuildDirName = "EngineBuild";
        public const string EngineInstallDirName = "EngineInstall";

        public static bool FromCI { get; private set; }

        // Build settings
        public static string Target { get; set; }
        public static string Configuration { get; set; }
        public static string TargetFullName { get { return (string.IsNullOrEmpty(Configuration)) ? Target : $"{Target}-{BuildEnvironment.Configuration}"; } }

        // Build setting utils.
        public static bool IsMSVCTarget { get { return Target.Contains("MSVC"); } }
        public static bool IsWebTarget { get { return Target == "Emscripten"; } }
        public static bool IsAndroidTarget { get { return Target.Contains("Android"); } }
        public static bool IsMacOSTarget { get { return Target.Contains("macOS"); } }
        public static bool IsIOSTarget { get { return Target.Contains("iOS"); } }

        public static string FbxSdkVS2017 { get; private set; }

        public static TargetInfo[] Targets = new TargetInfo[]
        {
            new TargetInfo(){ Name = "x64-windows-static", LibraryExt = "*.lib", PdbCopy = true, Package = true },
            //new TargetInfo(){ Name = "Emscripten", LibraryExt = "*.a" },
            //new TargetInfo(){ Name = "Android-arm64-v8a", LibraryExt = "*.a" },
            //new TargetInfo(){ Name = "Android-armeabi-v7a", LibraryExt = "*.a" },
            //new TargetInfo(){ Name = "Android-x86", LibraryExt = "*.a"},
            //new TargetInfo(){ Name = "Android-x86_64", LibraryExt = "*.a" },
            //new TargetInfo(){ Name = "macOS", LibraryExt = "*.a", Package = true },
            //new TargetInfo(){ Name = "iOS-SIMULATOR64", LibraryExt = "*.a" },
            //new TargetInfo(){ Name = "iOS-OS", LibraryExt = "*.a" },
        };

        public static void Initialize(Build builder)
        {
            FromCI = Environment.GetEnvironmentVariable("LN_BUILD_FROM_CI") != null;

            InstallTools(builder);
            FindFbxSdk();

            Console.WriteLine("BuildEnv initialization succeeded");
            Console.WriteLine("  FromCI: {0}", FromCI);
            Console.WriteLine("  RootDir: {0}", builder.RootDir);
        }

        private static void InstallTools(Build builder)
        {
            AndoridBuildEnv.Initialize(builder);
        }

        private static void FindFbxSdk()
        {
            var candidates = new string[]
            {
               @"C:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1",
               @"D:\Program Files\Autodesk\FBX\FBX SDK\2020.0.1",
            };

            FbxSdkVS2017 = candidates.FirstOrDefault(path => File.Exists(path));
        }
    }
}
