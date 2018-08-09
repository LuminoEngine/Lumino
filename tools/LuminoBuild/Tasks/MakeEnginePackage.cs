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
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp");
            var targetRootDir = Path.Combine(builder.LuminoBuildDir, "EnginePackage");

            Utils.CopyDirectory(
                Path.Combine(builder.LuminoRootDir, "src", "LuminoCore", "include"),
                Path.Combine(targetRootDir, "include"));

            Utils.CopyDirectory(
                Path.Combine(builder.LuminoRootDir, "src", "LuminoEngine", "include"),
                Path.Combine(targetRootDir, "include"));

            Utils.CopyDirectory(
                Path.Combine(tempInstallDir, "MSVC2017-x86-MD", "lib"),
                Path.Combine(targetRootDir, "lib", "MSVC2017-x86-MD"));

            Utils.CopyDirectory(
                Path.Combine(tempInstallDir, "MSVC2017-x86-MT", "lib"),
                Path.Combine(targetRootDir, "lib", "MSVC2017-x86-MT"));

            Utils.CopyDirectory(
                Path.Combine(tempInstallDir, "MSVC2017-x64-MD", "lib"),
                Path.Combine(targetRootDir, "lib", "MSVC2017-x64-MD"));

            Utils.CopyDirectory(
                Path.Combine(tempInstallDir, "MSVC2017-x64-MT", "lib"),
                Path.Combine(targetRootDir, "lib", "MSVC2017-x64-MT"));

            Utils.CopyDirectory(
                Path.Combine(tempInstallDir, "Emscripten", "lib"),
                Path.Combine(targetRootDir, "lib", "Emscripten"));
        }
    }
}
