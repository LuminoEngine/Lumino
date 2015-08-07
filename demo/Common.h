
#pragma once

#include <Lumino.h>
using namespace Lumino;

class DemoManager
{
public:
	typedef void(*DemoMainFunc)();

	struct DemoInfo
	{
		String			Group;
		String			Name;
		DemoMainFunc	Main;
	};

public:
	static DemoMainFunc RegisterDemo(const char* name, DemoMainFunc func);
	static Array<DemoInfo>	m_demos;
};

#define DemoMain(name) \
void DemoMain_##name(); \
static DemoManager::DemoMainFunc _localDemoMain_##name = DemoManager::RegisterDemo(#name, DemoMain_##name); \
void DemoMain_##name()

