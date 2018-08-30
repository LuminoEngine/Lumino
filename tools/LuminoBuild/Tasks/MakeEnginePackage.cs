using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class MakeEnginePackage : BuildTask
    {
        public override string CommandName => "MakeEnginePackage";

        public override string Description => "MakeEnginePackage";

        public override void Build(Builder builder)
        {
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir);
            var targetRootDir = Path.Combine(builder.LuminoBuildDir, "ReleasePackage");
            Directory.CreateDirectory(targetRootDir);

            // docs
            {
                // Readme
                Utils.GenerateFile(
                    Path.Combine(targetRootDir, "Readme.txt"),
                    Path.Combine(builder.LuminoPackageSourceDir, "Readme.txt.template"),
                    new Dictionary<string, string>{ { "%%LuminoVersion%%", builder.VersionString } });
            }

            // C++ Engine
            {
                string cppEngineRoot = Path.Combine(targetRootDir, "Engine", "Cpp");
                Directory.CreateDirectory(cppEngineRoot);
                
                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoSetup.cmake"),
                    Path.Combine(cppEngineRoot, "LuminoSetup.cmake"), true);


                var engineArchs = new string[]
                {
                    "MSVC2017-x86-MD",
                    "MSVC2017-x86-MT",
                    "MSVC2017-x64-MD",
                    "Emscripten",
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

                var externalInstallDir = Path.Combine(builder.LuminoBuildDir, "MSVC2017-x86-MD", "ExternalInstall");
                
                foreach (var arch in engineArchs)
                {
                    var targetDir = Path.Combine(cppEngineRoot, arch);

                    Utils.CopyDirectory(
                        Path.Combine(builder.LuminoSourceDir, "LuminoCore", "include"),
                        Path.Combine(targetDir, "include"));

                    Utils.CopyDirectory(
                        Path.Combine(builder.LuminoSourceDir, "LuminoEngine", "include"),
                        Path.Combine(targetDir, "include"));

                    Utils.CopyDirectory(
                        Path.Combine(tempInstallDir, arch, "lib"),
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
