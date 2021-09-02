using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    class BuildPackage : BuildRule
    {
        public override string Name => "BuildPackage";

        public override void Build(Builder builder)
        {
            var local = new BuildLocalPackage();
            local.Build(builder);

            if (Utils.IsWin32)
            {
                builder.DoTask("MakeNuGetPackage_Core");
                builder.DoTask("MakeInstaller_Win32");
            }
        }
    }
}
