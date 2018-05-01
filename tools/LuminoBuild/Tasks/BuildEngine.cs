using System;
using System.IO;
using System.Linq;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class BuildEngine : BuildTask
    {
        public override string CommandName { get { return "BuildEngine"; } }

        public override string Description { get { return "Build engine for C++."; } }
        
        public override void Build(Builder builder)
        {
            string oldCD = Directory.GetCurrentDirectory();

            if (Utils.IsWin32)
            {
                var list = MakeVSProjects.Targets;
                
                foreach (var t in list)
                {
                    Directory.SetCurrentDirectory(builder.LuminoBuildDir + t.DirName);
                    Utils.CallProcess("cmake", "--build . --config Debug --target INSTALL");
                    Utils.CallProcess("cmake", "--build . --config Release --target INSTALL");
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
