using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class BuildExternals
    {
        public void Run(Build b)
        {
            using (b.CurrentDir(b.BuildDir))
            {
                if (!Directory.Exists(b.VcpkgDir))
                {
                    b.Call("git", "clone https://github.com/microsoft/vcpkg.git");
                }

                using (b.CurrentDir(b.VcpkgDir))
                {
                    //b.Call("bootstrap-vcpkg");
                    Utils.CallProcessShell("bootstrap-vcpkg");
                }
            }
        }
    }
}
