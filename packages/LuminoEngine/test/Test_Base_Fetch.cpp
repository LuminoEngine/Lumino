#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Base/Fetch.hpp>

class Test_Base_Fetch : public ::testing::Test {};

TEST_F(Test_Base_Fetch, Basic) {
    RuntimeModule::initialize();

    //auto r = detail::FetchManager::getSync(U"http://example.com");

    RuntimeModule::terminate();

}
