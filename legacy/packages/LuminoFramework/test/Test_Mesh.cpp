#include "Common.hpp"

class Test_Mesh : public ::testing::Test {};

#if 0	// TODO: App の外側から Bone 更新できる仕組みを作らないとだめ

TEST_F(Test_Mesh, Basic)
{
    Engine::camera()->setPosition(0, 0, -5);

	auto mesh = SkinnedMesh::load(u"Mesh/SkinnedMesh2.glb");
    mesh->setRotation(0, Math::PI, 0);
    mesh->setShadingModel(ShadingModel::Unlit);

    auto model = mesh->skinnedMeshComponent()->model();
    auto skeleton = model->skeleton(0);
    auto node = model->meshNodes()[3];
    node->setRotation(0, 0, Math::PI / 2);

    TestEnv::updateFrame();
    ASSERT_SCREEN_S(LN_ASSETFILE("Mesh/Expects/Test_Mesh-Basic-1.png"));
    LN_TEST_CLEAN_SCENE;
}

#endif


#if 0
TEST_F(Test_Mesh, StaticMesh)
{
    Engine::camera()->setPosition(8, 8, -8);
    Engine::camera()->lookAt(0, 1, 0);

    auto mesh = StaticMesh::load(u"Mesh/SkinnedAxis1.glb");
    mesh->setShadingModel(ShadingModel::Unlit);

    TestEnv::updateFrame();
    ASSERT_SCREEN_S(LN_ASSETFILE("Mesh/Expects/Test_Mesh-StaticMesh-1.png"));
    LN_TEST_CLEAN_SCENE;
}



TEST_F(Test_Mesh, SkinnedMesh)
{
    //Engine::camera()->setPosition(5, 5, -5);
    //Engine::camera()->lookAt(0, 0, 0);
    Engine::camera()->setPosition(10, 5, 0);
    Engine::camera()->lookAt(5, 0, 5);


    auto mesh = SkinnedMesh::load(u"Mesh/SkinnedAxis1.glb");
    mesh->setPosition(5, 0, 5);                 // 少し移動して、WorldMatrix が適用できていることを確認する
    mesh->setShadingModel(ShadingModel::Unlit);

    TestEnv::updateFrame();
    ASSERT_SCREEN(LN_ASSETFILE("Mesh/Expects/Test_Mesh-SkinnedMesh-1.png"));
    LN_TEST_CLEAN_SCENE;
}


#endif
