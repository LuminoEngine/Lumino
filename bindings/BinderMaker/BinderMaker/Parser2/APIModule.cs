using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;
using BinderMaker.Parser;
using BinderMaker.Decls;

namespace BinderMaker.Parser2
{
    class ApiModule
    {
        // モジュール
        public static readonly Parser<BinderMaker.Decls.ModuleDecl> Module =
            from doc        in ApiDocument.CommentText.GenericToken()         // コメント文字列 (/** ～ */)
            from start      in Parse.String("LN_MODULE")                        // キーワード
            from lparen     in Parse.Char('(').GenericToken()                   // (
            from name       in ParserUtils.IdentifierOrNumeric.GenericToken()   // モジュール名
            from rparen     in Parse.Char(')').GenericToken()                   // )
            from body       in Parse.AnyChar.Until(Parse.String("LN_MODULE_END")).Text()    // 終端キーワードが見つかるまで ("LN_MODULE_END" は消費される)
            select new BinderMaker.Decls.ModuleDecl(name, doc, body);

        // ルート要素
        public static readonly Parser<BinderMaker.Decls.ModuleDecl> CompileUnit =
            from lead       in Parse.AnyChar.Except(CLAPIDocument.CommentStart).Many()   // ファイル先頭から最初のドキュメントコメントまでを読み飛ばす。("/**" は消費しない)
            from module     in Module               // 1 つの Module
            from any        in Parse.AnyChar.Many()     // 後は適当な文字
            select module;

        // body
        public static readonly Parser<IEnumerable<ClassDecl>> ModuleBody =
            from lead       in Parse.AnyChar.Except(CLAPIDocument.CommentStart).Many()  // 最初のドキュメントコメントまでを読み飛ばす (/** は消費しない)
            from classes    in ApiClass.ClassDecl.Many()                              // 任意の数のクラス定義
            select classes;
        
        /// <summary>
        /// パース実行
        /// </summary>
        public static BinderMaker.Decls.ModuleDecl DoParse(string text)
        {
            return CompileUnit.Parse(text);
        }

        /// <summary>
        /// パース実行
        /// </summary>
        public static IEnumerable<ClassDecl> DoParseModuleBody(string text)
        {
            return ModuleBody.Parse(text);
        }
    }
}
