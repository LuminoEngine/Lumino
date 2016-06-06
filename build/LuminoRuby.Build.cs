using System;
using System.IO;
using LuminoBuildTool;

class LuminoRubyRule : ModuleRule
{
    public string _devKitDir;

    /// <summary>
    /// ルールの名前
    /// </summary>
    public override string Name
    {
        get { return "Ruby"; }
    }

    /// <summary>
    /// 前提条件の確認
    /// </summary>
    public override void CheckPrerequisite(Builder builder)
    {
        // ruby
        if (!Utils.ExistsProgram("ruby"))
        {
            Logger.WriteLineError("Not found ruby.");
            return;
        }

        // devkit
        string rubyDir = builder.LuminoBindingsDir + "Ruby/";
        if (Utils.TryCallProcessStdErr("ruby", '"' + rubyDir + "FindDevKit.rb" + '"', out _devKitDir) != 0)
        {
            Logger.WriteLineError("Not installed ruby dev kit.");
            return;
        }
        _devKitDir += "/";

        Buildable = true;
    }

    /// <summary>
    /// ビルド実行
    /// </summary>
    public override void Build(Builder builder)
    {
        string rubyDir = builder.LuminoBindingsDir + "Ruby/";
        string rubyBuildDir = rubyDir + "build/";

        // フルパスにしないと PATH を切れない
        var rubyBin = Path.GetFullPath(_devKitDir + "bin");
        var mingwBin = Path.GetFullPath(_devKitDir + "mingw/bin");

		var oldEnv = Environment.GetEnvironmentVariable("PATH");
        Environment.SetEnvironmentVariable("PATH", rubyBin + ";" + mingwBin + ";" + oldEnv);

        // build フォルダへ必要なファイルをコピーする
        Directory.CreateDirectory(rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.cpp", rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.h", rubyBuildDir);
        Utils.CopyFiles(rubyDir, "*.rb", rubyBuildDir);   // ドキュメント用のソースも。yardoc は readme.md を自動的に取り込んでしまう
        File.Copy(builder.LuminoLibDir + "x86/Release/LuminoC.dll", rubyBuildDir + "LuminoC.dll", true);

        // ビルド
        Logger.WriteLine("Building ext lib...");
        var oldDir = Directory.GetCurrentDirectory();
        Directory.SetCurrentDirectory(rubyBuildDir);
        Utils.CallProcess("ruby", "extconf.rb");
        Utils.CallProcess("make");

        // ドキュメントの作成
        Logger.WriteLine("Building documents...");
        Utils.CallProcessShell("yardoc", "-o doc LuminoRubyDoc.rb");

        // テスト実行
        Logger.WriteLine("Running test...");
        Utils.CallProcess("ruby", "Test.rb");

        // 変更したものを元に戻す
        Directory.SetCurrentDirectory(oldDir);
        Environment.SetEnvironmentVariable("PATH", oldEnv);
    }
}
