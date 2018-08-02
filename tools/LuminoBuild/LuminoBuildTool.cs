using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Text;
using System.ComponentModel;
using System.IO.Compression;
using System.Runtime.InteropServices;

namespace LuminoBuild
{
	class Builder
    {
        public int MajorVersion;
        public int MinorVersion;
        public int RevisionVersion;
        public int BuildVersion;
        public string VersionString;

        public string LuminoRootDir;
        public string LuminoBuildDir;
        public string LuminoBindingsDir;
        public string LuminoLibDir;
        public string LuminoToolsDir;
        public string LuminoDocDir;
        public string LuminoPackageDir;
        public string LuminoPackageLibDir;
        public string LuminoPackageSourceDir;
        public string LuminoPackageReleaseDir;
        public string LuminoExternalDir;
        public List<BuildTask> Tasks = new List<BuildTask>();
        public List<BuildRule> Rules = new List<BuildRule>();

        public void DoTaskOrRule(string name)
        {
            var rule = Rules.Find((r) => r.Name == name);
            if (rule != null)
            {
                DoRule(name);
            }
            else
            {
                DoTask(name);
            }
        }

        public void DoTask(string name)
        {
            try
            {
                Execute(name);
            }
            catch (Exception e)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine(e.ToString());
                Console.ResetColor(); // 色のリセット
                throw new Exception("DoTask failed.");
            }
        }

        public void DoRule(string name)
        {
            var rule = Rules.Find((r) => r.Name == name);
            try
            {
                Logger.WriteLine("[{0}] Rule started.", rule.Name);
                rule.Build(this);
                Logger.WriteLine("[{0}] Rule succeeded.", rule.Name);
            }
            catch (Exception e)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine(e.ToString());
                Console.ResetColor(); // 色のリセット
            }
        }

        private void Execute(string commands)
        {
            var tasks = new List<BuildTask>();

            if (commands == "all")
            {
                foreach (var rule in Tasks)
                {
                    tasks.Add(rule);
                }
            }
            else
            {
                string[] list = commands.Split(',');
                foreach (var cmd in list)
                {
                    var task = Tasks.Find((r) => r.CommandName == cmd);
                    if (task != null) tasks.Add(task);
                }
            }

            foreach (var task in tasks)
            {
                task.CheckPrerequisite(this);
            }
            foreach (var task in tasks)
            {
                if (task.Buildable)
                {
                    Logger.WriteLine("[{0}] Task started.", task.CommandName);
                    string oldCD = Directory.GetCurrentDirectory();
                    try
                    {
                        task.Build(this);
                    }
                    finally
                    {
                        Directory.SetCurrentDirectory(oldCD);
                    }
                    Logger.WriteLine("[{0}] Task succeeded.", task.CommandName);
                }
            }
        }

        public void CheckPrerequisite()
        {
            foreach (var rule in Tasks)
            {
                rule.CheckPrerequisite(this);
            }
        }

        //public void Build()
        //{
        //    foreach (var rule in Tasks)
        //    {
        //        if (rule.Buildable)
        //        {
        //            Logger.WriteLine("[{0}] Rule started.", rule.CommandName);
        //            rule.Build(this);
        //            Logger.WriteLine("[{0}] Rule succeeded.", rule.CommandName);
        //        }
        //    }
        //}
    }


    /// <summary>
    /// ビルドタスク
    /// </summary>
    /// <remarks>
    /// タスク自体は依存関係を定義しない。一般的なビルドツールとはやや異なるが。
    /// これは、特定のタスクだけを繰り返し実行できるようにするため。例えば Ruby バインダのデバッグ中、これだけビルドしなおすなど。
    /// </remarks>
    abstract class BuildTask
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
        public virtual void CheckPrerequisite(Builder builder) { Buildable = true; }

        /// <summary>
        /// このルールをビルドする
        /// </summary>
        /// <param name="builder">Builder.</param>
		public abstract void Build(Builder builder);
	}

    /// <summary>
    /// 一連の BuildTask
    /// </summary>
    abstract class BuildRule
    {
        /// <summary>
        /// 名前
        /// </summary>
        public abstract string Name { get; }

        /// <summary>
        /// 実行
        /// </summary>
        /// <param name="builder"></param>
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
        public static string ToUnixPath(string path)
        {
            return path.Replace("\\", "/");
        }
        public static string ToWin32Path(string path)
        {
            return path.Replace("/", "\\");
        }

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
            zipFilePath = Path.GetFullPath(zipFilePath);
            File.Delete(zipFilePath);
            ZipFile.CreateFromDirectory(Path.GetFullPath(dirPath), zipFilePath, CompressionLevel.Optimal, includeBaseDirectory);
        }

        /// <summary>
        /// フォルダから .zip を作る
        /// </summary>
        public static void ExtractZipFile(string zipFilePath, string dirPath)
        {
            if (!Directory.Exists(dirPath))
            {
                ZipFile.ExtractToDirectory(zipFilePath, dirPath);
            }
        }

        public static void MoveFileForce(string sourceFileName, string destFileName)
        {
            if (File.Exists(destFileName))
            {
                File.Delete(destFileName);
            }

            File.Move(sourceFileName, destFileName);
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

		public static string CallProcess(string program, string args = ""/*, bool useShell = false*/, Dictionary<string, string> environmentVariables = null)
		{
            Logger.WriteLine($"{program} {args}");
            
            using (Process p = new Process())
			{
                var sb = new StringBuilder();
				p.StartInfo.Arguments = args;
				p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
				p.StartInfo.UseShellExecute = false;
				p.StartInfo.FileName = program;

                if (!p.StartInfo.UseShellExecute)
                {
                    p.StartInfo.RedirectStandardOutput = true;
                    p.OutputDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); sb.Append(e.Data); };
                    p.StartInfo.RedirectStandardError = true;
                    p.ErrorDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); };
                }

                if (environmentVariables != null)
                {
                    foreach (var pair in environmentVariables)
                    {
                        if (p.StartInfo.EnvironmentVariables.ContainsKey(pair.Key))
                        {
                            p.StartInfo.EnvironmentVariables[pair.Key] = pair.Value;
                        }
                        else
                        {
                            p.StartInfo.EnvironmentVariables.Add(pair.Key, pair.Value);
                        }
                    }
                }

				p.Start();

                if (!p.StartInfo.UseShellExecute)
                {
                    p.BeginOutputReadLine();
                    p.BeginErrorReadLine();
                }

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
                p.StartInfo.UseShellExecute = true;

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

		[DllImport("libc")]
		static extern int uname(IntPtr buf);

		public static bool IsMac
		{
            get
            {
                IntPtr buf = IntPtr.Zero;
                try
                {
                    buf = Marshal.AllocHGlobal(8192);
                    // This is a hacktastic way of getting sysname from uname ()
                    if (uname(buf) == 0)
                    {
                        string os = Marshal.PtrToStringAnsi(buf);
                        if (os == "Darwin")
                            return true;
                    }
                }
                catch
                {
                }
                finally
                {
                    if (buf != IntPtr.Zero)
                        Marshal.FreeHGlobal(buf);
                }
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
                //var regkey = Microsoft.Win32.Registry.LocalMachine.OpenSubKey(@"SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7");
                //return (string)regkey.GetValue("15.0") + @"MSBuild\15.0\Bin\MSBuild.exe";

                if (!Utils.ExistsProgram("MSBuild"))
                {
                    Logger.WriteLineError("Not found MSBuild.");
                    return null;
                }
                return "MSBuild";
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
