using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace LuminoBuild.Tasks
{
    class MakeReleasePackage : BuildTask
    {
        public override string CommandName => "MakeReleasePackage";

        public override string Description => "MakeReleasePackage";

        public override void Build(Builder builder)
        {
            var tempInstallDir = Path.Combine(builder.LuminoBuildDir, BuildEnvironment.CMakeTargetInstallDir);
            string targetEnvName;
            if (Utils.IsWin32)
                targetEnvName = "Windows";
            else
                targetEnvName = "macOS";

            string packageName = $"Lumino-{builder.VersionString}-{targetEnvName}";

            string targetRootDir = Path.Combine(builder.LuminoRootDir, packageName);
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
                File.Copy(
                    Path.Combine(builder.LuminoSourceDir, "LuminoSetup.cmake"),
                    Path.Combine(nativeEngineLib, "LuminoSetup.cmake"), true);

                File.Copy(
                    Path.Combine(builder.LuminoExternalDir, "ImportExternalLibraries.cmake"),
                    Path.Combine(nativeEngineLib, "ImportExternalLibraries.cmake"), true);

                File.WriteAllText(
                    Path.Combine(nativeEngineRoot, ".gitignore"),
                    "include/\nlib/\n");
            }

            var externalLibs = new string[]
            {
                "glad",
                "glfw",
                "glslang",
                "libpng",
                "openal-soft",
                "SDL2",
                "SPIRV-Cross",
                "zlib",
            };

            // C++ Engine
            {
                // include files
                {
                    Utils.CopyDirectory(
                        Path.Combine(builder.LuminoRootDir, "include"),
                        Path.Combine(nativeEngineRoot, "include"));
                }

                // lib files
                foreach (var arch in BuildEnvironment.TargetArchs)
                {
                    if (Directory.Exists(Path.Combine(tempInstallDir, arch.SourceDirName)))   // copy if directory exists.
                    {
                        var targetDir = Path.Combine(nativeEngineRoot, "lib", arch.DestDirName);

                        Utils.CopyDirectory(
                            Path.Combine(tempInstallDir, arch.SourceDirName/*, "lib"*/),
                            targetDir);

                        var externalInstallDir = Path.Combine(builder.LuminoBuildDir, arch.SourceDirName, "ExternalInstall");
                        foreach (var lib in externalLibs)
                        {
                            var srcDir = Path.Combine(externalInstallDir, lib, "lib");
                            if (Directory.Exists(srcDir))   // copy if directory exists. openal-soft etc are optional.
                            {
                                Utils.CopyDirectory(
                                    srcDir,
                                    targetDir);
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
                                    File.Copy(file, Path.Combine(targetDir, Path.GetFileName(file)), true);
                                    Console.WriteLine(file);
                                }
                            }
                        }
                    }
                }

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
                            Path.Combine(tempInstallDir, "macOS", "bin"),
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
            }

            // zip
            {
                Utils.CreateZipFile(targetRootDir, Path.Combine(builder.LuminoBuildDir, packageName + ".zip"), true);
            }
        }
    }
}
