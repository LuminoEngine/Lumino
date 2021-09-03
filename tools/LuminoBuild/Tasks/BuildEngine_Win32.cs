using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class BuildEngine_Win32 : BuildTask
    {
        public class MSVCTargetInfo
        {
            public string Generator;
            public string Arch;
            public string StaticRuntime;
        }

        public static Dictionary<string, MSVCTargetInfo> TargetInfoMap = new Dictionary<string, MSVCTargetInfo>()
        {
            { "x64-windows", new MSVCTargetInfo { Generator = "Visual Studio 16 2019", Arch="x64", StaticRuntime = "OFF" } },
            { "x86-windows", new MSVCTargetInfo { Generator = "Visual Studio 16 2019", Arch="Win32", StaticRuntime = "OFF" } },
        };


        public override void Run(Build b)
        {
            var targetInfo = TargetInfoMap[b.Triplet];

            using (b.CurrentDir(b.EngineBuildDir))
            {
                // Configuration
                {
                    var args = new string[]
                    {
                        $"-G\"{targetInfo.Generator}\"",
                        $"-DCMAKE_TOOLCHAIN_FILE=\"{b.VcpkgDir}/scripts/buildsystems/vcpkg.cmake\"",
                        $"-DCMAKE_INSTALL_PREFIX=\"{b.EngineInstallDir}\"",
                        $"-DCMAKE_DEBUG_POSTFIX=d",
                        $"-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
                        $"-DLN_MSVC_STATIC_RUNTIME={targetInfo.StaticRuntime}",
                        $"-DLN_BUILD_TESTS=ON",
                        $"-DLN_BUILD_EDITOR=ON",
                        $"-DLN_BUILD_SHARED_LIBRARY=ON",
                        $"-DLN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=ON",
                        b.RootDir,
                    };
                    Utils.CallProcess("cmake", string.Join(' ', args));
                }

                // Build
                {
                    if (b.IsDebug)
                    {
                        Utils.CallProcess("cmake", $"--build . --config Debug");
                        Utils.CallProcess("ctest", $"-C Debug --output-on-failure --verbose");
                        Utils.CallProcess("cmake", $"--build . --config Debug --target INSTALL");
                    }

                    if (b.IsRelease)
                    {
                        Utils.CallProcess("cmake", $"--build . --config Release");
                        Utils.CallProcess("ctest", $"-C Release --output-on-failure --verbose");
                        Utils.CallProcess("cmake", $"--build . --config Release --target INSTALL");
                    }
                }
            }

            if (b.FromCI) Utils.DeleteDirectory(b.EngineBuildDir);   // Disk space saving
        }
    }
}
