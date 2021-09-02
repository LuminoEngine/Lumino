using System;
using System.Collections.Generic;
using System.IO;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class BuildEngine_Android : BuildTask
    {
        public override string CommandName => "BuildEngine_Android";

        public struct Target
        {
            public string ABI;
            public string BuildType;
        }

        public static string[] TargetABIs = new string[]
        {
            "x86",
            "x86_64",
            "arm64-v8a",
            "armeabi-v7a",
        };

        public static string[] Configurations = new string[] {
            //"Debug",
            "Release",
        };


        public override void Build(Builder builder)
        {
            string cmakeHomeDir = builder.LuminoRootDir;
            string platform = AndoridBuildEnv.AndroidTargetPlatform;

            foreach (var abi in TargetABIs)
            {
                foreach (var config in Configurations)
                {
                    var targetName = $"Android-{abi}";
                    var targetDir = Path.Combine(builder.LuminoBuildDir, targetName);
                    if (Directory.Exists(targetDir))
                    {
                        var cmakeBuildDir = Path.Combine(targetDir, "EngineBuild", config);
                        var cmakeInstallDir = Path.Combine(targetDir, BuildEnvironment.EngineInstallDirName);

                        var args = new string[]
                        {
                            // Basic options https://developer.android.com/ndk/guides/cmake.html
                            $"-H{cmakeHomeDir}",
                            $"-B{cmakeBuildDir}",
                            //$"-G\"Android Gradle - Ninja\"",
                            $"-G\"Ninja\"",
                            $"-DANDROID_ABI={abi}",
                            $"-DANDROID_NDK={AndoridBuildEnv.AndroidNdkRootDir}",
                            $"-DCMAKE_BUILD_TYPE={config}",
                            $"-DCMAKE_MAKE_PROGRAM={AndoridBuildEnv.AndroidSdkNinja}",
                            $"-DCMAKE_TOOLCHAIN_FILE={AndoridBuildEnv.AndroidCMakeToolchain}",
                            
                            // Lumino required
                            $"-DCMAKE_DEBUG_POSTFIX=d",
                            $"-DCMAKE_INSTALL_PREFIX={cmakeInstallDir}",
                            $"-DANDROID_PLATFORM={platform}",
                            //$"-DCMAKE_CXX_FLAGS=-std=c++14",
                            $"-DANDROID_STL=c++_shared",
                            $"-DANDROID_NATIVE_API_LEVEL=26",
                            $"-DLN_TARGET_ARCH={targetName}",
                            $"-DLN_BUILD_TESTS=OFF",
                            $"-DLN_BUILD_TOOLS=OFF",
                        };

                        Utils.CallProcess(AndoridBuildEnv.AndroidSdkCMake, string.Join(' ', args));
                        Utils.CallProcess(AndoridBuildEnv.AndroidSdkCMake, "--build " + cmakeBuildDir);
                        Utils.CallProcess(AndoridBuildEnv.AndroidSdkCMake, "--build " + cmakeBuildDir + " --target install");

                        //Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "ImportExternalLibraries.cmake"), cmakeInstallDir);
                    }
                }
                
            }
        }
    }
}
