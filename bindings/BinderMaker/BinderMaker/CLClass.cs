using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker
{
    class CLClass : CLType
    {
        #region Constants

        /// <summary>
        /// Array 型クラスの定義
        /// ※これらは  LinkTypes() でも参照オブジェクトではないとする条件で使っているので、追加するときはそこも修正する。
        /// </summary>
        public static CLClass Array;
        public static CLClass ByteArray;
        //public static CLClass IntArray;

        #endregion

        #region Properties

        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// オリジナルのクラス名 (LN プレフィックス付き)
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// クラス名
        /// </summary>
        public string Name { get; set; }

        /// <summary>
        /// メンバメソッドリスト
        /// </summary>
        public List<CLMethod> Methods { get; private set; }

        /// <summary>
        /// プロパティリスト
        /// </summary>
        public List<CLProperty> Properties { get; private set; }

        /// <summary>
        /// オプション
        /// </summary>
        public CLOption Option { get; private set; }

        /// <summary>
        /// ベースクラス
        /// </summary>
        public CLClass BaseClass { get; private set; }
        
        /// <summary>
        /// RefObj クラス であるか
        /// </summary>
        public bool IsReferenceObject { get; private set; }

        /// <summary>
        /// static クラス であるか
        /// </summary>
        public bool IsStatic { get; private set; }

        /// <summary>
        /// ジェネリッククラス であるか
        /// </summary>
        public bool IsGeneric { get; private set; }

        /// <summary>
        /// ジェネリッククラスの型引数 (null の場合もある。その場合、バインドされていない(インスタンス化されていない))
        /// </summary>
        public CLType BindingType { get; private set; }

        /// <summary>
        /// struct であるか
        /// </summary>
        public bool IsStruct { get; private set; }

        /// <summary>
        /// 構造体情報
        /// </summary>
        public CLStruct StructData { get; private set; }

        /// <summary>
        /// 拡張クラス であるか (各言語のコードで独自定義するもの。RefObject など)
        /// </summary>
        public bool IsExtension { get; private set; }

        /// <summary>
        /// 組み込みクラス型であるか (Array など)
        /// </summary>
        public bool IsPreDefined { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="startTag"></param>
        /// <param name="doc"></param>
        /// <param name="methods"></param>
        /// <param name="option"></param>
        public CLClass(IEnumerable<char> startTag, string docText, string originalName, string bodyText, string optionText /*CLDocument doc, string name, IEnumerable<CLMethod> methods, CLOption option*/)
        {
            Document = Parser.CLAPIDocument.DocumentComment.Parse(docText);
            OriginalName = originalName.Trim();
            Name = OriginalName.Substring(2);
            Methods = new List<CLMethod>(Parser.CLAPIClass.ClassBody.Parse(bodyText));
            Option = (string.IsNullOrEmpty(optionText)) ? new CLOption() : Parser.CLAPIOptions.OptionComment.Parse(optionText);

            Methods.ForEach((m) => m.OwnerClass = this);

            // クラス種別チェック
            string t = new string(startTag.ToArray());
            if (t.IndexOf("LN_STATIC_CLASS") >= 0)
                IsStatic = true;
            if (t.IndexOf("LN_GENERIC_CLASS") >= 0)
                IsGeneric = true;
            if (t.IndexOf("LN_STRUCT_CLASS") >= 0)
                IsStruct = true;
            if (t.IndexOf("LN_EXTENSION_CLASS") >= 0)
                IsExtension = true;

            // 登録
            Manager.AllClasses.Add(this);
        }

        /// <summary>
        /// 組み込みクラス型を定義するために使用する。例えば Array。
        /// </summary>
        /// <param name="name"></param>
        public CLClass(string name, bool generic)
        {
            OriginalName = name;
            IsGeneric = generic;
            IsPreDefined = true;
            Methods = new List<CLMethod>();
            Manager.AllClasses.Add(this);
        }

        /// <summary>
        /// コンストラクタ (型をバインドしたジェネリッククラス(インスタンス化))
        /// ※C# ではインスタンス化しているかにかかわらず Type クラスであらわされる。
        ///   インスタンス化していないものは GenericTypeArgments 配列の要素数が 0 である。
        /// </summary>
        /// <param name="name"></param>
        /// <param name="?"></param>
        public CLClass(CLClass ownerGenericClass, CLType bindingType)
        {
            OriginalName = ownerGenericClass.Name;
            BindingType = bindingType;
            IsGeneric = true;
            IsPreDefined = true;
            Methods = new List<CLMethod>();
            Manager.AllClasses.Add(this);
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、オリジナルの型名から CLType を検索して参照する
        /// </summary>
        public override void LinkTypes()
        {
            base.LinkTypes();

            // 構造体リンク
            StructData = Manager.AllStructDefs.Find((t) => t.OriginalName == OriginalName);
            if (StructData != null && !IsStruct) throw new InvalidOperationException("invalid struct type.");

            // RefObject 型チェック
            IsReferenceObject = false;
            if (!IsStatic && !IsStruct && !IsPreDefined)
            {
                IsReferenceObject = true;
            }

            if (!IsPreDefined)
            {
                // ベースクラス
                string baseClassName = Document.GetBaseClassOriginalName();
                if (IsReferenceObject && string.IsNullOrWhiteSpace(baseClassName))  // ベースクラスを省略している RefObject　型はデフォルト継承
                    BaseClass = Manager.ReferenceObjectClass;
                else
                    BaseClass = Manager.AllClasses.Find((c) => c.OriginalName == baseClassName);
            }
        }

        /// <summary>
        /// メソッドからプロパティを集計する
        /// </summary>
        public void CreateProperties()
        {
            Properties = new List<CLProperty>();
            foreach (var method in Methods)
            {
                if (CLProperty.CheckProperty(method))
                {
                    var prop = Properties.Find((p) => p.Name == CLProperty.GetPropertyName(method));
                    if (prop != null)
                        prop.Attach(method);
                    else
                        Properties.Add(new CLProperty(method));
                }
            }
        }

        #endregion

        #region Fields


        #endregion
    }
}
