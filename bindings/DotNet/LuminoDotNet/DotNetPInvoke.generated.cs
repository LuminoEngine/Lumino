using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(ResultCode errorCode);

    /// <summary>
    /// 結果・エラーコード
    /// </summary>
    public enum ResultCode
    {
        /// <summary>
        /// 成功
        /// </summary>
        OK = 0,

        /// <summary>
        /// 不明なエラー
        /// </summary>
        ErrorUnknown = 1,

        /// <summary>
        /// 前提条件の検証エラー
        /// </summary>
        ErrorVERIFY = 2,

        /// <summary>
        /// 引数が無効
        /// </summary>
        ErrorARGUMENT = 3,

        /// <summary>
        /// オブジェクトの現在の状態に対して無効な呼び出しが行われた
        /// </summary>
        ErrorINVALID_OPERATION = 4,

        /// <summary>
        /// 未実装の機能を呼び出した
        /// </summary>
        ErrorNOT_IMPLEMENTED = 5,

        /// <summary>
        /// メモリ確保に失敗
        /// </summary>
        ErrorOUT_OF_MEMORY = 6,

        /// <summary>
        /// 値が有効な範囲に存在しない
        /// </summary>
        ErrorOUT_OF_RANGE = 7,

        /// <summary>
        /// 検索に使用したキーが存在しない
        /// </summary>
        ErrorKEY_NOT_FOUND = 8,

        /// <summary>
        /// 数値演算によるオーバーフローが発生した
        /// </summary>
        ErrorOVERFLOW = 9,

        /// <summary>
        /// その他のIOエラー
        /// </summary>
        ErrorIO = 10,

        /// <summary>
        /// ファイルにアクセスしようとして失敗した
        /// </summary>
        ErrorFILE_NOT_FOUND = 11,

        /// <summary>
        /// 無効なディレクトリにアクセスしようとした
        /// </summary>
        ErrorDIRECTORY_NOT_FOUND = 12,

        /// <summary>
        /// ファイルや文字列等の形式が不正
        /// </summary>
        ErrorINVALID_FORMAT = 13,

        /// <summary>
        /// ストリームの末尾を越えてアクセスしようとした
        /// </summary>
        ErrorEND_OF_STREAM = 14,

        /// <summary>
        /// 文字コードの変換中、マッピングできない文字または不正シーケンスが見つかった
        /// </summary>
        ErrorENCODING = 15,

        /// <summary>
        /// WindowsAPI のエラー
        /// </summary>
        ErrorWIN32 = 16,

        /// <summary>
        /// COM のエラー
        /// </summary>
        ErrorCOM = 17,

    }



    public class API
    {
    
#if DEBUG
    	internal const string DLLName = "LuminoUd";
#else
    	internal const string DLLName = "LuminoU";
#endif
        internal const CharSet DLLCharSet = CharSet.Unicode;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNRectF_RectF(ref RectF rectf);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNRectF_RectFXYWH(ref RectF rectf, float x, float y, float width, float height);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNRectF_GetLeft(ref RectF rectf, out float outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNRectF_SetSize(ref RectF rectf, ref Size size);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNRectF_GetSize(ref RectF rectf, out Size outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_GetLength(ref Vector3 vector3, out float outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_GetLengthSquared(ref Vector3 vector3, out float outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_Normalize(ref Vector3 vector3);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_Clamp(ref Vector3 vector3, ref Vector3 minVec, ref Vector3 maxVec);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNEngine_Initialize();

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNEngine_Terminate();

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNEngine_Update(out bool outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_SetVolume(IntPtr sound, float volume);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_GetVolume(IntPtr sound, out float outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_SetPitch(IntPtr sound, float pitch);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_GetPitch(IntPtr sound, out float outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_SetLoopEnabled(IntPtr sound, bool enabled);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_IsLoopEnabled(IntPtr sound, out bool outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_SetLoopRange(IntPtr sound, int begin, int length);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_Play(IntPtr sound);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_Stop(IntPtr sound);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_Pause(IntPtr sound);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_Resume(IntPtr sound);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSound_Initialize(string filePath, out IntPtr outSound);



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
