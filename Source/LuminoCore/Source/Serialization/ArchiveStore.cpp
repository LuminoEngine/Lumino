
#include "../Internal.h"
#include <Lumino/Serialization/ArchiveStore.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Archive2
//==============================================================================
// save...
// サブクラスは作成直後は root を作る必要は無い。
// 最初はかならず writeObject か writeArray が呼ばれる。
//
// load...
// 最初にルート要素を準備しておく。
// アーカイブバージョンのチェックのため、ルート要素に対して find がかけられる。

LN_NAMESPACE_END
