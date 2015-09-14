using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 *  enum は Module の定数として定義する。
 *  必要なものは、
 *  ・Module を表すグローバル変数
 *  ・Init 時の rb_define_module_under
 *  ・モジュールメンバの rb_define_const
 */

namespace BinderMaker.Builder
{
    class RubyEnumBuilder : Builder
    {
        OutputBuffer _allTypeDefineGlobalVariables = new OutputBuffer();
        OutputBuffer _allModuleDefine = new OutputBuffer(1);

        /// <summary>
        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnEnumLooked(CLEnum enumType)
        {
            // 出力しないもの
            if (enumType.Name == "Bool")
                return;

            // Module 用グローバル変数
            string varName = "g_enum_" + enumType.Name;
            _allTypeDefineGlobalVariables.AppendLine("VALUE {0};", varName);

            // Module 定義
            _allModuleDefine.AppendLine(@"{0} = rb_define_module_under(g_luminoModule, ""{1}"");", varName, enumType.Name);

            // const 定義
            foreach (var member in enumType.Members)
            {
                if (member.IsTerminator) continue;  // ターミネータは出力しない

                string name = member.CommonName.ToUpper();
                _allModuleDefine.AppendLine(@"rb_define_const({0}, ""{1}"", INT2FIX({2}));", varName, name, member.Value);
            }
            _allModuleDefine.NewLine();
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string template = GetTemplate("RubyEnums.txt");
            return template
                .Replace("__GLOBALS_", _allTypeDefineGlobalVariables.ToString())
                .Replace("__DEFINES__", _allModuleDefine.ToString());
        }
    }
}
