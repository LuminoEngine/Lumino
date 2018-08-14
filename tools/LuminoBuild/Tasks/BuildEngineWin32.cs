using System;
using System.IO;
using System.Linq;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class BuildEngineWin32 : BuildTask
    {
        public override string CommandName { get { return "BuildEngineWin32"; } }

        public override string Description { get { return "Build engine for C++."; } }
        
        public override void Build(Builder builder)
        {
            string oldCD = Directory.GetCurrentDirectory();

            if (Utils.IsWin32)
            {
                var list = MakeVSProjects.Targets;
                
                foreach (var t in list)
                {
                    Directory.SetCurrentDirectory(Path.Combine(builder.LuminoBuildDir, t.DirName));
                    Utils.CallProcess("cmake", "--build . --config Debug --target INSTALL");
                    Utils.CallProcess("ctest", "-C Debug");

                    Utils.CallProcess("cmake", "--build . --config Release --target INSTALL");
                    Utils.CallProcess("ctest", "-C Release");
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
