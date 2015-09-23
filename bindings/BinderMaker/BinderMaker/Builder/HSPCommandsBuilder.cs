

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
    return true;
}
";
        struct TypeInfo
        {
            public string GetParamExp;
            public string SetToHSPVal;

            public TypeInfo(string getParamExp, string setToHSPVal)
            {
                GetParamExp = getParamExp;
                SetToHSPVal = setToHSPVal;
            }
        }

        private Dictionary<CLType, TypeInfo> _primitiveTypeInfoTable = new Dictionary<CLType, TypeInfo>()
        {
            //{ CLPrimitiveType.Void,     "void" },
            { CLPrimitiveType.String,       new TypeInfo("CodeGetS({1})", "code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_STR, p{0});") },
            { CLPrimitiveType.Bool,         new TypeInfo("(LNBool)CodeGetI({1})", "int rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_INT, &rp{0});") },
            { CLPrimitiveType.Byte,         new TypeInfo("CodeGetI({1})", "int rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_INT, &rp{0});") },
            { CLPrimitiveType.Int32,        new TypeInfo("CodeGetI({1})", "int rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_INT, &rp{0});") },
            { CLPrimitiveType.UInt32,       new TypeInfo("CodeGetI({1})", "int rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_INT, &rp{0});") },
            { CLPrimitiveType.Int64,        new TypeInfo("CodeGetI({1})", "int rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_INT, &rp{0});") },
            { CLPrimitiveType.Float,        new TypeInfo("CodeGetD({1})", "double rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_DOUBLE, &rp{0});") },
            { CLPrimitiveType.Double,       new TypeInfo("CodeGetD({1})", "double rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_DOUBLE, &rp{0});") },
            { CLPrimitiveType.VoidPtr,      new TypeInfo("(void*)CodeGetS({1})", "int rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_INT, &rp{0});") },
            { CLPrimitiveType.IntPtr,       new TypeInfo("CodeGetI({1})", "int rp{0} = p{0}; code_setva(pval_p{0}, aptr_p{0}, HSPVAR_FLAG_INT, &rp{0});") },
        };

        public const int ConstIdBegin = 100;
        private int _idCount = ConstIdBegin;
        private OutputBuffer _cmdCases = new OutputBuffer(1);
        private OutputBuffer _prototype = new OutputBuffer();

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (classType.IsStruct)
            {
                _prototype.AppendLine("extern int hsp{0}_typeid();", classType.StructData.OriginalName);
            }
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
            var argsText = new OutputBuffer();
            var afterCall = new OutputBuffer(1);
            for (int i = 0; i < method.FuncDecl.Params.Count; ++i)
            {
                var param = method.FuncDecl.Params[i];
                MakeCall(beforeCall, argsText, afterCall, i, param);
            }

            // 呼び出し式
            string callExp = "    ";
            if (method.FuncDecl.ReturnType != CLPrimitiveType.Void)
                callExp += "stat = ";
            callExp += method.FuncDecl.OriginalFullName + "(" + argsText.ToString() + ");";

            string t = CmdCaseTemplate.TrimStart();
            t = t.Replace("[ID]", string.Format("0x{0:X4}", _idCount));
            t = t.Replace("[BEFORE]", beforeCall.ToString().TrimEnd());
            t = t.Replace("[CALL]", callExp);
            t = t.Replace("[AFTER]", afterCall.ToString().TrimEnd());
            _cmdCases.AppendWithIndent(t);
            _idCount++;
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string t = GetTemplate("HSPCommands.txt");
            t = t.Replace("[PROTOTYPE]", _prototype.ToString());
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
        private void MakeCall(OutputBuffer beforeCall, OutputBuffer argsText, OutputBuffer afterCall, int index, CLParam param)
        {
            string t;
            string defaultValue = (string.IsNullOrEmpty(param.OriginalDefaultValue)) ? "" : param.OriginalDefaultValue;

            // out 型は一律、値を格納する HSP 変数への参照を準備
            if (param.IOModifier == IOModifier.Out)
            {
                beforeCall.AppendLine(string.Format("PVal* pval_p{0};", index));
                beforeCall.AppendLine(string.Format("APTR aptr_p{0} = code_getva(&pval_p{0});", index));
            }

            // プリミティブ型
            if (_primitiveTypeInfoTable.ContainsKey(param.Type))
            {
                if (param.IOModifier == IOModifier.Out)
                {
                    // 前処理
                    t = string.Format("{0} p{1};", CppCommon.ConvertTypeToCName(param.Type), index);
                    beforeCall.AppendLine(t);
                    // 実引数
                    argsText.AppendCommad("&p{0}", index.ToString());
                    // 後処理
                    t = string.Format(_primitiveTypeInfoTable[param.Type].SetToHSPVal, index);
                    afterCall.AppendLine(t);
                }
                else
                {
                    // 前処理
                    string name = "p" + index.ToString();
                    t = string.Format(_primitiveTypeInfoTable[param.Type].GetParamExp, index, defaultValue);
                    if (param.Type == CLPrimitiveType.String)
                    {
                        // string 型はキャッシュしたい
                        name = "str_" + name;
                        t = string.Format("{0} = {1};", name, t);
                        name += ".c_str()";
                    }
                    else
                    {
                        t = string.Format("{0} {1} = {2};", CppCommon.ConvertTypeToCName(param.Type), name, t);
                    }
                    beforeCall.AppendLine(t);
                    // 実引数
                    argsText.AppendCommad(name);
                }
                return;
            }

            // struct 型
            var classType = param.Type as CLClass;
            if (classType != null && classType.IsStruct)
            {
                if (param.IOModifier == IOModifier.Out)
                {
                    // 前処理
                    t = string.Format("{0} p{1};", classType.OriginalName, index);
                    beforeCall.AppendLine(t);
                    // 実引数
                    argsText.AppendCommad("&p{0}", index.ToString());
                    // 後処理
                    t = string.Format("code_setva(pval_p{0}, aptr_p{0}, hsp{1}_typeid(), &p{0});", index, classType.OriginalName);
                    afterCall.AppendLine(t);
                }
                else
                {
                    t = string.Format(
                        "PVal* pval_p{0}; CodeGetVA_TypeChecked(&pval_p{0}, {1});",
                        index, classType.OriginalName);
                    beforeCall.AppendLine(t);
                    argsText.AppendCommad("({0}*)pval_p{1}->pt", classType.OriginalName, index.ToString());
                }
                return;
            }

            // class 型は intptr と同じ
            if (classType != null && !classType.IsStruct)
            {
                if (param.IOModifier == IOModifier.Out)
                {
                    // 前処理
                    t = string.Format("{0} p{1};", "intptr_t", index);
                    beforeCall.AppendLine(t);
                    // 実引数
                    argsText.AppendCommad("&p{0}", index.ToString());
                    // 後処理
                    t = string.Format(_primitiveTypeInfoTable[CLPrimitiveType.IntPtr].SetToHSPVal, index);
                    afterCall.AppendLine(t);
                }
                else
                {
                    string name = "p" + index.ToString();
                    t = string.Format(_primitiveTypeInfoTable[CLPrimitiveType.IntPtr].GetParamExp, index, defaultValue);
                    t = string.Format("intptr_t {0} = {1};", name, t);
                    beforeCall.AppendLine(t);
                    argsText.AppendCommad(name);
                }
                return;
            }

            // enum 型は int と同じ。ただ、キャスト式が付く
            CLEnum enumType = param.Type as CLEnum;
            if (enumType != null)
            {
                if (param.IOModifier == IOModifier.Out)
                {
                    // 前処理
                    t = string.Format("{0} p{1};", enumType.OriginalName, index);
                    beforeCall.AppendLine(t);
                    // 実引数
                    argsText.AppendCommad("&p{0}", index.ToString());
                    // 後処理
                    t = string.Format(_primitiveTypeInfoTable[CLPrimitiveType.Int32].SetToHSPVal, index);
                    afterCall.AppendLine(t);
                }
                else
                {
                    string name = "p" + index.ToString();
                    t = string.Format(_primitiveTypeInfoTable[CLPrimitiveType.Int32].GetParamExp, index, defaultValue);
                    t = string.Format("int {0} = {1};", name, t);
                    beforeCall.AppendLine(t);
                    argsText.AppendCommad("(" + enumType.OriginalName + ")" + name);
                }
                return;
            }

            throw new InvalidOperationException();
        }
    }
}
