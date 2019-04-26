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
        private void BuildProject(Builder builder, string projectDirName, string buildType, string externalSourceDir, string buildArch, string generator, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName); // zlib/contrib/minizip のような場合に minizip だけ取り出す
            var targetName = buildArch + "-" + buildType;
            var buildDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, targetName, "ExternalBuild", projectName));
            var installDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalInstall", projectName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(builder.LuminoRootDir, "src", "CFlagOverrides.cmake");

            Logger.WriteLine($"BuildProject ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            
            var args = new string[]
            {
                $"-DCMAKE_INSTALL_PREFIX={installDir}",
                $"-DCMAKE_DEBUG_POSTFIX=d",     // cmake の find_package で Debug/Release 両対応するために、同じフォルダに lib を入れておきたい。(Qt 参考)
                $"-DCMAKE_USER_MAKE_RULES_OVERRIDE={ov}",
                $"{additionalOptions}",
                $"-G \"{generator}\"",
                $"{cmakeSourceDir}",
            };
            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build . --config {buildType}");
            Utils.CallProcess("cmake", $"--build . --config {buildType} --target install");
        }
        private void BuildProjectMSVC(Builder builder, string projectDirName, string externalSourceDir, string buildArch, string generator, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName); // zlib/contrib/minizip のような場合に minizip だけ取り出す
            var targetName = buildArch;
            var buildDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, targetName, "ExternalBuild", projectName));
            var installDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalInstall", projectName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(builder.LuminoRootDir, "src", "CFlagOverrides.cmake");

            Logger.WriteLine($"BuildProject ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);

            var args = new string[]
            {
                $"-DCMAKE_INSTALL_PREFIX={installDir}",
                $"-DCMAKE_DEBUG_POSTFIX=d",     // cmake の find_package で Debug/Release 両対応するために、同じフォルダに lib を入れておきたい。(Qt 参考)
                $"-DCMAKE_USER_MAKE_RULES_OVERRIDE={ov}",
                $"{additionalOptions}",
                $"-G \"{generator}\"",
                $"{cmakeSourceDir}",
            };
            Utils.CallProcess("cmake", string.Join(' ', args));
            Utils.CallProcess("cmake", $"--build . --config Debug");
            Utils.CallProcess("cmake", $"--build . --config Debug --target install");
            Utils.CallProcess("cmake", $"--build . --config Release");
            Utils.CallProcess("cmake", $"--build . --config Release --target install");
        }

        private void BuildProjectEm(Builder builder, string projectDirName, string externalSourceDir, string buildArchDir, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName);
            var buildDir = Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalBuild", projectName);
            var installDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalInstall", projectName));
            var cmakeSourceDir = Utils.ToUnixPath(Path.Combine(externalSourceDir, projectDirName));

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

        private void BuildProjectAndroid(Builder builder, string projectDirName, string externalSourceDir, string abi, string buildType, string additionalOptions = "")
        {
            var projectName = Path.GetFileName(projectDirName);
            string cmakeHomeDir = Path.Combine(externalSourceDir, projectDirName);//builder.LuminoRootDir;
            string platform = BuildEnvironment.AndroidTargetPlatform;
            
            var targetName = $"Android-{abi}-{buildType}";
            var cmakeBuildDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalBuild", projectName);
            var cmakeInstallDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalInstall", projectName);

            var args = new string[]
            {
                $"-H{cmakeHomeDir}",
                $"-B{cmakeBuildDir}",
                $"-DLN_TARGET_ARCH_NAME={targetName}",
                $"-DCMAKE_INSTALL_PREFIX={cmakeInstallDir}",
                $"-DCMAKE_DEBUG_POSTFIX=d",
                $"-DANDROID_ABI={abi}",
                $"-DANDROID_PLATFORM={platform}",
                $"-DCMAKE_BUILD_TYPE={buildType}",
                $"-DANDROID_NDK={BuildEnvironment.AndroidNdkRootDir}",
                $"-DCMAKE_CXX_FLAGS=-std=c++14",
                $"-DANDROID_STL=c++_shared",
                $"-DCMAKE_TOOLCHAIN_FILE={BuildEnvironment.AndroidCMakeToolchain}",
                $"-DCMAKE_MAKE_PROGRAM={BuildEnvironment.AndroidSdkNinja}",
                $"-DANDROID_NATIVE_API_LEVEL=26",
                additionalOptions,
                $"-G\"Android Gradle - Ninja\"",
            };

            Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, string.Join(' ', args));
            Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, "--build " + cmakeBuildDir);
            Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, "--build " + cmakeBuildDir + " --target install");
        }

        public override void Build(Builder builder)
        {
            var reposDir = Path.Combine(builder.LuminoBuildDir, "ExternalSource");
            Directory.CreateDirectory(reposDir);
            Directory.SetCurrentDirectory(reposDir);

            if (!Directory.Exists("ios-cmake"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b 3.0.1 https://github.com/leetal/ios-cmake.git ios-cmake");
            }
            if (!Directory.Exists("zlib"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b v1.2.11 https://github.com/madler/zlib.git zlib");
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "zlib", "CMakeLists.txt"), "zlib");
            }
            if (!Directory.Exists("libpng"))
            {
                //Utils.CallProcess("git", "clone --progress --depth 1 -b v1.6.9 git://git.code.sf.net/p/libpng/code libpng");
                Utils.CallProcess("git", "clone --progress --depth 1 -b libpng17 https://github.com/glennrp/libpng.git libpng");

#if false
                var zip = Path.Combine(reposDir, "lpng1635.zip");
                Utils.DownloadFile("https://download.sourceforge.net/libpng/lpng1635.zip", zip);

                var dir = Path.Combine(reposDir, "lpng1635");
                Utils.ExtractZipFile(zip, dir);
                Directory.Move(Path.Combine(dir, "lpng1635"), Path.Combine(reposDir, "libpng"));
                //Directory.Move(dir, Path.Combine(reposDir, "libpng"));

                //Utils.CallProcess("git", "clone --progress --depth 1 -b libpng17 https://github.com/glennrp/libpng.git libpng");
#endif
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "libpng", "CMakeLists.txt"), "libpng");
            }
            if (!Directory.Exists("glslang"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b 7.11.3113 https://github.com/KhronosGroup/glslang.git glslang");
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
                // TODO: #glfw 816 の対策が現時点の最新 3.2.1 には入っていないので、開発中の master を取ってくる
                // 3.3 リリース後、そのタグで clone するようにしておく。
                if (Utils.IsMac)
                {
                    Utils.CallProcess("git", "clone --progress https://github.com/glfw/glfw.git glfw");
                    Directory.SetCurrentDirectory("glfw");
                    Utils.CallProcess("git", "checkout 5afcd0981bf2fe9b9550f24ba298857aac6c35c2");
                    Directory.SetCurrentDirectory(reposDir);
                }
                else
                {
                    Utils.CallProcess("git", "clone --progress --depth 1 -b 3.2.1 https://github.com/glfw/glfw.git glfw");
                }
            }
            if (!Directory.Exists("glad"))
            {
                Utils.CopyDirectory(Path.Combine(builder.LuminoExternalDir, "glad"), "glad");
                //Utils.CallProcess("git", "clone --progress --depth 1 -b v0.1.26 https://github.com/Dav1dde/glad.git glad");
            }
            if (!Directory.Exists("openal-soft"))
            {
                // https://github.com/kcat/openal-soft/issues/183 の問題の修正後、まだタグが降られていない。そのため latest を取得
                Utils.CallProcess("git", "clone --progress https://github.com/kcat/openal-soft.git openal-soft");
                Directory.SetCurrentDirectory("openal-soft");
                Utils.CallProcess("git", "checkout 7d76cbddd6fbdb52eaa917845435b95ae89efced");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("SDL2"))
            {
                var zip = Path.Combine(reposDir, "SDL2-2.0.8.zip");
                using (var wc = new System.Net.WebClient())
                {
                    wc.DownloadFile("https://www.libsdl.org/release/SDL2-2.0.8.zip", zip);
                }

                Utils.ExtractZipFile(zip, Path.Combine(reposDir, "SDL2-2.0.8"));
                System.Threading.Thread.Sleep(1000);
                Directory.Move(Path.Combine(reposDir, "SDL2-2.0.8", "SDL2-2.0.8"), Path.Combine(reposDir, "SDL2"));
            }
            if (!Directory.Exists("freetype2"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b VER-2-7-1 git://git.sv.nongnu.org/freetype/freetype2.git freetype2");
                
                // freetype2 の CMakeList.txt は iOS ツールチェインを独自で持っているが、
                // 2018/11/19 時点では master のサポートでもビルドを通すことができない。
                // そのため ↑のほうにある ios-cmake を有効にするため、PLATFORM に関する設定を削除した CMakeLists.txt を使用する。
                Utils.CopyFile(Path.Combine(builder.LuminoExternalDir, "freetype2", "CMakeLists.txt"), "freetype2");
            }
            if (!Directory.Exists("ogg"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b v1.3.3 https://github.com/xiph/ogg.git ogg");
            }
            if (!Directory.Exists("vorbis"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b v1.3.6-lumino https://github.com/lriki/vorbis.git vorbis");
            }
            if (!Directory.Exists("bullet3"))
            {
                //Utils.CallProcess("git", "clone --progress --depth 1 -b 2.87 https://github.com/bulletphysics/bullet3.git bullet3");

                // 2.87 時点では Android ターゲットのビルドができない。
                // - ルートの CMakeLists.txt が python を探しに行く、{} 初期化リストで暗黙変換を使っている。など。
                // 2018/12/29 時点の master では対策されていたのでこれを使用する。
                Utils.CallProcess("git", "clone --progress https://github.com/bulletphysics/bullet3.git bullet3");
                Directory.SetCurrentDirectory("bullet3");
                Utils.CallProcess("git", "checkout 8bc1c8e01b1b2b9284df08385da0e03241f4e6aa");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("tinyobjloader"))
            {
                // v1.0.6 より後はタグが降られていないが、頂点カラーなどの対応が入っている。それを持ってくる。
                Utils.CallProcess("git", "clone --progress https://github.com/syoyo/tinyobjloader.git tinyobjloader");
                Directory.SetCurrentDirectory("tinyobjloader");
                Utils.CallProcess("git", "checkout f37fed32f3eb0912cc10a970f78774cd98598ef6");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("Streams"))
            {
                Utils.CallProcess("git", "clone --progress https://github.com/jscheiny/Streams.git Streams");
                Directory.SetCurrentDirectory("Streams");
                Utils.CallProcess("git", "checkout 8fc0657b977cfd8f075ad0afb4dca3800630b56c");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("pcre"))
            {
                Utils.DownloadFile("ftp://ftp.csx.cam.ac.uk/pub/software/programming/pcre/pcre2-10.31.zip", "pcre2-10.31.zip");
                Utils.ExtractZipFile("pcre2-10.31.zip", reposDir, true);
                Directory.Move("pcre2-10.31", "pcre");
            }
            if (!Directory.Exists("tmxlite"))
            {
                Utils.CallProcess("git", "clone --progress https://github.com/fallahn/tmxlite.git tmxlite");
                Directory.SetCurrentDirectory("tmxlite");
                Utils.CallProcess("git", "checkout 8ed41071fe0774947fc7f7c6ece77de3061a5239");
                Directory.SetCurrentDirectory(reposDir);
            }
            if (!Directory.Exists("stb"))
            {
                Utils.CallProcess("git", "clone --progress https://github.com/nothings/stb.git stb");
                Utils.CallProcess("git", "-C stb checkout e6afb9cbae4064da8c3e69af3ff5c4629579c1d2");
            }
            if (!Directory.Exists("Box2D"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b v2.3.1 https://github.com/erincatto/Box2D.git Box2D");
            }
            if (!Directory.Exists("Vulkan-Headers"))
            {
                Utils.CallProcess("git", "clone --progress --depth 1 -b v1.1.105 https://github.com/KhronosGroup/Vulkan-Headers.git Vulkan-Headers");
            }

            const string bulletOptions = "-DBUILD_BULLET2_DEMOS=OFF -DBUILD_CLSOCKET=OFF -DBUILD_CPU_DEMOS=OFF -DBUILD_ENET=OFF -DBUILD_EXTRAS=OFF -DBUILD_OPENGL3_DEMOS=OFF -DBUILD_UNIT_TESTS=OFF -DINSTALL_LIBS=ON";

            if (Utils.IsWin32)
            {
                // Android
                if (BuildEnvironment.AndroidStudioFound && BuildEnvironment.BuildTarget.HasFlag(BuildTargetFlags.Android))
                {
                    foreach (var target in BuildEngine_AndroidJNI.Targets)
                    {
                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"Android-{target.ABI}-{target.BuildType}", "ExternalInstall", "zlib"));
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"Android-{target.ABI}-{target.BuildType}", "ExternalInstall", "ogg"));

                        BuildProjectAndroid(builder, "zlib", reposDir, target.ABI, target.BuildType);
                        BuildProjectAndroid(builder, "libpng", reposDir, target.ABI, target.BuildType, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                        BuildProjectAndroid(builder, "freetype2", reposDir, target.ABI, target.BuildType);
                        BuildProjectAndroid(builder, "ogg", reposDir, target.ABI, target.BuildType);
                        BuildProjectAndroid(builder, "vorbis", reposDir, target.ABI, target.BuildType, $"-DOGG_ROOT={oggInstallDir} -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH");
                        BuildProjectAndroid(builder, "bullet3", reposDir, target.ABI, target.BuildType, bulletOptions);
                        BuildProjectAndroid(builder, "pcre", reposDir, target.ABI, target.BuildType, "-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF");
                        BuildProjectAndroid(builder, "tmxlite/tmxlite", reposDir, target.ABI, target.BuildType, "-DTMXLITE_STATIC_LIB=ON");
                        BuildProjectAndroid(builder, "Box2D/Box2D", reposDir, target.ABI, target.BuildType, "-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON");
                        BuildProjectAndroid(builder, "Vulkan-Headers", reposDir, target.ABI, target.BuildType);
                    }
                }

                // Emscripten
                if (BuildEnvironment.BuildTarget.HasFlag(BuildTargetFlags.Web))
                {
                    var externalInstallDir = Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall");
                    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall", "zlib"));
                    var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall", "ogg"));

                    BuildProjectEm(builder, "zlib", reposDir, "Emscripten");
                    BuildProjectEm(builder, "libpng", reposDir, "Emscripten", $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                    BuildProjectEm(builder, "glad", reposDir, "Emscripten", "-DGLAD_INSTALL=ON");
                    BuildProjectEm(builder, "freetype2", reposDir, "Emscripten");
                    BuildProjectEm(builder, "ogg", reposDir, "Emscripten");
                    BuildProjectEm(builder, "vorbis", reposDir, "Emscripten", $"-DOGG_ROOT={oggInstallDir} -DCMAKE_FIND_ROOT_PATH_MODE_INCLUDE=BOTH -DCMAKE_FIND_ROOT_PATH_MODE_LIBRARY=BOTH");
                    BuildProjectEm(builder, "bullet3", reposDir, "Emscripten", bulletOptions);
                    BuildProjectEm(builder, "pcre", reposDir, "Emscripten", "-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF");
                    BuildProjectEm(builder, "tmxlite/tmxlite", reposDir, "Emscripten", "-DTMXLITE_STATIC_LIB=ON");
                    BuildProjectEm(builder, "Box2D/Box2D", reposDir, "Emscripten", "-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON");
                    BuildProjectEm(builder, "Vulkan-Headers", reposDir, "Vulkan-Headers");
                }

                // Visual C++
                if (BuildEnvironment.BuildTarget.HasFlag(BuildTargetFlags.Windows))
                {
                    foreach (var target in MakeVSProjects.Targets)
                    {
                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{target.DirName}", "ExternalInstall", "zlib"));
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{target.DirName}", "ExternalInstall", "ogg"));
                        var bulletRuntime = "-DUSE_MSVC_RUNTIME_LIBRARY_DLL=" + (target.MSVCStaticRuntime == "ON" ? "OFF" : "ON");

                        BuildProjectMSVC(builder, "zlib", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                        BuildProjectMSVC(builder, "libpng", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                        BuildProjectMSVC(builder, "glslang", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                        BuildProjectMSVC(builder, "SPIRV-Cross", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                        BuildProjectMSVC(builder, "glfw", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=ON");
                        BuildProjectMSVC(builder, "glad", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DGLAD_INSTALL=ON");
                        BuildProjectMSVC(builder, "openal-soft", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                        BuildProjectMSVC(builder, "SDL2", reposDir, target.DirName, target.VSTarget, $"-DSDL_SHARED=OFF -DSDL_STATIC=ON -DSSE=OFF -DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                        BuildProjectMSVC(builder, "freetype2", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                        BuildProjectMSVC(builder, "ogg", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                        BuildProjectMSVC(builder, "vorbis", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DOGG_ROOT={oggInstallDir}");
                        BuildProjectMSVC(builder, "bullet3", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} {bulletRuntime} {bulletOptions}");
                        BuildProjectMSVC(builder, "pcre", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF");
                        BuildProjectMSVC(builder, "tmxlite/tmxlite", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DTMXLITE_STATIC_LIB=ON");
                        BuildProjectMSVC(builder, "Box2D/Box2D", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON");
                        BuildProjectMSVC(builder, "Vulkan-Headers", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                    }
                }
            }
            else
            {
                // iOS
                if (BuildEnvironment.BuildTarget.HasFlag(BuildTargetFlags.iOS))
                {
                    var targetInfos = new []
                    {
                        new { Config = "Debug", Platform = "OS64" },
                        new { Config = "Release", Platform = "OS64" },
                        new { Config = "Debug", Platform = "SIMULATOR64" },
                        new { Config = "Release", Platform = "SIMULATOR64" },
                    };
                    var iOSToolchainFile = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "ExternalSource", "ios-cmake", "ios.toolchain.cmake "));
                    
                    foreach (var t in targetInfos)
                    {
                        var dirName = $"iOS-{t.Platform}";
                        var args = $"-DCMAKE_TOOLCHAIN_FILE=\"{iOSToolchainFile}\" -DPLATFORM={t.Platform}";
                        var generator = "Xcode";
                        
                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{dirName}-{t.Config}", "ExternalInstall", "zlib"));
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{dirName}-{t.Config}", "ExternalInstall", "ogg"));

                        BuildProject(builder, "zlib", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "libpng", t.Config, reposDir, dirName, generator, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include " + args);
                        BuildProject(builder, "freetype2", t.Config, reposDir, dirName, generator, $"-DWITH_ZLIB=OFF -DWITH_BZip2=OFF  -DWITH_PNG=OFF -DWITH_HarfBuzz=OFF " + args);
                        BuildProject(builder, "ogg", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "vorbis", t.Config, reposDir, dirName, generator, $"-DOGG_ROOT={oggInstallDir} -DCMAKE_DEVELOPER_ROOT={builder.LuminoBuildDir} " + args);
                        BuildProject(builder, "bullet3", t.Config, reposDir, dirName, generator, $"{bulletOptions} " + args);
                        BuildProject(builder, "pcre", t.Config, reposDir, dirName, generator, $"-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF -DPCRE2_BUILD_TESTS=OFF " + args);
                        BuildProject(builder, "tmxlite/tmxlite", t.Config, reposDir, dirName, generator, $"-DTMXLITE_STATIC_LIB=ON " + args);
                        BuildProject(builder, "Box2D/Box2D", t.Config, reposDir, dirName, generator, $"-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON " + args);
                        BuildProject(builder, "Vulkan-Headers", t.Config, reposDir, dirName, generator, args);
                    }
                }

                // macOS
                if (BuildEnvironment.BuildTarget.HasFlag(BuildTargetFlags.macOS))
                {
                    var targetArgs = new[]
                    {
                        new { DirName = "macOS", Config = "Debug",Args = "" },
                        new { DirName = "macOS", Config = "Release",Args = "" },
                    };

                    foreach (var t in targetArgs)
                    {
                        var dirName = t.DirName;
                        var args = t.Args;
                        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{dirName}-{t.Config}", "ExternalInstall", "zlib"));
                        var oggInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, $"{dirName}-{t.Config}", "ExternalInstall", "ogg"));

                        var generator = "Xcode";
                        BuildProject(builder, "zlib", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "libpng", t.Config, reposDir, dirName, generator, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include " + args);
                        BuildProject(builder, "glslang", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "SPIRV-Cross", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "glfw", t.Config, reposDir, dirName, generator, $"-DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF -DGLFW_INSTALL=ON");
                        BuildProject(builder, "glad", t.Config, reposDir, dirName, generator, $"-DGLAD_INSTALL=ON " + args);
                        BuildProject(builder, "freetype2", t.Config, reposDir, dirName, generator, $"-DWITH_ZLIB=OFF -DWITH_BZip2=OFF  -DWITH_PNG=OFF -DWITH_HarfBuzz=OFF " + args);
                        BuildProject(builder, "ogg", t.Config, reposDir, dirName, generator, args);
                        BuildProject(builder, "vorbis", t.Config, reposDir, dirName, generator, $"-DOGG_ROOT={oggInstallDir} " + args);
                        BuildProject(builder, "bullet3", t.Config, reposDir, dirName, generator, $"{bulletOptions} " + args);
                        BuildProject(builder, "pcre", t.Config, reposDir, dirName, generator, $"-DPCRE2_BUILD_PCRE2_8=OFF -DPCRE2_BUILD_PCRE2_16=ON -DPCRE2_BUILD_PCRE2_32=OFF " + args);
                        BuildProject(builder, "tmxlite/tmxlite", t.Config, reposDir, dirName, generator, $"-DTMXLITE_STATIC_LIB=ON " + args);
                        BuildProject(builder, "Box2D/Box2D", t.Config, reposDir, dirName, generator, $"-DBOX2D_BUILD_EXAMPLES=OFF -DBOX2D_INSTALL_DOC=OFF -DBOX2D_BUILD_SHARED=OFF -DBOX2D_BUILD_STATIC=ON -DBOX2D_INSTALL=ON " + args);
                        BuildProject(builder, "Vulkan-Headers", t.Config, reposDir, dirName, generator, args);
                    }
                }
            }
        }
    }
}
