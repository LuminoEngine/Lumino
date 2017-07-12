
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

	void reportCapability(int indent, const char* name, const char* value);

	template<typename... TArgs>
	void reportCapability(int indent, const char* name, const char* format, const TArgs&... args)
	{
		StringA str = StringA::Format(format, args...);
		reportCapability(indent, name, str.c_str());
	}

	float getMainFPS() const;
	float getMainFPSCapacity() const;

	// Graphics
	void resetGraphicsFrameReport() { m_graphicsDeviceDrawCount = 0; }
	void increaseGraphicsDeviceDrawCount() { ++m_graphicsDeviceDrawCount; }
	int getGraphicsDeviceDrawCount() const { return m_graphicsDeviceDrawCount; }

	// SceneGraph
	void increaseVisualNodeDrawCount() { ++m_visualNodeDrawCount; }
	void resetVisualNodeDrawCount() { m_visualNodeDrawCount = 0; }
	int getVisualNodeDrawCount() const { return m_visualNodeDrawCount; }


	SectionId startSection(uint32_t threadId, const StringRef& sectionName);
	void endSection(SectionId sectionId);
	
private:
	EngineDiagCore();
	~EngineDiagCore();

	EngineManager*	m_manager;
	int				m_graphicsDeviceDrawCount;
	int				m_visualNodeDrawCount;
};

LN_NAMESPACE_END
