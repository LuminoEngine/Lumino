
#include "Internal.hpp"
#include <float.h>
#include "FpsController.hpp"

namespace ln {
namespace detail {

//=============================================================================
// FpsController

FpsController::FpsController()
    : m_frameRate(0)
    , m_frameTime(0)
    //, m_timer()
    //, m_currentGameTime(0)
    , m_frameCount(0)
    , m_elapsedGameTimeCache(0)
    , m_totalGameTimeCache(0)
    , m_measureTimes(true)
    , m_frameTimes()
    , m_externalTimes()
    , m_totalFps(0.f)
    , m_externalFps(0.f)
    , m_term(0)
    , m_averageTime(0)
    , m_externalAverageTime(0)
    , m_minTime(0)
    , m_maxTime(0)
    , m_minTimePerSeconds(0)
    , m_maxTimePerSeconds(0)
{
    setFrameRate(60);
    //m_timer.start();
	m_startTick = Environment::getTickCount();
	m_frameBeginTick = m_startTick;
}

void FpsController::setFrameRate(int value)
{
    m_frameRate = value;
    m_frameTime = 1000 / m_frameRate;
    m_frameTimes.resize(m_frameRate);
    m_externalTimes.resize(m_frameRate);
}

void FpsController::process()
{
#if 1
	uint64_t externalElapsedTick = Environment::getTickCount() - m_frameBeginTick;
	m_term = m_frameTime - externalElapsedTick;

	if (m_term > 0) {
		Thread::sleep(m_term);
	}

	uint64_t currentTick = Environment::getTickCount();
	uint64_t internalElapsedTick = currentTick - m_frameBeginTick;

	measureTimes(externalElapsedTick, internalElapsedTick);

	m_elapsedGameTimeCache = 0.001f * internalElapsedTick;
	m_totalGameTimeCache = 0.001 * (currentTick - m_startTick);
	m_frameCount = (++m_frameCount) % m_frameRate;
	m_frameBeginTick = currentTick;
#else
    uint64_t externalElapsedTime = m_timer.elapsedMilliseconds();
    m_currentGameTime += externalElapsedTime;
    m_term = m_frameTime - externalElapsedTime;


    if (m_term > 0) {
        Thread::sleep(m_term);
    }

    // containd Thread::sleep() time. (total frame time)
    uint64_t frameElapsedTime = m_timer.elapsedMilliseconds();
    m_elapsedGameTime = 0.001f * frameElapsedTime;
    m_totalGameTime = 0.001 * m_currentGameTime;

    measureTimes(externalElapsedTime, frameElapsedTime);

    m_frameCount = (++m_frameCount) % m_frameRate;

	//printf("%llu %f\n", m_currentGameTime, m_elapsedGameTime);

    m_timer.start();
#endif
}

void FpsController::processForMeasure()
{
#if 1
	uint64_t externalElapsedTick = Environment::getTickCount() - m_frameBeginTick;

	measureTimes(externalElapsedTick, externalElapsedTick);

	uint64_t currentTick = Environment::getTickCount();
	m_frameCount = (++m_frameCount) % m_frameRate;
	m_frameBeginTick = currentTick;
#else
    uint64_t externalElapsedTime = m_timer.elapsedMilliseconds();
    m_currentGameTime += externalElapsedTime;

    measureTimes(externalElapsedTime, externalElapsedTime);

    m_frameCount = (++m_frameCount) % m_frameRate;

    m_timer.start();
#endif
}

void FpsController::refreshSystemDelay()
{
    //m_timer.start();
	m_frameBeginTick = Environment::getTickCount();
    m_frameCount = 0;
}

void FpsController::measureTimes(uint64_t externalElapsedTime, uint64_t frameElapsedTime)
{
    if (m_measureTimes) {

        m_externalTimes[m_frameCount] = externalElapsedTime;
        m_frameTimes[m_frameCount] = frameElapsedTime;

        //m_minTime = std::max(std::min(m_minTime, frameElapsedTime), (uint64_t)0);
		//if (frameElapsedTime < m_minTime) m_minTime = 
		m_minTime = std::min(m_minTime, frameElapsedTime);
        m_maxTime = std::max(m_maxTime, frameElapsedTime);

        if (m_frameCount == m_frameTimes.size() - 1) {

            // calc average time (internal time)
            {
                uint64_t total = 0;
                for (auto t : m_frameTimes) {
                    total += t;
                }
                m_averageTime = total / m_frameRate;

                m_totalFps = (m_averageTime > 0) ? (1000.f / m_averageTime) : 0.f;
            }

            // calc average time (external time)
            {
                uint64_t total = 0;
                for (auto t : m_externalTimes) {
                    total += t;
                }
                m_externalAverageTime = total / m_frameRate;

                m_externalFps = (m_externalAverageTime > 0) ? (1000.f / m_externalAverageTime) : 0.f;
            }

            // get snapshot
            m_minTimePerSeconds = m_minTime;
            m_maxTimePerSeconds = m_maxTime;
            m_minTime = FLT_MAX;
            m_maxTime = FLT_MIN;
        }
    }
}

} // namespace detail
} // namespace ln

