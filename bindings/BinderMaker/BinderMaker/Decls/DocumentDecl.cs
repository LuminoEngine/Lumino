using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Decls
{
    /// <summary>
    /// モジュール、クラス、メソッドのドキュメント
    /// </summary>
    class DocumentDecl
    {
        #region Properties

        /// <summary>
        /// オリジナルのグループ名
        /// (このテキストはそのまま出力せず、LangContext を通すこと)
        /// </summary>
        public string OriginalGroup { get; private set; }

        /// <summary>
        /// オリジナルの概要テキスト
        /// (このテキストはそのまま出力せず、LangContext を通すこと)
        /// </summary>
        public string OriginalBriefText { get; private set; }

        /// <summary>
        /// オリジナルの仮引数リスト
        /// (このテキストはそのまま出力せず、LangContext を通すこと)
        /// </summary>
        public List<ParamDocumentDecl> OriginalParams { get; private set; }

        /// <summary>
        /// オリジナルの戻り値テキスト
        /// (このテキストはそのまま出力せず、LangContext を通すこと)
        /// </summary>
        public string OriginalReturnText { get; private set; }

        /// <summary>
        /// オリジナルの詳細テキスト
        /// (このテキストはそのまま出力せず、LangContext を通すこと)
        /// </summary>
        public string OriginalDetailsText { get; private set; }

        #endregion

        #region Methods

        public DocumentDecl(
            string group,
            string briefText,
            IEnumerable<ParamDocumentDecl> paramDocs,
            string returnText,
            string detailsText)
        {
            OriginalGroup = group.Trim();
            OriginalBriefText = briefText.Trim();   // 渡されるテキストは前後に空白が入っていたりするので Trim() とかしておく
            OriginalParams = new List<ParamDocumentDecl>(paramDocs);
            OriginalReturnText = returnText.Trim();
            OriginalDetailsText = detailsText.Trim();
        }

        #endregion
    }

    /// <summary>
    /// 仮引数のドキュメント
    /// </summary>
    class ParamDocumentDecl
    {
        #region Properties

        /// <summary>
        /// 入出力
        /// </summary>
        public IOModifier IOModifier { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// テキスト
        /// </summary>
        public string OriginalText { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="io"></param>
        /// <param name="name"></param>
        /// <param name="text"></param>
        public ParamDocumentDecl(string io, string name, string text)
        {
            // io は "[]" が含まれている。IndexOf で探す
            IOModifier = 0;
            IOModifier |= (io.IndexOf("in") >= 0) ? IOModifier.In : 0;
            IOModifier |= (io.IndexOf("out") >= 0) ? IOModifier.Out : 0;
            if (IOModifier == 0)
                throw new InvalidOperationException("io empty");  // IO が空等

            Name = name.Trim();
            OriginalText = text.Trim();
        }

        #endregion
    }
}
