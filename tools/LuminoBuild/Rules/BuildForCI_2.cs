using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    // CI 環境によっては実行時間が長すぎると強制終了させられる。そのため、一部のタスクを分離したりスキップするための BuildRule。
    class BuildForCI_2 : BuildRule
    {
        public override string Name => "BuildForCI_2";

        public override void Build(Builder builder)
        {
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

            if (EmscriptenBuildEnv.EmscriptenFound)
            {
                builder.DoTask("BuildEngine_Emscripten");
            }

            if (BuildEnvironment.AndroidStudioFound)
            {
                builder.DoTask("BuildEngine_AndroidJNI");
            }
        }
    }
}
