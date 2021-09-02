using System;
using System.Collections.Generic;
using System.IO;
using System.Diagnostics;
using System.Text;
using System.Linq;
using System.ComponentModel;
using System.IO.Compression;
using System.Runtime.InteropServices;
using System.Reflection;

namespace LuminoBuild
{

    class Build
    {
        // リリースのたびに変更する必要がある情報
        public string InstallerProductGUID = "f36ad49a-f86a-4196-aa55-07d8a61e02f3";
        public int MajorVersion = 0;
        public int MinorVersion = 10;
        public int RevisionVersion = 0;
        public int BuildVersion = 0;
        public string VersionString => string.Format("{0}.{1}.{2}", MajorVersion, MinorVersion, RevisionVersion);

        public readonly string RootDir;
        public readonly string BuildDir;
        public readonly string VcpkgDir;
        public readonly string EngineBuildDir;
        public readonly string EngineInstallDir;

        ////public string LuminoBuildCacheDir;
        //public string LuminoBindingsDir;
        //public string LuminoLibDir;
        //public string LuminoToolsDir;
        //public string LuminoDocDir;
        //public string LuminoSourceDir;
        //public string LuminoPackageDir;
        //public string LuminoPackageLibDir;
        //public string LuminoPackageSourceDir;
        //public string LuminoPackageReleaseDir;
        //public string LuminoExternalDir;
        //public List<BuildTask> Tasks = new List<BuildTask>();
        //public List<BuildRule> Rules = new List<BuildRule>();
        //public string[] Args;
        //public bool DirectTaskExecution { get { return Args.Contains("--direct-task-execution"); } }

        public string Triplet;

        public bool IsDebug = true;
        public bool IsRelease = true;
        public bool FromCI = false;


        public Build()
        {
            Triplet = "x64-windows";

            var thisAssembly = Assembly.GetEntryAssembly();
            var exeDir = Path.GetDirectoryName(thisAssembly.Location);
            RootDir = Path.GetFullPath(Path.Combine(exeDir, "../../../../../../")) + "/";
            BuildDir = Path.GetFullPath(Path.Combine(RootDir, "_build"));
            VcpkgDir = Path.GetFullPath(Path.Combine(BuildDir, "vcpkg"));
            EngineBuildDir = Path.GetFullPath(Path.Combine(BuildDir, Triplet, "EngineBuild"));
            EngineInstallDir = Path.GetFullPath(Path.Combine(BuildDir, Triplet, "EngineInstall"));

            Directory.CreateDirectory(BuildDir);
            Directory.CreateDirectory(EngineBuildDir);
            Directory.CreateDirectory(EngineInstallDir);
        }

        public void Call(string program, string args = "")
        {
            Utils.CallProcess(program, args);
        }

        public LuminoBuild.CurrentDir CurrentDir(string dir)
        {
            return LuminoBuild.CurrentDir.Enter(dir);
        }


        //public string LocalPackageName
        //{
        //    get
        //    {
        //        return "NativePackage";
        //    }
        //}

        //public string ReleasePackageName
        //{
        //    get
        //    {
        //        string targetEnvName;
        //        if (Utils.IsWin32)
        //            targetEnvName = "Windows";
        //        else
        //            targetEnvName = "macOS";

        //        return $"Lumino-latest-{targetEnvName}";
        //    }
        //}

        //public string GetExternalProjectBuildDir(string targetName)
        //{
        //    return Utils.ToUnixPath(Path.Combine(LuminoBuildDir, targetName, "ExternalBuild"));
        //}

        //public string GetExternalProjectBuildDir(string targetName, string externalProjectName)
        //{
        //    return Utils.ToUnixPath(Path.Combine(LuminoBuildDir, targetName, "ExternalBuild", externalProjectName));
        //}

        //public string GetExternalProjectInstallDir(string targetName, string externalProjectName)
        //{
        //    return Utils.ToUnixPath(Path.Combine(LuminoBuildDir, targetName, "ExternalInstall", externalProjectName));
        //}

