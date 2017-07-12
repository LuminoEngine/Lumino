using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace LuminoBuild.Tasks
{
    class MakeVersionHeader : BuildTask
    {
        /// <summary>
        /// ルールを実行するためのコマンド名
        /// </summary>
        public override string CommandName { get { return "MakeVersionHeader"; } }

        /// <summary>
        /// ルールの説明
        /// </summary>
        public override string Description { get { return ""; } }

        /// <summary>
        /// 前提条件の確認
        /// </summary>
        public override void CheckPrerequisite(Builder builder)
        {
            Buildable = true;
        }

        /// <summary>
        /// ビルド実行
        /// </summary>
        public override void Build(Builder builder)
        {
            string inFile = builder.LuminoRootDir + "/Source/LuminoEngine/Include/Lumino/Version.h.template";
            string outFile = builder.LuminoRootDir + "/Source/LuminoEngine/Include/Lumino/Version.h";

            string text = File.ReadAllText(inFile);
            text = text.Replace("%%MajorVersion%%", builder.MajorVersion.ToString());
            text = text.Replace("%%MinorVersion%%", builder.MinorVersion.ToString());
            text = text.Replace("%%RevisionVersion%%", builder.RevisionVersion.ToString());
            text = text.Replace("%%BuildVersion%%", builder.BuildVersion.ToString());
            text = text.Replace("%%VersionString%%", builder.VersionString);
            File.WriteAllText(outFile, text);
        }
    }
}
