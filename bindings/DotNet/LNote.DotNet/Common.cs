using System;
using System.Runtime.InteropServices;

namespace LN
{
    /// <summary>
    /// ライブラリ内部で発生した例外
    /// </summary>
    public class LNoteException : Exception
    {
        /// <summary>
        /// 結果コード
        /// </summary>
        public Result ResultCode { get; private set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name=""resultCode"">結果コード</param>
        public LNoteException(Result resultCode)
        {
            ResultCode = resultCode;
        }
    }
}
