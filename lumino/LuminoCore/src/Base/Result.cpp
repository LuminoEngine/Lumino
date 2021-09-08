
#include "Internal.hpp"
#include <LuminoCore/Base/Result.hpp>

namespace ln {

const Result Result::Fail = Result(ResultCode::Fail);
const Result Result::Success = Result(ResultCode::Success);

} // namespace ln
