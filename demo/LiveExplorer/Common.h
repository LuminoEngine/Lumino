
#pragma once
#include <LuminoEngine.h>
#include <Lumino/Testing/TestUtils.h>
using namespace ln;

#define LOCALFILE(fileName) TestUtils::GetFilePath(__FILE__, fileName).c_str()

class DemoManager
{
public:
	typedef void(*DemoMainFunc)();

	struct DemoInfo
	{
		String			Group1;
		String			Group2;
		String			Caption;
		DemoMainFunc	Main;
	};

public:
	static DemoMainFunc RegisterDemo(const char* name, DemoMainFunc func);
	static List<DemoInfo>	m_demos;
	static std::map<String, std::map<String, List<DemoInfo> > >	m_demosTable;
};

#define DemoMain(name, desc) \
void DemoMain_##name(); \
static DemoManager::DemoMainFunc _localDemoMain_##name = DemoManager::RegisterDemo(desc, DemoMain_##name); \
void DemoMain_##name()

