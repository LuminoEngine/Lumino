using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild.Rules
{
    // CI 環境によっては実行時間が長すぎると強制終了させられる。そのため、一部のタスクを分離したりスキップするための BuildRule。
    class BuildForCI_3 : BuildRule
    {
        public override string Name => "BuildForCI_3";

        public override void Build(Builder builder)
        {
            builder.DoTask(new Tasks.MakeNativePackage() { fileMoving = true });
            builder.DoTask("CompressPackage");
            
            if (Utils.IsWin32)
            {
                //builder.DoTask("MakeNuGetPackage_Core");
                builder.DoTask("MakeInstaller_Win32");
            }
        }
    }
}
