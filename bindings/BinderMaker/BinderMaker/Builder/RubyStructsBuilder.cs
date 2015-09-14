using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  必要なものは、
 *  ・Module を表すグローバル変数
 *  ・Init 時の rb_define_class_under
 *  ・rb_define_alloc_func で Alloc 関数を登録
 *  ・rb_define_private_method で initialize 関数を登録
 *  ・rb_define_method で各メンバメソッドを登録
 */

namespace BinderMaker.Builder
{
    class RubyStructsBuilder : Builder
    {
        OutputBuffer _allTypeDefineGlobalVariables = new OutputBuffer();
        OutputBuffer _allModuleDefine = new OutputBuffer(1);

        private static string StructAllocFuncTemplate = @"
static VALUE __MODULE_NAME___allocate( VALUE klass )
{
    VALUE obj;
    __STRUCT_NAME__* internalObj;

    internalObj = (__STRUCT_NAME__*)malloc(sizeof(__STRUCT_NAME__));
    if (internalObj == NULL) rb_raise( gLNoteModule, ""Faild alloc - __MODULE_NAME___allocate"" );
    obj = Data_Wrap_Struct(klass, __MODULE_NAME___mark, __MODULE_NAME___delete, internalObj);
    
    memset(internalObj, 0, sizeof(__STRUCT_NAME__));

    return obj;
}
";

        private static string StructInitializeFuncTemplate = @"
static VALUE __MODULE_NAME___struct_initialize( int argc, VALUE *argv, VALUE self )
{
    __STRUCT_NAME__* selfObj;
    Data_Get_Struct(self, __STRUCT_NAME__, selfObj);
    if (argc == 0) {
        return self;
    }
__CONTENTS__
}
";
        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (!classType.IsStruct) return false;

            // Class 用グローバル変数
            string varName = "g_struct_" + classType.Name;
            _allTypeDefineGlobalVariables.AppendLine("VALUE {0};", varName);

            // Class 定義 (基底は rb_cObject)
            _allModuleDefine.AppendLine(@"{0} = rb_define_class_under(g_luminoModule, ""{1}"", rb_cObject);", varName, classType.Name);

            // allocate 関数の登録
            _allModuleDefine.AppendLine(@"rb_define_alloc_func({0}, LN{1}_allocate);", varName, classType.Name);

            // initialize 関数の登録
            _allModuleDefine.AppendLine(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC(LN{1}_struct_initialize), -1);", varName, classType.Name);
            
            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            _allModuleDefine.NewLine();
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string template = GetTemplate("RubyStructs.txt");
            return template
                .Replace("__GLOBALS_", _allTypeDefineGlobalVariables.ToString())
                .Replace("__DEFINES__", _allModuleDefine.ToString());
        }
    }
}
