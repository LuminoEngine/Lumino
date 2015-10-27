#pragma once
#include "../Internal.h"

LN_NAMESPACE_BEGIN
class GUIManagerImpl;

/// グローバルな UIManager を取得する。
/// .h の include を増やしたくないので別関数として用意した。
GUIManagerImpl* GetUIManager();

LN_NAMESPACE_END
