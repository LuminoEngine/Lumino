using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEmscripten : BuildTask
    {
        public override string CommandName { get { return "BuildEmscripten"; } }

        public override string Description { get { return "Build Emscripten"; } }

        public override void Build(Builder builder)
        {
            string emRootDir = Environment.GetEnvironmentVariable("EMSCRIPTEN");
            string emInstallDir = Path.GetDirectoryName(Path.GetDirectoryName(emRootDir));
            string bundlePythonDir = Path.Combine(emInstallDir, "python", "2.7.5.3_64bit");
            string emcmake = Path.Combine(emRootDir, Utils.IsWin32 ? "emcmake.bat" : "emcmake");

            string path = Environment.GetEnvironmentVariable("PATH");
            path = bundlePythonDir + ";" + path;

            var environmentVariables = new Dictionary<string, string>()
            {
                { "PATH", path }
            };

            string buildDir = Path.Combine(builder.LuminoBuildDir, "Emscripten");
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            Utils.CallProcess(emcmake, $"cmake {builder.LuminoRootDir} -G \"MinGW Makefiles\"", environmentVariables);
            Utils.CallProcess("cmake", $"--build {buildDir}", environmentVariables);
        }
    }
}
