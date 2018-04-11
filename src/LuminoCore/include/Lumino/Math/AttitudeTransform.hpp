
#ifndef LUMINO_MATH_SQTTRANSFORM_H
#define LUMINO_MATH_SQTTRANSFORM_H

#include "Common.hpp"
#include "Vector3.hpp"
#include "Quaternion.hpp"

namespace ln {

/**
	@brief		位置、回転、スケールを定義します。
*/
struct LN_API AttitudeTransform
{
public:

	Vector3		scale;          ///< 拡大
	Quaternion	rotation;       ///< 回転
	Vector3		translation;    ///< 位置
	
public:
	
	static const AttitudeTransform   Identity;  ///< (Vector3::One, Quaternion::Identity, Vector3::Zero)
	
public:
	
	/**
		@brief		各要素を 拡縮1.0、単位クォータニオン、位置(0, 0) でインスタンスを初期化します。
	*/
	AttitudeTransform();

	/**
		@brief		各要素を指定してインスタンスを初期化します。
	*/
	AttitudeTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
	
};

} // namespace ln

#endif // LUMINO_MATH_SQTTRANSFORM_H
