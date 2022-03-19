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

        public CmdOptions Options;
        public string Triplet;
        public string Arch;
        public string System;

        public string RootDir;
        public string BuildDir;
        public string BuildToolsDir;
        public string VcpkgDir;
        public string EngineBuildDir;
        public string EngineInstallDir;

        public string LuminoBindingsDir;
        public string LuminoLibDir;
        public string LuminoToolsDir;
        public string LuminoDocDir;
        public string LuminoPackageDir;
        public string LuminoPackageLibDir;
        public string LuminoPackageSourceDir;
        public string LuminoPackageReleaseDir;
        public string LuminoExternalDir;
        public string[] Args;
        public bool DirectTaskExecution { get { return Args.Contains("--direct-task-execution"); } }

        public string LocalPackageName
        {
            get
            {
                return "NativePackage";
            }
        }
        
        public string ReleasePackageName
        {
            get
            {
                string targetEnvName;
                if (Utils.IsWin32)
                    targetEnvName = "Windows";
                else
                    targetEnvName = "macOS";

                return $"Lumino-latest-{targetEnvName}";
            }
        }

        public bool IsDesktopSystem { get => System == "windows" || System == "osx" || System == "linux"; }

        public Build(string triplet, CmdOptions options)
        {
            Options = options;
            Triplet = triplet;
            var tokens = triplet.Split("-");
            Arch = tokens[0];
            System = tokens[1];

            var thisAssembly = Assembly.GetEntryAssembly();
            var exeDir = Path.GetDirectoryName(thisAssembly.Location);
            RootDir = Path.GetFullPath(Path.Combine(exeDir, "../../../../../")) + "/";
            BuildDir = Path.GetFullPath(Path.Combine(RootDir, "build"));
            BuildToolsDir = Path.GetFullPath(Path.Combine(BuildDir, "tools"));
            VcpkgDir = (options.ExternalVcpkgDir != null) ? options.ExternalVcpkgDir : Path.GetFullPath(Path.Combine(BuildToolsDir, "vcpkg"));
            EngineBuildDir = Path.GetFullPath(Path.Combine(BuildDir, "buildtrees", Triplet, "lumino"));
            EngineInstallDir = Path.GetFullPath(Path.Combine(BuildDir, "installed", Triplet));

            Directory.CreateDirectory(BuildDir);
            Directory.CreateDirectory(BuildToolsDir);
            Directory.CreateDirectory(EngineBuildDir);
            Directory.CreateDirectory(EngineInstallDir);
        }






        public string GetExternalProjectBuildDir(string targetName)
        {
            return Utils.ToUnixPath(Path.Combine(BuildDir, targetName, "ExternalBuild"));
        }

        public string GetExternalProjectBuildDir(string targetName, string externalProjectName)
        {
            return Utils.ToUnixPath(Path.Combine(BuildDir, targetName, "ExternalBuild", externalProjectName));
        }

        public string GetExternalProjectInstallDir(string targetName, string externalProjectName)
        {
            return Utils.ToUnixPath(Path.Combine(BuildDir, targetName, "ExternalInstall", externalProjectName));
        }

        public string GetExternalInstallDir(string targetName)
        {
            return Utils.ToUnixPath(Path.Combine(BuildDir, targetName, "ExternalInstall"));
        }

        /// <summary>
        /// ExternalProject など、ある単位がビルド完了していることをマークする。
        /// CI 環境でのキャッシュのために用意したもの。
        /// </summary>
        /// <param name="name"></param>
        public void CommitCache(string dirPath)
        {
            File.WriteAllText(Path.Combine(dirPath, "_lnCacheCommitted"), "");
        }

        public bool ExistsCache(string dirPath)
        {
            //if (!Directory.Exists(dirPath)) return false;
            return File.Exists(Path.Combine(dirPath, "_lnCacheCommitted"));
        }



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

        public bool HasFlagArgument(string name)
        {
            return Args.Contains(name);
        }



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
}
