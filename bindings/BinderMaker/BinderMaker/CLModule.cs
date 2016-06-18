using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker
{
    class CLModule : CLEntity
    {
        #region Properties

        /// <summary>
        /// ドモジュール名
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// クラスリスト
        /// </summary>
        public List<CLClass> Classes { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="doc"></param>
        /// <param name="bodyText"></param>
        public CLModule(Decls.ModuleDecl moduleDecl)
        {
            Name = moduleDecl.Name;
            Document = new CLDocument(moduleDecl.Document);
            Classes = new List<CLClass>();
            foreach (var c in moduleDecl.Classes)
            {
                Classes.Add(new CLClass(this, c));
            }
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="doc"></param>
        /// <param name="bodyText"></param>
        //public CLModule(string name, string docText, string bodyText)
        //{
        //    Name = name;
        //    Document = Parser.CLAPIDocument.DocumentComment.Parse(docText);
        //    Classes = new List<CLClass>(Parser.CLAPIModule.ModuleBody.Parse(bodyText));
        //    foreach (var c in Classes)
        //    {
        //        c.OwnerModule = this;
        //    }
        //}

        #endregion

        #region Fields

        #endregion
    }
}
