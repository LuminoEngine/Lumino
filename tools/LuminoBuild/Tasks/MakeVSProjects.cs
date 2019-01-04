using System;
using System.IO;
using System.Text;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class CMakeTargetInfo
    {
        public string DirName;
        public string BuildType;
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
            //new CMakeTargetInfo { DirName = "MSVC2017-x86-MT", BuildType = "Debug", VSTarget = "Visual Studio 15", Platform="Win32", MSVCStaticRuntime = "ON", AdditionalOptions="" },
            new CMakeTargetInfo { DirName = "MSVC2017-x86-MT", BuildType = "Release", VSTarget = "Visual Studio 15", Platform="Win32", MSVCStaticRuntime = "ON", AdditionalOptions="" },

            // for Nuget (LuminoCore only)
            //new CMakeTargetInfo { DirName = "MSVC2017-x86-MD", BuildType = "Debug", VSTarget = "Visual Studio 15", Platform="Win32", MSVCStaticRuntime = "OFF", AdditionalOptions = "-DLN_BUILD_CORE_ONLY=ON" },
            //new CMakeTargetInfo { DirName = "MSVC2017-x86-MD", BuildType = "Release", VSTarget = "Visual Studio 15", Platform="Win32", MSVCStaticRuntime = "OFF", AdditionalOptions = "-DLN_BUILD_CORE_ONLY=ON" },
            //new CMakeTargetInfo { DirName = "MSVC2017-x64-MD", BuildType = "Debug", VSTarget = "Visual Studio 15 Win64", Platform="x64", MSVCStaticRuntime = "OFF", AdditionalOptions = "-DLN_BUILD_CORE_ONLY=ON" },
            //new CMakeTargetInfo { DirName = "MSVC2017-x64-MD", BuildType = "Release", VSTarget = "Visual Studio 15 Win64", Platform="x64", MSVCStaticRuntime = "OFF", AdditionalOptions = "-DLN_BUILD_CORE_ONLY=ON" },
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
                    var targetName = t.DirName + "-" + t.BuildType;

                    Directory.CreateDirectory(Path.Combine(builder.LuminoBuildDir, targetName));
                    Directory.SetCurrentDirectory(Path.Combine(builder.LuminoBuildDir, targetName));

                    var installDir = Path.Combine(builder.LuminoRootDir, "build", BuildEnvironment.CMakeTargetInstallDir, targetName);
                    
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
