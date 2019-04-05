using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    // 開発作業用のパッケージを作成する
    class MakeLocalPackage : BuildRule
    {
        public override string Name => "MakeLocalPackage";

        public override void Build(Builder builder)
        {
            if (!builder.HasFlagArgument("disable-build-external"))
            {
                builder.DoTask("BuildExternalProjects");
            }

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

            if (BuildEnvironment.EmscriptenFound)
            {
                builder.DoTask("BuildEngine_Emscripten");
            }

            if (BuildEnvironment.AndroidStudioFound)
            {
                builder.DoTask("BuildEngine_AndroidJNI");
            }

            builder.DoTask("BuildDocuments");

            builder.DoTask("MakeReleasePackage");
            builder.DoTask("CopyEngineLibsToRepoRoot");
        }
    }
}
