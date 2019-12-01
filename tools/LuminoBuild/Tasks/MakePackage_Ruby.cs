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

                File.Copy(
                    Path.Combine(builder.LuminoBuildDir, "MSVC2017-x64-MD", "EngineInstall", "bin", "LuminoEngine.dll"),
                    Path.Combine(gemprojDir, "ext", "LuminoEngine.dll"), true);

                Directory.SetCurrentDirectory(gemprojDir);
                Utils.CallProcessShell("bundle", "install");    // bundle.cmd
                Utils.CallProcessShell("rake", "build");
            }
        }

    }
}
