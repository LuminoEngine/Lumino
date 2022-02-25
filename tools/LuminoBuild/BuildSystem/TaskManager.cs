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
        public abstract void Build(Build builder);
    }


    class TaskManager
    {
        public List<BuildTask> Tasks = new List<BuildTask>();

        public void DoTask(Build buildSystem, string name)
        {
            try
            {
                var tasks = CollectDepends(name);
                foreach (var task in tasks)
                {
                    Execute(buildSystem, task);
                }
            }
            catch (Exception e)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Logger.WriteLineV(e.ToString());
                Console.ResetColor(); // 色のリセット
                throw new Exception($"[{name}] Task failed.");
            }
        }

        private BuildTask GetTask(string name)
        {
            var task = Tasks.Find((r) => r.CommandName == name);
            if (task == null) throw new Exception($"{name} task is not found.");
            return task;
        }

        private List<BuildTask> CollectDepends(string entryTaskName)
        {
            var result = new List<BuildTask>();
            var entryTask = GetTask(entryTaskName);
            var next = new List<BuildTask>() { entryTask };
            var level = 0;
            while (next.Any())
            {
                var depends = new List<BuildTask>();
                foreach (var task in next)
                {
                    // 次に調べたい対象 Task を取り出しておく
                    foreach (var d in task.Depends)
                    {
                        depends.Add(GetTask(d));
                    }

                    var r = result.Find(x => x == task);
                    if (r != null)
                    {
                        // 既に登録されていた場合は、今回のレベルまで実行を早めたい
                        r.DependLevel = level;
                    }
                    else
                    {
                        // 結果として登録
                        task.DependLevel = level;
                        result.Add(task);
                    }
                }

                next = depends;
                level++;
            }

            // 降順でソート。entryTask が一番最後に実行されるようにする。
            // 同一 Level は順不同。
            result.Sort((a, b) => b.DependLevel - a.DependLevel);
            return result;
        }

        private void Execute(Build buildSystem, BuildTask task)
        {
            Logger.WriteLine();
            Logger.WriteLine("================================================================================");
            Logger.WriteLine("[{0}] Task started.", task.CommandName);
            Logger.WriteLine();
            var sw = new System.Diagnostics.Stopwatch();
            sw.Start();
            string oldCD = Directory.GetCurrentDirectory();
            try
            {
                task.Build(buildSystem);
            }
            catch (Exception e)
            {
                throw;
            }
            finally
            {
                Directory.SetCurrentDirectory(oldCD);
            }
            sw.Stop();
            Logger.WriteLine();
            Logger.WriteLine("[{0}] Task succeeded. (time: {1})", task.CommandName, sw.Elapsed.ToString(@"hh\:mm\:ss"));
            Logger.WriteLine("================================================================================");
            Logger.WriteLine();
        }
    }
}
