/*
 * ・HSP はクラスという概念は存在しないので、基本的に関数の変換だけとなる。
 * ・戻り値として構造体または float を使用している箇所に工夫が必要。
 * ・とりあえず直近の Audio リリースに向けては struct/string は必要ないので必要最低限にする。
 * 
 * 
 * ▼ C
 *  typedef (*LNStream_Read_OverrideCallback) (LNHandle stream, void* buffer, int size);
 *  
 * LNStream_Read(LNHandle stream, void* buffer, int size)
 * {
 *      
 * }
 * 
 * LNStream_SetReadOverride(LNStream_Read_OverrideCallback callback)
 * {
 *      
 * }
 * 
 * ▼ lnote_hsp
 * 
 * LNHspStream_Override_Read(LNHandle stream, void* buffer, int size)
 * {
 *      HspFunc_fread();
 *      LNStream_Read();    // もし既定の処理を呼び出すならこう。
 * }
 * 
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class HSPHeaderBuilder : Builder
    {
        private Dictionary<CLType, string> _primitiveTypeInfoTable = new Dictionary<CLType, string>()
        {
            //{ CLPrimitiveType.Void,     "void" },
            { CLPrimitiveType.String,       "str" },
            { CLPrimitiveType.Bool,         "int" },
            { CLPrimitiveType.Byte,         "int" },
            { CLPrimitiveType.Int32,          "int" },
            { CLPrimitiveType.UInt32,       "int" },
            { CLPrimitiveType.Float,        "double" },
            { CLPrimitiveType.Double,       "double" },
            { CLPrimitiveType.IntPtr,       "int" },
        };

        private OutputBuffer _allEnumText = new OutputBuffer();
        private OutputBuffer _allFuncDeclText = new OutputBuffer();
        private List<CLMethod> _outputOverrideFuncs = new List<CLMethod>(); // 文字列または float の出力を持つ関数

        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnEnumLooked(CLEnum enumType)
        {
            _allEnumText.AppendLine("// " + enumType.OriginalName);
            foreach (var m in enumType.Members)
            {
                if (!m.IsTerminator)
                    _allEnumText.AppendLine("#const " + m.OriginalName + " " + m.Value);
            }
            _allEnumText.NewLine();
        }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (!classType.IsStruct)
            {
                _allFuncDeclText.AppendLine("//" + classType.OriginalName);
            }
            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            if (!classType.IsStruct)
            {
                _allFuncDeclText.NewLine();
            }
        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop)
        {
            // プロパティとして割り振られたメソッドは OnMethodLooked() ではなくこの OnPropertyLooked() にくる。
            // やりたいことは OnMethodLooked() と同じなのでそちらにまわす。
            if (prop.Getter != null) OnMethodLooked(prop.Getter);
            if (prop.Setter != null) OnMethodLooked(prop.Setter);
        }

        /// <summary>
        /// メソッド 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            string prefix = "_";
            string funcName = method.FuncDecl.OriginalFullName;

            // 文字列または float の出力があるかチェック
            //if (method.FuncDecl.Params.Find((item) => item.IsOutStringType) != null ||
            //    method.FuncDecl.Params.Find((item) => CheckFloatOutput(item)) != null)
            //{
            //    prefix += "_typeOverride_";
            //    _outputOverrideFuncs.Add(method);
            //}
            
            //-------------------------------------------------
            // #func
            string decl = "#func native_" + funcName + " \"" + funcName;
            if (method.FuncDecl.Params.Find((item) => item.Type == CLPrimitiveType.Float) != null)
                decl += "D";    // float 型の場合はサフィックス "D" の付いた関数を呼ぶようにする
            decl += "\"";

            // #func の仮引数
            var paramsText = new OutputBuffer();
            foreach (var param in method.FuncDecl.Params)
            {
                paramsText.AppendCommad(ConvertToHSPParam(param));
            }
            _allFuncDeclText.AppendLine(decl + " " + paramsText.ToString());

            //-------------------------------------------------
            // #define
            decl = "#define " + funcName;

            // #define の仮引数
            if (method.FuncDecl.Params.Count() > 0)
            {
                paramsText.Clear();
                int i = 0;
                foreach (var param in method.FuncDecl.Params)
                {
                    string str = "%" + (i + 1).ToString();
                    if (!string.IsNullOrEmpty(param.OriginalDefaultValue))
                        str += "=" + param.OriginalDefaultValue;
                    paramsText.AppendCommad(str);
                    i++;
                }
                decl += "(" + paramsText.ToString() + ")";
            }

            // #define の置換要素 (呼び出し関数名)
            decl += " native_" + funcName;
            //if (prefix == "___")    // 多重オーバーライド
            //    outputDecl += " __" + func.CPPName;
            //else
            //    outputDecl += " _" + func.CPPName;

            // #define の置換要素 (実引数)
            paramsText.Clear();
            int i2 = 0;
            foreach (var param in method.FuncDecl.Params)
            {
                paramsText.AppendCommad("%" + (i2 + 1).ToString());
                i2++;
            }

            _allFuncDeclText.AppendLine(decl + " " + paramsText.ToString());
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string output = GetTemplate("HSP/HSPHeader.txt");
            return output
                .Replace("__ENUMS__", _allEnumText.ToString())
                .Replace("__FUNCS__", _allFuncDeclText.ToString());
        }

        /// <summary>
        /// 出力ファイルのエンコーディング
        /// </summary>
        protected override Encoding GetOutputEncoding() { return Encoding.GetEncoding(932); }

        /// <summary>
        /// 出力 float 型の仮引数であるか
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        private bool CheckFloatOutput(CLParam param)
        {
            return param.IOModifier == IOModifier.Out && param.Type == CLPrimitiveType.Float;
        }

        /// <summary>
        /// HSP 用仮引数の型を求める
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        private string ConvertToHSPParam(CLParam param)
        {
            if (param.IOModifier == IOModifier.Out) return "var";    // 出力型は一律 ver
            return ConvertToHSPType2(param.Type);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string ConvertToHSPType2(CLType type)
        {
            // プリミティブ型
            if (_primitiveTypeInfoTable.ContainsKey(type))
                return _primitiveTypeInfoTable[type];

            // struct 型
            var classType = type as CLClass;
            if (classType != null && classType.IsStruct) return "var";

            return "int";    // 登録された方でなければenumとみなして int にする
        }
    }
}
