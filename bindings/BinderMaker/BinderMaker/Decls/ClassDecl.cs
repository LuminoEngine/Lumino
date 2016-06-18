using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace BinderMaker.Decls
{
    class ClassDecl
    {
        #region Properties

        /// <summary>
        /// ドキュメント
        /// </summary>
        public DocumentDecl Document { get; private set; }

        /// <summary>
        /// オリジナルのクラス名 (LN プレフィックス付き)
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// ベースクラスのオリジナルのクラス名 (LN プレフィックス付き)
        /// </summary>
        public string BaseClassOriginalName { get; private set; }
        
        /// <summary>
        /// メンバメソッドリスト
        /// </summary>
        public List<FuncDecl> FuncDecls { get; private set; }

        /// <summary>
        /// static クラス であるか
        /// </summary>
        public bool IsStatic { get; private set; }

        /// <summary>
        /// ジェネリッククラス であるか
        /// </summary>
        public bool IsGeneric { get; private set; }

        /// <summary>
        /// struct であるか
        /// </summary>
        public bool IsStruct { get; private set; }

        /// <summary>
        /// 拡張クラス であるか (各言語のコードで独自定義するもの。RefObject など)
        /// </summary>
        public bool IsExtension { get; private set; }

        /// <summary>
        /// ジェネリック型引数
        /// </summary>
        public List<string> GenericTypeParams { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="startTag"></param>
        /// <param name="doc"></param>
        /// <param name="methods"></param>
        /// <param name="option"></param>
        public ClassDecl(IEnumerable<char> startTag, string docText, IEnumerable<string> args, string bodyText)
        {
            var argList = new List<string>(args);

            Document = Parser2.ApiDocument.DoParse(docText);
            OriginalName = argList[0].Trim();
            BaseClassOriginalName = (argList.Count >= 2) ? argList[1].Trim() : "";
            FuncDecls = new List<FuncDecl>(Parser2.ApiClass.DoParseClassBody(bodyText));

            // クラス種別チェック
            string t = new string(startTag.ToArray());
            if (t.IndexOf("LN_STATIC_CLASS") >= 0)
                IsStatic = true;
            if (t.IndexOf("LN_GENERIC_CLASS") >= 0)
                IsGeneric = true;
            if (t.IndexOf("LN_STRUCT_CLASS") >= 0)
                IsStruct = true;
            if (t.IndexOf("LN_EXTENSION_CLASS") >= 0)
                IsExtension = true;

            // ジェネリックなら型引数名を取り出す
            if (IsGeneric)
            {
                GenericTypeParams = new List<string>();
                for (int i = 1; i < argList.Count; ++i)
                {
                    GenericTypeParams.Add(argList[i].Trim());
                }

                BaseClassOriginalName = "";
            }
        }

        #endregion
    }
}
