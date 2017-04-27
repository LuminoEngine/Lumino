using System;
using System.IO;
using LuminoBuildTool;

class LuminoCRule : ModuleRule
{
    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "c-b"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Build engine for C."; } }

    /// <summary>
    /// 前提条件の確認
    /// </summary>
    public override void CheckPrerequisite(Builder builder)
    {
        // doxygen
        if (!Utils.ExistsProgram("doxygen"))
        {
            Logger.WriteLineError("Not found doxygen.");
            return;
        }

        Buildable = true;
    }

    /// <summary>
    /// ビルド実行
    /// </summary>
    public override void Build(Builder builder)
    {
        string capiDir = builder.LuminoBindingsDir + "C_API/";

        var oldCurDir = Directory.GetCurrentDirectory();
        Directory.SetCurrentDirectory(capiDir);

        // doc
        Utils.CallProcess("doxygen");
        
        Directory.SetCurrentDirectory(oldCurDir);
    }
}
