using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngine : BuildTask
    {
        public override string CommandName => "BuildEngine";

        public class MSVCTargetInfo
        {
            public string Generator;
            public string Arch;
            public string StaticRuntime;
            public string LegacyTriplet;
        }

        public static Dictionary<string, MSVCTargetInfo> TargetInfoMap = new Dictionary<string, MSVCTargetInfo>()
        {
            { "x64-windows", new MSVCTargetInfo { Generator = "Visual Studio 16 2019", Arch="x64", StaticRuntime = "OFF", LegacyTriplet = "MSVC2019-x64-MD" } },
            { "x86-windows", new MSVCTargetInfo { Generator = "Visual Studio 16 2019", Arch="Win32", StaticRuntime = "OFF", LegacyTriplet = "MSVC2019-x86-MD" } },
            { "x64-windows-static", new MSVCTargetInfo { Generator = "Visual Studio 16 2019", Arch="x64", StaticRuntime = "ON", LegacyTriplet = "MSVC2019-x64-MT" } },
            { "x86-windows-static", new MSVCTargetInfo { Generator = "Visual Studio 16 2019", Arch="Win32", StaticRuntime = "ON", LegacyTriplet = "MSVC2019-x86-MT" } },
        };

        public override string[] GetDepends(Build b)
        {
            return new string[] { "BuildExternals" };
        }

        public override void Build(Build b)
        {
            if (b.Triplet.Contains("windows"))
            {
                BuildMSVC(b);
            }
            else if (b.Triplet.Contains("emscripten"))
            {
                BuildEmscripten(b);
            }
            else
            {
                throw new Exception($"Invalid triplet {b.Triplet}");
            }
        }

        public void BuildMSVC(Build b)
        {
            var targetInfo = TargetInfoMap[b.Triplet];

            var fileMoving = false;

            var buildDir = b.EngineBuildDir;
            var installDir = b.EngineInstallDir;

            using (CurrentDir.Enter(buildDir))
            {
                // Configuration
                {
                    var additional = "";
                    if (!string.IsNullOrEmpty(targetInfo.Arch))
                        additional += "-A " + targetInfo.Arch;
                    if (!string.IsNullOrEmpty(BuildEnvironment.FbxSdkVS2017))
                        additional += "-DLN_FBX_SDK_PATH:STRING=\"{BuildEnvironment.FbxSdkVS2017}\"";

                    if (b.Options.Components == "core")
                        additional += "-DLN_BUILD_CORE_ONLY=ON";

                    var args = new string[]
                    {
                        $"-G\"{targetInfo.Generator}\"",
                        $"-DCMAKE_TOOLCHAIN_FILE=\"{b.VcpkgDir}/scripts/buildsystems/vcpkg.cmake\"",
                        $"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
                        $"-DCMAKE_DEBUG_POSTFIX=d",
                        $"-DVCPKG_TARGET_TRIPLET={b.Triplet}",
                        $"-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
                        $"-DLN_MSVC_STATIC_RUNTIME={targetInfo.StaticRuntime}",
                        $"-DLN_BUILD_TESTS=ON",
                        $"-DLN_BUILD_EDITOR=ON",
                        //$"-DLN_BUILD_SHARED_LIBRARY=ON",
                        $"-DLN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=ON",
                        $"-DLN_TARGET_ARCH:STRING={targetInfo.LegacyTriplet}",
                        additional,
                        b.RootDir,
                    };
                    Utils.CallProcess("cmake", string.Join(' ', args));

                    // ポストイベントからファイルコピーが行われるため、先にフォルダを作っておく
                    //Directory.CreateDirectory(Path.Combine(b.LuminoLibDir, targetInfo.LegacyTriplet));
                }

                // Build
                {
                    if (string.IsNullOrEmpty(BuildEnvironment.Configuration) || BuildEnvironment.Configuration == "Debug")
                    {
                        Utils.CallProcess("cmake", $"--build . --config Debug");
                        Utils.CallProcess("ctest", $"-C Debug --output-on-failure --verbose");
                        Utils.CallProcess("cmake", $"--build . --config Debug --target INSTALL");
                        if (BuildEnvironment.FromCI)
                            Utils.CallProcess("cmake", $"--build . --config Debug --target clean"); // Free disk space.
                    }

                    if (string.IsNullOrEmpty(BuildEnvironment.Configuration) || BuildEnvironment.Configuration == "Release")
                    {
                        Utils.CallProcess("cmake", $"--build . --config Release");
                        Utils.CallProcess("ctest", $"-C Release --output-on-failure --verbose");
                        Utils.CallProcess("cmake", $"--build . --config Release --target INSTALL");
                        if (BuildEnvironment.FromCI)
                            Utils.CallProcess("cmake", $"--build . --config Release --target clean"); // Free disk space.
                    }
                }
            }

            if (BuildEnvironment.FromCI) Utils.DeleteDirectory(b.EngineBuildDir);   // Free disk space.
        }

        private void BuildEmscripten(Build b)
        {
            using (CurrentDir.Enter(b.EngineBuildDir))
            {
                // Configuration
                {
                    var generator = "Ninja";

                    var buildType = b.Options.Debug ? "Debug" : "Release";

                    var args = new List<string>()
                    {
                        b.RootDir,
                        $"-G\"{generator}\"",
                        $"-DCMAKE_MAKE_PROGRAM=" + EmscriptenEnv.Ninja,
                        $"-DCMAKE_BUILD_TYPE={buildType}",
                        $"-DCMAKE_TOOLCHAIN_FILE=\"{b.VcpkgDir}/scripts/buildsystems/vcpkg.cmake\"",
                        $"-DCMAKE_INSTALL_PREFIX=\"{b.EngineInstallDir}\"",
                        $"-DCMAKE_DEBUG_POSTFIX=d",
                        $"-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE={EmscriptenEnv.EmsdkDir}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake",
                        $"-DVCPKG_TARGET_TRIPLET=\"{b.Triplet}\"",
                        $"-DLN_BUILD_TESTS=OFF",
                    };

                    if (BuildEnvironment.FromCI)
                    {
                        args.Add($"-DLN_BUILD_EXAMPLES=OFF");
                    }

                    Utils.CallProcess("cmake", string.Join(' ', args));
                }

                // Build
                {
                    Utils.CallProcess("cmake", $"--build . -j8");
                    Utils.CallProcess("cmake", $"--build . --target install");
                }
            }
        }
    }
}
