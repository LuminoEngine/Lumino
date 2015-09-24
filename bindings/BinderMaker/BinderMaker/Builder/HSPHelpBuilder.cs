
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class HSPHelpBuilder : Builder
    {
        #region Templates
        const string HeaderTemplate = @"
;============================================================
; Lumino ヘルプファイル
;============================================================

%dll
Lumino

%ver
_LIBRARY_VERSION_

%date
_BUILD_TIME_

%author
lriki

%note
lumino.as をインクルードしてください

%type
拡張命令

%url
http://nnmy.sakura.ne.jp/

";

        const string FuncTemplate = @"
;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
_NAME_
_BRIEF_
%group
_GROUP_
%prm
_PRM_LIST_
_PRM_DETAIL_
%inst
_INST_
%href
_HREF_
";
        #endregion

        private OutputBuffer _output = new OutputBuffer();

        /// <summary>
        /// ビルド開始前(初期化)通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnInitialize()
        {
            // ヘッダ
            string t = HeaderTemplate.TrimStart();
            t = t.Replace("_LIBRARY_VERSION_", CLManager.GetLibararyVersion());
            t = t.Replace("_BUILD_TIME_", DateTime.Today.ToString("yyyy/MM/dd"));
            _output.AppendWithIndent(t);
        }

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
            if (method.Option.CheckDisabled(LangFlags.HSP)) return;

            // 基本
            string t = FuncTemplate.TrimStart();
            t = t.Replace("_NAME_", method.FuncDecl.OriginalFullName);
            t = t.Replace("_BRIEF_", method.Document.OriginalBriefText);
            t = t.Replace("_INST_", method.Document.OriginalDetailsText);
            t = t.Replace("_HREF_", "");
            t = t.Replace("_GROUP_", method.OwnerClass.OriginalName);

        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            return _output.ToString();
        }

        /// <summary>
        /// 出力ファイルのエンコーディング
        /// </summary>
        protected override Encoding GetOutputEncoding() { return Encoding.GetEncoding(932); }
    }
}
