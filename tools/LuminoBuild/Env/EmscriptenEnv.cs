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
        public const string emsdkVer = "1.40.1";
        public const string emVer = "1.40.1";
        public static string EmsdkDir;
        public static string Ninja;


        static public void Setup(Build b)
        {
            EmsdkDir = Path.Combine(b.BuildToolsDir, "emsdk");
            if (!Directory.Exists(EmsdkDir))
            {
                using (CurrentDir.Enter(b.BuildToolsDir))
                {
                    Proc.Make("git", "clone https://github.com/juj/emsdk.git").WithSilent().Call();

                    using (CurrentDir.Enter(EmsdkDir))
                    {
                        Proc.Make("emsdk", "install " + emsdkVer)
                            .WithShell()
                            .WithSilent()
                            .Call();
                    }
                }
            }

            // $ emsdk activate xxxx
            var path = Environment.GetEnvironmentVariable("PATH");
            path = EmsdkDir + ";" + path;
            path = Path.Combine(EmsdkDir, "upstream", "emscripten") + ";" + path;
            Environment.SetEnvironmentVariable("PATH", path);
            Environment.SetEnvironmentVariable("EMSDK", EmsdkDir);
            Environment.SetEnvironmentVariable("EM_CONFIG", Path.Combine(EmsdkDir, ".emscripten"));

            Ninja = b.VcpkgDir + "/downloads/tools/ninja/1.10.1-windows/ninja.exe";

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
