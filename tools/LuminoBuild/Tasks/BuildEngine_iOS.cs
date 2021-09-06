using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngine_iOS : BuildTask
    {
        public override string CommandName => "BuildEngine_iOS";

        public override void Build(Build builder)
        {
            // framework 作成時は CMAKE_DEBUG_POSTFIX が効かないようなので個別に作る
            BuildProject(builder, "iOS-OS64", "OS64");
            //BuildProject(builder, "iOS-OS64-Release", "Release", "OS64");
            BuildProject(builder, "iOS-SIMULATOR64", "SIMULATOR64");
            //BuildProject(builder, "iOS-SIMULATOR64-Release", "Release", "SIMULATOR64");
        }

        private void BuildProject(Build builder, string buildDirName, string platform)
        {
            string cmakeInstallDir = Path.Combine(builder.BuildDir, buildDirName, BuildEnvironment.EngineInstallDirName);

            var iOSToolchainFile = Utils.ToUnixPath(Path.Combine(builder.BuildDir, "ExternalSource", "ios-cmake", "ios.toolchain.cmake"));
            var buildDir = Path.Combine(builder.BuildDir, buildDirName, "EngineBuild");
            var generator = "Xcode";
            var args = new string[]
            {
                $"{builder.RootDir}",
                $"-DCMAKE_INSTALL_PREFIX={cmakeInstallDir}",
                $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\"",
                $"-DPLATFORM={platform}",
                $"-DLN_TARGET_ARCH:STRING={buildDirName}",
                $"-DLN_BUILD_TESTS=OFF",
                $"-DLN_BUILD_TOOLS=OFF",
                $"-DLN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=OFF",
                $"-G\"Xcode\"",
            };
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            Utils.CallProcess("cmake", string.Join(' ', args));

            Utils.CallProcess("cmake", $"--build . --config Debug");
            Utils.CallProcess("cmake", $"--build . --config Debug --target install");

            Utils.CallProcess("cmake", $"--build . --config Release");
            Utils.CallProcess("cmake", $"--build . --config Release --target install");
        }
    }
}
