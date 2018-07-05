using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class BuildExternalProjects : BuildTask
    {
        public override string CommandName { get { return "BuildExternalProjects"; } }

        public override string Description { get { return "BuildExternalProjects"; } }

        public override void Build(Builder builder)
        {
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
                                File.Move(path, Path.Combine(dir, "zlibd" + ext));
                            else
                                File.Move(path, Path.Combine(dir, "zlib" + ext));
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
                                File.Move(path, Path.Combine(dir, "libpng16d" + ext));
                            else
                                File.Move(path, Path.Combine(dir, "libpng16" + ext));
                        }
                    }
                    
#if false

                    {

                        var projectName = "libpng";
                        var buildDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalBuild", projectName);
                        var installDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalInstall");
                        var sourceDir = "../../../../external/" + projectName;

                        Directory.CreateDirectory(buildDir);
                        Directory.SetCurrentDirectory(buildDir);
                        Utils.CallProcess("cmake", $"-DLN_MSVC_STATIC_RUNTIME=ON -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} {sourceDir}");
                        Utils.CallProcess("cmake", "--build . --config Debug");
                        Utils.CallProcess("cmake", "--build . --config Release");
                    }

                    return;
                    {
                        var projectName = "zlib";
                        var buildDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalBuild", projectName);
                        var installDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalInstall");
                        var sourceDir = "../../../../external/" + projectName;

                        Directory.CreateDirectory(buildDir);
                        Directory.SetCurrentDirectory(buildDir);
                        Utils.CallProcess("cmake", $"-DLN_MSVC_STATIC_RUNTIME=ON -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} {sourceDir}");
                        Utils.CallProcess("cmake", "--build . --config Debug");
                        Utils.CallProcess("cmake", "--build . --config Release");






                        // zlib は cmake のコンフィグレーションで static lib のみ生成できない。
                        // また、cmake の find_XXXX 系でライブラリを探すとき、static lib 優先にできない。
                        // そのため dynamic lib を削除して static lib の名前を置き換えることで対策する。
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
                                File.Move(path, Path.Combine(dir, "zlibd" + ext));
                            else
                                File.Move(path, Path.Combine(dir, "zlib" + ext));
                        }
                    }
                    //var buildDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalBuild");
                    //var installDir = Path.Combine(builder.LuminoBuildDir, target.DirName, "ExternalInstall");

                    //Directory.CreateDirectory(buildDir);
                    //Directory.SetCurrentDirectory(buildDir);
                    //Utils.CallProcess("cmake", $"-DLN_MSVC_STATIC_RUNTIME=ON -DLN_EXTERNAL_BUILD_DIR={buildDir.Replace("\\", "/")} -DLN_EXTERNAL_INSTALL_DIR={installDir} ../../../external");
                    //Utils.CallProcess("cmake", "--build . --config Debug");
                    //Utils.CallProcess("cmake", "--build . --config Release");










                    break;
#endif
                }
            }
        }
    }
}
