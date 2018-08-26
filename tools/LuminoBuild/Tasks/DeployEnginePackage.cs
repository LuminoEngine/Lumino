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

                Utils.CopyDirectory(
                    Path.Combine(tempInstallDir, "MSVC2017-x86-MD", "lib"),
                    Path.Combine(cppEngineRoot, "lib", "MSVC2017-x86-MD"));

                Utils.CopyDirectory(
                    Path.Combine(tempInstallDir, "MSVC2017-x86-MT", "lib"),
                    Path.Combine(cppEngineRoot, "lib", "MSVC2017-x86-MT"));

                Utils.CopyDirectory(
                    Path.Combine(tempInstallDir, "MSVC2017-x64-MD", "lib"),
                    Path.Combine(cppEngineRoot, "lib", "MSVC2017-x64-MD"));

                Utils.CopyDirectory(
                    Path.Combine(tempInstallDir, "MSVC2017-x64-MT", "lib"),
                    Path.Combine(cppEngineRoot, "lib", "MSVC2017-x64-MT"));

                Utils.CopyDirectory(
                    Path.Combine(tempInstallDir, "Emscripten", "lib"),
                    Path.Combine(cppEngineRoot, "lib", "Emscripten"));
            }
        }
    }
}
