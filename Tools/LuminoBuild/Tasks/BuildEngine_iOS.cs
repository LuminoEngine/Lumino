using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngine_iOS : BuildTask
    {
        public override string CommandName { get { return "BuildEngine_iOS"; } }

        public override string Description { get { return "BuildEngine_iOS"; } }

        public override void Build(Builder builder)
        {
            string cmakeOutputDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir, "macOS");

            string buildDir = Path.Combine(builder.LuminoBuildDir, "macOS");
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            Utils.CallProcess("cmake", $"{builder.LuminoRootDir} -DCMAKE_INSTALL_PREFIX={cmakeOutputDir} -G \"Xcode\"");
            Utils.CallProcess("cmake", $"--build {buildDir}");
            Utils.CallProcess("cmake", $"--build {buildDir} --target install");
        }
    }
}
