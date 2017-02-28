
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace detail { class PhysicsWorldCore; }

/**
	@brief	
*/
class PhysicsWorld
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:


LN_CONSTRUCT_ACCESS:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void Initialize();

LN_INTERNAL_ACCESS:
	detail::PhysicsWorldCore* GetImpl() const;
	void StepSimulation(float elapsedTime);

private:
	RefPtr<detail::PhysicsWorldCore>	m_impl;
};

LN_NAMESPACE_END
