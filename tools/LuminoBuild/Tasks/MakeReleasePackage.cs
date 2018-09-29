using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class MakeReleasePackage : BuildTask
    {
        public override string CommandName => "MakeReleasePackage";

        public override string Description => "MakeReleasePackage";

        public override void Build(Builder builder)
        {
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir);

            string targetRootDir;
            if (Utils.IsWin32)
                targetRootDir = Path.Combine(builder.LuminoRootDir, "ReleasePackage.Win32");
            else
                targetRootDir = Path.Combine(builder.LuminoRootDir, "ReleasePackage.macOS");
            Directory.CreateDirectory(targetRootDir);

            string cppEngineRoot = Path.Combine(targetRootDir, "Engine", "Cpp");
            Directory.CreateDirectory(cppEngineRoot);

            // docs
            {
                // Readme
                Utils.GenerateFile(
                    Path.Combine(targetRootDir, "Readme.txt"),
                    Path.Combine(builder.LuminoPackageSourceDir, "Readme.txt.template"),
                    new Dictionary<string, string> { { "%%LuminoVersion%%", builder.VersionString } });
            }
            
            // C++ Engine (common)
            {
                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoSetup.cmake"),
                    Path.Combine(cppEngineRoot, "LuminoSetup.cmake"), true);

                File.Copy(
                    Path.Combine(builder.LuminoExternalDir, "ImportExternalLibraries.cmake"),
                    Path.Combine(cppEngineRoot, "ImportExternalLibraries.cmake"), true);
            }


            var engineArchs = new[]
            {
                new { Src = "MSVC2017-x86-MT", Dst = "MSVC2017-x86-MT" },
                new { Src = "Emscripten", Dst = "Emscripten" },
                new { Src = "Android-arm64-v8a-Debug", Dst = "Android-arm64-v8a" },
                new { Src = "Android-arm64-v8a-Release", Dst = "Android-arm64-v8a" },
                new { Src = "Android-armeabi-v7a-Debug", Dst = "Android-armeabi-v7a" },
                new { Src = "Android-armeabi-v7a-Release", Dst = "Android-armeabi-v7a" },
                new { Src = "Android-x86-Debug", Dst = "Android-x86" },
                new { Src = "Android-x86-Release", Dst = "Android-x86" },
                new { Src = "Android-x86_64-Debug", Dst = "Android-x86_64" },
                new { Src = "Android-x86_64-Release", Dst = "Android-x86_64" },
                new { Src = "macOS", Dst = "macOS" },
                new { Src = "iOS-SIMULATOR64-Debug", Dst = "iOS-SIMULATOR64-Debug" },
                new { Src = "iOS-SIMULATOR64-Release", Dst = "iOS-SIMULATOR64-Release" },
                new { Src = "iOS-OS-Debug", Dst = "iOS-OS-Debug" },
                new { Src = "iOS-OS-Release", Dst = "iOS-OS-Release" },
            };

            var externalLibs = new string[]
            {
                "glad",
                "glslang",
                "libpng",
                "openal-soft",
                "SDL2",
                "SPIRV-Cross",
                "zlib",
            };

            //if (Utils.IsWin32)
            {
                // C++ Engine
                {

                    foreach (var arch in engineArchs)
                    {
                        if (Directory.Exists(Path.Combine(tempInstallDir, arch.Src)))   // copy if directory exists.
                        {
                            var externalInstallDir = Path.Combine(builder.LuminoBuildDir, arch.Src, "ExternalInstall");
                            var targetDir = Path.Combine(cppEngineRoot, arch.Dst);

                            Utils.CopyDirectory(
                                Path.Combine(builder.LuminoSourceDir, "LuminoCore", "include"),
                                Path.Combine(targetDir, "include"));

                            Utils.CopyDirectory(
                                Path.Combine(builder.LuminoSourceDir, "LuminoEngine", "include"),
                                Path.Combine(targetDir, "include"));

                            Utils.CopyDirectory(
                                Path.Combine(tempInstallDir, arch.Src, "lib"),
                                Path.Combine(targetDir, "lib"));

                            foreach (var lib in externalLibs)
                            {
                                var srcDir = Path.Combine(externalInstallDir, lib, "lib");
                                if (Directory.Exists(srcDir))   // copy if directory exists. openal-soft etc are optional.
                                {
                                    Utils.CopyDirectory(
                                        srcDir,
                                        Path.Combine(targetDir, "lib"));
                                }
                            }
                        }
                    }
                }
            }

        }
    }
}
