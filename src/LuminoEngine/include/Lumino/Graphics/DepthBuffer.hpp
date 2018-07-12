
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class IDepthBuffer; }

class DepthBuffer
	: public GraphicsResource
{
public:
LN_INTERNAL_ACCESS:
	detail::IDepthBuffer* resolveRHIObject() { LN_NOTIMPLEMENTED(); return nullptr; }
};

} // namespace ln
