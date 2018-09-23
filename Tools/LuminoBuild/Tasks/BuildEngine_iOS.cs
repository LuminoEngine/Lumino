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
            // framework 作成時は CMAKE_DEBUG_POSTFIX が効かないようなので個別に作る
            //BuildProject(builder, "iOS-OS-Debug", "Debug", "OS");
            //BuildProject(builder, "iOS-OS-Release", "Release", "OS");
            BuildProject(builder, "iOS-SIMULATOR64-Debug", "Debug", "SIMULATOR64");
            //BuildProject(builder, "iOS-SIMULATOR64-Release", "Release", "SIMULATOR64");
        }

        private void BuildProject(Builder builder, string buildDirName, string config, string platform)
        {
            string cmakeInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir, buildDirName);

            var iOSToolchainFile = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "ExternalSource", "ios-cmake", "ios.toolchain.cmake"));
            var buildDir = Path.Combine(builder.LuminoBuildDir, buildDirName);
            var generator = "Xcode";
            var args = new string[]
            {
                $"{builder.LuminoRootDir}",
                $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\"",
                $"-DIOS_PLATFORM={platform}",
                $"-DCMAKE_INSTALL_PREFIX={cmakeInstallDir}",
                $"-DLN_BUILD_TESTS=OFF",
                $"-DLN_BUILD_TOOLS=OFF",
                $"-DLN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=OFF",
                $"-G\"Xcode\"",
            };
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build . --config {config} --target install");
        }
    }
}
