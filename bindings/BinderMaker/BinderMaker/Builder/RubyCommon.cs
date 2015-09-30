using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * Ruby用拡張ライブラリをCで作る
 * http://members.jcom.home.ne.jp/mitakelp/makeext.html
 */

namespace BinderMaker.Builder
{
    static class RubyCommon
    {
        public class RubyTypeInfo
        {
            public string RubyType;
            public string CheckExp;     // 型判別式
            public string CVarType;     // 変数型
            public string ConvRToCExp;  // Ruby → C 変換式
            public string CVarInitExp;  // C言語宣言文変換式    (いらない？)
            public RubyTypeInfo(string rubyType, string checkExp, string cVarType, string convRToCExp, string cVarInitExp)
            {
                RubyType = rubyType;
                CheckExp = checkExp;
                CVarType = cVarType;
                ConvRToCExp = convRToCExp;
                CVarInitExp = cVarInitExp;
            }
        }

        public static Dictionary<CLType, RubyTypeInfo> _rubyTypeTable = new Dictionary<CLType, RubyTypeInfo>()
        {
            //{ CLPrimitiveType.Void }

            //{ CLPrimitiveType.ByteArray,    new RubyTypeInfo("Array",   "", "", "", "") },
            { CLPrimitiveType.String,       new RubyTypeInfo("String",  "isRbString({0})", "char*",        "StringValuePtr({0})",       "char* {0} = StringValuePtr({1});") },

            { CLPrimitiveType.Bool,         new RubyTypeInfo("Bool",    "isRbBool({0})",   "LNBool",       "RbBooltoBool({0})",         "lnBool {0} = RbBooltoBool({1});") },
            { CLPrimitiveType.Byte,         new RubyTypeInfo("Integer", "isRbNumber({0})", "uint8_t",         "FIX2INT({0})",              "lnU8 {0} = FIX2INT({1});") },
            { CLPrimitiveType.Int32,          new RubyTypeInfo("Integer", "isRbNumber({0})", "int",          "FIX2INT({0})",              "int {0} = FIX2INT({1});") },
            { CLPrimitiveType.UInt32,       new RubyTypeInfo("Integer", "isRbNumber({0})", "uint64_t",        "FIX2INT({0})",              "lnU32 {0} = FIX2INT({1});") },
            { CLPrimitiveType.Int64,       new RubyTypeInfo("Integer", "isRbNumber({0})", "uint32_t",        "FIX2INT({0})",              "lnU32 {0} = FIX2INT({1});") },
            { CLPrimitiveType.Float,        new RubyTypeInfo("Float",   "isRbFloat({0})",  "float",        "((float)NUM2DBL({0}))",    "float {0} = static_cast<float>(NUM2DBL({1}));") },
            { CLPrimitiveType.Double,       new RubyTypeInfo("Float",   "isRbFloat({0})",  "double",       "NUM2DBL({0})",              "double {0} = NUM2DBL({1});") },

            // { CLPrimitiveType.ResultCode,   new RubyTypeInfo("Integer", "isRbNumber({0})", "lnResultCode", "(lnResultCode)FIX2INT({0})",   "lnResultCode {0} = (lnResultCode)FIX2INT({1});") },
            //{ CLPrimitiveType.ExceptionCallback, new RubyTypeInfo("",   "isRbNumber({0})", "lnExceptionCallback", null, null) },
            { CLPrimitiveType.VoidPtr,       new RubyTypeInfo("String",        "isRbNumber({0})", "void*", "((void*)FIX2INT({0}))", null) },  
            { CLPrimitiveType.IntPtr,       new RubyTypeInfo("Integer",        "isRbNumber({0})", "intptr_t", "FIX2INT({0})", null) },    // ruby の FixNum は x64 なら 64bit までOK
            // { CLPrimitiveType.HWND,         new RubyTypeInfo("",        "isRbNumber({0})", "HWND",         "FIX2INT({0})", "int {0} = FIX2INT({1});") },      // TODO:bignum?

            //{ CLPrimitiveType.Handle = new CLPrimitiveType("Handle");
            //{ CLPrimitiveType.Generic, new RubyTypeInfo("",             "isRbObject({0})",      "lnHandle",     "FIX2INT({0})", "lnHandle {0} = FIX2INT({1});") },

        };

