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


        static public void Setup(Build b)
        {
            EmsdkDir = Path.Combine(b.BuildToolsDir, "emsdk");
            if (!Directory.Exists(EmsdkDir))
            {
                using (b.CurrentDir(b.BuildToolsDir))
                {
                    b.Call("git", "clone https://github.com/juj/emsdk.git");

                    using (b.CurrentDir(EmsdkDir))
                    {
                        b.Call("emsdk", "install " + emsdkVer);
                    }
                }
            }

            // $ emsdk activate xxxx
            var path = Environment.GetEnvironmentVariable("PATH");
            path = EmsdkDir + ";" + path;
            Environment.SetEnvironmentVariable("PATH", path);
            Environment.SetEnvironmentVariable("EMSDK", EmsdkDir);
            Environment.SetEnvironmentVariable("EM_CONFIG", Path.Combine(EmsdkDir, ".emscripten"));

            //                    EM_CONFIG = C:\Proj\LN\Lumino\_build\tools\emsdk\.emscripten
            //EMSDK_NODE = C:\Proj\LN\Lumino\_build\tools\emsdk\node\14.15.5_64bit\bin\node.exe
            //EMSDK_PYTHON = C:\Proj\LN\Lumino\_build\tools\emsdk\python\3.9.2 - 1_64bit\python.exe
            //  JAVA_HOME = C:\Proj\LN\Lumino\_build\tools\emsdk\java\8.152_64bit


        }
    }
}
