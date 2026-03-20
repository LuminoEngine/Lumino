#pragma once
#include "RHIObject.hpp"

namespace ln {
namespace detail {

class ISamplerState : public RHIDeviceObject {
public:
protected:
    ISamplerState();
    virtual ~ISamplerState();
};

} // namespace detail
} // namespace ln
