// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#include "Common.hpp"
#include "Vector4.hpp"

namespace ln {
struct Vector3;
struct Quaternion;
struct AttitudeTransform;
struct Plane;

/**
 * 4x4 の行列を定義します。
 * 
 * 行列は数学的な定義とメモリレイアウト共に行優先 (Row-major) です。
 * 
 * このクラスは似た機能の static 関数とインスタンス関数があります。
 * 例えば makeRotationX(static 関数) と rotateX(インスタンス関数) です。
 * 前者は新しい行列を作成して返すのに対し、後者は現在の行列を変更します。
 * 例えば、以下の m1 と m2 は同じ結果になります。
 * 
 * ~~~
 * Matrix m1 = Matrix::makeRotationX(0.1) * Matrix::makeRotationY(0.2) * Matrix::makeRotationZ(0.3);
 * 
 * Matrix m2;
 * m2.rotateX(0.1);
 * m2.rotateY(0.2);
 * m2.rotateZ(0.3);
 * ~~~
 * 
 * なお、後者は行列の生成と乗算をまとめて行うように最適化されており、高速に動作します。
 */
LN_STRUCT()
struct LN_API Matrix
{
public:
#if defined(LUMINO_TRANSCODER)
    /** row0 */
    LN_FIELD()
    Vector4 row0;
    
    /** row1 */
    LN_FIELD()
    Vector4 row1;
    
    /** row2 */
    LN_FIELD()
    Vector4 row2;
    
    /** row3 */
    LN_FIELD()
    Vector4 row3;
#else
    union
    {
        struct
        {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
        };
        float m[4][4];
    };
#endif

    /** 単位行列 */
    static const Matrix Identity;

public:
    /** 単位行列を設定してインスタンスを初期化します。 */
    LN_METHOD(OverloadPostfix = "Zeros")
    Matrix();

    /** 各要素を指定してインスタンスを初期化します。 */
    LN_METHOD()
    Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    /** 各列を指定してインスタンスを初期化します。 */
    Matrix(const Vector4& row1, const Vector4& row2, const Vector4& row3, const Vector4& row4);

    /** AttitudeTransform から Matrix へ変換します。 */
    Matrix(const AttitudeTransform& transform);

    /** 各要素を設定します。 */
    void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);

    /** この行列が単位行列であるかを判定します。 */
    bool isIdentity() const;

    /** この行列の右方向を示すベクトルを取得します。 */
    const Vector3& right() const;

    /** この行列の上方向を示すベクトルを取得します。 */
    const Vector3& up() const;

    /** この行列の正面方向を示すベクトルを取得します。 */
    const Vector3& front() const;

    /** この行列の平行移動量を示すベクトルを取得します。 */
    const Vector3& position() const;

    /** この行列の行要素を設定します。 */
    void setRow(int index, const Vector4& row);

    /** この行列の行要素を取得します。 */
    const Vector4& row(int index) const;

#if !defined(LUMINO_TRANSCODER)
	/** Returns pointer to the first element. */
	float* data() LN_NOEXCEPT { return &m11; }

	/** Returns pointer to the first element. */
	const float* data() const LN_NOEXCEPT { return &m11; }
#endif

    /**
	 * この行列を平行移動します。
	 * @param[in]   x   : X 軸の移動量
	 * @param[in]   y   : Y 軸の移動量
	 * @param[in]   z   : Z 軸の移動量
      */
    void translate(float x, float y, float z);

    /**
	 * この行列を平行移動します。
	 * @param[in]   vec        : 移動量
	 */
    void translate(const Vector3& vec);

    /**
	 * この行列を X 軸で回転します。
	 * @param[in]   r   : 回転角度 (ラジアン単位)
	 */
    void rotateX(float r);

    /**
	 * この行列を Y 軸で回転します。
	 * @param[in]   r   : 回転角度 (ラジアン単位)
	 */
    void rotateY(float r);

    /**
	 * この行列を Z 軸で回転します。
	 * @param[in]   r   : 回転角度 (ラジアン単位)
	 */
    void rotateZ(float r);

    /**
	 * この行列を XYZ 各軸のオイラー角を指定して回転します。
	 * @param[in]   x        : X 軸の回転角度 (ラジアン単位)
	 * @param[in]   y        : Y 軸の回転角度 (ラジアン単位)
	 * @param[in]   z        : Z 軸の回転角度 (ラジアン単位)
	 * @param[in]   order    : 回転順序
	 */
    void rotateEulerAngles(float x, float y, float z, RotationOrder order = RotationOrder::ZXY);

