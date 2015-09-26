using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * yard 0.8.7.4 は、module をネストすると解析できない不具合がある。
 * そのため、全てのクラスに LN:: を付けて module を表現している。
 */

namespace BinderMaker.Builder
{
    class RubyDocBuilder : Builder
    {
        class OverloadMethodInfo
        {
            public CLMethod Method;
            public CLParam ReturnParam;
        }

        class CurrentClassInfo
        {
            /// <summary>
            /// オーバーロードメソッドの集計 (コンストラクタは Constructor という名前で登録する)
            /// </summary>
            public Dictionary<string, List<OverloadMethodInfo>> OverloadTable = new Dictionary<string, List<OverloadMethodInfo>>();
        }

        private CurrentClassInfo _currentClassInfo;
        private OutputBuffer _output = new OutputBuffer();

        public RubyDocBuilder()
        {
            //_output.AppendWithIndent("module LN").NewLine();
            //_output.IncreaseIndent();
        }

        /// <summary>
        /// ドキュメントの置換要素を作る
        /// </summary>
        /// <param name="entity"></param>
        //public override string MakeCommentConvertItem(CLEntity entity)
        //{
        //    return null;
        //}

        /// <summary>
        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnEnumLooked(CLEnum enumType)
        {
            // 出力しないもの
            if (enumType.Name == "Bool")
                return;

            _output.AppendWithIndent("# " + enumType.Name.ToUpper()).NewLine();
            _output.AppendWithIndent("module LN::" + enumType.Name/*.ToUpper()*/).NewLine();
            _output.IncreaseIndent();
            foreach (var member in enumType.Members)
            {
                if (member.IsTerminator) continue;  // ターミネータは出力しない

                _output.AppendWithIndent("# " + member.Comment).NewLine();

                string name = member.CommonUpperSnakeName;

                // BEGIN と END はキーワードなので別名にする
                if (enumType.Name == "SeekOrigin")
                    name = "SEEK_" + name;
                else if (enumType.Name == "KeyCode")
                    name = "KEY_" + name;

                _output.AppendWithIndent("{0} = {1}", name, member.Value).NewLine();

            }
            _output.DecreaseIndent();
            _output.AppendWithIndent("end").NewLine();
        }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            _currentClassInfo = new CurrentClassInfo();
            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            //-------------------------------------------------
            // クラス定義の開始・スーパークラス
            _output.AppendWithIndent("# " + classType.Document.OriginalBriefText).NewLine();
            _output.AppendWithIndent("class LN::" + classType.Name);
            if (classType.BaseClass != null)
                _output.Append(" < " + classType.BaseClass.Name);
            _output.NewLine();
            _output.IncreaseIndent();

            //-------------------------------------------------
            // initialize 関数
            if (classType.IsStruct)
            {
                #region 構造体の場合はコンストラクタ・アクセサを定義する

                // def initialize
                var argsText = new OutputBuffer();
                var paramsText = new OutputBuffer();
                foreach (var memberDecl in classType.StructData.Members)
                {
                    // TODO:型名
                    paramsText.AppendLine("#   @param [{0}] {1} {2}", memberDecl.Type.Name, memberDecl.Name.ToLower(), memberDecl.Comment);
                    argsText.AppendCommad(memberDecl.Name.ToLower());
                }
                _output.AppendWithIndent("# 初期化").NewLine();
                _output.AppendWithIndent("# @overload initialize({0})", argsText.ToString()).NewLine();
                _output.AppendWithIndent(paramsText.ToString());
                _output.AppendWithIndent("def initialize").NewLine();
                _output.AppendWithIndent("end").NewLine(2);

                // プロパティアクセサ
                foreach (var memberDecl in classType.StructData.Members)
                {
                    string name = memberDecl.Name.ToLower();
                    // setter
                    _output.AppendWithIndent("# " + memberDecl.Comment).NewLine();
                    _output.AppendWithIndent("# @overload {0}=(v)", name).NewLine();
                    _output.AppendWithIndent("#   @param [{0}] v ", memberDecl.Type.Name).NewLine();
                    _output.AppendWithIndent("def {0}=(v)", name).NewLine();
                    _output.AppendWithIndent("end").NewLine(2);
                    // getter
                    _output.AppendWithIndent("# " + memberDecl.Comment).NewLine();
                    _output.AppendWithIndent("# @overload {0}", name).NewLine();
                    _output.AppendWithIndent("#   @return [{0}]", memberDecl.Type.Name).NewLine();
                    _output.AppendWithIndent("def {0}", name).NewLine();
                    _output.AppendWithIndent("end").NewLine(2);
                }
                #endregion
            }

            //-------------------------------------------------
            // メソッド出力
            foreach (var overloads in _currentClassInfo.OverloadTable)
            {
                MakeFuncDecl(overloads.Value);
            }

            _output.DecreaseIndent();
            _output.AppendWithIndent("end").NewLine();


        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop)
        {
            if (prop.Getter != null) OnMethodLooked(prop.Getter);
            if (prop.Setter != null) OnMethodLooked(prop.Setter);
        }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            // Ruby として出力できるメソッドであるか
            if (!RubyCommon.CheckInvalidMethod(method))
                return;

