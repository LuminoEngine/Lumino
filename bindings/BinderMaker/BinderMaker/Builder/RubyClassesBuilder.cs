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
 */

namespace BinderMaker.Builder
{
    class RubyBuilder : Builder
    {
        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop)
        {
        }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            return "";
            //string template = GetTemplate("RubyStructs.txt");
            //return template
            //    .Replace("__GLOBALS__", _allTypeDefineGlobalVariables.ToString())
            //    .Replace("__FUNCTIONS__", _allFuncDefines.ToString())
            //    .Replace("__DEFINES__", _allModuleDefines.ToString());
        }
    }
}
