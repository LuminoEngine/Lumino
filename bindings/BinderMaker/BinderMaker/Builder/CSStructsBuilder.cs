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
    class CSStructsBuilder : Builder
    {
        private LangContext _context = new LangContext(LangFlags.CS);
        private OutputBuffer _allStructText = new OutputBuffer(1);
        private OutputBuffer _structText;
        private OutputBuffer _fieldsText;
        private OutputBuffer _methodsText;

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="enumType"></param>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (!classType.IsStruct) return false;

            _structText = new OutputBuffer();
            _fieldsText = new OutputBuffer();
            _methodsText = new OutputBuffer();

            // XMLコメント
            CSCommon.MakeSummaryXMLComment(_structText, _context.GetBriefText(classType));
            CSCommon.MakeRemarksXMLComment(_structText, _context.GetDetailsText(classType));

            // StructLayout 属性
            _structText.Append("[StructLayout(LayoutKind.Sequential)]").NewLine();
            // 構造体ヘッダ
            _structText.Append("public struct {0}", classType.Name);

            // 構造体のメンバの数だけ public フィールドを作る
            foreach (var member in classType.StructData.Members)
            {
                CSCommon.MakeSummaryXMLComment(_fieldsText, member.Comment);
                _fieldsText.Append("public {0} {1};", CSCommon.MakeTypeName(member.Type), member.Name).NewLine(2);
            }

            // 各要素指定のコンストラクタを作る
            {
                // XML コメント
                CSCommon.MakeSummaryXMLComment(_methodsText, "各要素を指定して初期化します。");

                // メソッドヘッダ
                _methodsText.Append("public " + classType.Name + "(");

                // 仮引数リスト → (float x, float y) 等
                var paramsText = new OutputBuffer();
                foreach (var member in classType.StructData.Members)  
                    paramsText.AppendCommad(CSCommon.MakeTypeName(member.Type) + " " + member.Name.ToLower());
                _methodsText.Append(paramsText).Append(")").NewLine();

                // フィールド格納 代入式 → X = x; Y = y; 等
                _methodsText.AppendWithIndent("{").NewLine().IncreaseIndent();
                foreach (var member in classType.StructData.Members)  
                    _methodsText.AppendWithIndent(member.Name + " = " + member.Name.ToLower() + ";").NewLine();
                _methodsText.DecreaseIndent().AppendWithIndent("}").NewLine(2);
            }

            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            _structText.NewLine();
            _structText.AppendWithIndent("{").NewLine();
            _structText.IncreaseIndent();
            _structText.AppendWithIndent(_fieldsText.ToString()).NewLine();      // フィールド
            //_structText.AppendWithIndent(_propertiesText.ToString()).NewLine();  // プロパティ
            _structText.AppendWithIndent(_methodsText.ToString()).NewLine();     // メソッド
            _structText.DecreaseIndent();
            _structText.AppendWithIndent("};").NewLine(2);

            // 全クラステキストへ
            _allStructText.AppendWithIndent(_structText.ToString());
        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop) { }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            // XML コメントと メソッドヘッダを作る
            MakeMethodHeaderText(method);
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string output = GetTemplate("CSStructs.txt");
            output = output.Replace("STRUCTS", _allStructText.ToString());
            return output;
        }

        /// <summary>
        /// メソッドヘッダ定義の作成
        /// </summary>
        /// <param name="method"></param>
        /// <param name="returnParam"></param>
        /// <returns></returns>
        private void MakeMethodHeaderText(CLMethod method)
        {
            // XMLコメント - summary
            var xmlCommentText = new OutputBuffer();
            CSCommon.MakeSummaryXMLComment(xmlCommentText, _context.GetBriefText(method));

            // 戻り値型
            string returnTypeText = "";
            if (!method.IsRefObjectConstructor)  // コンストラクタ以外は戻り値型名を作る
                returnTypeText = CSCommon.MakeTypeName(method.ReturnType);

            // 仮引数リストを作る
            var paramsText = new OutputBuffer();
            foreach (var param in method.Params)
            {
                // {ref/out} {型名} {引数名}
                paramsText.AppendCommad("{0} {1} {2}", CSCommon.GetParamIOModifier(param), CSCommon.MakeTypeName(param.Type), param.Name);
                if (!string.IsNullOrEmpty(param.OriginalDefaultValue))   // デフォルト引数
                    paramsText.Append(" = " + CSCommon.ConvertLiteral(param.OriginalDefaultValue, false));

                // XMLコメント - params
                CSCommon.MakeParamXMLComment(xmlCommentText, param.Name, _context.GetParamText(param));
            }

            // XMLコメント - return
            CSCommon.MakeReturnXMLComment(xmlCommentText, _context.GetReturnParamText(method.ReturnParam));
            // XMLコメント - Remarks
            CSCommon.MakeRemarksXMLComment(xmlCommentText, _context.GetDetailsText(method));

            // 修飾子を決める
            var modifier = "public ";
            if (method.IsStatic)
                modifier += "static ";

            // {修飾子} {戻り値型名} {メソッド名} ({仮引数リスト})
            string methodHeader = string.Format("{0}{1} {2}({3})", modifier, returnTypeText, method.Name, paramsText.ToString());

            // コンストラクタの場合は base 追加
            //if (method.IsRefObjectConstructor)
            //    methodHeader += " : base(_LNInternal.InternalBlock)";

            // 出力
            _methodsText.AppendWithIndent(xmlCommentText.ToString());
            _methodsText.AppendWithIndent(methodHeader);
            _methodsText.NewLine();
        }
    }
}
