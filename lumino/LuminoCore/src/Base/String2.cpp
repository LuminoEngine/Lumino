
#include "Internal.hpp"
#include <limits.h>
#include <iostream>
#include <memory>
#if defined(LN_OS_WIN32) && _MSC_VER >= 1900
#include <cuchar>
#endif
#include <LuminoCore/Base/Buffer.hpp>
#include <LuminoCore/Base/String2.hpp>
#include <LuminoCore/Base/StringHelper.hpp>
#include <LuminoCore/Base/RefObject.hpp>
#include <LuminoCore/Base/CRCHash.hpp>
#include <LuminoCore/Text/Encoding.hpp>

namespace ln {


//==============================================================================
// String


String2::String2() {

}

} // namespace ln
