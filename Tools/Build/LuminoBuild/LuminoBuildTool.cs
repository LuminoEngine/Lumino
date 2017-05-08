using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Text;
using System.ComponentModel;
using System.IO.Compression;

namespace LuminoBuildTool
{
	class Builder
    {
        public string VersionString;
        public string LuminoRootDir;
        public string LuminoBuildDir;
        public string LuminoBindingsDir;
        public string LuminoLibDir;
        public string LuminoToolsDir;
        public string LuminoDocDir;
        public string LuminoPackageDir;
        public string LuminoPackageSourceDir;
        public string LuminoPackageReleaseDir;
        public List<ModuleRule> Rules = new List<ModuleRule>();

        public void Execute(string commands)
        {
            var rules = new List<ModuleRule>();

            if (commands == "all")
            {
                foreach (var rule in Rules)
                {
                    rules.Add(rule);
                }
            }
            else
            {
                string[] list = commands.Split(',');
                foreach (var cmd in list)
                {
                    var rule = Rules.Find((r) => r.CommandName == cmd);
                    if (rule != null) rules.Add(rule);
                }
            }

            foreach (var rule in rules)
            {
                rule.CheckPrerequisite(this);
            }
            foreach (var rule in rules)
            {
                if (rule.Buildable)
                {
                    Logger.WriteLine("[{0}] Rule started.", rule.CommandName);
                    rule.Build(this);
                    Logger.WriteLine("[{0}] Rule succeeded.", rule.CommandName);
                }
            }
        }

        public void CheckPrerequisite()
        {
            foreach (var rule in Rules)
            {
                rule.CheckPrerequisite(this);
            }
        }

        public void Build()
        {
            foreach (var rule in Rules)
            {
                if (rule.Buildable)
                {
                    Logger.WriteLine("[{0}] Rule started.", rule.CommandName);
                    rule.Build(this);
                    Logger.WriteLine("[{0}] Rule succeeded.", rule.CommandName);
                }
            }
        }
    }

    abstract class ModuleRule
    {
        /// <summary>
        /// ルールを実行するためのコマンド名
        /// </summary>
        public abstract string CommandName { get; }

        /// <summary>
        /// ルールの説明
        /// </summary>
        public abstract string Description { get; }

        /// <summary>
        /// ビルドできるか (CheckPrerequisite() で確定する)
        /// </summary>
        public bool Buildable { get; protected set; }

        /// <summary>
        /// 前提条件の確認
        /// </summary>
        /// <returns>この Rule を実行できるなら true</returns>
        public abstract void CheckPrerequisite(Builder builder);

        /// <summary>
        /// このルールをビルドする
        /// </summary>
        /// <param name="builder">Builder.</param>
		public abstract void Build(Builder builder);
	}

