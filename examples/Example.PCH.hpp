#pragma once
#include <Lumino.hpp>

#undef LUMINO_APP

struct Example
{
    std::function<Ref<Application>()> app;
    Path filePath;
};

#define LUMINO_APP(appClass) \
    Example CreateApp_##appClass() \
    { \
        Example e { \
            []() { return makeRef<appClass>(); }, \
            Path(String::fromCString(__FILE__)) \
        }; \
        return e; \
    }
