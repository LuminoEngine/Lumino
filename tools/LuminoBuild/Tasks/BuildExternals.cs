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

                    b.Call("vcpkg", "install gtest:" + b.Triplet);
                    b.Call("vcpkg", "install fmt:" + b.Triplet);
                }
            }
        }
    }
}
