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
        public string SourceCppDir;
        public string ReleaseDir;
        public string ReleaseCppDirName;
        public string ReleaseCppDir;

        public Builder(string luminoRootDir)
        {
            LuminoRootDir = Path.GetFullPath(luminoRootDir) + "/";
            PackageRootDir = Path.Combine(LuminoRootDir, "package");
            SourceCppDir = Path.Combine(PackageRootDir, "PackageSource", "Cpp") + "/";
            ReleaseDir = Path.Combine(PackageRootDir, "ReleasePackage") + "/";
            ReleaseCppDirName = Path.Combine(ReleaseDir, "LuminoCpp_x.y.z");
            ReleaseCppDir = ReleaseCppDirName + "/";

            Directory.CreateDirectory(ReleaseCppDir);
        }
    }
}
