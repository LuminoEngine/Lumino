using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class CompressPackage : BuildTask
    {
        public override string CommandName => "CompressPackage";

        public override void Build(Build builder)
        {
            string localPackage = Path.Combine(builder.BuildDir, builder.LocalPackageName);
            string releasePackage = Path.Combine(builder.BuildDir, builder.ReleasePackageName);

            // rename
            Directory.Move(localPackage, releasePackage);

            var zipPath = Path.Combine(builder.BuildDir, builder.ReleasePackageName + ".zip");
            Utils.CreateZipFile(releasePackage, zipPath, true);

            // undo, rename
            Directory.Move(releasePackage, localPackage);

            Console.WriteLine(zipPath);
        }
    }
}
