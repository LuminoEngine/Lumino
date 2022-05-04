
#include "Internal.hpp"
#include <LuminoCore/Base/Environment.hpp>
#include <LuminoCore/IO/FileSystem.hpp>
#include "../IO/PathHelper.hpp"

#if defined(LN_OS_WIN32)
#include <Shlobj.h>
#include "Environment_Win32.hpp"
#elif defined(LN_OS_MAC)
#include "Environment_Mac.hpp"
#else
#include "Environment_Linux.hpp"
#endif

namespace ln {

Path Environment::currentDirectory() {
    PlatformEnvironment::StringType path;
    PlatformEnvironment::getCurrentDirectory(&path);
    return String::fromStdString(path);
}

void Environment::setCurrentDirectory(const Path& path) {
    PlatformEnvironment::setCurrentDirectory(path);
}

Path Environment::executablePath() {
    return String::fromStdString(PlatformEnvironment::getExecutablePath());
}

Path Environment::executableDirectory() {
    return executablePath().parent();
}

CaseSensitivity Environment::pathCaseSensitivity() {
#ifdef LN_OS_WIN32
    return CaseSensitivity::CaseInsensitive;
#else
    return CaseSensitivity::CaseSensitive;
#endif
}

Path Environment::specialFolderPath(SpecialFolder specialFolder) {
    PlatformEnvironment::StringType path;
    PlatformEnvironment::getSpecialFolderPath(specialFolder, &path);
    return String::fromStdString(path);
}

Path Environment::specialFolderPath(SpecialFolder specialFolder, const StringView& relativeDirPath, SpecialFolderOption option) {
    if (!relativeDirPath.isEmpty()) {
        if (LN_REQUIRE(!detail::PathTraits::isAbsolutePath(relativeDirPath.data(), relativeDirPath.length()))) return Path();
    }

    Path path2(Environment::specialFolderPath(specialFolder));
    if (!relativeDirPath.isEmpty()) {
        path2.append(relativeDirPath);
    }

    switch (option) {
        case SpecialFolderOption::None:
            if (ln::FileSystem::existsDirectory(path2))
                return path2;
            else
                return Path();

        case SpecialFolderOption::Create:
            if (!ln::FileSystem::existsDirectory(path2))
                ln::FileSystem::createDirectory(path2);
            return path2;

        case SpecialFolderOption::DoNotVerify:
            return path2;

        default:
            LN_UNREACHABLE();
            return Path();
    }
}

Optional<String> Environment::getEnvironmentVariable(const StringView& variableName) {
    const char* value = ::getenv(variableName.toStdString().c_str());
    if (value)
        return String::fromCString(value);
    else
        return std::nullopt;
}

void Environment::setEnvironmentVariable(const StringView& variableName, const StringView& value) {
    PlatformEnvironment::setEnvironmentVariable(variableName, value);
}

ByteOrder Environment::byteOrder() {
    const unsigned short x = 0x0001;
    if ((*(unsigned char*)&x) != 0)
        return ByteOrder::LittleEndian;
    else
        return ByteOrder::BigEndian;
}

uint64_t Environment::getTickCount() {
    return PlatformEnvironment::getTickCount();
}

bool Environment::isRuntimePlatform(RuntimePlatform value) {
#if defined(_WIN32)
    if (value == RuntimePlatform::Windows) {
        return true;
    }
    else {
        return false;
    }
#elif defined(__EMSCRIPTEN__)
    if (value == RuntimePlatform::Web) {
        return true;
    }
    else {
        return false;
    }
#else
#error "Not implemented."
#endif
}

} // namespace ln
