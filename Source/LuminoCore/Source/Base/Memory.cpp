
#include <new>
#include "../Internal.h"
#include "../../include/Lumino/Base/Memory.h"
using namespace ln;

//------------------------------------------------------------------------------
void* LN_CDECL operator new ( size_t size, MemoryFlag flag )
{
	void* ptr = NULL;
	try
	{
		ptr = ::operator new(size);
	}
	catch (std::bad_alloc)
	{
	}
	if (ptr == NULL) {
		LN_THROW(0, OutOfMemoryException);
	}
	return ptr;
}

//------------------------------------------------------------------------------
void* LN_CDECL operator new[] ( size_t size, MemoryFlag flag )
{
	void* ptr = NULL;
	try
	{
		ptr = ::operator new[](size);
	}
	catch (std::bad_alloc)
	{
	}
	if (ptr == NULL) {
		LN_THROW(0, OutOfMemoryException);
	}
	return ptr;
}

//------------------------------------------------------------------------------
void  LN_CDECL operator delete ( void* ptr, MemoryFlag flag )
{
	return ::operator delete(ptr);
}

//------------------------------------------------------------------------------
void  LN_CDECL operator delete[] ( void* ptr, MemoryFlag flag )
{
	return ::operator delete[](ptr);
}

