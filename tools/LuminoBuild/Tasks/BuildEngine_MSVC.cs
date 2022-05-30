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

        public override void Build(Build b)
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

                /*
                // Copy external libs to EngineInstall dir
                {
                    var externalInstallDir = Path.Combine(b.BuildDir, targetName, "ExternalInstall");
                    var installLibDir = Path.Combine(installDir, "lib");

                    foreach (var dir in Directory.GetDirectories(externalInstallDir))
                    {
                        var libDir = Path.Combine(dir, "lib");
                        if (Directory.Exists(libDir))
                        {
                            foreach (var file in Directory.GetFiles(libDir, "*.lib", SearchOption.TopDirectoryOnly))
                            {
                                if (fileMoving)
                                    File.Move(file, Path.Combine(installLibDir, Path.GetFileName(file)));
                                else
                                    File.Copy(file, Path.Combine(installLibDir, Path.GetFileName(file)), true);
                            }
                        }
                    }
                }

                // Copy .pdb
                {
                    // CMake では static library の PDB 出力先をコントロールできない。https://cmake.org/cmake/help/v3.1/prop_tgt/PDB_OUTPUT_DIRECTORY.html
                    // そのためビルドスクリプト側でコントロールする。
                    // 以下、パスに "Debug" を含むもののうち、lib と同じ名前の pdb ファイルをコピーする。

                    var installLibDir = Path.Combine(installDir, "lib");
                    var libfiles = Directory.GetFiles(installLibDir, "*.lib", SearchOption.TopDirectoryOnly);
                    var libnames = new HashSet<string>(libfiles.Select(x => Path.GetFileNameWithoutExtension(x)));
                    var pdbFiles = Directory.GetFiles(buildDir, "*.pdb", SearchOption.AllDirectories);
                    foreach (var pdbFile in pdbFiles)
                    {
                        var pdbName = Path.GetFileNameWithoutExtension(pdbFile);

                        // png と glfw は出力ファイル名がカスタマイズされており .pdb と名前が異なっているため個別に指定する
                        if (pdbName.StartsWith("png16"))
                            pdbName = "libpng16";
                        else if (pdbName.StartsWith("glfw"))
                            pdbName = "glfw3";

                        if (pdbFile.Contains("Debug") && libnames.Contains(pdbName))
                        {
                            // FIXME: CI サーバのストレージ不足対策
                            if (fileMoving)
                                File.Move(pdbFile, Path.Combine(installLibDir, Path.GetFileName(pdbFile)));
                            else
                                File.Copy(pdbFile, Path.Combine(installLibDir, Path.GetFileName(pdbFile)), true);
                        }
                    }
                }
                */
            }

            if (BuildEnvironment.FromCI) Utils.DeleteDirectory(b.EngineBuildDir);   // Free disk space.
        }
    }
}
