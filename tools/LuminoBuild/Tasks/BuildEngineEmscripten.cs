using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngineEmscripten : BuildTask
    {
        public override string CommandName { get { return "BuildEngineEmscripten"; } }

        public override string Description { get { return "Build Emscripten"; } }

        public override void Build(Builder builder)
        {
            string emRootDir = BuildEnvironment.EmscriptenDir;
            string emInstallDir = BuildEnvironment.EmscriptenDir;
            string bundlePythonDir = Path.Combine(emInstallDir, "python", "2.7.5.3_64bit");

            string cmakeOutputDir = Path.Combine(builder.LuminoBuildDir, "CMakeInstallTemp", "Emscripten");

            string path = Environment.GetEnvironmentVariable("PATH");
            path = bundlePythonDir + ";" + path;

            var environmentVariables = new Dictionary<string, string>()
            {
                { "PATH", path }
            };

            string buildDir = Path.Combine(builder.LuminoBuildDir, "Emscripten");
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            Utils.CallProcess(BuildEnvironment.emcmake, $"cmake {builder.LuminoRootDir} -DCMAKE_INSTALL_PREFIX={cmakeOutputDir} -G \"MinGW Makefiles\"", environmentVariables);
            Utils.CallProcess("cmake", $"--build {buildDir}", environmentVariables);
            Utils.CallProcess("cmake", $"--build {buildDir} --target install", environmentVariables);
        }
    }
}
