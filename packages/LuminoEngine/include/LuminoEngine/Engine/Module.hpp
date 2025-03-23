#pragma once
#include "Common2.hpp"

namespace ln {

class Module : public RefObject {
public:
protected:
    Module();
    virtual ~Module();
    virtual void onRegisterTypes(EngineInstance* context);

private:
    EngineInstance* m_context;

    friend class EngineInstance;
};

} // namespace ln
