using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild
{
    class BuildEnvironment
    {
        public const string emsdkVer = "sdk-1.38.10-64bit";
        public const string emVer = "1.38.10";

        public static string BuildToolsDir { get; set; }

        public static string EmsdkDir { get; set; }
        public static string EmscriptenDir { get; set; }
        public static string emcmake { get; set; }


        public static string AndroidSdkRootDir { get; set; }
        public static string AndroidSdkCMake { get; set; }
        public static string AndroidSdkNinja { get; set; }
        public static string AndroidNdkRootDir { get; set; }
        public static string AndroidCMakeToolchain { get; set; }

        public static void Initialize()
        {
            string appDir = Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
            BuildToolsDir = Path.Combine(appDir, "Lumino", "BuildTools");

            EmsdkDir = Path.Combine(BuildToolsDir, "emsdk");
            EmscriptenDir = Path.Combine(EmsdkDir, "emscripten", emVer);
            emcmake = Path.Combine(EmscriptenDir, Utils.IsWin32 ? "emcmake.bat" : "emcmake");

            string localAppDir = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            AndroidSdkRootDir = Path.Combine(localAppDir, @"Android\Sdk");

            AndroidSdkCMake = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\cmake.exe");
            AndroidSdkNinja = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\ninja.exe");

            AndroidNdkRootDir = Path.Combine(AndroidSdkRootDir, "ndk-bundle");
            AndroidCMakeToolchain = Path.Combine(AndroidNdkRootDir, @"build\cmake\android.toolchain.cmake");


            InstallTools();
        }

        private static void InstallTools()
        {
            Directory.CreateDirectory(BuildToolsDir);
            
            if (!Directory.Exists(EmsdkDir))
            {
                Directory.SetCurrentDirectory(BuildToolsDir);
                Utils.CallProcessShell("git", "clone https://github.com/juj/emsdk.git");
            }
            if (!Directory.Exists(EmscriptenDir))
            {
                Directory.SetCurrentDirectory(Path.GetFullPath(EmsdkDir));
                Utils.CallProcessShell("emsdk", "install " + emsdkVer);
            }
        }
    }
}
