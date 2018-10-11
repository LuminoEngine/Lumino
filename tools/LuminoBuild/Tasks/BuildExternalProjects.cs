using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    /*
     * build/MSVC2017-x64-MT/ の下など、各アーキテクチャフォルダの中に
     * - ExternalBuild
     * - ExternalInstall    → ライブラリのインストールフォルダ 
     * を作る。
     */
    class BuildExternalProjects : BuildTask
    {
        public override string CommandName { get { return "BuildExternalProjects"; } }

        public override string Description { get { return "BuildExternalProjects"; } }


        // (システム標準の cmake を使う系)
        private void BuildProject(Builder builder, string projectDirName, string externalSourceDir, string buildArchDir, string generator, string additionalOptions = "")
        {

            var buildDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalBuild", projectDirName));
            var installDir = Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalInstall", projectDirName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(builder.LuminoRootDir, "src", "CFlagOverrides.cmake");

            Logger.WriteLine($"BuildProject ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            Utils.CallProcess("cmake", $"-DCMAKE_DEBUG_POSTFIX=d -DCMAKE_INSTALL_PREFIX={installDir} -DCMAKE_USER_MAKE_RULES_OVERRIDE={ov} {additionalOptions} -G \"{generator}\" {cmakeSourceDir}");
            Utils.CallProcess("cmake", "--build . --config Debug");
            Utils.CallProcess("cmake", "--build . --config Debug --target install");
            Utils.CallProcess("cmake", "--build . --config Release");
            Utils.CallProcess("cmake", "--build . --config Release --target install");

            /*
                MSVC と Xcode は Debug,Release の 2 つの構成をもつプロジェクトが出力される。
            */
        }

        private void BuildProjectEm(Builder builder, string projectDirName, string externalSourceDir, string buildArchDir, string additionalOptions = "")
        {
            var buildDir = Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalBuild", projectDirName);
            var installDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalInstall", projectDirName));
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);

            Logger.WriteLine($"BuildProjectEm ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);

            var script = Path.Combine(buildDir, "build.bat");
            using (var f = new StreamWriter(script))
            {
                f.WriteLine($"cd /d \"{BuildEnvironment.EmsdkDir}\"");
                f.WriteLine($"call emsdk activate {BuildEnvironment.emsdkVer}");
                f.WriteLine($"call emsdk_env.bat");
                f.WriteLine($"cd /d \"{Utils.ToWin32Path(buildDir)}\"");
                f.WriteLine($"call emcmake cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX={installDir} {additionalOptions} -G \"MinGW Makefiles\" {cmakeSourceDir}");
                f.WriteLine($"call cmake --build .");
                f.WriteLine($"call cmake --build . --target install");
            }

            Utils.CallProcess(script); // bat の中でエラーが発生すれば、例外に乗って出てくる
        }

        private void BuildProjectAndroid(Builder builder, string projectDirName, string externalSourceDir, string abi, string additionalOptions = "")
        {
            string cmakeHomeDir = Path.Combine(externalSourceDir, projectDirName);//builder.LuminoRootDir;
            string platform = BuildEnvironment.AndroidTargetPlatform;
            
            var buildTypes = new string[]
            {
                "Debug",
                "Release",
            };
            
            foreach(var buildType in buildTypes)
            {
                var targetName = $"Android-{abi}-{buildType}";
                var cmakeBuildDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalBuild", projectDirName);
                var cmakeInstallDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalInstall", projectDirName);

                var args = new string[]
                {
                    $"-H{cmakeHomeDir}",
                    $"-B{cmakeBuildDir}",
                    $"-DLN_TARGET_ARCH_NAME={targetName}",
                    $"-DCMAKE_DEBUG_POSTFIX=d",
                    $"-DCMAKE_INSTALL_PREFIX={cmakeInstallDir}",
                    $"-DANDROID_ABI={abi}",
                    $"-DANDROID_PLATFORM={platform}",
                    $"-DCMAKE_BUILD_TYPE={buildType}",
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
            }
        }

        public override void Build(Builder builder)
        {
            var reposDir = Path.Combine(builder.LuminoBuildDir, "ExternalSource");
            Directory.CreateDirectory(reposDir);
            Directory.SetCurrentDirectory(reposDir);

            if (!Directory.Exists("ios-cmake"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b 2.0.0 https://github.com/leetal/ios-cmake.git ios-cmake");
            }
            if (!Directory.Exists("zlib"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b v1.2.11 https://github.com/madler/zlib.git zlib");
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "zlib", "CMakeLists.txt"), "zlib");
            }
            if (!Directory.Exists("libpng"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b v1.6.9 git://git.code.sf.net/p/libpng/code libpng");
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "libpng", "CMakeLists.txt"), "libpng");
            }
            if (!Directory.Exists("glslang"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b 6.2.2596 https://github.com/KhronosGroup/glslang.git glslang");
            }
            if (!Directory.Exists("SPIRV-Cross"))
            {
                Utils.CallProcess("git", "clone --progress https://github.com/KhronosGroup/SPIRV-Cross.git SPIRV-Cross");
                Directory.SetCurrentDirectory("SPIRV-Cross");
                Utils.CallProcess("git", "checkout be7425ef70231ab82930331959ab487d605d0482");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("glfw"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b 3.2.1 https://github.com/glfw/glfw.git glfw");
            }
            if (!Directory.Exists("glad"))
            {
                Utils.CopyDirectory(Path.Combine(builder.LuminoExternalDir, "glad"), "glad");
                //Utils.CallProcess("git", "clone --progress --depth 1 -b v0.1.26 https://github.com/Dav1dde/glad.git glad");
            }
            

            if (Utils.IsWin32)
            {
                // Android
                if (BuildEnvironment.AndroidStudioFound)
                {
                    foreach (var target in BuildEngine_AndroidJNI.Targets)
                    {
                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "Android-" + target.ABI, "ExternalInstall", "zlib"));
                        BuildProjectAndroid(builder, "zlib", reposDir, target.ABI);
                        BuildProjectAndroid(builder, "libpng", reposDir, target.ABI, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                    }
                }

                // Emscripten
                {
                    var externalInstallDir = Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall");
                    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall", "zlib"));

                    BuildProjectEm(builder, "zlib", reposDir, "Emscripten");
                    BuildProjectEm(builder, "libpng", reposDir, "Emscripten", $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                    BuildProjectEm(builder, "glad", reposDir, "Emscripten", "-DGLAD_INSTALL=ON");
                }

                // Visual C++
                foreach (var target in MakeVSProjects.Targets)
                {
                    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalInstall", "zlib"));

                    BuildProject(builder, "zlib", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                    BuildProject(builder, "libpng", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                    BuildProject(builder, "glslang", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                    BuildProject(builder, "SPIRV-Cross", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                    BuildProject(builder, "glfw", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=ON");
                    BuildProject(builder, "glad", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DGLAD_INSTALL=ON");
                }
            }
            else
            {
                // iOS
                {
                    var targetInfos = new []
                    {
                        new { Config = "Debug", Platform = "OS" },
                        new { Config = "Release", Platform = "OS" },
                        new { Config = "Debug", Platform = "SIMULATOR64" },
                        new { Config = "Release", Platform = "SIMULATOR64" },
                    };
                    var iOSToolchainFile = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "ExternalSource", "ios-cmake", "ios.toolchain.cmake "));
                    
                    foreach (var t in targetInfos)
                    {
                        var dirName = $"iOS-{t.Platform}-{t.Config}";
                        var args = $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\" -DIOS_PLATFORM=OS";
                        var generator = "Xcode";
                        BuildProject(builder, "libpng", reposDir, dirName, generator, args);
                    }
                }


                var targetArgs = new []
                {
                    // macOS
                    new { DirName = "macOS", Args = "" },

                    // iOS
                    //new { DirName = "iOS", Args = $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\" -DIOS_PLATFORM=OS" },
                };

                foreach (var t in targetArgs)
                {
                    var dirName = t.DirName;
                    var args = t.Args;
                    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, dirName, "ExternalInstall", "zlib"));

                    var generator = "Xcode";
                    BuildProject(builder, "zlib", reposDir, dirName, generator, args);
                    BuildProject(builder, "libpng", reposDir, dirName, generator, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include " + args);
                    BuildProject(builder, "glslang", reposDir, dirName, generator, args);
                    BuildProject(builder, "SPIRV-Cross", reposDir, dirName, generator, args);
                    BuildProject(builder, "glfw", reposDir, dirName, generator, $"-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=ON");
                    BuildProject(builder, "glad", reposDir, dirName, generator, $"-DGLAD_INSTALL=ON " + args);
                }
            }
        }
    }
}
