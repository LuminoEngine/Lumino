using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngine_Emscripten : BuildTask
    {
        public override string CommandName => "BuildEngine_Emscripten";

        //public override List<string> Dependencies => new List<string>() { "BuildExternalProjects" };

        public override void Build(Builder builder)
        {
            var buildArchDir = "Emscripten";

            var buildDir = Path.Combine(builder.LuminoBuildDir, buildArchDir);
            var installDir = Path.Combine(builder.LuminoBuildDir, buildArchDir, BuildEnvironment.EngineInstallDirName);
            var cmakeSourceDir = builder.LuminoRootDir;
            var dependenciesRoot = Path.Combine(EmscriptenBuildEnv.EmscriptenSysRootLocal, "ExternalInstall");

            Directory.CreateDirectory(buildDir);

            var script = Path.Combine(buildDir, "build.bat");
            using (var f = new StreamWriter(script))
            {
                f.WriteLine($"cd /d \"{EmscriptenBuildEnv.EmsdkDir}\"");
                f.WriteLine($"call emsdk activate {EmscriptenBuildEnv.emsdkVer}");
                f.WriteLine($"call emsdk_env.bat");
                f.WriteLine($"cd /d \"{Utils.ToWin32Path(buildDir)}\"");
                f.WriteLine($"call emcmake cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX={installDir} -DLN_DEPENDENCIES_ROOT={dependenciesRoot} -DLN_BUILD_TESTS=OFF -DLN_BUILD_TOOLS=OFF -G \"MinGW Makefiles\" {cmakeSourceDir}");
                //f.WriteLine($"call emcmake cmake -DCMAKE_BUILD_TYPE=Release -DLN_BUILD_TESTS=OFF -DLN_BUILD_TOOLS=OFF -G \"MinGW Makefiles\" {cmakeSourceDir}");
                f.WriteLine($"call cmake --build . -j8");
                f.WriteLine($"call cmake --build . --target install");
            }

            Utils.CallProcess(script); // bat の中でエラーが発生すれば、例外に乗って出てくる


            // Path.Combine(EmscriptenBuildEnv.EmscriptenSysRootLocal, "Lumino");//


            // emcmake で find_library などを行う場合、Emscripten のシステムフォルダ以外は検索しないようにツールチェインファイルで封印されている。
            // Lumino 本体のビルド時にライブラリを探すことができるようにするため、システムフォルダに一式コピーしておく。
            //Utils.CopyDirectory(installDir, Path.Combine(BuildEnvironment.EmscriptenDir, "system"));


            //string emRootDir = BuildEnvironment.EmscriptenDir;
            //string emInstallDir = BuildEnvironment.EmscriptenDir;
            //string bundlePythonDir = Path.Combine(emInstallDir, "python", "2.7.5.3_64bit");

            //string cmakeOutputDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir, "Emscripten");

            //string path = Environment.GetEnvironmentVariable("PATH");
            //path = bundlePythonDir + ";" + path;

            //var environmentVariables = new Dictionary<string, string>()
            //{
            //    { "PATH", path }
            //};

            //string buildDir = Path.Combine(builder.LuminoBuildDir, "Emscripten");
            //Directory.CreateDirectory(buildDir);
            //Directory.SetCurrentDirectory(buildDir);

            //Utils.CallProcess(BuildEnvironment.emcmake, $"cmake {builder.LuminoRootDir} -DCMAKE_INSTALL_PREFIX={cmakeOutputDir} -G \"MinGW Makefiles\"", environmentVariables);
            //Utils.CallProcess("cmake", $"--build {buildDir}", environmentVariables);
            //Utils.CallProcess("cmake", $"--build {buildDir} --target install", environmentVariables);
        }
    }
}
