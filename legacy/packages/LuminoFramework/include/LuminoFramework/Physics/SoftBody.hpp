#pragma once
#include <memory>
#include "PhysicsObject.hpp"

namespace ln {
class PhysicsWorld;
class MeshResource;

class SoftBody
	: public PhysicsObject
{
public:
    void setTransform(const Matrix& transform);

    void setMass(float mass);



    int nodeCount() const;

    Vector3 nodePosition(int nodeIndex) const;
    Vector3 nodeVelocity(int nodeIndex) const;

    /** 指定したインデックスのノードの質量を設定します。0 を設定すると、静的なノードとなります。 */
    void setNodeMass(int nodeIndex, float mass);


    // material, ↓各剛性係数。変形しやすさ。

    /** Linear stiffness coefficient [0,1], default:1.0 */
    void setLinearStiffness(float value);

    /** Area/Angular stiffness coefficient [0,1], default:1.0 */
    void setAngularStiffness(float value);

    /** Volume stiffness coefficient [0,1], default:1.0 */
    void setVolumeStiffness(float value);


    /** Pose matching coefficient [0,1] (setPoseの効果) */
    void setPoseMatching(float value);


    /** default:0.25 */
    void setCollisionMargin(float value);


    // TODO: internal
    void createFromMesh(MeshResource* mesh, PhysicsWorld* world);
    bool raycast(const Vector3& from, const Vector3& to, Vector3* outHitPosition, Vector3* outHitNormal) const;

LN_CONSTRUCT_ACCESS:
    SoftBody();
    virtual ~SoftBody();
    void init();

private:
    void setDefaultConfiguration();

    std::unique_ptr<btSoftBody> m_body;
    float m_mass;
    uint32_t m_group;
    uint32_t m_groupMask;

    float m_LST;
    float m_AST;
    float m_VST;

    float m_collisionMargin;

    /// Linear stiffness coefficient [0,1]
    float configLST_;
    /// 
    float m_MT;
    /// Volume conservation coefficient [0,+inf]
    float configVC_;
    /// Pressure coefficient [-inf,+inf]
    float configPR_;
    //float configDP_;
    //float configCHR_;


    friend class PhysicsWorld;
};

} // namespace ln

