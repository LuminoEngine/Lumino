#pragma once

#include <tl/expected.hpp>
#include <string>

namespace ln {

/** Lumino の操作に使用するエラーコード。 */
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

/** コードとオプションのメッセージを持つ軽量エラー型。 */
struct Error {
    ErrorCode code = ErrorCode::Unknown;
    std::string message;

    Error() = default;
    Error(ErrorCode c) : code(c) {}
    Error(ErrorCode c, std::string msg) : code(c), message(std::move(msg)) {}
};

/** 結果型。値 T またはエラー Error のいずれかを持つ。 */
template <typename T>
using Result = tl::expected<T, Error>;

/** void を返す操作用の結果型。 */
using VoidResult = tl::expected<void, Error>;

// clang-format off
#define LN_MAKE_SUCCESS() {}

#define LN_FORWARD_ERROR(otherResult) tl::make_unexpected(otherResult.error())

// ErrorCode: RuntimeError
#define LN_MAKE_ERROR(...) ::ln::detail::makeInternalError(::ln::detail::formatString(__VA_ARGS__), __FILE__, __func__, __LINE__);

// 指定した ErrorCode を持つエラーを生成する (例: ErrorCode::DeviceLost)
#define LN_MAKE_ERROR_WITH_CODE(code, ...) ::ln::detail::makeInternalErrorWithCode(code, ::ln::detail::formatString(__VA_ARGS__), __FILE__, __func__, __LINE__);

namespace detail {
inline std::string formatString() { return {}; }
std::string formatString(const char* format, ...);
tl::unexpected<Error> makeInternalError(const std::string& message, const char* file, const char* function, int line);
tl::unexpected<Error> makeInternalErrorWithCode(ErrorCode code, const std::string& message, const char* file, const char* function, int line);
} // namespace detail
} // namespace ln
