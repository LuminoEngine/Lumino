#pragma once
#include <LuminoEngine/Graphics/Common.hpp>

namespace ln {
namespace detail {

class EngineProfiler
{
public:
	EngineProfiler();

	void beginFrame(int32_t frameRate);
	void lapBeginUpdate() { m_updateBeginTime = m_frameTimer.elapsedNanoseconds(); }
	void lapEndUpdate() { m_updateEndTime = m_frameTimer.elapsedNanoseconds(); }
	void lapBeginUIUpdate() { m_updateUIBeginTime = m_frameTimer.elapsedNanoseconds(); }
	void lapEndUIUpdate() { m_updateUIEndTime = m_frameTimer.elapsedNanoseconds(); }
	void lapBeginRendering() { m_renderingBeginTime = m_frameTimer.elapsedNanoseconds(); }
	void lapEndRendering() { m_renderingEndTime = m_frameTimer.elapsedNanoseconds(); }
	void endFrame() { m_totalFrameTime = m_frameTimer.elapsedNanoseconds(); }

	int32_t frameRate() const { return m_frameRate; }
	double totalFrameTime() const{ return static_cast<double>(m_totalFrameTime) / 1000000000; }
	double updateTime() const { return static_cast<double>(m_updateEndTime - m_updateBeginTime) / 1000000000; }
	double updateUITime() const { return static_cast<double>(m_updateUIEndTime - m_updateUIBeginTime) / 1000000000; }
	double renderingTime() const{ return static_cast<double>(m_renderingEndTime - m_renderingBeginTime) / 1000000000; }

private:
	ElapsedTimer m_frameTimer;
	int32_t m_frameRate;
	uint64_t m_totalFrameTime;
	uint64_t m_updateBeginTime;
	uint64_t m_updateEndTime;
	uint64_t m_updateUIBeginTime;
	uint64_t m_updateUIEndTime;
	uint64_t m_renderingBeginTime;
	uint64_t m_renderingEndTime;
};

} // namespace detail
} // namespace ln

