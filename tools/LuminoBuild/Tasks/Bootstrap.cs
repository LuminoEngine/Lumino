using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using LuminoBuild;


namespace LuminoBuild.Tasks
{
    class Bootstrap : BuildTask
    {
        public override string CommandName => "Bootstrap";

        public override string[] GetDepends(Build b)
        {
            var depends = new List<string>();
            depends.Add("BuildExternals");
            depends.Add("BuildEngine");

            return depends.ToArray();
        }

        public override void Build(Build b)
        {
            if (Utils.IsWin32)
            {
                Utils.CopyFile(Path.Combine(
                    b.EngineInstallDir, "bin", "lumino.exe"),
                    b.RootDir);
            }
        }
    }
}
