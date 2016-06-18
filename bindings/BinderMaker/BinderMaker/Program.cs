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
            //try
            {


                new CLManager();
                CLManager.Instance.Initialize();

                // 各種型情報が定義されているヘッダを解析する
                var typedefParser = new Parser.CLTypedefHeaderParser();
                var typedefs = typedefParser.Analyze(SourcesRootDir + "LNTypedef.h");

                // 関数が定義されているヘッダを解析する
                var apiHeaders = new string[]
                {
                    SourcesRootDir + "LNEngine.h",
                    SourcesRootDir + "LNBase.h",
                    SourcesRootDir + "LNMath.h",
                    SourcesRootDir + "LNAudio.h",
                    SourcesRootDir + "LNGraphics.h",
                    SourcesRootDir + "LNScene.h",
                    SourcesRootDir + "LNUI.h",
                };
                var parser2 = new Parser2.ApiDeclParser();
                var modules2 = parser2.Analyze(apiHeaders);


                //var parser = new Parser.CLAPIHeaderParser();
                //var modules = parser.Analyze(apiHeaders);

                var modules = new List<CLModule>();
                modules2.ForEach((m) => modules.Add(new CLModule(m)));


                // インスタンス化が必要なジェネリッククラスを調べる
                var genericInstanceFind = new GenericInstanceFindVisitor();
                genericInstanceFind.Run(modules2);
                foreach (var pair in genericInstanceFind.GenericInstanceMap)
                {
                    var classDecl = genericInstanceFind.AllClasses.Find((item) => item.OriginalName == pair.Key);
                    var module = modules.Find((item) => item.Name == "Base");   // とりあえず Base モジュールに追加する
                    module.Classes.Add(new CLClass(module, classDecl, pair.Value));
                }


                // 解析結果を Manager に登録する
                CLManager.Instance.LinkEntities();

                // Wrapper
                //var wrapperBuilder = new Builder.WrapperImplementBuilder();
                //wrapperBuilder.Build(CLManager.Instance, DotNetOutputDir + "C_API/src/LNWrapperImplement.h");
                //return 0;

                // C#
                var csContet = new Builder.CSContext();
                var csPInvoleBuilder = new Builder.CSPInvokeBuilder();
                csPInvoleBuilder.Build(CLManager.Instance, csContet, DotNetOutputDir + "DotNet/LuminoDotNet/API.cs");
                var csStructsBuilder = new Builder.CSStructsBuilder();
                csStructsBuilder.Build(CLManager.Instance, csContet, DotNetOutputDir + "DotNet/LuminoDotNet/Structs.cs");
                var csClassesBuilder = new Builder.CSClassesBuilder();
                csClassesBuilder.Build(CLManager.Instance, csContet, DotNetOutputDir + "DotNet/LuminoDotNet/Classes.cs");

                // Ruby
                var rubyContet = new Builder.RubyContext();
                var rubyEnumBuilder = new Builder.RubyEnumBuilder();
                rubyEnumBuilder.Build(CLManager.Instance, rubyContet, DotNetOutputDir + "Ruby/RubyEnums.cpp");
                var rubyStructsBuilder = new Builder.RubyStructsBuilder();
                rubyStructsBuilder.Build(CLManager.Instance, rubyContet, DotNetOutputDir + "Ruby/RubyStructs.cpp");
                var rubyStructsHeaderBuilder = new Builder.RubyStructsHeaderBuilder();
                rubyStructsHeaderBuilder.Build(CLManager.Instance, rubyContet, DotNetOutputDir + "Ruby/RubyStructs.h");
                var rubyClassesBuilder = new Builder.RubyClassesBuilder();
                rubyClassesBuilder.Build(CLManager.Instance, rubyContet, DotNetOutputDir + "Ruby/LuminoRubyClasses.cpp");
                var rubyDocBuilder = new Builder.RubyDocBuilder();
                rubyDocBuilder.Build(CLManager.Instance, rubyContet, DotNetOutputDir + "Ruby/LuminoRubyDoc.rb");

                // HSP
                var hspContet = new Builder.HSPContext();
                var hspHeaderBuilder = new Builder.HSPHeaderBuilder();
                hspHeaderBuilder.Build(CLManager.Instance, hspContet, DotNetOutputDir + "HSP/lumino.as");
                var hspCommandsBuilder = new Builder.HSPCommandsBuilder();
                hspCommandsBuilder.Build(CLManager.Instance, hspContet, DotNetOutputDir + "HSP/LuminoHSP/LuminoHSP/HSPCommands.cpp");
                var hspStructsBuilder = new Builder.HSPStructsBuilder();
                hspStructsBuilder.Build(CLManager.Instance, hspContet, DotNetOutputDir + "HSP/LuminoHSP/LuminoHSP/HSPStructs.cpp");
                var hspHelpBuilder = new Builder.HSPHelpBuilder();
                hspHelpBuilder.Build(CLManager.Instance, hspContet, DotNetOutputDir + "HSP/lumino.hs");
                var hspFuncListBuilder = new Builder.HSPFuncListBuilder();
                hspFuncListBuilder.Build(CLManager.Instance, hspContet, DotNetOutputDir + "HSP/CommandList.txt");
            }
            //catch (Exception e)
            //{
            //    Console.WriteLine("Error: " + e.Message);
            //    return 1;
            //}
            return 0;
        }
    }
}
