#pragma once
#include "Common2.hpp"
#include "../Reflection/Object.hpp"

namespace ln {

class CoreApplication : public Object {
public:
    static CoreApplication* instance() { return s_instance; }

    CoreApplication();
    virtual ~CoreApplication();

protected:

private:
    static CoreApplication* s_instance;
};

} // namespace ln

#define LN_CORE_APP ::ln::CoreApplication::instance()