    /**
     * この行列を XYZ 各軸のオイラー角を指定して回転します。
     * @param[in]   angles  : XYZ 各軸の回転角度 (ラジアン単位)
     * @param[in]   order   : 回転順序
     */
    void rotateEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder::ZXY);

    /**
     * この行列を任意軸で回転します。
     * @param[in]   axis    : 回転軸
     * @param[in]   r       : 回転角度 (ラジアン単位)
     * 
     * axis が単位ベクトルでない場合は正規化してから計算を行います。
     */
    void rotateAxis(const Vector3& axis, float r);

    /**
     * この行列を指定したクォータニオンで回転します。
     * @param[in]   qua     : 処理の基になるクォータニオン
     */
    void rotateQuaternion(const Quaternion& qua);

    /**
     * この行列を各軸にそってスケーリングします。
     * @param[in]   x       : X 軸の拡縮率
     * @param[in]   y       : Y 軸の拡縮率
     * @param[in]   z       : Z 軸の拡縮率
     */
    void scale(float x, float y, float z);

    /**
     * この行列を各軸にそってスケーリングします。
     * @param[in]   vec     : 各軸の拡縮率
     */
    void scale(const Vector3& vec);

    /**
     * この行列を各軸にそってスケーリングします。
     * @param[in]   xyz     : 全ての軸の拡縮率
     */
    void scale(float xyz);

    /**
     * この行列を逆行列化します。
     */
    void inverse();

    /**
     * この行列を転置します。
     */
    void transpose();

    /**
     * この行列をスケーリング、回転、移動要素に分解します。
     * @param[in]   scale       : スケーリング要素を格納するベクトルのポインタ
     * @param[in]   rotation    : 回転要素を格納するクォータニオンのポインタ
     * @param[in]   translation : 平行移動要素を格納するベクトルのポインタ
	 * 
     * 各引数は必要が無ければ NULL を指定できます。
     */
    void decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

    /**
     * この行列をスケーリング、回転、移動行列に分解します。
     * @param[in]   scale       : スケーリング行列を格納する変数のポインタ
     * @param[in]   rotation    : 回転行列を格納する変数のポインタ
     * @param[in]   translation : 平行移動行列を格納する変数のポインタ
	 * 
     * 各引数は必要が無ければ NULL を指定できます。
     */
    void decomposeMatrices(Matrix* scale, Matrix* rotation, Matrix* translation) const;

    /**
     * この回転行列を XYZ 各軸のオイラー角度に変換します。
     * @param[in]  order        : 回転順序
     * @param[in]  locked       : ジンバルロックの発生有無を格納する変数へのポインタ (ジンバルロックが発生した場合 true)
     * @return      XYZ 各軸のオイラー角度 (ラジアン単位) を格納したベクトル
	 * 
     * 回転行列は order に指定された順序で乗算されたものでなければ本来の回転角度が取得できないことに注意してください。
	 * 
     * 例えば RotationYawPitchRoll() は、回転軸 Z → Y → X の順に回転を行いますが、
     * これから元の角度を取り出すには RotationOrder_ZXY を指定します。
     */
    Vector3 toEulerAngles(RotationOrder order = RotationOrder::ZXY, bool* locked = nullptr) const;

    /**
     * この行列の行要から回転行列を取り出します。
	 * 
     * 分解ではなく、単純に3x3の部分をコピーします。
     * それ以外の要素は単位行列の値で初期化されます。
     */
    Matrix getRotationMatrix() const;

    /**
     * 要素のいずれかが NaN または Inf かを判別します。
     */
    bool isNaNOrInf() const;

	/** 行列の要素にアクセスします。 */
	const float& operator()(int row, int column) const;

	/** 行列の要素にアクセスします。 */
	float & operator()(int row, int column);

    String toString() const;

