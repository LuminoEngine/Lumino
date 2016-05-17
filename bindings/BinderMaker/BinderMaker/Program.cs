using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class Program
    {
        const string SourcesRootDir = "../../../../C_API/include/";
        const string DotNetOutputDir = "../../../../";

        static int Main(string[] args)
        {
            try
            {


                new CLManager();
                CLManager.Instance.Initialize();

                // 各種型情報が定義されているヘッダを解析する
                var typedefParser = new Parser.CLTypedefHeaderParser();
                var typedefs = typedefParser.Analyze(SourcesRootDir + "LNTypedef.h");

                // 関数が定義されているヘッダを解析する
                var apiHeaders = new string[]
                {
                    SourcesRootDir + "LNApplication.h",
                    SourcesRootDir + "LNBase.h",
                    SourcesRootDir + "LNMath.h",
                    SourcesRootDir + "LNAudio.h",
                    SourcesRootDir + "LNGraphics.h",
                    SourcesRootDir + "LNScene.h",
                };
                var parser = new Parser.CLAPIHeaderParser();
                var modules = parser.Analyze(apiHeaders);

                // 解析結果を Manager に登録する
                CLManager.Instance.LinkEntities();

                // Wrapper
                //var wrapperBuilder = new Builder.WrapperImplementBuilder();
                //wrapperBuilder.Build(CLManager.Instance, DotNetOutputDir + "C_API/src/LNWrapperImplement.h");
                //return 0;

                // C#
                var csPInvoleBuilder = new Builder.CSPInvokeBuilder();
                csPInvoleBuilder.Build(CLManager.Instance, DotNetOutputDir + "DotNet/LuminoDotNet/API.cs");
                var csStructsBuilder = new Builder.CSStructsBuilder();
                csStructsBuilder.Build(CLManager.Instance, DotNetOutputDir + "DotNet/LuminoDotNet/Structs.cs");
                var csClassesBuilder = new Builder.CSClassesBuilder();
                csClassesBuilder.Build(CLManager.Instance, DotNetOutputDir + "DotNet/LuminoDotNet/Classes.cs");

#if false
            // Ruby
            var rubyEnumBuilder = new Builder.RubyEnumBuilder();
            rubyEnumBuilder.Build(CLManager.Instance, DotNetOutputDir + "Ruby/RubyEnums.cpp");
            var rubyStructsBuilder = new Builder.RubyStructsBuilder();
            rubyStructsBuilder.Build(CLManager.Instance, DotNetOutputDir + "Ruby/RubyStructs.cpp");
            var rubyStructsHeaderBuilder = new Builder.RubyStructsHeaderBuilder();
            rubyStructsHeaderBuilder.Build(CLManager.Instance, DotNetOutputDir + "Ruby/RubyStructs.h");
            var rubyClassesBuilder = new Builder.RubyClassesBuilder();
            rubyClassesBuilder.Build(CLManager.Instance, DotNetOutputDir + "Ruby/LuminoRubyClasses.cpp");
            var rubyDocBuilder = new Builder.RubyDocBuilder();
            rubyDocBuilder.Build(CLManager.Instance, DotNetOutputDir + "Ruby/LuminoRubyDoc.rb");

            // HSP
            var hspHeaderBuilder = new Builder.HSPHeaderBuilder();
            hspHeaderBuilder.Build(CLManager.Instance, DotNetOutputDir + "HSP/lumino.as");
            var hspCommandsBuilder = new Builder.HSPCommandsBuilder();
            hspCommandsBuilder.Build(CLManager.Instance, DotNetOutputDir + "HSP/LuminoHSP/LuminoHSP/HSPCommands.cpp");
            var hspStructsBuilder = new Builder.HSPStructsBuilder();
            hspStructsBuilder.Build(CLManager.Instance, DotNetOutputDir + "HSP/LuminoHSP/LuminoHSP/HSPStructs.cpp");
            var hspHelpBuilder = new Builder.HSPHelpBuilder();
            hspHelpBuilder.Build(CLManager.Instance, DotNetOutputDir + "HSP/lumino.hs");
            var hspFuncListBuilder = new Builder.HSPFuncListBuilder();
            hspFuncListBuilder.Build(CLManager.Instance, DotNetOutputDir + "HSP/CommandList.txt");
#endif
            }
            catch (Exception e)
            {
                Console.WriteLine("Error: " + e.Message);
                return 1;
            }
            return 0;
        }
    }
}
