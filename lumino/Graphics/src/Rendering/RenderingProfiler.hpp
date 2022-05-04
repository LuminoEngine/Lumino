#pragma once

namespace ln {
namespace detail {

class RenderingProfiler
{
public:
	struct SceneRendererSection
	{
		const char* name;
		uint64_t elapsedTime;

		double time() const { return static_cast<double>(elapsedTime) / 1000000000; }
	};

	RenderingProfiler();

	void reset();
	void beginSceneRenderer(const char* sectionName);
	void endSceneRenderer();

	int32_t sceneRendererSectionCount() const { return m_sceneRendererSectionCount; }
	const SceneRendererSection& sceneRendererSection(int32_t index) { return m_sceneRendererSections[index]; }

private:
	ElapsedTimer m_frameTimer;
	uint64_t m_sectionBeginTime;
	int32_t m_sceneRendererSectionCount;
	std::array<SceneRendererSection, 32> m_sceneRendererSections;
};

} // namespace detail
} // namespace ln

