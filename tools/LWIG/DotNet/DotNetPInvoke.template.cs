using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(Result errorCode);

%EnumList%

    public class API
    {
    
#if DEBUG
    	internal const string DLLName = "LuminoCUd";
#else
    	internal const string DLLName = "LuminoCU";
#endif
        internal const CharSet DLLCharSet = CharSet.Unicode;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
%APIList%

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open( string filePath,  string password);

        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile( string filePath,  string accessName);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();

        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrStringLength(IntPtr strPtr, out int len);

        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrString(IntPtr strPtr, StringBuilder outBuffer);
    }
}
