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


    void setTransform(const Matrix& transform);
    const Matrix& transform() const { return m_transform; }

    /** 衝突グループを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroup(uint32_t value);

    /** 衝突グループマスクを設定します。デフォルトは 0x0000FFFF で、0～15番のグループと衝突することを示します。 */
    void setCollisionGroupMask(uint32_t value);

protected:
    void onDispose(bool explicitDisposing) override;
    void onPrepareStepSimulation() override;
    void onAfterStepSimulation() override;

LN_CONSTRUCT_ACCESS:
    TriggerBody();
    void init();
    void init(CollisionShape* shape);

private:
    void removeFromBtWorld() override;

    class LocalGhostObject;
    
    enum DirtyFlags
    {
        DirtyFlags_None = 0,

        DirtyFlags_InitialUpdate = 1 << 0,
        DirtyFlags_Shapes = 1 << 1,
        DirtyFlags_Group = 1 << 2,
        DirtyFlags_Transform = 1 << 3,

        DirtyFlags_All = 0xFFFF,
    };

    void createBtObject();
    void deleteBtObject();
    void readdToWorld();

    uint32_t m_dirtyFlags = DirtyFlags_All;
    uint32_t m_group = 0x00000001;
    uint32_t m_groupMask = 0x0000FFFF;
    Matrix m_transform;

    LocalGhostObject* m_btGhostObject = nullptr;
    bool m_btWorldAdded = false;

    detail::BtShapeManager m_shapeManager;
};

} // namespace ln