        //public string GetExternalInstallDir(string targetName)
        //{
        //    return Utils.ToUnixPath(Path.Combine(LuminoBuildDir, targetName, "ExternalInstall"));
        //}

        ///// <summary>
        ///// ExternalProject など、ある単位がビルド完了していることをマークする。
        ///// CI 環境でのキャッシュのために用意したもの。
        ///// </summary>
        ///// <param name="name"></param>
        //public void CommitCache(string dirPath)
        //{
        //    File.WriteAllText(Path.Combine(dirPath, "_lnCacheCommitted"), "");
        //}

        //public bool ExistsCache(string dirPath)
        //{
        //    //if (!Directory.Exists(dirPath)) return false;
        //    return File.Exists(Path.Combine(dirPath, "_lnCacheCommitted"));
        //}



        //public void DoTaskOrRule(string name)
        //{
        //    var rule = Rules.Find((r) => r.Name == name);
        //    if (rule != null)
        //    {
        //        DoRule(name);
        //    }
        //    else
        //    {
        //        DoTask(name);
        //    }
        //}

        //public void DoTask(string name)
        //{
        //    try
        //    {
        //        Execute(name);
        //    }
        //    catch (Exception e)
        //    {
        //        Console.ForegroundColor = ConsoleColor.Red;
        //        Console.WriteLine(e.ToString());
        //        Console.ResetColor(); // 色のリセット
        //        throw new Exception($"[{name}] Task failed.");
        //    }
        //}
        //public void DoTask(BuildTask task)
        //{
        //    try
        //    {
        //        ExecuteTask(task);
        //    }
        //    catch (Exception e)
        //    {
        //        Console.ForegroundColor = ConsoleColor.Red;
        //        Console.WriteLine(e.ToString());
        //        Console.ResetColor(); // 色のリセット
        //        throw new Exception($"[{task.CommandName}] Task failed.");
        //    }
        //}

        //public void DoRule(string name)
        //{
        //    var rule = Rules.Find((r) => r.Name == name);
        //    try
        //    {
        //        Logger.WriteLine("[{0}] Rule started.", rule.Name);
        //        var sw = new System.Diagnostics.Stopwatch();
        //        sw.Start();

        //        rule.Build(this);

        //        sw.Stop();
        //        Logger.WriteLine("[{0}] Rule succeeded. ({1})", rule.Name, sw.Elapsed.ToString());
        //    }
        //    catch (Exception e)
        //    {
        //        Console.ForegroundColor = ConsoleColor.Red;
        //        Console.WriteLine(e.ToString());
        //        Console.ResetColor(); // 色のリセット
        //        throw new Exception($"[{name}] Rule failed.");
        //    }
        //}

        //private void Execute(string taskName)
        //{
        //    var tasks = ResoleveDependencies(taskName);

        //    if (DirectTaskExecution)
        //    {
        //        ExecuteTask(tasks.Last());
        //    }
        //    else
        //    {
        //        Console.WriteLine("Task execution order:");
        //        tasks.ForEach(x => Console.WriteLine("  " + x.CommandName));
        //        tasks.ForEach(x => ExecuteTask(x));
        //    }
        //}

        //private void ExecuteTask(BuildTask task)
        //{
        //    Logger.WriteLine("--------------------------------------------------------------------------------");
        //    Logger.WriteLine("[{0}] Task started.", task.CommandName);
        //    var sw = new System.Diagnostics.Stopwatch();
        //    sw.Start();

        //    string oldCD = Directory.GetCurrentDirectory();
        //    try
        //    {
        //        task.Build(this);
        //    }
        //    finally
        //    {
        //        Directory.SetCurrentDirectory(oldCD);
        //    }

