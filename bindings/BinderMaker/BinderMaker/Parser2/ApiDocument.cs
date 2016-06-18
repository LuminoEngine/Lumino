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
    class ApiDocument
    {
        // ドキュメントコメントの開始マーク
        public static readonly Parser<IEnumerable<char>> CommentStart = Parse.String("/**");

        // ドキュメントコメントの終了マーク
        public static readonly Parser<IEnumerable<char>> CommentEnd = Parse.String("*/");

        // ドキュメントコメント
        public static readonly Parser<string> CommentText =
            from start in Parse.String("/**")
            from text in Parse.AnyChar.Until(Parse.String("*/")).Text()       // "*/" が見つかるまで任意の文字を繰り返す。見つかった "*/" は破棄される。
            select "/**" + text + "*/";


        // ドキュメントコメントの 1セクション の終了マーク
        public static readonly Parser<IEnumerable<char>> DocumentCommentSectionEnd =
            Parse.String("@").XOr(Parse.String("*/"));    // 次の @ または //*/

        // ドキュメントコメント - グループ
        public static readonly Parser<string> DocumentCommentDefGroup =
            from mark in Parse.String("@defgroup").GenericToken()                         // "@defgroup" から
            from tag in ParserUtils.Identifier.GenericToken()
            from text in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select text;

        // ドキュメントコメント - 概要
        public static readonly Parser<string> DocumentCommentBrief =
            from mark1 in Parse.String("@brief").GenericToken()                            // "@biref" から
            from summary1 in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select summary1;

        // ドキュメントコメント - 引数
        public static readonly Parser<ParamDocumentDecl> DocumentCommentParam =
            from start in Parse.String("@param").GenericToken()
            from io in Parse.Regex(@"\[.+\]").Text()
            from name in Parse.AnyChar.Until(Parse.Char(':'))                             // : までの全ての文字を取りだす。':' は消費される。
            from text in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select new ParamDocumentDecl(io, new string(name.ToArray()), text);

        // ドキュメントコメント - 戻り値
        public static readonly Parser<string> DocumentCommentReturn =
            from start in Parse.String("@return").GenericToken()
            from text in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select text;

        // ドキュメントコメント - 詳細
        public static readonly Parser<string> DocumentCommentDetails =
            from start in Parse.String("@details").GenericToken()
            from text in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select text;

        // ドキュメントコメント - スコープ開始
        public static readonly Parser<string> DocumentCommentStartScope =
            from mark in Parse.String("@{").GenericToken()
            from text in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select text;

        // ドキュメントコメント
        public static readonly Parser<DocumentDecl> DocumentComment =
            from start in Parse.String("/**")
            from group1 in DocumentCommentDefGroup.Or(Parse.Return(""))
            from brief in DocumentCommentBrief
            from params1 in DocumentCommentParam.Many()      // 引数 (0個以上)
            from return1 in DocumentCommentReturn.Or(Parse.Return(""))
            from details in DocumentCommentDetails.Or(Parse.Return(""))
            from scope in DocumentCommentStartScope.Or(Parse.Return(""))
            from text in Parse.AnyChar.Until(Parse.String("*/")).Text()       // "*/" が見つかるまで任意の文字を繰り返す。見つかった "*/" は破棄される。
            select new DocumentDecl(group1, brief, params1, return1, details);

        /// <summary>
        /// パース実行
        /// </summary>
        public static DocumentDecl DoParse(string text)
        {
            return DocumentComment.Parse(text);
        }
    }
}
