
#include "Internal.hpp"
#pragma warning(disable: 5033)	// disable warning in bullet headers
#include <bullet/BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <bullet/BulletSoftBody/btSoftBodyHelpers.h>
#include <bullet/BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Physics/PhysicsWorld.hpp>
#include <LuminoEngine/Physics/SoftBody.hpp>
#include "BulletUtils.hpp"

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
    , m_LST(1.0f)
    , m_AST(1.0f)
    , m_VST(1.0f)
    , m_collisionMargin(0.25f)
    , configLST_(DEFAULT_CONFIG_VALUE)
    , m_MT(0.0f)
    , configVC_(DEFAULT_CONFIG_VALUE)
    , configPR_(DEFAULT_CONFIG_PR)
{
}

SoftBody::~SoftBody()
{
}

void SoftBody::init()
{
    PhysicsObject::init();
}

void SoftBody::setTransform(const Matrix& transform)
{
    m_body->transform( detail::BulletUtil::LNMatrixToBtTransform(transform));
}

void SoftBody::setMass(float mass)
{
    m_mass = std::max(mass, 0.0f);
    // TODO: readdToWorld
    //m_body->setTotalMass(mass);
}

void SoftBody::setLinearStiffness(float value)
{
    m_LST = Math::clamp01(value);
}

void SoftBody::setAngularStiffness(float value)
{
    m_AST = Math::clamp01(value);
}

void SoftBody::setVolumeStiffness(float value)
{
    m_VST = Math::clamp01(value);
}

void SoftBody::setPoseMatching(float value)
{
    m_MT = Math::clamp01(value);
}

void SoftBody::setCollisionMargin(float value)
{
    m_collisionMargin = value;
}

int SoftBody::nodeCount() const
{
    return m_body->m_nodes.size();
}

Vector3 SoftBody::nodePosition(int nodeIndex) const
{
    return detail::BulletUtil::btVector3ToLNVector3(m_body->m_nodes[nodeIndex].m_x);
}

Vector3 SoftBody::nodeVelocity(int nodeIndex) const
{
    return detail::BulletUtil::btVector3ToLNVector3(m_body->m_nodes[nodeIndex].m_v);
}

void SoftBody::setNodeMass(int nodeIndex, float mass)
{
    m_body->setMass(nodeIndex, mass);
}

void SoftBody::createFromMesh(MeshResource* mesh, PhysicsWorld* world)
{
    //{
    //    const btScalar s = 6;
    //    const btScalar h =0;
    //    const int r = 10;
    //    const btVector3 p[] = { btVector3(+s, h, -s),
    //                           btVector3(-s, h, -s),
    //                           btVector3(+s, h, +s),
    //                           btVector3(-s, h, +s) };
    //    btSoftBody* psb = btSoftBodyHelpers::CreatePatch(*world->softBodyWorldInfo(), p[0], p[1], p[2], p[3], r, r, 1 + 2 + 4 + 8, true);
    //    world->getBtWorld()->addSoftBody(psb);
    //    psb->m_cfg.piterations = 1;
    //    psb->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//SB同士のコリジョン
    //}

    //{
    //    //TRACEDEMO
    //    btSoftBody* psb = btSoftBodyHelpers::CreateEllipsoid(*world->softBodyWorldInfo(), btVector3(0, 5, 0),
    //        btVector3(1, 1, 1) * 2,
    //        64);
    //    psb->m_materials[0]->m_kLST = 0.45;
    //    psb->m_cfg.kVC = 20;
    //    psb->setTotalMass(25, false);
    //    psb->setPose(true, false);
    //    psb->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//SB同士のコリジョン
    //    world->getBtWorld()->addSoftBody(psb);

    //    //Ctor_BigPlate(pdemo);
    //    //Ctor_LinearStair(pdemo, btVector3(0, 0, 0), btVector3(2, 1, 5), 0, 10);
    //    //pdemo->m_autocam = true;
    //}

    //return;


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
    for (int i = 0; i < numIndices / 3; i++)
    {
        std::swap(btIndices[i * 3 + 1], btIndices[i * 3 + 2]);
    }



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
    m_body->generateBendingConstraints(2);
    // void SoftBody::CopyNodesTransform()
    //{

    //    SetTransform(node_->GetPosition(), node_->GetRotation());
    //    node_->SetPosition(Vector3::ZERO);
    //    node_->SetRotation(Quaternion::IDENTITY);
    //}

    // void SoftBody::UpdateMass()

    //m_body->setVolumeMass(m_mass);
    //m_body->m_cfg.collisions = btSoftBody::fCollision::CL_SS + btSoftBody::fCollision::CL_RS;

    ///pass zero in generateClusters to create  cluster for each tetrahedron or triangle
    m_body->generateClusters(8);
    //psb->m_materials[0]->m_kLST=.2;
    //m_body->m_cfg.kDF = 10.;

    //btSoftRigidDynamicsWorld* world = (btSoftRigidDynamicsWorld*)physicsWorld()->getBtWorld();
    btSoftRigidDynamicsWorld* worldw = world->getBtWorld();//physicsWorld()->getBtWorld();
    worldw->addSoftBody(m_body.get()/*, m_group, m_groupMask*/);
    //inWorld_ = true;


    //btSoftBody::sRayCast result;
    //bool r = m_body->rayTest(btVector3(0.02, 0.02, -10), btVector3(0.02, 0.02, 10), result);
    //Vector3 pos = Vector3::lerp(Vector3(0.02, 0.02, -10), Vector3(0.02, 0.02, 10), result.fraction);

}

