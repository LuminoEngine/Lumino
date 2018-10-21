

#ifdef _WIN32

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


#endif // _WIN32

// TODO: dummy
//https://blogs.msdn.microsoft.com/oldnewthing/20140321-00/?p=1433

class DummyApplication : public ln::Application
{
public:
	DummyApplication()
		: ln::Application()
	{
	}
};

void LN_API LNEngine_Initialize()
{
	DummyApplication dummyApp;
	ln::detail::ApplicationHelper::initialize(&dummyApp);
}

void LN_API LNEngine_Finalize2()
{
	DummyApplication dummyApp;
	ln::detail::ApplicationHelper::initialize(&dummyApp);
}
