
#pragma once
#include <memory>
#include <Lumino/Base/Array.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/ElapsedTimer.h>
#include <Lumino/Threading/Mutex.h>

namespace Lumino
{

/**
	@brief
*/
class Profiler
{
public:
	static Profiler	Instance;		///< グローバルインスタンス


	static int		Group_MainThread;
	static int		Group_RenderThread;

	static int		Section_MainThread_Update;
	static int		Section_MainThread_PrepareRendering;
	static int		Section_MainThread_GUILayput;

	struct Section
	{
		String		Name;					///< セクション名
		uint64_t	ElapsedTime;			///< 経過時間 (ns)
	};

	// グループはスレッドごと
	struct Group
	{
		String		Name;					///< グループ名
		Array< std::shared_ptr<Section> >	Sections;
		//Section		Sections[MaxSections];
		ElapsedTimer	Timer;
	};

	struct CommitedSection
	{
		String		Name;					///< セクション名
		uint64_t	ElapsedTime;			///< 経過時間 (ns)
	};

	struct CommitedGroup
	{
		String					Name;		///< グループ名
		Array<CommitedSection>	Sections;
	};

public:
	Profiler();

public:
	/// グループを作成する
	int RegisterGroup(const TCHAR* name);

	/// セクションを作成する
	int RegisterSection(int parentGroupIndex, const TCHAR* name);

	/// 計測開始 (区間のネストはできない)
	void StartSection(int groupIndex, int sectionIndex);

	/// 計測終了
	void EndSection(int groupIndex, int sectionIndex);

	/// 計測の有効設定
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	/// 計測の有効設定
	bool IsEnabled() const { return m_enabled; }

	void Commit();

	const Array<CommitedGroup>& GetCommitedGroups() const { return m_commitedGroups; }

private:
	Array< std::shared_ptr<Group> >			m_groups;
	Array<CommitedGroup>	m_commitedGroups;
	Threading::Mutex		m_commitMutex;
	bool					m_enabled;
};

class ScopedProfilerSection
{
public:
	ScopedProfilerSection(int group, int section)
		: m_group(group)
		, m_section(section)
	{
		Profiler::Instance.StartSection(m_group, m_section);
	}

	~ScopedProfilerSection()
	{
		Profiler::Instance.EndSection(m_group, m_section);
	}

private:
	int	m_group;
	int m_section;
};


} // namespace Lumino
