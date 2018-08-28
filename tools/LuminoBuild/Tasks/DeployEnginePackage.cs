using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class DeployEnginePackage : BuildTask
    {
        public override string CommandName => "DeployEnginePackage";

        public override string Description => "DeployEnginePackage";

        public override void Build(Builder builder)
        {
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir);
            var targetRootDir = Path.Combine(builder.LuminoBuildDir, "ReleasePackage");

            // C++ Engine
            {
                string cppEngineRoot = Path.Combine(targetRootDir, "Engine", "Cpp");

                Utils.CopyDirectory(
                    Path.Combine(builder.LuminoRootDir, "src", "LuminoCore", "include"),
                    Path.Combine(cppEngineRoot, "include"));

                Utils.CopyDirectory(
                    Path.Combine(builder.LuminoRootDir, "src", "LuminoEngine", "include"),
                    Path.Combine(cppEngineRoot, "include"));


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
                    Utils.CopyDirectory(
                        Path.Combine(tempInstallDir, arch, "lib"),
                        Path.Combine(cppEngineRoot, "lib", arch));

                    foreach (var lib in externalLibs)
                    {
                        Utils.CopyDirectory(
                            Path.Combine(externalInstallDir, lib, "lib"),
                            Path.Combine(cppEngineRoot, "lib", "MSVC2017-x86-MD"));
                    }
                }
            }
        }
    }
}
