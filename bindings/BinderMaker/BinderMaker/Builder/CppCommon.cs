using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class CppCommon
    {
        public class CTypeInfo
        {
            public string InType;
            public string OutType;
            public string CppInType;
            public string CppOutType;
            public string CppInOutType;
            public CTypeInfo(string inType, string outType, string cppType, string cppOutType, string cppInOutType)
            {
                InType = inType;
                OutType = outType;
                CppInType = cppType;
                CppOutType = cppOutType;
                CppInOutType = cppInOutType;
            }
        }

        // CLType → C 型名の変換テーブル
        public static Dictionary<CLType, CTypeInfo> _typeNameTable = new Dictionary<CLType, CTypeInfo>()
        {
            { CLPrimitiveType.Void,         new CTypeInfo("void", "void", "void", "void", null) },
            //{ CLPrimitiveType.ByteArray,    new CTypeInfo("const void*", "void**", "const void*", "void**", "void*") },
            { CLPrimitiveType.String,       new CTypeInfo("const lnChar*", "lnChar**", "const lnChar*", "lnChar**", null) },

            { CLPrimitiveType.Bool,         new CTypeInfo("LNBool", "LNBool*", "bool", "bool*", null) },
            { CLPrimitiveType.Byte,         new CTypeInfo("uint8_t", "uint8_t*", "uint8_t", "uint8_t*", null) },
            { CLPrimitiveType.Int32,          new CTypeInfo("int", "int*", "int", "int*", null) },
            { CLPrimitiveType.UInt32,       new CTypeInfo("uint32_t", "uint32_t*", "uint32_t", "uint32_t*", null) },
            { CLPrimitiveType.Int64,       new CTypeInfo("int64_t", "int64_t*", "int64_t", "int64_t*", null) },
            { CLPrimitiveType.Float,        new CTypeInfo("float", "float*", "float", "float*", null) },
            { CLPrimitiveType.Double,       new CTypeInfo("double", "double*", "double", "double*", null) },

            //{ CLPrimitiveType.ResultCode,   new CTypeInfo("lnResult", "lnResult*", "lnResult", "lnResult*", null) },
            //{ CLPrimitiveType.ExceptionCallback, new CTypeInfo("lnExceptionCallback", "lnExceptionCallback*", "lnExceptionCallback", "lnExceptionCallback*", null) },
            //{ CLPrimitiveType.IntPtr,       new CTypeInfo("lnIntPtr", "lnIntPtr*", "lnIntPtr", "lnIntPtr*", null) },
            //{ CLPrimitiveType.HWND,         new CTypeInfo("HWND", "HWND*", "void*", "void**", null) },

            //{ CLPrimitiveType.Handle,       new CTypeInfo("void", "void") },
            //{ CLPrimitiveType.Generic, "double" },

            //{ CLPrimitiveType.IntNativeArray,   new CTypeInfo("const int*", null, "const int*", null, null) },
        };

        /// <summary>
        /// C言語の型名への変換
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string ConvertTypeToCName(CLType type)
        {
            // テーブル検索
            CTypeInfo info;
            if (_typeNameTable.TryGetValue(type, out info))
            {
                return info.InType;
            }

            // enum 型であるか
            if (type is CLEnum)
                return ((CLEnum)type).OriginalName;

            // 参照オブジェクトまたは struct 型であるか
            var classType = type as CLClass;
            if (classType != null)
            {
                string name;
                if (classType.IsStruct)
                    name = classType.OriginalName;
                else
                    name = "LNHandle";
                return name;
            }

            // Generic
            //if (type == CLPrimitiveType.Generic)
            //    return "lnHandle";

            throw new InvalidOperationException();
        }
    }
}
