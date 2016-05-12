using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace LuminoBuild
{
    class Builder
    {
        public string LuminoRootDir;
        public string PackageRootDir;
        public string ReleaseDir;
        public string ReleaseCppDir;

        public Builder(string luminoRootDir)
        {
            LuminoRootDir = Path.GetFullPath(luminoRootDir);
            PackageRootDir = Path.Combine(LuminoRootDir, "package");
            ReleaseDir = Path.Combine(PackageRootDir, "ReleasePackage");
            ReleaseCppDir = Path.Combine(ReleaseDir, "Cpp");

            Directory.CreateDirectory(ReleaseCppDir);
        }
    }
}
