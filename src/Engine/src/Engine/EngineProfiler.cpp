
#include "Internal.hpp"
#include "EngineProfiler.hpp"

namespace ln {
namespace detail {

//==============================================================================
// EngineProfiler

EngineProfiler::EngineProfiler()
{
}

void EngineProfiler::beginFrame(int32_t frameRate)
{
	m_frameRate = frameRate;
	m_frameTimer.start();
}

} // namespace detail
} // namespace ln

