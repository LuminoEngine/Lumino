using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    class MakeInstaller : BuildRule
    {
        public override string Name => "MakeInstaller";

        public override void Build(Builder builder)
        {
            builder.DoTask("setup");
            builder.DoTask("MakeVersionHeader");
            builder.DoTask("cpp-b");
            builder.DoTask("cpp-p");
            builder.DoTask("installer");
        }
    }
}
