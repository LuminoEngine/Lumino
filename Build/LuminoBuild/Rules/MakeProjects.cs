using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    class MakeProjects : BuildRule
    {
        public override string Name => "MakeProjects";

        public override void Build(Builder builder)
        {
            builder.DoTask("MakeVersionHeader");
            builder.DoTask("make-vs");
        }
    }
}
