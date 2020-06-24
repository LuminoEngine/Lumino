
#pragma once
#include "Common.hpp"
#include "../Engine/Object.hpp"

namespace ln {

/**
 * アセットファイルやその他の外部ファイルをインポートして構築されるオブジェクトのベースクラスです。
 */
LN_CLASS()
class AssetObject
    : public Object
{
	LN_OBJECT;
public:

protected:

LN_CONSTRUCT_ACCESS:
    AssetObject();
    bool init();

private:
};

} // namespace ln
