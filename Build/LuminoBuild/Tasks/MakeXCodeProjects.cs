using System;
using System.IO;
using LuminoBuild;

namespace LuminoBuild.Tasks
{
    class MakeXCodeProjects : BuildTask
    {
        /// <summary>
        /// ルールを実行するためのコマンド名
        /// </summary>
        public override string CommandName { get { return "MakeXCodeProjects"; } }

        /// <summary>
        /// ルールの説明
        /// </summary>
        public override string Description { get { return "Make xcode projects."; } }

        /// <summary>
        /// 前提条件の確認
        /// </summary>
        public override void CheckPrerequisite(Builder builder)
        {
            // cmake
            if (!Utils.ExistsProgram("cmake"))
            {
                Logger.WriteLineError("Not found cmake.");
                return;
            }

            Buildable = true;
        }

        /// <summary>
        /// ビルド実行
        /// </summary>
        public override void Build(Builder builder)
        {
            string oldCD = Directory.GetCurrentDirectory();

            if (Utils.IsMac)
            {
                var dir = Path.Combine(builder.LuminoBuildDir, "Xcode");

                Directory.CreateDirectory(dir);
                Directory.SetCurrentDirectory(dir);
                Utils.CallProcess("cmake", string.Format("-G \"Xcode\" ../.."));
            }
            else
            {
                throw new NotImplementedException();
            }

            Directory.SetCurrentDirectory(oldCD);
        }
    }
}
