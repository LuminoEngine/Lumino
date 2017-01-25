using System;
using System.IO;
using LuminoBuildTool;
using System.Text;
using System.IO.Compression;

class SetupDependencies : ModuleRule
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
        var extraName = builder.LuminoRootDir + "External";

        var wc = new System.Net.WebClient();
        wc.DownloadFile(
            "https://github.com/lriki/LuminoDependencies/releases/download/v1/LuminoDependencies.zip",
            zipPath);
        wc.Dispose();

        ZipFile.ExtractToDirectory(zipPath, extraName);
    }
}
