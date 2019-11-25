using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    // Debug and Release build:
    //      dotnet run -- BuildEngine_MSVC MSVC2017-x64-MD
    //
    // Debug only:
    //      dotnet run -- BuildEngine_MSVC MSVC2017-x64-MD Debug
    //
    // Release only:
    //      dotnet run -- BuildEngine_MSVC MSVC2017-x64-MD Release
    //
    class BuildEngine_MSVC : BuildTask
    {
        public override string CommandName => "BuildEngine_MSVC";
        
        class MSVCTargetInfo
        {
            public string Generator;
            public string Platform;
            public string StaticRuntime;
        }

        Dictionary<string, MSVCTargetInfo> _targetInfoMap = new Dictionary<string, MSVCTargetInfo>()
        {
            { "MSVC2017-x86-MD", new MSVCTargetInfo { Generator = "Visual Studio 15", Platform="Win32", StaticRuntime = "OFF" } },
            { "MSVC2017-x86-MT", new MSVCTargetInfo { Generator = "Visual Studio 15", Platform="Win32", StaticRuntime = "ON" } },
            { "MSVC2017-x64-MD", new MSVCTargetInfo { Generator = "Visual Studio 15 Win64", Platform="x64", StaticRuntime = "OFF" } },
            { "MSVC2017-x64-MT", new MSVCTargetInfo { Generator = "Visual Studio 15 Win64", Platform="x64", StaticRuntime = "ON" } },
        };

        public override void Build(Builder builder)
        {
            var targetInfo = _targetInfoMap[BuildEnvironment.Target];

            var targetName = BuildEnvironment.Target;
            if (!string.IsNullOrEmpty(BuildEnvironment.Configuration)) targetName += "-" + BuildEnvironment.Configuration;

            Directory.CreateDirectory(Path.Combine(builder.LuminoBuildDir, targetName));
            Directory.SetCurrentDirectory(Path.Combine(builder.LuminoBuildDir, targetName));

            // Configuration
            {
                var installDir = Path.Combine(builder.LuminoRootDir, "build", targetName, BuildEnvironment.EngineInstallDirName);

                var additional = "";
                if (builder.Args.Contains("--enable-bindings") && targetInfo.StaticRuntime == "OFF" && targetInfo.Platform == "Win32")
                {
                    additional += " -DLN_BUILD_BINDINGS=ON";
                }

                var args = new string[]
                {
                    $"-G\"{targetInfo.Generator}\"",
                    $"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
                    $"-DCMAKE_DEBUG_POSTFIX=d",
                    $"-DLN_MSVC_STATIC_RUNTIME={targetInfo.StaticRuntime}",
                    $"-DLN_BUILD_TESTS=ON",
                    $"-DLN_BUILD_TOOLS=ON",
                    $"-DLN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=ON",
                    $"-DLN_TARGET_ARCH:STRING={BuildEnvironment.Target}",
                    additional,
                    $" ../..",
                };
                Utils.CallProcess("cmake", string.Join(' ', args));

                // ポストイベントからファイルコピーが行われるため、先にフォルダを作っておく
                Directory.CreateDirectory(Path.Combine(builder.LuminoLibDir, targetName));
            }

            // Build
            {
                if (string.IsNullOrEmpty(BuildEnvironment.Configuration) || BuildEnvironment.Configuration == "Debug")
                {
                    Utils.CallProcess("cmake", $"--build . --config Debug");
                    Utils.CallProcess("ctest", $"-C Debug --output-on-failure");
                    Utils.CallProcess("cmake", $"--build . --config Debug --target INSTALL");
                }

                if (string.IsNullOrEmpty(BuildEnvironment.Configuration) || BuildEnvironment.Configuration == "Release")
                {
                    Utils.CallProcess("cmake", $"--build . --config Release");
                    Utils.CallProcess("ctest", $"-C Release --output-on-failure");
                    Utils.CallProcess("cmake", $"--build . --config Release --target INSTALL");
                }
            }
        }
    }
}
