using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class BuildEngine : BuildTask
    {
        public override string CommandName => "BuildEngine";

        public override string[] Depends => new string[] { "BuildExternals" };

        public override void Build(Build b)
        {
            using (CurrentDir.Enter(b.EngineBuildDir))
            {
                // Configuration
                {
                    //var generator = "MinGW Makefiles";
                    var generator = "Ninja";

                    var args = new string[]
                    {
                        $"-G\"{generator}\"",
                        $"-DCMAKE_MAKE_PROGRAM=" + EmscriptenEnv.Ninja,
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
