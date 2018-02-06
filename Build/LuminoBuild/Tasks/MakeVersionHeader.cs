using System;
using System.IO;
using System.Text;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class MakeVersionHeader : BuildTask
    {
        public override string CommandName { get { return "MakeVersionHeader"; } }

        public override string Description { get { return "MakeVersionHeader."; } }

        public override void Build(Builder builder)
        {
            // make Version.h
            {
                string inFile = builder.LuminoRootDir + "/Source/LuminoEngine/Include/Lumino/Version.h.template";
                string outFile = builder.LuminoRootDir + "/Source/LuminoEngine/Include/Lumino/Version.h";

                string text = File.ReadAllText(inFile);
                text = text.Replace("%%MajorVersion%%", builder.MajorVersion.ToString());
                text = text.Replace("%%MinorVersion%%", builder.MinorVersion.ToString());
                text = text.Replace("%%RevisionVersion%%", builder.RevisionVersion.ToString());
                text = text.Replace("%%BuildVersion%%", builder.BuildVersion.ToString());
                text = text.Replace("%%VersionString%%", builder.VersionString);
                File.WriteAllText(outFile, text, new UTF8Encoding(true));
            }
        }
    }
}
