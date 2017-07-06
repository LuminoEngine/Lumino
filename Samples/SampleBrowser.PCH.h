#pragma once
#define LUMINO_LINK_LIBRARY_DISABLED
#include <Lumino.h>

#define LN_SAMPLE_INITIALIZE
#define LN_SAMOLE_TERMINATE

namespace sample {

using SampleMainFunc = void(*)();

SampleMainFunc registerSample(const char* name, SampleMainFunc func);

} // namespace sample


#define LN_SAMPLE_MAIN(name) \
	void SampleMain_##name(); \
	static sample::SampleMainFunc _localSampleMain_##name = sample::registerSample(#name, SampleMain_##name); \
	void SampleMain_##name()

