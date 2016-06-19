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

            _implesText.AppendLine("#include \"LNInternal.h\"");
            _implesText.AppendLine("#include \"../include/LNBase.h\"");
            _implesText.AppendLine("#include \"../include/LNGenericInstance.generated.h\"");
            _implesText.NewLine();
        }

        protected override bool OnClassLookedStart(CLClass classType)
        {
            // ジェネリックインスタンスだけが処理対象
            if (!classType.IsGenericinstance) return false;
            

            //_classText = new OutputBuffer();

            //_classText.AppendLine("class LNWI{0} : public ln::{0}", classType.Name);
            //_classText.AppendLine("{");

            return true;
        }

        protected override void OnClassLookedEnd(CLClass classType)
        {
            //_classText.AppendLine("};");
            //_classText.NewLine();
            //_outputText.AppendWithIndent(_classText.ToString());
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
                //if (param.IsGenericTemplate) _implesText.Append("&");     // boxing というか void* にする
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
