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
        /// X 要素
        /// </summary>
        public float X;

        /// <summary>
        /// Y 要素
        /// </summary>
        public float Y;

        /// <summary>
        /// Z 要素
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
        public float GetLength()
        {
            float outReturn;
            var result = API.LNVector3_GetLength(ref this, out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

        /// <summary>
        /// ベクトルの長さの 2 乗を返します。
        /// </summary>
        public float GetLengthSquared()
        {
            float outReturn;
            var result = API.LNVector3_GetLengthSquared(ref this, out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

        /// <summary>
        /// このベクトルを正規化します。
        /// </summary>
        /// <remarks>
        /// ベクトルの長さが 0 の場合は正規化を行いません。
        /// </remarks>
        public void Normalize()
        {
            
            var result = API.LNVector3_Normalize(ref this);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// このベクトルを指定された最大値と最小値の範囲にクランプします。
        /// </summary>
        /// <param name="minVec">
        /// 最小値
        /// </param>
        /// <param name="maxVec">
        /// 最大値
        /// </param>
        public void Clamp(Vector3 minVec, Vector3 maxVec)
        {
            
            var result = API.LNVector3_Clamp(ref this, ref minVec, ref maxVec);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// 指定ベクトルを正規化したベクトルを返します。
        /// </summary>
        /// <param name="x">
        /// 処理の基になるベクトルの X 要素
        /// </param>
        /// <param name="y">
        /// 処理の基になるベクトルの Y 要素
        /// </param>
        /// <param name="z">
        /// 処理の基になるベクトルの Z 要素
        /// </param>
        /// <returns>
        /// 正規化されたベクトル
        /// </returns>
        public static Vector3 Normalize(float x, float y, float z)
        {
            Vector3 outReturn = new Vector3();
            var result = API.LNVector3_NormalizeXYZ(x, y, z, out outReturn);
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
        public static Vector3 Normalize(Vector3 vec)
        {
            Vector3 outReturn = new Vector3();
            var result = API.LNVector3_NormalizeV(ref vec, out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

    }
    /// <summary>
    /// 2次元上のオブジェクトサイズを表します。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Size
    {
        /// <summary>
        /// 横幅
        /// </summary>
        public float Width;

        /// <summary>
        /// 縦幅
        /// </summary>
        public float Height;

        public Size(float width, float height)
        {
            Width = width;
            Height = height;
        }

    }
    /// <summary>
    /// 2次元の矩形を表すクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct RectF
    {
        /// <summary>
        /// 左辺の X 座標
        /// </summary>
        public float X;

        /// <summary>
        /// 上辺の Y 座標
        /// </summary>
        public float Y;

        /// <summary>
        /// 幅
        /// </summary>
        public float Width;

        /// <summary>
        /// 高さ
        /// </summary>
        public float Height;

        public RectF(float x, float y, float width, float height)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }

        /// <summary>
        /// 左辺の x 軸の値を取得します。
        /// </summary>
        public float GetLeft()
        {
            float outReturn;
            var result = API.LNRectF_GetLeft(ref this, out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

        /// <summary>
        /// 幅と高さを取得します。
        /// 幅と高さを設定します。
        /// </summary>
        public Size Size
        {
            get
            {
                Size outReturn = new Size();
                var result = API.LNRectF_GetSize(ref this, out outReturn);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outReturn;
            }
            set
            {
                
                var result = API.LNRectF_SetSize(ref this, ref value);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                
            }
        }

    }

}

