using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakePackage_HSP3 : BuildTask
    {
        public override string CommandName => "MakePackage_HSP3";

        public override void Build(Builder builder)
        {
            if (Utils.IsWin32)
            {
                var srcDir = Path.Combine(builder.LuminoToolsDir, "Bindings", "HSP3");
                var packageDir = Path.Combine(builder.LuminoToolsDir, "Bindings", "HSP3", "_package", "LuminoHSP3-latest");
                Directory.CreateDirectory(packageDir);

                Utils.CopyFile(Path.Combine(srcDir, "lumino.as"), packageDir);
                Utils.CopyFile(Path.Combine(srcDir, "lumino.hs"), packageDir);
                //Utils.CopyFile(Path.Combine(builder.LuminoBuildDir, "MSVC2019-x86-MT", "tools", "Bindings", "HSP3", "Release", "LuminoHSP3.dll"), packageDir);
                Utils.CopyFile(Path.Combine(builder.LuminoBuildDir, "NativePackage", "Engine", "MSVC2019-x86-MT", "bin", "LuminoHSP3.dll"), packageDir);
                Utils.CopyFile(Path.Combine(srcDir, "README_HSP3.txt"), packageDir);
                Utils.CopyDirectory(Path.Combine(srcDir, "samples"), Path.Combine(packageDir, "samples"));
                MakeNativePackage.GenerateReadme(builder, packageDir);
            }
        }

    }
}
