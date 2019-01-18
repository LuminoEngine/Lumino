using System;
using System.Collections.Generic;
using System.IO;

namespace LuminoBuild.Tasks
{
    class MakeInstaller_Win32 : BuildTask
    {
        public override string CommandName => "MakeInstaller_Win32";

        public override string Description => "MakeInstaller_Win32";

        public override void Build(Builder builder)
        {
            string heat = Path.Combine(Environment.GetEnvironmentVariable("WIX"), "bin", "heat");
            string candle = Path.Combine(Environment.GetEnvironmentVariable("WIX"), "bin", "candle");
            string light = Path.Combine(Environment.GetEnvironmentVariable("WIX"), "bin", "light");
            string tmpDir = Path.Combine(builder.LuminoBuildDir, "InstallerTemp");
            string pkgSrcInstallerDir = Path.Combine(builder.LuminoToolsDir, "PackageSource", "Installer");

            var targets = new[]
            {
                new
                {
                    ContentFilesDir = Path.Combine(builder.LuminoBuildDir, builder.LocalPackageName),
                    WXSFileTemplate = Path.Combine(pkgSrcInstallerDir, "LuminoInstaller.wxs.template"),
                    TargetDirId = "ID_LUMINO_DIR",
                    ProductGUID = builder.InstallerProductGUID_MSVC2017,
                    Output = builder.ReleasePackageName + ".msi",
                },
            };

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
                args = string.Format("-nologo -v -ext WixUIExtension -cultures:ja-jp {0}.wixobj {1}.wixobj -pdbout {0}.wixpdb -out {2}", installerWXS, contentFilesWXS, Path.Combine(builder.LuminoBuildDir, t.Output));
                Utils.CallProcess(light, args);
            }
        }
    }
}
