using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Text;

namespace LuminoBuild
{
    class Proc
    {
        public string Program { get; private set; }

        public string Args { get; private set; }

        public bool Shell { get; private set; } = false;

        public bool Silent { get; private set; } = false;

        public string StdErrorString { get; private set; }


        public static Proc Make(string program)
        {
            var p = new Proc(program);
            return p;
        }

        public static Proc Make(string program, string args)
        {
            var p = new Proc(program);
            p.Args = args;
            return p;
        }

        public Proc(string program)
        {
            Program = program;
        }

        public Proc WithShell(bool enabled = true)
        {
            Shell = enabled;
            return this;
        }
        public Proc WithSilent(bool enabled = true)
        {
            Silent = enabled;
            return this;
        }

        public string Call(Dictionary<string, string> environmentVariables = null, Action<StreamWriter> stdinWrite = null)
        {
            Logger.WriteLine($"{Program} {Args}");

            using (var p = new System.Diagnostics.Process())
            {
                var sb = new StringBuilder();
                var sbAll = new StringBuilder();
                var stdErr = new StringBuilder();
                p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                p.StartInfo.UseShellExecute = false;

                if (Shell)
                {
                    p.StartInfo.FileName = "cmd";
                    p.StartInfo.Arguments = $" /c {Program} {Args}";
                }
                else
                {
                    p.StartInfo.FileName = Program;
                    p.StartInfo.Arguments = Args;
                }

                if (!p.StartInfo.UseShellExecute)
                {
                    p.StartInfo.RedirectStandardOutput = true;
                    p.OutputDataReceived += (object sender, DataReceivedEventArgs e) =>
                    {
                        if (!Silent) Console.WriteLine(e.Data);
                        sb.Append(e.Data);
                        sbAll.Append(e.Data + Environment.NewLine);
                    };
                    p.StartInfo.RedirectStandardError = true;
                    p.ErrorDataReceived += (object sender, DataReceivedEventArgs e) =>
                    {
                        if (!Silent) Console.Error.WriteLine(e.Data);
                        stdErr.Append(e.Data + Environment.NewLine);
                        sbAll.Append(e.Data + Environment.NewLine);
                    };
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

                StdErrorString = stdErr.ToString();

                if (p.ExitCode != 0)
                {
                    Console.Error.WriteLine(sbAll.ToString());
                    Logger.WriteLineError($"Error: {Program} {Args}");
                    throw new InvalidOperationException($"Failed Process. ExitCode: {p.ExitCode}");
                }
                return sb.ToString();
            }
        }

        public static string Call(string command, bool silent = false)
        {
            var space1 = command.IndexOf(" ");
            if (space1 >= 0)
            {
                var program = command.Substring(0, space1);
                var args = command.Substring(space1 + 1);
                return Make(program, args).WithShell().WithSilent(silent).Call();
            }
            else
            {
                return Make(command).WithShell().WithSilent(silent).Call();
            }
        }
    }
}
