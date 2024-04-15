
#include "Internal.hpp"
#include <LuminoEngine/Base/Builder.hpp>

namespace ln {

//==============================================================================
// AbstractBuilderDetails

AbstractBuilderDetails::~AbstractBuilderDetails() = default;

Ref<Object> AbstractBuilderDetails::create() const
{
	return nullptr;
}

} // namespace ln

