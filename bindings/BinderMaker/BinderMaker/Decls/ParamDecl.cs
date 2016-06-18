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
        public string TypeName { get; private set; }

        /// <summary>
        /// 仮引数名
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// デフォルト値
        /// </summary>
        public string DefaultValue { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public ParamDecl(string attr, string typeName, string varName, string defaultValue)
        {
            AttributeText = attr;
            TypeName = typeName.Trim();
            Name = varName.Trim();
            DefaultValue = defaultValue.Trim();
        }

        #endregion
    }
}
