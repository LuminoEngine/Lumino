using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// ドキュメントコメント等を各言語用に変換する
    /// </summary>
    class LangContext
    {
        #region Constants
        public const string BriefTag = "brief";
        public const string ParamTag = "param";
        public const string DetailsTag = "details";
        #endregion

        #region Fields
        struct ReplacePair
        {
            public string OldText;
            public string NewText;
        }

        private LangFlags _langFlags;
        private List<ReplacePair> _textReplacePairs;
        #endregion

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langFlags"></param>
        public LangContext(LangFlags langFlags)
        {
            _langFlags = langFlags;
            _textReplacePairs = new List<ReplacePair>();
        }

        /// <summary>
        /// すべてのテキストに対する置換要素の追加
        /// </summary>
        public void AddReplacePair(string oldText, string newText)
        {
            _textReplacePairs.Add(new ReplacePair() { OldText = oldText, NewText = newText });
        }

        /// <summary>
        /// クラスの brief 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetBriefText(CLClass classType)
        {
            return MakeText(classType.Document.OriginalBriefText, classType.Document, BriefTag);
        }

        /// <summary>
        /// メソッドの brief 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetBriefText(CLMethod method)
        {
            return MakeText(method.Document.OriginalBriefText, method.Document, BriefTag);
        }

        /// <summary>
        /// プロパティの brief 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetBriefText(CLProperty prop)
        {
            string getter = (prop.Getter != null) ? MakeText(prop.Getter.Document.OriginalBriefText, prop.Getter.Document, BriefTag) : "";
            string setter = (prop.Setter != null) ? MakeText(prop.Setter.Document.OriginalBriefText, prop.Setter.Document, BriefTag) : "";

            getter = getter.Replace("の取得", "");
            getter = getter.Replace("を取得します。", "");
            setter = setter.Replace("の設定", "");
            setter = setter.Replace("を設定します。", "");
            getter = getter.Replace("の確認", "");
            getter = getter.Replace("を確認します。", "");

            // もし両方ある場合はコメント内容が一致しているはず
            //if (!string.IsNullOrEmpty(getter) && !string.IsNullOrEmpty(setter) && getter != setter)
            //    throw new InvalidOperationException();
            // ここで止まる場合、「を設定する」とかになってないかチェック

            if (!string.IsNullOrEmpty(getter)) return getter;
            if (!string.IsNullOrEmpty(setter)) return setter;
            throw new InvalidOperationException();
        }

        /// <summary>
        /// 仮引数の説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetParamText(CLParam param)
        {
            return MakeText(param.Document.OriginalText, param.OwnerFunc.OwnerMethod.Document, ParamTag);
        }

        /// <summary>
        /// クラスの details 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetDetailsText(CLClass classType)
        {
            return MakeText(classType.Document.OriginalDetailsText, classType.Document, DetailsTag);
        }

        /// <summary>
        /// メソッドの details 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetDetailsText(CLMethod method)
        {
            return MakeText(method.Document.OriginalDetailsText, method.Document, DetailsTag);
        }

        /// <summary>
        /// プロパティの details 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetDetailsText(CLProperty prop)
        {
            string getter = (prop.Getter != null) ? MakeText(prop.Getter.Document.OriginalDetailsText, prop.Getter.Document, DetailsTag) : "";
            string setter = (prop.Setter != null) ? MakeText(prop.Setter.Document.OriginalDetailsText, prop.Setter.Document, DetailsTag) : "";

            // 両方あれば間を改行で区切って出力
            if (!string.IsNullOrEmpty(getter) && !string.IsNullOrEmpty(getter))
                return getter + Builder.OutputBuffer.NewLineCode + Builder.OutputBuffer.NewLineCode + setter;

            if (!string.IsNullOrEmpty(getter)) return getter;
            if (!string.IsNullOrEmpty(setter)) return setter;
            return "";
        }

        /// <summary>
        /// return Param コメント作成
        /// (return として選択された Param のコメント作成で使用する)
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="cppReturn"></param>
        public string GetReturnParamText(CLParam param)
        {
            if (param == null) return "";

            string text = GetParamText(param);
            // "演算結果を格納する Vector3 変数" 等の後半を切り取る
            var r = new Regex("を格納する.*");
            return r.Replace(text, "");
        }

        /// <summary>
        /// メソッドが有効であるかを確認
        /// </summary>
        /// <returns></returns>
        public bool CheckEnabled(CLMethod method)
        {
            var b = method.Option.DisableOptions.Find((opt) => opt.LangFlags == _langFlags);
            return b == null;
        }

        /// <summary>
        /// メソッドのオーバーライドコード
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public bool TryGetOverrideCode(CLMethod method, out string code)
        {
            var d = method.Option.OverrideOptions.Find((opt) => opt.LangFlags == _langFlags);
            if (d != null)
            {
                code = d.Code;
                return true;
            }
            code = "";
            return false;
        }


        private string MakeText(string srcText, CLDocument doc, string targetSection)
        {
            var replaceDocs = doc.ReplaceDocs.FindAll((d) => d.LangFlags == _langFlags);
            var postscriptDocs = doc.PostscriptDocs.FindAll((d) => (d.LangFlags == _langFlags && d.TargetSection == targetSection));
            var overwriteDocs = doc.OverwriteDocs.FindAll((d) => (d.LangFlags == _langFlags && d.TargetSection == targetSection));

            foreach (var replaceDoc in replaceDocs)
            {
                srcText.Replace(replaceDoc.From, replaceDoc.To);
            }
            foreach (var postscriptDoc in postscriptDocs)
            {
                srcText += Builder.OutputBuffer.NewLineCode + Builder.OutputBuffer.NewLineCode;
                srcText += postscriptDoc.Text;
            }
            foreach (var overwriteDoc in overwriteDocs)
            {
                srcText = overwriteDoc.Text;
            }

            // 置換 (TODO:重くなるようならテキストに "LN" が含まれているかをまずチェックするとか)
            foreach (var r in _textReplacePairs)
            {
                srcText = srcText.Replace(r.OldText, r.NewText);
            }

            return srcText;
        }

    }
}
