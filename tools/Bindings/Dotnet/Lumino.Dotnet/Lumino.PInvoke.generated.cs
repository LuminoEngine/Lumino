using System;
using System.Runtime.InteropServices;

namespace Lumino
{
    /// <summary>
    /// ピクセルフォーマット
    /// </summary>
    public enum PixelFormat
    {
        /// <summary>
        /// Unknown
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// 8bit アルファ値のみのフォーマット
        /// </summary>
        A8 = 1,

        /// <summary>
        /// RGBA オーダーの各要素 8bit フォーマット
        /// </summary>
        RGBA8 = 2,

        /// <summary>
        /// RGB オーダーの各要素 8bit フォーマット
        /// </summary>
        RGB8 = 3,

        /// <summary>
        /// RGBA オーダーの各要素 32bit 浮動小数点フォーマット
        /// </summary>
        RGBA32F = 4,

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
        /// RGBA オーダーの各要素 8bit フォーマット
        /// </summary>
        RGBA8 = 1,

        /// <summary>
        /// RGB オーダーの各要素 8bit フォーマット
        /// </summary>
        RGB8 = 2,

        /// <summary>
        /// RGBA オーダーの各要素 16bit 浮動小数点フォーマット
        /// </summary>
        RGBA16F = 3,

        /// <summary>
        /// RGBA オーダーの各要素 32bit 浮動小数点フォーマット
        /// </summary>
        RGBA32F = 4,

        /// <summary>
        /// 16bit 浮動小数点フォーマット
        /// </summary>
        R16F = 5,

        /// <summary>
        /// 32bit 浮動小数点フォーマット
        /// </summary>
        R32F = 6,

        /// <summary>
        /// 32bit の符号なし整数フォーマット
        /// </summary>
        R32U = 7,

    }

    /// <summary>
    /// 深度バッファのフォーマット
    /// </summary>
    public enum DepthBufferFormat
    {
        /// <summary>
        /// S8 32 ビット
        /// </summary>
        D24S8 = 0,

    }


    /// <summary>
    /// 3次元のベクトルを定義します。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
    }

    /// <summary>
    /// クォータニオンを定義します。
    /// </summary>
    /// <remakes>
    /// このクォータニオンクラスの乗算の順番は左から右です。つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。
    /// この乗算順序は数学的な四元数の定義からは逆行している点に注意してください。
    /// </remakes>
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
    }


}
