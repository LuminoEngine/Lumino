#include "TestEnv.hpp"

class Test_Mesh : public ::testing::Test {
public:
    static void SetUpTestSuite() {
        TestEnv::initializeRendering();
    }
};

#if 0
TEST_F(Test_Mesh, LoadAsync) {
    RESET_VIEW();
    TestEnv::renderView->resetView(Vector3(8, 8, 8), Vector3(0, 1, 0));

    auto model1 = MeshModel::load(U"https://github.com/KhronosGroup/glTF-Sample-Models/raw/master/2.0/Box/glTF-Binary/Box.glb");
    auto instance1 = model1->createMeshModelInstance();

}
#endif
