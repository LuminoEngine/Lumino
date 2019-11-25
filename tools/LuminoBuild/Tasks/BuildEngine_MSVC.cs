using System;
using System.IO;
using System.Linq;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class BuildEngine_MSVC : BuildTask
    {
        public override string CommandName => "BuildEngine_MSVC";
        
        public override void Build(Builder builder)
        {
            string oldCD = Directory.GetCurrentDirectory();

            if (Utils.IsWin32)
            {
                foreach (var t in MakeVSProjects.Targets)
                {
                    BuildTarget(builder, t);
                }
            }
            else
            {
                throw new NotImplementedException();
            }

            Directory.SetCurrentDirectory(oldCD);
        }

        public static void BuildTarget(Builder builder, CMakeTargetInfo target)
        {
            var targetName = target.DirName;

            Directory.SetCurrentDirectory(Path.Combine(builder.LuminoBuildDir, targetName));


            Utils.CallProcess("cmake", $"--build . --config Debug");
            Utils.CallProcess("ctest", $"-C Debug --output-on-failure");
            Utils.CallProcess("cmake", $"--build . --config Debug --target INSTALL");

            Utils.CallProcess("cmake", $"--build . --config Release");
            Utils.CallProcess("ctest", $"-C Release --output-on-failure");
            Utils.CallProcess("cmake", $"--build . --config Release --target INSTALL");
        }
    }
}
