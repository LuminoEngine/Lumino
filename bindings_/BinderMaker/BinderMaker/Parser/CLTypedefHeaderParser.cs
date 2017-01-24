using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;
using BinderMaker.Parser2;

namespace BinderMaker.Parser
{
#if true
    /// <summary>
    /// enum、struct、関数ポインタの定義ファイル (ヘッダファイル) を解析する
    /// </summary>
    class CLTypedefHeaderParser
    {
        #region struct

        // struct メンバ
        private static readonly Parser<CLStructMember> StructMember =
            from type       in ParserUtils.TypeName.GenericToken()      // 型名
            from name       in ParserUtils.Identifier.GenericToken()    // メンバ名
            from colon      in Parse.Char(';').GenericToken()           // 文末
            from comment    in ParserUtils.DoxyLineComment2.Or(Parse.Return(""))     // コメントもメンバ扱い(opt)
            select new CLStructMember(type, name, comment);
        
        // struct 定義
        private static readonly Parser<CLStruct> StructDecl =
            from comment    in ParserUtils.DoxyLineComment1             // コメント
            from key1       in Parse.String("typedef").GenericToken()   // "typedef"
            from key12      in Parse.String("struct").GenericToken()    // "struct"
            from name       in ParserUtils.Identifier.GenericToken()    // 名前 ("tagXXXX" となっているはず)
            from lbrace     in Parse.Char('{').GenericToken()           // {
            from members    in StructMember.Many()                      //  メンバ
            from rbrace     in Parse.Char('}').GenericToken()           // }
            from name2      in ParserUtils.Identifier.GenericToken()    // typedef 名
            from end        in Parse.Char(';').GenericToken()           // ;
            select new CLStruct(comment, name2, members);

        #endregion

        #region enum

        // enum メンバの定数定義
        private static readonly Parser<string> EnumMemberValue =
            from equal      in Parse.Char('=').GenericToken()
            from value      in ParserUtils.IdentifierOrNumeric.GenericToken()
            select value;

        // enum メンバ
        private static readonly Parser<CLEnumMember> EnumMember =
            from name       in ParserUtils.Identifier.GenericToken()
            from value      in EnumMemberValue.Or(Parse.Return(""))     // 定数は opt
            from comma      in Parse.Char(',').GenericToken()           // 終端,
            from comment    in ParserUtils.DoxyLineComment2.Or(Parse.Return(""))     // コメントもメンバ扱い(opt)
            select new CLEnumMember(name, value, comment);

        // enum 定義
        private static readonly Parser<CLEntity> EnumDecl =
            from comment    in ParserUtils.DoxyLineComment1             // コメント
            from key1       in Parse.String("typedef").GenericToken()   // "typedef"
            from start      in Parse.String("enum").GenericToken()      // "enum"
            from name       in ParserUtils.Identifier.GenericToken()    // 名前 ("tagXXXX" となっているはず)
            from lbrace     in Parse.Char('{').GenericToken()           // {
            from members    in EnumMember.Many()                        // メンバ
            from rbrace     in Parse.Char('}').GenericToken()           // }
            from name2      in ParserUtils.Identifier.GenericToken()    // typedef 名
            from end        in Parse.Char(';').GenericToken()           // ;
            select new CLEnum(comment, name2, members);

        #endregion

        #region 関数ポインタ

        // 関数ポインタ
        private static readonly Parser<CLEntity> FuncPointerDecl =
            from doc        in ApiDocument.DocumentComment        // コメント
            from start      in Parse.String("typedef").GenericToken()
            from type       in ParserUtils.TypeName.GenericToken()      // 戻り値型
            from lparen1    in Parse.Char('(').GenericToken()
            from ast        in Parse.Char('*').GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()    // 名前
            from rparen1    in Parse.Char(')').GenericToken()
            from lparen     in Parse.Char('(').GenericToken()
            from params1    in ApiFunc.FuncParamDecls.Or(Parse.Return<IEnumerable<Decls.ParamDecl>>(null))    // 引数リストopt
            from rparen     in Parse.Char(')').GenericToken()
            from end1       in Parse.Char(';').GenericToken()
            select new CLDelegate(doc, type, name, params1);

        #endregion

        #region フレームワーク

        // 定義要素
        private static readonly Parser<CLEntity> DeclContent =
                StructDecl
            .Or(EnumDecl)
            .Or(FuncPointerDecl);

        // ルート要素
        private static readonly Parser<IEnumerable<CLEntity>> CompileUnit =
            from lead       in Parse.AnyChar.Except(Parse.String("/**")).Many()   // ファイル先頭から最初のドキュメントコメントまでを読み飛ばす (/** は消費しない)
            from decls      in DeclContent.Many()
            from any        in Parse.AnyChar.Many()     // 後は適当な文字
            select decls;

        #endregion

        #region Methods

        public List<CLEntity> Analyze(string filePath)
        {
            string text = System.IO.File.ReadAllText(filePath);
            text = text.Replace("\r\n", "\n");  // 改行コードは LF に統一
            var decls = CompileUnit.Parse(text);
            return new List<CLEntity>(decls);
        }

        #endregion
    }
#endif
}
