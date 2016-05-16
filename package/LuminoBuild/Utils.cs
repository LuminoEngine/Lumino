using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace LuminoBuild
{
    static class Utils
    {
        public static void CopyDirectory(string stSourcePath, string stDestPath, bool bOverwrite)
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
                foreach (string stCopyFrom in System.IO.Directory.GetFiles(stSourcePath))
                {
                    string stCopyTo = System.IO.Path.Combine(stDestPath, System.IO.Path.GetFileName(stCopyFrom));
                    System.IO.File.Copy(stCopyFrom, stCopyTo, true);
                }

                // 上書き不可能な場合は存在しない時のみコピーする
            }
            else {
                foreach (string stCopyFrom in System.IO.Directory.GetFiles(stSourcePath))
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
                CopyDirectory(stCopyFrom, stCopyTo, bOverwrite);
            }
        }

        public static void CallProcess(string program, string args)
        {
            using (Process p = new Process())
            {
                p.StartInfo.Arguments = args;
                p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.FileName = program;
                p.StartInfo.RedirectStandardOutput = true;
                p.OutputDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); };
                p.StartInfo.RedirectStandardError = true;
                p.ErrorDataReceived += (object sender, DataReceivedEventArgs e) => { Console.WriteLine(e.Data); };

                p.Start();
                p.BeginOutputReadLine();
                p.BeginErrorReadLine();

                p.WaitForExit();

                if (p.ExitCode != 0)
                    throw new InvalidOperationException("Failed Process.");
            }
        }
    }
}
