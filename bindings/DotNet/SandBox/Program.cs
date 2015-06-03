using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LNote
{
    public enum Result
    {
        /// <summary>
        /// 成功
        /// </summary>
        OK = 0,
    }

    static class API
    {


#if DEBUG
        internal const string DLLName = "LNote_d.dll";
#else
    	internal const string DLLName = "LNote.dll";
#endif
        internal const CharSet DLLCharSet = CharSet.Unicode;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;

        /// <summary>
        /// オブジェクトを解放します。
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_Release(IntPtr hadnleObject);

        /// <summary>
        /// オブジェクトの参照カウントをインクリメントします。
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_AddRef(IntPtr hadnleObject);

        /// <summary>
        /// オブジェクトの参照カウント取得します。
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        /// <param name="count">参照カウントを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_GetRefCount(IntPtr hadnleObject, out int count);





        /// <summary>
        /// LightNote を初期化します。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_Initialize();

        /// <summary>
        /// フレームを更新します。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_Update();

        /// <summary>
        /// 画面の更新タイミングをリセットします。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_ResetFrameDelay();

        /// <summary>
        /// アプリケーションを終了するべきかを確認します。
        /// </summary>
        /// <param name="requested">終了要求の有無を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_IsEndRequested(out bool requested);

        /// <summary>
        /// LightNote の終了処理を行います。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNApplication_Terminate();

    }
}

namespace SandBox
{

    class Program
    {
        static void Main(string[] args)
        {
            LNote.API.LNApplication_Initialize();

            bool endRequested = false;
            do
            {
                LNote.API.LNApplication_Update();
                LNote.API.LNApplication_IsEndRequested(out endRequested);
            } while (!endRequested);

            LNote.API.LNApplication_Terminate();
        }
    }
}
