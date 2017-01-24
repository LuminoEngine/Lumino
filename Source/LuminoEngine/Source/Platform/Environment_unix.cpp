
#include "../Internal.h"
#include "../../include/Lumino/Platform/Environment.h"

namespace Lumino
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint64_t Environment::GetTickCount()
{
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
uint64_t Environment::GetTickCountNS()
{
	return 0;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
template<>
const char* Environment::GetNewLine<char>()
{
	return "\n";
}
template<>
const wchar_t* Environment::GetNewLine<wchar_t>()
{
	return L"\n";
}

} // namespace Lumino
