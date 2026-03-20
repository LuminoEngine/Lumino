
#include "Internal.hpp"
#include <LuminoCore/Json/JsonDiagnostics.hpp>

namespace ln {

JsonDiag::JsonDiag()
    : code(JsonDiagCode::None)
    , line(0)
    , column(0)
    , message()
{
}

} // namespace ln
