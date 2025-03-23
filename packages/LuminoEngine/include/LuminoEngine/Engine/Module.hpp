#pragma once
#include "Common2.hpp"

namespace ln {

class Module : public RefObject {
public:
protected:
    Module();
    virtual ~Module();
    virtual void onRegisterTypes(EngineManager* context);

private:
    EngineManager* m_context;

    friend class EngineManager;
};

} // namespace ln
