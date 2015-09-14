using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker.Parser
{
    static class ParserUtils
    {
        /// <summary>
        /// 識別子
        /// </summary>
        public static readonly Parser<string> Identifier =
            from ident1 in Parse.LetterOrDigit.XOr(Parse.Char('_'))         // 1個必ず
            from ident2 in Parse.LetterOrDigit.XOr(Parse.Char('_')).Many()  // 0 個以上の繰り返し
            select ident1 + new string(ident2.ToArray());

        /// <summary>
        /// 識別子or数値 (_ や - を含む)
        /// </summary>
        public static readonly Parser<string> IdentifierOrNumeric =
            from ident1 in Parse.LetterOrDigit.XOr(Parse.Char('-')).XOr(Parse.Char('_')).XOr(Parse.Char('.'))
            from ident2 in Parse.LetterOrDigit.XOr(Parse.Char('-')).XOr(Parse.Char('_')).XOr(Parse.Char('.')).Many()
            select ident1 + new string(ident2.ToArray());

        /// <summary>
        /// 空白トークン
        /// Sprache の WhiteSpace は \n を含まないため、それを含むものを自分で用意。
        /// </summary>
        private static readonly Parser<IEnumerable<char>> GenericSpaceToken =
                 Parse.WhiteSpace.Many()        // 改行以外の空白
            .XOr(Parse.String("\n"))            // または改行
            .XOr(Parse.Regex(@"//.*\n"))        // または行コメント
            ;      

        /// <summary>
        /// 空白並び
        /// </summary>
        public static readonly Parser<string> GenericSpace =
            from sp in GenericSpaceToken.Many()
            select EmToString(sp);


        /// <summary>
        /// 型名 (識別子と、const、ポインタの * を含む)
        /// </summary>
        public static readonly Parser<string> TypeName =
            from modif1 in Parse.String("const").GenericToken().XOr(Parse.Return("")).Text()   // opt 修飾子 (const とか)
            from ident in ParserUtils.Identifier.GenericToken()
            from ast1 in (Parse.String("*").GenericToken()).XOr(Parse.Return("")).Text()   // opt
            from ast2 in (Parse.String("*").GenericToken()).XOr(Parse.Return("")).Text()   // opt ダブルポインタ許可
            select (modif1 + ((string.IsNullOrEmpty(modif1)) ? "" : " ") + new string(ident.ToArray()) + ast1 + ast2);

        /// <summary>
        /// 前後に空白を含むトークン
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="parser"></param>
        /// <returns></returns>
        public static Parser<T> GenericToken<T>(this Parser<T> parser)
        {
            if (parser == null)
            {
                throw new ArgumentNullException("parser");
            }
            return 
                from leading in GenericSpace
                from item in parser
                from trailing in GenericSpace
                select item;
        }

        /// <summary>
        /// Doxygen の行コメント
        /// </summary>
        public static readonly Parser<string> DoxyLineComment1 =
            from start in Parse.String("/**")
            //from comment in Parse.AnyChar.Until(Parse.String("*/")).Many()
            //from comment in Parse.AnyChar.Except(Parse.Char('\n')).Many().Text() // \n 以外の文字。\n は消費しない
            //from eol in Parse.String("*/")
            //select EmToString(comment);
            from comment in Parse.AnyChar.Except(Parse.String("*/")).Many().Text() // \n 以外の文字。\n は消費しない
            from eol in Parse.String("*/")
            select comment;

        /// <summary>
        /// Doxygen の行コメント
        /// </summary>
        public static readonly Parser<string> DoxyLineComment2 =
            from start in Parse.String("/**")
            //from comment in Parse.AnyChar.Until(Parse.String("*/")).Many()
            //select EmToString(comment);
            from comment in Parse.AnyChar.Except(Parse.String("*/")).Many().Text() // \n 以外の文字。\n は消費しない
            from eol in Parse.String("*/")
            select comment;


        /// <summary>
        /// 先頭に要素を挿入した IEnumerable を作る
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="head"></param>
        /// <param name="rest"></param>
        /// <returns></returns>
        public static IEnumerable<T> Cons<T>(T head, IEnumerable<T> rest)
        {
            yield return head;
            foreach (var item in rest)
                yield return item;
        }

        private static string EmToString(IEnumerable< IEnumerable<char> > list)
        {
            string t = "";
            foreach (var e in list)
            {
                t += new string(e.ToArray());
            }
            return t;
        }
    }
}
