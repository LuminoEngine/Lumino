
#pragma once
#include <memory>
#include <Lumino/Base/List.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/ElapsedTimer.h>
#include <Lumino/Threading/Mutex.h>

LN_NAMESPACE_BEGIN

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
	static int		Section_RenderThread_CommandExecute;

	struct Section
	{
		String		Name;					///< セクション名
		uint64_t	ElapsedTime;			///< 経過時間 (ns)
	};

	// グループはスレッドごと
	struct Group
	{
		String		Name;					///< グループ名
		List< std::shared_ptr<Section> >	Sections;
		//Section		Sections[MaxSections];
		ElapsedTimer	Timer;
		float			LimitElapsedTime;	///< 許容時間 (ns 単位。フレームレートの逆数)
	};

	struct CommitedSection
	{
		String		Name;					///< セクション名
		uint64_t	ElapsedTime;			///< 経過時間 (ns)
	};

	struct CommitedGroup
	{
		String					Name;		///< グループ名
		List<CommitedSection>	Sections;
		uint64_t				TotalTime;	///< CommitedSection のトータル ElapsedTime
		float					LimitElapsedTime;	///< 許容時間 (ns 単位。フレームレートの逆数)
	};

public:
	Profiler();

public:
	/// グループを作成する
	int registerGroup(const TCHAR* name);

	/// セクションを作成する
	int registerSection(int parentGroupIndex, const TCHAR* name);

	/// グループの測定基準となるフレームレートを設定する
	void setBaseFrameRate(int group, float baseFrameRate);

	/// 計測開始 (区間のネストはできない)
	void startSection(int groupIndex, int sectionIndex);

	/// 計測終了
	void endSection(int groupIndex, int sectionIndex);

	/// 計測の有効設定
	void setEnabled(bool enabled) { m_enabled = enabled; }

	/// 計測の有効設定
	bool isEnabled() const { return m_enabled; }

	/// メインスレッドの FPS
	void setMainFPS(float fps) { m_mainFPS = fps; }

	/// メインスレッドの FPS 余裕度
	void setMainFPSCapacity(float fps) { m_mainFPSCapacity = fps; }

	/// メインウィンドウのサイズ
	void setMainWindowSize(const SizeI& size) { m_mainWindowSize = size; }

	/// メインバックバッファのサイズ
	void setMainBackBufferSize(const SizeI& size) { m_mainBackbufferSize = size; }


	void commit();

	const List<CommitedGroup>& getCommitedGroups() const { return m_commitedGroups; }
	float getCommitedMainFPS() const { return m_commitedMainFPS; }
	float getCommitedMainFPSCapacity() const { return m_commitedMainFPSCapacity; }
	const SizeI& getCommitedMainWindowSize() const { return m_commitedMainWindowSize; }
	const SizeI& getCommitedMainBackbufferSize() const { return m_commitedMainBackbufferSize; }

private:
	List< std::shared_ptr<Group> >			m_groups;
	float					m_mainFPS;
	float					m_mainFPSCapacity;
	SizeI					m_mainWindowSize;
	SizeI					m_mainBackbufferSize;

	List<CommitedGroup>	m_commitedGroups;
	float					m_commitedMainFPS;
	float					m_commitedMainFPSCapacity;
	SizeI					m_commitedMainWindowSize;
	SizeI					m_commitedMainBackbufferSize;

	Mutex					m_commitMutex;
	bool					m_enabled;
};

class ScopedProfilerSection
{
public:
	ScopedProfilerSection(int group, int section)
		: m_group(group)
		, m_section(section)
	{
		Profiler::Instance.startSection(m_group, m_section);
	}

	~ScopedProfilerSection()
	{
		Profiler::Instance.endSection(m_group, m_section);
	}

private:
	int	m_group;
	int m_section;
};


LN_NAMESPACE_END
