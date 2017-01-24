
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_AUDIO_BEGIN

/**
	@brief		3D サウンドのリスナーのクラスです。
*/
class SoundListener
{
public:
	
	/**
		@brief		3D音声のリスナーの位置を設定します。
		@param[in]	position	: 3D 空間上の座標
	*/
	static void SetPosition(const Vector3& position);

	/**
		@brief		3D音声のリスナーの位置を設定します。
		@param[in]	x			: 3D 空間上の X 座標
		@param[in]	y			: 3D 空間上の Y 座標
		@param[in]	z			: 3D 空間上の Z 座標
	*/
	static void SetPosition(float x, float y, float z);
	
	/**
		@brief		3D音声のリスナーの位置を取得します。
	*/
	static const Vector3& GetPosition();

	/**
		@brief		3D音声のリスナーの正面方向を設定します。
		@param[in]	direction		: 向き
	*/
	static void SetDirection(const Vector3& direction);

	/**
		@brief		3D音声のリスナーの正面方向を設定します。
		@param[in]	x			: 向きの X 成分
		@param[in]	y			: 向きの Y 成分
		@param[in]	z			: 向きの Z 成分
	*/
	static void SetDirection(float x, float y, float z);

	/**
		@brief		3D音声のリスナーの正面方向を取得します。
	*/
	static const Vector3& GetDirection();

	/**
		@brief		3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
		@param[in]	direction	: 上方向
	*/
	static void SetUpDirection(const Vector3& direction);

	/**
		@brief		3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
		@param[in]	x			: 向きの X 成分
		@param[in]	y			: 向きの Y 成分
		@param[in]	z			: 向きの Z 成分
	*/
	static void SetUpDirection(float x, float y, float z);
	
	/**
		@brief		3D音声のリスナーの上方向を取得します。
	*/
	static const Vector3& GetUpDirection(const Vector3& direction);

	/**
		@brief		3D音声のリスナーの速度を設定します。
		@param[in]	velocity	: 速度
	*/
	static void SetVelocity(const Vector3& velocity);

	/**
		@brief		3D音声のリスナーの速度を設定します。
		@param[in]	x			: 速度の X 成分
		@param[in]	y			: 速度の Y 成分
		@param[in]	z			: 速度の Z 成分
	*/
	static void SetVelocity(float x, float y, float z);

	/**
		@brief		3D音声のリスナーの速度を設定します。
		@param[in]	velocity	: 速度
	*/
	static const Vector3& GetVelocity();

};

LN_NAMESPACE_AUDIO_END
LN_NAMESPACE_END
