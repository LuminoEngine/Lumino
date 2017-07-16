
#pragma once
#include <memory>
#include <Lumino/Base/List.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/ElapsedTimer.h>
#include <Lumino/Threading/Mutex.h>

LN_NAMESPACE_BEGIN
class Profiler2;

class ProfilingKey
{
public:
	ProfilingKey(const StringRef& name);
	ProfilingKey(ProfilingKey* parent, const StringRef& name);

	//uint32_t GetHashCode() const { return m_keyHash; }

	const String& getName() const { return m_name; }
	const List<ProfilingKey*>& getChildren() const { return m_children; }

private:
	String				m_name;
	//uint32_t			m_keyHash;
	ProfilingKey*		m_parent;
	List<ProfilingKey*>	m_children;
};

class ProfilingKeys
{
public:
	static ProfilingKey* Engine;
	static ProfilingKey* Engine_UpdateFrame;
	static ProfilingKey* Engine_RenderFrame;
	static ProfilingKey* Engine_PresentFrame;

	static ProfilingKey* Rendering;
	static ProfilingKey* Rendering_PresentDevice;
};

class ProfilingSection
{
public:
	ProfilingSection();
	void begin();
	void end();

	float getElapsedSeconds() const { return static_cast<float>(m_committedTime) * 0.000000001f; }
	uint64_t getElapsedNanoseconds() const { return m_committedTime; }

private:
	void commitFrame(int frameCount);

	ElapsedTimer			m_timer;
	std::atomic<uint64_t>	m_totalTime;
	ConditionFlag			m_measuring;

	int						m_committedFrameCount;
	uint64_t				m_committedTime;

	friend class Profiler2;
};


class Profiler2
{
public:
	static void initializeGlobalSections();
	static void finalizeGlobalSections();

	static void registerProfilingSection(const ProfilingKey* key);

	static ProfilingSection* getSection(const ProfilingKey* key);

	static float GetMainFps();
	static float GetMainFpsCapacity();

	static void commitFrame();
};


// RAII で計測範囲を制御する
class ScopedProfilingSection2
{
public:
	ScopedProfilingSection2(const ProfilingKey* key)
	{
		m_section = Profiler2::getSection(key);
		if (m_section != nullptr)
		{
			m_section->begin();
		}
	}

	~ScopedProfilingSection2()
	{
		if (m_section != nullptr)
		{
			m_section->end();
		}
	}

private:
	ProfilingSection*	m_section;
};



/**
	@brief
*/
// [obsolete]
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
