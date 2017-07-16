
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
PlatformFileOpenDialogPtr PlatformFileOpenDialog::create()
{
#ifdef LN_OS_WIN32
	auto ptr = Ref<Win32PlatformFileOpenDialog>::makeRef();
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
