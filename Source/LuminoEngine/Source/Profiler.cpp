
#include "Internal.h"
#include <Lumino/Profiler.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ProfilingKey
//==============================================================================

//------------------------------------------------------------------------------
ProfilingKey::ProfilingKey(const StringRef& name)
	: m_name(name)
	//, m_keyHash(0)
	, m_parent(nullptr)
{
	//m_keyHash = Hash::calcHash(m_name.c_str());
}

//------------------------------------------------------------------------------
ProfilingKey::ProfilingKey(ProfilingKey* parent, const StringRef& name)
	: ProfilingKey(name)
{
	m_parent = parent;
	//m_keyHash += m_parent->m_keyHash;
}

//==============================================================================
// ProfilingKeys
//==============================================================================
const ProfilingKey* ProfilingKeys::Engine_UpdateFrame = nullptr;
const ProfilingKey* ProfilingKeys::Rendering_PresentDevice = nullptr;

//==============================================================================
// ProfilingSection
//==============================================================================

//------------------------------------------------------------------------------
ProfilingSection::ProfilingSection()
	: m_timer()
	, m_totalTime(0)
	, m_measuring()
	, m_committedFrameCount(0)
	, m_committedTime(0)
{
}

//------------------------------------------------------------------------------
void ProfilingSection::begin()
{
	m_measuring.setTrue();
	m_timer.start();
}

//------------------------------------------------------------------------------
void ProfilingSection::end()
{
	m_totalTime = m_timer.getElapsed();
	m_measuring.setFalse();
}

//------------------------------------------------------------------------------
void ProfilingSection::commitFrame(int frameCount)
{
	if (m_measuring.isFalse())
	{
		m_committedTime = m_totalTime;
		m_committedFrameCount = frameCount;
	}
}


//==============================================================================
// Profiler
//==============================================================================

static std::vector<std::shared_ptr<ProfilingKey>>	g_globalKeys;
static std::unordered_map<const ProfilingKey*, std::shared_ptr<ProfilingSection>>	g_sectionsMap;
static int g_frameCount;
static Mutex	g_mutex;

//------------------------------------------------------------------------------
void Profiler2::initializeGlobalSections()
{
	{
		auto key = std::make_shared<ProfilingKey>(_T("Engine_UpdateFrame"));
		g_globalKeys.push_back(key);
		ProfilingKeys::Engine_UpdateFrame = key.get();
		registerProfilingSection(key.get());
	}
	{
		auto key = std::make_shared<ProfilingKey>(_T("Rendering_PresentDevice"));
		g_globalKeys.push_back(key);
		ProfilingKeys::Rendering_PresentDevice = key.get();
		registerProfilingSection(key.get());
	}
}

//------------------------------------------------------------------------------
void Profiler2::finalizeGlobalSections()
{
	g_globalKeys.clear();
}

//------------------------------------------------------------------------------
void Profiler2::registerProfilingSection(const ProfilingKey* key)
{
	if (LN_CHECK_ARG(key != nullptr)) return;
	g_sectionsMap[key] = std::make_shared<ProfilingSection>();
}

//------------------------------------------------------------------------------
ProfilingSection* Profiler2::getSection(const ProfilingKey* key)
{
	return g_sectionsMap[key].get();
}

//------------------------------------------------------------------------------
void Profiler2::commitFrame()
{
	MutexScopedLock lock(g_mutex);
	for (auto& section : g_sectionsMap)
	{
		section.second->commitFrame(g_frameCount);
	}
	g_frameCount++;
}


//==============================================================================
// Profiler
//==============================================================================

Profiler	Profiler::Instance;

int		Profiler::Group_MainThread = 0;
int		Profiler::Group_RenderThread = 1;

int		Profiler::Section_MainThread_Update = 0;
int		Profiler::Section_MainThread_PrepareRendering = 1;
int		Profiler::Section_MainThread_GUILayput = 2;
int		Profiler::Section_RenderThread_CommandExecute = 3;


//------------------------------------------------------------------------------
Profiler::Profiler()
	: m_groups()
	, m_mainFPS(0.0f)
	, m_mainFPSCapacity(0.0f)
	, m_commitedGroups()
	, m_commitedMainFPS(0.0f)
	, m_commitedMainFPSCapacity(0.0f)
	, m_commitMutex()
	, m_enabled(false)
{
	Group_MainThread = registerGroup(_T("Main"));
	Group_RenderThread = registerGroup(_T("Rendering"));
	Section_MainThread_Update = registerSection(Group_MainThread, _T("Update"));
	Section_MainThread_PrepareRendering = registerSection(Group_MainThread, _T("Prepare rendering"));
	Section_MainThread_GUILayput = registerSection(Group_MainThread, _T("GUI layput"));
	Section_RenderThread_CommandExecute = registerSection(Group_RenderThread, _T("Execute commands"));
}

//------------------------------------------------------------------------------
int Profiler::registerGroup(const TCHAR* name)
{
	std::shared_ptr<Group> group(LN_NEW Group());
	group->Name = name;
	m_groups.add(group);

	CommitedGroup cg;
	cg.Name = name;
	m_commitedGroups.add(cg);

	return m_groups.getCount() - 1;
}

//------------------------------------------------------------------------------
int Profiler::registerSection(int parentGroupIndex, const TCHAR* name)
{
	std::shared_ptr<Section> section(LN_NEW Section());
	section->Name;
	m_groups[parentGroupIndex]->Sections.add(section);

	CommitedSection cs;
	cs.Name = name;
	m_commitedGroups[parentGroupIndex].Sections.add(cs);

	return m_groups[parentGroupIndex]->Sections.getCount() - 1;
}

//------------------------------------------------------------------------------
void Profiler::setBaseFrameRate(int group, float baseFrameRate)
{
	m_groups[group]->LimitElapsedTime = (1.0f / baseFrameRate) * 1000 * 1000 * 1000;	// ns 単位
}

//------------------------------------------------------------------------------
void Profiler::startSection(int groupIndex, int sectionIndex)
{
	if (!m_enabled) { return; }
	m_groups[groupIndex]->Timer.start();
	//Section* s = m_groups[groupIndex]->Sections[sectionIndex];

}

//------------------------------------------------------------------------------
void Profiler::endSection(int groupIndex, int sectionIndex)
{
	if (!m_enabled) { return; }
	//TODO:
	//m_groups[groupIndex]->Sections[sectionIndex]->ElapsedTime = m_groups[groupIndex]->Timer.GetElapsedTimeNS();
}

//------------------------------------------------------------------------------
void Profiler::commit()
{
	MutexScopedLock lock(m_commitMutex);

	m_commitedMainFPS = m_mainFPS;
	m_commitedMainFPSCapacity = m_mainFPSCapacity;
	m_commitedMainWindowSize = m_mainWindowSize;
	m_commitedMainBackbufferSize = m_mainBackbufferSize;

	for (int iGroup = 0; iGroup < m_groups.getCount(); ++iGroup)
	{
		uint64_t totalTime = 0;
		for (int iSection = 0; iSection < m_groups[iGroup]->Sections.getCount(); ++iSection)
		{
			Section*			s1 = m_groups[iGroup]->Sections[iSection].get();
			CommitedSection*	s2 = &m_commitedGroups[iGroup].Sections[iSection];
			s2->ElapsedTime = s1->ElapsedTime;
			s1->ElapsedTime = 0;
			totalTime += s2->ElapsedTime;
		}
		m_commitedGroups[iGroup].TotalTime = totalTime;
		m_commitedGroups[iGroup].LimitElapsedTime = m_groups[iGroup]->LimitElapsedTime;
	}
}


LN_NAMESPACE_END
