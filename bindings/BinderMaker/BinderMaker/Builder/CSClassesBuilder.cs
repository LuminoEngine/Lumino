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
    class CSClassesBuilder : Builder
    {
        private LangContext _context = new LangContext(LangFlags.CS);
        private OutputBuffer _allClassesText = new OutputBuffer(1);
        private OutputBuffer _classText;
        private OutputBuffer _fieldsText;
        private OutputBuffer _methodsText;
        private OutputBuffer _propertiesText;
        private OutputBuffer _typeInfoRegistersText = new OutputBuffer();
        private OutputBuffer _typeInfoPInvolesText = new OutputBuffer();

        /// <summary>
        /// ビルド開始前(初期化)通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnInitialize()
        {
            // bool コメント置換
            _context.AddReplacePair("LN_TRUE", "true");
            _context.AddReplacePair("LN_FALSE", "false");

            // enum メンバのコメント置換
            foreach (var t in Manager.AllEnums)
                foreach (var m in t.Members)
                    _context.AddReplacePair(m.OriginalName, t.Name + "." + m.CapitalizedName);

        }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="enumType"></param>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (classType.IsExtension) return false;  // 拡張クラスはなにもしない
            if (classType.IsStruct) return false;

            _classText = new OutputBuffer();
            _fieldsText = new OutputBuffer();
            _methodsText = new OutputBuffer();
            _propertiesText = new OutputBuffer();

            // XMLコメント
            CSCommon.MakeSummaryXMLComment(_classText, _context.GetBriefText(classType));
            CSCommon.MakeRemarksXMLComment(_classText, _context.GetDetailsText(classType));

            // クラスヘッダ
            _classText.Append("public partial class {0}", classType.Name);

            // ベースクラス
            if (classType.BaseClass != null)
                _classText.Append(" : " + classType.BaseClass.Name);
            _classText.NewLine();

            // ReferenceObject なら型情報を作る
            if (classType.IsReferenceObject)
            {
                // ファクトリ
                _typeInfoRegistersText.AppendWithIndent(@"
var _{0} = new TypeInfo(){{ Factory = (handle) =>
    {{
        var obj = new {0}(_LNInternal.InternalBlock);
        obj.SetHandle(handle);
        return obj;
    }}
}};", classType.Name).NewLine();
                _typeInfoRegistersText.AppendLine("_typeInfos.Add(_{0});", classType.Name);
                _typeInfoRegistersText.AppendLine("{0}_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));", classType.OriginalName);
                // P/Invoke
                _typeInfoPInvolesText.AppendLine("[DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]");
                _typeInfoPInvolesText.AppendLine("private static extern void {0}_SetBindingTypeInfo(IntPtr data);", classType.OriginalName).NewLine();
                // internal コンストラクタ
                _methodsText.AppendLine("internal {0}(_LNInternal i) : base(i) {{}}", classType.Name).NewLine();
            }

            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            _classText.AppendLine("{");
            _classText.IncreaseIndent();
            _classText.AppendWithIndent(_fieldsText.ToString()).NewLine();      // フィールド
            _classText.AppendWithIndent(_propertiesText.ToString()).NewLine();  // プロパティ
            _classText.AppendWithIndent(_methodsText.ToString()).NewLine();     // メソッド
            _classText.DecreaseIndent();
            _classText.AppendLine("};").NewLine();

            // 全クラステキストへ
            _allClassesText.AppendWithIndent(_classText.ToString());
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
            _propertiesText.AppendWithIndent(xmlCommentText.ToString());
            _propertiesText.AppendLine("{0}{1} {2}", modifier, typeName, prop.Name);
            _propertiesText.AppendLine("{");
            _propertiesText.IncreaseIndent();
            _propertiesText.AppendWithIndent(getterText.ToString());
            _propertiesText.AppendWithIndent(setterText.ToString());
            _propertiesText.DecreaseIndent();
            _propertiesText.AppendLine("}");
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
            string template = GetTemplate("CSClassLib.txt");
            return template
                .Replace("CLASSES", _allClassesText.ToString())
                .Replace("TYPE_INFO_REGISTERS", _typeInfoRegistersText.ToString())
                .Replace("TYPE_INFO_PINVOKES", _typeInfoPInvolesText.ToString());
        }

        private void MakeMethodBodyText(CLMethod method, bool isPropSetter, OutputBuffer output)
        {
            
            string overrideCode;
            if (_context.TryGetOverrideCode(method, out overrideCode))
            {
                output.AppendWithIndent(overrideCode);
            }
            else
            {
                var initStmtText = new OutputBuffer();  // API 呼び出し前の処理
                var argsText = new OutputBuffer();      // API 実引数
                var postStmtText = new OutputBuffer();  // API 呼び出し後の処理
                var returnStmtText = "";                // return 文

                // Initializer の場合はファクトリ登録処理を埋め込む
                if (method.IsLibraryInitializer)
                    initStmtText.AppendWithIndent("InternalManager.Initialize();").NewLine();

                // インスタンスメソッドの時、this を表す先頭の仮引数がある
                if (method.ThisParam != null)
                {
                    var param = method.ThisParam;
                    string io = CSCommon.GetAPIParamIOModifier(param);
                    //if (method.IsRefObjectConstructor)
                    //{
                    //    // コンストラクタの場合は、Handle を一度ローカル変数に受け取り、
                    //    // 最後に Internalmanager.RegisterWrapperObject で this にセットしつつ管理リストに登録する。
                    //    initStmtText.AppendLine("IntPtr {0};", param.Name);
                    //    argsText.Append(string.Format("{0} {1}", io, param.Name));
                    //    postStmtText.AppendLine("InternalManager.RegisterWrapperObject(this, {0});", param.Name);
                    //}
                    //else
                    //{
                        argsText.Append(string.Format("{0} _handle", io));  // RefClass instance
                    //}
                }

                // 普通の引数
                foreach (var param in method.Params)
                {
                    string name = (isPropSetter) ? "value" : param.Name;    // setter の場合は名前を value にする
                    string exp = name;
                    if (CLType.CheckRefObjectType(param.Type))   // RefObj の場合は .Handle から取得する
                        exp = "(" + name + " != null) ? " + name + ".Handle : default(IntPtr)";
                    argsText.AppendCommad("{0} {1}", CSCommon.GetAPIParamIOModifier(param), exp);
                }

                // return として選択された引数がある場合、一時変数を作って結果を受け取り、それを返す
                if (method.ReturnParam != null)
                {
                    var param = method.ReturnParam;

                    // string 型の return
                    if (param.IsOutStringType)
                    {
                        // まずは文字列の長さを取得しメモリ確保、その後コピーする。
                        #region string 型の return
                        // 一時変数初期化
                        initStmtText.AppendWithIndent("IntPtr strPtr;").NewLine();
                        // 実引数
                        argsText.AppendCommad("out strPtr");
                        // 後処理
                        returnStmtText += @"
int len;
API.LCSInternal_GetIntPtrStringLength(strPtr, out len);
var sb = new StringBuilder(len + 1);
API.LCSInternal_GetIntPtrString(strPtr, sb);
return sb.ToString();".Trim();
                        #endregion
                    }
                    // 参照オブジェクト型の return (参照オブジェクト型の return は、自動生成ではプロパティのみ可能としている)
                    else if (param.IsOutRefObjectType)
                    {
                        if (method.IsRefObjectConstructor)
                        {
                            // コンストラクタの場合は、Handle を一度ローカル変数に受け取り、
                            // 最後に Internalmanager.RegisterWrapperObject で this にセットしつつ管理リストに登録する。
                            initStmtText.AppendLine("IntPtr {0};", param.Name);
                            argsText.AppendCommad(string.Format("out {0}", param.Name));
                            postStmtText.AppendLine("InternalManager.RegisterWrapperObject(this, {0});", param.Name);
                        }
                        else
                        {
                            #region 参照オブジェクト型の return
                            // やっていることは、
                            // ・Wrap 側に未登録のオブジェクトなら登録する
                            // ・return するオブジェクトを保持するフィールドの作成
                            string typeName = CSCommon.MakeTypeName(param.Type);
                            string fieldName = "_" + method.Name; // 現在プロパティ名を受け取っていないのでとりあえずこれを使う

                            // out 値を受け取る変数
                            initStmtText.AppendWithIndent("IntPtr {0};", param.Name).NewLine();
                            // 実引数
                            argsText.AppendCommad("out " + param.Name);
                            // 後処理
                            string format = @"
{0} = IntenalManager.GetWrapperObject<{1}>({2});
return {0};".Trim();
                            returnStmtText = string.Format(format, fieldName, typeName, param.Name);

                            // ラップオブジェクトを保持するメンバを追加
                            //string modifier = "";
                            //if (!method.IsInstanceMethod)
                            //    modifier += "static";
                            //fieldsOutput.AppendWithIndent(string.Format("{0} {1} {2};", modifier, typeName, fieldName)).NewLine();

                            /* 例)
                            IntPtr viewPane;
                            API.LViewPane_GetDefaultViewPane(out viewPane);
                            if (viewPane == null) {
                                _viewPane = null;
                            }
                            else if (_viewPane == null || _viewPane.Handle != viewPane) {
                                _viewPane = new ViewPane();
                                _viewPane._handle = viewPane;
                            }
                            return _viewPane;
                            */
                            #endregion
                        }
                    }
                    // その他の型の return
                    else
                    {
                        #region その他の型の return
                        // 一時変数初期化
                        initStmtText.AppendWithIndent("var {0} = new {1}();", param.Name, CSCommon.MakeTypeName(param.Type)).NewLine();
                        // 実引数
                        argsText.AppendCommad("{0} {1}", CSCommon.GetAPIParamIOModifier(param), param.Name);
                        // return 文
                        returnStmtText = "return " + param.Name + ";" + OutputBuffer.NewLineCode;
                        #endregion
                    }
                }

                // エラーコードと throw
                string preErrorStmt = "";
                string postErrorStmt = "";
                if (method.FuncDecl.ReturnType == Manager.ResultEnumType)
                    CSCommon.MakeThrowException(out preErrorStmt, out postErrorStmt);

                // Terminator の場合は終了処理を埋め込む
                if (method.IsLibraryTerminator)
                    initStmtText.AppendLine("InternalManager.Terminate();");

                // 定義文を結合
                output.AppendWithIndent("{").NewLine();
                output.IncreaseIndent();
                output.AppendWithIndent(
                    initStmtText.ToString() +
                    string.Format("{0}API.{1}({2});", preErrorStmt, method.FuncDecl.OriginalFullName, argsText.ToString()) + OutputBuffer.NewLineCode +
                    postErrorStmt +
                    postStmtText.ToString() +
                    returnStmtText).NewLine();
                output.DecreaseIndent();
                output.AppendWithIndent("}").NewLine(2);

            }

        }
    }
}
