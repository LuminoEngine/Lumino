using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild
{
    class CppRule : ModuleRule
    {
        public override void Build(Builder builder)
        {
            Console.WriteLine("copy include files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "include"), Path.Combine(builder.ReleaseCppDir, "include"), true);
            Console.WriteLine("copy Lumino.Core include files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "external/Lumino.Core/include"), Path.Combine(builder.ReleaseCppDir, "include"), true);
            Console.WriteLine("copy Lumino.Math include files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "external/Lumino.Math/include"), Path.Combine(builder.ReleaseCppDir, "include"), true);
            Console.WriteLine("copy lib files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "lib"), Path.Combine(builder.ReleaseCppDir, "lib"), true);
        }
    }
}
