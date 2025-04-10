#pragma once
#include "../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang.h"
#include "../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang-com-ptr.h"

namespace ln {
namespace kokage {

class SlangFileSystem : public ISlangFileSystem {
public:
    SLANG_NO_THROW SlangResult SLANG_MCALL loadFile(char const* path, ISlangBlob** outBlob) override;

    // Implemetes slang-com-object.h
    virtual ~SlangFileSystem() {}
    SlangFileSystem()
        : m_refCount(0) {}

    // ISlangUnknown
    SLANG_NO_THROW SlangResult SLANG_MCALL queryInterface(SlangUUID const& uuid, void** outObject) SLANG_OVERRIDE {
        return SLANG_E_NO_INTERFACE;
    }
    SLANG_NO_THROW uint32_t SLANG_MCALL addRef() SLANG_OVERRIDE { return ++m_refCount; }
    SLANG_NO_THROW uint32_t SLANG_MCALL release() SLANG_OVERRIDE { return _releaseImpl(); }
    // ISlangCastable
    SLANG_NO_THROW void* SLANG_MCALL castAs(const SlangUUID& guid) override;

private:
    uint32_t _releaseImpl();
    std::atomic<uint32_t> m_refCount;
};

} // namespace kokage
} // namespace ln
