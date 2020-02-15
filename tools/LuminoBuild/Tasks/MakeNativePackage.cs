using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakeNativePackage : BuildTask
    {
        public override string CommandName => "MakeNativePackage";

        public override List<string> Dependencies => new List<string>() { "BuildEngine_MSVC" };

        public bool fileMoving = false;

        public override void Build(Builder builder)
        {
            string destinationRootDir = Path.Combine(builder.LuminoBuildDir, builder.LocalPackageName);
            string destinationToolDir = Path.Combine(destinationRootDir, "Tools");
            Directory.CreateDirectory(destinationRootDir);
            Directory.CreateDirectory(destinationToolDir);

            foreach (var target in BuildEnvironment.Targets)
            {
                string sourceBuildDir = Path.Combine(builder.LuminoBuildDir, target.Name);
                if (Directory.Exists(sourceBuildDir))
                {
                    string destinationEngineRoot = Path.Combine(destinationRootDir, "Engine", target.Name);
                    string destinationEngineIncludeDir = Path.Combine(destinationEngineRoot, "include");
                    string destinationEngineLibDir = Path.Combine(destinationEngineRoot, "lib");
                    Directory.CreateDirectory(destinationEngineRoot);
                    Directory.CreateDirectory(destinationEngineIncludeDir);
                    Directory.CreateDirectory(destinationEngineLibDir);

                    string engineInstallDir = Path.Combine(builder.LuminoBuildDir, target.Name, BuildEnvironment.EngineInstallDirName);

                    // Engine include files
                    {
                        Utils.CopyDirectory(
                            Path.Combine(engineInstallDir, "include"),
                            Path.Combine(destinationEngineIncludeDir));
                    }

                    // Engine libs
                    {
                        Console.WriteLine($"Engine libs...");

                        var srcDir = Path.Combine(engineInstallDir, "lib");
                        Utils.CopyDirectory(srcDir, destinationEngineLibDir, recursive: false);
                        if (fileMoving)
                            Directory.Delete(srcDir, true); // FIXME: CI サーバのストレージ不足対策
                    }

                    // Engine libs
                    {
                        Console.WriteLine($"Engine libs...");

                        var srcDir = Path.Combine(engineInstallDir, "lib");
                        Utils.CopyDirectory(srcDir, destinationEngineLibDir, recursive: false);
                        if (fileMoving)
                            Directory.Delete(srcDir, true); // FIXME: CI サーバのストレージ不足対策
                    }

                    // External libs
                    var externalInstallDir = Path.Combine(builder.LuminoBuildDir, target.Name, "ExternalInstall");
                    foreach (var dir in Directory.EnumerateDirectories(externalInstallDir))
                    {
                        var srcDir = Path.Combine(dir, "lib");
                        if (Directory.Exists(srcDir))   // copy if directory exists. openal-soft etc are optional.
                        {
                            Console.WriteLine($"Copy {srcDir} to {destinationEngineLibDir}");

                            foreach (var lib in Directory.EnumerateFiles(srcDir, target.LibraryExt, SearchOption.TopDirectoryOnly))
                            {
                                if (fileMoving)
                                    File.Move(lib, Path.Combine(destinationEngineLibDir, Path.GetFileName(lib)));
                                else
                                    File.Copy(lib, Path.Combine(destinationEngineLibDir, Path.GetFileName(lib)), true);
                            }
                        }
                    }
                }
            }

            // Tools
            {
                Console.WriteLine($"Copy tools...");

                if (Utils.IsWin32)
                {
                    string engineInstallDir = Path.Combine(builder.LuminoBuildDir, "MSVC2017-x64-MT", BuildEnvironment.EngineInstallDirName);

                    Utils.CopyDirectory(Path.Combine(engineInstallDir, "bin"), destinationToolDir, pattern: "*.exe");
                    Utils.DownloadFile(BuildEnvironment.VSWhereUrl, Path.Combine(destinationToolDir, "vswhere.exe"));
                }
                else if (Utils.IsMac)
                {
                    string engineInstallDir = Path.Combine(builder.LuminoBuildDir, "macOS", BuildEnvironment.EngineInstallDirName);

                    Utils.CopyDirectory(Path.Combine(engineInstallDir, "bin"), destinationToolDir);

                    string setupFile = Path.Combine(destinationToolDir, "setup.sh");
                    File.Copy(Path.Combine(builder.LuminoRootDir, "Tools", "PackageSource", "macOS", "setup.sh"), setupFile, true);
                    Utils.chmod(setupFile, Utils.S_0755);
                }

                // Templates
                {
                    Utils.CopyDirectory(
                        Path.Combine(builder.LuminoToolsDir, "ProjectTemplates"),
                        Path.Combine(destinationToolDir, "ProjectTemplates"));
                }
            }

            // Docs
            {
                Console.WriteLine($"Copy docs...");

                // Readme
                Utils.GenerateFile(
                    Path.Combine(destinationRootDir, "Readme.txt"),
                    Path.Combine(builder.LuminoPackageSourceDir, "Readme.txt.template"),
                    new Dictionary<string, string> { { "%%LuminoVersion%%", builder.VersionString } });
            }

#if false
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir);

            string targetRootDir = Path.Combine(builder.LuminoBuildDir, builder.LocalPackageName);
            Directory.CreateDirectory(targetRootDir);

            string nativeEngineRoot = Path.Combine(targetRootDir, "Engine", "Native");
            string nativeEngineLib = Path.Combine(nativeEngineRoot, "lib");
            Directory.CreateDirectory(nativeEngineRoot);
            Directory.CreateDirectory(nativeEngineLib);

            // docs
            {
                // Readme
                Utils.GenerateFile(
                    Path.Combine(targetRootDir, "Readme.txt"),
                    Path.Combine(builder.LuminoPackageSourceDir, "Readme.txt.template"),
                    new Dictionary<string, string> { { "%%LuminoVersion%%", builder.VersionString } });
            }

            // C++ Engine (common)
            {
                string nativeEngineCMakeDir = Path.Combine(nativeEngineRoot, "lib", "cmake", "Lumino");
                Directory.CreateDirectory(nativeEngineCMakeDir);

                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoSetup.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "LuminoSetup.cmake"), true);
                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoCommon.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "LuminoCommon.cmake"), true);
                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoConfig.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "LuminoConfig.cmake"), true);

                File.Copy(
                    Path.Combine(builder.LuminoExternalDir, "ImportExternalLibraries.cmake"),
                    Path.Combine(nativeEngineCMakeDir, "ImportExternalLibraries.cmake"), true);

                File.WriteAllText(
                    Path.Combine(nativeEngineRoot, ".gitignore"),
                    "include/\nlib/\n");
            }

            // C++ Engine
            {
                // include files
                {
                    Utils.CopyDirectory(
                        Path.Combine(builder.LuminoRootDir, "include"),
                        Path.Combine(nativeEngineRoot, "include"));
                }

                // lib files
                CopyEngineLibs(builder, tempInstallDir, nativeEngineRoot, FileMoving);

                // bin files
                {
                    if (Utils.IsWin32)
                    {
                        Utils.CopyDirectory(
                            Path.Combine(tempInstallDir, "MSVC2017-x86-MT", "bin"),
                            Path.Combine(targetRootDir, "Tools"));

                        Utils.DownloadFile(
                            BuildEnvironment.VSWhereUrl,
                            Path.Combine(targetRootDir, "Tools", "vswhere.exe"));
                    }
                    else if (Utils.IsMac)
                    {
                        Utils.CopyDirectory(
                            Path.Combine(tempInstallDir, "macOS-Release", "bin"),
                            Path.Combine(targetRootDir, "Tools"));

                        string file = Path.Combine(targetRootDir, "setup.sh");
                        File.Copy(
                            Path.Combine(builder.LuminoRootDir, "Tools", "PackageSource", "macOS", "setup.sh"),
                            file, true);

                        Utils.chmod(file, Utils.S_0755);
                    }
                }
            }

            // Templates
            {
                Utils.CopyDirectory(
                    Path.Combine(builder.LuminoToolsDir, "LuminoCLI", "Templates", "NativeProject"),
                    Path.Combine(targetRootDir, "Tools", "Templates", "NativeProject"));

                Utils.CopyDirectory(
                    Path.Combine(builder.LuminoToolsDir, "LuminoCLI", "Templates", "SimpleDesktop"),
                    Path.Combine(targetRootDir, "Tools", "Templates", "SimpleDesktop"));
            }

            // FIXME: CI サーバのストレージ不足対策
            if (FileMoving)
            {
                Directory.Delete(tempInstallDir, true);

                foreach (var arch in BuildEnvironment.TargetArchs)
                {
                    var path = Path.Combine(builder.LuminoBuildDir, arch.SourceDirName);
                    if (Directory.Exists(path))
                        Directory.Delete(path, true);
                }
            }
