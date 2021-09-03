using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class BuildExternals : BuildTask
    {
        public override void Run(Build b)
        {
            using (b.CurrentDir(b.BuildDir))
            {
                if (!Directory.Exists(b.VcpkgDir))
                {
                    b.Call("git", "clone https://github.com/microsoft/vcpkg.git");
                }

                using (b.CurrentDir(b.VcpkgDir))
                {
                    if (!File.Exists("vcpkg.exe") && !File.Exists("vcpkg"))
                    {
                        Utils.CallProcessShell("bootstrap-vcpkg");
                    }



                    
                    b.Call("vcpkg", "install glad:" + b.Triplet);
                    b.Call("vcpkg", "install glslang:" + b.Triplet);
                    b.Call("vcpkg", "install spirv-cross:" + b.Triplet);
                    
                    b.Call("vcpkg", "install imgui[docking-experimental]:" + b.Triplet);

                    b.Call("vcpkg", "install openal-soft:" + b.Triplet);
                    b.Call("vcpkg", "install glfw3:" + b.Triplet);
                    b.Call("vcpkg", "install vulkan-headers:" + b.Triplet);
                    //b.Call("vcpkg", "install tmxlite:" + b.Triplet);
                    b.Call("vcpkg", "install zlib:" + b.Triplet);
                    b.Call("vcpkg", "install libpng:" + b.Triplet);
                    b.Call("vcpkg", "install nanovg:" + b.Triplet);
                    b.Call("vcpkg", "install stb:" + b.Triplet);
                    b.Call("vcpkg", "install toml11:" + b.Triplet);
                    b.Call("vcpkg", "install box2d:" + b.Triplet);
                    b.Call("vcpkg", "install bullet3:" + b.Triplet);
                    b.Call("vcpkg", "install tinyobjloader:" + b.Triplet);
                    b.Call("vcpkg", "install tinygltf:" + b.Triplet);
                    b.Call("vcpkg", "install freetype:" + b.Triplet);
                    b.Call("vcpkg", "install yaml-cpp:" + b.Triplet);
                    b.Call("vcpkg", "install pcre2:" + b.Triplet);
                    

                    b.Call("vcpkg", "install gtest:" + b.Triplet);
                    b.Call("vcpkg", "install fmt:" + b.Triplet);

                    
                    b.Call("vcpkg", "install libvorbis:" + b.Triplet);
                    //b.Call("vcpkg", "install imgui:" + b.Triplet);
                }
            }
        }
    }
}
