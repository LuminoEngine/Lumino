
#include "Internal.h"
#include <Lumino/Profiler.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// Profiler
//=============================================================================

Profiler	Profiler::Instance;

int		Profiler::Group_MainThread = 0;
int		Profiler::Group_RenderThread = 1;

int		Profiler::Section_MainThread_Update = 0;
int		Profiler::Section_MainThread_PrepareRendering = 1;
int		Profiler::Section_MainThread_GUILayput = 2;
int		Profiler::Section_RenderThread_CommandExecute = 3;


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
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
	Group_MainThread = RegisterGroup(_T("Main"));
	Group_RenderThread = RegisterGroup(_T("Rendering"));
	Section_MainThread_Update = RegisterSection(Group_MainThread, _T("Update"));
	Section_MainThread_PrepareRendering = RegisterSection(Group_MainThread, _T("Prepare rendering"));
	Section_MainThread_GUILayput = RegisterSection(Group_MainThread, _T("GUI layput"));
	Section_RenderThread_CommandExecute = RegisterSection(Group_RenderThread, _T("Execute commands"));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Profiler::RegisterGroup(const TCHAR* name)
{
	std::shared_ptr<Group> group(LN_NEW Group());
	group->Name = name;
	m_groups.Add(group);

	CommitedGroup cg;
	cg.Name = name;
	m_commitedGroups.Add(cg);

	return m_groups.GetCount() - 1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int Profiler::RegisterSection(int parentGroupIndex, const TCHAR* name)
{
	std::shared_ptr<Section> section(LN_NEW Section());
	section->Name;
	m_groups[parentGroupIndex]->Sections.Add(section);

	CommitedSection cs;
	cs.Name = name;
	m_commitedGroups[parentGroupIndex].Sections.Add(cs);

	return m_groups[parentGroupIndex]->Sections.GetCount() - 1;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Profiler::SetBaseFrameRate(int group, float baseFrameRate)
{
	m_groups[group]->LimitElapsedTime = (1.0f / baseFrameRate) * 1000 * 1000 * 1000;	// ns 単位
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Profiler::StartSection(int groupIndex, int sectionIndex)
{
	if (!m_enabled) { return; }
	m_groups[groupIndex]->Timer.Start();
	//Section* s = m_groups[groupIndex]->Sections[sectionIndex];
	
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Profiler::EndSection(int groupIndex, int sectionIndex)
{
	if (!m_enabled) { return; }
	m_groups[groupIndex]->Sections[sectionIndex]->ElapsedTime = m_groups[groupIndex]->Timer.GetElapsedTimeNS();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Profiler::Commit()
{
	Threading::MutexScopedLock lock(m_commitMutex);

	m_commitedMainFPS = m_mainFPS;
	m_commitedMainFPSCapacity = m_mainFPSCapacity;
	m_commitedMainWindowSize = m_mainWindowSize;
	m_commitedMainBackbufferSize = m_mainBackbufferSize;

	for (int iGroup = 0; iGroup < m_groups.GetCount(); ++iGroup)
	{
		uint64_t totalTime = 0;
		for (int iSection = 0; iSection < m_groups[iGroup]->Sections.GetCount(); ++iSection)
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
