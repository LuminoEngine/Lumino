using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;

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

        }

        public void BuildEngine()
        {
            string buildDir = LuminoRootDir + "build_msvc140x86u_MT";
            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            Utils.CallProcess("cmake", "-DLN_USE_UNICODE_CHAR_SET=ON -DLN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME=ON ..");
            Utils.CallProcess(MSBuildPath, "/p:Configuration=Debug /p:Platform=Win32 /m Lumino.sln");   // /t:Rebuild 
            Utils.CallProcess(MSBuildPath, "/p:Configuration=Release /p:Platform=Win32 /m Lumino.sln"); // /t:Rebuild 

            // ビルド後、DLL ができてからバージョンを取る
            ReleaseCppDirName = Path.Combine(ReleaseDir, "LuminoCpp_" + GetLibararyVersion());
            ReleaseCppDir = ReleaseCppDirName + "/";

            Directory.CreateDirectory(ReleaseCppDir);
        }




        [DllImport("../../../../lib/LuminoC_x86uMTd", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private extern static void LNVersion_GetString(out IntPtr outStr);

        [DllImport("../../../../lib/LuminoC_x86uMTd", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private extern static void LCSInternal_GetIntPtrStringLength(IntPtr str, out int len);

        [DllImport("../../../../lib/LuminoC_x86uMTd", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        private extern static void LCSInternal_GetIntPtrString(IntPtr str, StringBuilder buf);

        public static string GetLibararyVersion()
        {
            IntPtr str;
            int len = 0;
            LNVersion_GetString(out str);
            LCSInternal_GetIntPtrStringLength(str, out len);
            var b = new StringBuilder(len + 1);
            LCSInternal_GetIntPtrString(str, b);
            return b.ToString();
        }
    }
}