        /// <summary>
        /// メソッドが Ruby に出力できるかをチェックする
        /// </summary>
        /// <param name="methdo"></param>
        /// <returns></returns>
        public static bool CheckInvalidMethod(CLMethod method)
        {
            int count = method.Params.FindAll((param) => param.IOModifier == IOModifier.Out).Count;
            if (count >= 2)
            {
                Console.WriteLine("{0} Ruby では out タイプの引数を複数処理できないためスキップ。", method.FuncDecl.OriginalFullName);
                return false;
            }
            //var byteArrayParam = method.Params.Find((param) => param.Type == CLPrimitiveType.ByteArray);
            //if (byteArrayParam != null)
            //{
            //    Console.WriteLine("{0} Ruby では ByteArray 型の引数を処理できないためスキップ。", method.CName);
            //    return false;
            //}
            //var intArrayParam = method.Params.Find((param) => param.Type == CLPrimitiveType.IntNativeArray);
            //if (intArrayParam != null)
            //{
            //    Console.WriteLine("{0} Ruby では IntArray 型の引数を処理できないためスキップ。", method.CName);
            //    return false;
            //}

            return true;
        }


        /// <summary>
        /// VALUE → C型へのキャスト式
        /// </summary>
        /// <param name="type"></param>
        /// <param name="val">VALUE 型の変数名</param>
        /// <returns></returns>
        public static string GetCastExpVALUEtoC(CLType targetType, string val)
        {
            // プリミティブ型
            RubyTypeInfo info;
            if (_rubyTypeTable.TryGetValue(targetType, out info))
            {
                return string.Format(info.ConvRToCExp, val);
            }

            // enum
            var enumType = targetType as CLEnum;
            if (enumType != null)
            {
                return string.Format("({0})FIX2INT({1})", enumType.OriginalName, val);
            }

            throw new InvalidOperationException();
        }

        /// <summary>
        /// C言語変数 → VALUE 変換式の作成 (return 用)
        /// </summary>
        /// <returns></returns>
        public static void MakeReturnCastExpCToVALUE(CLType cType, string varName, OutputBuffer output)
        {
            // プリミティブ型
            if (cType is CLPrimitiveType)
            {
                output.AppendLine("return toVALUE({0});", varName);
                return;
            }

            // enum 型
            if (cType is CLEnum)
            {
                output.AppendLine("return INT2FIX({0});", varName);
                return;
            }

            var classType = cType as CLClass;
            if (classType != null)
            {

                // struct 型
                if (classType.IsStruct)
                {
                    // 新しいインスタンスを作って返す
                    output.AppendLine("VALUE retObj = {0}_allocate({1});", classType.OriginalName, GetModuleVariableName(classType));
                    output.AppendLine("*(({0}*)DATA_PTR(retObj)) = {1};", classType.OriginalName, varName);
                    output.AppendLine("return retObj;");
                    return;
                }
                // RefObj 型
                else
                {
                    throw new InvalidOperationException();
                }
            }
            throw new InvalidOperationException();
        }

