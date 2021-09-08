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
            using (CurrentDir.Enter(b.BuildToolsDir))
            {
                if (!Directory.Exists(b.VcpkgDir))
                {
                    Proc.Make("git", "clone -b 2021.05.12 https://github.com/microsoft/vcpkg.git").WithSilent().Call();
                }

                using (CurrentDir.Enter(b.VcpkgDir))
                {
                    if (!File.Exists("vcpkg.exe") && !File.Exists("vcpkg"))
                    {
                        Proc.Make("bootstrap-vcpkg").WithShell().WithSilent().Call();
                    }

                    Proc.Make("vcpkg", "install fmt:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install yaml-cpp:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install toml11:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install zlib:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install libpng:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install freetype[core,png,zlib]:" + b.Triplet).WithSilent().Call();    // emsdk では brotli がビルドエラーになるため機能を制限する
                    //Proc.Make("vcpkg", "install libvorbis:" + b.Triplet).WithSilent().Call();
                    Proc.Make("vcpkg", "install pcre2:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install box2d:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install bullet3:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install nanovg:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install stb:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install tinyobjloader:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install tinygltf:" + b.Triplet).WithSilent().Call();
                    //Proc.Make("vcpkg", "install imgui[docking-experimental]:" + b.Triplet).WithSilent().Call();

                    //if (b.System == "emscripten")
                    //{

                    //}
                    //else
                    //{
                    //    Proc.Make("vcpkg", "install glfw3:" + b.Triplet).WithSilent().Call();
                    //    Proc.Make("vcpkg", "install vulkan-headers:" + b.Triplet).WithSilent().Call();
                    //    Proc.Make("vcpkg", "install glad:" + b.Triplet).WithSilent().Call();
                    //    Proc.Make("vcpkg", "install glslang:" + b.Triplet).WithSilent().Call();
                    //    Proc.Make("vcpkg", "install spirv-cross:" + b.Triplet).WithSilent().Call();
                    //    Proc.Make("vcpkg", "install gtest:" + b.Triplet).WithSilent().Call();
                    //    Proc.Make("vcpkg", "install openal-soft:" + b.Triplet).WithSilent().Call();
                    //}
                }
            }
        }
    }
}
