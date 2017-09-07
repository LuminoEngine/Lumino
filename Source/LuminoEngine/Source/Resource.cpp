
#include "Internal.h"
#include "Resource.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// InternalResource
//==============================================================================
const String InternalResource::DirectXNotInstalledError(_LT("DirectXNotInstalledError"));

void InternalResource::initializeEngineResource()
{
	setString(DirectXNotInstalledError, _LT("%s が見つかりませんでした。\nDirectX エンドユーザーランタイム をインストールしてください。\nhttps://www.microsoft.com/ja-jp/download/details.aspx?id=35&"));
}

LN_NAMESPACE_END
