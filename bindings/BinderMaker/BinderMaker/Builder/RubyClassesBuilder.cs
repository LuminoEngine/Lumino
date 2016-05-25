using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * Ruby のクラスは以下の要素でワンセット
 * ・フィールドを保持する struct (wrapStruct)
 * ・rb_define_class_under() で登録したクラスを保持するグローバル変数
 * ・メモリ確保関数 (alloc)
 *      alloc → rubyオブジェクトにラップ(mark、delete 登録) → 0クリア の定型文
 * ・メモリ解放・デストラクト関数 (delete)
 *      RefObj なら Release → free
 * ・GCマーク関数 (mark)
 *      wrapStruct の VALUE 型を mark
 * ×初期化関数 (initialize)
 *      コンストラクタ扱い。必須ではない
 *      
 * RefObject 型に必要なもの
 * ・allocateForGetRefObject 関数
 *      RefObject を返す getter 等で使用する。
 *      まだ ruby の管理に入っていない RefObject の WrapObject を作成するために使用する。
 * ・コンストラクタ関数は、定義は他のメソッドと同じようにし、rb_define_private_method() で "initialize" に登録する。
 * 
 * 
 * RefObject 型の"プロパティ"の実現に必要なもの
 * ・wrapStruct に VALUE 型のメンバを持たせる
 * ・allocate 関数で初期化する
 * ・これは GCMark の対象。
 *      
 */

namespace BinderMaker.Builder
{
    class RubyClassesBuilder : Builder
    {
        private static string AllocFuncDeclTemplate = @"
static VALUE __MODULE_NAME___allocate( VALUE klass )
{
    VALUE obj;
    __STRUCT_NAME__* internalObj;

    internalObj = (__STRUCT_NAME__*)malloc(sizeof(__STRUCT_NAME__));
    if (internalObj == NULL) rb_raise( g_luminoModule, ""Faild alloc - __MODULE_NAME___allocate"" );
    obj = Data_Wrap_Struct(klass, __MODULE_NAME___mark, __MODULE_NAME___delete, internalObj);
    
    memset(internalObj, 0, sizeof(__STRUCT_NAME__));
__INIT__
    return obj;
}
";
        private static string Alloc2FuncDeclTemplate = @"
static VALUE __MODULE_NAME___allocateForGetRefObject(VALUE klass, LNHandle handle)
{
    VALUE obj;
    __STRUCT_NAME__* internalObj;

    internalObj = (__STRUCT_NAME__*)malloc(sizeof(__STRUCT_NAME__));
    if (internalObj == NULL) rb_raise( g_luminoModule, ""Faild alloc - __MODULE_NAME___allocate"" );
    obj = Data_Wrap_Struct(klass, __MODULE_NAME___mark, __MODULE_NAME___delete, internalObj);
    
    memset(internalObj, 0, sizeof(__STRUCT_NAME__));
__INIT__
    internalObj->Handle = handle;
    return obj;
}
";
        private static string DeleteFuncDeclTemplate = @"
static void __MODULE_NAME___delete(__STRUCT_NAME__* obj)
{
    __CONTENTS__
    free(obj);
}
";
        private static string MarkFuncDeclTemplate = @"
static void __MODULE_NAME___mark(__STRUCT_NAME__* obj)
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

            /// <summary>
            /// 追加のラップ構造体メンバ
            /// </summary>
            public OutputBuffer AdditionalWrapStructMember = new OutputBuffer();

            /// <summary>
            /// 追加のラップ構造体メンバの初期化式
            /// </summary>
            public OutputBuffer AdditionalWrapStructMemberInit = new OutputBuffer();

            /// <summary>
            /// 追加のラップ構造体メンバのマーク
            /// </summary>
            public OutputBuffer AdditionalWrapStructMemberMark = new OutputBuffer();

            public CurrentClassInfo()
            {
                AdditionalWrapStructMember.IncreaseIndent();
                AdditionalWrapStructMemberInit.IncreaseIndent();
                AdditionalWrapStructMemberMark.IncreaseIndent();
            }
        }