bool SoftBody::raycast(const Vector3& from, const Vector3& to, Vector3* outHitPosition, Vector3* outHitNormal) const
{
    if (Vector3::nearEqual(from, to)) return false;

    btSoftBody::sRayCast result;
    bool r = m_body->rayTest(
        detail::BulletUtil::LNVector3ToBtVector3(from),
        detail::BulletUtil::LNVector3ToBtVector3(to),
        result);
    if (outHitPosition) {
        *outHitPosition = Vector3::lerp(from, to, result.fraction);
    }

    if (outHitNormal) {
        if (result.feature == btSoftBody::eFeature::Face) {
            *outHitNormal = detail::BulletUtil::btVector3ToLNVector3(m_body->m_faces[result.index].m_normal);
        }
    }
    return r;
}

void SoftBody::setDefaultConfiguration()
{
    // btSoftBody のコンストラクタで Material はひとつ作られる。
    // Material は Node や Link など、Feature をベースとした各データクラスが参照する。
    // 各データ Add 時に指定するか、指定しない場合は m_materials[0] が使われる。

    // minimum default settings just to keep the volume, some what, intact
    //m_body->m_materials[0]->m_kLST = 0.45;//(btScalar)configLST_; // Linear stiffness coefficient [0,1]
    m_body->m_cfg.kMT = m_MT;// (btScalar)configMT_;  // Pose matching coefficient [0,1]
    //m_body->m_cfg.kVC = 20;// (btScalar)configVC_;  // Volume conservation coefficient [0,+inf]
    //m_body->m_cfg.kPR = (btScalar)configPR_;  // Pressure coefficient [-inf,+inf]

    //m_body->m_cfg.piterations = 1;     // ばねによる位置修正の最大反復回数
    m_body->m_materials[0]->m_kLST = m_LST;//0.5; // 剛性(Linear Stiffness Coefficient) (変形のしやすさ)
    m_body->m_materials[0]->m_kAST = m_AST;
    m_body->m_materials[0]->m_kVST = m_VST;
    //btSoftBody::fMaterial::DebugDraw;

    //m_body->m_cfg.collisions |= btSoftBody::fCollision::VF_SS;//SB同士のコリジョン

    if (m_mass > 0.0f)
    {
        m_body->setTotalMass(m_mass, true);
    }

    m_body->setPose(true, true);
    m_body->getCollisionShape()->setMargin(m_collisionMargin);
}

} // namespace ln

