using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild
{
    class EmscriptenBuildEnv
    {
        public const string emsdkVer = "1.39.8";
        public const string emVer = "1.39.8";

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

        public static void Initialize(Build builder)
        {
            //var cloneParentDir = Path.Combine(builder.LuminoBuildCacheDir, "Tools");
            var cloneParentDir = Path.Combine(builder.BuildDir, "Emscripten");
            EmsdkDir = Path.Combine(builder.BuildDir, "Emscripten", "emsdk");
            //EmsdkDir = Path.Combine(builder.LuminoBuildCacheDir, "Tools", "emsdk");
            EmscriptenRoot = Path.Combine(EmsdkDir, "upstream", "emscripten");
            EmscriptenSysRootLocal = Path.Combine(EmscriptenRoot, "system", "local");



            // Install emsdk
            if (BuildEnvironment.IsWebTarget && Utils.IsWin32)
            {
                if (!builder.ExistsCache(EmsdkDir))
                {
                    if (!Directory.Exists(EmsdkDir))
                    {
                        Directory.CreateDirectory(cloneParentDir);
                        using (var cd = CurrentDir.Enter(cloneParentDir))
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

                    builder.CommitCache(EmsdkDir);
                }
            }


            if (Directory.Exists(EmsdkDir))
            {
                EmscriptenFound = true;
            }
        }
    }
}
