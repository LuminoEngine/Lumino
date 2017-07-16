
#ifndef LUMINO_MATH_VECTOR4_H
#define LUMINO_MATH_VECTOR4_H

#include <stdio.h>
#include "Common.h"

LN_NAMESPACE_BEGIN
struct Vector2;
struct Vector3;
struct Matrix;

/**
	@brief	4次元のベクトルを定義します。
*/
struct LUMINO_EXPORT Vector4
{
public:

	float	x;		///< X 要素
	float	y;		///< Y 要素
	float	z;		///< Z 要素
	float	w;		///< W 要素

public:
	static const Vector4   Zero;		///< Vector4(0, 0, 0, 0)

public:

	/**
		@brief		すべての要素を 0.0 に設定してインスタンスを初期化します。
	*/
	Vector4();

	/**
		@brief		指定した値を使用してインスタンスを初期化します。
	*/
	Vector4(float x, float y, float z, float w);
	
	/**
		@brief		Vector2 と z, w 値を指定してインスタンスを初期化します。
	*/
	Vector4(const Vector2& vec, float z, float w);

	/**
		@brief		Vector3 と w 値を指定してインスタンスを初期化します。
	*/
	Vector4(const Vector3& vec, float w);

public:

	/**
		@brief		各要素に値を設定します。
	*/
	void set(float x, float y, float z, float w);
	
	/**
		@brief		このベクトルの X Y 要素を Vector2 として返します。
	*/
	const Vector2& getXY() const;
	
	/**
		@brief		このベクトルの X Y Z 要素を Vector3 として返します。
	*/
	const Vector3& GetXYZ() const;

	/**
		@brief		ベクトルの長さを返します。
	*/
	float getLength() const;

	/**
		@brief		ベクトルの長さの 2 乗を返します。
	*/
	float getLengthSquared() const;

	/**
		@brief		このベクトルを指定された最大値と最小値の範囲にクランプします。
	*/
	void clamp(float min_xyzw, float max_xyzw);

	/**
		@brief		このベクトルを指定された最大値と最小値の範囲にクランプします。
		@param[in]	minVec	: 最小値
		@param[in]	maxVec	: 最大値
	*/
	void clamp(const Vector4& minVec, const Vector4& maxVec);
	
	/**
		@brief		要素のいずれかが NaN または Inf かを判別します。
	*/
	bool isNaNOrInf() const;

	/**
		@brief		デバッグ用に文字列を標準出力します。
		@param[in]	format	: 書式指定文字列
		@param[in]	stream	: 出力先ストリーム
		@details	format が NULL の場合、書式は "%f, %f, %f, %f\n" を使用します。
	*/
	void print(const char* format = NULL, FILE* stream = NULL) const;

public:

	/**
		@brief		指定ベクトルを正規化したベクトルを返します。
		@param[in]	vec		: 処理の基になるベクトル
		@return		正規化されたベクトル
	*/
	static Vector4 normalize(const Vector4& vec);

	/**
		@brief		2つのベクトルの内積を計算します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		2つのベクトルの内積
	*/
	static float dot(const Vector4& vec1, const Vector4& vec2);

	/**
		@brief		2つのベクトルの最小値で構成されるベクトルを返します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		最小値から作成されたベクトル
	*/
	static Vector4 min(const Vector4& vec1, const Vector4& vec2);

	/**
		@brief		2つのベクトルの最大値で構成されるベクトルを返します。
		@param[in]	vec1	: 処理の基になるベクトル
		@param[in]	vec2	: 処理の基になるベクトル
		@return		最大値から作成されたベクトル
	*/
	static Vector4 max(const Vector4& vec1, const Vector4& vec2);

	/**
		@brief		指定された行列を使用してベクトルを座標変換します。
		@param[in]	vec		: 処理の基になるベクトル
		@param[in]	mat		: 処理の基になる行列
		@return		変換されたベクトル
	*/
	static Vector4 transform(const Vector4& vec, const Matrix& mat);

	/**
		@brief		2 つのベクトル間の線形補間を行います。
		@param[in]	start	: 開始ベクトル (t = 0.0 のときの値)
		@param[in]	end		: 終了ベクトル (t = 1.0 のときの値)
		@param[in]	t		: 加重係数
		@return		補間結果のベクトル
		@details	t は通常、0.0～1.0 を指定します。
	*/
	static Vector4 lerp(const Vector4& start, const Vector4& end, float t);

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
	static Vector4 hermite(const Vector4& v1, const Vector4& a1, const Vector4& v2, const Vector4& a2, float t);

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
	static Vector4 catmullRom(const Vector4& vec1, const Vector4& vec2, const Vector4& vec3, const Vector4& vec4, float t);

public:

	Vector4& operator += (const Vector4& v);
	Vector4& operator += (float v);
	Vector4& operator -= (const Vector4& v);
	Vector4& operator -= (float v);
	Vector4& operator *= (const Vector4& v);
	Vector4& operator *= (float v);
	Vector4& operator /= (const Vector4& v);
	Vector4& operator /= (float v);

	friend Vector4 operator + (const Vector4& v1,	const Vector4& v2);
	friend Vector4 operator + (const Vector4& v1,	float v2);
	friend Vector4 operator + (float v1,			const Vector4& v2);
	friend Vector4 operator - (const Vector4& v1,	const Vector4& v2);
	friend Vector4 operator - (const Vector4& v1,	float v2);
	friend Vector4 operator - (float v1,			const Vector4& v2);
	friend Vector4 operator * (const Vector4& v1,	const Vector4& v2);
	friend Vector4 operator * (const Vector4& v1,	float v2);
	friend Vector4 operator * (float v1,			const Vector4& v2);
	friend Vector4 operator / (const Vector4& v1,	const Vector4& v2);
	friend Vector4 operator / (const Vector4& v1,	float v2);
	friend Vector4 operator / (float v1,			const Vector4& v2);

	friend Vector4 operator - (const Vector4& v1);

	bool operator == (const Vector4& v) const;
	bool operator != (const Vector4& v) const;
};

LN_NAMESPACE_END

#include "Vector4.inl"

#endif // LUMINO_MATH_VECTOR4_H
