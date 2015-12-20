
#pragma once
#include "../Animation/AnimationState.h"
#include "ModelCore.h"

LN_NAMESPACE_BEGIN

/// ボーンのインスタンスクラス
class ModelBone
	: public RefObject
	, public Animation::IAnimationTargetAttribute
{
public:
	ModelBone();
	virtual ~ModelBone();

public:

	/// 作成
	void Create(ModelBoneCore* core);

	/// ModelBoneCore の取得
	ModelBoneCore* GetCore() { return m_core; }

	/// 子ボーンの追加
	void AddChildBone(ModelBone* bone) { m_children.Add(bone); }

	/// ボーン行列を階層的に更新する
	void UpdateTransformHierarchy(const Matrix& parentMatrix);

	///  結合済み行列 (モデル内のグローバル行列) の取得
	const Matrix& GetCombinedMatrix() const { return m_combinedMatrix; }

	/// ローカル行列を初期値に戻す
	void ResetLocalTransform() { m_localTransform = SQTTransform::Identity; }

protected:
	// override Animation::IAnimationTargetAttribute
	virtual const String& GetAnimationTargetName() const { return m_core->Name; }
	virtual void SetAnimationTargetValue(Animation::ValueType type, const void* value);

private:
	RefPtr<ModelBoneCore>	m_core;				///< 共有データクラス
	ModelBoneList			m_children;			///< 子ボーンリスト
	SQTTransform			m_localTransform;	///< モーションを書き込むのはここ
	Matrix					m_combinedMatrix;	///< 結合済み行列 (モデル内のグローバル行列)
};

LN_NAMESPACE_END

