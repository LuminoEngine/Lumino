/*
 * 従来の、LangBind に公開するコールバックが Event だけのときは、LeafWrapper が Native の Event をラップするような
 * 仕組みで実装していたので、std::function で足りていた。
 *
 * Event のように、LangBind に公開する NativeClass が直接のメンバ変数としてインスタンスを持つ場合は std::function でよかったのだが、
 * Promise コールバックのように Event での実装が適切ではない場合（LN_EVENT の Connection を return するルールなど）
 * Managed 側のオブジェクトの参照を持つために Object として std::function を実装する必要がある。
 * 
 */
#include "Internal.hpp"
#include <LuminoEngine/Base/Delegate.hpp>


namespace ln {

} // namespace ln

