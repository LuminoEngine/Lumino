#include "TestEnv.hpp"

class Test_Rendering_Mesh : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        TestEnv::initializeRendering();
    }
};

TEST_F(Test_Rendering_Mesh, Basic) {
    RESET_VIEW();
    TestEnv::renderView->resetView(Vector3(8, 8, 8), Vector3(0, 1, 0));

    auto model1 = MeshModel::load(LN_ASSETFILE("Mesh/StaticAxis1.glb"));
    auto instance1 = model1->createMeshModelInstance();

    auto proxy = makeURef<MeshBatchProxy>();
    proxy->modelInstance = instance1;

    auto* commandList = TestEnv::swapChain->beginFrame2();
    auto* target = TestEnv::swapChain->currentBackbuffer();
    auto* ctx = TestEnv::renderView->begin(commandList, target);
    {
        ctx->drawBatchProxy(proxy);

        //for (const auto& node : model1->meshNodes()) {
        //    if (node->meshContainerIndex() >= 0) {

        //        ctx->setTransfrom(model1->nodeGlobalTransform(node->index()));

        //        const auto& meshContainer = model1->meshContainers()[node->meshContainerIndex()];
        //        for (const auto& meshPrimitive : meshContainer->meshPrimitives()) {

        //            for (int iSection = 0; iSection < meshPrimitive->sections().size(); iSection++) {
        //                int materialIndex = meshPrimitive->sections()[iSection].materialIndex;
        //                if (materialIndex >= 0) {
        //                    ctx->setMaterial(model1->materials()[materialIndex]);

        //                    ctx->drawMesh(meshPrimitive, iSection);
        //                }
        //            }
        //        }
        //    }
        //}
    }
    TestEnv::renderView->end();
    TestEnv::swapChain->endFrame();

    ASSERT_RENDERTARGET(LN_ASSETFILE("Rendering/Expects/Test_Rendering_Mesh.Basic.png"), target);
}

TEST_F(Test_Rendering_Mesh, SkinnedMesh1) {
    RESET_VIEW();

    auto model1 = MeshModel::load(LN_ASSETFILE("Mesh/SkinnedMesh2.glb"));
    auto instance1 = model1->createMeshModelInstance();

    auto proxy = makeURef<MeshBatchProxy>();
    proxy->modelInstance = instance1;

    auto* commandList = TestEnv::swapChain->beginFrame2();
    auto* target = TestEnv::swapChain->currentBackbuffer();
    auto* ctx = TestEnv::renderView->begin(commandList, target);
    {
        instance1->updateFrame(0.016);

        // Manual update
        auto skeleton = instance1->skeletons()[0];
        auto node = skeleton->bones()[1]->node();
        node->setRotation(0, 0, Math::PI / 2);

        ctx->drawBatchProxy(proxy);

        //for (const auto& node : model1->meshNodes()) {
        //    if (node->meshContainerIndex() >= 0) {

        //        ctx->setTransfrom(model1->nodeGlobalTransform(node->index()));

        //        const auto& meshContainer = model1->meshContainers()[node->meshContainerIndex()];
        //        for (const auto& meshPrimitive : meshContainer->meshPrimitives()) {

        //            for (int iSection = 0; iSection < meshPrimitive->sections().size(); iSection++) {
        //                int materialIndex = meshPrimitive->sections()[iSection].materialIndex;
        //                if (materialIndex >= 0) {
        //                    ctx->setMaterial(model1->materials()[materialIndex]);

        //                    ctx->drawSkinnedMesh(
        //                        meshPrimitive, iSection, instance1->hasSkeleton() ? instance1->skeletons()[node->skeletonIndex] : nullptr, instance1->hasMorph() ? instance1->morphs()[node->meshContainerIndex()] : nullptr);
        //                }
        //            }
        //        }
        //    }
        //}
    }
    TestEnv::renderView->end();
    TestEnv::swapChain->endFrame();

    ASSERT_RENDERTARGET(LN_ASSETFILE("Rendering/Expects/Test_Rendering_Mesh.SkinnedMesh1.png"), target);
}

// 共通の MeshModel を、異なる手動ボーン操作で 2 つ描画してみる
TEST_F(Test_Rendering_Mesh, SkinnedMesh2) {
    RESET_VIEW();

    auto model1 = MeshModel::load(LN_ASSETFILE("Mesh/SkinnedMesh2.glb"));
    auto instance1 = model1->createMeshModelInstance();
    auto instance2 = model1->createMeshModelInstance();

    auto proxy1 = makeURef<MeshBatchProxy>();
    proxy1->modelInstance = instance1;

    auto proxy2 = makeURef<MeshBatchProxy>();
    proxy2->modelInstance = instance2;

    auto* commandList = TestEnv::swapChain->beginFrame2();
    auto* target = TestEnv::swapChain->currentBackbuffer();
    auto* ctx = TestEnv::renderView->begin(commandList, target);
    {
        instance1->updateFrame(0.016);
        instance2->updateFrame(0.016);

        // Manual update
        auto skeleton1 = instance1->skeletons()[0];
        auto bone1 = skeleton1->bones()[1];
        bone1->setRotation(0, 0, Math::PI / 2);

        // Manual update
        auto skeleton2 = instance2->skeletons()[0];
        auto bone2 = skeleton2->bones()[1];
        bone2->setRotation(0, 0, -Math::PI / 2);

        ctx->setBaseTransfrom(Matrix::makeTranslation(-1, 0, 0));
        ctx->drawBatchProxy(proxy1);

        ctx->setBaseTransfrom(Matrix::makeTranslation(1, 0, 0));
        ctx->drawBatchProxy(proxy2);
    }
    TestEnv::renderView->end();
    TestEnv::swapChain->endFrame();

    ASSERT_RENDERTARGET(LN_ASSETFILE("Rendering/Expects/Test_Rendering_Mesh.SkinnedMesh2.png"), target);
}

TEST_F(Test_Rendering_Mesh, RuntimeMaterial) {
    RESET_VIEW();

    auto model1 = MeshModel::load(LN_ASSETFILE("Mesh/SkinnedMesh2.glb"));
    auto instance1 = model1->createMeshModelInstance();
    auto instance2 = model1->createMeshModelInstance();

    auto material1 = Material::With().color(Color::Blue).build();
    instance1->setMaterial(0, material1);

    auto proxy1 = makeURef<MeshBatchProxy>();
    proxy1->modelInstance = instance1;

    auto proxy2 = makeURef<MeshBatchProxy>();
    proxy2->modelInstance = instance2;

    auto* commandList = TestEnv::swapChain->beginFrame2();
    auto* target = TestEnv::swapChain->currentBackbuffer();
    auto* ctx = TestEnv::renderView->begin(commandList, target);
    {
        instance1->updateFrame(0.016);
        instance2->updateFrame(0.016);

        ctx->setBaseTransfrom(Matrix::makeTranslation(-1, 0, 0));
        ctx->drawBatchProxy(proxy1);

        ctx->setBaseTransfrom(Matrix::makeTranslation(1, 0, 0));
        ctx->drawBatchProxy(proxy2);
    }
    TestEnv::renderView->end();
    TestEnv::swapChain->endFrame();

    ASSERT_RENDERTARGET(LN_ASSETFILE("Rendering/Expects/Test_Rendering_Mesh.RuntimeMaterial.png"), target);
}
