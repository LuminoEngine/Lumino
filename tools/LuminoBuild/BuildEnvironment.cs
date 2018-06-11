using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild
{
    class BuildEnvironment
    {
        public static string AndroidSdkRootDir { get; set; }
        public static string AndroidSdkCMake { get; set; }
        public static string AndroidSdkNinja { get; set; }
        public static string AndroidNdkRootDir { get; set; }
        public static string AndroidCMakeToolchain { get; set; }

        public static void Initialize()
        {
            string appDir = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            AndroidSdkRootDir = Path.Combine(appDir, @"Android\Sdk");

            AndroidSdkCMake = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\cmake.exe");
            AndroidSdkNinja = Path.Combine(AndroidSdkRootDir, @"cmake\3.6.4111459\bin\ninja.exe");

            AndroidNdkRootDir = Path.Combine(AndroidSdkRootDir, "ndk-bundle");
            AndroidCMakeToolchain = Path.Combine(AndroidNdkRootDir, @"build\cmake\android.toolchain.cmake");
        }
    }
}
