using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    // 開発作業用のパッケージを作成する
    class BuildLocalPackage : BuildRule
    {
        public override string Name => "BuildLocalPackage";

        public override void Build(Builder builder)
        {
            builder.DoTask("BuildExternalProjects");

            if (BuildEnvironment.IsMSVCTarget)
            {
                builder.DoTask("BuildEngine_MSVC");
            }
            if (Utils.IsMac)
            {
                builder.DoTask("BuildEngine_macOS");
                builder.DoTask("BuildEngine_iOS");
            }

            if (BuildEnvironment.IsWebTarget && EmscriptenBuildEnv.EmscriptenFound)
            {
                builder.DoTask("BuildEngine_Emscripten");
            }

            if (BuildEnvironment.IsAndroidTarget && AndoridBuildEnv.AndroidStudioFound)
            {
                builder.DoTask("BuildEngine_Android");
            }

            builder.DoTask("BuildDocuments");

            builder.DoTask("MakeNativePackage");
        }
    }
}
