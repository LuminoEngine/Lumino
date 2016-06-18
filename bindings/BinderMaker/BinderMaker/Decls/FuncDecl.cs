using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Decls
{
    class FuncDecl
    {
        #region Properties

        /// <summary>
        /// ドキュメント
        /// </summary>
        public DocumentDecl Document { get; private set; }

        /// <summary>
        /// 修飾子
        /// </summary>
        public MethodModifier Modifier { get; private set; }

        /// <summary>
        /// 属性
        /// </summary>
        public MethodAttribute Attribute { get; private set; }

        /// <summary>
        /// 戻り値型
        /// </summary>
        public string ReturnTypeName { get; private set; }

        /// <summary>
        /// オリジナルの完全関数名 (LNAudio_PlayBGM 等)
        /// </summary>
        public string OriginalFullName { get; private set; }

        /// <summary>
        /// オーバーロード元
        /// </summary>
        public string OverloadSourceName { get; private set; }
        
        /// <summary>
        /// 仮引数リスト
        /// </summary>
        public List<ParamDecl> Params { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public FuncDecl(
            string docText,
            IEnumerable<char> apiModifier,
            IEnumerable<string> apiAtribute,
            string returnType,
            string name,
            IEnumerable<ParamDecl> params1)
        {
            // ドキュメントコメント
            if (!string.IsNullOrEmpty(docText))
                Document = Parser2.ApiDocument.DoParse(docText);

            // 修飾子の決定
            string modifier = new string(apiModifier.ToArray());
            if (modifier.Contains(CLManager.APIModifier_Instance))
                Modifier = MethodModifier.Instance;
            else if (modifier.Contains(CLManager.APIModifier_Static))
                Modifier = MethodModifier.Static;
            else
                Modifier = MethodModifier.Internal;

            // 属性の決定
            foreach (string attr in apiAtribute)
            {
                //string attr = new string(apiAtribute.ToArray());
                Attribute = MethodAttribute.None;
                if (attr.Contains(CLManager.APIAttribute_Property))
                    Attribute = MethodAttribute.Property;
                if (attr.Contains(CLManager.APIAttribute_Constructor))
                    Attribute = MethodAttribute.Constructor;
                if (attr.Contains(CLManager.APIAttribute_LibraryInitializer))
                    Attribute = MethodAttribute.LibraryInitializer;
                if (attr.Contains(CLManager.APIAttribute_LibraryTerminator))
                    Attribute = MethodAttribute.LibraryTerminator;

                // オーバーロード
                if (attr.Contains(CLManager.APIAttribute_Overload))
                {
                    OverloadSourceName = attr.Substring(CLManager.APIAttribute_Overload.Length).Replace("(", "").Replace(")", "").Trim();
                }
            }

            // 戻り値型名
            ReturnTypeName = returnType;

            // 関数名
            OriginalFullName = name;

            // 仮引数
            Params = new List<ParamDecl>(params1);  // コピー
        }

        #endregion
    }
}
