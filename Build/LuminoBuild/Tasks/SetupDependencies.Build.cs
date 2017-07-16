using System;
using System.IO;
using LuminoBuild;
using System.Text;
using System.IO.Compression;

class SetupDependencies : BuildTask
{
    public override string CommandName { get { return "setup"; } }
    
    public override string Description { get { return "Setup dependencies."; } }
    
    public override void CheckPrerequisite(Builder builder)
    {
        Buildable = true;
    }
    
    public override void Build(Builder builder)
    {
        var zipPath = builder.LuminoRootDir + "External/LuminoDependencies.zip";
        if (File.Exists(zipPath)) return;   // もう完了している

        var extraName = builder.LuminoRootDir + "External";

        Logger.WriteLine("Downloading dependencies...");
        var wc = new System.Net.WebClient();
        wc.DownloadFile(
            "http://nnmy.sakura.ne.jp/archive/dependencies/LuminoDependencies.zip",
            zipPath);
        wc.Dispose();

        Logger.WriteLine("Extracting...");
        ZipFile.ExtractToDirectory(zipPath, extraName);


        // .lib
        Logger.WriteLine("copy dependencies lib files...");
        foreach (string dir in Directory.GetDirectories(builder.LuminoRootDir + "External/LuminoDependencies"))
        {
            // 今のところ LuminoDependencies/ライブラリ名/lib でフォルダ構成は統一されている。
            string libDir = dir + "/lib";
            if (Directory.Exists(libDir))
            {
                Utils.CopyDirectory(libDir, builder.LuminoRootDir + "lib");
            }
        }

        // extract Tools
        Utils.ExtractZipFile(builder.LuminoDependenciesDir + "Tools/wix311-binaries.zip", builder.LuminoDependenciesDir + "Tools/wix311-binaries");
    }
}
