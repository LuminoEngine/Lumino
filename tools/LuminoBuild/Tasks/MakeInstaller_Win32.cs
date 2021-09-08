using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    /// <summary>
    /// Archive "build\NativePackage" as an installer.
    /// </summary>
    class MakeInstaller_Win32 : BuildTask
    {
        public override string CommandName => "MakeInstaller_Win32";

        //public override List<string> Dependencies => new List<string>() { "BuildDocuments", "BuildEngine_MSVC" };

        public override void Build(Build builder)
        {
            string heat = Path.Combine(Environment.GetEnvironmentVariable("WIX"), "bin", "heat");
            string candle = Path.Combine(Environment.GetEnvironmentVariable("WIX"), "bin", "candle");
            string light = Path.Combine(Environment.GetEnvironmentVariable("WIX"), "bin", "light");
            string tmpDir = Path.Combine(builder.BuildDir, "InstallerTemp");
            string pkgSrcInstallerDir = Path.Combine(builder.LuminoToolsDir, "PackageSource", "Installer");

            var targets = new[]
            {
                new
                {
                    ContentFilesDir = Path.Combine(builder.BuildDir, builder.LocalPackageName),
                    WXSFileTemplate = Path.Combine(pkgSrcInstallerDir, "LuminoInstaller.wxs.template"),
                    TargetDirId = "ID_LUMINO_DIR",
                    ProductGUID = builder.InstallerProductGUID,
                    Output = builder.ReleasePackageName + ".msi",
                },
            };


            // VisualStudio project tempalte
            {
                // zip にして、.wxs が $(var.PackageSourceDir) で参照できるフォルダに配置する
                Utils.CreateZipFile(
                    Path.Combine(builder.LuminoToolsDir, "VisualStudio", "Templates", "Lumino-VS2019-Cpp"),
                    Path.Combine(builder.LuminoToolsDir, "PackageSource", "Installer", "Lumino-VS2019-Cpp.zip"),
                    false);
            }

            Directory.CreateDirectory(tmpDir);



            foreach (var t in targets)
            {
                Logger.WriteLine("Build {0} ...", t.Output);

                // インストーラの基本情報が記述された LuminoInstaller.wxs ファイルを作る
                string installerWXS = Path.Combine(tmpDir, "LuminoInstaller.wxs");
                string text = File.ReadAllText(t.WXSFileTemplate);
                text = text.Replace("%%Version%%", builder.VersionString);
                text = text.Replace("%%ProductGUID%%", t.ProductGUID);
                File.WriteAllText(installerWXS, text);

                // インストーラに含めるファイルが記述された LuminoFiles.wxs ファイルを作る (ContentFilesDir の中身すべて)
                string contentFilesWXS = Path.Combine(tmpDir, "LuminoFiles.wxs");
                string args = string.Format(
                    "dir {0} -dr {1} -cg LuminoFiles -gg -g1 -sfrag -srd -var \"var.LuminoFilesDir\" -out {2}",
                    t.ContentFilesDir,
                    t.TargetDirId,
                    contentFilesWXS);
                Utils.CallProcess(heat, args);

                // LuminoInstaller.wxs から LuminoInstaller.wixobj を生成する
                args = string.Format("-nologo {0} -dLuminoFilesDir={1} -dPackageSourceDir={2} -out {0}.wixobj", installerWXS, t.ContentFilesDir, pkgSrcInstallerDir);
                Utils.CallProcess(candle, args);

                // LuminoInstaller.wxs から LuminoInstaller.wixobj を生成する
                args = string.Format("-nologo {0} -dLuminoFilesDir={1} -dPackageSourceDir={2} -out {0}.wixobj", contentFilesWXS, t.ContentFilesDir, pkgSrcInstallerDir);
                Utils.CallProcess(candle, args);

                // .msi を作る
                args = string.Format("-nologo -v -ext WixUIExtension -cultures:ja-jp {0}.wixobj {1}.wixobj -pdbout {0}.wixpdb -out {2}", installerWXS, contentFilesWXS, Path.Combine(builder.BuildDir, t.Output));
                Utils.CallProcess(light, args);
            }

            // Create zip package
            {
                var orgName = Path.Combine(builder.BuildDir, builder.LocalPackageName);
                var tmpName = Path.Combine(builder.BuildDir, builder.ReleasePackageName);
                Directory.Move(orgName, tmpName);
                if (!BuildEnvironment.FromCI)
                {
                    Utils.CreateZipFile(tmpName, tmpName + ".zip");
                    Directory.Move(tmpName, orgName);
                }
            }

        }
    }
}
