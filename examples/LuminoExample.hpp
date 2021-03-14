#pragma once
#include <Lumino.hpp>

#undef LUMINO_APP

#define LUMINO_APP(appClass) \
    Ref<ln::Application> CreateApp_##appClass() \
    { \
        return makeRef<appClass>(); \
    }
