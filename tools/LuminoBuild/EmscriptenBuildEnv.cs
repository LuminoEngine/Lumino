using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild
{

    public class EmscriptenBuildEnv
    {
        public const string emsdkVer = "1.39.7";
        public const string emVer = "1.39.7";

        public static bool EmscriptenFound { get; set; }
        public static string EmsdkDir { get; set; }

        /// <summary>
        /// <Repo>/build/BuildTools/emsdk/upstream/emscripten
        /// (from home/.emscripten)
        /// </summary>
        public static string EmscriptenRoot { get; set; }

        /// <summary>
        /// <Repo>/build/BuildTools/emsdk/upstream/emscripten/system/local
        /// </summary>
        public static string EmscriptenSysRootLocal { get; set; }

        public static void Initialize(string repoRootDir)
        {
            var targetBuildDir = Path.Combine(repoRootDir, "build", "Emscripten");
            EmsdkDir = Path.Combine(targetBuildDir, "emsdk");
            EmscriptenRoot = Path.Combine(EmsdkDir, "upstream", "emscripten");
            EmscriptenSysRootLocal = Path.Combine(EmscriptenRoot, "system", "local");

            Directory.CreateDirectory(targetBuildDir);

            Console.WriteLine($"IsWebTarget: {BuildEnvironment.IsWebTarget}");
            Console.WriteLine($"IsWin32: {Utils.IsWin32}");

            // Install emsdk
            if (BuildEnvironment.IsWebTarget && Utils.IsWin32)
            {
                if (!Directory.Exists(EmsdkDir))
                {
                    using (var cd = CurrentDir.Enter(targetBuildDir))
                    {
                        Utils.CallProcess("git", "clone https://github.com/juj/emsdk.git");

                        //if (!Directory.Exists(EmscriptenDir))
                        {
                            Directory.SetCurrentDirectory(Path.GetFullPath(EmsdkDir));

                            if (Utils.IsWin32)
                                Utils.CallProcess("emsdk.bat", "install " + emsdkVer);
                            else
                                Utils.CallProcess("emsdk", "install " + emsdkVer);

                            //Utils.CopyFile(
                            //    Path.Combine(repoRootDir, "external", "emscripten", "Emscripten.cmake"),
                            //    Path.Combine(EmscriptenDir, "cmake", "Modules", "Platform"));
                        }
                    }
                }

                EmscriptenFound = true;
            }

        }
    }
}
