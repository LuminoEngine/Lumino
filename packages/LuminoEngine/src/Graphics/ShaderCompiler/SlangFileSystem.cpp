#include "Internal.hpp"
#include "SlangFileSystem.hpp"

namespace ln {
namespace kokage {

SLANG_NO_THROW SlangResult SLANG_MCALL SlangFileSystem::loadFile(char const* path, ISlangBlob** outBlob)
{
    return SLANG_OK;
}

SLANG_NO_THROW void* SLANG_MCALL SlangFileSystem::castAs(const SlangUUID& guid) {
    return this;
}

uint32_t SlangFileSystem::_releaseImpl() {
    LN_ASSERT(m_refCount != 0);
    const uint32_t count = --m_refCount;
    if (count == 0) {
        delete this;
    }
    return count;
}

} // namespace kokage
} // namespace ln
