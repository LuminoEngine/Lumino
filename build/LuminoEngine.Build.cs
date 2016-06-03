using System;
using System.IO;
using LuminoBuildTool;

class LuminoEngineRule : ModuleRule
{
    public string _msbuild;
    
    public override string Name
    {
        get { return "LuminoEngine"; }
    }

    public override void CheckPrerequisite()
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

    public override void Build(Builder builder)
    {
        string oldCD = Directory.GetCurrentDirectory();

        if (Utils.IsWin32)
        {
            var targets = new []
            {
               // new { DirName = "build_msvc120x86_MT", VSTarget = "Visual Studio 12", Unicode = "OFF" },
                //new { DirName = "build_msvc120x86u_MT", VSTarget = "Visual Studio 12", Unicode = "ON" },
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
        
        /*
        var dotnetDir = builder.RootDir + "DotNet/";
        var sandcastleDir = "D:/Program Files (x86)/EWSoftware/Sandcastle Help File Builder";

        // ライブラリをビルドする
        Logger.WriteLine("Building dll...");
        string sln = '"' + Path.GetFullPath(dotnetDir + "LuminoDotNet.sln") + '"';
        Utils.CallProcess(builder.MSBuildPath, "/t:Build /p:Configuration=\"Release\" /p:Platform=\"x86\" /m " + sln);

        // ドキュメントをビルドする
        if (Utils.IsWin32)
        {
            Logger.WriteLine("Building documents...");
            string shfbproj = '"' + dotnetDir + "LuminoDotNet.shfbproj" + '"';
            Utils.CallProcess(builder.MSBuildPath, "/p:Configuration=Release /property:SHFBROOT=\"" + sandcastleDir + "\" " + shfbproj);
        }

        // テスト出力場所に dll をコピーする
        string testOutputDir = dotnetDir + "Test/bin/x86/Release/";
        Utils.CopyFile(builder.LuminoLibDir + "Debug/LuminoC_x86uMTd.dll", testOutputDir);
        Utils.CopyFile(builder.LuminoLibDir + "Release/LuminoC_x86uMT.dll", testOutputDir);

        // テスト実行
        Logger.WriteLine("Running test...");
        Utils.CallProcess(testOutputDir + "Test.exe");
        */
    }
}
