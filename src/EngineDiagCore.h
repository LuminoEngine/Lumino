
#pragma once

LN_NAMESPACE_BEGIN

class EngineDiagCore
{
public:
	static EngineDiagCore	Instance;		// グローバルインスタンス

	void Initialize(EngineManager* manager);

	float GetMainFPS() const;
	float GetMainFPSCapacity() const;
	
private:
	EngineDiagCore();
	~EngineDiagCore();

	EngineManager*	m_manager;
};

LN_NAMESPACE_END
