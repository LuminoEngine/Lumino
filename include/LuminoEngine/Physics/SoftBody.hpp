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

    // TODO: internal
    void createFromMesh(MeshResource* mesh, PhysicsWorld* world);

LN_CONSTRUCT_ACCESS:
    SoftBody();
    virtual ~SoftBody();
    void initialize();

private:
    void setDefaultConfiguration();

    std::unique_ptr<btSoftBody> m_body;
    float m_mass;
    uint32_t m_group;
    uint32_t m_groupMask;

    /// Linear stiffness coefficient [0,1]
    float configLST_;
    /// Pose matching coefficient [0,1]
    float configMT_;
    /// Volume conservation coefficient [0,+inf]
    float configVC_;
    /// Pressure coefficient [-inf,+inf]
    float configPR_;
    //float configDP_;
    //float configCHR_;


    friend class PhysicsWorld;
};

} // namespace ln

