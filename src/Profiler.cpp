
#include "Internal.h"
#include <Lumino/Profiler.h>

namespace Lumino
{

//=============================================================================
// Profiler
//=============================================================================

Profiler	Profiler::Instance;

int		Profiler::Group_MainThread = 0;
int		Profiler::Group_RenderThread = 1;

int		Profiler::Section_MainThread_Update = 0;
int		Profiler::Section_MainThread_PrepareRendering = 1;
int		Profiler::Section_MainThread_GUILayput = 2;

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Profiler::Profiler()
	: m_enabled(false)
{
	Group_MainThread = RegisterGroup(_T("Main thread"));
	Group_RenderThread = RegisterGroup(_T("Rendering thread"));
	Section_MainThread_Update = RegisterSection(Group_MainThread, _T("Update"));
	Section_MainThread_PrepareRendering = RegisterSection(Group_MainThread, _T("Prepare rendering"));
	Section_MainThread_GUILayput = RegisterSection(Group_MainThread, _T("GUI layput"));
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

	for (int iGroup = 0; iGroup < m_groups.GetCount(); ++iGroup)
	{
		for (int iSection = 0; iSection < m_groups[iGroup]->Sections.GetCount(); ++iSection)
		{
			Section*			s1 = m_groups[iGroup]->Sections[iSection].get();
			CommitedSection*	s2 = &m_commitedGroups[iGroup].Sections[iSection];
			s2->ElapsedTime = s1->ElapsedTime;
			s1->ElapsedTime = 0;
		}
	}
}


} // namespace Lumino
