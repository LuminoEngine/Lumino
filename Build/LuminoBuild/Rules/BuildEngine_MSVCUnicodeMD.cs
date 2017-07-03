using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    class BuildEngine_MSVCUnicodeMD : ModuleRule
    {
        public string _msbuild;

        /// <summary>
        /// ルールを実行するためのコマンド名
        /// </summary>
        public override string CommandName { get { return "BuildEngine_MSVCUnicodeMD"; } }

        /// <summary>
        /// ルールの説明
        /// </summary>
        public override string Description { get { return ""; } }

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

            // MSBuild
            _msbuild = Utils.FindMSBuild();
            if (_msbuild == null)
            {
                Logger.WriteLineError("Not found MSBuild or xbuild.");
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

            if (Utils.IsWin32)
            {
                var list = LuminoEngineRule.targets.Where(t => t.DirName.Contains("x86U_MD"));

                // cmake で .sln を作ってビルドする
                foreach (var t in list)
                {
                    Directory.CreateDirectory(builder.LuminoBuildDir + t.DirName);
                    Directory.SetCurrentDirectory(builder.LuminoBuildDir + t.DirName);
                    if (Utils.TryCallProcess("cmake", string.Format("-G\"{0}\" -DLN_USE_UNICODE_CHAR_SET={1} -DLN_MSVC_STATIC_RUNTIME={2} ../..", t.VSTarget, t.Unicode, t.MSVCStaticRuntime)) == 0)
                    {
                        Utils.CallProcess(_msbuild, string.Format("Lumino.sln /t:Build /p:Configuration=\"Debug\" /p:Platform=\"{0}\" /m", t.Platform));
                        Utils.CallProcess(_msbuild, string.Format("Lumino.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"{0}\" /m", t.Platform));
                    }
                }
            }
            else
            {
                throw new NotImplementedException();
            }

            Directory.SetCurrentDirectory(oldCD);
        }
    }

}
