using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    // CI 環境によっては実行時間が長すぎると強制終了させられる。そのため、一部のタスクを分離したりスキップするための BuildRule。
    // 事前に BuildExternalProjects を実行しておくこと。
    // この Rule ではドキュメントやリリースパッケージの作成は行わない。
    class BuildForCI : BuildRule
    {
        public override string Name => "BuildForCI";

        public override void Build(Builder builder)
        {
            if (Utils.IsWin32)
            {
                builder.DoTask("MakeVSProjects");
                builder.DoTask("BuildEngine_MSVC");
                builder.DoTask("BuildEngine_Emscripten");
            }
            if (Utils.IsMac)
            {
                builder.DoTask("BuildEngine_macOS");
                builder.DoTask("BuildEngine_iOS");
            }

            if (BuildEnvironment.AndroidStudioFound)
            {
                builder.DoTask("BuildEngine_AndroidJNI");
            }
        }
    }
}
