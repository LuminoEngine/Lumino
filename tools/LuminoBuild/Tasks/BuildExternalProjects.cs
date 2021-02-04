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
        public override string CommandName => "BuildExternalProjects";


        // (システム標準の cmake を使う系)
        private void BuildProject(Builder builder, string projectDirName, string configuration, string externalSourceDir, string buildArch, string generator, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName); // zlib/contrib/minizip のような場合に minizip だけ取り出す
            var buildDir = builder.GetExternalProjectBuildDir(buildArch, projectName);
            var installDir = builder.GetExternalProjectInstallDir(buildArch, projectName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(builder.LuminoRootDir, "src", "CFlagOverrides.cmake");

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

        private void BuildProjectMSVC(Builder builder, string projectDirName, string externalSourceDir, string targetName, string targetFullName, string configuration, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName); // zlib/contrib/minizip のような場合に minizip だけ取り出す
            var buildDir = builder.GetExternalProjectBuildDir(targetFullName, projectName);
            var installDir = builder.GetExternalProjectInstallDir(targetFullName, projectName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(builder.LuminoRootDir, "src", "CFlagOverrides.cmake");

            var targetInfo = BuildEngine_MSVC.TargetInfoMap[targetName];

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

        private void BuildProjectEm(Builder builder, string projectDirName, string externalSourceDir, string buildArchDir, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName);
            var buildDir = builder.GetExternalProjectBuildDir(buildArchDir, projectName);
            var installDir = builder.GetExternalProjectInstallDir(buildArchDir, projectName);
            //var installDir = Utils.ToUnixPath(Path.Combine(EmscriptenBuildEnv.EmscriptenSysRootLocal, projectName));
            var cmakeSourceDir = Utils.ToUnixPath(Path.Combine(externalSourceDir, projectDirName));
            var ov = Path.Combine(builder.LuminoRootDir, "src", "CFlagOverrides.cmake");

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

        private void BuildProjectAndroid(Builder builder, string projectDirName, string externalSourceDir, string targetName, string additionalOptions = "")
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

        public override void Build(Builder builder)
        {
            if (builder.ExistsCache(builder.GetExternalInstallDir(BuildEnvironment.Target)))
            {
                Logger.WriteLine("BuildExternalProjects has cache.");
                return;
            }

            var reposDir = Path.Combine(builder.LuminoBuildDir, "ExternalSource");
            Directory.CreateDirectory(reposDir);
            Directory.SetCurrentDirectory(reposDir);

            if (!Directory.Exists("ios-cmake"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b 3.0.1 https://github.com/leetal/ios-cmake.git ios-cmake");
            }
            if (!Directory.Exists("zlib"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b v1.2.11 https://github.com/madler/zlib.git zlib");
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "zlib", "CMakeLists.txt"), "zlib");
            }
            if (!Directory.Exists("libpng"))
            {
                //Utils.CallProcess("git", "clone --depth 1 -b v1.6.9 git://git.code.sf.net/p/libpng/code libpng");
                Utils.CallProcess("git", "clone --depth 1 -b libpng17 https://github.com/glennrp/libpng.git libpng");

#if false
                var zip = Path.Combine(reposDir, "lpng1635.zip");
                Utils.DownloadFile("https://download.sourceforge.net/libpng/lpng1635.zip", zip);

                var dir = Path.Combine(reposDir, "lpng1635");
                Utils.ExtractZipFile(zip, dir);
                Directory.Move(Path.Combine(dir, "lpng1635"), Path.Combine(reposDir, "libpng"));
                //Directory.Move(dir, Path.Combine(reposDir, "libpng"));

                //Utils.CallProcess("git", "clone --depth 1 -b libpng17 https://github.com/glennrp/libpng.git libpng");
#endif
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "libpng", "CMakeLists.txt"), "libpng");
            }
            if (!Directory.Exists("glslang"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b SDK-candidate-26-Jul-2020 https://github.com/KhronosGroup/glslang.git glslang");
            }
            if (!Directory.Exists("SPIRV-Cross"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b 2020-06-29 https://github.com/KhronosGroup/SPIRV-Cross.git SPIRV-Cross");
                //Utils.CallProcess("git", "clone https://github.com/KhronosGroup/SPIRV-Cross.git SPIRV-Cross");
                //Directory.SetCurrentDirectory("SPIRV-Cross");
                //Utils.CallProcess("git", "checkout be7425ef70231ab82930331959ab487d605d0482");
                //Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("glfw"))
            {
                // TODO: #glfw 816 の対策が現時点の最新 3.2.1 には入っていないので、開発中の master を取ってくる
                // 3.3 リリース後、そのタグで clone するようにしておく。
                if (Utils.IsMac)
                {
                    Utils.CallProcess("git", "clone https://github.com/glfw/glfw.git glfw");
                    Directory.SetCurrentDirectory("glfw");
                    Utils.CallProcess("git", "checkout 5afcd0981bf2fe9b9550f24ba298857aac6c35c2");
                    Directory.SetCurrentDirectory(reposDir);
                }
                else
                {
                    Utils.CallProcess("git", "clone --depth 1 -b 3.2.1 https://github.com/glfw/glfw.git glfw");
                }
            }
            if (!Directory.Exists("glad"))
            {
                Utils.CopyDirectory(Path.Combine(builder.LuminoExternalDir, "glad"), "glad");
                //Utils.CallProcess("git", "clone --depth 1 -b v0.1.26 https://github.com/Dav1dde/glad.git glad");
            }
            if (!Directory.Exists("openal-soft"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b openal-soft-1.20.1 https://github.com/kcat/openal-soft.git");
                //// https://github.com/kcat/openal-soft/issues/183 の問題の修正後、まだタグが降られていない。そのため latest を取得
                //Utils.CallProcess("git", "clone https://github.com/kcat/openal-soft.git openal-soft");
                //Directory.SetCurrentDirectory("openal-soft");
                //Utils.CallProcess("git", "checkout 7d76cbddd6fbdb52eaa917845435b95ae89efced");
                //Directory.SetCurrentDirectory(reposDir);
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
            if (!Directory.Exists("freetype2"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b VER-2-7-1 git://git.sv.nongnu.org/freetype/freetype2.git freetype2");
                
                // freetype2 の CMakeList.txt は iOS ツールチェインを独自で持っているが、
                // 2018/11/19 時点では master のサポートでもビルドを通すことができない。
                // そのため ↑のほうにある ios-cmake を有効にするため、PLATFORM に関する設定を削除した CMakeLists.txt を使用する。
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "freetype2", "CMakeLists.txt"), "freetype2");
            }
            if (!Directory.Exists("ogg"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b v1.3.3 https://github.com/xiph/ogg.git ogg");
            }
            if (!Directory.Exists("vorbis"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b v1.3.6-lumino https://github.com/lriki/vorbis.git vorbis");
            }
            if (!Directory.Exists("bullet3"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b 2.89 https://github.com/bulletphysics/bullet3.git bullet3");

                // 2.87 時点では Android ターゲットのビルドができない。
                // - ルートの CMakeLists.txt が python を探しに行く、{} 初期化リストで暗黙変換を使っている。など。
                // 2018/12/29 時点の master では対策されていたのでこれを使用する。
                //Utils.CallProcess("git", "clone https://github.com/bulletphysics/bullet3.git bullet3");
                //Directory.SetCurrentDirectory("bullet3");
                //Utils.CallProcess("git", "checkout 8bc1c8e01b1b2b9284df08385da0e03241f4e6aa");
                //Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("pcre"))
            {
                Utils.DownloadFile("ftp://ftp.pcre.org/pub/pcre/pcre2-10.31.zip", "pcre2-10.31.zip");
                Utils.ExtractZipFile("pcre2-10.31.zip", reposDir, true);
                Directory.Move("pcre2-10.31", "pcre");
            }
            if (!Directory.Exists("tmxlite"))
            {
                Utils.CallProcess("git", "clone https://github.com/fallahn/tmxlite.git tmxlite");
                Directory.SetCurrentDirectory("tmxlite");
                Utils.CallProcess("git", "checkout 8ed41071fe0774947fc7f7c6ece77de3061a5239");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("Box2D"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b v2.3.1 https://github.com/erincatto/Box2D.git Box2D");
            }
            if (!Directory.Exists("Vulkan-Headers"))
            {
                Utils.CallProcess("git", "clone --depth 1 -b v1.1.105 https://github.com/KhronosGroup/Vulkan-Headers.git Vulkan-Headers");
            }
            if (!Directory.Exists("Effekseer"))
            {
                Utils.CallProcess("git", "clone https://github.com/effekseer/Effekseer Effekseer");
                Directory.SetCurrentDirectory("Effekseer");
                Utils.CallProcess("git", "submodule update --init");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("yaml-cpp"))
            {
                //Utils.CallProcess("git", "clone --depth 1 -b yaml-cpp-0.6.3 https://github.com/jbeder/yaml-cpp.git yaml-cpp");
                // まだタグの振られていない #824 の修正がほしい
                Utils.CallProcess("git", "clone https://github.com/jbeder/yaml-cpp.git yaml-cpp");
                Directory.SetCurrentDirectory("yaml-cpp");
                Utils.CallProcess("git", "checkout 4edff1fa5dbfca16fc72d89870841bee89f8ef89");
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
                    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{BuildEnvironment.TargetFullName}", "ExternalInstall", "zlib"));
                    var pngIncludeDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{BuildEnvironment.TargetFullName}", "ExternalInstall", "libpng", "include"));
                    var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{BuildEnvironment.TargetFullName}", "ExternalInstall", "ogg"));
                    var targetInfo = BuildEngine_MSVC.TargetInfoMap[targetName];
                    var altRuntime = "-DUSE_MSVC_RUNTIME_LIBRARY_DLL=" + (targetInfo.StaticRuntime == "ON" ? "OFF" : "ON");
                    var cppyamlRuntime = "-DYAML_MSVC_SHARED_RT=" + (targetInfo.StaticRuntime == "ON" ? "OFF" : "ON");

                    BuildProjectMSVC(builder, "nanovg", reposDir, targetName, targetFullName, configuration);
                    BuildProjectMSVC(builder, "yaml-cpp", reposDir, targetName, targetFullName, configuration, $"{cppyamlRuntime} -DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=OFF");
                    BuildProjectMSVC(builder, "zlib", reposDir, targetName, targetFullName, configuration);
                    BuildProjectMSVC(builder, "libpng", reposDir, targetName, targetFullName, configuration, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                    BuildProjectMSVC(builder, "glslang", reposDir, targetName, targetFullName, configuration);
                    BuildProjectMSVC(builder, "SPIRV-Cross", reposDir, targetName, targetFullName, configuration);
                    BuildProjectMSVC(builder, "glfw", reposDir, targetName, targetFullName, configuration, $"-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=ON");
                    BuildProjectMSVC(builder, "glad", reposDir, targetName, targetFullName, configuration, $"-DGLAD_INSTALL=ON");
                    BuildProjectMSVC(builder, "openal-soft", reposDir, targetName, targetFullName, configuration, $"-DALSOFT_BACKEND_DSOUND=OFF");  // find_package(DSound) で古い WindowsSDK の include パスが登録されてしまい、Windows.h など他の include が古い方に引っ張られてしまう
                    BuildProjectMSVC(builder, "SDL2", reposDir, targetName, targetFullName, configuration, $"-DSDL_SHARED=OFF -DSDL_STATIC=ON -DSSE=OFF ");
                    BuildProjectMSVC(builder, "freetype2", reposDir, targetName, targetFullName, configuration, $"-DPNG_FOUND=ON -DPNG_INCLUDE_DIRS={pngIncludeDir}");
                    BuildProjectMSVC(builder, "ogg", reposDir, targetName, targetFullName, configuration);
                    BuildProjectMSVC(builder, "vorbis", reposDir, targetName, targetFullName, configuration, $"-DOGG_ROOT={oggInstallDir}");
                    BuildProjectMSVC(builder, "bullet3", reposDir, targetName, targetFullName, configuration, $"{altRuntime} {bulletOptions}");
                    BuildProjectMSVC(builder, "pcre", reposDir, targetName, targetFullName, configuration, $"-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF");
                    BuildProjectMSVC(builder, "tmxlite/tmxlite", reposDir, targetName, targetFullName, configuration, $"-DTMXLITE_STATIC_LIB=ON");
                    BuildProjectMSVC(builder, "Box2D/Box2D", reposDir, targetName, targetFullName, configuration, $"-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON");
                    BuildProjectMSVC(builder, "Vulkan-Headers", reposDir, targetName, targetFullName, configuration);
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

                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{targetName}", "ExternalInstall", "zlib"));
                        var pngIncludeDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{BuildEnvironment.TargetFullName}", "ExternalInstall", "libpng", "include"));
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{targetName}", "ExternalInstall", "ogg"));

                        //BuildProjectAndroid(builder, "zlib", reposDir,targetName);
                        BuildProjectAndroid(builder, "libpng", reposDir, targetName);//, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                        BuildProjectAndroid(builder, "freetype2", reposDir,targetName, $"-DPNG_FOUND=ON -DPNG_INCLUDE_DIRS={pngIncludeDir}");
                        BuildProjectAndroid(builder, "ogg", reposDir,targetName);
                        BuildProjectAndroid(builder, "vorbis", reposDir,targetName, $"-DOGG_ROOT={oggInstallDir} -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH");
                        BuildProjectAndroid(builder, "bullet3", reposDir,targetName, bulletOptions);
                        BuildProjectAndroid(builder, "pcre", reposDir,targetName, "-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF");
                        BuildProjectAndroid(builder, "tmxlite/tmxlite", reposDir,targetName, "-DTMXLITE_STATIC_LIB=ON");
                        BuildProjectAndroid(builder, "Box2D/Box2D", reposDir,targetName, "-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON");
                        BuildProjectAndroid(builder, "Vulkan-Headers", reposDir,targetName);
                        BuildProjectAndroid(builder, "yaml-cpp", reposDir, targetName, $"-DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=OFF");
                        BuildProjectAndroid(builder, "nanovg", reposDir, targetName);
                    }
                }

                // Emscripten
                if (BuildEnvironment.IsWebTarget)
                {
                    var externalInstallDir = Path.Combine(EmscriptenBuildEnv.EmscriptenSysRootLocal, "ExternalInstall");
                    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(externalInstallDir, "zlib"));
                    var pngIncludeDir = Utils.ToUnixPath(Path.Combine(externalInstallDir, "libpng", "include"));
                    var oggInstallDir = Utils.ToUnixPath(Path.Combine(externalInstallDir, "ogg"));

                    BuildProjectEm(builder, "zlib", reposDir, "Emscripten");
                    BuildProjectEm(builder, "libpng", reposDir, "Emscripten", $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                    BuildProjectEm(builder, "glad", reposDir, "Emscripten", "-DGLAD_INSTALL=ON");
                    BuildProjectEm(builder, "freetype2", reposDir, "Emscripten", $"-DPNG_FOUND=ON -DPNG_INCLUDE_DIRS={pngIncludeDir}");
                    BuildProjectEm(builder, "ogg", reposDir, "Emscripten");
                    BuildProjectEm(builder, "vorbis", reposDir, "Emscripten", $"-DOGG_ROOT={oggInstallDir} -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH");
                    BuildProjectEm(builder, "bullet3", reposDir, "Emscripten", bulletOptions);
                    BuildProjectEm(builder, "pcre", reposDir, "Emscripten", "-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF");
                    BuildProjectEm(builder, "tmxlite/tmxlite", reposDir, "Emscripten", "-DTMXLITE_STATIC_LIB=ON");
                    BuildProjectEm(builder, "Box2D/Box2D", reposDir, "Emscripten", "-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON");
                    BuildProjectEm(builder, "yaml-cpp", reposDir, "Emscripten", $"-DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=OFF");
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
                    

                    var iOSToolchainFile = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "ExternalSource", "ios-cmake", "ios.toolchain.cmake "));
                    
                    //foreach (var t in targetInfos)
                    {
                        var dirName = BuildEnvironment.Target;//$"iOS-{t.Platform}";
                        var platform = dirName.Replace("iOS-", "");
                        var args = $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\" -DPLATFORM={platform}";
                        var generator = "Xcode";
                        
                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, dirName, "ExternalInstall", "zlib"));
                        var pngIncludeDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, dirName, "ExternalInstall", "libpng", "include"));
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, dirName, "ExternalInstall", "ogg"));

                        BuildProject(builder, "zlib", "", reposDir, dirName, generator, args);
                        BuildProject(builder, "libpng", "", reposDir, dirName, generator, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include " + args);
                        BuildProject(builder, "freetype2", "", reposDir, dirName, generator, $"-DWITH_ZLIB=OFF -DWITH_BZip2=OFF  -DWITH_PNG=OFF -DWITH_HarfBuzz=OFF -DPNG_FOUND=ON -DPNG_INCLUDE_DIRS={pngIncludeDir} " + args);
                        BuildProject(builder, "ogg", "", reposDir, dirName, generator, args);
                        BuildProject(builder, "vorbis", "", reposDir, dirName, generator, $"-DOGG_ROOT={oggInstallDir} -DCMAKE_DEVELOPER_ROOT={builder.LuminoBuildDir} " + args);
                        BuildProject(builder, "bullet3", "", reposDir, dirName, generator, $"{bulletOptions} " + args);
                        BuildProject(builder, "pcre", "", reposDir, dirName, generator, $"-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF -DPCRE2_BUILD_TESTS=OFF " + args);
                        BuildProject(builder, "tmxlite/tmxlite", "", reposDir, dirName, generator, $"-DTMXLITE_STATIC_LIB=ON " + args);
                        BuildProject(builder, "Box2D/Box2D", "", reposDir, dirName, generator, $"-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON " + args);
                        BuildProject(builder, "Vulkan-Headers", "", reposDir, dirName, generator, args);
                        BuildProject(builder, "yaml-cpp", "", reposDir, dirName, generator, $"-DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=OFF " + args);
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
                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, dirName, "ExternalInstall", "zlib"));
                        var pngIncludeDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, dirName, "ExternalInstall", "libpng", "include"));
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, dirName, "ExternalInstall", "ogg"));

                        var generator = "Xcode";
                        BuildProject(builder, "zlib", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "libpng", t.Config, reposDir, dirName, generator, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include " + args);
                        BuildProject(builder, "glslang", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "SPIRV-Cross", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "glfw", t.Config, reposDir, dirName, generator, $"-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=ON");
                        BuildProject(builder, "glad", t.Config, reposDir, dirName, generator, $"-DGLAD_INSTALL=ON " + args);
                        BuildProject(builder, "freetype2", t.Config, reposDir, dirName, generator, $"-DWITH_ZLIB=OFF -DWITH_BZip2=OFF  -DWITH_PNG=OFF -DWITH_HarfBuzz=OFF -DPNG_FOUND=ON -DPNG_INCLUDE_DIRS={pngIncludeDir} " + args);
                        BuildProject(builder, "ogg", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "vorbis", t.Config, reposDir, dirName, generator, $"-DOGG_ROOT={oggInstallDir} " + args);
                        BuildProject(builder, "bullet3", t.Config, reposDir, dirName, generator, $"{bulletOptions} " + args);
                        BuildProject(builder, "pcre", t.Config, reposDir, dirName, generator, $"-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF " + args);
                        BuildProject(builder, "tmxlite/tmxlite", t.Config, reposDir, dirName, generator, $"-DTMXLITE_STATIC_LIB=ON " + args);
                        BuildProject(builder, "Box2D/Box2D", t.Config, reposDir, dirName, generator, $"-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON " + args);
                        BuildProject(builder, "Vulkan-Headers", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "yaml-cpp", t.Config, reposDir, dirName, generator, $"-DYAML_CPP_BUILD_TESTS=OFF -DYAML_CPP_BUILD_CONTRIB=OFF -DYAML_CPP_BUILD_TOOLS=OFF " + args);
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