        CurrentClassInfo _currentClassInfo;
        OutputBuffer _allWrapStructs = new OutputBuffer();
        OutputBuffer _allTypeDefineGlobalVariables = new OutputBuffer();
        OutputBuffer _allFuncDefines = new OutputBuffer();
        OutputBuffer _allModuleDefines = new OutputBuffer(1);
        OutputBuffer _typeinfoRegisterText = new OutputBuffer(1);    // LNXXXX_SetBindingTypeInfo() による型情報の登録処理

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (classType.IsExtension) return false;  // 拡張クラスはなにもしない
            if (classType.IsStruct) return false;
            if (classType.Name == "RefObject") return false;

            _currentClassInfo = new CurrentClassInfo();

            // Class 用グローバル変数
            string varName = RubyCommon.GetModuleVariableName(classType);
            _allTypeDefineGlobalVariables.AppendLine("VALUE {0};", varName);

            // Class 定義 (基底は rb_cObject)
            _allModuleDefines.AppendLine(@"{0} = rb_define_class_under(g_luminoModule, ""{1}"", rb_cObject);", varName, classType.Name);

            if (!classType.IsStatic)
            {
                // allocate 関数の登録
                _allModuleDefines.AppendLine(@"rb_define_alloc_func({0}, LN{1}_allocate);", varName, classType.Name);
            }

            //-------------------------------------------------
            // クラス型の場合は型情報の登録
            if (classType.IsReferenceObject)
            {
                // 中身はマクロに任せる
                _typeinfoRegisterText.AppendLine("LNRB_REGISTER_TYPEINFO({0});", classType.Name);
            }

            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            // メソッドを持つクラスを表現するグローバル変数
            string typeValName = RubyCommon.GetModuleVariableName(classType);

            //-------------------------------------------------
            // モジュールをラップする構造体
            _allWrapStructs.AppendLine("struct {0}", GetWrapStructName(classType));
            if (classType.BaseClass != null)  // 継承
                _allWrapStructs.AppendLine("    : public " + GetWrapStructName(classType.BaseClass));
            _allWrapStructs.AppendLine("{");
            // プロパティフィールド
            _allWrapStructs.AppendLine(_currentClassInfo.AdditionalWrapStructMember.ToString());
            _allWrapStructs.AppendLine("};").NewLine();

            //-------------------------------------------------
            // フレームワーク関数

            if (!classType.IsStatic)
            {
                string t;
                // delete
                string deleteFuncContents = "if (obj->Handle != 0) LNObject_Release(obj->Handle);";
                t = DeleteFuncDeclTemplate.Trim()
                    .Replace("__MODULE_NAME__", classType.OriginalName)
                    .Replace("__STRUCT_NAME__", GetWrapStructName(classType))
                    .Replace("__CONTENTS__", deleteFuncContents);
                _allFuncDefines.AppendWithIndent(t).NewLine(2);
                // mark
                t = MarkFuncDeclTemplate.Trim()
                    .Replace("__MODULE_NAME__", classType.OriginalName)
                    .Replace("__STRUCT_NAME__", GetWrapStructName(classType))
                    .Replace("__CONTENTS__", _currentClassInfo.AdditionalWrapStructMemberMark.ToString());
                _allFuncDefines.AppendWithIndent(t).NewLine(2);
                // allocate
                t = AllocFuncDeclTemplate.Trim()
                     .Replace("__MODULE_NAME__", classType.OriginalName)
                     .Replace("__STRUCT_NAME__", GetWrapStructName(classType))
                     .Replace("__INIT__", _currentClassInfo.AdditionalWrapStructMemberInit.ToString());
                _allFuncDefines.AppendWithIndent(t).NewLine(2);
                // allocateForGetRefObject
                t = Alloc2FuncDeclTemplate.Trim()
                    .Replace("__MODULE_NAME__", classType.OriginalName)
                    .Replace("__STRUCT_NAME__", GetWrapStructName(classType))
                    .Replace("__INIT__", _currentClassInfo.AdditionalWrapStructMemberInit.ToString());
                _allFuncDefines.AppendWithIndent(t).NewLine(2);
            }

