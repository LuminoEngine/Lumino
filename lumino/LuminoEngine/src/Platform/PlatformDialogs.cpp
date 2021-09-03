
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformDialogs.hpp>
#ifdef LN_OS_WIN32
#include "Windows/Win32PlatformDialogs.hpp"
#endif

namespace ln {

//==============================================================================
// PlatformOpenFileDialog

Ref<PlatformOpenFileDialog> PlatformOpenFileDialog::create()
{
#ifdef LN_OS_WIN32
    return makeObject<Win32PlatformOpenFileDialog>();
#else
    LN_NOTIMPLEMENTED();
    return nullptr;
#endif
}

PlatformOpenFileDialog::~PlatformOpenFileDialog()
{
}

//==============================================================================
// PlatformSelectFolderDialog

Ref<PlatformSelectFolderDialog> PlatformSelectFolderDialog::create()
{
#ifdef LN_OS_WIN32
    return makeObject<Win32PlatformSelectFolderDialog>();
#else
    LN_NOTIMPLEMENTED();
    return nullptr;
#endif
}

PlatformSelectFolderDialog::~PlatformSelectFolderDialog()
{
}

} // namespace ln
