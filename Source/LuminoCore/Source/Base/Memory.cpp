
#include <new>
#include "../Internal.h"
#include "../../Include/Lumino/Base/Memory.h"
using namespace ln;

static ln::detail::NewCallback g_NewCallback = nullptr;
static ln::detail::DeleteCallback g_DeleteCallback = nullptr;

LN_NAMESPACE_BEGIN
namespace detail {

void setNewCallback(NewCallback callback)
{
	g_NewCallback = callback;
}
void setDeleteCallback(DeleteCallback callback)
{
	g_DeleteCallback = callback;
}

} // namespace detail
LN_NAMESPACE_END

//------------------------------------------------------------------------------
void* LN_CDECL operator new ( size_t size, MemoryFlag flag )
{
	void* ptr = NULL;
	try
	{
		ptr = ::operator new(size);
		if (g_NewCallback) g_NewCallback(ptr, size);
	}
	catch (std::bad_alloc)
	{
	}
	LN_FATAL(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
void* LN_CDECL operator new[] ( size_t size, MemoryFlag flag )
{
	void* ptr = NULL;
	try
	{
		ptr = ::operator new[](size);
		if (g_NewCallback) g_NewCallback(ptr, size);
	}
	catch (std::bad_alloc)
	{
	}
	LN_FATAL(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
void  LN_CDECL operator delete ( void* ptr, MemoryFlag flag )
{
	::operator delete(ptr);
	if (g_DeleteCallback) g_DeleteCallback(ptr);
}

//------------------------------------------------------------------------------
void  LN_CDECL operator delete[] ( void* ptr, MemoryFlag flag )
{
	::operator delete[](ptr);
	if (g_DeleteCallback) g_DeleteCallback(ptr);
}

