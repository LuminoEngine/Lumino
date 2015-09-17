using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  必要なものは、
 *  ・Module を表すグローバル変数
 *  ・Init 時の rb_define_class_under
 *  ・rb_define_alloc_func で allocate 関数を登録
 *  ・allocate 関数内で delete 関数を登録
 *  ・rb_define_private_method で initialize 関数を登録
 *  ・rb_define_method で各メンバメソッドを登録
 *  
 * 
 *  メソッド呼び出しは以下のような部分で一塊。オーバーロードする場合はこの塊が複数ある。
    if (1 <= argc && argc <= 3) {
        VALUE xSize;
        VALUE ySize;
        VALUE zSize;
        rb_scan_args(argc, argv, "12", &xSize, &ySize, &zSize);
        if (isRbNumber(xSize) && isRbNumber(ySize) && isRbNumber(zSize)) {
            int _xSize = FIX2INT(xSize);
            int _ySize = (ySize != Qnil) ? FIX2INT(ySize) : 1;
            int _zSize = (zSize != Qnil) ? FIX2INT(zSize) : 1;
            lnErrorCode errorCode = LNIntTable_Create(&selfObj->Handle, _xSize, _ySize, _zSize);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;
        }
    }
 */

namespace BinderMaker.Builder
{
    class RubyStructsBuilder : Builder
    {
        private static string StructAllocFuncTemplate = @"
VALUE __MODULE_NAME___allocate( VALUE klass )
{
    VALUE obj;
    __STRUCT_NAME__* internalObj;

    internalObj = (__STRUCT_NAME__*)malloc(sizeof(__STRUCT_NAME__));
    if (internalObj == NULL) rb_raise( g_luminoModule, ""Faild alloc - __MODULE_NAME___allocate"" );
    obj = Data_Wrap_Struct(klass, NULL, __MODULE_NAME___delete, internalObj);
    
    memset(internalObj, 0, sizeof(__STRUCT_NAME__));

    return obj;
}
";
        private static string StructInitializeFuncTemplate = @"
VALUE __MODULE_NAME___struct_initialize( int argc, VALUE *argv, VALUE self )
{
    __STRUCT_NAME__* selfObj;
    Data_Get_Struct(self, __STRUCT_NAME__, selfObj);
    if (argc == 0) {    // 引数 0 個を許可する
        return self;
    }
__CONTENTS__
}
";
        private static string StructDeleteFuncTemplate = @"
void __MODULE_NAME___delete(__STRUCT_NAME__* obj)
{
    free(obj);
}
";
        // __CAST__ は v をキャストする式にすること。
        private static string StructSetterFuncTemplate = @"
static VALUE __MODULE_NAME_____MEMBER___set(VALUE self, VALUE v)
{
    __STRUCT_NAME__* selfObj;
    Data_Get_Struct(self, __STRUCT_NAME__, selfObj);
    selfObj->__MEMBER__ = __CAST__;
    return self;
}
";
        private static string StructGetterFuncTemplate = @"
static VALUE __MODULE_NAME_____MEMBER___get(VALUE self)
{
    __STRUCT_NAME__* selfObj;
    Data_Get_Struct(self, __STRUCT_NAME__, selfObj);
    return toVALUE(selfObj->__MEMBER__);
}
";
        private static string FuncDeclTemplate = @"
static VALUE __FUNC_NAME__(int argc, VALUE *argv, VALUE self)
{
__CONTENTS__
}
";
        class CurrentClassInfo
        {
            /// <summary>
            /// オーバーロードメソッドの集計。
            /// static メソッドとインスタンスメソッドは分ける。
            /// static メソッドはキー名の頭に "static" を付ける。
            /// </summary>
            public Dictionary<string, List<CLMethod>> OverloadTable = new Dictionary<string, List<CLMethod>>();
        }

        CurrentClassInfo _currentClassInfo;
        OutputBuffer _allTypeDefineGlobalVariables = new OutputBuffer();
        OutputBuffer _allFuncDefines = new OutputBuffer();
        OutputBuffer _allModuleDefines = new OutputBuffer(1);

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (!classType.IsStruct) return false;

            _currentClassInfo = new CurrentClassInfo();

