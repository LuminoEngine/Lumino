using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Sprache;
using BinderMaker.Parser;
using BinderMaker.Decls;

namespace BinderMaker.Parser2
{
    class ApiClass
    {
        // クラス定義
        public static readonly Parser<ClassDecl> ClassDecl =
            from doc        in ApiDocument.CommentText.GenericToken()        // コメント文字列 (/** ～ */)
            from start      in Parse.String("LN_CLASS").Or(Parse.String("LN_STATIC_CLASS")).Or(Parse.String("LN_GENERIC_CLASS")).Or(Parse.String("LN_STRUCT_CLASS")).Or(Parse.String("LN_EXTENSION_CLASS"))
            from lparen     in Parse.Char('(').GenericToken()                   // (
            //from name       in ParserUtils.Identifier                           // クラス名
            //from c          in Parse.Char(',').Or(Parse.Return(' '))              // opt
            //from baseClass  in ParserUtils.Identifier.GenericToken().Or(Parse.Return(""))              // opt
            from args       in ParserUtils.CommadIdentifiers
            from rparen     in Parse.Char(')').GenericToken()                   // )
            from body       in Parse.AnyChar.Until(Parse.String("LN_CLASS_END")).Text()    // 終端キーワードが見つかるまで ("LN_CLASS_END" は消費される)
            select new ClassDecl(start, doc, args, body);

        // body
        public static readonly Parser<IEnumerable<FuncDecl>> ClassBody =
            //from lead       in Parse.AnyChar.Except(CLAPIDocument.CommentStart).Many()  // 最初のドキュメントコメントまでを読み飛ばす (/** は消費しない)
            from methods    in ApiFunc.FuncDeclUnit.Many()                            // 任意の数のメソッド定義
            select methods;

        /// <summary>
        /// パース実行
        /// </summary>
        public static IEnumerable<FuncDecl> DoParseClassBody(string text)
        {
            return ClassBody.Parse(text);
        }
    }
}
