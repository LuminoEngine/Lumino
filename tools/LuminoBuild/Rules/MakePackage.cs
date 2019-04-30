using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    class MakePackage : BuildRule
    {
        public override string Name => "MakePackage";

        public override void Build(Builder builder)
        {
            var local = new MakeLocalPackage();
            local.Build(builder);

            if (Utils.IsWin32)
            {
                builder.DoTask("MakeNuGetPackage_Core");
                builder.DoTask("MakeInstaller_Win32");
            }
        }
    }
}