            // Class 用グローバル変数
            string varName = RubyCommon.GetModuleVariableName(classType);
            _allTypeDefineGlobalVariables.AppendLine("VALUE {0};", varName);

            // Class 定義 (基底は rb_cObject)
            _allModuleDefines.AppendLine(@"{0} = rb_define_class_under(g_luminoModule, ""{1}"", rb_cObject);", varName, classType.Name);

            // allocate 関数の登録
            _allModuleDefines.AppendLine(@"rb_define_alloc_func({0}, LN{1}_allocate);", varName, classType.Name);

            // initialize 関数の登録
            _allModuleDefines.AppendLine(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC(LN{1}_struct_initialize), -1);", varName, classType.Name);

            //-------------------------------------------------
            // delete 関数の定義
            _allFuncDefines.AppendWithIndent(
               StructDeleteFuncTemplate
                 .Replace("__MODULE_NAME__", classType.OriginalName)
                 .Replace("__STRUCT_NAME__", classType.OriginalName));

            //-------------------------------------------------
            // allocate 関数の定義
            _allFuncDefines.AppendWithIndent(
               StructAllocFuncTemplate
                 .Replace("__MODULE_NAME__", classType.OriginalName)
                 .Replace("__STRUCT_NAME__", classType.OriginalName));

            //-------------------------------------------------
            // initialize 関数の定義
            var initStmtText = new OutputBuffer();
            var scan_argsText = new OutputBuffer();
            var setStmtText = new OutputBuffer();
            foreach (var member in classType.StructData.Members)
            {
                initStmtText.AppendLine("VALUE {0}_;", member.Name);
                scan_argsText.AppendCommad("&{0}_", member.Name);
                setStmtText.AppendLine("selfObj->{0} = {1};", member.Name, RubyCommon.GetCastExpVALUEtoC(member.Type, member.Name + "_"));
            }

            var contentsText = new OutputBuffer(1);
            contentsText.AppendWithIndent(initStmtText.ToString());
            contentsText.AppendLine(@"rb_scan_args(argc, argv, ""{0}"", {1});", classType.StructData.Members.Count().ToString(), scan_argsText.ToString());
            contentsText.AppendWithIndent(setStmtText.ToString());
            contentsText.AppendWithIndent("return self;");

            _allFuncDefines.Append(
               StructInitializeFuncTemplate
                 .Replace("__MODULE_NAME__", classType.OriginalName)
                 .Replace("__STRUCT_NAME__", classType.OriginalName)
                 .Replace("__CONTENTS__", contentsText.ToString()));

            //-------------------------------------------------
            // アクセサ
            foreach (var member in classType.StructData.Members)
            {
                string t;
                // set
                t = StructSetterFuncTemplate.Trim()
                    .Replace("__MODULE_NAME__", classType.OriginalName)
                    .Replace("__STRUCT_NAME__", classType.OriginalName)
                    .Replace("__MEMBER__", member.Name)
                    .Replace("__CAST__", RubyCommon.GetCastExpVALUEtoC(member.Type, "v"));
                _allFuncDefines.AppendWithIndent(t).NewLine(2);
                t = string.Format(@"rb_define_method({0}, ""{1}="", LN_TO_RUBY_FUNC({2}_{3}_set), 1);", varName, RubyCommon.GetSnakeStyleName(member.Name), classType.OriginalName, member.Name);
                _allModuleDefines.AppendLine(t);

                // get
                t = StructGetterFuncTemplate.Trim()
                    .Replace("__MODULE_NAME__", classType.OriginalName)
                    .Replace("__STRUCT_NAME__", classType.OriginalName)
                    .Replace("__MEMBER__", member.Name);
                _allFuncDefines.AppendWithIndent(t).NewLine(2);
                t = string.Format(@"rb_define_method({0}, ""{1}"", LN_TO_RUBY_FUNC({2}_{3}_get), 0);", varName, RubyCommon.GetSnakeStyleName(member.Name), classType.OriginalName, member.Name);
                _allModuleDefines.AppendLine(t);
            }

            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            // メソッド出力
            foreach (var overloads in _currentClassInfo.OverloadTable)
            {
                MakeFuncDecl(overloads.Value);
            }
            _allModuleDefines.NewLine();
        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop)
        {
            if (prop.Setter != null) OnMethodLooked(prop.Setter);
            if (prop.Getter != null) OnMethodLooked(prop.Getter);
        }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            // Ruby として出力できるメソッドであるか
            if (!RubyCommon.CheckInvalidMethod(method))
                return;

