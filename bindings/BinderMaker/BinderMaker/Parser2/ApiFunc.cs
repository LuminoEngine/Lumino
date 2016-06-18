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
    class ApiFunc
    {
        // 関数宣言 - API 修飾子
        private static readonly Parser<IEnumerable<char>> APIModifier =
                Parse.String(CLManager.APIModifier_Instance)    // LN_INSTANCE_API
            .Or(Parse.String(CLManager.APIModifier_Static)      // LN_STATIC_API
            .Or(Parse.String(CLManager.APIModifier_Internal))); // LN_INTERNAL_API
        
        // 属性引数
        public static readonly Parser<string> AttributeArgs =
            from lparen     in Parse.Char('(')
            from value      in ParserUtils.IdentifierOrNumeric
            from rparen     in Parse.Char(')')
            select "(" + value + ")";

        // 関数宣言 - 属性
        private static readonly Parser<string> FuncAttribute =
            from name       in  Parse.String(CLManager.APIAttribute_Property)               // LN_PROPERTY
                                .Or(Parse.String(CLManager.APIAttribute_Constructor))           // LN_STRUCT_CONSTRUCTOR
                                .Or(Parse.String(CLManager.APIAttribute_LibraryInitializer))    // LN_LIBRARY_INITIALIZER
                                .Or(Parse.String(CLManager.APIAttribute_LibraryTerminator))    // LN_LIBRARY_TERMINATOR
                                .Or(Parse.String(CLManager.APIAttribute_Overload))
            from args       in  AttributeArgs.Or(Parse.Return(""))       // 属性引数(opt)
            select new string(name.ToArray()) + args;
        
        // Handle 型
        public static readonly Parser<string> HandleType =
            from start      in Parse.String("LN_HANDLE").Text()
            from lparen     in Parse.Char('(').GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()
            from rparen     in Parse.Char(')')
            from ptr        in Parse.String("*").Or(Parse.Return(""))       // opt
            select start + lparen + name + rparen + new string(ptr.ToArray());  // とりあえずこの形式を許可したいだけなので文字列として返す

        
        // Handle 型
        public static readonly Parser<string> GenericReferenceType =
            from start      in Parse.String("LN_GENERIC_REFERENCE").Text()
            from lparen     in Parse.Char('(').GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()
            from rparen     in Parse.Char(')')
            from ptr        in Parse.String("*").Or(Parse.Return(""))       // opt
            select start + lparen + name + rparen + new string(ptr.ToArray());  // とりあえずこの形式を許可したいだけなので文字列として返す

        // 仮引数用の型名 (LN_HANDLE() または型名。LN_HANDLE は Identifer なので TypeName とのorの左側で先にパースする)
        private static readonly Parser<string> ParamType =
            HandleType
            .Or(GenericReferenceType)
            .Or(ParserUtils.TypeName);
        
        // デフォルト引数
        public static readonly Parser<string> FuncParamDefault =
            from mark       in Parse.String("LN_DEFAULT_ARG").GenericToken()
            from lparen     in Parse.Char('(').GenericToken()
            from value      in ParserUtils.IdentifierOrNumeric.GenericToken()
            from rparen     in Parse.Char(')').GenericToken()
            select value;

        // 仮引数定義
        public static readonly Parser<ParamDecl> FuncParamDecl =
            from attr       in Parse.String("LN_OUT").GenericToken().Or(Parse.Return(""))  // 引数属性(opt)
            from type       in ParamType.GenericToken()
            from name       in ParserUtils.Identifier//.GenericToken()
            from defaultVal in FuncParamDefault.Or(Parse.Return(""))        // opt
            select new ParamDecl(new string(attr.ToArray()), type, name, defaultVal);

        // 仮引数定義並び
        public static readonly Parser<IEnumerable<ParamDecl>> FuncParamDecls =
            from first1     in FuncParamDecl                                    // 先頭の1つ
            from rest1      in Parse.Char(',').Then(_ => FuncParamDecl).Many()  // 以降の , 区切りの定義が 0 個以上
            select ParserUtils.Cons(first1, rest1);

        // 関数宣言
        public static readonly Parser<FuncDecl> FuncDeclUnit =
            from docText    in (CLAPIDocument.CommentText.GenericToken()).Or(Parse.Return(""))  // コメント文字列 (/** ～ */)
            from apiMod     in APIModifier.GenericToken()
            from attr       in FuncAttribute.GenericToken().Many()//FuncAttribute.Or(Parse.Return(""))             // 属性(opt)
            from type1      in ParserUtils.TypeName.GenericToken()                  // 戻り値型
            from name1      in ParserUtils.Identifier.GenericToken()    // 関数名
            from lparen     in Parse.Char('(').GenericToken()
            from params1    in FuncParamDecls.Or(Parse.Return(new ParamDecl[0]))              // opt
            from rparen     in Parse.Char(')').GenericToken()
            from end1       in Parse.Char(';').GenericToken()
            select new FuncDecl(docText, apiMod, attr, type1, name1, params1);




        //// メソッド定義
        //public static readonly Parser<FuncDecl> MethodDecl =
        //    from docText    in (CLAPIDocument.CommentText.GenericToken()).Or(Parse.Return(""))  // コメント文字列 (/** ～ */)
        //    from funcText   in Parse.AnyChar.Until(Parse.String(";")).Text()                    // ; までの文字列 (";" は消費される)
        //    select DoParseDeclUnit(docText, funcText + ";");

        //public static FuncDecl DoParse(string text)
        //{
        //    return FuncDeclUnit.Parse(text);
        //}
    }
}
