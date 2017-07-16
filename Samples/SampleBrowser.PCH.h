#pragma once
#define LUMINO_LINK_LIBRARY_DISABLED
#include <Lumino.h>

#define LN_SAMPLE_INITIALIZE
#define LN_SAMPLE_TERMINATE
#define LN_SAMPLE_UPDATE		(Engine::update() && !sample::g_sceneChanging)

namespace sample {

using SampleMainFunc = void(*)();

SampleMainFunc registerSample(const char* group1, const char* group2, const char* caption, SampleMainFunc func);

extern bool g_sceneChanging;

} // namespace sample


#define LN_SAMPLE_MAIN(group1, group2, caption) \
	void SampleMain_##group1##group2##caption(); \
	static sample::SampleMainFunc _localSampleMain_##group1##group2##caption = sample::registerSample(#group1, #group2, #caption, SampleMain_##group1##group2##caption); \
	void SampleMain_##group1##group2##caption()

