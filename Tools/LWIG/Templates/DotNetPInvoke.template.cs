using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(ResultCode errorCode);

%DelegateList%

%EnumList%

    public class API
    {
    
        internal const string DLLName = "LuminoU";
    
        internal const CharSet DLLCharSet = CharSet.Unicode;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
%APIList%

        //==============================================================================
        // LNObject
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObject_Release(IntPtr handle);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObject_AddRef(IntPtr handle);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObject_GetRefCount(IntPtr handle, out int outCount);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static IntPtr LNObject_GetBindingTypeData(IntPtr handle);


        //==============================================================================
        // LNError

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNError_GetLastErrorCode();

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNError_GetLastErrorMessage(out IntPtr outStr);
        
        //==============================================================================
        // LNInternal
        
        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrStringLength(IntPtr strPtr, out int len);

        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrString(IntPtr strPtr, StringBuilder outBuffer);

        //==============================================================================
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open( string filePath,  string password);

        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile( string filePath,  string accessName);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();
    }
}
