using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Linq;
using LuminoBuild.Tasks;

namespace LuminoBuild
{

    /*
     * ./emsdk list
     */
    static class EmscriptenEnv
    {
        public const string emsdkVer = "3.1.56";
        public const string emVer = "3.1.56";
        public static string EmsdkDir;
        public static string Ninja;


        static public void Setup(Build b)
        {
            EmsdkDir = Path.Combine(b.BuildToolsDir, "emsdk");
            if (!Directory.Exists(EmsdkDir))
            {
                using (CurrentDir.Enter(b.BuildToolsDir))
                {
                    Proc.Make("git", "clone https://github.com/emscripten-core/emsdk.git").WithSilent().Call();

                    using (CurrentDir.Enter(EmsdkDir))
                    {
                        Proc.Make("emsdk", "install " + emsdkVer)
                            .WithShell()
                            .WithSilent()
                            .Call();
                    }
                }
            }



            var var_PATH = new List<string>();
            var var_EMSDK = "";
            var var_EM_CONFIG = "";
            var var_EMSDK_NODE = "";
            var var_EMSDK_PYTHON = "";
            var var_JAVA_HOME = "";
            using (CurrentDir.Enter(EmsdkDir))
            {
                var proc = Proc.Make("emsdk", "activate " + emsdkVer)
                    .WithShell()
                    .WithSilent();
                proc.Call();
                var logs = proc.StdErrorString.ToString();

                using (var reader = new StringReader(logs))
                {
                    string line;
                    while ((line = reader.ReadLine()) != null)
                    {
                        if (line.StartsWith("PATH +="))
                        {
                            var_PATH.Add(line.Substring(7).Trim());
                        }
                        else if (line.StartsWith("EMSDK ="))
                        {
                            var_EMSDK = line.Substring(7).Trim();
                        }
                        else if (line.StartsWith("EM_CONFIG ="))
                        {
                            var_EM_CONFIG = line.Substring(11).Trim();
                        }
                        else if (line.StartsWith("EMSDK_NODE ="))
                        {
                            var_EMSDK_NODE = line.Substring(12).Trim();
                        }
                        else if (line.StartsWith("EMSDK_PYTHON"))
                        {
                            int i = line.IndexOf("=");
                            var_EMSDK_PYTHON = line.Substring(i + 1).Trim();
                        }
                        else if (line.StartsWith("JAVA_HOME ="))
                        {
                            var_JAVA_HOME = line.Substring(11).Trim();
                        }
                    }
                }
            }

            var path = Environment.GetEnvironmentVariable("PATH");
            path = string.Join(";", var_PATH) + ";" + path;
            Environment.SetEnvironmentVariable("PATH", path);
            Environment.SetEnvironmentVariable("EMSDK", var_EMSDK);
            Environment.SetEnvironmentVariable("EM_CONFIG", var_EM_CONFIG);
            Environment.SetEnvironmentVariable("EMSDK_NODE", var_EMSDK_NODE);
            Environment.SetEnvironmentVariable("EMSDK_PYTHON", var_EMSDK_PYTHON);
            Environment.SetEnvironmentVariable("JAVA_HOME", var_JAVA_HOME);

            Console.WriteLine("Setting environment variables:");
            Console.WriteLine("  PATH = " + path);
            Console.WriteLine("  EMSDK = " + var_EMSDK);
            Console.WriteLine("  EM_CONFIG = " + var_EM_CONFIG);
            Console.WriteLine("  EMSDK_NODE = " + var_EMSDK_NODE);
            Console.WriteLine("  EMSDK_PYTHON = " + var_EMSDK_PYTHON);
            Console.WriteLine("  JAVA_HOME = " + var_JAVA_HOME);



            // $ emsdk activate xxxx
            //var path = Environment.GetEnvironmentVariable("PATH");
            //path = EmsdkDir + ";" + path;
            //path = Path.Combine(EmsdkDir, "upstream", "emscripten") + ";" + path;
            //Environment.SetEnvironmentVariable("PATH", path);
            //Environment.SetEnvironmentVariable("EMSDK", EmsdkDir);
            //Environment.SetEnvironmentVariable("EM_CONFIG", Path.Combine(EmsdkDir, ".emscripten"));

            //Environment.SetEnvironmentVariable("EMSDK_PYTHON", @"C:\Proj\LN\Lumino\build\cache\tools\emsdk\python\3.9.2-1_64bit\python.exe");
            //Environment.SetEnvironmentVariable("JAVA_HOME", @"C:\Proj\LN\Lumino\build\cache\tools\emsdk\java\8.152_64bit");


            if (Directory.Exists(b.VcpkgDir))
            {
                // 自分で cmake を叩くときはこれが必要。
                // 最初の BuildExternals 時点では存在しないが、その時は必要ない。
                Ninja = Path.Combine(b.BuildToolsDir, "ninja-win", "ninja.exe");
                //Ninja = Directory.GetFiles(b.VcpkgDir + "/downloads/tools/ninja", "ninja.exe", SearchOption.AllDirectories).First();
            }

            //                    EM_CONFIG = C:\Proj\LN\Lumino\_build\tools\emsdk\.emscripten
            //EMSDK_NODE = C:\Proj\LN\Lumino\_build\tools\emsdk\node\14.15.5_64bit\bin\node.exe
            //EMSDK_PYTHON = C:\Proj\LN\Lumino\_build\tools\emsdk\python\3.9.2 - 1_64bit\python.exe
            //  JAVA_HOME = C:\Proj\LN\Lumino\_build\tools\emsdk\java\8.152_64bit


            /*
             * C:\Proj\LN\Lumino\build\tools\vcpkg
             * C:\Proj\LN\Lumino\build\tools\emsdk
             */

        }
    }
}
