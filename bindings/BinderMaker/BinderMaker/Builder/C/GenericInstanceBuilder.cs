using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder.C
{
    class GenericInstanceBuilder : Builder
    {
        private OutputBuffer _declsText = new OutputBuffer();
        private OutputBuffer _implesText = new OutputBuffer();
        private string _outputHeaderPath;

        public GenericInstanceBuilder(string outputHeaderPath)
        {
            _outputHeaderPath = outputHeaderPath;
            
            // .h
            _declsText.AppendLine("extern \"C\" {");
            _declsText.NewLine();

            // .cpp
            _implesText.AppendLine("#include \"LNInternal.h\"");
            _implesText.AppendLine("#include <LuminoEngine.h>");
            _implesText.AppendLine("#include \"../include/LNBase.h\"");
            _implesText.AppendLine("#include \"../include/LNGenericInstance.generated.h\"");
            _implesText.NewLine();
            _implesText.AppendLine("extern \"C\" {");
            _implesText.NewLine();
        }

        protected override bool OnClassLookedStart(CLClass classType)
        {
            // ジェネリックインスタンスだけが処理対象
            if (!classType.IsGenericinstance) return false;

            _declsText.AppendLine("// " + classType.OriginalName);
            _declsText.NewLine();
            _declsText.AppendLine("LN_CLASS({0}, LNObject);", classType.OriginalName);
            _declsText.NewLine();

            _implesText.AppendLine("// " + classType.OriginalName);
            _implesText.NewLine();
            _implesText.AppendLine("LN_TYPE_INFO_IMPL({0}, {1});", classType.Name, classType.OriginalName);
            _implesText.NewLine();

            return true;
        }

        protected override void OnClassLookedEnd(CLClass classType)
        {
        }

        protected override void OnMethodLooked(CLMethod method)
        {
            // :: function header
            //      "LNResult 関数名(仮引数リスト)"
            var buffer = new OutputBuffer();
            buffer.Append("LN_API LNResult ");
            buffer.Append(method.FuncDecl.OriginalFullName);
            buffer.Append("(");
            foreach (var param in method.FuncDecl.Params)
            {
                if (param != method.FuncDecl.Params.First()) buffer.Append(", ");
                buffer.Append("{0} {1}", CppCommon.ConvertTypeToCNameModified(param), param.ParamDecl.Name);
            }
            buffer.Append(")");

            _declsText.AppendLine(buffer.ToString() + ";").NewLine();
            _implesText.AppendLine(buffer.ToString());

            // :: function body
            //      {
            //          LNSceneNodeList_GetCount(listObject, outCount);
            //      }
            _implesText.AppendLine("{");
            _implesText.IncreaseIndent();

            _implesText.AppendWithIndent("return ");
            _implesText.Append(method.FuncDecl.CoreFuncDecl.OriginalFullName);
            _implesText.Append("(");
            foreach (var param in method.FuncDecl.Params)
            {
                if (param != method.FuncDecl.Params.First()) _implesText.Append(", ");
                _implesText.Append(param.ParamDecl.Name);
            }
            _implesText.Append(");");
            _implesText.NewLine();

            _implesText.DecreaseIndent();
            _implesText.AppendLine("}");
            _implesText.NewLine();
        }

        protected override string OnMakeOutoutFileText()
        {
            _declsText.AppendLine("} // extern \"C\"");
            _implesText.AppendLine("} // extern \"C\"");
            return _implesText.ToString();
        }

        protected override void OnFinished()
        {
            // ファイルに出力
            string output = _declsText.ToString();
            output = output.Replace("\r\n", "\n").Replace("\n", "\r\n");
            System.IO.File.WriteAllText(_outputHeaderPath, output, GetOutputEncoding());
        }
    }
}
