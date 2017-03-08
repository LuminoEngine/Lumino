using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(ResultCode errorCode);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate ResultCode LNGameScene_OnStart_OverrideCaller(IntPtr gamescene);


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

    /// <summary>
    /// テクスチャのピクセルフォーマット
    /// </summary>
    public enum TextureFormat
    {
        /// <summary>
        /// Unknown
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM)
        /// </summary>
        R8G8B8A8 = 1,

        /// <summary>
        /// 32 ビットのアルファ無しフォーマット
        /// </summary>
        R8G8B8X8 = 2,

        /// <summary>
        /// 32 ビットのアルファ付きフォーマット (GDI互換フォーマット。MME 互換のために定義している)
        /// </summary>
        B8G8R8A8 = 3,

        /// <summary>
        /// 32 ビットのアルファ無しフォーマット
        /// </summary>
        B8G8R8X8 = 4,

        /// <summary>
        /// 64 ビットの浮動小数点フォーマット
        /// </summary>
        R16G16B16A16_Float = 5,

        /// <summary>
        /// 128 ビットの浮動小数点フォーマット
        /// </summary>
        R32G32B32A32_Float = 6,

        /// <summary>
        /// 16 ビットの浮動小数点フォーマット
        /// </summary>
        R16_Float = 7,

        /// <summary>
        /// 32 ビットの浮動小数点フォーマット
        /// </summary>
        R32_Float = 8,

        /// <summary>
        /// S8 32 ビットの深度バッファフォーマット
        /// </summary>
        D24S8 = 9,

    }



    public class API
    {
    
        internal const string DLLName = "LuminoU";
    
        internal const CharSet DLLCharSet = CharSet.Unicode;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_GetLength(ref Vector3 vector3, out float outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_GetLengthSquared(ref Vector3 vector3, out float outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_Normalize(ref Vector3 vector3);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_Clamp(ref Vector3 vector3, ref Vector3 minVec, ref Vector3 maxVec);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_NormalizeXYZ(float x, float y, float z, out Vector3 outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNVector3_NormalizeV(ref Vector3 vec, out Vector3 outReturn);

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

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNTexture2D_Initialize(int width, int height, TextureFormat format, bool mipmap, out IntPtr outTexture2D);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNTexture2D_InitializeFFM(string filePath, TextureFormat format, bool mipmap, out IntPtr outTexture2D);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSceneNode_SetPosition(IntPtr scenenode, ref Vector3 pos);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSceneNode_GetPosition(IntPtr scenenode, out Vector3 outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSceneNode_SetVisible(IntPtr scenenode, bool visible);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSceneNode_IsVisible(IntPtr scenenode, out bool outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSprite_SetTexture(IntPtr sprite, IntPtr texture);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSprite_GetTexture(IntPtr sprite, out IntPtr outReturn);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSprite2D_Initialize(out IntPtr outSprite2D);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNSprite2D_InitializeT(IntPtr texture, out IntPtr outSprite2D);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNGameApplication_Run(IntPtr gameapplication, IntPtr initialScene);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNGameApplication_Initialize(out IntPtr outGameApplication);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNGameScene_OnStart(IntPtr gamescene);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNGameScene_OnStart_CallVirtualBase(IntPtr gamescene);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNGameScene_OnStart_SetOverrideCaller(LNGameScene_OnStart_OverrideCaller func);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNGameScene_Initialize(out IntPtr outGameScene);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNBoxCollisionShape_Initialize(ref Vector3 size, out IntPtr outBoxCollisionShape);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNCollider_ConnectOnTriggerEnter(IntPtr collider, IntPtr handler);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNCollider_ConnectOnTriggerLeave(IntPtr collider, IntPtr handler);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNCollider_ConnectOnTriggerStay(IntPtr collider, IntPtr handler);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNCollider_Initialize(out IntPtr outCollider);



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