            // オーバーロードの集計 (RubyBuilder と同じ)
            {
                List<OverloadMethodInfo> overloads;
                string key = (method.IsRefObjectConstructor) ? "Constructor" : method.Name;
                if (_currentClassInfo.OverloadTable.TryGetValue(key, out overloads))
                {
                    var info = new OverloadMethodInfo();
                    info.Method = method;
                    info.ReturnParam = method.ReturnParam;
                    _currentClassInfo.OverloadTable[key].Add(info);
                }
                else
                {
                    // 新しく登録
                    var info = new OverloadMethodInfo();
                    info.Method = method;
                    info.ReturnParam = method.ReturnParam;
                    _currentClassInfo.OverloadTable[key] = new List<OverloadMethodInfo>() { info };
                }
            }
        }


        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string t = GetTemplate("RubyDoc.txt");
            t = t.Replace("[OUTPUT]", _output.ToString());
            return t;
        }

        /// <summary>
        /// オーバーロードを考慮しつつ、関数定義を作成する
        /// </summary>
        private void MakeFuncDecl(List<OverloadMethodInfo> overloadMethods)
        {
            var baseMethod = overloadMethods[0].Method;

            // メソッド名
            string rubyMethodName = RubyCommon.ConvertCommonNameToRubyMethodName(baseMethod);

            // コンストラクタの場合は名前を強制的に initialize にする
            if (baseMethod.IsRefObjectConstructor)
                rubyMethodName = "initialize";

            // キャプション。オーバーロード全てをリスト要素で並べる
            _output.AppendWithIndent("# " + baseMethod.Document.OriginalBriefText).NewLine();
            //foreach (var info in overloadMethods)
            //{
            //    _output.AppendWithIndent("# - " + info.Method.Summary).NewLine();
            //}

            foreach (var info in overloadMethods)
            {
                var paramsText = new OutputBuffer();
                var argsText = new OutputBuffer();
                string returnText = "";
                foreach (var param in info.Method.Params)
                {
                    // 第1引数かつインスタンスメソッドの場合はドキュメント化する必要はない
                    if (param == info.Method.Params.First() &&
                        info.Method.IsInstanceMethod)
                    {
                    }
                    else if (param == info.ReturnParam)
                    {
                        returnText = param.Document.OriginalText;
                    }
                    else
                    {
                        argsText.AppendCommad(param.Name);
                        if (!string.IsNullOrEmpty(param.OriginalDefaultValue))
                            argsText.Append("=" + ConvertDefaultArgVaue(param.OriginalDefaultValue));
                        paramsText.AppendWithIndent("#   @param [{0}] {1} {2}", ConvertTypeCToRuby(param.Type), param.Name, param.Document.OriginalText).NewLine();
                    }
                }

                //---------------------------------------------
                // 結合

                _output.AppendWithIndent("# @overload {0}({1})", rubyMethodName, argsText.ToString()).NewLine();

                // overrload がある場合は @overload 下にも概要を書いておく
                // (- を付けてリスト要素化することで details と区別する)
                if (overloadMethods.Count > 1)
                    _output.AppendWithIndent("#   " + info.Method.Document.OriginalBriefText).NewLine();

                // @note
                if (!string.IsNullOrEmpty(info.Method.Document.OriginalDetailsText))
                {
                    if (overloadMethods.Count == 1)
                        _output.AppendWithIndent("#   　").NewLine();    // ダミーキャプションとして全角スペースを入れる
                    //_output.AppendWithIndent("#   @note ").NewLine();
                    //_output.AppendWithIndent("#   ").NewLine();    // @note は必要なし。また、インデントも @override のすぐ下にしておく
                    // 概要とはインデントをひとつ下げると、別のセクションで表示できる。
                    string[] lines = info.Method.Document.OriginalDetailsText.Split('\n');
                    foreach (var line in lines)
                        _output.AppendWithIndent("#     " + line).NewLine();
                }

                // @param
                _output.AppendWithIndent(paramsText.ToString());

                // @return
                if (!string.IsNullOrEmpty(returnText))
                    _output.AppendWithIndent("#   @return [{0}] {1}", ConvertTypeCToRuby(info.ReturnParam.Type), returnText).NewLine();


            }

            _output.AppendWithIndent("def " + rubyMethodName);
            _output.NewLine();
            _output.AppendWithIndent("end").NewLine();
        }

        /// <summary>
        /// デフォルト引数値の変換
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        private string ConvertDefaultArgVaue(string value)
        {
            // double に変換できれば、数値リテラル
            double tmp;
            if (double.TryParse(value, out tmp))
                return value;

            if (value == "LN_TRUE")
                return "true";
            if (value == "LN_FALSE")
                return "false";
            if (value == "NULL")
                return "nil";

            // enum メンバかもしれない
            foreach (var enumType in CLManager.Instance.AllEnums)
            {
                foreach (var m in enumType.Members)
                {
                    if (m.OriginalName == value)
                        return m.CommonUpperSnakeName;//value.Substring(3);  // 先頭の LN_ を外す
                }
            }
            throw new InvalidOperationException("Not Found : " + value);
        }

        /// <summary>
        /// 型名変換
        /// </summary>
        /// <returns></returns>
        private string ConvertTypeCToRuby(CLType type)
        {
            // プリミティブ型
            RubyCommon.RubyTypeInfo info;
            if (RubyCommon._rubyTypeTable.TryGetValue(type, out info))
                return info.RubyType;

            // enum 型                    
            // ※ LN:: を付けて完全修飾名にすることで、ドキュメントにリンクが張られる
            var enumType = type as CLEnum;
            if (enumType != null)
                return "LN::" + enumType.Name; // 大文字化

            // class 型
            var classType = type as CLClass;
            if (classType != null)
                return "LN::" + classType.Name;

            throw new InvalidOperationException();
        }
    }
}
