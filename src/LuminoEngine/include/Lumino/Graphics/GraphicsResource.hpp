#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class GraphicsManager;
}

class LN_API GraphicsResource
	: public Object
{
public:

LN_CONSTRUCT_ACCESS:
	GraphicsResource();
	virtual ~GraphicsResource();
	void initialize();

private:
	detail::GraphicsManager* m_manager;
};

} // namespace ln
