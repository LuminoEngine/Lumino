using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngine_macOS : BuildTask
    {
        public override string CommandName { get { return "BuildEngine_macOS"; } }

        public override string Description { get { return "BuildEngine_macOS"; } }

        public override void Build(Builder builder)
        {
            //BuildProject(builder, "Debug");
            BuildProject(builder, "Release");
        }

        public void BuildProject(Builder builder, string config)
        {
            string cmakeOutputDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir, "macOS-Release");

            string buildDir = Path.Combine(builder.LuminoBuildDir, $"macOS-{config}");
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            var args = new string[]
            {
                $"{builder.LuminoRootDir}",
                $"-DCMAKE_INSTALL_PREFIX={cmakeOutputDir}",
                $"-DLN_BUILD_TESTS=ON",
                $"-DLN_BUILD_TOOLS=ON",
                $"-DLN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=ON",
                $"-G", "\"Xcode\"",
            };
            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build . --config {config} --target install");
        }
    }
}
