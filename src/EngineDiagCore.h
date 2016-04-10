
#pragma once

LN_NAMESPACE_BEGIN

class EngineDiagCore
{
public:
	typedef int SectionId;

	struct Section
	{
		String		name;			// セクション名
		uint64_t	elapsedTime;	// 経過時間 (ns)
	};

	// グループはスレッドごと
	struct SectionGroup
	{
		String							name;		// グループ名
		SortedArray<String, Section>	sections;
		ElapsedTimer					timer;
		//float							limitElapsedTime;	// 許容時間 (ns 単位。フレームレートの逆数)
	};

public:
	static EngineDiagCore	Instance;		// グローバルインスタンス

	void Initialize(EngineManager* manager);

	float GetMainFPS() const;
	float GetMainFPSCapacity() const;


	SectionId StartSection(uint32_t threadId, const StringRef& sectionName);
	void EndSection(SectionId sectionId);
	
private:
	EngineDiagCore();
	~EngineDiagCore();

	EngineManager*	m_manager;
};

LN_NAMESPACE_END
