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
                foreach (var t in MakeVSProjects.Targets)
                {
                    if (t.MSVCStaticRuntime == "ON")
                    {
                        BuildTarget(builder, t);
                    }
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
            var targetName = target.DirName + "-" + target.BuildType;

            Directory.SetCurrentDirectory(Path.Combine(builder.LuminoBuildDir, targetName));

            Utils.CallProcess("cmake", $"--build . --config {target.BuildType} --target INSTALL");
            Utils.CallProcess("ctest", $"-C {target.BuildType} --output-on-failure");

            //Utils.CallProcess("cmake", "--build . --config Release --target INSTALL");
            //Utils.CallProcess("ctest", "-C Release --output-on-failure");
        }
    }
}
