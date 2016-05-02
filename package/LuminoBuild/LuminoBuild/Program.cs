using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild
{
    class Program
    {
        public const string LuminoRootDir = "../../../../../";
        public const string PackageRootDir = LuminoRootDir + "package/";
        public const string ReleaseDir = LuminoRootDir + "package/Release";

        public const string ReleaseCppDir = LuminoRootDir + "package/Release/Cpp";

        static void Main(string[] args)
        {
            Directory.CreateDirectory(ReleaseCppDir);

            Utils.CopyDirectory(Path.Combine(LuminoRootDir, "include"), Path.Combine(ReleaseCppDir, "include"), true);
            Utils.CopyDirectory(Path.Combine(LuminoRootDir, "external/Lumino.Core/include"), Path.Combine(ReleaseCppDir, "include"), true);
            Utils.CopyDirectory(Path.Combine(LuminoRootDir, "external/Lumino.Math/include"), Path.Combine(ReleaseCppDir, "include"), true);
            Utils.CopyDirectory(Path.Combine(LuminoRootDir, "lib"), Path.Combine(ReleaseCppDir, "lib"), true);
        }
    }
}
