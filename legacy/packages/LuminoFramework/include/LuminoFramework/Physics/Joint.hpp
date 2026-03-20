#pragma once
#include "Common.hpp"

namespace ln {

/**
 * Physics joint.
 */
class Joint
    : public Object
{
public:
    /** この Joint が含まれている PhysicsWorld を取得します。 */
    PhysicsWorld* physicsWorld() const { return m_world; }

    /** この Joint が含まれている PhysicsWorld からこの Joint を除外します。 */
    void removeFromPhysicsWorld();

LN_CONSTRUCT_ACCESS:
    Joint();

private:
    virtual void removeFromBtWorld() = 0;

    PhysicsWorld* m_world;
    bool m_removing;

    friend class PhysicsWorld;
};

} // namespace ln