            // オーバーロードの集計
            string key = (method.IsStatic) ? "static" + method.Name : method.Name;
            if (_currentClassInfo.OverloadTable.ContainsKey(key))
            {
                _currentClassInfo.OverloadTable[key].Add(method);
            }
            else // 新しく登録
            {
                _currentClassInfo.OverloadTable[key] = new List<CLMethod>() { method };
            }
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string template = GetTemplate("RubyStructs.txt");
            return template
                .Replace("__GLOBALS__", _allTypeDefineGlobalVariables.ToString())
                .Replace("__FUNCTIONS__", _allFuncDefines.ToString())
                .Replace("__DEFINES__", _allModuleDefines.ToString());
        }

        /// <summary>
        /// オーバーロードを考慮しつつ、関数定義を作成する
        /// </summary>
        private void MakeFuncDecl(List<CLMethod> overloadMethods)
        {
            CLMethod baseMethod = overloadMethods[0];

            // メソッドを持つクラスを表現するグローバル変数
            string typeValName = RubyCommon.GetModuleVariableName(baseMethod.OwnerClass);

            // 関数名
            string funcName = "lnrb" + baseMethod.FuncDecl.OriginalFullName;
            if (baseMethod.IsStatic)
                funcName = "static_" + funcName;    // static メソッドの場合は先頭に static_ を付ける
            var funcBody = new OutputBuffer();
            funcBody.IncreaseIndent();

            // インスタンスメソッドの場合は this を表すオブジェクトを Data_Get_Struct で取りだす
            if (baseMethod.IsInstanceMethod)
            {
                funcBody.AppendLine("{0}* selfObj;", baseMethod.OwnerClass.OriginalName);
                funcBody.AppendLine("Data_Get_Struct(self, {0}, selfObj);", baseMethod.OwnerClass.OriginalName);
            }


            foreach (var method in overloadMethods)
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
                    // 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
                    if (param == method.FuncDecl.Params.First() && method.IsInstanceMethod)
                    {
                        argsText.Append("selfObj"); 
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
                if (method.ReturnType.IsResultCodeType)
                {
                    preErrorStmt = "LNResult errorCode = ";
                    postErrorStmt = @"if (errorCode != LN_OK) rb_raise(g_luminoError, ""internal error. code:%d"", errorCode);" + OutputBuffer.NewLineCode;
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
                callBody.AppendWithIndent((returnStmt.IsEmpty) ? "return Qnil;" : returnStmt.ToString());
                callBody.DecreaseIndent();
                callBody.AppendWithIndent("}").NewLine();
                callBody.DecreaseIndent();
                callBody.AppendWithIndent("}").NewLine();

                funcBody.AppendWithIndent(callBody.ToString());
            }

            // メソッド名
            string rubyMethodName = RubyCommon.ConvertCommonNameToRubyMethodName(baseMethod);

            // 関数終端まで到達することはない。仮の return
            funcBody.AppendWithIndent(@"rb_raise(rb_eArgError, ""Lumino::{0}.{1} - wrong argument type."");", baseMethod.OwnerClass.Name, rubyMethodName).NewLine();
            funcBody.AppendWithIndent("return Qnil;");

            // 関数作成
            string t = FuncDeclTemplate.Trim()
                .Replace("__FUNC_NAME__", funcName)
                .Replace("__CONTENTS__", funcBody.ToString());
            _allFuncDefines.AppendWithIndent(t).NewLine(2);

            // rb_define_method 等のメソッド登録を作成する
            RubyCommon.MakeRubyMethodRegister(_allModuleDefines, baseMethod, typeValName, rubyMethodName, funcName);
        }

    }

    class RubyStructsHeaderBuilder : Builder
    {
        OutputBuffer _externs = new OutputBuffer();

        /// <summary>
        /// ビルド開始前(初期化)通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnInitialize()
        {
            _externs.AppendLine("#pragma once");
        }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (!classType.IsStruct) return false;

            _externs.AppendLine("extern VALUE {0}_allocate(VALUE klass);", classType.OriginalName);

            return true;
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            return _externs.ToString();
        }
    }
}
