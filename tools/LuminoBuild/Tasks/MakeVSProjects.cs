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
        public string AdditionalOptions;
    }

    class MakeVSProjects : BuildTask
    {
        public static CMakeTargetInfo[] Targets = new CMakeTargetInfo[]
        {
            // for Native Engine
            new CMakeTargetInfo { DirName = "MSVC2017-x86-MT", VSTarget = "Visual Studio 15", Platform="Win32", MSVCStaticRuntime = "ON", AdditionalOptions="" },

            // for Nuget (LuminoCore only)
            new CMakeTargetInfo { DirName = "MSVC2017-x86-MD", VSTarget = "Visual Studio 15", Platform="Win32", MSVCStaticRuntime = "OFF", AdditionalOptions = "-DLN_BUILD_CORE_ONLY=ON" },
            new CMakeTargetInfo { DirName = "MSVC2017-x64-MD", VSTarget = "Visual Studio 15 Win64", Platform="x64", MSVCStaticRuntime = "OFF", AdditionalOptions = "-DLN_BUILD_CORE_ONLY=ON" },
            //new CMakeTargetInfo { DirName = "MSVC2017-x64-MT", VSTarget = "Visual Studio 15 Win64", Platform="x64", MSVCStaticRuntime = "ON" },
        };
        
        public override string CommandName { get { return "MakeVSProjects"; } }

        public override string Description { get { return "Make visual studio projects."; } }

        public override void Build(Builder builder)
        {

            if (Utils.IsWin32)
            {
                // cmake で .sln を作ってビルドする
                foreach (var t in Targets)
                {
                    Directory.CreateDirectory(Path.Combine(builder.LuminoBuildDir, t.DirName));
                    Directory.SetCurrentDirectory(Path.Combine(builder.LuminoBuildDir, t.DirName));

                    var installDir = Path.Combine(builder.LuminoRootDir, "build", BuildEnvironment.CMakeTargetInstallDir, t.DirName);
                    
                    var args = new string[]
                    {
                        $"-G\"{t.VSTarget}\"",
                        $"-DCMAKE_INSTALL_PREFIX=\"{installDir}\"",
                        $"-DLN_MSVC_STATIC_RUNTIME={t.MSVCStaticRuntime}",
                        $"-DLN_BUILD_TESTS=ON",
                        $"-DLN_BUILD_TOOLS=ON",
                        $"-DLN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER=ON",
                        t.AdditionalOptions,
                        $" ../..",
                    };

                    Utils.CallProcess("cmake", string.Join(' ', args));
                }
            }
            else
            {
                throw new NotImplementedException();
            }

        }
    }
}
