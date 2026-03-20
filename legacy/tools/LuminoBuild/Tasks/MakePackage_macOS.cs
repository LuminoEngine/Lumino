using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakePackage_macOS : BuildTask
    {
        public override string CommandName => "MakePackage_macOS";

        public override void Build(Build builder)
        {
            var orgName = Path.Combine(builder.BuildDir, builder.LocalPackageName);
            var tmpName = Path.Combine(builder.BuildDir, builder.ReleasePackageName);
            Directory.Move(orgName, tmpName);
            if (!BuildEnvironment.FromCI)
            {
                Utils.CreateZipFile(tmpName, tmpName + ".zip");
                Directory.Move(tmpName, orgName);
            }
        }
    }
}
