using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * Ruby用拡張ライブラリをCで作る
 * http://members.jcom.home.ne.jp/mitakelp/makeext.html
 */

namespace BinderMaker.Builder
{
    static class RubyCommon
    {
        public class RubyTypeInfo
        {
            public string RubyType;
            public string CheckExp;     // 型判別式
            public string CVarType;     // 変数型
            public string ConvRToCExp;  // Ruby → C 変換式
            public string CVarInitExp;  // C言語宣言文変換式    (いらない？)
            public RubyTypeInfo(string rubyType, string checkExp, string cVarType, string convRToCExp, string cVarInitExp)
            {
                RubyType = rubyType;
                CheckExp = checkExp;
                CVarType = cVarType;
                ConvRToCExp = convRToCExp;
                CVarInitExp = cVarInitExp;
            }
        }

        public static Dictionary<CLType, RubyTypeInfo> _rubyTypeTable = new Dictionary<CLType, RubyTypeInfo>()
        {
            //{ CLPrimitiveType.Void }

            //{ CLPrimitiveType.ByteArray,    new RubyTypeInfo("Array",   "", "", "", "") },
            { CLPrimitiveType.String,       new RubyTypeInfo("String",  "isRbString({0})", "char*",        "StringValuePtr({0})",       "char* {0} = StringValuePtr({1});") },

            { CLPrimitiveType.Bool,         new RubyTypeInfo("Bool",    "isRbBool({0})",   "lnBool",       "RbBooltoBool({0})",         "lnBool {0} = RbBooltoBool({1});") },
            { CLPrimitiveType.Byte,         new RubyTypeInfo("Integer", "isRbNumber({0})", "lnU8",         "FIX2INT({0})",              "lnU8 {0} = FIX2INT({1});") },
            { CLPrimitiveType.Int,          new RubyTypeInfo("Integer", "isRbNumber({0})", "int",          "FIX2INT({0})",              "int {0} = FIX2INT({1});") },
            { CLPrimitiveType.UInt32,       new RubyTypeInfo("Integer", "isRbNumber({0})", "lnU32",        "FIX2INT({0})",              "lnU32 {0} = FIX2INT({1});") },
            { CLPrimitiveType.Float,        new RubyTypeInfo("Float",   "isRbFloat({0})",  "float",        "((float)NUM2DBL({0}))",    "float {0} = static_cast<float>(NUM2DBL({1}));") },
            { CLPrimitiveType.Double,       new RubyTypeInfo("Float",   "isRbFloat({0})",  "double",       "NUM2DBL({0})",              "double {0} = NUM2DBL({1});") },

            // { CLPrimitiveType.ResultCode,   new RubyTypeInfo("Integer", "isRbNumber({0})", "lnResultCode", "(lnResultCode)FIX2INT({0})",   "lnResultCode {0} = (lnResultCode)FIX2INT({1});") },
            //{ CLPrimitiveType.ExceptionCallback, new RubyTypeInfo("",   "isRbNumber({0})", "lnExceptionCallback", null, null) },
            { CLPrimitiveType.IntPtr,       new RubyTypeInfo("",        "isRbNumber({0})", "lnIntPtr", null, null) },    // TODO:bignum?
            // { CLPrimitiveType.HWND,         new RubyTypeInfo("",        "isRbNumber({0})", "HWND",         "FIX2INT({0})", "int {0} = FIX2INT({1});") },      // TODO:bignum?

            //{ CLPrimitiveType.Handle = new CLPrimitiveType("Handle");
            //{ CLPrimitiveType.Generic, new RubyTypeInfo("",             "isRbObject({0})",      "lnHandle",     "FIX2INT({0})", "lnHandle {0} = FIX2INT({1});") },

        };

        /// <summary>
        /// VALUE → C型へのキャスト式
        /// </summary>
        /// <param name="type"></param>
        /// <param name="val">VALUE 型の変数名</param>
        /// <returns></returns>
        public static string GetCastExpVALUEtoC(CLType targetType, string val)
        {
            // プリミティブ型
            RubyTypeInfo info;
            if (_rubyTypeTable.TryGetValue(targetType, out info))
            {
                return string.Format(info.ConvRToCExp, val);
            }

            // enum
            var enumType = targetType as CLEnum;
            if (enumType != null)
            {
                return string.Format("({0})FIX2INT({1})", enumType.OriginalName, val);
            }

            throw new InvalidOperationException();
        }

        /// <summary>
        /// InitAuto → init_audio
        /// </summary>
        /// <returns></returns>
        public static string GetSnakeStyleName(string text)
        {
            string output = "";

            // 後ろから
            bool isLastLow = char.IsLower(text, text.Length - 1);
            int upperCount = 0;
            int lastIndex = text.Length;
            int i;
            for (i = text.Length - 2; i >= 0; i--)
            {
                if (isLastLow)
                {
                    if (char.IsUpper(text, i))
                    {
                        if (lastIndex - i > 0)
                        {
                            if (output.Length != 0) output = "_" + output;
                            output = text.Substring(i, lastIndex - i).ToLower() + output;
                        }
                        lastIndex = i;
                        isLastLow = false;
                    }
                    else
                    {
                        // 小文字が続いている
                    }
                }
                else
                {
                    upperCount++;
                    if (char.IsLower(text, i))
                    {
                        upperCount = 0;
                        if (upperCount == 1)
                            isLastLow = true;
                        else
                        {
                            if (lastIndex - i - 1 > 0)
                            {
                                if (output.Length != 0) output = "_" + output;
                                output = text.Substring(i + 1, lastIndex - i - 1).ToLower() + output;
                            }
                            lastIndex = i + 1;
                            isLastLow = true;
                        }
                    }
                    else
                    {
                        // 大文字が続いている
                    }
                }
            }

            if (lastIndex != 0)
            {
                if (lastIndex - i > 0)
                {
                    if (output.Length != 0) output = "_" + output;
                    output = text.Substring(0, lastIndex).ToLower() + output;
                }
            }

            return output;
        }
    }
}
