using System;
using System.IO;
using System.Linq;
using LuminoBuild;

class CMakeTargetInfo
{
    public string DirName;
    public string VSTarget;
    public string Unicode;
    public string Platform;
    public string MSVCStaticRuntime;
}

class LuminoEngineRule : BuildTask
{
    public string _msbuild;

    public static CMakeTargetInfo[] targets = new CMakeTargetInfo[]
    {
        new CMakeTargetInfo { DirName = "Build_MSVC120x86_MT", VSTarget = "Visual Studio 12", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "ON" },
        new CMakeTargetInfo { DirName = "Build_MSVC120x86_MD", VSTarget = "Visual Studio 12", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "OFF" },
        new CMakeTargetInfo { DirName = "Build_MSVC140x86_MT", VSTarget = "Visual Studio 14", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "ON" },
        new CMakeTargetInfo { DirName = "Build_MSVC140x86_MD", VSTarget = "Visual Studio 14", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "OFF" },
        new CMakeTargetInfo { DirName = "Build_MSVC150x86_MT", VSTarget = "Visual Studio 15", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "ON" },
        new CMakeTargetInfo { DirName = "Build_MSVC150x86_MD", VSTarget = "Visual Studio 15", Unicode = "ON", Platform="Win32", MSVCStaticRuntime = "OFF" },
    };

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
            var list = targets.Where(t => t.DirName.Contains("x86_MT"));

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