    static class Logger
    {
        public static void WriteLine(string format, params object[] args)
        {
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine(format, args);
            Console.ResetColor(); // 色のリセット
        }
        public static void WriteLineError(string format, params object[] args)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(format, args);
            Console.ResetColor();
        }
    }

    static class Utils
    {
        /// <summary>
        /// ファイルを別のフォルダへコピーする (ファイル名は変更しない)
        /// </summary>
        /// <param name="pattern">Pattern.</param>
        /// <param name="distDir">Dist dir.</param>
        public static void CopyFile(string srcFile, string dstDir)
        {
            File.Copy(srcFile, dstDir + "/" + Path.GetFileName(srcFile), true);
        }

        /// <summary>
        /// パターンに一致するファイル全てをコピーする
        /// </summary>
        /// <param name="pattern">Pattern.</param>
        /// <param name="distDir">Dist dir.</param>
        public static void CopyFiles(string srcDir, string pattern, string dstDir)
        {
            foreach (string file in Directory.GetFiles(srcDir, pattern))
            {
                File.Copy(file, dstDir + "/" + Path.GetFileName(file), true);
            }
        }

        /// <summary>
        /// ディレクトリ以下を全てコピーする
        /// </summary>
        /// <param name="stSourcePath">St source path.</param>
        /// <param name="stDestPath">St destination path.</param>
        /// <param name="bOverwrite">If set to <c>true</c> b overwrite.</param>
        public static void CopyDirectory(string stSourcePath, string stDestPath, bool bOverwrite = true, string pattern = "*")
        {
            // コピー先のディレクトリがなければ作成する
            if (!System.IO.Directory.Exists(stDestPath))
            {
                System.IO.Directory.CreateDirectory(stDestPath);
                System.IO.File.SetAttributes(stDestPath, System.IO.File.GetAttributes(stSourcePath));
                bOverwrite = true;
            }

            // コピー元のディレクトリにあるすべてのファイルをコピーする
            if (bOverwrite)
            {
                foreach (string stCopyFrom in System.IO.Directory.GetFiles(stSourcePath, pattern))
                {
                    string stCopyTo = System.IO.Path.Combine(stDestPath, System.IO.Path.GetFileName(stCopyFrom));
                    System.IO.File.Copy(stCopyFrom, stCopyTo, true);
                }

                // 上書き不可能な場合は存在しない時のみコピーする
            }
            else
            {
                foreach (string stCopyFrom in System.IO.Directory.GetFiles(stSourcePath, pattern))
                {
                    string stCopyTo = System.IO.Path.Combine(stDestPath, System.IO.Path.GetFileName(stCopyFrom));

                    if (!System.IO.File.Exists(stCopyTo))
                    {
                        System.IO.File.Copy(stCopyFrom, stCopyTo, false);
                    }
                }
            }

            // コピー元のディレクトリをすべてコピーする (再帰)
            foreach (string stCopyFrom in System.IO.Directory.GetDirectories(stSourcePath))
            {
                string stCopyTo = System.IO.Path.Combine(stDestPath, System.IO.Path.GetFileName(stCopyFrom));
                CopyDirectory(stCopyFrom, stCopyTo, bOverwrite, pattern);
            }
        }

        /// <summary>
        /// フォルダから .zip を作る
        /// </summary>
        public static void CreateZipFile(string dirPath, string zipFilePath, bool includeBaseDirectory = true)
        {
            File.Delete(zipFilePath);
            ZipFile.CreateFromDirectory(dirPath, zipFilePath, CompressionLevel.Optimal, includeBaseDirectory);
        }

        /// <summary>
        /// 指定した名前のコマンドが使用できるか確認する
        /// </summary>
        /// <returns><c>true</c>, if program was existsed, <c>false</c> otherwise.</returns>
        /// <param name="program">Program.</param>
        public static bool ExistsProgram(string program)
        {
            using (Process p = new Process())
            {
                if (IsWin32)
                    p.StartInfo.FileName = "where";
                else
                    p.StartInfo.FileName = "which";
                p.StartInfo.Arguments = program;
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.CreateNoWindow = true;

                p.Start();
                p.WaitForExit();
                return p.ExitCode == 0;
            }
        }

		public static string CallProcess(string program, string args = "")
		{
			using (Process p = new Process())
			{
                var sb = new StringBuilder();
				p.StartInfo.Arguments = args;
				p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.FileName = program;
				p.StartInfo.RedirectStandardOutput = true;
				p.OutputDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); sb.Append(e.Data); };
				p.StartInfo.RedirectStandardError = true;
				p.ErrorDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); };

				p.Start();
				p.BeginOutputReadLine();
				p.BeginErrorReadLine();

				p.WaitForExit();

				if (p.ExitCode != 0)
					throw new InvalidOperationException("Failed Process.");
                return sb.ToString();
			}
		}

        public static int TryCallProcess(string program, string args)
        {
            string dummy;
            return TryCallProcessStdErr(program, args, out dummy);
        }

        public static int TryCallProcessStdErr(string program, string args, out string outStdErr)
        {
            using (Process p = new Process())
            {
                var sb = new StringBuilder();
                p.StartInfo.Arguments = args;
                p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.FileName = program;
                p.StartInfo.RedirectStandardOutput = true;
                p.OutputDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); };
                p.StartInfo.RedirectStandardError = true;
                p.ErrorDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); sb.Append(e.Data); };

                p.Start();
                p.BeginOutputReadLine();
                p.BeginErrorReadLine();

                p.WaitForExit();

                outStdErr = sb.ToString();
                return p.ExitCode;
            }
        }

        public static void CallProcessShell(string program, string args = "")
        {
            using (Process p = new Process())
            {
                p.StartInfo.FileName = program;
                p.StartInfo.Arguments = args;

                p.Start();

                p.WaitForExit();

                if (p.ExitCode != 0)
                    throw new InvalidOperationException("Failed Process.");
            }
        }

        /// <summary>
        /// Windows 上で実行されているか
        /// </summary>
        public static bool IsWin32
        {
            get
            {
                if (Path.DirectorySeparatorChar == '\\')
                    return true;
                else
                    return false;
            }
        }

        /// <summary>
        /// MSBuild または mono の xbuild を探す
        /// </summary>
        /// <returns>The MS build path.</returns>
        public static string FindMSBuild()
        {
            if (Utils.IsWin32)
            {
                //var regkey = Microsoft.Win32.Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Microsoft\MSBuild\ToolsVersions\14.0");
                //return (string)regkey.GetValue("MSBuildToolsPath") + "MSBuild.exe";
                var regkey = Microsoft.Win32.Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7");
                return (string)regkey.GetValue("15.0") + @"MSBuild\15.0\Bin\MSBuild.exe";
            }
            else
            {
                if (!Utils.ExistsProgram("xbuild"))
                {
                    Logger.WriteLineError("Not found xbuild.");
                    return null;
                }
                return "xbuild";
            }
        }
	}
}
