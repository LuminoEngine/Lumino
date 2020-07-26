#pragma once
#include "Common.hpp"
#include "PhysicsObject.hpp"
#include "CollisionShape.hpp"

namespace ln {

class TriggerBody
	: public PhysicsObject
{
public:
    static Ref<TriggerBody> create(CollisionShape* shape);

    void addCollisionShape(CollisionShape* shape);


    /** 衝突グループを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroup(uint32_t value);

    /** 衝突グループマスクを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroupMask(uint32_t value);

    /** 位置を設定します。(default: 0, 0, 0) */
    void setPosition(const Vector3& value);



protected:
    void onBeforeStepSimulation() override;
    void onAfterStepSimulation() override;

LN_CONSTRUCT_ACCESS:
    TriggerBody();
    void init();
    void init(CollisionShape* shape);

private:
    enum DirtyFlags
    {
        DirtyFlags_None = 0,
        DirtyFlags_Shapes = 1 << 0,
        DirtyFlags_Group = 1 << 1,
        DirtyFlags_All = 0xFFFF,
    };
    uint32_t m_dirtyFlags;
    uint32_t m_group;
    uint32_t m_groupMask;
    Matrix m_transform;


    detail::BtShapeManager m_shapeManager;
};

} // namespace ln

