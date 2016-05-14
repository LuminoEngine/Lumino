using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild
{
    class CppRule : ModuleRule
    {
        public override void Build(Builder builder)
        {
            Directory.CreateDirectory(builder.ReleaseCppDir);

            Console.WriteLine("copy include files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "include"), Path.Combine(builder.ReleaseCppDir, "include"), true);

            Console.WriteLine("copy Lumino.Core include files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "external/Lumino.Core/include"), Path.Combine(builder.ReleaseCppDir, "include"), true);

            Console.WriteLine("copy Lumino.Math include files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "external/Lumino.Math/include"), Path.Combine(builder.ReleaseCppDir, "include"), true);

            Console.WriteLine("copy lib files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "lib"), Path.Combine(builder.ReleaseCppDir, "lib"), true);

            Console.WriteLine("copy other files...");
            Directory.CreateDirectory(builder.ReleaseCppDir + "tools");
            File.Copy(builder.LuminoRootDir + "tools/VS2015ProjectTemplate/LuminoProject.zip", builder.ReleaseCppDir + "tools/LuminoProject.zip", true);
            File.Copy(builder.SourceCppDir + "Lumino_Install.bat", builder.ReleaseCppDir + "Lumino_Install.bat", true);
            File.Copy(builder.SourceCppDir + "Lumino_Uninstall.bat", builder.ReleaseCppDir + "Lumino_Uninstall.bat", true);

            Console.WriteLine("compressing files...");
            File.Delete(builder.ReleaseCppDirName + ".zip");
            ZipFile.CreateFromDirectory(builder.ReleaseCppDirName, builder.ReleaseCppDirName + ".zip", CompressionLevel.Optimal, true);
        }
    }
}
