
#include "Internal.h"
#include "Resource.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// InternalResource
//==============================================================================
const String InternalResource::DirectXNotInstalledError(_T("DirectXNotInstalledError"));

void InternalResource::InitializeEngineResource()
{
	setString(DirectXNotInstalledError, _T("%s が見つかりませんでした。\nDirectX エンドユーザーランタイム をインストールしてください。\nhttps://www.microsoft.com/ja-jp/download/details.aspx?id=35&"));
}

LN_NAMESPACE_END
