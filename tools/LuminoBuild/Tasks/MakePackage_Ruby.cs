using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakePackage_Ruby : BuildTask
    {
        public override string CommandName => "MakePackage_Ruby";

        public override void Build(Builder builder)
        {
            if (Utils.IsWin32)
            {
                var gemprojDir = Path.Combine(builder.LuminoToolsDir, "Bindings", "Ruby", "GemProject");

                Directory.SetCurrentDirectory(gemprojDir);
                Utils.CallProcessShell("bundle", "install");    // bundle.cmd
                Utils.CallProcessShell("rake", "build");
            }
        }

    }
}
