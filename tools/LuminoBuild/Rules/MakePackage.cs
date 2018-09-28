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
            if (Utils.IsMac)
            {
                builder.DoTask("BuildEngine_macOS");
                builder.DoTask("BuildEngine_iOS");
                
            }
            builder.DoTask("BuildEngine_Emscripten");

            if (BuildEnvironment.AndroidStudioFound)
            {
                builder.DoTask("BuildEngine_AndroidJNI");
            }

            builder.DoTask("BuildDocuments");
            //builder.DoTask("MakeNuGetPackage_Core");

            builder.DoTask("MakeReleasePackage");
            builder.DoTask("MakeInstaller_Win32");
        }
    }
}
