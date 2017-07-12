#pragma once
#define LUMINO_LINK_LIBRARY_DISABLED
#include <Lumino.h>

#define LN_SAMPLE_INITIALIZE
#define LN_SAMPLE_TERMINATE
#define LN_SAMPLE_UPDATE		(Engine::update() && !sample::g_sceneChanging)

namespace sample {

using SampleMainFunc = void(*)();

SampleMainFunc registerSample(const char* group, const char* caption, SampleMainFunc func);

extern bool g_sceneChanging;

} // namespace sample


#define LN_SAMPLE_MAIN(group, caption) \
	void SampleMain_##caption(); \
	static sample::SampleMainFunc _localSampleMain_##caption = sample::registerSample(#group, #caption, SampleMain_##caption); \
	void SampleMain_##caption()

