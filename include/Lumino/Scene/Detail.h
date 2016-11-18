
#pragma once
#include "../Graphics/RenderState.h"
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

namespace detail
{

struct VisualNodeRenderState
{
	BlendMode	blendMode = BlendMode::Normal;
	CullingMode	cullingMode = CullingMode::Back;
	bool		depthTestEnabled = true;
	bool		depthWriteEnabled = true;
};


class IProfilerClient
{
public:

protected:
	virtual ~IProfilerClient() = default;
};

template<typename TProfileData>
class ProfilerBase
	: public IProfilerClient
{
public:
	ProfilerBase()
		: m_currentFrameData(LN_NEW TProfileData())
		, m_lastFrameData(LN_NEW TProfileData())
	{
	}

	virtual ~ProfilerBase()
	{
		LN_SAFE_DELETE(m_currentFrameData);
		LN_SAFE_DELETE(m_lastFrameData);
	}

	void BeginSession()
	{
		m_currentFrameData->Reset();
	}

	void EndSession()
	{
		std::swap(m_currentFrameData, m_lastFrameData);
	}

	TProfileData* GetCurrentFrameData() const { return m_currentFrameData; }
	TProfileData* GetLastFrameData() const { return m_lastFrameData; }

private:
	TProfileData*	m_currentFrameData;
	TProfileData*	m_lastFrameData;
};

class SceneGraphRenderingProfilerInterface
{
public:
	struct ProfileData
	{
		int	nodeDrawCount;

		void Reset()
		{
			nodeDrawCount = 0;
		}
	};

	void Subscribe() { m_profiler = std::make_shared<ProfilerBase<ProfileData>>(); }
	void Unsubscribe() { m_profiler = nullptr; }
	const ProfileData* GetLastFrameData() const { return (m_profiler != nullptr) ? m_profiler->GetLastFrameData() : nullptr; }

	void BeginSession() { if (m_profiler != nullptr) m_profiler->BeginSession(); }
	void EndSession() { if (m_profiler != nullptr) m_profiler->EndSession(); }
	void IncreaseNodeDrawCount() { if (m_profiler != nullptr) m_profiler->GetCurrentFrameData()->nodeDrawCount++; }

private:

	std::shared_ptr<ProfilerBase<ProfileData>>	m_profiler;
};

} // namespace detail 

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

