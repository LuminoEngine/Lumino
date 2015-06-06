using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// クラス・メソッドに付くオプション
    /// </summary>
    class CLOption : CLEntity
    {
        #region Properties
        /// <summary>
        /// DisableOption リスト
        /// </summary>
        public List<CLDisableOption> DisableOptions { get; private set; }

        /// <summary>
        /// OverrideOption リスト
        /// </summary>
        public List<CLOverrideOption> OverrideOptions { get; private set; }

        /// <summary>
        /// ClassAddCodeOption リスト
        /// </summary>
        public List<CLClassAddCodeOption> ClassAddCodeOptions { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ (空のオプション)
        /// </summary>
        /// <param name="options"></param>
        public CLOption()
        {
            DisableOptions = new List<CLDisableOption>();
            OverrideOptions = new List<CLOverrideOption>();
            ClassAddCodeOptions = new List<CLClassAddCodeOption>();
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="options"></param>
        public CLOption(IEnumerable<CLEntity> options)
            : this()    // コンストラクタ (空のオプション) 呼び出し
        {
            // 振り分け
            foreach (var opt in options)
            {
                if (opt is CLDisableOption)
                    DisableOptions.Add((CLDisableOption)opt);
                else if (opt is CLOverrideOption)
                    OverrideOptions.Add((CLOverrideOption)opt);
                else if (opt is CLClassAddCodeOption)
                    ClassAddCodeOptions.Add((CLClassAddCodeOption)opt);
            }
        }
        #endregion
    }

    /// <summary>
    /// 言語別メソッド無効フラグ
    /// </summary>
    class CLDisableOption : CLEntity
    {
        #region Properties
        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        public CLDisableOption(string langs)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
        }
        #endregion
    }

    /// <summary>
    /// 言語別メソッドオーバーライド
    /// </summary>
    class CLOverrideOption : CLEntity
    {
        #region Properties
        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }

        /// <summary>
        /// コード文字列
        /// </summary>
        public string Code { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="code"></param>
        public CLOverrideOption(string langs, string code)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
            Code = code.Trim();
        }
        #endregion
    }

    /// <summary>
    /// 言語別クラス追加コード
    /// </summary>
    class CLClassAddCodeOption : CLEntity
    {
        #region Properties
        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }

        /// <summary>
        /// コード文字列
        /// </summary>
        public string Code { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="code"></param>
        public CLClassAddCodeOption(string langs, string code)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
            Code = code.Trim();
        }
        #endregion
    }
}
