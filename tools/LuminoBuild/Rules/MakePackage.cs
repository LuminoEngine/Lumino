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
            builder.DoTask("MakeVSProjects");
            builder.DoTask("BuildEngine");

            //if (Utils.IsWin32) builder.DoTask("make-vs");
            //if (Utils.IsMac) builder.DoTask("MakeXCodeProjects");
        }
    }
}
