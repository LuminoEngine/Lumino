
#include "Internal.h"
#include "EffectDriver.h"
#include "EffectManager.h"
#include <Lumino/Effect/VisualEffect.h>
#include <Lumino/Effect/VisualEffectInstance.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// VisualEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(VisualEffect, tr::ReflectionObject);

//------------------------------------------------------------------------------
//VisualEffectPtr VisualEffect::create(const StringRef& filePath)
//{
//	auto* manager = detail::GetEffectManager(nullptr);
//	auto* obj = manager->GetEffectEngine()->CreateEffectCore(PathName(filePath));
//	return VisualEffectPtr(obj, false);
//}

//------------------------------------------------------------------------------
VisualEffect::~VisualEffect()
{
	ReleaseInstance();
}

//------------------------------------------------------------------------------
void VisualEffect::initialize(/*detail::EffectCore* core*/)
{
	//LN_REFOBJ_SET(m_core, core);
}

//------------------------------------------------------------------------------
void VisualEffect::ReleaseInstance()
{
	for (auto* inst : m_instanceList)
	{
		inst->release();
	}
	m_instanceList.clear();
}

//------------------------------------------------------------------------------
VisualEffectInstance* VisualEffect::Play()
{
	if (m_overlapEffects)
	{
		Stop();
	}

	VisualEffectInstance* inst = PlayNewInstance();
	m_instanceList.push_back(inst);
	return inst;
}

//------------------------------------------------------------------------------
void VisualEffect::Stop()
{
	for (auto* inst : m_instanceList)
	{
		inst->Stop();
	}
	ReleaseInstance();
}

//------------------------------------------------------------------------------
bool VisualEffect::IsPlaying() const
{
	for (auto* inst : m_instanceList)
	{
		if (inst->IsPlaying())
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------
void VisualEffect::SetOverlapEffects(bool enabled)
{
	m_overlapEffects = enabled;
}

//------------------------------------------------------------------------------
void VisualEffect::SetSyncEffects(bool enabled)
{
	m_syncEffects = enabled;
}

//------------------------------------------------------------------------------
void VisualEffect::SetWorldMatrix(const Matrix& matrix)
{
	m_worldMatrix = matrix;
	if (m_syncEffects)
	{
		for (auto* inst : m_instanceList)
		{
			inst->SetWorldMatrix(m_worldMatrix);
		}
	}
}

//==============================================================================
// VisualEffectInstance
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(VisualEffectInstance, tr::ReflectionObject);

LN_NAMESPACE_END
