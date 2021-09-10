
#include "Internal.hpp"
#include <LuminoEngine/Engine/EngineContext2.hpp>

namespace ln {

std::unique_ptr<EngineContext2> s_instance;

bool EngineContext2::initialize()
{
    if (LN_REQUIRE(!s_instance)) return false;
    s_instance = std::unique_ptr<EngineContext2>(LN_NEW EngineContext2());
    return s_instance->init();
}

void EngineContext2::terminate()
{
    if (s_instance) {
        s_instance->dispose();
        s_instance = nullptr;
    }
}

EngineContext2::EngineContext2()
{
}

bool EngineContext2::init()
{
    return true;
}

void EngineContext2::dispose()
{
}

} // namespace ln

