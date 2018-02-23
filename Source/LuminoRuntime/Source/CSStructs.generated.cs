using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
    /// <summary>
    /// 2次元のベクトルを定義します。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        /// <summary>
        /// X 要
        /// </summary>
        public float X;

        /// <summary>
        /// Y 要
        /// </summary>
        public float Y;

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

    }
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
        /// <param name="vec">
        /// 処理の基になるベクトル
        /// </param>
        /// <returns>
        /// 正規化されたベクトル
        /// </returns>
        public static Vector3 normalize(Vector3 vec)
        {
            Vector3 outReturn = new Vector3();
            var result = API.LNVector3_Normalize(ref vec, out outReturn);
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
        public static Vector3 normalize(float x, float y, float z)
        {
            Vector3 outReturn = new Vector3();
            var result = API.LNVector3_NormalizeXYZ(x, y, z, out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

    }
    /// <summary>
    /// クォータニオンを定義します。
    /// </summary>
    /// <remarks>
    /// このクォータニオンクラスの乗算の順番は左から右です。つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。この乗算順序は標準的な四元数の定義からは逆行していますが、ゲームプログラムをメインとした3Dグラフィックス向けの線形代数ライブラリとしてはこの動作がほぼ一般的なので、これに合わせています。<br>(DirectX(D3DX)、XNA、Unity、cocos2dx 等はこの動作)
    /// </remarks>
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
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

        /// <summary>
        /// W 要
        /// </summary>
        public float W;

        public Quaternion(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

    }
    /// <summary>
    /// 2次元上の点を表します。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Point
    {
        /// <summary>
        /// X 座標
        /// </summary>
        public float X;

        /// <summary>
        /// Y 座標
        /// </summary>
        public float Y;

        public Point(float x, float y)
        {
            X = x;
            Y = y;
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
    public struct Rect
    {
        /// <summary>
        /// 左辺の X 座標
        /// </summary>
        public float X;

        /// <summary>
        /// 辺の Y 座標
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

        public Rect(float x, float y, float width, float height)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }

        /// <summary>
        /// 左辺の x 軸の値を取得します。
        /// </summary>
        public float getLeft()
        {
            float outReturn;
            var result = API.LNRect_GetLeft(ref this, out outReturn);
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
                var result = API.LNRect_GetSize(ref this, out outReturn);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outReturn;
            }
            set
            {
                
                var result = API.LNRect_SetSize(ref this, ref value);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                
            }
        }

    }

}