            //-------------------------------------------------
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
            // RefObj型のプロパティは、フィールドに保持したい
            if (prop.Type is CLClass && ((CLClass)prop.Type).IsReferenceObject)
            {
                // WrapStruct のメンバを追加する
                _currentClassInfo.AdditionalWrapStructMember.AppendLine("VALUE {0};", prop.Name);
                // メンバの初期化
                _currentClassInfo.AdditionalWrapStructMemberInit.AppendLine("internalObj->{0} = Qnil;", prop.Name);
                // メンバの GCMark
                _currentClassInfo.AdditionalWrapStructMemberMark.AppendLine("rb_gc_mark(obj->{0});", prop.Name);
            }

            if (prop.Setter != null) OnMethodLooked(prop.Setter);
            if (prop.Getter != null) OnMethodLooked(prop.Getter);
        }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            if (method.Option.CheckDisabled(LangFlags.Ruby)) return;
            // Ruby として出力できるメソッドであるか
            if (!RubyCommon.CheckInvalidMethod(method)) return;

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
            string template = GetTemplate("RubyClasses.txt");
            return template
                .Replace("__WRAP_STRUCTS__", _allWrapStructs.ToString())
                .Replace("__GLOBALS__", _allTypeDefineGlobalVariables.ToString())
                .Replace("__FUNCTIONS__", _allFuncDefines.ToString())
                .Replace("__DEFINES__", _allModuleDefines.ToString())
                .Replace("__TYPEINFO_REGISTERS__", _typeinfoRegisterText.ToString());
        }

        /// <summary>
        /// wrapStruct 名の取得
        /// </summary>
        /// <param name="classType"></param>
        /// <returns></returns>
        private string GetWrapStructName(CLClass classType)
        {
            return "wrap" + classType.Name;
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
            if (baseMethod.IsStatic) funcName = "static_" + funcName;    // static メソッドの場合は先頭に static_ を付ける

            // 関数ヘッダ
            _allFuncDefines.AppendLine("static VALUE {0}(int argc, VALUE *argv, VALUE self)", funcName);
            _allFuncDefines.AppendLine("{");
            _allFuncDefines.IncreaseIndent();

            // インスタンスメソッドの場合は this を表すオブジェクトを Data_Get_Struct で取りだす
            var funcBody = new OutputBuffer();
            if (baseMethod.IsInstanceMethod)
            {
                funcBody.AppendLine("{0}* selfObj;", GetWrapStructName(baseMethod.OwnerClass));
                funcBody.AppendLine("Data_Get_Struct(self, {0}, selfObj);", GetWrapStructName(baseMethod.OwnerClass));
            }

            // Body 作成
            foreach (var method in overloadMethods)
            {
                // if () { ～ } までのオーバーロード呼び出し1つ分
                RubyCommon.MakeOverloadedMethod(method, funcBody);
            }

            // 関数終端まで到達してしまったら例外
            string rubyMethodName = RubyCommon.ConvertCommonNameToRubyMethodName(baseMethod);
            funcBody.AppendLine(@"rb_raise(rb_eArgError, ""Lumino::{0}.{1} - wrong argument type."");", baseMethod.OwnerClass.Name, rubyMethodName);
            funcBody.AppendLine("return Qnil;");

            // funcBody テキストを _allFuncDefines へ結合
            _allFuncDefines.AppendWithIndent(funcBody.ToString());

            // 関数終端の }
            _allFuncDefines.DecreaseIndent();
            _allFuncDefines.AppendLine("}").NewLine();

            // rb_define_method 等のメソッド登録を作成する
            RubyCommon.MakeRubyMethodRegister(_allModuleDefines, baseMethod, typeValName, rubyMethodName, funcName);
        }

    }
}
