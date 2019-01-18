#pragma once
#include <memory>
#include "Common.hpp"

namespace ln {
class RenderingContext;
class PhysicsObject;
namespace detail {
class PhysicsDebugRenderer;
}

class PhysicsWorld
	: public Object
{
public:
    void addPhysicsObject(PhysicsObject* physicsObject);

    // TODO: 衝突コールバック内から呼ばれるとNG。
    // destory() でマーク付けて後で GC する仕組みにしておく。
    // ※前は addingList, removingList 作ってたけど複雑すぎた。
    void removePhysicsObject(PhysicsObject* physicsObject);

public: // TODO: internal
    //btDiscreteDynamicsWorld* getBtWorld() { return m_btWorld; }
    btSoftRigidDynamicsWorld* getBtWorld() { return m_btWorld; }
    btSoftBodyWorldInfo* softBodyWorldInfo() const { return m_softBodyWorldInfo; }
    void stepSimulation(float elapsedSeconds);
    void renderDebug(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
    PhysicsWorld();
	virtual ~PhysicsWorld();
	void initialize();
    virtual void dispose() override;

private:
    void addObjectInternal(PhysicsObject* obj);
    void removeObjectInternal(PhysicsObject* obj);

    btDefaultCollisionConfiguration*		m_btCollisionConfig;
    btCollisionDispatcher*					m_btCollisionDispatcher;
    btDbvtBroadphase*						m_btBroadphase;
    //btAxisSweep3*							m_btBroadphase;
    btSequentialImpulseConstraintSolver*	m_btSolver;
    //btDiscreteDynamicsWorld*				m_btWorld;
    btSoftRigidDynamicsWorld*				m_btWorld;
    btGhostPairCallback*					m_btGhostPairCallback;
    btSoftBodyWorldInfo*					m_softBodyWorldInfo;
    std::unique_ptr<detail::PhysicsDebugRenderer> m_debugRenderer;

    List<Ref<PhysicsObject>> m_physicsObjectList;
};

} // namespace ln

