#include "TestEnv.hpp"

int main(int argc, char** argv) {
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    setlocale(LC_ALL, "");
    testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new TestEnv);
    return RUN_ALL_TESTS();
}
