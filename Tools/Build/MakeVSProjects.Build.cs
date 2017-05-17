using System;
using System.IO;
using LuminoBuildTool;

class MakeVSProjectsRule : ModuleRule
{
    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "make-vs"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Make visual studio projects."; } }

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

        if (Utils.IsWin32)
        {
            // cmake で .sln を作ってビルドする
            foreach (var t in LuminoEngineRule.targets)
            {
                Directory.CreateDirectory(builder.LuminoBuildDir + t.DirName);
                Directory.SetCurrentDirectory(builder.LuminoBuildDir + t.DirName);
                Utils.CallProcess("cmake", string.Format("-G\"{0}\" -DLN_USE_UNICODE_CHAR_SET={1} -DLN_MSVC_STATIC_RUNTIME={2} ../..", t.VSTarget, t.Unicode, t.MSVCStaticRuntime));
            }
        }
        else
        {
            throw new NotImplementedException();
        }

        Directory.SetCurrentDirectory(oldCD);
    }
}
