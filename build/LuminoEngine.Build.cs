using System;
using System.IO;
using LuminoBuildTool;

class LuminoEngineRule : ModuleRule
{
    public string _msbuild;

    /// <summary>
    /// ルールを実行するためのコマンド名
    /// </summary>
    public override string CommandName { get { return "cpp-b"; } }

    /// <summary>
    /// ルールの説明
    /// </summary>
    public override string Description { get { return "Build engine for C++."; } }

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
            var targets = new []
            {
                //new { DirName = "build_msvc120x86_MT", VSTarget = "Visual Studio 12", Unicode = "OFF", Platform="Win32" },
                //new { DirName = "build_msvc120x86u_MT", VSTarget = "Visual Studio 12", Unicode = "ON", Platform="Win32" },
                //new { DirName = "build_msvc120x64_MT", VSTarget = "Visual Studio 12 Win64", Unicode = "OFF" },
                //new { DirName = "build_msvc120x64u_MT", VSTarget = "Visual Studio 12 Win64", Unicode = "ON" },
                new { DirName = "build_msvc140x86_MT", VSTarget = "Visual Studio 14", Unicode = "OFF", Platform="Win32" },
                new { DirName = "build_msvc140x86u_MT", VSTarget = "Visual Studio 14", Unicode = "ON", Platform="Win32" },
                //new { DirName = "build_msvc140x64_MT", VSTarget = "Visual Studio 14 Win64", Unicode = "OFF" },
                //new { DirName = "build_msvc140x64u_MT", VSTarget = "Visual Studio 14 Win64", Unicode = "ON" },
            };

            // cmake で .sln を作ってビルドする
            foreach (var t in targets)
            {
                Directory.CreateDirectory(builder.LuminoBuildDir + t.DirName);
                Directory.SetCurrentDirectory(builder.LuminoBuildDir + t.DirName);
                Utils.CallProcess("cmake", string.Format("-G\"{0}\" -DLN_USE_UNICODE_CHAR_SET={1} -DLN_MSVC_LINK_MULTI_THREAD_STATIC_RUNTIME=ON ../..", t.VSTarget, t.Unicode));
                Utils.CallProcess(_msbuild, string.Format("Lumino.sln /t:Build /p:Configuration=\"Debug\" /p:Platform=\"{0}\" /m", t.Platform));
                Utils.CallProcess(_msbuild, string.Format("Lumino.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"{0}\" /m", t.Platform));
            }
        }
        else
        {
            throw new NotImplementedException();
        }

        Directory.SetCurrentDirectory(oldCD);
    }
}
