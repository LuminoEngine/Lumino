using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLEntity
    {
        #region Properties

        /// <summary>
        /// 管理クラス
        /// </summary>
        public CLManager Manager { get { return CLManager.Instance; } }

        /// <summary>
        /// このエンティティを参照しているオブジェクトのリスト
        /// </summary>
        public List<object> ReferenceList { get; private set; }

        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLEntity()
        {
            ReferenceList = new List<object>();
            Manager.AllEntities.Add(this);
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、オリジナルの型名から CLType を検索して参照する
        /// </summary>
        public virtual void LinkTypes()
        {
        }

        
        #endregion
    }

    /// <summary>
    /// 型エンティティ
    /// </summary>
    class CLType : CLEntity
    {
        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLType()
        {
            Manager.AllTypes.Add(this);
        }

        /// <summary>
        /// RefObject 型かをチェック
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static bool CheckRefObjectType(CLType type)
        {
            var c = type as CLClass;
            if (c != null && c.IsReferenceObject)
                return true;
            return false;
        }

        /// <summary>
        /// RefObject 型かをチェック
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static bool CheckStructType(CLType type)
        {
            var c = type as CLClass;
            if (c != null && c.IsStruct)
                return true;
            return false;
        }
        #endregion
    }

    /// <summary>
    /// プリミティブ型
    /// </summary>
    class CLPrimitiveType : CLType
    {
        #region Constants

        public static CLPrimitiveType Void;
        public static CLPrimitiveType String;
        public static CLPrimitiveType Bool;
        public static CLPrimitiveType Byte;
        public static CLPrimitiveType Int;
        public static CLPrimitiveType UInt32;
        public static CLPrimitiveType Float;
        public static CLPrimitiveType Double;
        public static CLPrimitiveType IntPtr;

        #endregion

        #region Properties
        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLPrimitiveType(string name)
        {
            Name = name;
        }
        #endregion
    }
    /// <summary>
    /// リテラル (bool、int、float、enum)
    /// デフォルト引数や enum メンバ値で使用する。
    /// </summary>
    class CLLiteral : CLEntity
    {
        #region Types
        public enum ValueType
        {
            Bool,
            Int,
            Float,
            Enum,
        }
        #endregion

        #region Properties
        /// <summary>
        /// 値の型
        /// </summary>
        public ValueType Type { get; private set; }

        /// <summary>
        /// 値
        /// </summary>
        public object Value { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="value">値の文字列表現</param>
        public CLLiteral(string value)
        {
            if (value == "LN_TRUE")
            {
                Type = ValueType.Bool;
                Value = true;
                return;
            }
            if (value == "LN_FALSE")
            {
                Type = ValueType.Bool;
                Value = false;
                return;
            }
            int vInt;
            if (int.TryParse(value, out vInt))  // 1.5 とか小数点があると失敗する
            {
                Type = ValueType.Int;
                Value = vInt;
                return;
            }
            float vFloat;
            if (float.TryParse(value, out vFloat))
            {
                Type = ValueType.Float;
                Value = vFloat;
                return;
            }

            // 以上に当てはまらなければとりあえず enum
            Type = ValueType.Enum;
            Value = value;
        }
        #endregion
    }

    /// <summary>
    /// struct 定義
    /// </summary>
    class CLStruct : CLType
    {
        #region Properties
        /// <summary>
        /// オリジナルの名前
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// メンバリスト
        /// </summary>
        public List<CLStructMember> Members { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="comment"></param>
        /// <param name="name"></param>
        /// <param name="members"></param>
        public CLStruct(string comment, string name, IEnumerable<CLStructMember> members)
        {
            OriginalName = name;
            Comment = comment.Trim();
            Name = name.Trim().Substring(2);   // プレフィックスを取り除く
            Members = new List<CLStructMember>(members);
            Manager.AllStructDefs.Add(this);    // 登録
        }

        #endregion
    }

    /// <summary>
    /// enum メンバ
    /// </summary>
    class CLStructMember : CLEntity
    {
        #region Fields
        private string _originalTypeName;   // オリジナルの型名
        #endregion

        #region Properties
        /// <summary>
        /// 型
        /// </summary>
        public CLType Type { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="typeName"></param>
        /// <param name="name"></param>
        /// <param name="comment"></param>
        public CLStructMember(string typeName, string name, string comment)
        {
            _originalTypeName = typeName.Trim();
            Name = name.Trim();
            Comment = comment.Trim();
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、オリジナルの型名から CLType を検索して参照する
        /// </summary>
        public override void LinkTypes()
        {
            Type = Manager.FindType(_originalTypeName);
            Type.ReferenceList.Add(this);
        }
        #endregion
    }

    /// <summary>
    /// enum 定義
    /// </summary>
    class CLEnum : CLType
    {
        #region Properties
        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// オリジナルの名前
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// メンバリスト
        /// </summary>
        public List<CLEnumMember> Members { get; private set; }

        /// <summary>
        /// C# の [Flags] 用
        /// </summary>
        public bool IsFlags { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="comment"></param>
        /// <param name="name"></param>
        /// <param name="members"></param>
        public CLEnum(string comment, string name, IEnumerable<CLEnumMember> members)
        {
            Comment = comment.Trim();
            OriginalName = name.Trim();
            Name = name.Trim().Substring(2);   // プレフィックスを取り除く
            Members = new List<CLEnumMember>(members);

            // [Flags] チェック
            IsFlags = false;
            foreach (var memnber in Members)
            {
                if (memnber.OriginalValue.Length >= 2 &&
                    memnber.OriginalValue[0] == '0' &&
                    memnber.OriginalValue[1] == 'x')
                {
                    IsFlags = true;
                    break;
                }
            }

            // 定数定義を持たない EnumMemberDecl に値を割り振っていく
            int lastValue = 0;
            foreach (var decl in Members)
            {
                decl.Value = decl.OriginalValue;
                // 定数定義が無い場合は直前の値
                if (string.IsNullOrEmpty(decl.Value))
                {
                    decl.Value = lastValue.ToString();
                }
                // 定数定義がある場合は lastValue にする
                else
                {
                    if (!int.TryParse(decl.Value, out lastValue))
                    {
                        // 16進数かもしれない
                        lastValue = (int)Convert.ToInt32(decl.Value, 16);   // Convert.ToInt32 は頭に 0x が付いていても変換できる
                    }
                }
                lastValue++;
            }

            // 登録
            Manager.AllEnums.Add(this);
        }

        #endregion
    }

    /// <summary>
    /// enum メンバ
    /// </summary>
    class CLEnumMember : CLEntity
    {
        #region Fields
        //private string _originalValue;   // オリジナルの値

        // CapitalizedName で小文字化しない enum 定数名
        private static readonly List<string> NonCapitalizeWords = new List<string>()
        {
            "XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX",
            "BGM", "BGS", "ME",
            "A8R8G8B8", "X8R8G8B8", "A16B16G16R16F", "A32B32G32R32F", "D24S8",
        };

        #endregion

        #region Properties
        /// <summary>
        /// オリジナルの名前
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// オリジナルの値
        /// </summary>
        public string OriginalValue { get; private set; }

        /// <summary>
        /// 値 (整数か16進整数 の文字列が必ず入っている)
        /// </summary>
        public string Value { get; set; }

        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }

        /// <summary>
        /// enum ターミネータであるか
        /// </summary>
        public bool IsTerminator
        {
            get { return OriginalName.LastIndexOf(CLManager.EnumTerminatorName) == OriginalName.Length - CLManager.EnumTerminatorName.Length; }
        }

        /// <summary>
        /// プレフィックスを除き、各単語の先頭を大文字にして _ を結合した名前
        /// 例: LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX → ScalingWithLetterBox
        /// </summary>
        public string CapitalizedName
        {
            get
            {
                if (OriginalName == "LN_OK") return "OK";   // これだけ特殊扱い

                // 先頭大文字化の準備
                CultureInfo cultureInfo = Thread.CurrentThread.CurrentCulture;
                TextInfo textInfo = cultureInfo.TextInfo;

                string[] idents = OriginalName.Split('_');
                string name = "";
                for (int i = 2; i < idents.Count(); i++)
                {
                    if (NonCapitalizeWords.Contains(idents[i]))
                        name += idents[i];
                    else
                        name += textInfo.ToTitleCase(textInfo.ToLower(idents[i]));//char.ToUpper(idents[i][0]) + idents[i].Substring(1);    // 文字列の先頭を大文字に
                }


                //---------------------------------------------
                // LN_BUTTON 特殊化 (数値だけになってしまうものは頭に "Button" を付ける)
                if (idents[1] == "BUTTON")
                {
                    int tmp;
                    if (int.TryParse(name, out tmp))
                        name = "Button" + name;
                }

                return name;
            }
        }

        /// <summary>
        /// プレフィックスを除いた名前
        /// 例: LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX → SCALING_WITH_LETTER_BOX
        /// </summary>
        public string CommonName
        { 
            get
            {
                if (OriginalName == "LN_OK") return "OK";   // これだけ特殊扱い

                // 先頭大文字化の準備
                CultureInfo cultureInfo = Thread.CurrentThread.CurrentCulture;
                TextInfo textInfo = cultureInfo.TextInfo;

                string[] idents = OriginalName.Split('_');
                string name = "";
                for (int i = 2; i < idents.Count(); i++)
                {
                    if (!string.IsNullOrEmpty(name))
                        name += "_";
                    name += idents[i];
                }
                return name;
            }
        }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="comment"></param>
        public CLEnumMember(string name, string value, string comment)
        {
            OriginalName = name.Trim();
            OriginalValue = value.Trim();
            Comment = comment.Trim();
        }
        #endregion
    }

    /// <summary>
    /// 関数ポインタ
    /// </summary>
    class CLDelegate : CLType
    {
        #region Fields
        private string _originalReturnTypeName;
        #endregion

        #region Properties

        /// <summary>
        /// オリジナルの名前
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// 仮引数リスト
        /// </summary>
        public List<CLParam> Params { get; private set; }
        #endregion

        #region Methods
        public CLDelegate(CLDocument doc, string returnType, string name, IEnumerable<CLParam> params1)
        {
            _originalReturnTypeName = returnType;
            OriginalName = name;

            Document = doc;
            Name = name.Substring(2);   // プレフィックスを取り除く
            Params = new List<CLParam>(params1);

            // 登録
            Manager.AllDelegates.Add(this);
        }

        #endregion
    }
}
