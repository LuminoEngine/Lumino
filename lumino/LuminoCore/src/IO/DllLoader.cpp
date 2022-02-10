
#include "Internal.hpp"
#ifdef _WIN32
#else
#include <dlfcn.h> // Ubuntu
#endif
#include <LuminoCore/IO/DllLoader.hpp>
#include <LuminoCore/IO/Path.hpp>

namespace ln {
namespace detail {

//==============================================================================
// DllLoader

static void* LoadDLL(const Char* filePath) {
#ifdef _WIN32
    detail::GenericStaticallyLocalPath<TCHAR> localPath(filePath, StringHelper::strlen(filePath));
    return ::LoadLibrary(localPath.c_str());
#else
    detail::GenericStaticallyLocalPath<char> localPath(filePath, StringHelper::strlen(filePath));
    return dlopen(localPath.c_str(), RTLD_LAZY);
#endif
}

static void UnloadDLL(void* module) {
#ifdef _WIN32
    ::FreeLibrary((HMODULE)module);
#else
    dlclose(module);
#endif
}

static void* GetProcAddr(void* module, const char* procName) {
#ifdef _WIN32
    return ::GetProcAddress((HMODULE)module, procName);
#else
    return dlsym(module, procName);
#endif
}

Ref<DllLoader> DllLoader::load(const StringView& filePath) {
    auto ptr = Ref<DllLoader>(LN_NEW DllLoader(), false);
    if (!ptr->init(filePath)) return nullptr;
    return ptr;
}

DllLoader::DllLoader()
    : m_module(nullptr) {
}

bool DllLoader::init(const StringView& filePath) {
    m_module = LoadDLL(String(filePath).c_str());
    if (LN_ENSURE(m_module)) return false;

    return true;
}

DllLoader::~DllLoader() {
    unload();
}

bool DllLoader::exists(const Char* filePath) {
    void* module = LoadDLL(filePath);
    if (module) {
        UnloadDLL(module);
        return true;
    }
    return false;
}

void DllLoader::unload() {
    if (m_module) {
        UnloadDLL(m_module);
        m_module = nullptr;
    }
}

void* DllLoader::getProcAddress(const char* procName) {
    if (LN_REQUIRE(m_module)) return nullptr;
    void* proc = GetProcAddr(m_module, procName);
    if (LN_ENSURE(proc)) return nullptr;
    return proc;
}

} // namespace detail
} // namespace ln
