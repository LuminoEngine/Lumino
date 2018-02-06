using System;
using System.IO;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class MakeXCodeProjects : BuildTask
    {
        public override string CommandName { get { return "MakeXCodeProjects"; } }

        public override string Description { get { return "Make xcode projects."; } }

        public override void Build(Builder builder)
        {
            string oldCD = Directory.GetCurrentDirectory();

            if (Utils.IsMac)
            {
                var dir = Path.Combine(builder.LuminoBuildDir, "Xcode");

                Directory.CreateDirectory(dir);
                Directory.SetCurrentDirectory(dir);
                Utils.CallProcess("cmake", string.Format("-G \"Xcode\" ../.."));
            }
            else
            {
                throw new NotImplementedException();
            }

            Directory.SetCurrentDirectory(oldCD);
        }
    }
}