        //    sw.Stop();
        //    Logger.WriteLine("[{0}] Task succeeded. ({1})", task.CommandName, sw.Elapsed.ToString());
        //    Logger.WriteLine("--------------------------------------------------------------------------------");
        //}

        //public bool HasFlagArgument(string name)
        //{
        //    return Args.Contains(name);
        //}



        //class NeedTaskInfo
        //{
        //    public BuildTask Task;
        //    public int Depth;
        //}

        //private List<BuildTask> ResoleveDependencies(string taskName)
        //{
        //    var resultList = new List<NeedTaskInfo>();
        //    var task = Tasks.Find((r) => r.CommandName == taskName);
        //    //resultList.Add(new NeedTaskInfo() { Task = task, Depth = 0 });
        //    ResoleveDependenciesHierarchical(resultList, task, 0);

        //    //foreach (var needTaskName in task.Dependencies)
        //    //{
        //    //    var needTask = Tasks.Find((r) => r.CommandName == needTaskName);

        //    //    if (resultList.Find((x) => x.Task == needTask) == null)
        //    //    {
        //    //        resultList.Add(new NeedTaskInfo() { Task = needTask, Depth = 1 });
        //    //    }

        //    //    ResoleveDependenciesHierarchical(resultList, needTask, 1);
        //    //}

        //    var stableSorted = resultList.OrderByDescending(x => x.Depth);
        //    return stableSorted.Select(x => x.Task).ToList();
        //}

        //private void ResoleveDependenciesHierarchical(List<NeedTaskInfo> resultList, BuildTask task, int depth)
        //{
        //    if (resultList.Find((x) => x.Task == task) == null)
        //    {
        //        resultList.Add(new NeedTaskInfo() { Task = task, Depth = depth });
        //    }

        //    var dependencies = task.Dependencies;
        //    if (dependencies != null)
        //    {
        //        foreach (var needTaskName in dependencies)
        //        {
        //            var needTask = Tasks.Find((r) => r.CommandName == needTaskName);

        //            ResoleveDependenciesHierarchical(resultList, needTask, depth + 1);
        //        }

        //    }
        //}
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
        public static void CopyFile2(string src, string dst)
        {
            File.Copy(src, dst, true);
            Console.WriteLine($"Copy: {src} to {dst}");
        }
        
        /// <summary>
        /// ファイルを別のフォルダへコピーする (ファイル名は変更しない)
        /// </summary>
        /// <param name="pattern">Pattern.</param>
        /// <param name="distDir">Dist dir.</param>
        public static void CopyFile(string srcFile, string dstDir)
        {
            var dst = dstDir + "/" + Path.GetFileName(srcFile);
            File.Copy(srcFile, dst, true);
            Console.WriteLine($"Copy: {srcFile} to {dst}");
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

        public static void GenerateFile(string dstFile, string templateFile, IEnumerable<KeyValuePair<string, string>> values)
        {
            string text = File.ReadAllText(templateFile);
            foreach (var vp in values)
            {
                text = text.Replace(vp.Key, vp.Value);
            }
            File.WriteAllText(dstFile, text);
        }

        /// <summary>
        /// ディレクトリ以下を全てコピーする
        /// </summary>
        /// <param name="stSourcePath">St source path.</param>
        /// <param name="stDestPath">St destination path.</param>
        /// <param name="bOverwrite">If set to <c>true</c> b overwrite.</param>
        public static void CopyDirectory(string stSourcePath, string stDestPath, bool bOverwrite = true, string pattern = "*", bool recursive = true)
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

            if (recursive)
            {
                foreach (string stCopyFrom in System.IO.Directory.GetDirectories(stSourcePath))
                {
                    string stCopyTo = System.IO.Path.Combine(stDestPath, System.IO.Path.GetFileName(stCopyFrom));
                    CopyDirectory(stCopyFrom, stCopyTo, bOverwrite, pattern);
                }
            }
        }

