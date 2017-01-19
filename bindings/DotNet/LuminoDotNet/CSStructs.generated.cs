using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
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
        /// このベクトルを指定された最大値と最小値の範囲にクランプします。@param[in]	minVec	: 最小値@param[in]	maxVec	: 最大値
        /// </summary>
        public void Clamp(ref Vector3 minVec, ref Vector3 maxVec)
        {
            
            var result = API.LNVector3_Clamp(ref this, ref minVec, ref maxVec);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

    }

}

