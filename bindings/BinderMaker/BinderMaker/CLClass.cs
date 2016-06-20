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
        //public static CLClass ByteArray;
        //public static CLClass IntArray;

        #endregion

        #region Properties

        /// <summary>
        /// 属しているモジュール
        /// </summary>
        public CLModule OwnerModule { get; private set; }

        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// オリジナルのクラス名 (LN プレフィックス付き)
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// ベースクラスのオリジナルのクラス名 (LN プレフィックス付き)
        /// </summary>
        public string BaseClassOriginalName { get; private set; }

        /// <summary>
        /// クラス名
        /// </summary>
        public override string Name { get; set; }

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
        /// ジェネリックからインスタンス化したクラス であるか
        /// </summary>
        public bool IsGenericinstance { get; private set; }

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

        /// <summary>
        /// ジェネリック型引数の対応表
        /// </summary>
        public Dictionary<string, CLType> GenericTypeArgsMap { get; private set; }

        public Decls.ClassDecl ClassDecl { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="startTag"></param>
        /// <param name="doc"></param>
        /// <param name="methods"></param>
        /// <param name="option"></param>
        public CLClass(CLModule ownerModule, Decls.ClassDecl classDecl)
        {
            OwnerModule = ownerModule;
            ClassDecl = classDecl;
            Document = new CLDocument(classDecl.Document);
            OriginalName = classDecl.OriginalName;
            BaseClassOriginalName = classDecl.BaseClassOriginalName;
            Name = OriginalName.Substring(2);
            IsStatic = classDecl.IsStatic;
            IsStruct = classDecl.IsStruct;
            IsExtension = classDecl.IsExtension;

            Methods = new List<CLMethod>();
            foreach (var f in classDecl.FuncDecls)
            {
                Methods.Add(new CLMethod(this, f));
            }


            // 登録
            Manager.AllClasses.Add(this);
        }

        /// <summary>
        /// コンストラクタ (ジェネリックのインスタンス化)
        /// </summary>
        /// <param name="ownerModule"></param>
        /// <param name="classDecl"></param>
        public CLClass(CLModule ownerModule, Decls.ClassDecl classDecl, List<string> typeParams)
        {
            OwnerModule = ownerModule;
            ClassDecl = classDecl;
            Document = new CLDocument(classDecl.Document);

            BaseClassOriginalName = classDecl.BaseClassOriginalName;
            IsStatic = classDecl.IsStatic;
            IsStruct = classDecl.IsStruct;
            IsExtension = classDecl.IsExtension;

            OriginalName = MakeGenericInstanceName(classDecl.OriginalName, typeParams);//"LN" + typeParams[0].Substring(2) + classDecl.OriginalName.Substring(2); // TODO: とりあえず [0] で名前を作るだけ
            Name = OriginalName.Substring(2);

            // ジェネリック型引数情報を作る
            IsGenericinstance = true;
            GenericTypeArgsMap = new Dictionary<string, CLType>();
            for (int i = 0; i < classDecl.GenericTypeParams.Count; i++)
            {
                GenericTypeArgsMap.Add(
                    classDecl.GenericTypeParams[i],
                    CLManager.Instance.FindType(typeParams[i]));
            }

            // メソッドをインスタンス化
            Methods = new List<CLMethod>();
            foreach (var f in classDecl.FuncDecls)
            {
                Methods.Add(new CLMethod(this, f, GenericTypeArgsMap));
            }

            // 登録
            Manager.AllClasses.Add(this);
        }

        /// <summary>
        /// 組み込みクラス型を定義するために使用する。例えば Array。
        /// </summary>
        /// <param name="name"></param>
        public CLClass(string name)
        {
            OriginalName = name;
            //IsGeneric = generic;
            IsPreDefined = true;
            Methods = new List<CLMethod>();
            Manager.AllClasses.Add(this);
        }

        public static string MakeGenericInstanceName(string originalClassName, List<string> originalTypeName)
        {
            return "LN" + originalTypeName[0].Substring(2) + originalClassName.Substring(2); // TODO: とりあえず [0] で名前を作るだけ
        }

        /// <summary>
        /// コンストラクタ (型をバインドしたジェネリッククラス(インスタンス化))
        /// ※C# ではインスタンス化しているかにかかわらず Type クラスであらわされる。
        ///   インスタンス化していないものは GenericTypeArgments 配列の要素数が 0 である。
        /// </summary>
        /// <param name="name"></param>
        /// <param name="?"></param>
        //public CLClass(CLClass ownerGenericClass, CLType bindingType)
        //{
        //    OriginalName = ownerGenericClass.Name;
        //    BindingType = bindingType;
        //    IsGeneric = true;
        //    IsPreDefined = true;
        //    Methods = new List<CLMethod>();
        //    Manager.AllClasses.Add(this);
        //}

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
                string baseClassName = BaseClassOriginalName;//Document.GetBaseClassOriginalName();
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
                    var prop = Properties.Find((p) => p.KeyName == CLProperty.GetPropertyKeyName(method));
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
