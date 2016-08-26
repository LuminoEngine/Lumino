
#include "Internal.h"
#include <Lumino/Platform/PlatformFileOpenDialog.h>
#ifdef LN_OS_WIN32
#include "Win32/Win32PlatformFileOpenDialog.h"
#endif

LN_NAMESPACE_BEGIN
namespace tr
{

//==============================================================================
// PlatformFileOpenDialog
//==============================================================================

//------------------------------------------------------------------------------
PlatformFileOpenDialogPtr PlatformFileOpenDialog::Create()
{
#ifdef LN_OS_WIN32
	auto ptr = RefPtr<Win32PlatformFileOpenDialog>::MakeRef();
	return ptr;
#endif
}

//------------------------------------------------------------------------------
PlatformFileOpenDialog::PlatformFileOpenDialog()
{
}

//------------------------------------------------------------------------------
PlatformFileOpenDialog::~PlatformFileOpenDialog()
{
}

} // namespace tr
LN_NAMESPACE_END
