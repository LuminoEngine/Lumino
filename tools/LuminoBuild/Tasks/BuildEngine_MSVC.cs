using System;
using System.IO;
using System.Linq;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class BuildEngine_MSVC : BuildTask
    {
        public override string CommandName { get { return "BuildEngine_MSVC"; } }

        public override string Description { get { return "Build engine for C++."; } }
        
        public override void Build(Builder builder)
        {
            string oldCD = Directory.GetCurrentDirectory();

            if (Utils.IsWin32)
            {
                var list = MakeVSProjects.Targets;
                
                foreach (var t in list)
                {
                    var targetName = t.DirName + "-" + t.BuildType;

                    Directory.SetCurrentDirectory(Path.Combine(builder.LuminoBuildDir, targetName));

                    Utils.CallProcess("cmake", $"--build . --config {t.BuildType} --target INSTALL");
                    Utils.CallProcess("ctest", $"-C {t.BuildType} --output-on-failure");

                    //Utils.CallProcess("cmake", "--build . --config Release --target INSTALL");
                    //Utils.CallProcess("ctest", "-C Release --output-on-failure");
                }
            }
            else
            {
                throw new NotImplementedException();
            }

            Directory.SetCurrentDirectory(oldCD);
        }
    }
}
