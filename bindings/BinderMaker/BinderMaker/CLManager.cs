using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// 中間コード管理クラス
    /// </summary>
    class CLManager
    {
        //①型の集計
        //②型の割り当て
        //③オーバーロードメソッドの集計
        //④プロパティの集計
        //⑤出力言語の指定

        #region Constants
        public const string APIModifier_Instance = "LN_INSTANCE_API";
        public const string APIModifier_Static = "LN_STATIC_API";
        public const string APIModifier_Internal = "LN_INTERNAL_API";

        public const string APIAttribute_LibraryInitializer = "LN_ATTR_LIBRARY_INITIALIZER";
        public const string APIAttribute_LibraryTerminator = "LN_ATTR_LIBRARY_TERMINATOR";
        public const string APIAttribute_Constructor = "LN_ATTR_CONSTRUCTOR";
        public const string APIAttribute_Property = "LN_ATTR_PROPERTY";
        public const string APIAttribute_Event = "LN_ATTR_EVENT";
        public const string APIAttribute_Overload = "LN_ATTR_OVERLOAD";

        public const string HandleTypeParamMacro = "LN_HANDLE";
        public const string GenericHandleTypeParamMacro = "LN_GENERIC_HANDLE";

        public const string EnumTerminatorName = "TERMINATOR";

        /// <summary>
        /// オーバーロード関数のサフィックス
        /// </summary>
        //public static string[] OverloadSuffix = new string[] 
        //{
        //    "XYWH","XYZW", "WH", "XYZ", "XY",   // 名前の長い方から見る
        //    "Vec2", "Vec3", "Vec4",
        //};

        #endregion

        #region Fields

        /// <summary>
        /// 基本的なC言語の型と CLType との変換テーブル
        /// </summary>
        private static Dictionary<string, CLType> _typeInfoTable;

        #endregion

        #region Properties
        /// <summary>
        /// グローバルインスタンス
        /// </summary>
        public static CLManager Instance { get; set; }

        /// <summary>
        /// 全 Entity リスト
        /// </summary>
        public List<CLEntity> AllEntities { get; private set; }

        /// <summary>
        /// 全型リスト
        /// </summary>
        public List<CLType> AllTypes { get; private set; }

        /// <summary>
        /// 全クラスリスト
        /// </summary>
        public List<CLClass> AllClasses { get; private set; }

        /// <summary>
        /// 全 struct 型リスト
        /// </summary>
        public List<CLStruct> AllStructDefs { get; private set; }

        /// <summary>
        /// 全 enum 型リスト
        /// </summary>
        public List<CLEnum> AllEnums { get; private set; }

        /// <summary>
        /// 全 メソッドリスト
        /// </summary>
        public List<CLMethod> AllMethods { get; private set; }

        /// <summary>
        /// 全 delegate リスト
        /// </summary>
        public List<CLDelegate> AllDelegates { get; private set; }


        /// <summary>
        /// Result 列挙型
        /// </summary>
        public CLEnum ResultEnumType { get; private set; }

        /// <summary>
        /// ReferenceObject クラス型
        /// </summary>
        public CLClass ReferenceObjectClass { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLManager()
        {
            Instance = this;

            AllEntities = new List<CLEntity>();
            AllTypes = new List<CLType>();
            AllClasses = new List<CLClass>();
            AllStructDefs = new List<CLStruct>();
            AllEnums = new List<CLEnum>();
            AllMethods = new List<CLMethod>();
            AllDelegates = new List<CLDelegate>();

            CLPrimitiveType.Void = new CLPrimitiveType("Void");
            CLPrimitiveType.String = new CLPrimitiveType("String");
            CLPrimitiveType.Bool = new CLPrimitiveType("Bool");
            CLPrimitiveType.Byte = new CLPrimitiveType("Byte");
            CLPrimitiveType.Int32 = new CLPrimitiveType("Int");
            CLPrimitiveType.UInt32 = new CLPrimitiveType("UInt32");
            CLPrimitiveType.Int64 = new CLPrimitiveType("Int64");
            CLPrimitiveType.Float = new CLPrimitiveType("Float");
            CLPrimitiveType.Double = new CLPrimitiveType("Double");
            CLPrimitiveType.VoidPtr = new CLPrimitiveType("VoidPtr");
            CLPrimitiveType.IntPtr = new CLPrimitiveType("IntPtr");

            CLClass.Array = new CLClass("Array");

            _typeInfoTable = new Dictionary<string, CLType>()
            {
                { "void",               CLPrimitiveType.Void },
                { "const void*",        CLPrimitiveType.VoidPtr },
                //{ "void**",             CLClass.ByteArray },  //TODO: この2つは
                //{ "const void**",       CLClass.ByteArray },   //TODO: バッファクラスを用意する必要がありそう

                { "const LNChar*",      CLPrimitiveType.String },
                { "const LNChar**",     CLPrimitiveType.String },

                { "int",                CLPrimitiveType.Int32 },
                { "int*",               CLPrimitiveType.Int32 },
                { "float",              CLPrimitiveType.Float },
                { "float*",             CLPrimitiveType.Float },
                { "double",             CLPrimitiveType.Double },
                { "double*",            CLPrimitiveType.Double },
                { "LNBool",             CLPrimitiveType.Bool },
                { "LNBool*",　          CLPrimitiveType.Bool },
                { "uint8_t",            CLPrimitiveType.Byte },
                { "uint32_t",           CLPrimitiveType.UInt32 },
                { "int32_t",            CLPrimitiveType.Int32 },
                { "int32_t*",           CLPrimitiveType.Int32 },
                { "int64_t",            CLPrimitiveType.Int64 },
                { "int64_t*",           CLPrimitiveType.Int64 },
                { "intptr_t",           CLPrimitiveType.IntPtr },
                { "LNUserData",         CLPrimitiveType.IntPtr },
                //{ "void*",              CLPrimitiveType.VoidPtr },
            
                //{ "const int*",         CLClass.IntArray },
            }; 

        }

        public void Initialize()
        {
            
        }

        /// <summary>
        /// パース済みの AllEntities を解析し、型情報やプロパティの関係を作成する。
        /// </summary>
        /// <remarks>
        /// RegisterEntities() が終わった時点では、個々の型情報 (構造体メンバの型等) は、識別子(名前) としてしか認識していない。
        /// それぞれ CLType を参照するようにする。
        /// </remarks>
        public void LinkEntities()
        {
            // Result 型を探す
            ResultEnumType = AllEnums.Find((e) => e.Name == "Result");
            if (ResultEnumType == null) throw new InvalidOperationException("not found ResultEnumType.");

            // RefObject 型を探す
            ReferenceObjectClass = AllClasses.Find((e) => e.OriginalName == "LNObject");
            if (ReferenceObjectClass == null) throw new InvalidOperationException("not found RefObject.");
            ReferenceObjectClass.Name = "RefObject";  // 特殊な名前にする

            // CLType をリンクする
            foreach (var e in AllEntities)
                e.LinkTypes();
            // オーバーロードメソッドの関係をリンクする
            foreach (var e in AllMethods)
                e.LinkOverloads();
            // プロパティを作る
            foreach (var c in AllClasses)
                c.CreateProperties();
        }

        /// <summary>
        /// 構築された中間構造を検証する
        /// </summary>
        public void Validation()
        {
            foreach (var e in AllEntities)
            {

            }
        }

        /// <summary>
        /// オリジナルの型名から CLType を検索する。
        /// "LN_HANDLE(LNSound)" とかもOK。 
        /// </summary>
        /// <param name="name"></param>
        public CLType FindType(string name)
        {
            // クラス型を検索
            int idx = name.IndexOf(HandleTypeParamMacro);
            if (idx >= 0)
            {
                // LN_HANDLE と () を削除
                name = name
                    .Substring(idx + HandleTypeParamMacro.Length)
                    .Replace("(", "")
                    .Replace(")", "")
                    .Replace("*", "")
                    .Trim();
                foreach (var t in AllTypes)
                {
                    CLClass c = t as CLClass;
                    if (c != null && c.OriginalName == name)
                    {
                        return c;
                    }
                }
                throw new InvalidOperationException("invalid class type. LN_HANDLE() に指定された型が見つからない。");
            }
            else
            {
                foreach (var t in AllTypes)
                {
                    CLClass c = t as CLClass;
                    if (c != null && c.OriginalName == name)
                    {
                        return c;
                    }
                }
            }

            // 基本的な型テーブルから検索
            CLType type;
            if (_typeInfoTable.TryGetValue(name, out type))
            {
                return type;
            }

            // 列挙型
            foreach (var t in AllEnums)
            {
                if (name.IndexOf(t.Name) >= 0)
                    return t;
            }

            // 構造体
            var structType = AllClasses.Find((c) => c.IsStruct && name.IndexOf(c.OriginalName) >= 0);
            if (structType != null) return structType;

            // delegate
            foreach (var t in AllDelegates)
            {
                if (name.IndexOf(t.OriginalName) >= 0)
                    return t;
            }

            throw new InvalidOperationException("invalid type.");
        }

        #endregion
    }
}
