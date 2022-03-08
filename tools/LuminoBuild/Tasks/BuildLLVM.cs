using System;
using System.IO;
using System.Linq;
using LuminoBuild;


namespace LuminoBuild.Tasks
{
    class BuildLLVM : BuildTask
    {
        public override string CommandName => "BuildLLVM";

        public override void Build(Build b)
        {
            using (CurrentDir.Enter(b.VcpkgDir))
            {
                Proc.Make("vcpkg", "install llvm:" + b.Triplet).Call();
            }
        }
    }
}
