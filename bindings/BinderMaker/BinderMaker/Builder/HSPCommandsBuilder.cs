

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class HSPCommandsBuilder : Builder
    {
        private static string CmdCaseTemplate = @"
case [ID]:
{
[BEFORE]
[CALL]
[AFTER]
    return;
}
";
        struct TypeInfo
        {
            public string GetParamExp;

            public TypeInfo(string getParamExp)
            {
                GetParamExp = getParamExp;
            }
        }

        private Dictionary<CLType, TypeInfo> _primitiveTypeInfoTable = new Dictionary<CLType, TypeInfo>()
        {
            //{ CLPrimitiveType.Void,     "void" },
            { CLPrimitiveType.String,       new TypeInfo("str_p{0} = CodeGetS({1});") },
            { CLPrimitiveType.Bool,         new TypeInfo("LNBool p{0} = CodeGetI({1});") },
            { CLPrimitiveType.Byte,         new TypeInfo("byte_t p{0} = CodeGetI({1});") },
            { CLPrimitiveType.Int32,        new TypeInfo("int32_t p{0} = CodeGetI({1});") },
            { CLPrimitiveType.UInt32,       new TypeInfo("uint32_t p{0} = CodeGetI({1});") },
            { CLPrimitiveType.Float,        new TypeInfo("float p{0} = CodeGetD({1});") },
            { CLPrimitiveType.Double,       new TypeInfo("double p{0} = CodeGetD({1});") },
            { CLPrimitiveType.VoidPtr,      new TypeInfo("void* p{0} = (void*)CodeGetI({1});") },
            { CLPrimitiveType.IntPtr,       new TypeInfo("intptr_t p{0} = CodeGetI({1});") },
        };

        public const int ConstIdBegin = 100;
        private int _idCount = ConstIdBegin;
        private OutputBuffer _cmdCases = new OutputBuffer(1);

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
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
            var beforeCall = new OutputBuffer(1);
            for (int i = 0; i < method.FuncDecl.Params.Count; ++i)
            {
                var param = method.FuncDecl.Params[i];
                MakeGetParamExp(beforeCall, i, param);
            }
            string callExp = "    ";
            if (method.FuncDecl.ReturnType != null)
                callExp += "stat = ";
            callExp += method.FuncDecl.OriginalFullName + "(" + ");";

            string t = CmdCaseTemplate.TrimStart();
            t = t.Replace("[ID]", string.Format("0x{0:X4}", _idCount));
            t = t.Replace("[BEFORE]", beforeCall.ToString());
            t = t.Replace("[CALL]", callExp);
            t = t.Replace("[AFTER]", "");
            _cmdCases.AppendWithIndent(t);
            _idCount++;
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string t = GetTemplate("HSPCommands.txt");
            t = t.Replace("[COMMANDS]", _cmdCases.ToString());
            return t;
        }

        /// <summary>
        /// 出力ファイルのエンコーディング
        /// </summary>
        protected override Encoding GetOutputEncoding() { return Encoding.GetEncoding(932); }


        /// <summary>
        /// 呼び出し前処理
        /// </summary>
        /// <returns></returns>
        private void MakeGetParamExp(OutputBuffer beforeCall, int index, CLParam param)
        {
            string defaultValue = (string.IsNullOrEmpty(param.OriginalDefaultValue)) ? "NULL" : param.OriginalDefaultValue;

            // out 型は一律これ
            if (param.IOModifier == IOModifier.Out)
            {
                beforeCall.AppendLine(string.Format("PVal* pval{0};", index));
                beforeCall.AppendLine(string.Format("APTR aptr{0} = code_getva(&pval{0});", index));
                return;
            }

            // プリミティブ型
            if (_primitiveTypeInfoTable.ContainsKey(param.Type))
            {
                string t = string.Format(
                    _primitiveTypeInfoTable[param.Type].GetParamExp,
                    index, defaultValue);
                beforeCall.AppendLine(t);
                return;
            }

            // struct 型
            var classType = param.Type as CLClass;
            if (classType != null && classType.IsStruct)
            {
                string t = string.Format(
                    "PVal* pval{0}; CodeGetVA_TypeChecked(&pval{0}, {1});",
                    index, classType.OriginalName);
                beforeCall.AppendLine(t);
                return;
            }

            // class 型は intptr と同じ
            if (classType != null && !classType.IsStruct)
            {
                string t = string.Format(
                    _primitiveTypeInfoTable[CLPrimitiveType.IntPtr].GetParamExp,
                    index, defaultValue);
                beforeCall.AppendLine(t);
                return;
            }

            // enum 型は int と同じ
            if (param.Type is CLEnum)
            {
                string t = string.Format(
                    _primitiveTypeInfoTable[CLPrimitiveType.Int32].GetParamExp,
                    index, defaultValue);
                beforeCall.AppendLine(t);
                return;
            }

            throw new InvalidOperationException();
        }
    }
}
