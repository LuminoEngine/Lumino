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
    [Obsolete]
    class BuildExternalProjects : BuildTask
    {
        public override string CommandName => "BuildExternalProjects";


        // (システム標準の cmake を使う系)
        private void BuildProject(Build builder, string projectDirName, string configuration, string externalSourceDir, string buildArch, string generator, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName); // zlib/contrib/minizip のような場合に minizip だけ取り出す
            var buildDir = builder.GetExternalProjectBuildDir(buildArch, projectName);
            var installDir = builder.GetExternalProjectInstallDir(buildArch, projectName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(builder.RootDir, "src", "CFlagOverrides.cmake");

            Logger.WriteLine($"BuildProject ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            
            var args = new string[]
            {
                $"-DCMAKE_INSTALL_PREFIX={installDir}",
                $"-DCMAKE_DEBUG_POSTFIX:STRING=d",     // cmake の find_package で Debug/Release 両対応するために、同じフォルダに lib を入れておきたい。(Qt 参考)
                $"-DCMAKE_USER_MAKE_RULES_OVERRIDE:STRING={ov}",
                $"{additionalOptions}",
                $"-G \"{generator}\"",
                $"{cmakeSourceDir}",
            };
            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build . --config {configuration}");
            Utils.CallProcess("cmake", $"--build . --config {configuration} --target install");

            if (string.IsNullOrEmpty(configuration) || configuration.Contains("Debug"))
            {
                Utils.CallProcess("cmake", $"--build . --config Debug");
                Utils.CallProcess("cmake", $"--build . --config Debug --target install");
            }
            if (string.IsNullOrEmpty(configuration) || configuration.Contains("Release"))
            {
                Utils.CallProcess("cmake", $"--build . --config Release");
                Utils.CallProcess("cmake", $"--build . --config Release --target install");
            }
        }

        private void BuildProjectMSVC(Build b, string projectDirName, string externalSourceDir, string targetName, string targetFullName, string configuration, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName); // zlib/contrib/minizip のような場合に minizip だけ取り出す
            var buildDir = b.GetExternalProjectBuildDir(targetFullName, projectName);
            var installDir = b.GetExternalProjectInstallDir(targetFullName, projectName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(b.RootDir, "src", "CFlagOverrides.cmake");

            var targetInfo = BuildEngine.TargetInfoMap[b.Triplet];

            Logger.WriteLine($"BuildProject ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            var localAdditional = "";
            if (!string.IsNullOrEmpty(targetInfo.Arch))
            {
                localAdditional += "-A " + targetInfo.Arch;
            }

            var args = new string[]
            {
                $"-DCMAKE_INSTALL_PREFIX={installDir}",
                $"-DCMAKE_DEBUG_POSTFIX:STRING=d",     // cmake の find_package で Debug/Release 両対応するために、同じフォルダに lib を入れておきたい。(Qt 参考)
                $"-DCMAKE_USER_MAKE_RULES_OVERRIDE:STRING={ov}",
                $"-DLN_MSVC_STATIC_RUNTIME:BOOL={targetInfo.StaticRuntime}",
                $"{localAdditional}",
                $"{additionalOptions}",
                $"-G \"{targetInfo.Generator}\"",
                $"{cmakeSourceDir}",
            };
            Utils.CallProcess("cmake", string.Join(' ', args));

            if (string.IsNullOrEmpty(configuration) || targetName.Contains("Debug"))
            {
                Utils.CallProcess("cmake", $"--build . --config Debug");
                Utils.CallProcess("cmake", $"--build . --config Debug --target install");
            }
            if (string.IsNullOrEmpty(configuration) || targetName.Contains("Release"))
            {
                Utils.CallProcess("cmake", $"--build . --config Release");
                Utils.CallProcess("cmake", $"--build . --config Release --target install");
            }
        }

        private void BuildProjectEm(Build builder, string projectDirName, string externalSourceDir, string buildArchDir, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName);
            var buildDir = builder.GetExternalProjectBuildDir(buildArchDir, projectName);
            var installDir = builder.GetExternalProjectInstallDir(buildArchDir, projectName);
            //var installDir = Utils.ToUnixPath(Path.Combine(EmscriptenBuildEnv.EmscriptenSysRootLocal, projectName));
            var cmakeSourceDir = Utils.ToUnixPath(Path.Combine(externalSourceDir, projectDirName));
            var ov = Path.Combine(builder.RootDir, "src", "CFlagOverrides.cmake");

            Logger.WriteLine($"BuildProjectEm ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);

            var script = Path.Combine(buildDir, "build.bat");
            using (var f = new StreamWriter(script))
            {
                f.WriteLine($"cd /d \"{EmscriptenBuildEnv.EmsdkDir}\"");
                f.WriteLine($"call emsdk activate {EmscriptenBuildEnv.emsdkVer}");
                f.WriteLine($"call emsdk_env.bat");
                f.WriteLine($"cd /d \"{Utils.ToWin32Path(buildDir)}\"");
                f.WriteLine($"call emcmake cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX={installDir} -DCMAKE_USER_MAKE_RULES_OVERRIDE:STRING={ov} {additionalOptions} -G \"MinGW Makefiles\" {cmakeSourceDir}");
                f.WriteLine($"call cmake --build . -j8");
                f.WriteLine($"call cmake --build . --target install");
            }

            Utils.CallProcess(script); // bat の中でエラーが発生すれば、例外に乗って出てくる

            // emcmake で find_library などを行う場合、Emscripten のシステムフォルダ以外は検索しないようにツールチェインファイルで封印されている。
            // Lumino 本体のビルド時にライブラリを探すことができるようにするため、システムフォルダに一式コピーしておく。
            Utils.CopyDirectory(installDir, Path.Combine(EmscriptenBuildEnv.EmscriptenSysRootLocal, "ExternalInstall", projectName));
        }

        private void BuildProjectAndroid(Build builder, string projectDirName, string externalSourceDir, string targetName, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName);
            string cmakeHomeDir = Path.Combine(externalSourceDir, projectDirName);//builder.LuminoRootDir;
            string platform = AndoridBuildEnv.AndroidTargetPlatform;
            var installDir = builder.GetExternalProjectInstallDir(targetName, projectName);

            foreach (var config in new string[] { /*"Debug",*/ "Release" })
            {
                //var targetName = $"Android-{abi}-{buildType}";
                var buildDir = Path.Combine(builder.GetExternalProjectBuildDir(targetName, projectName), config);

                var args = new string[]
                {
                    // Basic options https://developer.android.com/ndk/guides/cmake.html
                    $"-H{cmakeHomeDir}",
                    $"-B{buildDir}",
                    //$"-G\"Android Gradle - Ninja\"",
                    $"-G\"Ninja\"",
                    $"-DANDROID_ABI={AndoridBuildEnv.GetABIFromTargetName(targetName)}",
                    $"-DANDROID_NDK={AndoridBuildEnv.AndroidNdkRootDir}",
                    $"-DCMAKE_BUILD_TYPE={config}",
                    $"-DCMAKE_MAKE_PROGRAM={AndoridBuildEnv.AndroidSdkNinja}",
                    $"-DCMAKE_TOOLCHAIN_FILE={AndoridBuildEnv.AndroidCMakeToolchain}",
                    $"-DANDROID_TOOLCHAIN=clang",

                    // Lumino required
                    $"-DCMAKE_INSTALL_PREFIX={installDir}",
                    $"-DCMAKE_DEBUG_POSTFIX=d",
                    $"-DANDROID_PLATFORM={platform}",
                    $"-DANDROID_STL=c++_shared",
                    $"-DANDROID_NATIVE_API_LEVEL=26",
                    $"-DLN_TARGET_ARCH={targetName}",
                    additionalOptions,
                };
#if false
                var args = new string[]
                {
                    $"-H{cmakeHomeDir}",
                    $"-B{buildDir}",
                    $"-DLN_TARGET_ARCH={targetName}",
                    $"-DCMAKE_INSTALL_PREFIX={installDir}",
                    $"-DCMAKE_DEBUG_POSTFIX=d",
                    $"-DANDROID_ABI={AndoridBuildEnv.GetABIFromTargetName(targetName)}",
                    $"-DANDROID_PLATFORM={platform}",
                    $"-DCMAKE_BUILD_TYPE={config}",
                    $"-DANDROID_NDK={AndoridBuildEnv.AndroidNdkRootDir}",
                    //$"-DCMAKE_CXX_FLAGS=-std=c++14",
                    $"-DANDROID_STL=c++_shared",
                    $"-DCMAKE_TOOLCHAIN_FILE={AndoridBuildEnv.AndroidCMakeToolchain}",
                    //$"-DCMAKE_MAKE_PROGRAM={AndoridBuildEnv.AndroidSdkNinja}",
                    $"-DANDROID_NATIVE_API_LEVEL=26",
                    additionalOptions,
                    //$"-G\"Android Gradle - Ninja\"",
                    $"-G\"Unix Makefile\"",
                };
#endif

                Utils.CallProcess(AndoridBuildEnv.AndroidSdkCMake, string.Join(' ', args));
                Utils.CallProcess(AndoridBuildEnv.AndroidSdkCMake, $"--build {buildDir} ");
                Utils.CallProcess(AndoridBuildEnv.AndroidSdkCMake, $"--build {buildDir} --target install");
            }
        }

        public override void Build(Build builder)
        {
            if (builder.ExistsCache(builder.GetExternalInstallDir(BuildEnvironment.Target)))
            {
                Logger.WriteLine("BuildExternalProjects has cache.");
                return;
            }

            var reposDir = Path.Combine(builder.BuildDir, "ExternalSource");
            Directory.CreateDirectory(reposDir);
            Directory.SetCurrentDirectory(reposDir);

            if (!Directory.Exists("ios-cmake"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b 3.0.1 https://github.com/leetal/ios-cmake.git ios-cmake");
            }
            if (!Directory.Exists("SDL2"))
            {
                var zip = Path.Combine(reposDir, "SDL2-2.0.12.zip");
                using (var wc = new System.Net.WebClient())
                {
                    wc.DownloadFile("https://www.libsdl.org/release/SDL2-2.0.12.zip", zip);
                }

                Utils.ExtractZipFile(zip, Path.Combine(reposDir, "SDL2-2.0.12"));
                var dst = Path.Combine(reposDir, "SDL2-2.0.12", "SDL2-2.0.12");
                Utils.CopyDirectory(dst, Path.Combine(reposDir, "SDL2"));

                //System.Threading.Thread.Sleep(1000);
                //var dst = Path.Combine(reposDir, "SDL2-2.0.12", "SDL2-2.0.12");
                //var dir = new DirectoryInfo(dst);
                //dir.Attributes = dir.Attributes & ~FileAttributes.ReadOnly;
                //Directory.Move(dst, Path.Combine(reposDir, "SDL2"));
            }
            if (!Directory.Exists("tmxlite"))
            {
                Utils.CallProcess("git", "clone https://github.com/fallahn/tmxlite.git tmxlite");
                Directory.SetCurrentDirectory("tmxlite");
                Utils.CallProcess("git", "checkout 8ed41071fe0774947fc7f7c6ece77de3061a5239");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("Effekseer"))
            {
                Utils.CallProcess("git", "clone https://github.com/effekseer/Effekseer Effekseer");
                Directory.SetCurrentDirectory("Effekseer");
                Utils.CallProcess("git", "submodule update --init");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("lua"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b v5.4.0 https://github.com/lua/lua.git");
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "lua", "CMakeLists.txt"), "lua");
            }
            if (!Directory.Exists("nanovg"))
            {
                Utils.CallProcess("git", "clone https://github.com/memononen/nanovg.git");
                using (CurrentDir.Enter("nanovg"))
                {
                    Utils.CallProcess("git", "checkout c35e80c3fed7445b4e2973fccccc89afd97834cf");
                }

                // TODO: https://github.com/memononen/nanovg/pull/565 のマージ待ち
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "nanovg", "CMakeLists.txt"), "nanovg");
            }
            if (!BuildEnvironment.FromCI && !Directory.Exists("glTF-Sample-Models"))
            {
                Utils.CallProcess("git", "clone https://github.com/KhronosGroup/glTF-Sample-Models");
            }

            const string bulletOptions = "-DBUILD_BULLET2_DEMOS=OFF -DBUILD_CLSOCKET=OFF -DBUILD_CPU_DEMOS=OFF -DBUILD_ENET=OFF -DBUILD_EXTRAS=OFF -DBUILD_OPENGL3_DEMOS=OFF -DBUILD_UNIT_TESTS=OFF -DINSTALL_LIBS=ON";


            if (Utils.IsWin32)
            {
                // Visual C++
                if (BuildEnvironment.IsMSVCTarget)
                {
                    var targetName = BuildEnvironment.Target;
                    var targetFullName = BuildEnvironment.TargetFullName;
                    var configuration = BuildEnvironment.Configuration;
                    var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.BuildDir, $"{BuildEnvironment.TargetFullName}", "ExternalInstall", "ogg"));
                    var targetInfo = BuildEngine.TargetInfoMap[builder.Triplet];
                    var altRuntime = "-DUSE_MSVC_RUNTIME_LIBRARY_DLL=" + (targetInfo.StaticRuntime == "ON" ? "OFF" : "ON");
                    var cppyamlRuntime = "-DYAML_MSVC_SHARED_RT=" + (targetInfo.StaticRuntime == "ON" ? "OFF" : "ON");

                    BuildProjectMSVC(builder, "nanovg", reposDir, targetName, targetFullName, configuration);
                    BuildProjectMSVC(builder, "SDL2", reposDir, targetName, targetFullName, configuration, $"-DSDL_SHARED=OFF -DSDL_STATIC=ON -DSSE=OFF ");
                    BuildProjectMSVC(builder, "tmxlite/tmxlite", reposDir, targetName, targetFullName, configuration, $"-DTMXLITE_STATIC_LIB=ON");
                    BuildProjectMSVC(builder, "lua", reposDir, targetName, targetFullName, configuration);

                    if (builder.Args.Contains("--enable-Effekseer"))
                    {
                        BuildProjectMSVC(builder, "Effekseer", reposDir, targetName, targetFullName, configuration, $"{altRuntime} -DBUILD_VIEWER=ON -DBUILD_EDITOR=ON -DBUILD_VULKAN=ON");
                    }
                }

                // Android
                if (LuminoBuild.AndoridBuildEnv.AndroidStudioFound && BuildEnvironment.IsAndroidTarget)
                {
                    //foreach (var target in BuildEngine_AndroidJNI.Targets)
                    {
                        var targetName = BuildEnvironment.Target;
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.BuildDir, $"{targetName}", "ExternalInstall", "ogg"));

                        BuildProjectAndroid(builder, "tmxlite/tmxlite", reposDir,targetName, "-DTMXLITE_STATIC_LIB=ON");
                        BuildProjectAndroid(builder, "nanovg", reposDir, targetName);
                    }
                }

                // Emscripten
                if (BuildEnvironment.IsWebTarget)
                {
                    var externalInstallDir = Path.Combine(EmscriptenBuildEnv.EmscriptenSysRootLocal, "ExternalInstall");
                    var oggInstallDir = Utils.ToUnixPath(Path.Combine(externalInstallDir, "ogg"));

                    BuildProjectEm(builder, "tmxlite/tmxlite", reposDir, "Emscripten", "-DTMXLITE_STATIC_LIB=ON");
                    BuildProjectEm(builder, "nanovg", reposDir, "Emscripten");
                }

            }
            else
            {
                // iOS
                if (BuildEnvironment.IsIOSTarget)
                {
                    //var targetInfos = new []
                    //{
                    //    new { Config = "", Platform = "OS64" },
                        //new { Config = "Release", Platform = "OS64" },
                    //    new { Config = "", Platform = "SIMULATOR64" },
                        //new { Config = "Release", Platform = "SIMULATOR64" },
                    //};
                    

                    var iOSToolchainFile = Utils.ToUnixPath(Path.Combine(builder.BuildDir, "ExternalSource", "ios-cmake", "ios.toolchain.cmake "));
                    
                    //foreach (var t in targetInfos)
                    {
                        var dirName = BuildEnvironment.Target;//$"iOS-{t.Platform}";
                        var platform = dirName.Replace("iOS-", "");
                        var args = $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\" -DPLATFORM={platform}";
                        var generator = "Xcode";
                        
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.BuildDir, dirName, "ExternalInstall", "ogg"));

                        BuildProject(builder, "tmxlite/tmxlite", "", reposDir, dirName, generator, $"-DTMXLITE_STATIC_LIB=ON " + args);
                        BuildProject(builder, "nanovg", "", reposDir, dirName, generator, args);
                    }
                }

                // macOS
                if (BuildEnvironment.IsMacOSTarget)
                {
                    var targetArgs = new[]
                    {
                        //new { DirName = "macOS", Config = "Debug",Args = "" },
                        new { DirName = "macOS", Config = "",Args = "" },
                    };

                    foreach (var t in targetArgs)
                    {
                        var dirName = t.DirName;
                        var args = t.Args;
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.BuildDir, dirName, "ExternalInstall", "ogg"));

                        var generator = "Xcode";
                        BuildProject(builder, "tmxlite/tmxlite", t.Config, reposDir, dirName, generator, $"-DTMXLITE_STATIC_LIB=ON " + args);
                        BuildProject(builder, "nanovg", t.Config, reposDir, dirName, generator, args);
                    }
                }
            }

            if (BuildEnvironment.FromCI)
            {
                // CI 環境でディスク消費を抑えるため、一時ファイルを削除する
                var buildDir = builder.GetExternalProjectBuildDir(BuildEnvironment.Target);
                Utils.DeleteDirectory(buildDir);
            }

            builder.CommitCache(builder.GetExternalInstallDir(BuildEnvironment.Target));
        }
    }
}
