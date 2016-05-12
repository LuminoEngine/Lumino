
#include "../Internal.h"
#include "ModelBone.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// ModelBone
//==============================================================================

//------------------------------------------------------------------------------
ModelBone::ModelBone()
{
}

//------------------------------------------------------------------------------
ModelBone::~ModelBone()
{
}

//------------------------------------------------------------------------------
void ModelBone::Create(ModelBoneCore* core)
{
	m_core = core;
}

//------------------------------------------------------------------------------
void ModelBone::UpdateTransformHierarchy(const Matrix& parentMatrix)
{
	// m_localTransform は、ボーンのローカル姿勢でアニメーションが適用されている。
	// 適用されていなければ Identity。
	m_combinedMatrix = m_localTransform;

	// 親からの平行移動量
	m_combinedMatrix.Translate(m_core->GetOffsetFromParent());

	// 親行列と結合
	m_combinedMatrix *= parentMatrix;

	// 子ボーン更新
	LN_FOREACH(ModelBone* bone, m_children)
	{
		bone->UpdateTransformHierarchy(m_combinedMatrix);
	}
}

//------------------------------------------------------------------------------
void ModelBone::SetAnimationTargetValue(ValueType type, const void* value)
{
	LN_CHECK_ARG(type == ValueType_SQTTransform);
	m_localTransform = *((SQTTransform*)value);
}

LN_NAMESPACE_END

