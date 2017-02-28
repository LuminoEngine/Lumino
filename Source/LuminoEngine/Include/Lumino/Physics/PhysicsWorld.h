
#pragma once
#include "../MultiThreadingObjectList.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
namespace detail { class PhysicsWorldCore; }
class RigidBody;

/**
	@brief	
*/
class PhysicsWorld
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	void AddRigidBody(RigidBody* rigidBody);
	void RemoveRigidBody(RigidBody* rigidBody);

LN_CONSTRUCT_ACCESS:
	PhysicsWorld();
	virtual ~PhysicsWorld();
	void Initialize();

LN_INTERNAL_ACCESS:
	detail::PhysicsWorldCore* GetImpl() const;
	void StepSimulation(float elapsedTime);

	// AutoAdd interface
	void AutoAddChild(RigidBody* child);

private:
	RefPtr<detail::PhysicsWorldCore>		m_impl;
	MultiThreadingInFrameGCList<RigidBody>	m_rigidBodyList;
};

LN_NAMESPACE_END
