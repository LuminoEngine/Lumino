using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
    /// <summary>
    /// オブジェクトの幅と高さを表します。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Size
    {
        /// <summary>
        /// 幅
        /// </summary>
        public int Width;
        
        /// <summary>
        /// 高さ
        /// </summary>
        public int Height;
        
    
    
        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Size(int width, int height)
        {
            Width = width;
            Height = height;
        }
        
    
    };
    
    /// <summary>
    /// 2次元のベクトルのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float X;
        
        public float Y;
        
    
        /// <summary>
        /// 2D ベクトルの長さ
        /// </summary>
        public float Length
        {
            get
            {
                var outLength = new float();
                var result = API.LNVector2_GetLength(ref this, out outLength);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outLength;
            }
            
        }
        
        /// <summary>
        /// 2D ベクトルの長さの2乗
        /// </summary>
        public float SquareLength
        {
            get
            {
                var outLength = new float();
                var result = API.LNVector2_GetSquareLength(ref this, out outLength);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outLength;
            }
            
        }
        
    
        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
        
        /// <summary>
        /// 2D ベクトルに値を設定します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        public void Set( float x,  float y)
        {
            var result = API.LNVector2_Set(ref this,  x,  y);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 2D ベクトルを正規化します。
        /// </summary>
        /// <param name="vec">処理の基になる 2D ベクトル</param>
        public static Vector2 Normalize( Vector2 vec)
        {
            var outVec = new Vector2();
            var result = API.LNVector2_Normalize(ref vec, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 2D ベクトルを正規化します。
        /// </summary>
        public void Normalize()
        {
            var result = API.LNVector2_NormalizeV(ref this);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
    
    };
    
    /// <summary>
    /// 3 次元ベクトルのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float X;
        
        public float Y;
        
        public float Z;
        
    
        /// <summary>
        /// 3Dベクトルの長さ
        /// </summary>
        public float Length
        {
            get
            {
                var outLength = new float();
                var result = API.LNVector3_GetLength(ref this, out outLength);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outLength;
            }
            
        }
        
        /// <summary>
        /// 3D ベクトルの長さの2乗を計算する
        /// </summary>
        public float SquareLength
        {
            get
            {
                var outLength = new float();
                var result = API.LNVector3_GetSquareLength(ref this, out outLength);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outLength;
            }
            
        }
        
    
        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
        
        /// <summary>
        /// 3Dベクトルに値を設定します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        public void Set( float x,  float y,  float z)
        {
            var result = API.LNVector3_Set(ref this,  x,  y,  z);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 2D ベクトル と Z値 を指定して、3Dベクトルに値を設定します。
        /// </summary>
        /// <param name="vec2">2D ベクトル</param>
        /// <param name="z">Z 値</param>
        public void Set( Vector2 vec2,  float z)
        {
            var result = API.LNVector3_SetVZ(out this, ref vec2,  z);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 3D ベクトルを正規化します。
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        public static Vector3 Normalize( Vector3 vec)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Normalize(ref vec, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 3D ベクトルを正規化します。
        /// </summary>
        public void Normalize()
        {
            var result = API.LNVector3_NormalizeV(ref this);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 2 つの 3D ベクトルの内積を計算します。
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        public static float Dot( Vector3 vec1,  Vector3 vec2)
        {
            var dot = new float();
            var result = API.LNVector3_Dot(ref vec1, ref vec2, out dot);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return dot;
        }
        
        /// <summary>
        /// 2 つの 3D ベクトルの外積を計算します。
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        public static Vector3 Cross( Vector3 vec1,  Vector3 vec2)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Cross(ref vec1, ref vec2, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 指定された法線で反射させたベクトルを計算します。
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        public static Vector3 Reflect( Vector3 vec,  Vector3 normal)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Reflect(ref vec, ref normal, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 指定された法線と方向から滑りベクトルを計算します。
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        public static Vector3 Slide( Vector3 vec,  Vector3 normal)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Slide(ref vec, ref normal, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 2 つの 3D ベクトル間を線形補間します。
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
        /// <param name="vec2">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
        /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
        public static Vector3 Lerp( Vector3 vec1,  Vector3 vec2,  float t)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Lerp(ref vec1, ref vec2,  t, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 指定された 3D ベクトルを Catmull-Rom 補間します。
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
        /// <param name="vec3">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
        /// <param name="vec4">処理の基になる 3D ベクトル</param>
        /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
        public static Vector3 CatmullRom( Vector3 vec1,  Vector3 vec2,  Vector3 vec3,  Vector3 vec4,  float t)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_CatmullRom(ref vec1, ref vec2, ref vec3, ref vec4,  t, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 指定された行列を使用して座標変換します。
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="mat">処理の基になる 行列</param>
        /// <remarks>
        /// ベクトルを ( x, y, z, 1 ) として座標変換します。
        /// 		            結果の w は出力されません。
        /// 		            結果を w = 1 に射影する (x y z を w で除算) 場合は
        /// 		            Vector3TransformCoord() を使用してください。
        /// </remarks>
        public static Vector4 Transform( Vector3 vec,  Matrix mat)
        {
            var outVec = new Vector4();
            var result = API.LNVector3_Transform(ref vec, ref mat, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 指定された行列を使用して座標変換し、結果を w = 1 に射影します。
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="mat">処理の基になる 行列</param>
        /// <remarks>
        /// ベクトルを ( x, y, z, 1 ) として座標変換し、
        /// 		            結果を w = 1 に射影 (x y z を w で除算) します。
        /// </remarks>
        public static Vector3 TransformCoord( Vector3 vec,  Matrix mat)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_TransformCoord(ref vec, ref mat, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
    
    };
    
    /// <summary>
    /// 4 次元ベクトルのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4
    {
        public float X;
        
        public float Y;
        
        public float Z;
        
        public float W;
        
    
    
        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }
        
        /// <summary>
        /// 4Dベクトルに値を設定します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        /// <param name="w">W 値</param>
        public void Set( float x,  float y,  float z,  float w)
        {
            var result = API.LNVector4_Set(ref this,  x,  y,  z,  w);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
    
    };
    
    /// <summary>
    /// 4x4 行列のクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Matrix
    {
        public float M11;
        
        public float M12;
        
        public float M13;
        
        public float M14;
        
        public float M21;
        
        public float M22;
        
        public float M23;
        
        public float M24;
        
        public float M31;
        
        public float M32;
        
        public float M33;
        
        public float M34;
        
        public float M41;
        
        public float M42;
        
        public float M43;
        
        public float M44;
        
    
        /// <summary>
        /// 右方向を示す 3D ベクトル
        /// </summary>
        public Vector3 Right
        {
            get
            {
                var outVec = new Vector3();
                var result = API.LNMatrix_GetRight(ref this, out outVec);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outVec;
            }
            
        }
        
        /// <summary>
        /// 上方向を示す 3D ベクトル
        /// </summary>
        public Vector3 Up
        {
            get
            {
                var outVec = new Vector3();
                var result = API.LNMatrix_GetUp(ref this, out outVec);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outVec;
            }
            
        }
        
        /// <summary>
        /// 正面方向を示す 3D ベクトル
        /// </summary>
        public Vector3 Front
        {
            get
            {
                var outVec = new Vector3();
                var result = API.LNMatrix_GetFront(ref this, out outVec);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outVec;
            }
            
        }
        
        /// <summary>
        /// 位置を示す 3D ベクトル
        /// </summary>
        public Vector3 Position
        {
            get
            {
                var outVec = new Vector3();
                var result = API.LNMatrix_GetPosition(ref this, out outVec);
                if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outVec;
            }
            
        }
        
    
        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
        {
            M11 = m11;
            M12 = m12;
            M13 = m13;
            M14 = m14;
            M21 = m21;
            M22 = m22;
            M23 = m23;
            M24 = m24;
            M31 = m31;
            M32 = m32;
            M33 = m33;
            M34 = m34;
            M41 = m41;
            M42 = m42;
            M43 = m43;
            M44 = m44;
        }
        
        /// <summary>
        /// 単位行列を作成します。
        /// </summary>
        public void Identity()
        {
            var result = API.LNMatrix_Identity(out this);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列を平行移動します。
        /// </summary>
        /// <param name="x">X 方向の移動量</param>
        /// <param name="y">Y 方向の移動量</param>
        /// <param name="z">Z 方向の移動量</param>
        /// <remarks>
        /// 与えられた引数から平行移動行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Translate( float x,  float y,  float z)
        {
            var result = API.LNMatrix_Translate(ref this,  x,  y,  z);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列を平行移動します。
        /// </summary>
        /// <param name="vec">移動量</param>
        /// <remarks>
        /// 与えられた引数から平行移動行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void TranslateVec3( Vector3 vec)
        {
            var result = API.LNMatrix_TranslateVec3(ref this, ref vec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// X 軸を回転軸にして行列を回転します。 (ラジアン単位)
        /// </summary>
        /// <param name="radian">回転角度 ( 軸方向に対して反時計回り )</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateX( float radian)
        {
            var result = API.LNMatrix_RotateX(ref this,  radian);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// Y 軸を回転軸にして行列を回転します。 (ラジアン単位)
        /// </summary>
        /// <param name="radian">回転角度</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateY( float radian)
        {
            var result = API.LNMatrix_RotateY(ref this,  radian);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// Z 軸を回転軸にして行列を回転します。 (ラジアン単位)
        /// </summary>
        /// <param name="radian">回転角度</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateZ( float radian)
        {
            var result = API.LNMatrix_RotateZ(ref this,  radian);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列を回転します。 (ラジアン単位)
        /// </summary>
        /// <param name="xRad">X 軸を回転軸とした角度</param>
        /// <param name="yRad">Y 軸を回転軸とした角度</param>
        /// <param name="zRad">Z 軸を回転軸とした角度</param>
        /// <param name="rotOrder">回転順序の指定</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Rotate( float xRad,  float yRad,  float zRad,  RotationOrder rotOrder = RotationOrder.XYZ)
        {
            var result = API.LNMatrix_Rotate(ref this,  xRad,  yRad,  zRad,  rotOrder);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列を回転します。 (ラジアン単位)
        /// </summary>
        /// <param name="vec">各軸の回転角度</param>
        /// <param name="rotOrder">回転順序の指定</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateVec3( Vector3 vec,  RotationOrder rotOrder = RotationOrder.XYZ)
        {
            var result = API.LNMatrix_RotateVec3(ref this, ref vec,  rotOrder);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 任意の軸を回転軸にして行列を回転します。 (ラジアン単位)
        /// </summary>
        /// <param name="axis">回転軸を示す Vector3</param>
        /// <param name="radian">回転角度</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// 						与えられた軸が正規化されていない場合は正規化したものを使って計算します。
        /// </remarks>
        public void RotateAxis( Vector3 axis,  float radian)
        {
            var result = API.LNMatrix_RotateAxis(ref this, ref axis,  radian);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// クォータニオンを使って行列を回転します。
        /// </summary>
        /// <param name="qua">処理の基になる Quaternion</param>
        /// <remarks>
        /// クォータニオンから回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateQuaternion( Quaternion qua)
        {
            var result = API.LNMatrix_RotateQuaternion(ref this, ref qua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列をスケーリングします。
        /// </summary>
        /// <param name="xyz">X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)</param>
        /// <remarks>
        /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Scale( float xyz)
        {
            var result = API.LNMatrix_Scale(ref this,  xyz);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列をスケーリングします。
        /// </summary>
        /// <param name="x">X 方向の拡大率</param>
        /// <param name="y">Y 方向の拡大率</param>
        /// <param name="z">Z 方向の拡大率</param>
        /// <remarks>
        /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void ScaleXYZ( float x,  float y,  float z)
        {
            var result = API.LNMatrix_ScaleXYZ(ref this,  x,  y,  z);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列をスケーリングします。
        /// </summary>
        /// <param name="scale">各方向の拡大率</param>
        /// <remarks>
        /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void ScaleVec3( Vector3 scale)
        {
            var result = API.LNMatrix_ScaleVec3(ref this, ref scale);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 行列を乗算します。
        /// </summary>
        /// <param name="mat1">処理の基になる行列</param>
        /// <param name="mat2">処理の基になる行列</param>
        public static Matrix Multiply( Matrix mat1,  Matrix mat2)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_Multiply(ref mat1, ref mat2, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 逆行列を求めます。
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Matrix Inverse( Matrix mat)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_Inverse(ref mat, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 転置行列を求めます。
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Matrix Transpose( Matrix mat)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_Transpose(ref mat, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 左手座標系ビュー行列を作成します。
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        public static Matrix ViewTransformLH( Vector3 pos,  Vector3 lookAt,  Vector3 upDir)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_ViewTransformLH(ref pos, ref lookAt, ref upDir, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 右手座標系ビュー行列を作成します。
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        public static Matrix ViewTransformRH( Vector3 pos,  Vector3 lookAt,  Vector3 upDir)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_ViewTransformRH(ref pos, ref lookAt, ref upDir, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 左手座標系射影行列を作成します。
        /// </summary>
        /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
        /// <param name="aspect">アスペクト比</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix PerspectiveFovLH( float fovY,  float aspect,  float nearZ,  float farZ)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_PerspectiveFovLH( fovY,  aspect,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 右手座標系射影行列作成します。
        /// </summary>
        /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
        /// <param name="aspect">アスペクト比</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix PerspectiveFovRH( float fovY,  float aspect,  float nearZ,  float farZ)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_PerspectiveFovRH( fovY,  aspect,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 左手座標系正射影行列作成します。
        /// </summary>
        /// <param name="width">ビューの幅</param>
        /// <param name="height">ビューの高さ</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix OrthoLH( float width,  float height,  float nearZ,  float farZ)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_OrthoLH( width,  height,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 右手座標系正射影行列作成します。
        /// </summary>
        /// <param name="width">ビューの幅</param>
        /// <param name="height">ビューの高さ</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix OrthoRH( float width,  float height,  float nearZ,  float farZ)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_OrthoRH( width,  height,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return matOut;
        }
        
        /// <summary>
        /// 回転行列からオイラー角を計算します。
        /// </summary>
        public Vector3 GetEulerAngles()
        {
            var outVec = new Vector3();
            var result = API.LNMatrix_GetEulerAngles(ref this, out outVec);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outVec;
        }
        
        /// <summary>
        /// 行列をスケーリング、回転、移動成分に分解します。
        /// </summary>
        /// <param name="scale">各軸のスケーリング量を格納する Vector3 変数</param>
        /// <param name="rot">各軸の回転情報を格納する Quaternion 変数</param>
        public Vector3 Decompose(out Vector3 scale, out Quaternion rot)
        {
            var trans = new Vector3();
            var result = API.LNMatrix_Decompose(ref this, out scale, out rot, out trans);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return trans;
        }
        
    
    };
    
    /// <summary>
    /// クォータニオンのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
        public float X;
        
        public float Y;
        
        public float Z;
        
        public float W;
        
    
    
        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Quaternion(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }
        
        /// <summary>
        /// クォータニオンに値を設定します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        /// <param name="w">W 値</param>
        public void Set( float x,  float y,  float z,  float w)
        {
            var result = API.LNQuaternion_Set(ref this,  x,  y,  z,  w);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
        }
        
        /// <summary>
        /// 単位クォータニオンを作成します。
        /// </summary>
        public static Quaternion Identity()
        {
            var qua = new Quaternion();
            var result = API.LNQuaternion_Identity(out qua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return qua;
        }
        
        /// <summary>
        /// 任意の軸を回転軸にして回転するクォータニオンを作成します。
        /// </summary>
        /// <param name="axis">回転軸を示す Vector3 変数</param>
        /// <param name="r">回転角度 (ラジアン)</param>
        /// <remarks>
        /// 与えられた軸が正規化されていない場合は正規化された軸を使用します。
        /// </remarks>
        public static Quaternion RotationAxis( Vector3 axis,  float r)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_RotationAxis(ref axis,  r, out outQua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outQua;
        }
        
        /// <summary>
        /// 回転行列からクォータニオンを作成します。
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Quaternion RotationMatrix( Matrix mat)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_RotationMatrix(ref mat, out outQua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outQua;
        }
        
        /// <summary>
        /// ヨー、ピッチ、およびロールを指定してクォータニオンを作成します。
        /// </summary>
        /// <param name="yaw">y 軸を中心とするヨー (ラジアン単位)</param>
        /// <param name="pitch">x 軸を中心とするピッチ (ラジアン単位)</param>
        /// <param name="roll">z 軸を中心とするロール (ラジアン単位)</param>
        public static Quaternion RotationYawPitchRoll( float yaw,  float pitch,  float roll)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_RotationYawPitchRoll( yaw,  pitch,  roll, out outQua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outQua;
        }
        
        /// <summary>
        /// クォータニオンを正規化します。
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        public static Quaternion Normalize( Quaternion qua)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Normalize(ref qua, out outQua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outQua;
        }
        
        /// <summary>
        /// クォータニオンの共役を計算します。
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        public static Quaternion Conjugate( Quaternion qua)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Conjugate(ref qua, out outQua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outQua;
        }
        
        /// <summary>
        /// 2 つのクォータニオンの積を計算します。
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        public static Quaternion Multiply( Quaternion qua1,  Quaternion qua2)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Multiply(ref qua1, ref qua2, out outQua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outQua;
        }
        
        /// <summary>
        /// 2 つのクォータニオンを球面線形補間します。
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        /// <param name="t">補間値</param>
        public static Quaternion Slerp( Quaternion qua1,  Quaternion qua2,  float t)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Slerp(ref qua1, ref qua2,  t, out outQua);
            if (result != Result.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outQua;
        }
        
    
    };
    

}
