using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakePackage_macOS : BuildTask
    {
        public override string CommandName => "MakePackage_macOS";

        public override void Build(Builder builder)
        {
            var orgName = Path.Combine(builder.LuminoBuildDir, builder.LocalPackageName);
            var tmpName = Path.Combine(builder.LuminoBuildDir, builder.ReleasePackageName);
            Directory.Move(orgName, tmpName);
            if (!BuildEnvironment.FromCI)
            {
                Utils.CreateZipFile(tmpName, tmpName + ".zip");
                Directory.Move(tmpName, orgName);
            }
        }
    }
}
