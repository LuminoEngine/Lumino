using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    // CI 環境によっては実行時間が長すぎると強制終了させられる。そのため、一部のタスクを分離したりスキップするための BuildRule。
    class BuildForCI_1 : BuildRule
    {
        public override string Name => "BuildForCI_1";

        public override void Build(Builder builder)
        {
            builder.DoTask("BuildExternalProjects");
        }
    }
}
