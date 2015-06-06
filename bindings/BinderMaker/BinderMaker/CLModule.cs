using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLModule : CLEntity
    {
        #region Properties

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
        /// <param name="classes"></param>
        public CLModule(CLDocument doc, IEnumerable<CLClass> classes)
        {
            Document = doc;
            Classes = new List<CLClass>(classes);
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
    }
}
