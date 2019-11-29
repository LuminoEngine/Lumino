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
                File.Copy(Path.Combine(builder.LuminoBuildDir, "MSVC2017-x64-MT", "EngineInstall", "bin", "LuminoEngine.dll"));
            }
        }

    }
}
