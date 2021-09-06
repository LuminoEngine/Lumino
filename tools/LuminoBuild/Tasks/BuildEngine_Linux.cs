using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngine_Linux : BuildTask
    {
        public override string CommandName => "BuildEngine_Linux";

        public override void Build(Build builder)
        {
            var buildDir = Path.Combine(builder.BuildDir, "Linux-x86_64");
            var installDir = Path.Combine(builder.BuildDir, "Linux-x86_64", BuildEnvironment.EngineInstallDirName);

            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            var args = string.Format(
                "-DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=\"{0}\" -DLN_BUILD_TESTS=ON ../..",
                installDir);

            Utils.CallProcess("cmake", args);
            Utils.CallProcess("cmake", "--build");
            Utils.CallProcess("ctest", "-V");
            Utils.CallProcess("cmake", "--build . --target install");
        }
    }
}
