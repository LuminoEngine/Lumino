using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakePackage_Ruby : BuildTask
    {
        public override string CommandName => "MakePackage_Ruby";

        public override void Build(Build builder)
        {
            if (Utils.IsWin32)
            {
                var gemprojDir = Path.Combine(builder.LuminoToolsDir, "Bindings", "Ruby", "GemProject");

                //File.Copy(
                //    Path.Combine(builder.LuminoRootDir, "build/MSVC2019-x64-MT/EngineInstall/bin/LuminoEngine.dll"),
                //    Path.Combine(gemprojDir, "ext", "LuminoEngine.dll"));

                using (CurrentDir.Enter(gemprojDir))
                {
                    Utils.CallProcessShell("bundle", "install");    // bundle.cmd
                    Utils.CallProcessShell("rake", "build");
                    Utils.CallProcessShell("gem", "install " + Directory.EnumerateFiles("pkg", "*.gem").First());
                }
            }
        }

    }
}
