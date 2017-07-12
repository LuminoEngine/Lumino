
#ifndef LUMINO_MATH_QUATERNION_H
#define LUMINO_MATH_QUATERNION_H

#include <stdio.h>
#include "Common.h"

LN_NAMESPACE_BEGIN
class Vector3;
class Matrix;

/**
	@brief		クォータニオンを定義します。

	@details	このクォータニオンクラスの乗算の順番は左から右です。
				つまり、q1 * q2 は、先に q1、次に q2 の回転を適用する事と同じです。

				この乗算順序は標準的な四元数の定義からは逆行していますが、
				ゲームプログラムをメインとした3Dグラフィックス向けの線形代数ライブラリとしては
				この動作がほぼ一般的なので、これに合わせています。<br>
				(DirectX(D3DX)、XNA、Unity、cocos2dx 等はこの動作)
*/
class LUMINO_EXPORT Quaternion
{
public:

	float	x;		///< X 要素
	float	y;		///< Y 要素
	float	z;		///< Z 要素
	float	w;		///< W 要素

public:

	static const Quaternion Identity;	///< 単位クォータニオン (0, 0, 0, 1)

public:

	/**
		@brief		単位クォータニオンを設定してインスタンスを初期化します。
	*/
	Quaternion();

	/**
		@brief		指定した値を使用してインスタンスを初期化します。
	*/
	Quaternion(float x, float y, float z, float w);
	
	/**
		@brief		回転軸と回転角度を指定してインスタンスを初期化します。
		@param[in]  axis	: 回転軸
		@param[in]  r		: 回転角度 (ラジアン単位)
		@details	axis が単位ベクトルでなければ正規化してから計算を行います。
	*/
	Quaternion(const Vector3& axis, float r);

public:

	/**
		@brief		各要素に値を設定します。
	*/
	void set(float x, float y, float z, float w);

	/**
		@brief		このクォータニオンの長さを返します。
	*/
	float getLength() const;

	/**
		@brief		このクォータニオンの長さの 2 乗を返します。
	*/
	float getLengthSquared() const;
	
	/**
		@brief		このクォータニオンを正規化します。
	*/
	void normalize();
	
	/**
		@brief		このクォータニオンが単位クォータニオンであるかを判定します。
		@return		true の場合、単位クォータニオンである
	*/
	bool isIdentity() const;

	/**
		@brief		このクォータニオンに指定されたクォータニオンを乗算します。
		@param[in]	qua		: 乗算するクォータニオン
	*/
	void multiply(const Quaternion& qua);

	/**
		@brief		このクォータニオンを X 軸で回転します。
		@param[in]	angle	: 回転角度 (ラジアン単位)
	*/
	void rotateX(float angle);

	/**
		@brief		このクォータニオンを Y 軸で回転します。
		@param[in]	angle	: 回転角度 (ラジアン単位)
	*/
	void rotateY(float angle);

	/**
		@brief		このクォータニオンを Z 軸で回転します。
		@param[in]	angle	: 回転角度 (ラジアン単位)
	*/
	void rotateZ(float angle);
	
	/**
		@brief		このクォータニオンを回転軸と角度を指定して回転します。
		@param[in]  axis	: 回転軸
		@param[in]  r		: 回転角度 (ラジアン単位)
		@details	axis が単位ベクトルでなければ正規化してから計算を行います。
	*/
	void rotateAxis(const Vector3& axis, float r);

	/**
		@brief		このクォータニオンを XYZ 各軸のオイラー角度に変換します。
		@param[in]  order		: 回転順序
		@param[in]  locked		: ジンバルロックの発生有無を格納する変数へのポインタ (ジンバルロックが発生した場合 true)
		@return		XYZ 各軸のオイラー角度 (ラジアン単位) を格納したベクトル
		@details	クォータニオンは order に指定された順序で乗算されたものでなければ本来の回転角度が取得できないことに注意してください。<br>
					例えば RotationYawPitchRoll() は、回転軸 Z → Y → X の順に回転を行いますが、
					これから元の角度を取り出すには RotationOrder_ZXY を指定します。
	*/
	Vector3 toEulerAngles(RotationOrder order = RotationOrder::ZXY, bool* locked = nullptr) const;

	/**
		@brief		このクォータニオンの回転軸と回転角度を取得します。
		@param[in]	axis		: 回転軸を格納するベクトルのポインタ
		@param[in]	angle		: 回転角度を格納する変数のポインタ
		@details	axis には単位ベクトルを格納します。
	*/
	void toAxisAngle(Vector3* axis, float* angle) const;
	
	/**
		@brief		要素のいずれかが NaN または Inf かを判別します。
	*/
	bool isNaNOrInf() const;

	/**
		@brief		デバッグ用に文字列を標準出力します。
		@param[in]	format	: 書式指定文字列
		@param[in]	stream	: 出力先ストリーム
		@details	format が NULL の場合、書式は "(%f, %f, %f, %f)\n" を使用します。
	*/
	void print(const char* format = nullptr, FILE* stream = nullptr) const;

public:

	/**
		@brief		指定クォータニオンを正規化したクォータニオンを返します。
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		正規化されたクォータニオン
	*/
    static Quaternion normalize(const Quaternion& qua);
	
