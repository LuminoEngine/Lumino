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
    static class Logger
    {
        private static StreamWriter _writer;

        public static void Init(Build b)
        {
            _writer = new StreamWriter(Path.Combine(b.RootDir, "BuildLog.log"));
        }

        public static void Close()
        {
            if (_writer != null)
            {
                _writer.Close();
                _writer = null;
            }
        }

        public static void WriteLine()
        {
            Console.WriteLine();
        }

        public static void WriteLine(string text)
        {
            if (text == null) return;
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine(text);
            _writer.WriteLine(text);
            Console.ResetColor(); // 色のリセット
        }


        public static void WriteLine(string format, params object[] args)
        {
            if (format == null) return;
            Console.ForegroundColor = ConsoleColor.Green;
            Console.WriteLine(format, args);
            _writer.WriteLine(format, args);
            Console.ResetColor(); // 色のリセット
        }

        public static void WriteLineError(string text)
        {
            if (text == null) return;
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(text);
            _writer.WriteLine(text);
            Console.ResetColor();
        }

        public static void WriteLineError(string format, params object[] args)
        {
            if (format == null) return;
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine(format, args);
            _writer.WriteLine(format, args);
            Console.ResetColor();
        }

        public static void WriteLineV(string text)
        {
            if (text == null) return;
            Console.ForegroundColor = ConsoleColor.White;
            Console.WriteLine(text);
            _writer.WriteLine(text);
            Console.ResetColor(); // 色のリセット
        }

        public static void WriteLineV(string format, params object[] args)
        {
            if (format == null) return;
            Console.ForegroundColor = ConsoleColor.White;
            Console.WriteLine(format, args);
            _writer.WriteLine(format, args);
            Console.ResetColor(); // 色のリセット
        }
    }
}
