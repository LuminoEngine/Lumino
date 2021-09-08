using LuminoBuild;
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
            if (Shell)
            {
                Utils.CallProcessShell(Program, Args);
                return null;
            }
            else
            {
                Logger.WriteLine($"{Program} {Args}");

                using (var p = new System.Diagnostics.Process())
                {
                    var sb = new StringBuilder();
                    var sbAll = new StringBuilder();
                    p.StartInfo.Arguments = Args;
                    p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                    p.StartInfo.UseShellExecute = false;
                    p.StartInfo.FileName = Program;

                    if (!p.StartInfo.UseShellExecute)
                    {
                        p.StartInfo.RedirectStandardOutput = true;
                        p.OutputDataReceived += (object sender, DataReceivedEventArgs e) => { 
                            if (!Silent) Console.WriteLine(e.Data);
                            sb.Append(e.Data);
                            sbAll.Append(e.Data);
                        };
                        p.StartInfo.RedirectStandardError = true;
                        p.ErrorDataReceived += (object sender, DataReceivedEventArgs e) => {
                            if (!Silent) Console.Error.WriteLine(e.Data);
                            sbAll.Append(e.Data);
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

                    if (p.ExitCode != 0)
                    {
                        Console.Error.WriteLine(sbAll.ToString());
                        Logger.WriteLineError($"Error: {Program} {Args}");
                        throw new InvalidOperationException($"Failed Process. ExitCode: {p.ExitCode}");
                    }
                    return sb.ToString();
                }
            }

        }
    }
}
