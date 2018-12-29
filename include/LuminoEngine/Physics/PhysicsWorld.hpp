#pragma once
#include <memory>
#include "Common.hpp"

namespace ln {
class PhysicsDebugRenderer;

class PhysicsWorld
	: public Object
{
public:

LN_CONSTRUCT_ACCESS:
    PhysicsWorld();
	virtual ~PhysicsWorld();
	void initialize();
    virtual void dispose() override;

private:
    class IDebugRenderer
    {
    public:
        virtual void drawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor) = 0;
    };

    btDefaultCollisionConfiguration*		m_btCollisionConfig;
    btCollisionDispatcher*					m_btCollisionDispatcher;
    btDbvtBroadphase*						m_btBroadphase;
    //btAxisSweep3*							m_btBroadphase;
    btSequentialImpulseConstraintSolver*	m_btSolver;
    btDiscreteDynamicsWorld*				m_btWorld;
    //btSoftRigidDynamicsWorld*				m_btWorld;
    btGhostPairCallback*					m_btGhostPairCallback;
    btSoftBodyWorldInfo*					m_softBodyWorldInfo;
    std::unique_ptr<PhysicsDebugRenderer> m_debugRenderer;
};

} // namespace ln

