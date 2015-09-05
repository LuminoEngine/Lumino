using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class Program
    {
        const string SourcesRootDir = "../../../../LightNote/C_API/";
        const string DotNetOutputDir = "../../../../";

        static void Main(string[] args)
        {
            new CLManager();
            CLManager.Instance.Initialize();

            // 各種型情報が定義されているヘッダを解析する
            var typedefParser = new Parser.CLTypedefHeaderParser();
            var typedefs = typedefParser.Analyze(SourcesRootDir + "LNTypedef.h");

            // 関数が定義されているヘッダを解析する
            var apiHeaders = new string[]
            {
                SourcesRootDir + "LNBase.h",
                SourcesRootDir + "LNApplication.h",
                SourcesRootDir + "LNMath.h",
                SourcesRootDir + "LNAudio.h",
            };
            var parser = new Parser.CLAPIHeaderParser();
            var modules = parser.Analyze(apiHeaders);

            // 解析結果を Manager に登録する
            CLManager.Instance.LinkEntities();


            var csPInvoleBuilder = new Builder.CSPInvokeBuilder();
            csPInvoleBuilder.Build(CLManager.Instance, DotNetOutputDir + "DotNet/LNote.DotNet/API.cs");

            var csStructsBuilder = new Builder.CSStructsBuilder();
            csStructsBuilder.Build(CLManager.Instance, DotNetOutputDir + "DotNet/LNote.DotNet/Structs.cs");

            var csClassesBuilder = new Builder.CSClassesBuilder();
            csClassesBuilder.Build(CLManager.Instance, DotNetOutputDir + "DotNet/LNote.DotNet/Classes.cs");
        }
    }
}
