using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    class MakePackage : BuildRule
    {
        public override string Name => "MakePackage";

        public override void Build(Builder builder)
        {
            builder.DoTask("BuildExternalProjects");
            if (Utils.IsWin32)
            {
                builder.DoTask("MakeVSProjects");
                builder.DoTask("BuildEngine_MSVC");
            }
            builder.DoTask("BuildEngine_Emscripten");

            //builder.DoTask("BuildEngine_AndroidJNI");


            builder.DoTask("BuildDocuments");
            //builder.DoTask("MakeNuGetPackage_Core");


            builder.DoTask("MakeEnginePackage");
            builder.DoTask("MakeInstaller_Win32");
        }
    }
}
