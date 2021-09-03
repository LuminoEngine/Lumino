using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class BuildEngine_Emscripten : BuildTask
    {
        public override void Run(Build b)
        {
            using (b.CurrentDir(b.EngineBuildDir))
            {
                // Configuration
                {
                    //var generator = "MinGW Makefiles";
                    var generator = "Ninja";

                    var args = new string[]
                    {
                        $"-G\"{generator}\"",
                        $"-DCMAKE_MAKE_PROGRAM=" + @"C:\Proj\LN\Lumino\_build\vcpkg\downloads\tools\ninja\1.10.2-windows\ninja.exe",
                        $"-DCMAKE_BUILD_TYPE=Release",
                        $"-DCMAKE_TOOLCHAIN_FILE=\"{b.VcpkgDir}/scripts/buildsystems/vcpkg.cmake\"",
                        $"-DCMAKE_INSTALL_PREFIX=\"{b.EngineInstallDir}\"",
                        $"-DCMAKE_DEBUG_POSTFIX=d",
                        $"-DVCPKG_CHAINLOAD_TOOLCHAIN_FILE={EmscriptenEnv.EmsdkDir}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake",
                        //$"-DX_VCPKG_APPLOCAL_DEPS_INSTALL=ON",
                        $"-DVCPKG_TARGET_TRIPLET=\"{b.Triplet}\"",
                        $"-DLN_BUILD_TESTS=OFF",
                        b.RootDir,
                    };
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
