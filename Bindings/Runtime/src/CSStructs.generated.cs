using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
    /// <summary>
    /// 3次元のベクトルを定義します。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        /// <summary>
        /// X 要
        /// </summary>
        public float X;

        /// <summary>
        /// Y 要
        /// </summary>
        public float Y;

        /// <summary>
        /// Z 要
        /// </summary>
        public float Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// ベクトルの長さを返します。
        /// </summary>
        public float getLength()
        {
            float outReturn;
            var result = API.LNVector3_GetLength(ref this, out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

        /// <summary>
        /// ベクトルの長さの 2 乗を返します。
        /// </summary>
        public float getLengthSquared()
        {
            float outReturn;
            var result = API.LNVector3_GetLengthSquared(ref this, out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

        /// <summary>
        /// 指定ベクトルを正規化したベクトルを返します。
        /// </summary>
        /// <param name="x">
        /// 処理の基になるベクトルの X 要
        /// </param>
        /// <param name="y">
        /// 処理の基になるベクトルの Y 要
        /// </param>
        /// <param name="z">
        /// 処理の基になるベクトルの Z 要
        /// </param>
        /// <returns>
        /// 正規化されたベクトル
        /// </returns>
        public static Vector3 normalize()
        {
            Vector3 outReturn = new Vector3();
            var result = API.LNVector3_Normalize(out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

        /// <summary>
        /// 指定ベクトルを正規化したベクトルを返します。
        /// </summary>
        /// <param name="vec">
        /// 処理の基になるベクトル
        /// </param>
        /// <returns>
        /// 正規化されたベクトル
        /// </returns>
        public static Vector3 normalize()
        {
            Vector3 outReturn = new Vector3();
            var result = API.LNVector3_Normalize(out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

    }

}