#endif
        }

        public static string[] externalLibs = new string[]
        {
                "Box2D",
                "bullet3",
                "freetype2",
                "glad",
                "glfw",
                "glslang",
                "libpng",
                "ogg",
                "openal-soft",
                "pcre",
                "SDL2",
                "SPIRV-Cross",
                "tmxlite",
                "vorbis",
                "zlib",
        };

        public static void CopyEngineLibs(Builder builder, string tempInstallDir, string nativeEngineRoot, bool fileMoving)
        {
#if false
            foreach (var arch in BuildEnvironment.TargetArchs)
            {
                if (Directory.Exists(Path.Combine(tempInstallDir, arch.SourceDirName)))   // copy if directory exists.
                {
                    var targetDir = Path.Combine(nativeEngineRoot, "lib", arch.DestDirName);

                    // Engine libs
                    {
                        var srcDir = Path.Combine(tempInstallDir, arch.SourceDirName, "lib");

                        Console.WriteLine($"Copy {srcDir} to {targetDir}");
                        Utils.CopyDirectory(srcDir, targetDir);
                        if (fileMoving)
                            Directory.Delete(srcDir, true); // FIXME: CI サーバのストレージ不足対策

                        // cmake
                        var cmakeDir = Path.Combine(tempInstallDir, arch.SourceDirName, "cmake");
                        if (Directory.Exists(cmakeDir))
                            Utils.CopyDirectory(cmakeDir, targetDir);
                    }

                    // External libs
                    var externalInstallDir = Path.Combine(builder.LuminoBuildDir, arch.SourceDirName, "ExternalInstall");
                    foreach (var lib in externalLibs)
                    {
                        var srcDir = Path.Combine(externalInstallDir, lib, "lib");
                        if (Directory.Exists(srcDir))   // copy if directory exists. openal-soft etc are optional.
                        {
                            Console.WriteLine($"Copy {srcDir} to {targetDir}");
                            Utils.CopyDirectory(srcDir, targetDir);

                            if (fileMoving)
                                Directory.Delete(srcDir, true); // FIXME: CI サーバのストレージ不足対策
                        }
                    }

                    // .pdb
                    // CMake では static library の PDB 出力先をコントロールできない。https://cmake.org/cmake/help/v3.1/prop_tgt/PDB_OUTPUT_DIRECTORY.html
                    // そのためビルドスクリプト側でコントロールする。
                    // 以下、パスに "Debug" を含む者のうち、lib と同じ名前の pdb ファイルをコピーする。
                    if (arch.PdbCopy)
                    {
                        var libfiles = Directory.GetFiles(targetDir, "*.lib", SearchOption.TopDirectoryOnly);
                        var libnames = new HashSet<string>(libfiles.Select(x => Path.GetFileNameWithoutExtension(x)));
                        var files1 = Directory.GetFiles(Path.Combine(builder.LuminoBuildDir, arch.SourceDirName), "*.pdb", SearchOption.AllDirectories);
                        foreach (var file in files1)
                        {
                            if (file.Contains("Debug") && libnames.Contains(Path.GetFileNameWithoutExtension(file)))
                            {
                                // FIXME: CI サーバのストレージ不足対策
                                if (fileMoving)
                                    File.Move(file, Path.Combine(targetDir, Path.GetFileName(file)));
                                else
                                    File.Copy(file, Path.Combine(targetDir, Path.GetFileName(file)), true);
                                Console.WriteLine(file);
                            }
                        }
                    }
                }
            }
#endif
        }
    }
}