public:
    /**
     * 2つの行列の積を計算します。
     * @param[in]   mat1    : 処理の基になる行列
     * @param[in]   mat2    : 処理の基になる行列
     * @return      演算結果の行列
     */
    static Matrix multiply(const Matrix& mat1, const Matrix& mat2);

    /**
     * 平行移動行列を作成します。
     * @param[in]   x        : X 軸の移動量
     * @param[in]   y        : Y 軸の移動量
     * @param[in]   z        : Z 軸の移動量
     * @return      演算結果の行列
     */
    static Matrix makeTranslation(float x, float y, float z);

    /**
     * 平行移動行列を作成します。
     * @param[in]   vec     : 移動量
     * @return      演算結果の行列
     */
    static Matrix makeTranslation(const Vector3& vec);

    /**
     * X 軸を回転軸にして回転する行列を作成します。
     * @param[in]   r       : 回転角度 (ラジアン単位)
     * @return      演算結果の行列
     */
    static Matrix makeRotationX(float r);

    /**
     * Y 軸を回転軸にして回転する行列を作成します。
     * @param[in]   r       : 回転角度 (ラジアン単位)
     * @return      演算結果の行列
     */
    static Matrix makeRotationY(float r);

    /**
     * Z 軸を回転軸にして回転する行列を作成します。
     * @param[in]   r       : 回転角度 (ラジアン単位)
     * @return      演算結果の行列
     */
    static Matrix makeRotationZ(float r);

    /**
     * 回転軸と角度を指定して行列を作成します。
     * @param[in]   axis     : 回転軸
     * @param[in]   r        : 回転角度 (ラジアン単位)
     * @return      演算結果の行列
	 * 
     * axis が単位ベクトルでない場合は正規化してから計算を行います。
     */
    static Matrix makeRotationAxis(const Vector3& axis, float r);

    /**
     * クォータニオンから回転行列を作成します。
     * @param[in]   qua      : 処理の基になるクォータニオン
     * @return      演算結果の行列
     */
    static Matrix makeRotationQuaternion(const Quaternion& qua);

    /**
     * XYZ 各軸のオイラー角から回転行列を作成します。
     * @param[in]   x       : X 軸の回転角度 (ラジアン単位)
     * @param[in]   y       : Y 軸の回転角度 (ラジアン単位)
     * @param[in]   z       : Z 軸の回転角度 (ラジアン単位)
     * @param[in]   order   : 回転順序
     * @return      演算結果の行列
     */
    static Matrix makeRotationEulerAngles(float x, float y, float z, RotationOrder order = RotationOrder::ZXY);

    /**
     * XYZ 各軸のオイラー角から回転行列を作成します。
     * @param[in]   angles  : XYZ 各軸の回転角度 (ラジアン単位)
     * @param[in]   order   : 回転順序
     * @return      演算結果の行列
     */
    static Matrix makeRotationEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder::ZXY);

    /**
     * ヨー、ピッチ、ロールを指定して行列を作成します。
     * @param[in]   yaw     : y 軸を中心とするヨー (ラジアン単位)
     * @param[in]	pitch	: x 軸を中心とするピッチ (ラジアン単位)
     * @param[in]	roll	: z 軸を中心とするロール (ラジアン単位)
     * @return      演算結果の行列
	 * 
     * 回転順序は Z → X → Y です。引数の並びとは逆順であることに注意してください。
     * これは  D3DXMatrixRotationYawPitchRoll の回転順序と同じで、
     *  RotationEulerAngles に RotationOrder_ZXY を指定して計算した結果とも同じです。
     */
    static Matrix makeRotationYawPitchRoll(float yaw, float pitch, float roll);

    /**
     * 各軸にそってスケーリングする行列を作成します。
     * @param[in]   x   : X 軸の拡縮率
     * @param[in]   y   : Y 軸の拡縮率
     * @param[in]   z   : Z 軸の拡縮率
     * @return      演算結果の行列
     */
    static Matrix makeScaling(float x, float y, float z);

    /**
     * 各軸にそってスケーリングする行列を作成します。
     * @param[in]   vec : 各軸の拡縮率
     * @return      演算結果の行列
     */
    static Matrix makeScaling(const Vector3& vec);

    /**
     * 各軸にそってスケーリングする行列を作成します。
     * @param[in]   xyz : 全ての軸の拡縮率
     * @return      演算結果の行列
     */
    static Matrix makeScaling(float xyz);

    /**
     * 指定された行列の逆行列を作成します。
     * @param[in]   mat : 処理の基になる行列
     * @return      作成された逆行列
     */
    static Matrix makeInverse(const Matrix& mat);

    /**
     * 指定された行列の転置行列を作成します。
     * @param[in]   mat : 処理の基になる行列
     * @return      作成された転置行列
     */
    static Matrix makeTranspose(const Matrix& mat);

    /**
     * 平面に対して座標系を反転するための行列を作成します。
     * @param[in]   plane   : 処理の基になる平面
     * @return      演算結果の行列
	 * 
     * plane は正規化してから計算を行います。
     */
    static Matrix makeReflection(const Plane& plane);

    /**
     * 左手座標系ビュー行列を作成します。
     * @param[in]   position    : 視点の位置
     * @param[in]   lookAt      : 注視点
     * @param[in]   up          : 上方向
     * @return      演算結果の行列
     */
    static Matrix makeLookAtLH(const Vector3& position, const Vector3& lookAt, const Vector3& up);

    /**
     * 右手座標系ビュー行列を作成します。
     * @param[in]   position    : 視点の位置
     * @param[in]   lookAt      : 注視点
     * @param[in]   up          : 上方向
     * @return      演算結果の行列
     */
    static Matrix makeLookAtRH(const Vector3& position, const Vector3& lookAt, const Vector3& up);

    /**
     * 左手座標系パースペクティブ射影行列を作成します。
     * @param[in]   fovY        : Y 方向への視野角 (ラジアン単位)
     * @param[in]   aspect      : アスペクト比 (横幅/高さ)
     * @param[in]   near        : 近いビュー平面の Z 値
     * @param[in]   far         : 遠いビュー平面の Z 値
     * @return      演算結果の行列
     */
    static Matrix makePerspectiveFovLH(float fovY, float aspect, float near, float far);

    /**
     * 右手座標系パースペクティブ射影行列を作成します。
     * @param[in]   fovY        : Y 方向への視野角 (ラジアン単位)
     * @param[in]   aspect      : アスペクト比 (横幅/高さ)
     * @param[in]   near        : 近いビュー平面の Z 値
     * @param[in]   far         : 遠いビュー平面の Z 値
     * @return      演算結果の行列
     */
    static Matrix makePerspectiveFovRH(float fovY, float aspect, float near, float far);

    /**
     * 左手座標系正射影行列を作成します。
     * @param[in]   width       : スクリーンの幅
     * @param[in]   height      : スクリーンの高さ
     * @param[in]   near        : 一番近いビュー平面の Z 値
     * @param[in]   far         : 一番遠いビュー平面の Z 値
     * @return      演算結果の行列
     */
    static Matrix makeOrthoLH(float width, float height, float near, float far);

    /**
     * 右手座標系正射影行列を作成します。
     * @param[in]   width       : スクリーンの幅
     * @param[in]   height      : スクリーンの高さ
     * @param[in]   near        : 一番近いビュー平面の Z 値
     * @param[in]   far         : 一番遠いビュー平面の Z 値
     * @return      演算結果の行列
     */
    static Matrix makeOrthoRH(float width, float height, float near, float far);

    /**
     * 2D 座標系用の左手座標系正射影行列を作成します。
     * @param[in]   width       : スクリーンの幅
     * @param[in]   height      : スクリーンの高さ
     * @param[in]   near        : 一番近いビュー平面の Z 値
     * @param[in]   far         : 一番遠いビュー平面の Z 値
     * @return      演算結果の行列
	 * 
     * ビューの左上を原点とし、右方向を X+、下方向を Y+、手前方向を Z+ とします。
     */
    static Matrix makePerspective2DLH(float width, float height, float near, float far);

    /**
     * 2D 座標系用の右手座標系正射影行列を作成します。
     * @param[in]   width       : スクリーンの幅
     * @param[in]   height      : スクリーンの高さ
     * @param[in]   near        : 一番近いビュー平面の Z 値
     * @param[in]   far         : 一番遠いビュー平面の Z 値
     * @return      演算結果の行列
	 * 
     * ビューの左上を原点とし、右方向を X+、下方向を Y+、奥方向を Z+ とします。
     */
    static Matrix makePerspective2DRH(float width, float height, float near, float far);

    /**
     * アフィン変換行列を作成します。
     * @param[in]   scaling         : スケーリング係数
     * @param[in]   rotationCenter  : 回転の中心位置
     * @param[in]   rotation        : 回転
     * @param[in]   translation     : 平行移動
     * @return      演算結果の行列
     */
    static Matrix makeAffineTransformation(const Vector3& scaling, const Vector3& rotationCenter, const Quaternion& rotation, const Vector3& translation);

    /** アフィン変換行列 から回転成分を抽出します。 */
    static Matrix extractRotation(const Matrix& mat);

    static Matrix makeAffineLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);

public:
    Matrix& operator*=(const Matrix& mat);

    friend Matrix operator*(const Matrix& mat1, const Matrix& mat2);
    friend Matrix operator*(const Matrix& mat1, float v);

    bool operator==(const Matrix& mat) const;
    bool operator!=(const Matrix& mat) const;

#ifdef LN_MATH_MATRIX_EXTENSION
    LN_MATH_MATRIX_EXTENSION
#endif
};

#if !defined(LUMINO_TRANSCODER)
inline const Vector3& Matrix::right() const
{
    return (Vector3&)m[0][0];
}

inline const Vector3& Matrix::up() const
{
    return (Vector3&)m[1][0];
}

inline const Vector3& Matrix::front() const
{
    return (Vector3&)m[2][0];
}

inline const Vector3& Matrix::position() const
{
    return (Vector3&)m[3][0];
}

inline void Matrix::setRow(int index, const Vector4& row)
{
    ((Vector4&)m[index][0]) = row;
}

inline const Vector4& Matrix::row(int index) const
{
    return (Vector4&)m[index][0];
}

inline std::ostream& operator<<(std::ostream& s, const Matrix& v)
{
	return detail::MathHelper::outputStream(s, v.data(), 16);
}
#endif

} // namespace ln
