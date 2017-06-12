
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

	void initialize(EngineManager* manager);

	void ReportCapability(int indent, const char* name, const char* value);

	template<typename... TArgs>
	void ReportCapability(int indent, const char* name, const char* format, const TArgs&... args)
	{
		StringA str = StringA::Format(format, args...);
		ReportCapability(indent, name, str.c_str());
	}

	float GetMainFPS() const;
	float GetMainFPSCapacity() const;

	// Graphics
	void ResetGraphicsFrameReport() { m_graphicsDeviceDrawCount = 0; }
	void IncreaseGraphicsDeviceDrawCount() { ++m_graphicsDeviceDrawCount; }
	int GetGraphicsDeviceDrawCount() const { return m_graphicsDeviceDrawCount; }

	// SceneGraph
	void IncreaseVisualNodeDrawCount() { ++m_visualNodeDrawCount; }
	void ResetVisualNodeDrawCount() { m_visualNodeDrawCount = 0; }
	int GetVisualNodeDrawCount() const { return m_visualNodeDrawCount; }


	SectionId StartSection(uint32_t threadId, const StringRef& sectionName);
	void EndSection(SectionId sectionId);
	
private:
	EngineDiagCore();
	~EngineDiagCore();

	EngineManager*	m_manager;
	int				m_graphicsDeviceDrawCount;
	int				m_visualNodeDrawCount;
};

LN_NAMESPACE_END
