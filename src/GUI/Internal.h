#pragma once
#include "../Internal.h"

namespace Lumino
{
class GUIManagerImpl;

/// グローバルな UIManager を取得する。
/// .h の include を増やしたくないので別関数として用意した。
GUIManagerImpl* GetUIManager();

} // namespace Lumino
