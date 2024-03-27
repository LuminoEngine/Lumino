#include "Internal.hpp"
#include <LuminoEngine/Rendering/detail/RenderingProfiler.hpp>

namespace ln {
namespace detail {

//==============================================================================
// RenderingProfiler

RenderingProfiler::RenderingProfiler()
{
	reset();
}

void RenderingProfiler::reset()
{
	m_frameTimer.start();
	m_sceneRendererSectionCount = 0;
}

void RenderingProfiler::beginSceneRenderer(const char* sectionName)
{
	m_sceneRendererSections[m_sceneRendererSectionCount].name = sectionName;
	m_sceneRendererSections[m_sceneRendererSectionCount].elapsedTime = 0;
	m_sectionBeginTime = m_frameTimer.elapsedNanoseconds();
}

void RenderingProfiler::endSceneRenderer()
{
	m_sceneRendererSections[m_sceneRendererSectionCount].elapsedTime = m_frameTimer.elapsedNanoseconds() - m_sectionBeginTime;
	m_sceneRendererSectionCount++;
}

} // namespace detail
} // namespace ln

