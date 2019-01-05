
#include "Internal.hpp"
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <LuminoEngine/Mesh/Mesh.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/SoftBody.hpp>

namespace ln {

//==============================================================================
// SoftBody
// http://slis.tsukuba.ac.jp/~fujisawa.makoto.fu/lecture/iml/text/5_softbody.html
    // http://zerogram.info/?p=1581

static const float DEFAULT_CONFIG_VALUE = 0.1f;
static const float DEFAULT_CONFIG_PR = 1.0f;
static const float DEFAULT_COLLISION_MARGIN = 0.01f;

SoftBody::SoftBody()
    : PhysicsObject(PhysicsObjectType::SoftBody)
    , m_mass(1.0f)
    , m_group(1)
    , m_groupMask(0xffffffff)
    , configLST_(DEFAULT_CONFIG_VALUE)
    , configMT_(DEFAULT_CONFIG_VALUE)
    , configVC_(DEFAULT_CONFIG_VALUE)
    , configPR_(DEFAULT_CONFIG_PR)
{
}

SoftBody::~SoftBody()
{
}

void SoftBody::initialize()
{
    PhysicsObject::initialize();
}

void SoftBody::createFromMesh(MeshResource* mesh, PhysicsWorld* world)
{
    {
        const btScalar s = 6;
        const btScalar h =0;
        const int r = 10;
        const btVector3 p[] = { btVector3(+s, h, -s),
                               btVector3(-s, h, -s),
                               btVector3(+s, h, +s),
                               btVector3(-s, h, +s) };
        btSoftBody* psb = btSoftBodyHelpers::CreatePatch(*world->softBodyWorldInfo(), p[0], p[1], p[2], p[3], r, r, 1 + 2 + 4 + 8, true);
        world->getBtWorld()->addSoftBody(psb);
        psb->m_cfg.piterations = 1;
        psb->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//SB同士のコリジョン
    }

    {
        //TRACEDEMO
        btSoftBody* psb = btSoftBodyHelpers::CreateEllipsoid(*world->softBodyWorldInfo(), btVector3(0, 5, 0),
            btVector3(1, 1, 1) * 2,
            64);
        psb->m_materials[0]->m_kLST = 0.45;
        psb->m_cfg.kVC = 20;
        psb->setTotalMass(25, false);
        psb->setPose(true, false);
        psb->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//SB同士のコリジョン
        world->getBtWorld()->addSoftBody(psb);

        //Ctor_BigPlate(pdemo);
        //Ctor_LinearStair(pdemo, btVector3(0, 0, 0), btVector3(2, 1, 5), 0, 10);
        //pdemo->m_autocam = true;
    }

    return;


    std::vector<btScalar> btVertices;
    std::vector<int> btIndices;



    unsigned numVertices = mesh->vertexCount();
    btVertices.resize(numVertices * 3);
    for (int i = 0; i < numVertices; i++)
    {
        const Vertex& v = mesh->vertex(i);
        btVertices[i * 3 + 0] = (btScalar)v.position.x;
        btVertices[i * 3 + 1] = (btScalar)v.position.y;
        btVertices[i * 3 + 2] = (btScalar)v.position.z;
    }

    unsigned numIndices = mesh->indexCount();
    btIndices.resize(numIndices);
    for (int i = 0; i < numIndices; i++)
    {
        btIndices[i] = mesh->index(i);
    }
    //for (int i = 0; i < numIndices / 3; i++)
    //{
    //    std::swap(btIndices[i * 3 + 1], btIndices[i * 3 + 2]);
    //}



    // create
    m_body.reset(btSoftBodyHelpers::CreateFromTriMesh(*world->softBodyWorldInfo(), btVertices.data(), btIndices.data(), (int)numIndices / 3));

    //const btScalar s = 6;
    //const btScalar h = 2;
    //const int r = 16;
    //const btVector3 p[] = { btVector3(+s, h, -s),
    //                       btVector3(-s, h, -s),
    //                       btVector3(+s, h, +s),
    //                       btVector3(-s, h, +s) };
    //m_body.reset(btSoftBodyHelpers::CreatePatch(*world->softBodyWorldInfo(), p[0], p[1], p[2], p[3], r, r, 1 + 2 + 4 + 8, true));


    // set default config
    setDefaultConfiguration();

    //btSoftBody::Material* pm = m_body->appendMaterial();
    //pm->m_kLST = 0.4;
    //pm->m_flags -= btSoftBody::fMaterial::DebugDraw;
    //m_body->generateBendingConstraints(2, pm);

    // void SoftBody::ApplyClothSetting()
    //{

    //    m_body->generateBendingConstraints(2);

    //    m_body->m_cfg.kLF = 0.05f;
    //    m_body->m_cfg.kDG = 0.01f;
    //}

    // void SoftBody::CopyNodesTransform()
    //{

    //    SetTransform(node_->GetPosition(), node_->GetRotation());
    //    node_->SetPosition(Vector3::ZERO);
    //    node_->SetRotation(Quaternion::IDENTITY);
    //}

    // void SoftBody::UpdateMass()
    if (m_mass > 0.0f)
    {
        m_body->setTotalMass(m_mass, true);
    }

    //m_body->setVolumeMass(m_mass);
    //m_body->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS;

    ///pass zero in generateClusters to create  cluster for each tetrahedron or triangle
    m_body->generateClusters(8);
    //psb->m_materials[0]->m_kLST=.2;
    m_body->m_cfg.kDF = 10.;

    //btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)physicsWorld()->getBtWorld();
    btSoftRigidDynamicsWorld* worldw = world->getBtWorld();//physicsWorld()->getBtWorld();
    worldw->addSoftBody(m_body.get()/*, m_group, m_groupMask*/);
    //inWorld_ = true;





}

void SoftBody::setDefaultConfiguration()
{
    // minimum default settings just to keep the volume, some what, intact
    m_body->m_materials[0]->m_kLST = (btScalar)configLST_; // Linear stiffness coefficient [0,1]
    m_body->m_cfg.kMT = (btScalar)configMT_;  // Pose matching coefficient [0,1]
    m_body->m_cfg.kVC = (btScalar)configVC_;  // Volume conservation coefficient [0,+inf]
    m_body->m_cfg.kPR = (btScalar)configPR_;  // Pressure coefficient [-inf,+inf]

    m_body->m_cfg.piterations = 4;     // ばねによる位置修正の最大反復回数
    m_body->m_materials[0]->m_kLST = 0.5; // 剛性(Linear Stiffness Coefficient) (変形のしやすさ)
    //btSoftBody::fMaterial::DebugDraw;

    m_body->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//SB同士のコリジョン

    m_body->setPose(true, true);
    m_body->getCollisionShape()->setMargin(DEFAULT_COLLISION_MARGIN);
}

} // namespace ln

