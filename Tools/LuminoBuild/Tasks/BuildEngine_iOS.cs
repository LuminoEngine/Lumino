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
            string cmakeOutputDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir, "iOS");

            var iOSToolchainFile = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "ExternalSource", "ios-cmake", "ios.toolchain.cmake "));
            var buildDir = Path.Combine(builder.LuminoBuildDir, "iOS");
            var generator = "Xcode";
            var args = new string[]
            {
                $"{builder.LuminoRootDir}",
                $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\"",
                $"-DIOS_PLATFORM=OS",
                $"-DCMAKE_INSTALL_PREFIX={cmakeOutputDir}",
                $"-DLN_BUILD_TESTS=OFF",
                $"-DLN_BUILD_TOOLS=OFF",
                $"-G\"Xcode\"",
            };
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build {buildDir}");
            Utils.CallProcess("cmake", $"--build {buildDir} --target install");
        }
    }
}
