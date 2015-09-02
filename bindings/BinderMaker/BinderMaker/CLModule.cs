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
        public CLModule(string name, string docText, string bodyText)
        {
            Name = name;
            Document = Parser.CLAPIDocument.DocumentComment.Parse(docText);
            Classes = new List<CLClass>(Parser.CLAPIModule.ModuleBody.Parse(bodyText));
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、階層的に Manager の管理リストに登録する
        /// </summary>
        public override void Register()
        {
            base.Register();
            Document.Register();
            Classes.ForEach((c) => c.Register());   // 子クラスすべて登録
        }

        #endregion

        #region Fields

        #endregion
    }
}
