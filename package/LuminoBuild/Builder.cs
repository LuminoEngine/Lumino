using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace LuminoBuild
{
    class Builder
    {
        /// <summary>
        /// MSBuildのパス
        /// </summary>
        private const string MSBuildPath = @"C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe";

        public string LuminoRootDir;
        public string PackageRootDir;
        public string SourceCppDir;
        public string ReleaseDir;
        public string ReleaseCppDirName;
        public string ReleaseCppDir;

        public Builder(string luminoRootDir)
        {
            LuminoRootDir = Path.GetFullPath(luminoRootDir) + "/";
            PackageRootDir = Path.Combine(LuminoRootDir, "package");
            SourceCppDir = Path.Combine(PackageRootDir, "PackageSource", "Cpp") + "/";
            ReleaseDir = Path.Combine(PackageRootDir, "ReleasePackage") + "/";
            ReleaseCppDirName = Path.Combine(ReleaseDir, "LuminoCpp_x.y.z");
            ReleaseCppDir = ReleaseCppDirName + "/";

            Directory.CreateDirectory(ReleaseCppDir);
        }

        public void BuildEngine()
        {
            string buildDir = LuminoRootDir + "build_msvc140x86u_MT";
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            Utils.CallProcess("cmake", "-DLN_USE_UNICODE_CHAR_SET=ON -DLN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME=ON ..");
            Utils.CallProcess(MSBuildPath, "/t:Rebuild /p:Configuration=Debug /p:Platform=Win32 /m Lumino.sln");
            Utils.CallProcess(MSBuildPath, "/t:Rebuild /p:Configuration=Release /p:Platform=Win32 /m Lumino.sln");
        }
    }
}
