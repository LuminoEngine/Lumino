using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Decls
{
    /// <summary>
    /// 仮引数
    /// </summary>
    class ParamDecl
    {
        #region Properties

        /// <summary>
        /// 属性
        /// </summary>
        public string AttributeText { get; private set; }

        /// <summary>
        /// 型名 ("LN_HANDLE()" などの修飾文字列を含む)
        /// </summary>
        public string OriginalTypeName { get; private set; }

        /// <summary>
        /// 仮引数名
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// デフォルト値
        /// </summary>
        public string DefaultValue { get; private set; }

        /// <summary>
        /// 型がジェネリックか
        /// </summary>
        public bool IsGenericInstance { get { return GenericTypeArgs != null; } }
        
        /// <summary>
        /// ジェネリッククス名 (LNList など)
        /// </summary>
        public string GenericClassName { get; private set; }

        /// <summary>
        /// ジェネリック型引数
        /// </summary>
        public List<string> GenericTypeArgs { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public ParamDecl(string attr, string typeName, string varName, string defaultValue)
        {
            AttributeText = attr;
            OriginalTypeName = typeName.Trim();
            Name = varName.Trim();
            DefaultValue = defaultValue.Trim();

            // ジェネリックなら型引数名を取り出す
            if (OriginalTypeName.Contains("LN_GENERIC_HANDLE"))
            {
                string typeParamsText = OriginalTypeName.Replace("LN_GENERIC_HANDLE", "").Replace("(", "").Replace(")", "").Replace("*", "");
                var tokens = typeParamsText.Split(',');
                GenericTypeArgs = new List<string>();
                for (int i = 1; i < tokens.Length; ++i)
                {
                    GenericTypeArgs.Add(tokens[i].Trim());
                }
                GenericClassName = tokens[0];
            }
        }

        #endregion
    }
}
