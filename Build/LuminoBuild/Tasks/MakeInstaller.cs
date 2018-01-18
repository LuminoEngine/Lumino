using System;
using System.IO;
using LuminoBuild;
using System.Text;
using System.IO.Compression;

namespace LuminoBuild.Tasks
{
    class MakeInstaller : BuildTask
    {
        public override string CommandName { get { return "MakeInstaller"; } }

        public override string Description { get { return "Make installer."; } }

        public override void CheckPrerequisite(Builder builder)
        {
            Buildable = true;
        }

        public override void Build(Builder builder)
        {
            string heat = builder.LuminoRootDir + "External/LuminoDependencies/Tools/wix311-binaries/heat";
            string candle = builder.LuminoRootDir + "External/LuminoDependencies/Tools/wix311-binaries/candle";
            string light = builder.LuminoRootDir + "External/LuminoDependencies/Tools/wix311-binaries/light";
            string tmpDir = builder.LuminoPackageDir + "tmp/";
            string pkgSrcInstallerDir = builder.LuminoPackageSourceDir + "Installer";
            
            var targets = new[]
            {
                new { ContentFilesDir = builder.LuminoPackageDir + "Release/Lumino/MSVC2015", WXSFileTemplate = builder.LuminoPackageSourceDir + "Installer/LuminoInstaller_MSVC2015.wxs.template", TargetDirId = "LUMINO_MSVC2015", ProductGUID = builder.InstallerProductGUID_MSVC2015, Output = "LuminoInstaller_MSVC2015.msi" },
                new { ContentFilesDir = builder.LuminoPackageDir + "Release/Lumino/MSVC2017", WXSFileTemplate = builder.LuminoPackageSourceDir + "Installer/LuminoInstaller_MSVC2017.wxs.template", TargetDirId = "LUMINO_MSVC2017", ProductGUID = builder.InstallerProductGUID_MSVC2017, Output = "LuminoInstaller_MSVC2017.msi" },
            };

            Directory.CreateDirectory(tmpDir);

            foreach (var t in targets)
            {
                Logger.WriteLine("Build {0} ...", t.Output);

                string installerWXS = tmpDir + "LuminoInstaller.wxs";
                string contentFilesWXS = tmpDir + "LuminoFiles.wxs";

                string text = File.ReadAllText(t.WXSFileTemplate);
                text = text.Replace("%%Version%%", builder.VersionString);
                text = text.Replace("%%ProductGUID%%", t.ProductGUID);
                File.WriteAllText(installerWXS, text);

                string args = string.Format(
                    "dir {0} -dr {1} -cg LuminoFiles -gg -g1 -sfrag -srd -var \"var.LuminoFilesDir\" -out {2}",
                    t.ContentFilesDir,
                    t.TargetDirId,
                    contentFilesWXS);
                Utils.CallProcess(heat, args);

                args = string.Format("-nologo {0} -dLuminoFilesDir={1} -dPackageSourceDir={2} -out {0}.wixobj", installerWXS, t.ContentFilesDir, pkgSrcInstallerDir);
                Utils.CallProcess(candle, args);

                args = string.Format("-nologo {0} -dLuminoFilesDir={1} -dPackageSourceDir={2} -out {0}.wixobj", contentFilesWXS, t.ContentFilesDir, pkgSrcInstallerDir);
                Utils.CallProcess(candle, args);

                args = string.Format("-nologo -v -ext WixUIExtension -cultures:ja-jp {0}.wixobj {1}.wixobj -pdbout {0}.wixpdb -out {2}", installerWXS, contentFilesWXS, builder.LuminoPackageReleaseDir + t.Output);
                Utils.CallProcess(light, args);
            }
        }
    }
}
