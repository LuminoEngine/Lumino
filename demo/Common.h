
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
	static DemoMainFunc RegisterDemo(const char* group, const char* name, DemoMainFunc func);
	static Array<DemoInfo>	m_demos;
};

#define DemoMain(group, name, title) \
void DemoMain_##group_##name(); \
static DemoManager::DemoMainFunc _localDemoMain_##group_##name = DemoManager::RegisterDemo(#group, #name, DemoMain_##group_##name); \
void DemoMain_##group_##name()