	/**
		@brief		指定クォータニオンの共役を返します。
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		共役クォータニオン
	*/
    static Quaternion conjugate(const Quaternion& qua);
	
	/**
		@brief		2つのクォータニオンの内積を計算します。
		@param[in]	qua1	: 処理の基になるクォータニオン
		@param[in]	qua2	: 処理の基になるクォータニオン
		@return		2つのクォータニオンの内積
	*/
	static float dot(const Quaternion& qua1, const Quaternion& qua2);

	/**
		@brief		2つのクォータニオンの積を計算します。
		@param[in]	qua1	: 処理の基になるクォータニオン
		@param[in]	qua2	: 処理の基になるクォータニオン
		@details	Multiply(qua1, qua2) は、先に qua1、次に qua2 の回転を行うクォータニオンを作成します。
	*/
	static Quaternion multiply(const Quaternion& qua1, const Quaternion& qua2);
	
	/**
		@brief		クォータニオンを共役し、再正規化します。
		@param[in]	qua		: 処理の基になるクォータニオン
		@return		逆数クォータニオン
	*/
	static Quaternion makeInverse(const Quaternion& qua);

	/**
		@brief		回転軸と角度を指定してクォータニオンを作成します。
		@param[in]  axis	: 回転軸
		@param[in]  r		: 回転角度 (ラジアン単位)
		@return		新しく作成されたクォータニオン
		@details	axis が単位ベクトルでなければ正規化してから計算を行います。
	*/
	static Quaternion makeFromRotationAxis(const Vector3& axis, float r);
	
	/**
		@brief		回転行列からクォータニオンを作成します。
		@param[in]  mat		: 処理の基になる行列
		@return		新しく作成されたクォータニオン
	*/
	static Quaternion makeFromRotationMatrix(const Matrix& mat);
	
	/**
		@brief		ヨー、ピッチ、ロールを指定してクォータニオンを作成します。
		@param[in]	yaw		: y 軸を中心とするヨー (ラジアン単位)
		@param[in]	pitch	: x 軸を中心とするピッチ (ラジアン単位)
		@param[in]	roll	: z 軸を中心とするロール (ラジアン単位)
		@return		新しく作成されたクォータニオン
		@details	回転順序は Z → X → Y です。引数の並びとは逆順であることに注意してください。
					これは  D3DXQuaternionRotationYawPitchRoll の回転順序と同じで、
					RotationEulerAngles に RotationOrder_ZXY を指定して計算した結果とも同じです。
	*/
	static Quaternion makeFromYawPitchRoll(float yaw, float pitch, float roll);

	/**
		@brief		XYZ 各軸のオイラー角からクォータニオンを作成します。
		@param[in]  angles	: XYZ 各軸の回転角度 (ラジアン単位)
		@param[in]  order	: 回転順序
		@return		演算結果のクォータニオン
	*/
	static Quaternion makeFromEulerAngles(const Vector3& angles, RotationOrder order = RotationOrder::ZXY);

	/**
		@brief		球面線形補間を使用して、2 つのクオータニオン間を補間します。
		@param[in]	qua1	: 処理の基になるクオータニオン (t = 0.0 のときの値)
		@param[in]	qua2	: 処理の基になるクオータニオン (t = 1.0 のときの値)
		@param[in]	t		: 加重係数
		@return		補間結果のクォータニオン
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static Quaternion slerp(const Quaternion& qua1, const Quaternion& qua2, float t);

	/**
		@brief		指定した方向を向くように回転するクオータニオンを作成します。
		@param[in]	forward	: 向かせたい方向
		@param[in]	up		: 上方向
		@return		演算結果のクォータニオン
	*/
	static Quaternion lookRotation(const Vector3& forward, const Vector3& up = Vector3::UnitY);

public:
	
	Quaternion& operator += (const Quaternion& v);
	Quaternion& operator += (float v);
	Quaternion& operator -= (const Quaternion& v);
	Quaternion& operator -= (float v);
	Quaternion& operator *= (const Quaternion& v);	///< this->Multiply(v); と同じです。
	Quaternion& operator *= (float v);
	Quaternion& operator /= (float v);

	friend Quaternion operator + (const Quaternion& v1,	const Quaternion& v2);
	friend Quaternion operator + (const Quaternion& v1,	float v2);
	friend Quaternion operator + (float v1,				const Quaternion& v2);
	friend Quaternion operator - (const Quaternion& v1,	const Quaternion& v2);
	friend Quaternion operator - (const Quaternion& v1,	float v2);
	friend Quaternion operator - (float v1,				const Quaternion& v2);
	friend Quaternion operator * (const Quaternion& v1, const Quaternion& v2);	///< Quaternion::Multiply(v1, v2); と同じです。
	friend Quaternion operator * (const Quaternion& v1,	float v2);
	friend Quaternion operator * (float v1,				const Quaternion& v2);
	friend Quaternion operator / (const Quaternion& v1,	float v2);

	friend Quaternion operator - (const Quaternion& v1);

	bool operator == (const Quaternion& v) const;
	bool operator != (const Quaternion& v) const;

};

LN_NAMESPACE_END

#include "Quaternion.inl"

#endif // LUMINO_MATH_QUATERNION_H