        /// <summary>
        /// ファイルをダウンロードする
        /// </summary>
        /// <param name="url"></param>
        /// <param name="fileName"></param>
        public static void DownloadFile(string url, string fileName)
        {
            Console.WriteLine($"Download: {url} to {fileName}");

            var request = System.Net.WebRequest.Create(url);
            var response = request.GetResponse();
            var stream = response.GetResponseStream();
            using (var file = new FileStream(fileName, FileMode.OpenOrCreate, FileAccess.Write))
            {
                int read;
                byte[] buffer = new byte[1024];
                while ((read = stream.Read(buffer, 0, buffer.Length)) > 0)
                {
                    file.Write(buffer, 0, read);
                }
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
        public static void ExtractZipFile(string zipFilePath, string dirPath, bool force = false)
        {
            if (!Directory.Exists(dirPath) || force)
            {
                Console.WriteLine($"Extract: {zipFilePath} to {dirPath}");
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

        public static void DeleteDirectory(string dirPath)
        {
            if (File.Exists(dirPath))
            {
                const int TryCount = 10;
                int i = 0;
                IOException e2 = null;
                for (; i < TryCount; i++)
                {
                    System.Threading.Thread.Sleep(2000);
                    try
                    {
                        Directory.Delete(dirPath, true);
                    }
                    catch (IOException e)
                    {
                        e2 = e;
                    }
                }

                if (i >= TryCount && e2 != null)
                {
                    throw e2;
                }

                Console.WriteLine($"Removed: {dirPath}");
            }
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

		public static string CallProcess(string program, string args = ""/*, bool useShell = false*/, Dictionary<string, string> environmentVariables = null, Action<StreamWriter> stdinWrite = null)
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
                if (stdinWrite != null)
                {
                    p.StartInfo.RedirectStandardInput = true;
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

                if (stdinWrite != null)
                {
                    stdinWrite(p.StandardInput);
                }

                if (!p.StartInfo.UseShellExecute)
                {
                    p.BeginOutputReadLine();
                    p.BeginErrorReadLine();
                }

				p.WaitForExit();

				if (p.ExitCode != 0)
					throw new InvalidOperationException($"Failed Process. ExitCode: {p.ExitCode}");
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
            Logger.WriteLine($"{program} {args}");

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

        public static void CallProcessShell(string program, string args = "", Dictionary<string, string> environmentVariables = null, Action<StreamWriter> stdinWrite = null)
        {
            //if (Utils.IsWin32)
            //{
            //    CallProcess("cmd", $" /c {program} {args}", environmentVariables, stdinWrite);
            //}
            //else
            //{
            //    throw new NotImplementedException();
            //}
            //Logger.WriteLine($"{program} {args}");

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


        [DllImport("libc", SetLastError = true)]
        public static extern int chmod(string pathname, int mode);

        // user permissions
        public const int S_IRUSR = 0x100;
        public const int S_IWUSR = 0x80;
        public const int S_IXUSR = 0x40;

        // group permission
        public const int S_IRGRP = 0x20;
        public const int S_IWGRP = 0x10;
        public const int S_IXGRP = 0x8;

        // other permissions
        public const int S_IROTH = 0x4;
        public const int S_IWOTH = 0x2;
        public const int S_IXOTH = 0x1;
        
        public const int S_0755 =
            S_IRUSR | S_IXUSR | S_IWUSR |
            S_IRGRP | S_IXGRP |
            S_IROTH | S_IXOTH;
	}

    class CurrentDir : IDisposable
    {
        private string _prev;

        public static CurrentDir Enter(string path)
        {
            var c = new CurrentDir() { _prev = Directory.GetCurrentDirectory() };
            Directory.SetCurrentDirectory(path);
            Console.WriteLine("Enter CurrentDir: " + path);
            return c;
        }

        public void Dispose()
        {
            Directory.SetCurrentDirectory(_prev);
            Console.WriteLine("Leave CurrentDir: " + _prev);
        }
    }
}
