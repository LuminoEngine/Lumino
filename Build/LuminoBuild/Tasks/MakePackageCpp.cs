using System;
using System.Text;
using System.IO;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class MakePackageCpp : BuildTask
    {
        [Flags]
        enum VSBuildFlags
        {
            None = 0x0000,
            VS2015 = 0x0002,
            VS2017 = 0x0004,
        }

        public override string CommandName { get { return "MakePackageCpp"; } }
        
        public override string Description { get { return "Make package for C++."; } }
        
        public override void Build(Builder builder)
        {
            //string zipFilePath = builder.LuminoPackageReleaseDir + "LuminoCpp.zip";

            VSBuildFlags vsTarget = /*VSBuildFlags.VS2015 | */VSBuildFlags.VS2017;

            if (vsTarget.HasFlag(VSBuildFlags.VS2015))
            {
                string releaseDir = builder.LuminoPackageReleaseDir + "MSVC2015/";
                Directory.CreateDirectory(releaseDir);
                CopyCommonFiles(builder, releaseDir);

                // lib, dll
                {
                    Logger.WriteLine("copy lib files...");
                    string outputLibDir = releaseDir + "lib/";

                    string target = "MSVC2015-x86-MT";
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Debug", "*.lib");
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Debug", "*.dll");
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Release", "*.lib");
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Release", "*.dll");
                }

                Logger.WriteLine("copy other files...");
                Directory.CreateDirectory(releaseDir + "tools/VS2015ProjectTemplate");
                Utils.CreateZipFile(builder.LuminoToolsDir + "VS2015ProjectTemplate/LuminoProjectCpp", releaseDir + "Tools/VS2015ProjectTemplate/LuminoProjectCpp.zip", false);
            }

            if (vsTarget.HasFlag(VSBuildFlags.VS2017))
            {
                string releaseDir = builder.LuminoPackageReleaseDir + "MSVC2017/";
                Directory.CreateDirectory(releaseDir);
                CopyCommonFiles(builder, releaseDir);

                // lib, dll
                {
                    Logger.WriteLine("copy lib files...");
                    string outputLibDir = releaseDir + "lib/";

                    string target = "MSVC2017-x86-MT";
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Debug", "*.lib");
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Debug", "*.dll");
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Release", "*.lib");
                    CopyFileFlat(builder.LuminoBuildDir + target, outputLibDir + target, "Release", "*.dll");
                }

                Logger.WriteLine("copy other files...");
                Directory.CreateDirectory(releaseDir + "tools/VS2017ProjectTemplate");
                Utils.CreateZipFile(builder.LuminoToolsDir + "VS2017ProjectTemplate/LuminoProjectCpp", releaseDir + "Tools/VS2017ProjectTemplate/LuminoProjectCpp.zip", false);
                // ↑ Kaspersky で、このタイミングでのzip圧縮でウィルス扱いされることがあった。
                // clone 直後など、クリーンな状態では発生しない。Release フォルダに上書きコピー → zpi 化がキーだろうか？
                // Release フォルダを削除しておくとセーフ。
            }

            // .zip に圧縮する
            //Logger.WriteLine("compressing files...");
            //Utils.CreateZipFile(releaseDir, zipFilePath);
        }

        void CopyCommonFiles(Builder builder, string releaseDir)
        {
            string pkgSrcDir = builder.LuminoPackageDir + "PackageSource/";

            // include
            Logger.WriteLine("copy include files...");
            string releaseIncludeDir = Path.Combine(releaseDir, "include");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Source/LuminoCore/Include"), releaseIncludeDir, true);
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Source/LuminoEngine/Include"), releaseIncludeDir, true);

            // Assets
            Logger.WriteLine("copy Assets files...");
            Utils.CopyDirectory(Path.Combine(builder.LuminoRootDir, "Samples/Assets"), releaseDir + "assets", true);

            // Readme.txt (バージョン名を埋め込む)
            string text = File.ReadAllText(pkgSrcDir + "Readme.template.txt");
            text = text.Replace("$(LuminoVersion)", builder.VersionString);
            File.WriteAllText(releaseDir + "Readme.txt", text, new UTF8Encoding(true));

            // ReleaseNote
            Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", releaseDir);
            Utils.CopyFile(builder.LuminoPackageSourceDir + "ReleaseNote.txt", builder.LuminoPackageReleaseDir);
        }

        void CopyFileFlat(string srcRootDir, string dstDir, string configration, string fileFilter)
        {
            var libs = Directory.EnumerateFileSystemEntries(srcRootDir, fileFilter, SearchOption.AllDirectories);
            var outputDir = Path.Combine(dstDir, configration); 

            Directory.CreateDirectory(outputDir);
            foreach (var src in libs)
            {
                if (src.Contains(configration))
                {
                    var dst = Path.Combine(outputDir, Path.GetFileName(src));
                    File.Copy(src, dst, true);
                }
            }
        }
    }
}
