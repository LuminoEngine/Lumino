
#ifndef LUMINO_MATH_VECTOR2_H
#define LUMINO_MATH_VECTOR2_H

#include <stdio.h>
#include "Common.h"

LN_NAMESPACE_BEGIN

class Vector4;
class Matrix;

/**
	@brief	2次元のベクトルを定義します。
*/
class LUMINO_EXPORT Vector2
{
public:

	float	x;		///< X 要素
	float	y;		///< Y 要素

public:

    static const Vector2   Zero;		///< Vector2(0, 0)
    static const Vector2   UnitX;		///< Vector2(1, 0)
    static const Vector2   UnitY;		///< Vector2(0, 1)
	static const Vector2   Ones;		///< Vector2(1, 1)

public:

	/**
		@brief		すべての要素を 0.0 に設定してインスタンスを初期化します。
	*/
    Vector2();

	/**
		@brief		指定した値を使用してインスタンスを初期化します。
	*/
	Vector2(float x, float y);

public:

	/**
		@brief		各要素に値を設定します。
	*/
	void Set(float x, float y);

	/**
		@brief		このベクトルの長さを返します。
	*/
	float GetLength() const;

	/**
		@brief		このベクトルの長さの 2 乗を返します。
	*/
	float GetLengthSquared() const;

	/**
		@brief		このベクトルを正規化します。
		@details	ベクトルの長さが 0 の場合は正規化を行いません。
	*/
    void Normalize();

	/**
		@brief		指定された行列を使用してこのベクトルを座標変換します。
		@param[in]	mat		: 座標変換行列
		@details	ベクトルを (X, Y, 0.0, 1.0) として座標変換を行い、結果を w で除算します。
	*/
	void TransformCoord(const Matrix& mat);
	
	/**
		@brief		要素のいずれかが NaN または Inf かを判別します。
	*/
	bool IsNaNOrInf() const;

	/**
		@brief		デバッグ用に文字列を標準出力します。
		@param[in]	format	: 書式指定文字列
		@param[in]	stream	: 出力先ストリーム
		@details	format が NULL の場合、書式は "%f, %f\n" を使用します。
	*/
	void Print(const char* format = NULL, FILE* stream = NULL) const;

public:

	/**
		@brief		指定ベクトルを正規化したベクトルを返します。
		@param[in]	vec		: 処理の基になるベクトル
		@return		正規化されたベクトル
	*/
	static Vector2 Normalize(const Vector2& vec);

	/**
		@brief		2つのベクトルの内積を計算します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		2つのベクトルの内積
	*/
	static float Dot(const Vector2& vec1, const Vector2& vec2);

	/**
		@brief		2つのベクトルの最小値で構成されるベクトルを返します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		最小値から作成されたベクトル
	*/
	static Vector2 Min(const Vector2& vec1, const Vector2& vec2);

	/**
		@brief		2つのベクトルの最大値で構成されるベクトルを返します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		最大値から作成されたベクトル
	*/
	static Vector2 Max(const Vector2& vec1, const Vector2& vec2);

	/**
		@brief		指定された行列を使用してベクトルを座標変換します。
		@param[in]	vec		: 処理の基になるベクトル
		@param[in]	mat		: 処理の基になる行列
		@return		変換されたベクトル
	*/
	static Vector4 Transform(const Vector2& vec, const Matrix& mat);

	/**
		@brief		指定された行列を使用してベクトルを座標変換します。
		@param[in]	vec		: 処理の基になるベクトル
		@param[in]	mat		: 処理の基になる行列
		@details	ベクトルを (X, Y, 0.0, 1.0) として座標変換を行い、結果を w で除算します。
		@return		変換されたベクトル
	*/
	static Vector2 TransformCoord(const Vector2& vec, const Matrix& mat);

	/**
		@brief		2 つのベクトル間の線形補間を行います。
		@param[in]	start	: 開始ベクトル (t = 0.0 のときの値)
		@param[in]	end		: 終了ベクトル (t = 1.0 のときの値)
		@param[in]	t		: 加重係数
		@return		補間結果のベクトル
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);

	/**
		@brief		指定されたベクトルを使用して エルミートスプライン補間を実行します。
		@param[in]	v1	: 開始ベクトル
		@param[in]	a1	: 開始ベクトルの接線ベクトル(速度)
		@param[in]	v2	: 終了ベクトル
		@param[in]	a2	: 終了ベクトルの接線ベクトル(速度)
		@param[in]	t	: 加重係数
		@return		補間結果の値
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static Vector2 Hermite(const Vector2& v1, const Vector2& a1, const Vector2& v2, const Vector2& a2, float t);

	/**
		@brief		指定されたベクトルを使用して Catmull-Rom 補間を行います。
		@param[in]	vec1	: 1番目の位置
		@param[in]	vec2	: 2番目の位置 (t = 0.0 のときの値)
		@param[in]	vec3	: 3番目の位置 (t = 1.0 のときの値)
		@param[in]	vec4	: 4番目の位置
		@param[in]	t		: 加重係数
		@return		補間結果のベクトル
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static Vector2 CatmullRom(const Vector2& vec1, const Vector2& vec2, const Vector2& vec3, const Vector2& vec4, float t);

public:

	Vector2& operator += (const Vector2& v);
	Vector2& operator += (float v);
	Vector2& operator -= (const Vector2& v);
	Vector2& operator -= (float v);
	Vector2& operator *= (const Vector2& v);
	Vector2& operator *= (float v);
	Vector2& operator /= (const Vector2& v);
	Vector2& operator /= (float v);

	friend Vector2 operator + (const Vector2& v1,	const Vector2& v2);
	friend Vector2 operator + (const Vector2& v1,	float v2);
	friend Vector2 operator + (float v1,			const Vector2& v2);
	friend Vector2 operator - (const Vector2& v1,	const Vector2& v2);
	friend Vector2 operator - (const Vector2& v1,	float v2);
	friend Vector2 operator - (float v1,			const Vector2& v2);
	friend Vector2 operator * (const Vector2& v1,	const Vector2& v2);
	friend Vector2 operator * (const Vector2& v1,	float v2);
	friend Vector2 operator * (float v1,			const Vector2& v2);
	friend Vector2 operator / (const Vector2& v1,	const Vector2& v2);
	friend Vector2 operator / (const Vector2& v1,	float v2);
	friend Vector2 operator / (float v1,			const Vector2& v2);

	friend Vector2 operator - (const Vector2& v1);

	bool operator == (const Vector2& v) const;
	bool operator != (const Vector2& v) const;

};

LN_NAMESPACE_END

#include "Vector2.inl"

#endif // LUMINO_MATH_VECTOR2_H
