/*
 *	new に失敗した時は必ず OutOfMemoryException 例外を発生させます。
 *	戻り値を NULL チェックする必要はありません。
 *	使い方は普通の new と同じです。
 *	例)
 *		A* a = LN_NEW A(1, 2);
 */

#pragma once

LN_NAMESPACE_BEGIN

enum MemoryFlag
{
	LN_MM_NORMAL_BLOCK = 1,
};

LN_NAMESPACE_END

//void* LN_CDECL operator new ( size_t size );
//void  LN_CDECL operator delete ( void* ptr );

void* LN_CDECL operator new (size_t size, ::ln::MemoryFlag flag);
void* LN_CDECL operator new[](size_t size, ::ln::MemoryFlag flag);
void  LN_CDECL operator delete (void* ptr, ::ln::MemoryFlag flag);
void  LN_CDECL operator delete[](void* ptr, ::ln::MemoryFlag flag);

#ifdef LN_OS_WIN32
#include <crtdbg.h>	// TODO
#define LN_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)
#else
#define LN_NEW						new(::ln::LN_MM_NORMAL_BLOCK)
#endif

#define LN_OPERATOR_NEW(size)		::operator new(size,::ln:: LN_MM_NORMAL_BLOCK)
#define LN_OPERATOR_DELETE(ptr) {	::operator delete(ptr); (ptr) = 0; }
