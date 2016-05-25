using System;
using System.IO;
using System.Diagnostics;

namespace LuminoBuildTool
{
	class Builder
	{
		public string RootDir;
		public string LuminoLibDir;
	}
		
	abstract class ModuleRule
	{
		public abstract void Build(Builder builder);
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

		public static void CallProcess(string program, string args = "")
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
