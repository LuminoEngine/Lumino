using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class CompressPackage : BuildTask
    {
        public override string CommandName => "CompressPackage";

        public override void Build(Builder builder)
        {
            string localPackage = Path.Combine(builder.LuminoBuildDir, builder.LocalPackageName);
            string releasePackage = Path.Combine(builder.LuminoBuildDir, builder.ReleasePackageName);

            // rename
            Directory.Move(localPackage, releasePackage);

            var zipPath = Path.Combine(builder.LuminoBuildDir, builder.ReleasePackageName + ".zip");
            Utils.CreateZipFile(releasePackage, zipPath, true);

            // undo, rename
            Directory.Move(releasePackage, localPackage);

            Console.WriteLine(zipPath);
        }
    }
}
