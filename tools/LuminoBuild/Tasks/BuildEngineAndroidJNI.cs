using System;
using System.Collections.Generic;
using System.IO;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class BuildEngineAndroidJNI : BuildTask
    {
        public struct Target
        {
            public string ABI;
        }

        public List<Target> Targets = new List<Target>()
        {
            new Target() { ABI = "x86" },
            new Target() { ABI = "x86_64" },
            new Target() { ABI = "arm64-v8a" },
            new Target() { ABI = "armeabi-v7a" },
        };

        public override string CommandName { get { return "BuildEngineAndroidJNI"; } }

        public override string Description { get { return "Build Android JNI"; } }

        public override void Build(Builder builder)
        {
            string appDir = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            string sdkDir = Path.Combine(appDir, @"Android\Sdk");
            string ndk = Path.Combine(sdkDir, "ndk-bundle");
            string cmake = Path.Combine(sdkDir, @"cmake\3.6.4111459\bin\cmake.exe");
            string cmakeHomeDir = builder.LuminoRootDir;
            string cmakeToolchain = Path.Combine(sdkDir, @"ndk-bundle\build\cmake\android.toolchain.cmake");
            string cmakeMake = Path.Combine(sdkDir, @"cmake\3.6.4111459\bin\ninja.exe");
            
            string platform = "android-24";

            foreach (var target in Targets)
            {
                string abi = target.ABI;
                string cmakeBuildDir = Path.Combine(builder.LuminoBuildDir, "Android", abi);
                string cmakeOutputDir = Path.Combine(builder.LuminoBuildDir, "Package", "lib", "Android", abi);
                string buildType = "Release";
                string args = $"-H{cmakeHomeDir} -B{cmakeBuildDir} -DANDROID_ABI={abi} -DANDROID_PLATFORM={platform} -DCMAKE_LIBRARY_OUTPUT_DIRECTORY={cmakeOutputDir} -DCMAKE_BUILD_TYPE={buildType} -DANDROID_NDK={ndk} -DCMAKE_CXX_FLAGS=-std=c++14 -DCMAKE_TOOLCHAIN_FILE={cmakeToolchain} -DCMAKE_MAKE_PROGRAM={cmakeMake} -G\"Android Gradle - Ninja\"";
                
                Utils.CallProcess(cmake, args);
                Utils.CallProcess(cmake, "--build " + cmakeBuildDir);
            }
        }
    }
}
