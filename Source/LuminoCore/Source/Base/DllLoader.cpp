
#include "../Internal.h"
#ifdef _WIN32
	#include <windows.h>
#else
	#include <dlfcn.h>	// Ubuntu
#endif
#include <Lumino/Base/DllLoader.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// DllLoader
//==============================================================================

static void* LoadDLL(const TCHAR* filePath)
{
#ifdef _WIN32
	return ::LoadLibrary(filePath);
#else
#ifdef LN_UNICODE
	LN_LOCAL_MBCS_FILEPATH(mbcsPath, filePath);
	return dlopen(mbcsPath, RTLD_LAZY);
#else
	return dlopen(filePath, RTLD_LAZY);
#endif
#endif
}

static void UnloadDLL(void* module)
{
#ifdef _WIN32
	::FreeLibrary((HMODULE)module);
#else
	dlclose(module);
#endif
}

static void* GetProcAddr(void* module, const char* procName)
{
#ifdef _WIN32
	return ::GetProcAddress((HMODULE)module, procName);
#else
	return dlsym(module, procName);
#endif
}

//------------------------------------------------------------------------------
DllLoader::DllLoader()
	: m_module(NULL)
{
}

//------------------------------------------------------------------------------
DllLoader::DllLoader(const TCHAR* filePath)
	: m_module(NULL)
{
	Load(filePath);
}

//------------------------------------------------------------------------------
DllLoader::~DllLoader()
{
	Unload();
}

//------------------------------------------------------------------------------
bool DllLoader::Exists(const TCHAR* filePath)
{
	// ロードできるかでチェック
	void* module = LoadDLL(filePath);
	if (module != NULL)
	{
		UnloadDLL(module);
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void DllLoader::Load(const TCHAR* filePath)
{
	Unload();
	m_module = LoadDLL(filePath);
	LN_THROW(m_module != NULL, FileNotFoundException);
}

//------------------------------------------------------------------------------
bool DllLoader::TryLoad(const TCHAR* filePath)
{
	Unload();
	m_module = LoadDLL(filePath);
	return m_module != NULL;
}

//------------------------------------------------------------------------------
void DllLoader::Unload()
{
	if (m_module != NULL)
	{
		UnloadDLL(m_module);
		m_module = NULL;
	}
}

//------------------------------------------------------------------------------
void* DllLoader::GetProcAddress(const char* procName)
{
	LN_THROW(m_module != NULL, InvalidOperationException);
	void* proc = GetProcAddr(m_module, procName);
	LN_THROW(proc != NULL, InvalidOperationException);
	return proc;
}

LN_NAMESPACE_END
