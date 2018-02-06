using System;
using System.IO;
using System.Text;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class CMakeTargetInfo
    {
        public string DirName;
        public string VSTarget;
        public string Unicode;
        public string Platform;
        public string MSVCStaticRuntime;
    }

    class MakeVSProjects : BuildTask
    {
        public static CMakeTargetInfo[] Targets = new CMakeTargetInfo[]
        {
            //new CMakeTargetInfo { DirName = "MSVC2013-x86-MT", VSTarget = "Visual Studio 12", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "ON" },
            //new CMakeTargetInfo { DirName = "MSVC2013-x86-MD", VSTarget = "Visual Studio 12", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "OFF" },
            new CMakeTargetInfo { DirName = "MSVC2015-x86-MT", VSTarget = "Visual Studio 14", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "ON" },
            new CMakeTargetInfo { DirName = "MSVC2015-x86-MD", VSTarget = "Visual Studio 14", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "OFF" },
            new CMakeTargetInfo { DirName = "MSVC2017-x86-MT", VSTarget = "Visual Studio 15", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "ON" },
            new CMakeTargetInfo { DirName = "MSVC2017-x86-MD", VSTarget = "Visual Studio 15", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "OFF" },
        };
        
        public override string CommandName { get { return "MakeVSProjects"; } }

        public override string Description { get { return "Make visual studio projects."; } }

        public override void Build(Builder builder)
        {
            string oldCD = Directory.GetCurrentDirectory();

            if (Utils.IsWin32)
            {
                // cmake で .sln を作ってビルドする
                foreach (var t in Targets)
                {
                    Directory.CreateDirectory(builder.LuminoBuildDir + t.DirName);
                    Directory.SetCurrentDirectory(builder.LuminoBuildDir + t.DirName);
                    Utils.CallProcess("cmake", string.Format("-G\"{0}\" -DLN_USE_UNICODE_CHAR_SET={1} -DLN_MSVC_STATIC_RUNTIME={2} ../..", t.VSTarget, t.Unicode, t.MSVCStaticRuntime));
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
