
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace detail { class PhysicsWorld; }

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

private:
	RefPtr<detail::PhysicsWorld>	m_impl;
};

LN_NAMESPACE_END
