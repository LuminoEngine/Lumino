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
        private OutputBuffer _propertiesText;
        

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
            _propertiesText = new OutputBuffer();

            // XMLコメント
            CSCommon.MakeSummaryXMLComment(_structText, _context.GetBriefText(classType));
            CSCommon.MakeRemarksXMLComment(_structText, _context.GetDetailsText(classType));

            // StructLayout 属性
            _structText.AppendLine("[StructLayout(LayoutKind.Sequential)]");
            // 構造体ヘッダ
            _structText.AppendLine("public struct {0}", classType.Name);

            // 構造体のメンバの数だけ public フィールドを作る
            foreach (var member in classType.StructData.Members)
            {
                CSCommon.MakeSummaryXMLComment(_fieldsText, member.Comment);
                _fieldsText.AppendLine("public {0} {1};", CSCommon.MakeTypeName(member.Type), member.Name).NewLine();
            }

#if false   // C_API の Create をコンストラクタとするようにした。
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
#endif

            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            _structText.AppendLine("{");
            _structText.IncreaseIndent();
            _structText.AppendWithIndent(_fieldsText.ToString()).NewLine();      // フィールド
            _structText.AppendWithIndent(_propertiesText.ToString()).NewLine();  // プロパティ
            _structText.AppendWithIndent(_methodsText.ToString()).NewLine();     // メソッド
            _structText.DecreaseIndent();
            _structText.AppendLine("};").NewLine();

            // 全クラステキストへ
            _allStructText.AppendWithIndent(_structText.ToString());
        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop)
        {
            // XMLコメント
            var xmlCommentText = new OutputBuffer();
            CSCommon.MakeSummaryXMLComment(xmlCommentText, _context.GetBriefText(prop));
            CSCommon.MakeRemarksXMLComment(xmlCommentText, _context.GetDetailsText(prop));

            // getter
            var getterText = new OutputBuffer();
            if (prop.Getter != null && _context.CheckEnabled(prop.Getter))
            {
                getterText.AppendLine("get");
                MakeMethodBodyText(prop.Getter, false, getterText);
            }
            // setter
            var setterText = new OutputBuffer();
            if (prop.Setter != null && _context.CheckEnabled(prop.Setter))
            {
                setterText.AppendLine("set");
                MakeMethodBodyText(prop.Setter, true, setterText);
            }

            // 修飾子
            var modifier = "public ";
            if (prop.IsStatic) modifier += "static ";

            // 型名
            var typeName = CSCommon.MakeTypeName(prop.Type);
            // 結合
            _propertiesText.Append(xmlCommentText.ToString());
            _propertiesText.Append("{0}{1} {2}", modifier, typeName, prop.Name).NewLine();
            _propertiesText.Append("{").NewLine();
            _propertiesText.IncreaseIndent();
            _propertiesText.AppendWithIndent(getterText.ToString());
            _propertiesText.AppendWithIndent(setterText.ToString());
            _propertiesText.DecreaseIndent();
            _propertiesText.Append("}").NewLine(2);
        }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            CSCommon.MakeMethodHeaderText(method, _context, _methodsText);
            MakeMethodBodyText(method, false, _methodsText);
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

        private void MakeMethodBodyText(CLMethod method, bool isPropSetter, OutputBuffer output)
        {
            var initStmtText = new OutputBuffer();  // API 呼び出し前の処理
            var argsText = new OutputBuffer();      // API 実引数
            var returnStmtText = "";                // return 文

            // インスタンスメソッドの時、this を表す先頭の仮引数がある
            if (method.ThisParam != null)
            {
                argsText.Append(CSCommon.GetAPIParamIOModifier(method.ThisParam) + " this");
            }

            // 普通の引数
            foreach (var param in method.Params)
            {
                string name = (isPropSetter) ? "value" : param.Name;    // setter の場合は名前を value にする
                argsText.AppendCommad("{0} {1}", CSCommon.GetAPIParamIOModifier(param), name);
            }

            // return として選択された引数がある場合、一時変数を作って結果を受け取り、それを返す
            if (method.ReturnParam != null)
            {
                var param = method.ReturnParam;

#if false   // 以前は "Set" をコンストラクタ扱いにしていたが、やめた
                // コンストラクタの場合は out this 固定。
                if (method.IsRefObjectConstructor)
                {
                    argsText.AppendCommad("out this");
                }
                else
#endif
                {
                    // 一時変数初期化
                    initStmtText.AppendWithIndent("var {0} = new {1}();", param.Name, CSCommon.MakeTypeName(param.Type)).NewLine();
                    // 実引数
                    argsText.AppendCommad("{0} {1}", CSCommon.GetAPIParamIOModifier(param), param.Name);
                    // return 文
                    returnStmtText = "return " + param.Name + ";" + OutputBuffer.NewLineCode;
                }
            }

            // エラーコードと throw
            string preErrorStmt = "";
            string postErrorStmt = "";
            if (method.FuncDecl.ReturnType == Manager.ResultEnumType)
            {
                preErrorStmt = "var result = ";
                postErrorStmt = "if (result != Result.OK) throw new LuminoException(result);" + OutputBuffer.NewLineCode;
            }

            // 定義文を結合
            output.AppendWithIndent("{").NewLine();
            output.IncreaseIndent();
            output.AppendWithIndent(
                initStmtText.ToString() +
                string.Format("{0}API.{1}({2});", preErrorStmt, method.FuncDecl.OriginalFullName, argsText.ToString()) + OutputBuffer.NewLineCode +
                postErrorStmt +
                returnStmtText);
            output.DecreaseIndent();
            output.AppendWithIndent("}").NewLine(2);
        }
    }
}