        // <summary>
        /// VALUE → C宣言式
        /// </summary>
        public static string GetDeclCastExpVALUEtoC(CLType type, string cVarName, string rubyVarName, string defaultValueSource)
        {
            string decl = null;
            string exp = null;

            // プリミティブ型
            RubyTypeInfo info;
            if (_rubyTypeTable.TryGetValue(type, out info))
            {
                decl = string.Format("{0} {1}", info.CVarType, cVarName);
                exp = string.Format(info.ConvRToCExp, rubyVarName);
            }

            // enum
            var enumType = type as CLEnum;
            if (enumType != null)
            {
                decl = string.Format("{0} {1}", enumType.OriginalName, cVarName);
                exp = string.Format("({0})FIX2INT({1})", enumType.OriginalName, rubyVarName);
            }

            var classType = type as CLClass;
            if (type is CLClass)
            {
                // struct 型
                if (classType.IsStruct)
                {
                    if (!string.IsNullOrEmpty(defaultValueSource))
                        throw new InvalidOperationException();  // struct 型のデフォルト引数は未実装
                    string t = string.Format("{0}* tmp_{1}; Data_Get_Struct({2}, {0}, tmp_{1});", classType.OriginalName, cVarName, rubyVarName);
                    t = t + string.Format("{0}& {1} = *tmp_{1};", classType.OriginalName, cVarName);
                    return t;
                }
                // RefObj 型
                else
                {
                    decl = string.Format("LNHandle {0}", cVarName);
                    exp = string.Format("RbRefObjToHandle({0})", rubyVarName);
                }
            }

            if (!string.IsNullOrEmpty(decl) && !string.IsNullOrEmpty(exp))
            {
                // デフォルト引数があれば、省略されている場合に格納する
                if (!string.IsNullOrEmpty(defaultValueSource))
                    return string.Format("{0} = ({1} != Qnil) ? {2} : {3};", decl, rubyVarName, exp, defaultValueSource);
                else
                    return string.Format("{0} = {1};", decl, exp);
            }

            throw new InvalidOperationException();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static string GetModuleVariableName(CLClass type)
        {
            if (type.IsStruct)
                return string.Format(@"g_struct_{0}", type.Name);
            else
                return string.Format(@"g_class_{0}", type.Name);
        }

        /// <summary>
        /// Ruby の VALUE が type であるかを識別するための式を返す
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static string GetTypeCheckExp(CLType type, string rubyValueName)
        {
            // プリミティブ型
            RubyTypeInfo info;
            if (_rubyTypeTable.TryGetValue(type, out info))
                return string.Format(info.CheckExp, rubyValueName);

            // 定数は T_FIXNUM
            if (type is CLEnum)
                return string.Format("isRbNumber({0})", rubyValueName);

            // struct / class
            if (type is CLClass)
                return string.Format("isRbObject({0})", rubyValueName);

            throw new InvalidOperationException();
        }

        /// <summary>
        /// C関数名を Ruby メソッド名に変換する
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        public static string ConvertCommonNameToRubyMethodName(CLMethod method)
        {
            string name;
            if (method.OwnerProperty != null)
            {
                name = GetSnakeStyleName(method.OwnerProperty.Name);

                // 先頭が is の場合は 末尾 ? に変換
                if (method.PropertyNameType == PropertyNameType.Is && name.IndexOf("is_") == 0)
                {
                    //if (char.IsNumber(name, 3))    // 変換した結果数値が識別子の先頭にならないこと
                        name += "?";                // ? はつけてあげる
                    //else
                    //    name = name.Substring(3) + "?";
                }
            }
            else
            {
                name = GetSnakeStyleName(method.Name);
            }

            // まずはスネークスタイルに変換
            //var name = GetSnakeStyleName(method.Name);

            //// 先頭が is の場合は 末尾 ? に変換
            //if (name.IndexOf("is_") == 0)
            //{
            //    if (char.IsNumber(name, 3))    // 変換した結果数値が識別子の先頭にならないこと
            //        name += "?";                // ? はつけてあげる
            //    else
            //        name = name.Substring(3) + "?";
            //}

            // プロパティの場合は = 等に変更
            if (method.PropertyNameType != PropertyNameType.NotProperty)
            {
                /*if (method.PropertyNameType == PropertyNameType.Get)
                    name = name.Substring(4);   // 先頭の get_ を取り除く
                else*/ if (method.PropertyNameType == PropertyNameType.Set)
                    name += /*name.Substring(4) + */"=";   // 先頭の set_ を取り除き、後ろに =
            }

            return name;
        }

        /// <summary>
        /// InitAuto → init_audio
        /// </summary>
        /// <returns></returns>
        public static string GetSnakeStyleName(string text)
        {
            string output = "";

            //if (text == "PlaySE3D")
            //{
            //    Console.WriteLine();
            //}

            // 後ろから
            bool isLastLow = char.IsLower(text, text.Length - 1);
            int upperCount = 0;
            int lastIndex = text.Length;
            int i;
            for (i = text.Length - 2; i >= 0; i--)
            {
                // 小文字の連続を探している状態
                if (isLastLow)
                {
                    if (char.IsUpper(text, i) || char.IsNumber(text, i))
                    {
                        if (lastIndex - i > 0)
                        {
                            if (output.Length != 0) output = "_" + output;
                            output = text.Substring(i, lastIndex - i).ToLower() + output;
                        }
                        lastIndex = i;
                        isLastLow = false;
                    }
                    else
                    {
                        // 小文字が続いている
                    }
                }
                // 大文字の連続を探している状態
                else
                {
                    upperCount++;
                    if (char.IsLower(text, i))
                    {
                        upperCount = 0;
                        if (upperCount == 1)
                            isLastLow = true;
                        else
                        {
                            if (lastIndex - i - 1 > 0)
                            {
                                if (output.Length != 0) output = "_" + output;
                                output = text.Substring(i + 1, lastIndex - i - 1).ToLower() + output;
                            }
                            lastIndex = i + 1;
                            isLastLow = true;
                        }
                    }
                    else
                    {
                        // 大文字が続いている
                    }
                }
            }

            if (lastIndex != 0)
            {
                if (lastIndex - i > 0)
                {
                    if (output.Length != 0) output = "_" + output;
                    output = text.Substring(0, lastIndex).ToLower() + output;
                }
            }

            return output;
        }


        /// <summary>
        /// rb_define_method 等のメソッド登録を作成する
        /// </summary>
        public static void MakeRubyMethodRegister(OutputBuffer output, CLMethod baseMethod, string typeValName, string rubyMethodName, string funcName)
        {
            string def;
            if (baseMethod.IsRefObjectConstructor)
                def = string.Format(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC({1}), -1);", typeValName, funcName);
            else if (baseMethod.IsInstanceMethod)
                def = string.Format(@"rb_define_method({0}, ""{1}"", LN_TO_RUBY_FUNC({2}), -1);", typeValName, rubyMethodName, funcName);
            else
                def = string.Format(@"rb_define_singleton_method({0}, ""{1}"", LN_TO_RUBY_FUNC({2}), -1);", typeValName, rubyMethodName, funcName);
            output.AppendLine(def);
        }

        /// <summary>
        /// オーバーロード1つ分の関数定義を作成する。
        /// </summary>
        public static void MakeOverloadedMethod(CLMethod method, OutputBuffer funcBody)
        {
            var callBody = new OutputBuffer();

            int normalArgsCount = 0;
            int defaultArgsCount = 0;
            var scan_args_Inits = new OutputBuffer();
            var scan_args_Args = new OutputBuffer();
            string typeCheckExp = "";
            var initStmt = new OutputBuffer();
            var argsText = new OutputBuffer();
            var postStmt = new OutputBuffer();
            var returnStmt = new OutputBuffer();

            // オリジナルの全仮引数を見ていく
            foreach (var param in method.FuncDecl.Params)
            {
                // コンストラクタの最後の引数は、WrapStruct::Handle への格納になる
                if (method.IsRefObjectConstructor && param == method.FuncDecl.Params.Last())
                {
                    argsText.AppendCommad("&selfObj->Handle");
                }
                // 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
                else if (
                    !method.IsRefObjectConstructor &&
                    method.IsInstanceMethod &&
                    param == method.FuncDecl.Params.First())
                {
                    var classType = param.Type as CLClass;
                    if (classType == null) throw new InvalidOperationException("インスタンスメソッドの第1引数が不正。");
                    if (classType.IsStruct)
                        argsText.AppendCommad("selfObj");
                    else
                        argsText.AppendCommad("selfObj->Handle");
                }
                // return として選択されている引数である場合
                else if (param == method.ReturnParam)
                {
                    var varName = "_" + param.Name;
                    // 宣言
                    initStmt.AppendLine("{0} {1};", CppCommon.ConvertTypeToCName(param.Type), varName);
                    // API実引数
                    argsText.AppendCommad("&" + varName);
                    // return
                    RubyCommon.MakeReturnCastExpCToVALUE(param.Type, varName, returnStmt);
                }
                // return として選択されていない引数である場合
                else
                {
                    // 通常引数とデフォルト引数のカウント
                    if (string.IsNullOrEmpty(param.OriginalDefaultValue))
                        normalArgsCount++;
                    else
                        defaultArgsCount++;

                    // out の場合は C++ 型で受け取るための変数を定義
                    if (param.IOModifier == IOModifier.Out)
                    {
                        // ruby は複数 out を扱えないためここに来ることはないはず
                        throw new InvalidOperationException();
                    }
                    // 入力引数 (in とinout)
                    else
                    {
                        // rb_scan_args の格納先 VALUE 宣言
                        scan_args_Inits.AppendLine("VALUE {0};", param.Name);
                        // rb_scan_args の引数
                        scan_args_Args.AppendCommad("&{0}", param.Name);
                        // 型チェック条件式
                        if (!string.IsNullOrEmpty(typeCheckExp))
                            typeCheckExp += " && ";
                        typeCheckExp += RubyCommon.GetTypeCheckExp(param.Type, param.Name);
                        // C変数宣言 & 初期化代入
                        initStmt.AppendLine(RubyCommon.GetDeclCastExpVALUEtoC(param.Type, "_" + param.Name, param.Name, param.OriginalDefaultValue));
                        // API実引数
                        if (param.Type is CLClass &&
                            ((CLClass)param.Type).IsStruct)
                            argsText.AppendCommad("&_" + param.Name);   // struct は 参照渡し
                        else
                            argsText.AppendCommad("_" + param.Name);
                    }
                }
            }

            // rb_scan_args の呼び出し
            string rb_scan_args_Text = "";
            if (!scan_args_Args.IsEmpty)
                rb_scan_args_Text = string.Format(@"rb_scan_args(argc, argv, ""{0}{1}"", {2});", normalArgsCount, (defaultArgsCount > 0) ? defaultArgsCount.ToString() : "", scan_args_Args);

            // 型チェック式が空なら true にしておく
            if (string.IsNullOrEmpty(typeCheckExp))
                typeCheckExp = "true";

            // エラーコードと throw
            string preErrorStmt = "";
            string postErrorStmt = "";
            if (method.FuncDecl.ReturnType.IsResultCodeType)
            {
                preErrorStmt = "LNResult errorCode = ";
                postErrorStmt = @"if (errorCode != LN_OK) rb_raise(g_luminoError, ""Lumino error. (%d)\n%s"", errorCode, LNGetLastErrorMessage());" + OutputBuffer.NewLineCode;
            }

            // API 呼び出し
            var apiCall = string.Format("{0}({1});", method.FuncDecl.OriginalFullName, argsText.ToString());

            // オーバーロードひとつ分の塊を作成する
            callBody.AppendWithIndent("if ({0} <= argc && argc <= {1}) {{", normalArgsCount.ToString(), (normalArgsCount + defaultArgsCount).ToString()).NewLine();
            callBody.IncreaseIndent();
            callBody.AppendWithIndent(scan_args_Inits.ToString());
            callBody.AppendWithIndent(rb_scan_args_Text).NewLine();
            callBody.AppendWithIndent("if ({0}) {{", typeCheckExp).NewLine();
            callBody.IncreaseIndent();
            callBody.AppendWithIndent(initStmt.ToString());
            callBody.AppendWithIndent(preErrorStmt + apiCall + OutputBuffer.NewLineCode);
            callBody.AppendWithIndent(postErrorStmt);
            callBody.AppendWithIndent(postStmt.ToString());
            callBody.AppendLine((returnStmt.IsEmpty) ? "return Qnil;" : returnStmt.ToString());
            callBody.DecreaseIndent();
            callBody.AppendWithIndent("}").NewLine();
            callBody.DecreaseIndent();
            callBody.AppendWithIndent("}").NewLine();

            funcBody.AppendWithIndent(callBody.ToString());
        }
    }
}
