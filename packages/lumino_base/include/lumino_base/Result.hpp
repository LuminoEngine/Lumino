#pragma once

#include <tl/expected.hpp>
#include <string>

namespace ln {

/// Error code for Lumino operations.
enum class ErrorCode {
    OK = 0,
    Unknown,
    InvalidArgument,
    OutOfMemory,
    IOError,
    NotSupported,
    NotInitialized,
    AlreadyInitialized,
    DeviceLost,
    ShaderCompilationFailed,
    RuntimeError,
};

/// Lightweight error type carrying a code and optional message.
struct Error {
    ErrorCode code = ErrorCode::Unknown;
    std::string message;

    Error() = default;
    Error(ErrorCode c) : code(c) {}
    Error(ErrorCode c, std::string msg) : code(c), message(std::move(msg)) {}
};

/// Result type: either a value T or an Error.
template <typename T>
using Result = tl::expected<T, Error>;

/// Result type for void-returning operations.
using VoidResult = tl::expected<void, Error>;

// clang-format off
#define LN_MAKE_SUCCESS() {}

} // namespace ln
