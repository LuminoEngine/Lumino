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
            builder.DoTask("BuildEngineWin32");
            builder.DoTask("BuildDocuments");
            builder.DoTask("MakeNuGetPackage_Core");
        }
    }
}
