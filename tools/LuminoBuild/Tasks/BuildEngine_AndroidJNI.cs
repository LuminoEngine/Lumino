using System;
using System.Collections.Generic;
using System.IO;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class BuildEngine_AndroidJNI : BuildTask
    {
        public struct Target
        {
            public string ABI;
            public string BuildType;
        }

        public static List<Target> Targets = new List<Target>()
        {
            new Target() { ABI = "x86", BuildType = "Debug" },
            new Target() { ABI = "x86", BuildType = "Release" },
            new Target() { ABI = "x86_64", BuildType = "Debug" },
            new Target() { ABI = "x86_64", BuildType = "Release" },
            new Target() { ABI = "arm64-v8a", BuildType = "Debug" },
            new Target() { ABI = "arm64-v8a", BuildType = "Release" },
            new Target() { ABI = "armeabi-v7a", BuildType = "Debug" },
            new Target() { ABI = "armeabi-v7a", BuildType = "Release" },
        };

        public override string CommandName { get { return "BuildEngine_AndroidJNI"; } }

        public override string Description { get { return "Build Android JNI"; } }

        public override void Build(Builder builder)
        {
            string cmakeHomeDir = builder.LuminoRootDir;
            string platform = BuildEnvironment.AndroidTargetPlatform;

            foreach (var target in Targets)
            {
                string abi = target.ABI;
                
                var targetName = $"Android-{abi}-{target.BuildType}";
                string cmakeBuildDir = Path.Combine(builder.LuminoBuildDir, targetName);
                string cmakeInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir, targetName);

                var args = new string[]
                {
                    $"-H{cmakeHomeDir}",
                    $"-B{cmakeBuildDir}",
                    $"-DLN_BUILD_TESTS=OFF",
                    $"-DLN_BUILD_TOOLS=OFF",
                    $"-DLN_TARGET_ARCH_NAME={targetName}",
                    $"-DCMAKE_DEBUG_POSTFIX=d",
                    $"-DCMAKE_INSTALL_PREFIX={cmakeInstallDir}",
                    $"-DANDROID_ABI={abi}",
                    $"-DANDROID_PLATFORM={platform}",
                    $"-DCMAKE_BUILD_TYPE={target.BuildType}",
                    $"-DANDROID_NDK={BuildEnvironment.AndroidNdkRootDir}",
                    $"-DCMAKE_CXX_FLAGS=-std=c++14",
                    $"-DANDROID_STL=c++_shared",
                    $"-DCMAKE_TOOLCHAIN_FILE={BuildEnvironment.AndroidCMakeToolchain}",
                    $"-DCMAKE_MAKE_PROGRAM={BuildEnvironment.AndroidSdkNinja}",
                    $"-G\"Android Gradle - Ninja\"",
                };

                Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, string.Join(' ', args));
                Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, "--build " + cmakeBuildDir);
                Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, "--build " + cmakeBuildDir + " --target install");

                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "ImportExternalLibraries.cmake"), cmakeInstallDir);
                
            }
        }
    }
}
