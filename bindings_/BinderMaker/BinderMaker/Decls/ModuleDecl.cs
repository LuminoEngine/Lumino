using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Decls
{
    class ModuleDecl
    {
        #region Properties

        /// <summary>
        /// ドモジュール名
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// ドキュメント
        /// </summary>
        public DocumentDecl Document { get; private set; }

        /// <summary>
        /// クラスリスト
        /// </summary>
        public List<ClassDecl> Classes { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="doc"></param>
        /// <param name="bodyText"></param>
        public ModuleDecl(string name, string docText, string bodyText)
        {
            Name = name;
            Document = Parser2.ApiDocument.DoParse(docText);
            Classes = new List<ClassDecl>(Parser2.ApiModule.DoParseModuleBody(bodyText));
        }

        #endregion
    }
}
