
#pragma once
#include <list>
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief		視覚効果オブジェクトを操作する機能を提供します。
*/
class VisualEffect
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		指定したファイルから VisualEffect のインスタンスを作成します。
		@param[in]	filePath	: エフェクトファイル
		@details	対応しているエフェクトファイルは Effekseer のファイル (.efk) です。
	*/
	static VisualEffectPtr Create(const StringRef& filePath);

public:

	/**
		@brief		エフェクトの再生を開始します。
		@param[in]	overlap	: trueの場合は再生中のエフェクトがあれば停止せずに新しいエフェクトの再生を開始し、false の場合は停止してから開始する。
		@return		開始されたエフェクトを示すクラス
	*/
	VisualEffectInstance* Play(bool overlap);

	/**
		@brief		このオブジェクトから再生されたエフェクトを停止します。
	*/
	void Stop();

	/**
		@brief		このオブジェクトから再生されたエフェクトのいずれかが再生中かを確認します。
	*/
	bool IsPlaying() const;
	
	/**
		@brief		再生中のエフェクトの姿勢 (ワールド変換行列) をこのオブジェクトと同期するかを設定します。
		@param[in]	enabled	: true の場合、同期する (デフォルト true)
	*/
	void SetSyncEffects(bool enabled);

	/**
		@brief		このオブジェクトの姿勢 (ワールド変換行列) を設定します。
	*/
	void SetWorldMatrix(const Matrix& matrix);

protected:
	VisualEffect() = default;
	virtual ~VisualEffect() = default;
	void Initialize(detail::EffectCore* core);

private:
	detail::EffectCore*					m_core = nullptr;
	std::list<VisualEffectInstance*>	m_instanceList;
	Matrix								m_worldMatrix;
	bool								m_syncEffects = true;
};


LN_NAMESPACE_END
