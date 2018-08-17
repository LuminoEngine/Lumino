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


        private void BuildProject(Builder builder, string projectDirName, string externalSourceDir, string buildArchDir, string generator, string additionalOptions = "")
        {

            var buildDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalBuild", projectDirName));
            var installDir = Path.Combine(builder.LuminoBuildDir, buildArchDir, "ExternalInstall", projectDirName);
            var cmakeSourceDir = Path.Combine(externalSourceDir, projectDirName);
            var ov = Path.Combine(builder.LuminoRootDir, "src", "CFlagOverrides.cmake");

            Logger.WriteLine($"BuildProject ({projectDirName}) buildDir:{buildDir}");

            Directory.CreateDirectory(buildDir);
            Directory.SetCurrentDirectory(buildDir);
            Utils.CallProcess("cmake", $"-DCMAKE_INSTALL_PREFIX={installDir} -DCMAKE_USER_MAKE_RULES_OVERRIDE={ov} {additionalOptions} -G \"{generator}\" {cmakeSourceDir}");
            Utils.CallProcess("cmake", "--build . --config Debug");
            Utils.CallProcess("cmake", "--build . --config Debug --target INSTALL");
            Utils.CallProcess("cmake", "--build . --config Release");
            Utils.CallProcess("cmake", "--build . --config Release --target INSTALL");
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

            Utils.CallProcessShell(script); // bat の中でエラーが発生すれば、例外に乗って出てくる

            // emcmake で find_library などを行う場合、Emscripten のシステムフォルダ以外は検索しないようにツールチェインファイルで封印されている。
            // Lumino 本体のビルド時にライブラリを探すことができるようにするため、システムフォルダに一式コピーしておく。
            Utils.CopyDirectory(installDir, Path.Combine(BuildEnvironment.EmscriptenDir, "system"));
        }

        private void BuildProjectAndroid(Builder builder, string projectDirName, string externalSourceDir, string abi, string additionalOptions = "")
        {
            string cmakeHomeDir = Path.Combine(externalSourceDir, projectDirName);//builder.LuminoRootDir;
            string platform = BuildEnvironment.AndroidTargetPlatform;

            var targetName = "Android-" + abi;

            string cmakeBuildDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalBuild", projectDirName);
            string cmakeInstallDir = Path.Combine(builder.LuminoBuildDir, targetName, "ExternalInstall", projectDirName);
            string buildType = "Release";
            string args = $"-H{cmakeHomeDir} -B{cmakeBuildDir} -DLN_TARGET_ARCH_NAME={targetName} -DCMAKE_INSTALL_PREFIX={cmakeInstallDir} -DANDROID_ABI={abi} -DANDROID_PLATFORM={platform} -DCMAKE_BUILD_TYPE={buildType} -DANDROID_NDK={BuildEnvironment.AndroidNdkRootDir} -DCMAKE_CXX_FLAGS=-std=c++14 -DANDROID_STL=c++_shared -DCMAKE_TOOLCHAIN_FILE={BuildEnvironment.AndroidCMakeToolchain} -DCMAKE_MAKE_PROGRAM={BuildEnvironment.AndroidSdkNinja} -G\"Android Gradle - Ninja\"";

            Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, args);
            Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, "--build " + cmakeBuildDir);
            Utils.CallProcess(BuildEnvironment.AndroidSdkCMake, "--build " + cmakeBuildDir + " --target install");
        }

        public override void Build(Builder builder)
        {
            var reposDir = Path.Combine(builder.LuminoBuildDir, "ExternalSource");
            Directory.CreateDirectory(reposDir);
            Directory.SetCurrentDirectory(reposDir);

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
            }
            if (!Directory.Exists("glad"))
            {
                Utils.CopyDirectory(Path.Combine(builder.LuminoExternalDir, "glad"), "glad");
                //Utils.CallProcess("git", "clone --progress --depth 1 -b v0.1.26 https://github.com/Dav1dde/glad.git glad");
            }
            

            if (Utils.IsWin32)
            {
                //// Android
                //{
                //    foreach (var target in BuildEngineAndroidJNI.Targets)
                //    {
                //        var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "Android-" + target.ABI, "ExternalInstall", "zlib"));
                //        BuildProjectAndroid(builder, "zlib", reposDir, target.ABI);
                //        BuildProjectAndroid(builder, "libpng", reposDir, target.ABI, $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                //        BuildProjectAndroid(builder, "glslang", reposDir, target.ABI);
                //        BuildProjectAndroid(builder, "SPIRV-Cross", reposDir, target.ABI);
                //    }
                //}

                //// Emscripten
                //{
                //    var externalInstallDir = Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall");
                //    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall", "zlib"));

                //    BuildProjectEm(builder, "zlib", reposDir, "Emscripten");
                //    BuildProjectEm(builder, "libpng", reposDir, "Emscripten", $"-DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                //    BuildProjectEm(builder, "glslang", reposDir, "Emscripten");
                //    BuildProjectEm(builder, "SPIRV-Cross", reposDir, "Emscripten");
                //    BuildProjectEm(builder, "glad", reposDir, "Emscripten", "-DGLAD_INSTALL=ON");
                //}
          
                // Visual C++
                foreach (var target in MakeVSProjects.Targets)
                {
                    var zlibInstallDir = Utils.ToUnixPath(Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalInstall", "zlib"));

                    BuildProject(builder, "zlib", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                    BuildProject(builder, "libpng", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DZLIB_INCLUDE_DIR={zlibInstallDir}/include");
                    BuildProject(builder, "glslang", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime}");
                    BuildProject(builder, "SPIRV-Cross", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DCMAKE_DEBUG_POSTFIX=d");
                    BuildProject(builder, "glad", reposDir, target.DirName, target.VSTarget, $"-DLN_MSVC_STATIC_RUNTIME={target.MSVCStaticRuntime} -DGLAD_INSTALL=ON");
                }
            }




            return;








            if (Utils.IsWin32)
            {
                foreach (var target in MakeVSProjects.Targets)
                {

                    //var projectName = "zlib";
                    var buildDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalBuild");
                    var installDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalInstall");
                    var sourceDir = "../../../external/";// + projectName;

                    Directory.CreateDirectory(buildDir);
                    Directory.SetCurrentDirectory(buildDir);
                    Utils.CallProcess("cmake", $"-DLN_MSVC_STATIC_RUNTIME=ON -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} {sourceDir}");
                    Utils.CallProcess("cmake", "--build . --config Debug");
                    Utils.CallProcess("cmake", "--build . --config Release");






                    // zlib は cmake のコンフィグレーションで static lib のみ生成できない。
                    // また、cmake の find_XXXX 系でライブラリを探すとき、static lib 優先にできない。
                    // そのため dynamic lib を削除して static lib の名前を置き換えることで対策する。
                    {
                        var binDir = Path.Combine(installDir, "zlib", "bin");
                        if (Directory.Exists(binDir))
                            Directory.Delete(Path.Combine(binDir), true);
                        var zlibList = Directory.EnumerateFiles(Path.Combine(installDir, "zlib", "lib"), "zlib*", SearchOption.AllDirectories).Where(x => !x.Contains("zlibstatic"));
                        var zlibstaticList = Directory.EnumerateFiles(Path.Combine(installDir, "zlib", "lib"), "zlib*", SearchOption.AllDirectories).Where(x => x.Contains("zlibstatic"));
                        foreach (var path in zlibList)
                        {
                            File.Delete(path);
                        }
                        foreach (var path in zlibstaticList)
                        {
                            var dir = Path.GetDirectoryName(path);
                            var ext = Path.GetExtension(path);
                            var name = Path.GetFileNameWithoutExtension(path);

                            if (name == "zlibstaticd")  // debug lib
                                Utils.MoveFileForce(path, Path.Combine(dir, "zlibd" + ext));
                            else
                                Utils.MoveFileForce(path, Path.Combine(dir, "zlib" + ext));
                        }
                    }

                    // libpng も
                    {
                        var libpngstaticList = Directory.EnumerateFiles(Path.Combine(installDir, "libpng", "lib"), "libpng*", SearchOption.TopDirectoryOnly);
                        foreach (var path in libpngstaticList)
                        {
                            var dir = Path.GetDirectoryName(path);
                            var ext = Path.GetExtension(path);
                            var name = Path.GetFileNameWithoutExtension(path);

                            if (name == "libpng16_staticd")  // debug lib
                                Utils.MoveFileForce(path, Path.Combine(dir, "libpng16d" + ext));
                            else if(name == "libpng16_static")
                                Utils.MoveFileForce(path, Path.Combine(dir, "libpng16" + ext));
                        }
                    }
                }

                // Emscripten
                {
                    var buildDir = Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalBuild");
                    var installDir = Path.Combine(builder.LuminoBuildDir, "Emscripten", "ExternalInstall");
                    var sourceDir = "../../../external/";

                    Directory.CreateDirectory(buildDir);

                    var script = Path.Combine(buildDir, "build.bat");
                    using (var f = new StreamWriter(script))
                    {
                        f.WriteLine($"cd \"{BuildEnvironment.EmsdkDir}\"");
                        //f.WriteLine($"emsdk activate {BuildEnvironment.emsdkVer}");
                        f.WriteLine($"call emsdk_env.bat");
                        f.WriteLine($"cd \"{Utils.ToWin32Path(buildDir)}\"");
                        f.WriteLine($"call emcmake cmake -DCMAKE_BUILD_TYPE=Release -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} -G \"MinGW Makefiles\" {sourceDir}");
                        f.WriteLine($"cmake --build .");
                    }

                    Utils.CallProcessShell(script);

                    //Directory.SetCurrentDirectory(BuildEnvironment.EmsdkDir);
                    //Utils.CallProcess("emsdk_env.bat");
                    //var ff = Environment.GetEnvironmentVariables();
                    //Utils.CallProcess("emcmake", $"cmake -DCMAKE_BUILD_TYPE=Release -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} -G \"MinGW Makefiles\" {sourceDir}");

                    //Directory.CreateDirectory(buildDir);
                    //Directory.SetCurrentDirectory(buildDir);
                    //Utils.CallProcess(BuildEnvironment.emcmake, $"cmake -DCMAKE_BUILD_TYPE=Release -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} -G \"MinGW Makefiles\" {sourceDir}");
                    //Utils.CallProcess("cmake", "--build .");
                }
            }
            else
            {
                var buildDir = Path.Combine(builder.LuminoBuildDir, "Linux-x86_64", "ExternalBuild");
                var installDir = Path.Combine(builder.LuminoBuildDir, "Linux-x86_64", "ExternalInstall");
                var sourceDir = "../../../external/";

                Directory.CreateDirectory(buildDir);
                Directory.SetCurrentDirectory(buildDir);
                Utils.CallProcess("cmake", $"-DCMAKE_BUILD_TYPE=Release -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} {sourceDir}");
                Utils.CallProcess("cmake", "--build .");
                //Utils.CallProcess("cmake", "--build . --config Debug");
            }

        }
    }
}
