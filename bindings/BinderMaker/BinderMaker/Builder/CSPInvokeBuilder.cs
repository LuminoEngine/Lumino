using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    /// <summary>
    /// C# P/Invole ソース
    /// </summary>
    class CSPInvokeBuilder : Builder
    {

        #region Templates
        const string FuncDeclTempalte = @"
[DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
public extern static RETURN_TYPE FUNC_NAME(ARGS);
";
        #endregion

        #region Fields
        private LangContext _context = new LangContext(LangFlags.CS);
        private OutputBuffer _enumText = new OutputBuffer(1);
        private OutputBuffer _funcsText = new OutputBuffer(2);
        #endregion

        /// <summary>
        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnEnumLooked(CLEnum enumType)
        {
            // LNBool は enum として出力しない
            if (enumType.Name == "Bool")
                return;

            // XML コメント
            CSCommon.MakeSummaryXMLComment(_enumText, enumType.Comment);

            // [Flags] 属性
            if (enumType.IsFlags)
                _enumText.AppendWithIndent("[Flags]\n");

            // enum ヘッダ
            _enumText.AppendWithIndent("public enum {0}\n", enumType.Name);
            _enumText.AppendWithIndent("{\n");
            _enumText.IncreaseIndent();

            // 各メンバ
            foreach (var member in enumType.Members)
            {
                // LN_xxxx_MAX のようなターミネータは出力しない
                if (member.IsTerminator) continue;

                // XML コメント
                CSCommon.MakeSummaryXMLComment(_enumText, member.Comment);

                // メンバ定義
                _enumText.AppendWithIndent("{0} = {1},", member.CapitalizedName, member.Value).NewLine(2);
            }

            _enumText.DecreaseIndent();
            _enumText.AppendWithIndent("}\n\n");
        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop)
        {
            // プロパティとして割り振られたメソッドは OnMethodLooked() ではなくこの OnPropertyLooked() にくる。
            // やりたいことは OnMethodLooked() と同じなのでそちらにまわす。
            if (prop.Getter != null) OnMethodLooked(prop.Getter);
            if (prop.Setter != null) OnMethodLooked(prop.Setter);
        }

        /// <summary>
        /// メソッド 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            //// この関数は定義しない
            //if (method.IsCopy)
            //    return;

            // XML コメント
            CSCommon.MakeSummaryXMLComment(_funcsText, _context.GetBriefText(method));
            foreach (var param in method.FuncDecl.Params)
            {
                CSCommon.MakeParamXMLComment(_funcsText, param.Name, _context.GetParamText(param));
            }
            // TODO:returnコメント
            //CSBuilderCommon.MakeReturnXMLComment(method);
            CSCommon.MakeRemarksXMLComment(_context.GetDetailsText(method));

            // DLLImport・型名・関数名
            string declText = FuncDeclTempalte.Trim();
            declText = declText.Replace("RETURN_TYPE", CSCommon.MakeTypeName(method.FuncDecl.ReturnType));
            declText = declText.Replace("FUNC_NAME", method.FuncDecl.OriginalFullName);

            // 仮引数リスト
            string argsText = "";
            foreach (var param in method.FuncDecl.Params)
            {
                if (argsText != "") argsText += ", ";

                // 型名と引数名
                argsText += string.Format("{0} {1}", GetParamTypeName(param), param.Name);
            }
            declText = declText.Replace("ARGS", argsText);

            _funcsText.AppendWithIndent(declText).NewLine(2);
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string output = GetTemplate("CSPInvoke.txt");
            output = output.Replace("ENUM_LIST", _enumText.ToString());
            output = output.Replace("API_LIST", _funcsText.ToString());
            return output;
        }

       

        /// <summary>
        /// C# の型名を求める (仮引数用)
        /// </summary>
        /// <returns></returns>
        private string GetParamTypeName(CLParam param)
        {
            if ((param.Type is CLClass && ((CLClass)param.Type).IsReferenceObject) ||
                (param.Type == CLPrimitiveType.String && param.IOModifier == IOModifier.Out))
                return string.Format("{0} IntPtr", CSCommon.GetAPIParamIOModifier(param));
            else
                return string.Format("{0} {1}", CSCommon.GetAPIParamIOModifier(param), CSCommon.MakeTypeName(param.Type));
        }
    }
}
