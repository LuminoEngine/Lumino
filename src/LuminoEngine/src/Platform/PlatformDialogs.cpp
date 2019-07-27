
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformDialogs.hpp>
#ifdef LN_OS_WIN32
#include "Win32PlatformDialogs.hpp"
#endif

namespace ln {


//==============================================================================
// PlatformFileOpenDialog

Ref<PlatformFileOpenDialog> PlatformFileOpenDialog::create()
{
#ifdef LN_OS_WIN32
    return makeObject<Win32PlatformFileOpenDialog>(Win32PlatformFileOpenDialog::Type::Open);
#else
    LN_NOTIMPLEMENTED();
    return nullptr;
#endif
}

PlatformFileOpenDialog::PlatformFileOpenDialog()
{
}

PlatformFileOpenDialog::~PlatformFileOpenDialog()
{
}

} // namespace ln
