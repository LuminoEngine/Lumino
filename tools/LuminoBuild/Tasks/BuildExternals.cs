using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class BuildExternals : BuildTask
    {
        public override string CommandName => "BuildExternals";

        public override void Build(Build b)
        {
            string lockFile = $"{b.Triplet}.lock";

            using (CurrentDir.Enter(b.BuildToolsDir))
            {
                if (File.Exists(lockFile)) return;

                if (!Directory.Exists(b.VcpkgDir))
                {
                    Proc.Make("git", "clone -b 2022.02.23 https://github.com/microsoft/vcpkg.git").WithSilent().Call();
                }

                using (CurrentDir.Enter(b.VcpkgDir))
                {
                    if (!File.Exists("vcpkg.exe") && !File.Exists("vcpkg"))
                    {
                        Proc.Make("bootstrap-vcpkg").WithShell().WithSilent().Call();
                    }

                    // Toolchain ファイルのテストするときは --editable 付けると良い
                    var options = $"--editable --overlay-triplets={b.RootDir}/external/custom-triplets";
                    //var options = $"--overlay-triplets={b.RootDir}/external/custom-triplets";

                    Proc.Make("vcpkg", $"install nanovg:{b.Triplet} {options}").Call();

                    Proc.Make("vcpkg", $"install fmt:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install yaml-cpp:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install toml11:{b.Triplet} {options}").WithSilent().Call();

                    Proc.Make("vcpkg", $"install zlib:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install libpng:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install freetype[core,png,zlib]:{b.Triplet} {options}").WithSilent().Call();    // emsdk では brotli がビルドエラーになるため機能を制限する
                    Proc.Make("vcpkg", $"install libvorbis:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install pcre2:{b.Triplet} {options}").WithSilent().Call();

                    Proc.Make("vcpkg", $"install box2d:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install bullet3:{b.Triplet} {options}").WithSilent().Call();

                    Proc.Make("vcpkg", $"install stb:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install tinyobjloader:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install tinygltf:{b.Triplet} {options}").WithSilent().Call();
                    Proc.Make("vcpkg", $"install imgui[docking-experimental]:{b.Triplet} {options}").WithSilent().Call();

                    if (b.IsDesktopSystem)
                    {
                        Proc.Make("vcpkg", $"install gtest:{b.Triplet} {options}").WithSilent().Call();

                        // wasm ではそもそもビルドできない。
                        // …というより glslangValidator がビルドされないことで vcpkg の vcpkg_copy_tools タスクが失敗している。
                        Proc.Make("vcpkg", $"install glslang:{b.Triplet} {options}").WithSilent().Call();
                        Proc.Make("vcpkg", $"install spirv-cross:{b.Triplet} {options}").WithSilent().Call();

                        // wasm で必要ないもの
                        Proc.Make("vcpkg", $"install glfw3:{b.Triplet} {options}").WithSilent().Call();
                        Proc.Make("vcpkg", $"install vulkan-headers:{b.Triplet} {options}").WithSilent().Call();
                        Proc.Make("vcpkg", $"install glad:{b.Triplet} {options}").WithSilent().Call();
                        Proc.Make("vcpkg", $"install gtest:{b.Triplet} {options}").WithSilent().Call();
                        Proc.Make("vcpkg", $"install openal-soft:{b.Triplet} {options}").WithSilent().Call();
                    }

                    if (BuildEnvironment.FromCI)
                    {
                        // Free disk space
                        // https://github.com/Microsoft/vcpkg/issues/2352
                        Directory.Delete("downloads", true);
                        Directory.Delete("buildtrees", true);
                    }
                }

                File.WriteAllText(lockFile, "The presence of this file indicates that the dependency is ready to be placed by LuminoBuild.");
            }
        }
    }
}
